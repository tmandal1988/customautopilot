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

// Copies whatever followed the verb of the latest command, with surrounding
// whitespace removed. Reads last_cmd_buf_, which OnUsbDataReceived writes
// before it publishes command_ready_, so a caller that has already observed
// HasNewCommand() sees the matching argument.
void UsbConsole::CopyLatestArgument(char* out_buf, size_t max_len) const {
  if (!out_buf || max_len == 0) return;
  out_buf[0] = '\0';

  // Skip the verb, then the separator run before the argument.
  size_t read_index = 0;
  while (read_index < kMaxCmdLen && last_cmd_buf_[read_index] != '\0' &&
         last_cmd_buf_[read_index] != ' ') {
    ++read_index;
  }
  while (read_index < kMaxCmdLen && last_cmd_buf_[read_index] == ' ') {
    ++read_index;
  }

  size_t write_index = 0;
  while (read_index < kMaxCmdLen && last_cmd_buf_[read_index] != '\0' &&
         write_index + 1 < max_len) {
    out_buf[write_index++] = last_cmd_buf_[read_index++];
  }
  out_buf[write_index] = '\0';

  // Trailing separators, so "DELETE ALL " is still recognized as "ALL".
  while (write_index > 0 && out_buf[write_index - 1] == ' ') {
    out_buf[--write_index] = '\0';
  }
}

// Compares only the verb, so commands that carry an argument still match.
static bool VerbMatches(const char* input, size_t verb_len, const char* verb) {
  return (strlen(verb) == verb_len) && (strncmp(input, verb, verb_len) == 0);
}

UsbCommand UsbConsole::ParseCommand(const char* str) {
  const char* separator = strchr(str, ' ');
  const size_t verb_len = (separator != nullptr)
                              ? static_cast<size_t>(separator - str)
                              : strlen(str);

  if (VerbMatches(str, verb_len, "START")) return UsbCommand::START;
  if (VerbMatches(str, verb_len, "STOP")) return UsbCommand::STOP;
  if (VerbMatches(str, verb_len, "LIST")) return UsbCommand::LIST;
  if (VerbMatches(str, verb_len, "DELETE")) return UsbCommand::DELETE;
  if (VerbMatches(str, verb_len, "MSC")) return UsbCommand::MSC;

  return UsbCommand::UNKNOWN;
}

UsbCommand UsbConsole::GetLatestCommand(){
  command_ready_ = false;
  return last_command_;
}

bool UsbConsole::HasNewCommand() const {
  return command_ready_;
}





