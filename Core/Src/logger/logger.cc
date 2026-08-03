/*
 * logger.cc
 *
 *  Created on: Apr 5, 2025
 *      Author: tanmay
 */

#include "logger.h"

#include <limits>

#include "parameters/parameter_store.h"
#include "pubsub/topic.h"
#if RTOS_STACK_WATERMARK_METRICS_ENABLE
#include "fcsModel_types.h"
#endif

namespace {

constexpr bool SequenceAfter(uint32_t candidate, uint32_t reference) {
  return static_cast<int32_t>(candidate - reference) > 0;
}

constexpr bool SequenceAtOrAfter(uint32_t candidate, uint32_t reference) {
  return (candidate == reference) || SequenceAfter(candidate, reference);
}

static_assert(SequenceAfter(1U, 0U));
static_assert(SequenceAfter(0U, UINT32_MAX));
static_assert(!SequenceAfter(UINT32_MAX, 0U));

parameters::ParameterChangeEvent MakeGapEvent(
    uint32_t first_missing, uint32_t last_missing,
    uint32_t session_epoch) {
  parameters::ParameterChangeEvent gap{};
  static constexpr char kGapName[] = "PARAM_LOG_GAP";
  static_assert(sizeof(kGapName) <= sizeof(gap.name));
  std::memcpy(gap.name, kGapName, sizeof(kGapName));
  gap.timestamp_ms = getCurrentTimeMs();
  // For EventsDropped records these fields are sequence metadata rather than
  // parameter values: [previous, value] is the missing inclusive range and
  // active_value_bits is the modulo-32-bit missing-event count.
  gap.previous_value_bits = first_missing;
  gap.value_bits = last_missing;
  gap.active_value_bits = last_missing - first_missing + 1U;
  gap.sequence = last_missing;
  gap.session_epoch = session_epoch;
  gap.catalog_index = std::numeric_limits<uint16_t>::max();
  gap.type = parameters::ParameterValueType::Int32;
  gap.kind = parameters::ParameterChangeKind::EventsDropped;
  return gap;
}

}  // namespace

Logger::Logger()
    // Must outrank SdWriteTask: the logger samples latest-value topics on a
    // deadline, while the SD task has seconds of multi-buffer slack.
    : TaskBase("LoggerTask", 4096, osPriorityNormal) {
}

void Logger::SetupTopics() {
  // Add more safely up to TopicID::MAX_TOPICS.
  AddLogConfig<ImuData>(TopicID::ICM20948, 4);
  AddLogConfig<BaroData>(TopicID::BMP390L, 16);
  AddLogConfig<RcChannels>(TopicID::RCCHANNELS, 20);
  AddLogConfig<GpsData>(TopicID::UBLOXM9N, 20);
  AddLogConfig<EkfData>(TopicID::EKF, 8);
  AddLogConfig<FcsDebugData>(TopicID::FCSDEBUG, 8);
  AddLogConfig<PwmData>(TopicID::PWM, 20);
  AddLogConfig<Mtf01pData>(TopicID::MTF01P, 10);
  AddLogConfig<MagnetometerData>(TopicID::IST8310, 10);
}

void Logger::Run() {
  SetupTopics();

  const TickType_t interval_ticks = pdMS_TO_TICKS(INTERVAL_MS);
  osDelay(1000);
  TickType_t last_wake_time = xTaskGetTickCount();
  ConfigurePeriodicMetrics(INTERVAL_MS * 1000U, INTERVAL_MS * 1000U);

  while (true) {
    BeginMetricsCycle();
    const uint32_t now_ticks = xTaskGetTickCount();
    // STOP freezes the ordinary-record boundary before it closes the
    // parameter application epoch. Parameter tail records may then drain
    // without flight data being written under an unrepresented new value.
    if (DataBuffer::parameter_stop_requested_.load(
            std::memory_order_acquire) == 0U) {
      for (size_t i = 0; i < log_config_count_; ++i) {
        log_configs_[i]->TryLog(now_ticks, this);
      }
#if RTOS_METRICS_LOGGING_ENABLE
      ServiceOneRtosMetricsRecord(now_ticks);
#endif
    }
    // At most one exact change/gap and one paced snapshot row per cycle. This
    // is a fixed SPSC pop, never a catalog-draining burst.
    ServiceOneParameterRecord();

    // Wait until the next cycle.
    EndMetricsCycle();
    vTaskDelayUntil(&last_wake_time, interval_ticks);
  }
}

#if RTOS_METRICS_LOGGING_ENABLE
void Logger::ServiceOneRtosMetricsRecord(uint32_t now_ticks) {
  constexpr uint32_t kReportIntervalTicks = pdMS_TO_TICKS(1000U);

  if (!rtos_metrics_schedule_initialized_) {
    rtos_metrics_schedule_initialized_ = true;
    rtos_metrics_last_tick_ = now_ticks;
    rtos_metrics_last_context_switch_count_ =
        rtos_metrics::ContextSwitchCount();
    return;
  }

  const uint32_t elapsed_ticks = now_ticks - rtos_metrics_last_tick_;
  if (elapsed_ticks < kReportIntervalTicks) {
    return;
  }

  // Schedule from "now" so logger stalls never cause a catch-up burst.
  rtos_metrics_last_tick_ = now_ticks;
  const uint32_t context_switch_count =
      rtos_metrics::ContextSwitchCount();
  const uint32_t context_switch_delta =
      context_switch_count - rtos_metrics_last_context_switch_count_;
  rtos_metrics_last_context_switch_count_ = context_switch_count;

  auto& tasks = TaskBase::GetTaskList();
  if (tasks.empty()) {
    return;
  }
  if (rtos_metrics_cursor_ >= tasks.size()) {
    rtos_metrics_cursor_ = 0U;
  }

  constexpr size_t kMaxPersistedTaskCount = UINT8_MAX;
  const size_t bounded_task_count =
      (tasks.size() < kMaxPersistedTaskCount)
          ? tasks.size()
          : kMaxPersistedTaskCount;
  if (rtos_metrics_cursor_ >= bounded_task_count) {
    rtos_metrics_cursor_ = 0U;
  }

  RtosMetricsData record{};
#if RTOS_STACK_WATERMARK_METRICS_ENABLE
  if (rtos_stack_state_subscriber_.copy(rtos_stack_state_)) {
    rtos_stack_state_known_ = true;
  }
  // The FreeRTOS high-water API performs a variable-length fill-pattern scan.
  // Its result is lifetime-minimum state, so waiting until INACTIVE retains
  // the complete flight's peak use without doing this work during flight.
  if (rtos_stack_state_known_ &&
      (static_cast<enumStateMachine>(rtos_stack_state_.sm_mode) ==
       enumStateMachine::INACTIVE)) {
    tasks[rtos_metrics_cursor_]->RefreshStackHighWaterMark();
  }
#endif
  tasks[rtos_metrics_cursor_]->CaptureMetrics(
      static_cast<uint8_t>(rtos_metrics_cursor_),
      static_cast<uint8_t>(bounded_task_count),
      ++rtos_metrics_snapshot_sequence_, context_switch_count,
      context_switch_delta, elapsed_ticks * portTICK_PERIOD_MS, &record);
  record.timestamp_ms = getCurrentTimeMs();
  static_assert((1U + 1U + sizeof(record) + sizeof(uint16_t)) <=
                kScratchBufferSize);
  static_cast<void>(WriteFramedRecord(TopicID::RTOS_METRICS, &record,
                                      sizeof(record), false));

  ++rtos_metrics_cursor_;
}
#endif

void Logger::ServiceOneParameterRecord() {
  const uint32_t state_epoch =
      DataBuffer::logging_state_epoch_.load(std::memory_order_acquire);
  auto& store = parameters::ParameterStore::Instance();

  if ((state_epoch & 1U) == 0U) {
    // No file exists for these events. Drain one per cycle so pre-session
    // tuning cannot consume capacity needed in flight; the next session starts
    // with an explicit configured/active snapshot.
    has_pending_parameter_event_ = false;
    has_pending_parameter_gap_ = false;
    has_trailing_gap_candidate_ = false;
    parameters::ParameterChangeEvent ignored{};
    static_cast<void>(store.PopChangeEvent(&ignored));
    return;
  }

  if (state_epoch != parameter_session_epoch_) {
    parameter_session_epoch_ = state_epoch;
    parameter_snapshot_cursor_ = 0U;
    parameter_session_start_ms_ = getCurrentTimeMs();
    has_pending_parameter_event_ = false;
    has_pending_parameter_gap_ = false;
    has_trailing_gap_candidate_ = false;

    // SdUtils opened parameter admission before it enabled ordinary file
    // writes. The logging-state acquire above makes this session ID visible.
    parameter_event_session_epoch_ =
        DataBuffer::parameter_log_session_epoch_.load(
            std::memory_order_relaxed);
    parameter_session_sequence_cutoff_ =
        DataBuffer::parameter_log_start_sequence_.load(
            std::memory_order_relaxed);
    next_expected_parameter_sequence_ =
        parameter_session_sequence_cutoff_ + 1U;

    // Necessary one-time session capture in the non-control Logger task. The
    // paced writes below use this fixed image; each row's seqlock revision
    // states exactly which concurrent update it already incorporates.
    for (uint16_t index = 0U; index < store.Count(); ++index) {
      const bool captured = store.ReadSnapshotValues(
          index, &parameter_snapshot_configured_[index],
          &parameter_snapshot_active_[index],
          &parameter_snapshot_state_revision_[index]);
      configASSERT(captured);
      if (!captured) {
        parameter_snapshot_configured_[index] = {};
        parameter_snapshot_active_[index] = {};
        parameter_snapshot_state_revision_[index] = 0U;
      }
    }
  }

  const uint32_t stop_phase =
      DataBuffer::parameter_stop_requested_.load(
          std::memory_order_acquire);
  const bool cutoff_ready = stop_phase == 2U;
  const uint32_t stop_cutoff =
      DataBuffer::parameter_stop_cutoff_sequence_.load(
          std::memory_order_relaxed);

  // Retry a gap marker before the event that follows it. This prevents a later
  // successful event from being serialized ahead of an earlier queue loss.
  if (has_pending_parameter_gap_) {
    if (!WriteFramedRecord(TopicID::PARAMETER_UPDATE,
                           &pending_parameter_gap_,
                           sizeof(pending_parameter_gap_), false)) {
      return;
    }
    next_expected_parameter_sequence_ =
        pending_parameter_gap_.value_bits + 1U;
    has_pending_parameter_gap_ = false;
    return;
  }

  // Pop at most one live event per cycle. The exact START sequence discards
  // events already assigned before the baseline. An application from the
  // preceding epoch whose completion is assigned later is intentionally
  // logged as well; its per-value revision makes baseline overlap explicit.
  bool queue_was_empty = false;
  if (!has_pending_parameter_event_) {
    has_pending_parameter_event_ =
        store.PopChangeEvent(&pending_parameter_event_);
    queue_was_empty = !has_pending_parameter_event_;
    if (has_pending_parameter_event_) {
      has_trailing_gap_candidate_ = false;
    }
  }
  if (has_pending_parameter_event_ &&
      (!SequenceAfter(pending_parameter_event_.sequence,
                      parameter_session_sequence_cutoff_))) {
    has_pending_parameter_event_ = false;
  }

  if (has_pending_parameter_event_ && cutoff_ready &&
      SequenceAfter(pending_parameter_event_.sequence, stop_cutoff)) {
    // Queue order proves no successful event at or below the finite cutoff can
    // remain behind this one. Close a failed included tail before discarding
    // the post-cutoff event; otherwise continuous later updates could keep
    // STOP waiting forever on the missing final sequence.
    if (!SequenceAfter(next_expected_parameter_sequence_, stop_cutoff)) {
      pending_parameter_gap_ = MakeGapEvent(
          next_expected_parameter_sequence_, stop_cutoff,
          parameter_event_session_epoch_);
      has_pending_parameter_gap_ = true;
      if (!WriteFramedRecord(TopicID::PARAMETER_UPDATE,
                             &pending_parameter_gap_,
                             sizeof(pending_parameter_gap_), false)) {
        return;
      }
      next_expected_parameter_sequence_ = stop_cutoff + 1U;
      has_pending_parameter_gap_ = false;
    }
    has_pending_parameter_event_ = false;
  }

  if (has_pending_parameter_event_) {
    const uint32_t event_sequence = pending_parameter_event_.sequence;
    if (SequenceAfter(next_expected_parameter_sequence_, event_sequence)) {
      // A duplicate or stale event cannot move the monotonic cursor backward.
      has_pending_parameter_event_ = false;
    } else if (SequenceAfter(event_sequence,
                             next_expected_parameter_sequence_)) {
      uint32_t gap_last = event_sequence - 1U;
      if (cutoff_ready && SequenceAfter(gap_last, stop_cutoff)) {
        gap_last = stop_cutoff;
      }
      pending_parameter_gap_ = MakeGapEvent(
          next_expected_parameter_sequence_, gap_last,
          parameter_event_session_epoch_);
      has_pending_parameter_gap_ = true;
      if (!WriteFramedRecord(TopicID::PARAMETER_UPDATE,
                             &pending_parameter_gap_,
                             sizeof(pending_parameter_gap_), false)) {
        return;
      }
      next_expected_parameter_sequence_ = gap_last + 1U;
      has_pending_parameter_gap_ = false;
      return;
    }

    if (has_pending_parameter_event_) {
      if (!WriteFramedRecord(TopicID::PARAMETER_UPDATE,
                             &pending_parameter_event_,
                             sizeof(pending_parameter_event_), false)) {
        return;
      }
      next_expected_parameter_sequence_ = event_sequence + 1U;
      has_pending_parameter_event_ = false;
    }
  }

  // If the producer has reported a failed tail enqueue and no surviving later
  // event exists yet, close that gap now. A successful enqueue never updates
  // LatestFailedChangeSequence(), so a producer/consumer race cannot create a
  // false gap for an event that is merely waiting in the SPSC ring.
  if (queue_was_empty) {
    uint32_t latest_failed = store.LatestFailedChangeSequence();
    if (cutoff_ready && SequenceAfter(latest_failed, stop_cutoff)) {
      latest_failed = stop_cutoff;
    }
    if (SequenceAtOrAfter(latest_failed,
                          next_expected_parameter_sequence_)) {
      // Require two consecutive empty observations before declaring a tail
      // gap. This closes the bounded race where the producer enqueues a
      // successful event immediately after the first empty probe.
      if (has_trailing_gap_candidate_ &&
          (trailing_gap_candidate_ == latest_failed)) {
        pending_parameter_gap_ = MakeGapEvent(
            next_expected_parameter_sequence_, latest_failed,
            parameter_event_session_epoch_);
        has_pending_parameter_gap_ = true;
        if (!WriteFramedRecord(TopicID::PARAMETER_UPDATE,
                               &pending_parameter_gap_,
                               sizeof(pending_parameter_gap_), false)) {
          return;
        }
        next_expected_parameter_sequence_ = latest_failed + 1U;
        has_pending_parameter_gap_ = false;
        has_trailing_gap_candidate_ = false;
      } else {
        trailing_gap_candidate_ = latest_failed;
        has_trailing_gap_candidate_ = true;
      }
    } else {
      has_trailing_gap_candidate_ = false;
    }
  }

  // Emit the fixed configured+active baseline at every logging-session start.
  // One row per 2 ms bounds each buffer operation. Live event service above
  // continues during the snapshot, so catalog growth cannot fill the event
  // queue merely because baseline serialization is in progress.
  if (parameter_snapshot_cursor_ < store.Count()) {
    parameters::ParameterChangeEvent snapshot{};
    if (store.MakeConfiguredSnapshotEvent(
            parameter_snapshot_cursor_, parameter_session_start_ms_,
            parameter_session_sequence_cutoff_,
            parameter_event_session_epoch_,
            parameter_snapshot_state_revision_[parameter_snapshot_cursor_],
            parameter_snapshot_configured_[parameter_snapshot_cursor_],
            parameter_snapshot_active_[parameter_snapshot_cursor_],
            &snapshot) &&
        WriteFramedRecord(TopicID::PARAMETER_UPDATE, &snapshot,
                          sizeof(snapshot), false)) {
      ++parameter_snapshot_cursor_;
    }
  }

  if (cutoff_ready &&
      (parameter_snapshot_cursor_ >= store.Count()) &&
      SequenceAfter(next_expected_parameter_sequence_, stop_cutoff) &&
      SequenceAtOrAfter(store.LatestRecordedChangeSequence(), stop_cutoff) &&
      !has_pending_parameter_event_ && !has_pending_parameter_gap_) {
    if (DataBuffer::parameter_stop_acknowledged_.exchange(
            1U, std::memory_order_acq_rel) == 0U) {
      if (DataBuffer::sd_task_handle_ != nullptr) {
        xTaskNotifyGive(DataBuffer::sd_task_handle_);
      }
    }
  }
}

bool Logger::WriteFramedRecord(TopicID topic_id, const void* payload,
                               size_t payload_size, bool count_drop) {
  const size_t total_size = 1U + 1U + payload_size + sizeof(uint16_t);
  if ((payload == nullptr) || (total_size > kScratchBufferSize)) {
    return false;
  }

  scratch_buffer_[0] = kHeaderByte;
  scratch_buffer_[1] = static_cast<uint8_t>(topic_id);
  std::memcpy(scratch_buffer_ + 2U, payload, payload_size);
  const uint16_t crc =
      ComputeCrc16(scratch_buffer_, 2U + payload_size);
  std::memcpy(scratch_buffer_ + 2U + payload_size, &crc, sizeof(crc));
  return WriteBuffered(scratch_buffer_, total_size, count_drop);
}

bool Logger::WriteBuffered(const uint8_t* data, size_t len,
                           bool count_drop) {
  namespace db = DataBuffer;

  // A record is never larger than one buffer.
  if ((data == nullptr) || (len > db::kBufferSize)) {
    return false;
  }

  bool notify_sd = false;
  bool buffered = false;

  xSemaphoreTake(db::mutex_, portMAX_DELAY);
  if (!db::logging_enabled_) {
    xSemaphoreGive(db::mutex_);
    return false;
  }

  uint8_t cur = db::current_buffer_index_;
  uint8_t next = (cur + 1) % db::kNumBuffers;

  // If the active buffer is full and already handed to the SD task, move on
  // to the next buffer if that one is free.
  if (db::buffer_pending_[cur] && !db::buffer_pending_[next]) {
    db::current_buffer_index_ = next;
    cur = next;
    next = (cur + 1) % db::kNumBuffers;
  }

  const size_t available = db::kBufferSize - db::buffer_offsets_[cur];

  if (db::buffer_pending_[cur] ||
      (len > available && db::buffer_pending_[next])) {
    // Every buffer the record would touch is still waiting on the SD card.
    // Drop the whole record rather than overwriting unflushed data.
    if (count_drop) {
      ++db::dropped_records_;
      db::dropped_bytes_ += len;
    }
  } else if (len <= available) {
    // The record fits in the active buffer.
    std::memcpy(db::buffers_[cur] + db::buffer_offsets_[cur], data, len);
    db::buffer_offsets_[cur] += len;
    buffered = true;

    // If this write fills the buffer, hand it to the SD task.
    if (db::buffer_offsets_[cur] == db::kBufferSize) {
      db::buffer_pending_[cur] = true;
      // Advance ownership immediately. If SD finishes this buffer before the
      // next logger call, leaving current_buffer_index_ on cur would let the
      // logger reuse cur out of ring order and the SD task could miss it.
      db::current_buffer_index_ = next;
      notify_sd = true;
    }
  } else {
    // The record spills over: fill the active buffer, hand it to the SD
    // task and put the remainder in the next (flushed, empty) buffer.
    // len <= kBufferSize and available >= 1 here, so the remainder can
    // never fill the other buffer.
    std::memcpy(db::buffers_[cur] + db::buffer_offsets_[cur], data, available);
    db::buffer_offsets_[cur] = db::kBufferSize;
    db::buffer_pending_[cur] = true;
    notify_sd = true;

    const size_t remaining = len - available;
    std::memcpy(db::buffers_[next], data + available, remaining);
    db::buffer_offsets_[next] = remaining;
    db::current_buffer_index_ = next;
    buffered = true;
  }
  xSemaphoreGive(db::mutex_);

  // Wake the SD task so a full buffer is flushed immediately.
  if (notify_sd && db::sd_task_handle_ != nullptr) {
    xTaskNotifyGive(db::sd_task_handle_);
  }
  return buffered;
}

uint16_t Logger::ComputeCrc16(const uint8_t* data, size_t length) {
  static const uint16_t table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
  };

  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < length; ++i) {
    uint8_t tbl_idx = (crc >> 8) ^ data[i];
    crc = (crc << 8) ^ table[tbl_idx];
  }
  return crc;
}

// Static instance for self-registration
Logger logger_task_instance_;
