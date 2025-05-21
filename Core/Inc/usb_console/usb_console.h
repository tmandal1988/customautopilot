/*
 * usb_console.h
 *
 *  Created on: May 17, 2025
 *      Author: tanmay
 */

#ifndef USB_CONSOLE_USB_CONSOLE_H_
#define USB_CONSOLE_USB_CONSOLE_H_

#include <atomic>
#include <string>

enum class UsbCommand {
  NONE,
  START,
  STOP,
  LIST,
  COPY,
  UNKNOWN
};

class UsbConsole {
 public:
  static UsbConsole& Instance();

  void OnUsbDataReceived(const char* data, size_t len);

  // Returns true if a new command is available
  bool HasNewCommand() const;

  // Retrieves and clears the last command
  UsbCommand GetLatestCommand();

  bool DebugPrintAllowed() const { return debug_print_allowed_; }

  // New: get the raw command buffer (copied safely)
  void GetLatestRawCommand(char* out_buf, size_t max_len);

 private:
  UsbConsole() = default;
  static constexpr size_t kMaxCmdLen = 128;

  bool overflow_ = false;
  size_t input_len_ = 0;

  std::atomic<UsbCommand> last_command_{UsbCommand::NONE};
  std::atomic<bool> command_ready_{false};
  std::atomic<bool> debug_print_allowed_{true};

  UsbCommand ParseCommand(const char* str);

  char last_cmd_buf_[kMaxCmdLen] = {0};  // stores cleaned up command
  char input_buf_[kMaxCmdLen] = {0};
};

#endif  // USB_CONSOLE_USB_CONSOLE_H_


