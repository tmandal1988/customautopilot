/*
 * sd_utils.cc
 *
 *  Created on: Apr 12, 2025
 *      Author: tanmay
 */

#include "sd_utils.h"

#include "data_buffer.h"
#include "logger_parameter_catalog.h"
#include "parameter_catalog.h"
#include "parameters/parameter_store.h"
#include "usb_console/usb_console.h"

SdUtils::SdUtils()
    // Lowest priority in the system: RAM buffering absorbs normal SD-card
    // latency while deadline-driven producer/logger tasks keep running.
    : TaskBase("SdWriteTask", 2304, osPriorityBelowNormal),
      sync_every_buffers_(static_cast<uint32_t>(
          parameters::generated::kLoggerParameterDefaults.log_sync_bufs)),
      idle_wait_ticks_(pdMS_TO_TICKS(static_cast<uint32_t>(
          parameters::generated::kLoggerParameterDefaults.log_idle_ms))) {
}

void SdUtils::LoadBootParameters() {
  auto& store = parameters::ParameterStore::Instance();
  std::int32_t configured_value = 0;

  // The generated defaults initialized the members above. A failed read is
  // therefore safe, while a successful read is already type/range validated.
  const bool sync_read = store.ReadInt32(
      parameters::generated::ParameterId::LogSyncBufs, &configured_value);
  configASSERT(sync_read);
  if (sync_read) {
    sync_every_buffers_ = static_cast<uint32_t>(configured_value);
  }

  const bool idle_read = store.ReadInt32(
      parameters::generated::ParameterId::LogIdleMs, &configured_value);
  configASSERT(idle_read);
  if (idle_read) {
    idle_wait_ticks_ =
        pdMS_TO_TICKS(static_cast<uint32_t>(configured_value));
  }
}

void SdUtils::SetLoggingEnabled(bool enabled) {
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  if (DataBuffer::logging_enabled_ != enabled) {
    if (enabled) {
      // Open parameter-event admission before ordinary records can enter the
      // new file session. Logger observes this value through the subsequent
      // release of logging_state_epoch_.
      const uint32_t parameter_session =
          parameters::ParameterStore::Instance().BeginChangeLogSession();
      DataBuffer::parameter_log_session_epoch_.store(
          parameter_session, std::memory_order_relaxed);
      DataBuffer::parameter_log_start_sequence_.store(
          parameters::ParameterStore::Instance()
              .LatestAssignedChangeSequence(),
          std::memory_order_relaxed);
      DataBuffer::parameter_stop_cutoff_sequence_.store(
          0U, std::memory_order_relaxed);
      DataBuffer::parameter_stop_requested_.store(
          0U, std::memory_order_relaxed);
      DataBuffer::parameter_stop_acknowledged_.store(
          0U, std::memory_order_relaxed);
    } else {
      // Error shutdowns cannot guarantee a log tail because the storage path
      // itself failed. Controlled STOP uses the explicit handshake instead.
      DataBuffer::parameter_stop_requested_.store(
          0U, std::memory_order_relaxed);
      DataBuffer::parameter_stop_acknowledged_.store(
          0U, std::memory_order_relaxed);
    }
    DataBuffer::logging_enabled_ = enabled;
    DataBuffer::logging_state_epoch_.fetch_add(1U,
                                                std::memory_order_release);
  }
  xSemaphoreGive(DataBuffer::mutex_);
}

void SdUtils::ResetBufferState() {
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  if (DataBuffer::logging_enabled_) {
    DataBuffer::logging_enabled_ = false;
    DataBuffer::logging_state_epoch_.fetch_add(1U,
                                                std::memory_order_release);
  }
  DataBuffer::current_buffer_index_ = 0;
  for (size_t i = 0; i < DataBuffer::kNumBuffers; ++i) {
    DataBuffer::buffer_offsets_[i] = 0;
    DataBuffer::buffer_pending_[i] = false;
  }
  DataBuffer::parameter_stop_requested_.store(0U,
                                               std::memory_order_relaxed);
  DataBuffer::parameter_stop_acknowledged_.store(
      0U, std::memory_order_relaxed);
  xSemaphoreGive(DataBuffer::mutex_);
  next_flush_index_ = 0;
}

void SdUtils::ResetTelemetry() {
  write_count_ = 0;
  sync_count_ = 0;
  write_error_count_ = 0;
  sync_error_count_ = 0;
  close_error_count_ = 0;
  partial_write_count_ = 0;
  append_padding_bytes_ = 0;
  max_write_ticks_ = 0;
  max_sync_ticks_ = 0;
  buffers_since_sync_ = 0;

  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  DataBuffer::dropped_records_ = 0;
  DataBuffer::dropped_bytes_ = 0;
  xSemaphoreGive(DataBuffer::mutex_);
}

void SdUtils::PrintTelemetry() {
  uint32_t dropped_records;
  uint32_t dropped_bytes;
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  dropped_records = DataBuffer::dropped_records_;
  dropped_bytes = DataBuffer::dropped_bytes_;
  xSemaphoreGive(DataBuffer::mutex_);

  DEBUG_PRINT(
      "SD log stats: writes=%lu, syncs=%lu, write_errors=%lu, "
      "partial_writes=%lu, sync_errors=%lu, close_errors=%lu, "
      "max_write_ticks=%lu, max_sync_ticks=%lu, dropped_records=%lu, "
      "dropped_bytes=%lu, append_padding_bytes=%lu\r\n",
      static_cast<unsigned long>(write_count_),
      static_cast<unsigned long>(sync_count_),
      static_cast<unsigned long>(write_error_count_),
      static_cast<unsigned long>(partial_write_count_),
      static_cast<unsigned long>(sync_error_count_),
      static_cast<unsigned long>(close_error_count_),
      static_cast<unsigned long>(max_write_ticks_),
      static_cast<unsigned long>(max_sync_ticks_),
      static_cast<unsigned long>(dropped_records),
      static_cast<unsigned long>(dropped_bytes),
      static_cast<unsigned long>(append_padding_bytes_));
}

bool SdUtils::IsBufferPending(uint8_t buffer_index) {
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  const bool pending = DataBuffer::buffer_pending_[buffer_index];
  xSemaphoreGive(DataBuffer::mutex_);
  return pending;
}

bool SdUtils::SyncFile() {
  if (!file_open_ || file_ == nullptr) {
    ++sync_error_count_;
    ERROR_PRINT("Cannot sync: no log file is open\r\n");
    return false;
  }

  const TickType_t start_ticks = xTaskGetTickCount();
  const FRESULT result = f_sync(file_);
  const TickType_t elapsed_ticks = xTaskGetTickCount() - start_ticks;
  ++sync_count_;
  if (elapsed_ticks > max_sync_ticks_) {
    max_sync_ticks_ = elapsed_ticks;
  }

  if (result != FR_OK) {
    ++sync_error_count_;
    SetLoggingEnabled(false);
    ERROR_PRINT("SD f_sync failed (res=%d)\r\n", result);
    return false;
  }

  buffers_since_sync_ = 0;
  return true;
}

bool SdUtils::AlignFileForAppend() {
  const FSIZE_t file_position = f_tell(file_);
  const UINT padding_size = static_cast<UINT>(
      (kSdSectorSize - (file_position % kSdSectorSize)) % kSdSectorSize);
  if (padding_size == 0) {
    return true;
  }

  // Padding is inserted only between STOP/START logging sessions. The binary
  // decoder already resynchronizes on the 0xA5 frame header, so zero bytes are
  // skipped. Starting actual records at a sector boundary keeps subsequent
  // 64 KiB writes on the SD driver's fast, aligned multi-block DMA path.
  alignas(32) static uint8_t zero_padding[kSdSectorSize] = {};
  UINT bytes_written = 0;
  const TickType_t start_ticks = xTaskGetTickCount();
  const FRESULT result =
      f_write(file_, zero_padding, padding_size, &bytes_written);
  const TickType_t elapsed_ticks = xTaskGetTickCount() - start_ticks;
  ++write_count_;
  if (elapsed_ticks > max_write_ticks_) {
    max_write_ticks_ = elapsed_ticks;
  }

  if (result != FR_OK || bytes_written != padding_size) {
    ++write_error_count_;
    if (bytes_written > 0 && bytes_written != padding_size) {
      ++partial_write_count_;
    }
    SetLoggingEnabled(false);
    ERROR_PRINT(
        "Failed to sector-align append position (requested=%u, written=%u, "
        "res=%d)\r\n",
        static_cast<unsigned int>(padding_size),
        static_cast<unsigned int>(bytes_written), result);
    return false;
  }

  append_padding_bytes_ += padding_size;
  return true;
}

bool SdUtils::WritePendingBuffer(uint8_t buffer_index) {
  size_t bytes_to_write;
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  if (!DataBuffer::buffer_pending_[buffer_index]) {
    xSemaphoreGive(DataBuffer::mutex_);
    return true;
  }
  bytes_to_write = DataBuffer::buffer_offsets_[buffer_index];
  xSemaphoreGive(DataBuffer::mutex_);

  UINT bytes_written = 0;
  const TickType_t start_ticks = xTaskGetTickCount();
  const FRESULT result =
      f_write(file_, DataBuffer::buffers_[buffer_index],
              static_cast<UINT>(bytes_to_write), &bytes_written);
  const TickType_t elapsed_ticks = xTaskGetTickCount() - start_ticks;
  ++write_count_;
  if (elapsed_ticks > max_write_ticks_) {
    max_write_ticks_ = elapsed_ticks;
  }

  if (result != FR_OK || bytes_written != bytes_to_write) {
    ++write_error_count_;
    if (bytes_written > 0 && bytes_written != bytes_to_write) {
      ++partial_write_count_;
    }
    SetLoggingEnabled(false);
    ERROR_PRINT(
        "SD f_write failed (buffer=%u, requested=%lu, written=%u, res=%d)"
        "\r\n",
        static_cast<unsigned int>(buffer_index),
        static_cast<unsigned long>(bytes_to_write),
        static_cast<unsigned int>(bytes_written), result);
    // Do not clear this buffer. A retry is deliberately not automatic: a
    // short write may already have advanced the file pointer and replaying
    // the entire buffer would duplicate data.
    return false;
  }

  // f_write() has consumed the source bytes, so release this RAM buffer
  // before a potentially slow periodic f_sync().
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  DataBuffer::buffer_offsets_[buffer_index] = 0;
  DataBuffer::buffer_pending_[buffer_index] = false;
  xSemaphoreGive(DataBuffer::mutex_);

  ++buffers_since_sync_;
  return true;
}

bool SdUtils::CloseFile() {
  if (!file_open_) {
    return true;
  }

  const FRESULT result = f_close(file_);
  if (result != FR_OK) {
    ++close_error_count_;
    ERROR_PRINT("SD f_close failed (res=%d)\r\n", result);
    return false;
  }
  file_open_ = false;
  return true;
}

void SdUtils::RequestParameterLogStop() {
  DataBuffer::parameter_stop_acknowledged_.store(
      0U, std::memory_order_relaxed);
  // Freeze ordinary flight-record admission first. Owner applications then
  // acquire against the epoch exchange below: an old-epoch application is
  // drained into this file, while a new-epoch application remains completely
  // legal and is represented by the next session snapshot.
  DataBuffer::parameter_stop_requested_.store(
      1U, std::memory_order_release);
  auto& store = parameters::ParameterStore::Instance();
  const uint32_t closed_epoch = store.EndChangeLogSession();
  configASSERT(
      closed_epoch == DataBuffer::parameter_log_session_epoch_.load(
                          std::memory_order_relaxed));
}

void SdUtils::BeginStop() {
  // Disable logging and hand the active partial buffer to the SD task while
  // holding the same mutex used for record copies. Thus STOP can never cut a
  // record in half in RAM.
  xSemaphoreTake(DataBuffer::mutex_, portMAX_DELAY);
  if (DataBuffer::logging_enabled_) {
    DataBuffer::logging_enabled_ = false;
    DataBuffer::logging_state_epoch_.fetch_add(1U,
                                                std::memory_order_release);
  }
  const uint8_t active = DataBuffer::current_buffer_index_;
  if (DataBuffer::buffer_offsets_[active] > 0 &&
      !DataBuffer::buffer_pending_[active]) {
    DataBuffer::buffer_pending_[active] = true;
  }
  xSemaphoreGive(DataBuffer::mutex_);
}

bool SdUtils::SdInit() {
  file_ = &SDFile;

  FRESULT result = f_mount(&SDFatFS, (TCHAR const*)SDPath, 1);
  if (result != FR_OK) {
    ERROR_PRINT("SD Card mount failed (f_mount res=%d)\r\n", result);
    return false;
  }
  DEBUG_PRINT("SD Card mounted successfully\r\n");

  result = f_open(file_, file_name, FA_CREATE_ALWAYS | FA_WRITE);
  if (result != FR_OK) {
    ERROR_PRINT("Failed to open %s (res=%d)\r\n", file_name, result);
    return false;
  }

  file_open_ = true;
  DEBUG_PRINT("%s created\r\n", file_name);
  if (!SyncFile()) {
    CloseFile();
    return false;
  }
  return true;
}

void SdUtils::Run() {
  DataBuffer::sd_task_handle_ = xTaskGetCurrentTaskHandle();
  ConfigureEventMetrics();

  // ParameterStore::Initialize() and its Flash restore complete before the
  // scheduler starts. Reboot-only Logger settings are therefore stable here.
  LoadBootParameters();

  // main() creates the coordination mutex before any task starts.
  configASSERT(DataBuffer::mutex_ != nullptr);
  ResetBufferState();
  ResetTelemetry();

  const bool initialized = SdInit();
  SetLoggingEnabled(initialized);
  CardState state = initialized ? CardState::IDLEREADY : CardState::ERROR;
  UsbCommand usb_cmd{UsbCommand::NONE};

  while (true) {
    if (UsbConsole::Instance().HasNewCommand()) {
      DIR dir;
      FILINFO file_info;
      FRESULT result;
      usb_cmd = UsbConsole::Instance().GetLatestCommand();
      switch (usb_cmd) {
        case UsbCommand::STOP:
          if (state == CardState::ERROR) {
            ERROR_PRINT(
                "Logging is in an SD error state; reboot before stopping or "
                "restarting\r\n");
          } else if (file_open_ && state != CardState::DRAININGSTOP &&
                     state != CardState::WAITINGPARAMSTOP) {
            RequestParameterLogStop();
            state = CardState::WAITINGPARAMSTOP;
          }
          break;

        case UsbCommand::START:
          if (state == CardState::IDLESTOP && !file_open_) {
            state = CardState::IDLESTART;
          } else if (state == CardState::ERROR) {
            ERROR_PRINT(
                "Logging is in an SD error state; reboot before restarting"
                "\r\n");
          } else {
            DEBUG_PRINT("Log file is already open or still draining\r\n");
          }
          break;

        case UsbCommand::LIST:
          if (!file_open_) {
            result = f_opendir(&dir, "/");
            if (result != FR_OK) {
              DEBUG_PRINT("Failed to open root dir\r\n");
              break;
            }
            while (true) {
              result = f_readdir(&dir, &file_info);
              if (result != FR_OK || file_info.fname[0] == 0) {
                break;
              }
              if (!(file_info.fattrib & AM_DIR)) {
                DEBUG_PRINT("[FILE] %-20s %lu B\r\n", file_info.fname,
                            static_cast<unsigned long>(file_info.fsize));
              }
              osDelay(1);
            }
            f_closedir(&dir);
          } else {
            DEBUG_PRINT(
                "SD Card write in progress, issue a STOP command first\r\n");
          }
          break;

        case UsbCommand::COPY:
          if (!file_open_) {
            result = f_open(file_, file_name, FA_READ);
            if (result != FR_OK) {
              DEBUG_PRINT("Failed to open file: %s\r\n", file_name);
              break;
            }

            DEBUG_PRINT("== Contents of %s ==\r\n", file_name);
            UINT bytes_read;
            char buffer[128];
            while (true) {
              result =
                  f_read(file_, buffer, sizeof(buffer) - 1, &bytes_read);
              if (result != FR_OK || bytes_read == 0) {
                break;
              }
              buffer[bytes_read] = '\0';
              DEBUG_PRINT("%s", buffer);
              osDelay(1);
            }
            f_close(file_);
            DEBUG_PRINT("\r\n== End of file ==\r\n");
          } else {
            DEBUG_PRINT(
                "SD Card write in progress, issue a STOP command first\r\n");
          }
          break;

        default:
          break;
      }
    }

    // Measure one bounded SD state-machine dispatch. The USB LIST/COPY command
    // loops above are intentionally excluded because they are interactive,
    // post-flight operations that can exceed the 32-bit DWT wrap interval.
    auto metrics_scope = MeasureMetricsScope();
    switch (state) {
      case CardState::IDLESTART: {
        ResetBufferState();
        ResetTelemetry();
        const FRESULT result =
            f_open(file_, file_name, FA_OPEN_APPEND | FA_WRITE);
        if (result != FR_OK) {
          ERROR_PRINT("Failed to reopen %s (res=%d)\r\n", file_name,
                      result);
          state = CardState::ERROR;
          break;
        }

        file_open_ = true;
        if (!AlignFileForAppend()) {
          CloseFile();
          PrintTelemetry();
          state = CardState::ERROR;
          break;
        }
        if (!SyncFile()) {
          CloseFile();
          PrintTelemetry();
          state = CardState::ERROR;
          break;
        }
        SetLoggingEnabled(true);
        DEBUG_PRINT("Reopened the log file to append logs\r\n");
        state = CardState::IDLEREADY;
        break;
      }

      case CardState::IDLEREADY:
        if (IsBufferPending(next_flush_index_)) {
          state = CardState::WRITING;
          continue;
        }
        // Do not spend the available buffer headroom on f_sync while older
        // buffers are queued. Once writes catch up, a sync starts with no
        // pending buffer and the producer has maximum ring capacity available.
        if (buffers_since_sync_ >= sync_every_buffers_) {
          if (!SyncFile()) {
            CloseFile();
            PrintTelemetry();
            state = CardState::ERROR;
            break;
          }
          continue;
        }
        break;

      case CardState::WRITING:
        if (!WritePendingBuffer(next_flush_index_)) {
          SetLoggingEnabled(false);
          CloseFile();
          PrintTelemetry();
          state = CardState::ERROR;
          break;
        }
        next_flush_index_ =
            (next_flush_index_ + 1) % DataBuffer::kNumBuffers;
        state = CardState::IDLEREADY;
        continue;

      case CardState::WAITINGPARAMSTOP:
        // Keep releasing full buffers while Logger serializes the bounded
        // parameter tail. Logger notifies this task when it acknowledges the
        // cutoff, so the configurable idle timeout does not delay STOP.
        if (IsBufferPending(next_flush_index_)) {
          if (!WritePendingBuffer(next_flush_index_)) {
            SetLoggingEnabled(false);
            CloseFile();
            PrintTelemetry();
            state = CardState::ERROR;
            break;
          }
          next_flush_index_ =
              (next_flush_index_ + 1) % DataBuffer::kNumBuffers;
          continue;
        }
        if ((DataBuffer::parameter_stop_requested_.load(
                 std::memory_order_acquire) == 1U) &&
            (parameters::ParameterStore::Instance()
                 .PendingChangeRecordCount(
                     DataBuffer::parameter_log_session_epoch_.load(
                         std::memory_order_relaxed)) == 0U)) {
          // Every application classified into the closed epoch now has an
          // event or an explicit failed sequence. Later unrestricted updates
          // use the next epoch, so continuous PARAM_SET traffic cannot extend
          // this finite cutoff.
          const uint32_t cutoff =
              parameters::ParameterStore::Instance()
                  .LatestRecordedChangeSequence();
          DataBuffer::parameter_stop_cutoff_sequence_.store(
              cutoff, std::memory_order_relaxed);
          DataBuffer::parameter_stop_requested_.store(
              2U, std::memory_order_release);
        }
        if ((DataBuffer::parameter_stop_requested_.load(
                 std::memory_order_acquire) == 2U) &&
            (DataBuffer::parameter_stop_acknowledged_.load(
                 std::memory_order_acquire) != 0U)) {
          BeginStop();
          DataBuffer::parameter_stop_requested_.store(
              0U, std::memory_order_release);
          state = CardState::DRAININGSTOP;
          continue;
        }
        break;

      case CardState::DRAININGSTOP:
        if (IsBufferPending(next_flush_index_)) {
          if (!WritePendingBuffer(next_flush_index_)) {
            CloseFile();
            PrintTelemetry();
            state = CardState::ERROR;
            break;
          }
          next_flush_index_ =
              (next_flush_index_ + 1) % DataBuffer::kNumBuffers;
          continue;
        }

        // f_sync is unconditional at a controlled stop so both data and
        // filesystem metadata are committed before f_close returns.
        if (!SyncFile()) {
          CloseFile();
          PrintTelemetry();
          state = CardState::ERROR;
          break;
        }
        if (!CloseFile()) {
          PrintTelemetry();
          state = CardState::ERROR;
          break;
        }
        PrintTelemetry();
        ResetBufferState();
        DEBUG_PRINT("Closed file for logging\r\n");
        state = CardState::IDLESTOP;
        break;

      case CardState::IDLESTOP:
      case CardState::ERROR:
        break;
    }
    metrics_scope.Complete();

    // A full buffer wakes this task immediately; the timeout preserves USB
    // command polling while no buffer needs service.
    ulTaskNotifyTake(pdTRUE, idle_wait_ticks_);
  }
}

// Static instance for self-registration
SdUtils sdutils_task_instance_;
