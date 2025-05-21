/*
 * debug.h
 *
 *  Created on: Mar 30, 2025
 *      Author: tanmay
 */

#include <stdio.h>
#include "debug_config.h"
#include "usb_console/usb_console.h"

// Macro for Debug Message
#if ENABLE_DEBUG_MESSAGES
#define DEBUG_PRINT(fmt, ...)                                             \
  do {                                                                    \
    if (UsbConsole::Instance().DebugPrintAllowed()) {                    \
      printf("\rDEBUG: " fmt, ##__VA_ARGS__);                             \
    }                                                                     \
  } while (0)
#else
#define DEBUG_PRINT(fmt, ...)
#endif

// Macro for Error Message
#if ENABLE_ERROR_MESSAGES
#define ERROR_PRINT(fmt, ...)                                             \
  do {                                                                    \
    if (UsbConsole::Instance().DebugPrintAllowed()) {                    \
      printf("\rERROR: " fmt, ##__VA_ARGS__);                             \
    }                                                                     \
  } while (0)
#else
#define ERROR_PRINT(fmt, ...)
#endif


