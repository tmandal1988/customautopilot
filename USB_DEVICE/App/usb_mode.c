/*
 * usb_mode.c
 *
 *  Created on: Aug 5, 2026
 *      Author: tanmay
 *
 * See usb_mode.h for why this switches classes instead of running a
 * composite device, and why it is one-way.
 */

#include "usb_mode.h"

#include "cmsis_os.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

/* usbd_desc.c gives these external linkage but only declares FS_Desc in its
 * header. MSC mode reuses every string except the product name. */
extern uint8_t *USBD_FS_LangIDStrDescriptor(USBD_SpeedTypeDef speed,
                                            uint16_t *length);
extern uint8_t *USBD_FS_ManufacturerStrDescriptor(USBD_SpeedTypeDef speed,
                                                  uint16_t *length);
extern uint8_t *USBD_FS_SerialStrDescriptor(USBD_SpeedTypeDef speed,
                                            uint16_t *length);
extern uint8_t *USBD_FS_ConfigStrDescriptor(USBD_SpeedTypeDef speed,
                                            uint16_t *length);
extern uint8_t *USBD_FS_InterfaceStrDescriptor(USBD_SpeedTypeDef speed,
                                               uint16_t *length);
extern uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ];

#define USBD_VID_MSC              1155
/* One past the CDC product ID. A distinct PID keeps hosts from reusing the
 * cached CDC driver binding for what is now a mass storage device. */
#define USBD_PID_MSC              22337
#define USBD_PRODUCT_STRING_MSC   "CustomAp Flight Logs"

/* Device descriptor for download mode. Identical to the CDC one except for
 * the product ID and the class triplet: MSC is described entirely by its
 * interface descriptor, so bDeviceClass must be 0 rather than CDC's 0x02. */
__ALIGN_BEGIN static uint8_t USBD_MSC_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END = {
  0x12,                       /*bLength*/
  USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
  0x00,                       /*bcdUSB*/
  0x02,
  0x00,                       /*bDeviceClass*/
  0x00,                       /*bDeviceSubClass*/
  0x00,                       /*bDeviceProtocol*/
  USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
  LOBYTE(USBD_VID_MSC),       /*idVendor*/
  HIBYTE(USBD_VID_MSC),       /*idVendor*/
  LOBYTE(USBD_PID_MSC),       /*idProduct*/
  HIBYTE(USBD_PID_MSC),       /*idProduct*/
  0x00,                       /*bcdDevice rel. 2.00*/
  0x02,
  USBD_IDX_MFC_STR,           /*Index of manufacturer string*/
  USBD_IDX_PRODUCT_STR,       /*Index of product string*/
  USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
  USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
};

/* Written once by the SD task, polled every control frame by the motor task.
 * volatile is not for atomicity (an aligned bool is already atomic here) but
 * to stop the compiler caching the value in the motor task's loop. Without it
 * this stays correct only because LTO happens to be off, and a later build
 * change could silently disable the motor inhibit. */
static volatile bool mass_storage_active = false;

static uint8_t *UsbModeDeviceDescriptor(USBD_SpeedTypeDef speed,
                                        uint16_t *length)
{
  UNUSED(speed);
  *length = sizeof(USBD_MSC_DeviceDesc);
  return USBD_MSC_DeviceDesc;
}

static uint8_t *UsbModeProductStrDescriptor(USBD_SpeedTypeDef speed,
                                            uint16_t *length)
{
  UNUSED(speed);
  USBD_GetString((uint8_t *)USBD_PRODUCT_STRING_MSC, USBD_StrDesc, length);
  return USBD_StrDesc;
}

static USBD_DescriptorsTypeDef MSC_Desc = {
  UsbModeDeviceDescriptor,
  USBD_FS_LangIDStrDescriptor,
  USBD_FS_ManufacturerStrDescriptor,
  UsbModeProductStrDescriptor,
  USBD_FS_SerialStrDescriptor,
  USBD_FS_ConfigStrDescriptor,
  USBD_FS_InterfaceStrDescriptor
};

bool UsbModeIsMassStorageActive(void)
{
  return mass_storage_active;
}

bool UsbModeEnterMassStorage(void)
{
  if (mass_storage_active)
  {
    return true;
  }

  /* Drop off the bus first. USBD_Stop() asserts the soft-disconnect, which is
   * what makes the host discard the CDC configuration and re-enumerate rather
   * than continuing to talk to a device that has silently changed class. */
  (void)USBD_Stop(&hUsbDeviceFS);
  (void)USBD_DeInit(&hUsbDeviceFS);

  /* Hosts need to observe the disconnect before the reconnect. Anything from
   * a few milliseconds is enough electrically; this is generous because the
   * console is idle at this point and nothing is waiting on it. */
  osDelay(200);

  if (USBD_Init(&hUsbDeviceFS, &MSC_Desc, DEVICE_FS) != USBD_OK)
  {
    return false;
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK)
  {
    return false;
  }
  if (USBD_MSC_RegisterStorage(&hUsbDeviceFS,
                               &USBD_Storage_Interface_fops_FS) != USBD_OK)
  {
    return false;
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    return false;
  }

  mass_storage_active = true;
  return true;
}
