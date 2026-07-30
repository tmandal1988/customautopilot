/*
 * data_buffer.h
 *
 *  Created on: Apr 11, 2025
 *      Author: tanmay
 */

#pragma once

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

namespace DataBuffer {

constexpr size_t kBufferSize = 65536; // 64KB per buffer
// Three buffers let one buffer be written while two more absorb roughly
// 1.3 seconds of data at the current configured log rate.
constexpr size_t kNumBuffers = 3;

// The SDMMC DMA path casts this storage to uint32_t*. A cache-line alignment
// also keeps the buffers safe if D-cache maintenance is enabled later.
alignas(32) extern uint8_t buffers_[kNumBuffers][kBufferSize];

// Bytes filled in each buffer
extern size_t buffer_offsets_[kNumBuffers];

// Currently active buffer for logging
extern uint8_t current_buffer_index_;

// Mutex guarding all transitions of the coordination state below
// (current_buffer_index_, buffer_offsets_, buffer_pending_). It makes the
// buffer ownership handoff between the logger and SD tasks atomic; buffer
// contents themselves are protected by that ownership, not by the mutex.
extern SemaphoreHandle_t mutex_;

// Per-buffer flag: buffer is full and waiting for the SD task to write it.
// All accesses are protected by mutex_.
extern bool buffer_pending_[kNumBuffers];

// False while the SD task is stopped, draining, or in an error state.
// The logger checks this under mutex_ before modifying a buffer.
extern bool logging_enabled_;

// Records/bytes dropped because every buffer needed by a record was pending.
extern uint32_t dropped_records_;
extern uint32_t dropped_bytes_;

// Handle of the SD write task, notified when a buffer fills
extern TaskHandle_t sd_task_handle_;

// Initialize the buffer system (call once in main/init)
void DataBuffersAccessMutexInit();
}  // namespace DataBuffer
