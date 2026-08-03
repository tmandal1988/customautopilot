/*
 * icm20948.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "messages/imu_data.h"
#include "constants.h"
#include "pubsub/publisher.h"
#include "debug.h"
#include "sensor_config.h"

class ReadIcm20948 : public TaskBase {
public:
	ReadIcm20948(I2C_HandleTypeDef* i2cHandle);
	bool Icm20948Init();

    void Run() override;
    static void DmaCompleteCallback(I2C_HandleTypeDef *hi2c);
    static void ErrorCallback(I2C_HandleTypeDef *hi2c);
    static ReadIcm20948* read_icm20948_instance_; // Static instance for DMA callback

private:
    enum class TransferResult : uint8_t {
        kIdle,
        kPending,
        kComplete,
        kError,
    };

    TaskHandle_t read_icm20948_task_handle_;
    static constexpr uint16_t READ_INTERVAL_MS = 2; // 500Hz

    /////////////////ALL THE IMPORTANT REGISTERS//////////////////////
	static constexpr uint8_t ICM20948_ADDR = 0x68;
	static constexpr uint8_t USRBANK_SEL_REG = 0x7F;

	////////////////IMPORTANT USER BANK 0 REGISTERS///////////////////
	static constexpr uint8_t UB0_WHOAMI_REG = 0x00;
	static constexpr uint8_t UB0_USER_CTRL = 0x03;
	static constexpr uint8_t UB0_LP_CONFIG = 0x05;
	static constexpr uint8_t UB0_PWR_MGMT_1 = 0x06;
	static constexpr uint8_t UB0_PWR_MGMT_2 = 0x07;
	static constexpr uint8_t UB0_INT_ENABLE_1 = 0x11;
	static constexpr uint8_t UB0_INT_PIN_CFG = 0x0F;
	static constexpr uint8_t UB0_I2C_MST_STATUS = 0x17;
	static constexpr uint8_t UB0_INT_STATUS_1 = 0x1A;
	static constexpr uint8_t UB0_ACCEL_XOUT_H = 0x2D;
	static constexpr uint8_t UB0_EXT_SLV_SENS_DATA_00 = 0x3B;
	////////////////IMPORTANT USER BANK 0 REGISTERS///////////////////

	////////////////IMPORTANT USER BANK 2 REGISTERS///////////////////
	static constexpr uint8_t UB2_GYRO_SMPLRT_DIV = 0x00;
	static constexpr uint8_t UB2_GYRO_CONFIG_1 = 0x01;
	static constexpr uint8_t UB2_ODR_ALIGN_EN = 0x09;
	static constexpr uint8_t UB2_ACCEL_SMPLRT_DIV_1 = 0x10;
	static constexpr uint8_t UB2_ACCEL_SMPLRT_DIV_2 = 0x11;
	static constexpr uint8_t UB2_ACCEL_CONFIG = 0x14;
	////////////////IMPORTANT USER BANK 2 REGISTERS///////////////////

	////////////////IMPORTANT USER BANK 3 REGISTERS///////////////////
	static constexpr uint8_t UB3_I2C_MST_ODR_CONFIG = 0x00;
	static constexpr uint8_t UB3_I2C_MST_CTRL = 0x01;
	static constexpr uint8_t UB3_I2C_SLV0_ADDR = 0x03;
	static constexpr uint8_t UB3_I2C_SLV0_REG = 0x04;
	static constexpr uint8_t UB3_I2C_SLV0_CTRL = 0x05;
	static constexpr uint8_t UB3_I2C_SLV0_DO = 0x06;
	static constexpr uint8_t UB3_I2C_SLV4_ADDR = 0x13;
	static constexpr uint8_t UB3_I2C_SLV4_REG = 0x14;
	static constexpr uint8_t UB3_I2C_SLV4_CTRL = 0x15;
	static constexpr uint8_t UB3_I2C_SLV4_DO = 0x16;
	static constexpr uint8_t UB3_I2C_SLV4_DI = 0x17;
	////////////////IMPORTANT USER BANK 3 REGISTERS///////////////////

	/////////////////MAGNETOMETER REGISTERS///////////////////////////
	static constexpr uint8_t AK09916_ADDR = 0x0C;
	static constexpr uint8_t AK09916_WIA1 = 0x00;
	static constexpr uint8_t AK09916_WIA2 = 0x01;
	static constexpr uint8_t AK09916_ST1 = 0x10;
	static constexpr uint8_t AK09916_HXL = 0x11;
	static constexpr uint8_t AK09916_CNTL2 = 0x31;
	static constexpr uint8_t AK09916_CNTL3 = 0x32;
	static constexpr uint8_t AK09916_READ_MASK = 0x01;
	static constexpr uint8_t AK09916_WRITE_MASK = 0x00;
	static constexpr uint8_t I2CMASTER_READ_MASK = 0x80;
	static constexpr uint8_t I2CMASTER_WRITE_MASK = 0x00;
	/////////////////MAGNETOMETER REGISTERS///////////////////////////

	/////////////////ALL THE IMPORTANT REGISTERS//////////////////////

	// Amount of time to wait for I2c Transmit or Receive
	static constexpr uint32_t icm_i2c_wait_time_ms_ = HAL_MAX_DELAY;
	static constexpr uint8_t who_am_i_val_ = 0xEA;
	static constexpr uint16_t kRawReadSize =
			(kStateEstimatorMagnetometerSource == MagnetometerSource::kIcm20948)
					? 23U
					: 14U;

	/////////////////struct PACKED TYPEDEF//////////////////////
	struct PACKED ICM_20948_USER_BANK_t{
		uint8_t reserved_0 : 4;
		uint8_t USER_BANK : 2;
		uint8_t reserved_1 : 2;
	};

	struct PACKED ICM_20948_UB0_USER_CTRL_t{
		uint8_t reserved_0 : 1;
		uint8_t I2C_MST_RST : 1;
		uint8_t SRAM_RST : 1;
		uint8_t DMP_RST  : 1;
		uint8_t I2C_IF_DIS : 1;
		uint8_t I2C_MST_EN  : 1;
		uint8_t FIFO_EN : 1;
		uint8_t DMP_EN : 1;

	};

	struct PACKED ICM_20948_UB0_PWR_MGMT_1_t{
		uint8_t CLKSEL : 3;
		uint8_t TEMP_DIS : 1;
		uint8_t reserved_0 : 1;
		uint8_t LP_EN : 1;
		uint8_t SLEEP : 1;
		uint8_t DEVICE_RESET : 1;
	};

	struct PACKED ICM_20948_UB0_LP_CONFIG_t{
		uint8_t reserved_0 : 4;
		uint8_t GYRO_CYCLE : 1;
		uint8_t ACCEL_CYCLE : 1;
		uint8_t I2C_MST_CYCLE : 1;
		uint8_t reserved_1 : 1;
	};

	struct PACKED ICM_20948_UB0_INT_PIN_CFG_t{
		uint8_t reserved_0 : 1;
		uint8_t BYPASS_EN : 1;
		uint8_t FSYNC_INT_MODE_EN: 1;
		uint8_t ACTL_FSYNC : 1;
		uint8_t INT_ANYRD_2CLEAR : 1;
		uint8_t INT1_LATCH__EN : 1;
		uint8_t INT1_OPEN: 1;
		uint8_t INT1_ACTL: 1;
	};

	struct PACKED ICM_20948_UB0_INT_ENABLE_1_t{
		uint8_t RAW_DATA_0_RDY_EN : 1;
		uint8_t reserved_0 : 7;
	};

	struct PACKED ICM_20948_UB2_ACCEL_CONFIG_t{
		uint8_t ACCEL_FCHOICE : 1;
		uint8_t ACCEL_FS_SEL : 2;
		uint8_t ACCEL_DLPFCFG : 3;
		uint8_t reserved_0 : 2;
	};

	struct PACKED ICM_20948_UB2_ACCEL_UB2_GYRO_CONFIG_1_t{
		uint8_t GYRO_FCHOICE : 1;
		uint8_t GYRO_FS_SEL : 2;
		uint8_t GYRO_DLPFCFG : 3;
		uint8_t reserved_0 : 2;
	};

	struct PACKED ICM_20948_UB2_ODR_ALIGN_EN_t{
		uint8_t ODR_ALIGN_EN : 1;
		uint8_t reserved : 7;

	};

	struct PACKED ICM_20948_UB3_I2C_MST_CTRL_t{
		uint8_t I2C_MST_CLK : 4;
		uint8_t I2C_MST_P_NSR : 1;
		uint8_t reserved_0 : 2;
		uint8_t MULT_MST_EN : 1;

	};

	/////////////////struct PACKED TYPEDEF//////////////////////
	uint8_t icm20948_raw_buf_[23]={0};
	I2C_HandleTypeDef* icm20948_i2c_; // This I2C is used to communicate with ICM20948
	volatile TransferResult transfer_result_ = TransferResult::kIdle;

	// Active user bank
	uint8_t active_usr_bank_ = 37;

	HAL_StatusTypeDef Icm20948Write(uint8_t MemAddress,
				uint8_t *pData, uint16_t Size, uint32_t Timeout);

	HAL_StatusTypeDef Icm20948Read(uint8_t MemAddress,
			uint8_t *pData, uint16_t Size, uint32_t Timeout);

	bool Icm20948TestConnection();

	bool Icm20948ChangeRegBank(uint8_t usr_bank);

	bool Icm20948SwReset();

	bool Icm20948ConfigAccels();

	bool Icm20948ConfigGyros();

	bool Ic20948DisableI2cMstPassThrough();

	bool Ic20948EnableI2cMst();

	bool Ic20948ResetI2cMst();

	bool Icm20948ConfigMag();

	bool Icm20948Slv4ReadWriteByte(uint8_t reg, bool read, uint8_t *pData);

	bool Icm20948StartSlv0MagRead();

	bool Icm20948SetSampleMode();

	void Icm20948GetData(ImuData *icm20948_data);
};
