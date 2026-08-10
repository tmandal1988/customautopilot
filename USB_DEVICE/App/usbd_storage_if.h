/*
 * usbd_storage_if.h
 *
 *  Created on: Aug 5, 2026
 *      Author: tanmay
 *
 * SCSI storage interface backing the USB MSC class with the on-board SD card.
 * Only reachable in log-download mode; see usb_mode.h for how that mode is
 * entered and why it cannot be entered while the vehicle is flyable.
 */

#pragma once

#include "usbd_msc.h"

#ifdef __cplusplus
extern "C" {
#endif

extern USBD_StorageTypeDef USBD_Storage_Interface_fops_FS;

#ifdef __cplusplus
}
#endif
