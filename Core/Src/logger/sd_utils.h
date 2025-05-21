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
  static constexpr uint16_t INTERVAL_MS = 500; // 2Hz logging rate

  const char* file_name = "log_file.bin";
  bool file_open_ = false;

  FIL* file_;
  bool SdInit();

  enum class CardState {
	  IDLESTOP = 0,
	  IDLESTART,
      IDLEREADY,
      WRITING
  };
};




