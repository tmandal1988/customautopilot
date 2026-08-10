/*
 * usb_console_c_wrapper.h
 *
 *  Created on: May 17, 2025
 *      Author: tanmay
 */

#ifndef USB_CONSOLE_USB_CONSOLE_C_WRAPPER_H_
#define USB_CONSOLE_USB_CONSOLE_C_WRAPPER_H_

// Provides a C-callable wrapper to forward USB CDC data
// to the UsbConsole C++ class for command parsing.

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void UsbConsoleOnUsbData(const char* data, size_t len);

#ifdef __cplusplus
}
#endif

#endif  // USB_CONSOLE_USB_CONSOLE_C_WRAPPER_H_


