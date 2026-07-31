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
  void RequestParameterLogStop();
  void BeginStop();
  void ResetBufferState();
  void ResetTelemetry();
  void SetLoggingEnabled(bool enabled);
  void PrintTelemetry();
  void LoadBootParameters();

  // LOG_SYNC_BUFS and LOG_IDLE_MS require a reboot. They are copied once at
  // task startup, so parameter access never enters the steady-state SD loop.
  uint32_t sync_every_buffers_;
  TickType_t idle_wait_ticks_;
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
      WAITINGPARAMSTOP,
      DRAININGSTOP,
      ERROR
  };
};
