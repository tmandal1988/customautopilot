/*
 * usb_mode.h
 *
 *  Created on: Aug 5, 2026
 *      Author: tanmay
 *
 * Runtime switch of the USB device from CDC (console) to MSC (log download).
 *
 * The two classes are not composite: OTG_FS has 1.25 KB of FIFO RAM and the
 * existing carve-up uses all of it. They do not need to coexist, because MSC
 * claims the same endpoints CDC does (0x81 IN / 0x01 OUT) and needs one fewer
 * TX FIFO, so the stock FIFO configuration serves either class unchanged.
 *
 * The switch is deliberately one-way. Returning to CDC would mean tearing the
 * volume out from under a host that still has it mounted, and a reboot is
 * both simpler and the only way to guarantee the flight configuration is
 * fully restored. Power-cycling the board is the way out of download mode.
 */

#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Tears down the CDC device and re-enumerates as USB mass storage backed by
// the SD card. Returns false and leaves USB stopped if any stage fails.
//
// The caller must have closed the log file and unmounted FatFs first: from
// here on the host owns the volume at block level, and a mounted FatFs would
// be handing out stale cached sectors.
//
// Never call this while the vehicle is flyable. SdUtils gates it on the SD
// state machine being idle and the FCS reporting INACTIVE.
bool UsbModeEnterMassStorage(void);

// True once UsbModeEnterMassStorage() has succeeded. Only a reboot clears it.
bool UsbModeIsMassStorageActive(void);

#ifdef __cplusplus
}
#endif
