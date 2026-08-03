/*
 * logger.h
 *
 *  Created on: Apr 5, 2025
 *      Author: tanmay
 */

#pragma once

#include <array>

#include "task_manager/task_base.h"
#include "pubsub/subscriber.h"
#include "messages/imu_data.h"
#include "messages/baro_data.h"
#include "messages/rc_channels.h"
#include "messages/gps_data.h"
#include "messages/ekf_data.h"
#include "messages/fcs_debug_data.h"
#include "messages/pwm_data.h"
#include "messages/mtf01p_data.h"
#include "messages/magnetometer_data.h"
#if RTOS_METRICS_LOGGING_ENABLE
#include "messages/rtos_metrics_data.h"
#endif
#include "parameters/parameter_change_event.h"
#include "parameter_catalog.h"
#include "debug.h"
#include "data_buffer.h"

#include <cstring>

class Logger : public TaskBase {
 public:
  explicit Logger();
  void Run() override;

 private:
  static constexpr size_t kMaxLogConfigs = static_cast<size_t>(TopicID::MAX_TOPICS);
  static constexpr uint8_t kHeaderByte = 0xA5;
  static constexpr uint16_t INTERVAL_MS = 2; // 500Hz, fastest logging rate

  // Scratch space for framing one record before it is buffered.
  static constexpr size_t kScratchBufferSize = 512;
  uint8_t scratch_buffer_[kScratchBufferSize];

  struct LogConfigBase {
    virtual void TryLog(uint32_t now_ticks, Logger* logger) = 0;
    virtual ~LogConfigBase() {}
  };

  template<typename T>
  class LogConfig : public LogConfigBase {
   public:
    LogConfig(TopicID topic_id, uint32_t interval_ticks)
        : subscriber_(topic_id),
          topic_id_(topic_id),
          interval_ticks_(interval_ticks),
          last_log_tick_(0) {}

    void TryLog(uint32_t now_ticks, Logger* logger) override {
      if ((now_ticks - last_log_tick_) < interval_ticks_) return;

      T data;
      if (!subscriber_.copy(data)) return;

      static_assert((1U + 1U + sizeof(T) + 2U) <= kScratchBufferSize,
                    "record does not fit in the scratch buffer");
      static_cast<void>(logger->WriteFramedRecord(
          topic_id_, &data, sizeof(T)));
      last_log_tick_ = now_ticks;
    }

   private:
    Subscriber<T> subscriber_;
    TopicID topic_id_;
    uint32_t interval_ticks_;
    uint32_t last_log_tick_;
  };

  LogConfigBase* log_configs_[kMaxLogConfigs];
  size_t log_config_count_ = 0;

  void SetupTopics();

  // Registers a topic to be logged every interval_ms milliseconds.
  template <typename T>
  void AddLogConfig(TopicID topic_id, uint32_t interval_ms) {
    if (log_config_count_ < kMaxLogConfigs) {
      log_configs_[log_config_count_++] =
          new LogConfig<T>(topic_id, pdMS_TO_TICKS(interval_ms));
    }
  }

  // Appends one framed record to the active DataBuffer, handing full
  // buffers to the SD task.
  bool WriteBuffered(const uint8_t* data, size_t len,
                     bool count_drop = true);
  bool WriteFramedRecord(TopicID topic_id, const void* payload,
                         size_t payload_size, bool count_drop = true);
  void ServiceOneParameterRecord();
#if RTOS_METRICS_LOGGING_ENABLE
  void ServiceOneRtosMetricsRecord(uint32_t now_ticks);
#endif

  // CRC-16/CCITT-FALSE over the record header and payload.
  static uint16_t ComputeCrc16(const uint8_t* data, size_t length);

  uint32_t parameter_session_epoch_ = 0U;
  uint32_t parameter_event_session_epoch_ = 0U;
  uint32_t parameter_session_sequence_cutoff_ = 0U;
  uint32_t next_expected_parameter_sequence_ = 1U;
  uint16_t parameter_snapshot_cursor_ = 0U;
  uint64_t parameter_session_start_ms_ = 0U;
  std::array<parameters::ParameterValue,
             parameters::generated::kParameterCount>
      parameter_snapshot_configured_{};
  std::array<parameters::ParameterValue,
             parameters::generated::kParameterCount>
      parameter_snapshot_active_{};
  std::array<uint32_t, parameters::generated::kParameterCount>
      parameter_snapshot_state_revision_{};
  parameters::ParameterChangeEvent pending_parameter_event_{};
  bool has_pending_parameter_event_ = false;
  parameters::ParameterChangeEvent pending_parameter_gap_{};
  bool has_pending_parameter_gap_ = false;
  uint32_t trailing_gap_candidate_ = 0U;
  bool has_trailing_gap_candidate_ = false;

#if RTOS_METRICS_LOGGING_ENABLE
  size_t rtos_metrics_cursor_ = 0U;
  uint32_t rtos_metrics_snapshot_sequence_ = 0U;
  uint32_t rtos_metrics_last_tick_ = 0U;
  uint32_t rtos_metrics_last_context_switch_count_ = 0U;
  bool rtos_metrics_schedule_initialized_ = false;
#endif
#if RTOS_STACK_WATERMARK_METRICS_ENABLE
  // One low-rate, nonblocking state observation prevents variable-length
  // stack fill scans while the vehicle is armed or in flight.
  Subscriber<FcsDebugData> rtos_stack_state_subscriber_{TopicID::FCSDEBUG};
  FcsDebugData rtos_stack_state_{};
  bool rtos_stack_state_known_ = false;
#endif
};
