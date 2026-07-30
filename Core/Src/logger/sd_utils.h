/*
 * sd_utils.h
 *
 *  Created on: Apr 12, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "debug.h"

#include "fatfs.h"

class SdUtils : public TaskBase {
 public:
  explicit SdUtils();
  void Run() override;

 private:
  // Idle wake-up period: USB commands are polled at this rate. Full data
  // buffers do not wait for it — the logger wakes this task immediately
  // via task notification.
  static constexpr uint16_t INTERVAL_MS = 500;
  // f_write() still runs for every full RAM buffer.  f_sync() is the
  // expensive durability barrier, so perform it periodically instead of
  // after every 64 KiB write, and once more when logging stops.
  static constexpr uint8_t kSyncEveryBuffers = 8;
  static constexpr uint16_t kSdSectorSize = 512;

  const char* file_name = "log_file.bin";
  bool file_open_ = false;

  FIL* file_ = nullptr;
  bool SdInit();
  bool AlignFileForAppend();
  bool IsBufferPending(uint8_t buffer_index);
  bool SyncFile();
  bool WritePendingBuffer(uint8_t buffer_index);
  bool CloseFile();
  void BeginStop();
  void ResetBufferState();
  void ResetTelemetry();
  void SetLoggingEnabled(bool enabled);
  void PrintTelemetry();

  uint8_t next_flush_index_ = 0;
  uint32_t buffers_since_sync_ = 0;
  uint32_t write_count_ = 0;
  uint32_t sync_count_ = 0;
  uint32_t write_error_count_ = 0;
  uint32_t sync_error_count_ = 0;
  uint32_t close_error_count_ = 0;
  uint32_t partial_write_count_ = 0;
  uint32_t append_padding_bytes_ = 0;
  TickType_t max_write_ticks_ = 0;
  TickType_t max_sync_ticks_ = 0;

  enum class CardState {
      IDLESTOP = 0,
      IDLESTART,
      IDLEREADY,
      WRITING,
      DRAININGSTOP,
      ERROR
  };
};
