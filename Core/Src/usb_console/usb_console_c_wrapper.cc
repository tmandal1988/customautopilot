/*
 * usb_console_c_wrapper.cc
 *
 *  Created on: May 17, 2025
 *      Author: tanmay
 */

#include "usb_console/usb_console.h"

extern "C" void UsbConsoleOnUsbData(const char* data, size_t len) {
  UsbConsole::Instance().OnUsbDataReceived(data, len);
}


