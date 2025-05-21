/*
 * usb_console.cc
 *
 *  Created on: May 17, 2025
 *      Author: tanmay
 */

#include "usb_console/usb_console.h"
#include <cstring>
#include <cstdio>

UsbConsole& UsbConsole::Instance() {
  static UsbConsole instance;
  return instance;
}

// Called from USB RX (likely ISR or callback context)
void UsbConsole::OnUsbDataReceived(const char* data, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    char c = data[i];

    if (c == '\r' || c == '\n') {
      if (overflow_) {
        // Clear bad input if overflow occurred
        input_len_ = 0;
        overflow_ = false;
        continue;  // don't process garbage
      }

      if (input_len_ > 0) {
        input_buf_[input_len_] = '\0';
        memcpy(last_cmd_buf_, input_buf_, input_len_ + 1);
        last_command_ = ParseCommand(last_cmd_buf_);
        command_ready_ = true;
        input_len_ = 0;
      }
    } else {
      if (overflow_) {
        continue;  // skip all input until \n
      }

      if (input_len_ < kMaxCmdLen - 1) {
        input_buf_[input_len_++] = c;
      } else {
        // Buffer full, mark overflow
        overflow_ = true;
        input_len_ = 0;
      }
    }
  }
}


// Provide a copy of the latest raw command to caller (task-safe)
void UsbConsole::GetLatestRawCommand(char* out_buf, size_t max_len) {
  if (!out_buf || max_len == 0) return;

  for (size_t i = 0; i < max_len - 1; ++i) {
    out_buf[i] = last_cmd_buf_[i];
    if (last_cmd_buf_[i] == '\0') break;
  }

  // Always null-terminate
  out_buf[max_len - 1] = '\0';
  command_ready_ = false;
}

UsbCommand UsbConsole::ParseCommand(const char* str) {
  if (strcmp(str, "START") == 0) return UsbCommand::START;
  if (strcmp(str, "STOP") == 0) return UsbCommand::STOP;
  if (strcmp(str, "LIST") == 0) return UsbCommand::LIST;

  return UsbCommand::UNKNOWN;
}

UsbCommand UsbConsole::GetLatestCommand(){
  command_ready_ = false;
  return last_command_;
}

bool UsbConsole::HasNewCommand() const {
  return command_ready_;
}





