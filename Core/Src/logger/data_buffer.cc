/*
 * data_buffer.cc
 *
 *  Created on: Apr 11, 2025
 *      Author: tanmay
 */


#include "data_buffer.h"
//#include <cstring>

namespace DataBuffer {

alignas(32) uint8_t buffers_[kNumBuffers][kBufferSize];
size_t buffer_offsets_[kNumBuffers] = {};
SemaphoreHandle_t mutex_ = nullptr;
bool buffer_pending_[kNumBuffers] = {};
bool logging_enabled_ = false;
std::atomic<uint32_t> logging_state_epoch_{0U};
std::atomic<uint32_t> parameter_log_session_epoch_{0U};
std::atomic<uint32_t> parameter_log_start_sequence_{0U};
std::atomic<uint32_t> parameter_stop_cutoff_sequence_{0U};
std::atomic<uint32_t> parameter_stop_requested_{0U};
std::atomic<uint32_t> parameter_stop_acknowledged_{0U};
static_assert(std::atomic<uint32_t>::is_always_lock_free);
uint32_t dropped_records_ = 0;
uint32_t dropped_bytes_ = 0;
TaskHandle_t sd_task_handle_ = nullptr;
uint8_t current_buffer_index_ = 0;

void DataBuffersAccessMutexInit() {
  mutex_ = xSemaphoreCreateMutex();
  configASSERT(mutex_ != nullptr);
}
}  // namespace DataBuffer
