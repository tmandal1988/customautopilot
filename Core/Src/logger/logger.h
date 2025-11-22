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
#include "messages/mavlink_params_data.h"
#include "messages/mtf01p_data.h"
#include "debug.h"
#include "data_buffer.h"

#include "fatfs.h"
#include <cstring>

class Logger : public TaskBase {
 public:
  explicit Logger();
  void Run() override;

 private:
  static constexpr size_t kMaxLogConfigs = static_cast<size_t>(TopicID::MAX_TOPICS);
  static constexpr uint8_t kHeaderByte = 0xA5;
//  static constexpr size_t kBufferSize = 65536;
//  static constexpr size_t kNumBuffers = 2;
  static constexpr uint16_t INTERVAL_MS = 2; // 500Hz, fastest logging rate

  static constexpr size_t kScratchBufferSize = 512;
  uint8_t scratch_buffer_[kScratchBufferSize];

  FIL* file_;
  // Double buffering: two buffers with individual offsets.
//  uint8_t buffers_[kNumBuffers][kBufferSize];
//  size_t buffer_offsets_[kNumBuffers] = {0, 0};
//  uint8_t current_buffer_index_ = 0;

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

      // Print once per eligible interval
//      DEBUG_PRINT("[TryLog] Topic ID %d eligible at tick %lu\n", static_cast<int>(topic_id_), now_ticks);

      T data;
      if (!subscriber_.copy(data)) return;
//      if constexpr (std::is_same<T, EkfData>::value) {
//    	  DEBUG_PRINT("[Logger] EKF sizeof: %u, is_gps_valid: %u, timestamp: %g\n",
//    	                  static_cast<unsigned>(sizeof(data)),
//    	                  static_cast<unsigned>(data.is_gps_valid),
//    	                  data.timestamp_us * 0.000001f);
//
//      }

      const size_t payload_size = sizeof(T);
      const size_t total_size = 1 + 1 + payload_size + 2;
      // Use the preallocated scratch buffer.
      uint8_t* temp = logger->GetScratchBuffer();

      temp[0] = kHeaderByte;
      temp[1] = static_cast<uint8_t>(topic_id_);
      std::memcpy(temp + 2, &data, payload_size);

      uint16_t crc = ComputeCrc16(temp, 2 + payload_size);
      std::memcpy(temp + 2 + payload_size, &crc, sizeof(crc));

      logger->WriteBuffered(temp, total_size);
      last_log_tick_ = now_ticks;
    }

   private:
    Subscriber<T> subscriber_;
    TopicID topic_id_;
    uint32_t interval_ticks_;
    uint32_t last_log_tick_;

    static uint16_t ComputeCrc16(const uint8_t* data, size_t length);
  };

  LogConfigBase* log_configs_[kMaxLogConfigs];
  size_t log_config_count_ = 0;

  void SetupTopics();
  void WriteBuffered(const uint8_t* data, size_t len);

  uint8_t* GetScratchBuffer() { return scratch_buffer_; }
};
