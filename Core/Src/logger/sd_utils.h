/*
 * sd_utils.h
 *
 *  Created on: Apr 12, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "debug.h"
#include "log_file_name.h"
#include "messages/fcs_debug_data.h"
#include "pubsub/subscriber.h"

#include "fatfs.h"

class SdUtils : public TaskBase {
 public:
  explicit SdUtils();
  void Run() override;

 private:
  static constexpr uint16_t kSdSectorSize = 512;

  // Long enough for a log name plus the "ALL" keyword, with margin.
  static constexpr size_t kMaxCommandArgLength = 32U;

  // Name and index of the file this power cycle logs into. Chosen once during
  // SdInit() and reused by every subsequent STOP/START session.
  char active_file_name_[log_file_name::kNameBufferSize] = {0};
  uint32_t active_log_index_ = 0U;
  bool file_open_ = false;

  // Directory scratch shared by the boot scan, LIST and DELETE. A FILINFO is
  // roughly 280 bytes with long names enabled, which is a large fraction of
  // this task's stack, and all three users run on this task alone.
  DIR directory_scratch_{};
  FILINFO file_info_scratch_{};

  FIL* file_ = nullptr;
  bool SdInit();
  bool FindHighestLogIndex(uint32_t* highest_index);
  bool SelectNextLogFileName();
  void HandleListCommand();
  void HandleDeleteCommand(const char* argument);
  void DeleteAllLogs();
  bool HandleMassStorageCommand();
  bool FlightStateIsInactive(const char* action);

  // Latest FCS state, used to refuse console commands that must not run while
  // the vehicle is flyable. Read the same way parameter_persistence gates
  // Flash writes: a nonblocking latest-value copy, outside any control loop.
  //
  // latest_fcs_state_ is a cache rather than a scratch variable. Subscriber
  // copy() returns false both when no new sample has been published since the
  // last read and when its zero-timeout mutex take loses a race with the
  // 250 Hz publisher, so treating a failed copy as "unknown state" would
  // refuse commands at random. Keeping the last value observed makes the gate
  // decide on the freshest sample that ever arrived.
  Subscriber<FcsDebugData> fcs_state_subscriber_{TopicID::FCSDEBUG};
  FcsDebugData latest_fcs_state_{};
  bool fcs_state_seen_ = false;
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
      ERROR,
      // Terminal: the host owns the card at block level and FatFs is
      // unmounted. Only a reboot leaves this state.
      MASSSTORAGE
  };
};
