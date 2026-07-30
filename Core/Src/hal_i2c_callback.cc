/*
 * Central dispatch for the application's non-registered HAL I2C callbacks.
 * Keep ISR work bounded: sensor tasks handle parsing, printing, and recovery.
 */

#include "sensors/icm20948/icm20948.h"
#include "sensors/ist8310/ist8310.h"

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* i2c_handle) {
    if (i2c_handle == nullptr) {
        return;
    }

    if ((i2c_handle->Instance == ICM20948I2C) &&
        (ReadIcm20948::read_icm20948_instance_ != nullptr)) {
        ReadIcm20948::DmaCompleteCallback(i2c_handle);
    } else if (i2c_handle->Instance == I2C1) {
        ReadIst8310::DmaCompleteCallback(i2c_handle);
    }
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* i2c_handle) {
    if (i2c_handle == nullptr) {
        return;
    }

    if ((i2c_handle->Instance == ICM20948I2C) &&
        (ReadIcm20948::read_icm20948_instance_ != nullptr)) {
        ReadIcm20948::ErrorCallback(i2c_handle);
    } else if (i2c_handle->Instance == I2C1) {
        ReadIst8310::ErrorCallback(i2c_handle);
    }
}
