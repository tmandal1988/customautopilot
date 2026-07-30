/*
 * logger.h
 *
 *  Created on: Apr 5, 2025
 *      Author: tanmay
 */

#pragma once

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

      // Frame the record in the shared scratch buffer:
      // [0xA5][topic id][payload][crc16 over header + payload]
      constexpr size_t payload_size = sizeof(T);
      constexpr size_t total_size = 1 + 1 + payload_size + 2;
      static_assert(total_size <= kScratchBufferSize,
                    "record does not fit in the scratch buffer");
      uint8_t* record = logger->GetScratchBuffer();

      record[0] = kHeaderByte;
      record[1] = static_cast<uint8_t>(topic_id_);
      std::memcpy(record + 2, &data, payload_size);

      const uint16_t crc = ComputeCrc16(record, 2 + payload_size);
      std::memcpy(record + 2 + payload_size, &crc, sizeof(crc));

      logger->WriteBuffered(record, total_size);
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
  void WriteBuffered(const uint8_t* data, size_t len);

  // CRC-16/CCITT-FALSE over the record header and payload.
  static uint16_t ComputeCrc16(const uint8_t* data, size_t length);

  uint8_t* GetScratchBuffer() { return scratch_buffer_; }
};
