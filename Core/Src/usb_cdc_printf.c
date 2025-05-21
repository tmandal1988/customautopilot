/*
 * usb_cdc_printf.c
 *
 *  Created on: Mar 30, 2025
 *      Author: tanmay
 */

#include "usbd_cdc_if.h"
#include "cmsis_os.h"

// Declare the mutex
osMutexId_t usbMutex = NULL;

// Initialize the mutex (call this once in your USB init)
void InitUsbMutex(void) {
    if (usbMutex == NULL) {
        usbMutex = osMutexNew(NULL);
    }
}

// Thread safe
int _write(int file, char *ptr, int len) {
    // If RTOS is running, use mutex for thread safety
    if (osKernelGetState() == osKernelRunning) {
        // Lazy init mutex
        if (usbMutex == NULL) {
            usbMutex = osMutexNew(NULL);
            if (usbMutex == NULL) return -1;
        }

        // Acquire mutex
        if (osMutexAcquire(usbMutex, 10) != osOK) {
            return -1;
        }
    }

    // Try to send data with retries
    int retries = 100;
    while (CDC_Transmit_FS((uint8_t*) ptr, len) == USBD_BUSY) {
        if (--retries <= 0) {
            if (osKernelGetState() == osKernelRunning && usbMutex != NULL) {
                osMutexRelease(usbMutex);
            }
            return -1;
        }

        if (osKernelGetState() == osKernelRunning) {
            osDelay(1);
        } else {
            HAL_Delay(1);  // fallback before RTOS
        }
    }

    // Release mutex if we're in RTOS
    if (osKernelGetState() == osKernelRunning && usbMutex != NULL) {
        osMutexRelease(usbMutex);
    }

    return len;
}







