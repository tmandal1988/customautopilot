/*
 * data_buffer.cc
 *
 *  Created on: Apr 11, 2025
 *      Author: tanmay
 */


#include "data_buffer.h"
//#include <cstring>

namespace DataBuffer {

uint8_t buffers_[kNumBuffers][kBufferSize];
size_t buffer_offsets_[kNumBuffers] = {0, 0};
uint8_t current_buffer_index = 0;
SemaphoreHandle_t mutex_ = nullptr;
bool buffer_full_ = false;
uint8_t flush_buffer_index_ = 0;
uint8_t current_buffer_index_ = 0;

void DataBuffersAccessMutexInit() {
  mutex_ = xSemaphoreCreateMutex();
  configASSERT(mutex_ != nullptr);
}
}  // namespace DataBuffer
