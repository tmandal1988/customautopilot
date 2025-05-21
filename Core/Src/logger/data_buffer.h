/*
 * data_buffer.h
 *
 *  Created on: Apr 11, 2025
 *      Author: tanmay
 */

#pragma once

#include "FreeRTOS.h"
#include "semphr.h"

namespace DataBuffer {

constexpr size_t kBufferSize = 65536; // 64KB per buffer
constexpr size_t kNumBuffers = 2;     // Double buffering

// The actual buffers
extern uint8_t buffers_[kNumBuffers][kBufferSize];

// Bytes filled in each buffer
extern size_t buffer_offsets_[kNumBuffers];

// Currently active buffer for logging
extern uint8_t current_buffer_index_;

// Mutex for thread safety (optional but good practice)
extern SemaphoreHandle_t mutex_;

//A flag to indicate that a buffer is full
extern bool buffer_full_;

extern uint8_t flush_buffer_index_;
extern uint8_t current_buffer_index_ ;

// Initialize the buffer system (call once in main/init)
void DataBuffersAccessMutexInit();
}  // namespace DataBuffer

