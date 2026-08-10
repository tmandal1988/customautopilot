/*
 * usbd_storage_if.c
 *
 *  Created on: Aug 5, 2026
 *      Author: tanmay
 *
 * SCSI storage interface backing the USB MSC class with the on-board SD card.
 *
 * These callbacks run in USB interrupt context, so they use the polled
 * BSP_SD_ReadBlocks/WriteBlocks entry points rather than the DMA variants
 * that sd_diskio.c uses. The DMA path signals completion through an RTOS
 * message queue, which cannot be waited on from an ISR.
 *
 * Blocking the USB interrupt for the duration of a transfer is acceptable
 * here and only here: this interface is reachable only in log-download mode,
 * which cannot be entered while the vehicle is flyable, and FatFs is
 * unmounted for the whole time it is active so nothing else touches the card.
 */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

#include "bsp_driver_sd.h"

/* Private defines -----------------------------------------------------------*/
#define STORAGE_LUN_NBR                  1U
#define STORAGE_BLK_SIZ                  512U

/* Long enough to cover a slow card's worst-case single-block access without
 * hanging the USB interrupt indefinitely if the card is removed mid-transfer. */
#define STORAGE_TIMEOUT_MS               1000U

/* Private function prototypes -----------------------------------------------*/
static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num,
                                     uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                              uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                               uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USB Mass storage Standard Inquiry Data */
static int8_t STORAGE_Inquirydata_FS[] = {
  /* LUN 0 */
  0x00,
  0x80,  /* Removable: lets the host eject the volume and lets this firmware
          * report "no medium" rather than a hard error when appropriate. */
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'C', 'u', 's', 't', 'o', 'm', 'A', 'p',            /* Manufacturer: 8 bytes */
  'F', 'l', 'i', 'g', 'h', 't', ' ', 'L',            /* Product     : 16 bytes */
  'o', 'g', 's', ' ', ' ', ' ', ' ', ' ',
  '1', '.', '0', '0',                                /* Version     : 4 bytes  */
};

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS = {
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the storage unit (medium).
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_Init_FS(uint8_t lun)
{
  UNUSED(lun);
  /* main() already brought the SD peripheral up and MX_FATFS_Init() mounted
   * the card. The switch into download mode unmounts FatFs but leaves the
   * peripheral initialized, so re-initializing here would needlessly reset a
   * working card. Report readiness instead. */
  return (BSP_SD_GetCardState() == SD_TRANSFER_OK) ? USBD_OK : USBD_FAIL;
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number.
  * @param  block_num: Number of total blocks.
  * @param  block_size: Block size.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num,
                                     uint16_t *block_size)
{
  UNUSED(lun);
  BSP_SD_CardInfo info;

  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return (int8_t)USBD_FAIL;
  }

  BSP_SD_GetCardInfo(&info);
  *block_num = info.LogBlockNbr;
  *block_size = (uint16_t)info.LogBlockSize;

  return (int8_t)USBD_OK;
}

/**
  * @brief  Checks whether the medium is ready.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  UNUSED(lun);

  if (BSP_SD_IsDetected() != SD_PRESENT)
  {
    return (int8_t)USBD_FAIL;
  }

  return (BSP_SD_GetCardState() == SD_TRANSFER_OK) ? (int8_t)USBD_OK
                                                   : (int8_t)USBD_FAIL;
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  UNUSED(lun);
  /* Writable on purpose: the host needs to delete downloaded logs, and a
   * read-only volume makes desktop file managers complain on mount. */
  return (int8_t)USBD_OK;
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number.
  * @param  buf: Buffer that receives the data.
  * @param  blk_addr: Logical block address of the first block.
  * @param  blk_len: Number of blocks to read.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                              uint16_t blk_len)
{
  UNUSED(lun);

  if (BSP_SD_ReadBlocks((uint32_t *)(void *)buf, blk_addr, blk_len,
                        STORAGE_TIMEOUT_MS) != MSD_OK)
  {
    return (int8_t)USBD_FAIL;
  }

  /* BSP_SD_ReadBlocks() is the polled variant and has already completed when
   * it returns, so there is nothing to wait on here. */
  return (int8_t)USBD_OK;
}

/**
  * @brief  Writes data to the medium.
  * @param  lun: Logical unit number.
  * @param  buf: Buffer holding the data to write.
  * @param  blk_addr: Logical block address of the first block.
  * @param  blk_len: Number of blocks to write.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr,
                               uint16_t blk_len)
{
  UNUSED(lun);

  if (BSP_SD_WriteBlocks((uint32_t *)(void *)buf, blk_addr, blk_len,
                         STORAGE_TIMEOUT_MS) != MSD_OK)
  {
    return (int8_t)USBD_FAIL;
  }

  /* Wait for the card to leave its internal programming state before the next
   * command. Skipping this is the classic source of corrupted MSC writes. */
  uint32_t deadline = HAL_GetTick() + STORAGE_TIMEOUT_MS;
  while (BSP_SD_GetCardState() != SD_TRANSFER_OK)
  {
    if (HAL_GetTick() > deadline)
    {
      return (int8_t)USBD_FAIL;
    }
  }

  return (int8_t)USBD_OK;
}

/**
  * @brief  Returns the number of supported logical units.
  * @retval Number of logical units, minus one.
  */
static int8_t STORAGE_GetMaxLun_FS(void)
{
  return (int8_t)(STORAGE_LUN_NBR - 1U);
}
