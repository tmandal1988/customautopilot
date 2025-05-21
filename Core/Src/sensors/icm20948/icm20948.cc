/*
 * icm20948.cc
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */
#include "icm20948.h"


// Initialize the static pointer to nullptr
ReadIcm20948* ReadIcm20948::read_icm20948_instance_ = nullptr;

extern I2C_HandleTypeDef hi2c2;
ReadIcm20948 read_icm20948_task_instance_(&hi2c2);


ReadIcm20948::ReadIcm20948(I2C_HandleTypeDef* hi2c):
TaskBase("Icm20948Task", 1500, osPriorityAboveNormal),
read_icm20948_task_handle_(nullptr),
icm20948_i2c_(hi2c){
	read_icm20948_instance_ = this;
}

/* This function is a wrapper for HAL_I2C_Master_Transmit,
 * it adds retry logic and a print statement in case of failure
 */
HAL_StatusTypeDef ReadIcm20948::Icm20948Write(uint8_t MemAddress,
								uint8_t *pData, uint16_t Size, uint32_t Timeout){
	uint8_t rx_count = 0;                 // Initialize retry counter to 0
	HAL_StatusTypeDef ret_;               // Variable to store the return status of HAL function
	while(rx_count < 5){                  // Retry loop, allowing up to 5 attempts
		ret_ = HAL_I2C_Mem_Write(icm20948_i2c_,
				ICM20948_ADDR << 1,      // Shift the 7-bit address left by 1 to create 8-bit address
				MemAddress,              // Memory address (register) to write to
				I2C_MEMADD_SIZE_8BIT,    // Size of memory address (8 bits)
				pData,                   // Data buffer to be transmitted
				Size,                    // Size of data to be transmitted
				Timeout);                // Timeout duration for I2C communication
		if(ret_ == HAL_OK){               // If transmission was successful, return HAL_OK
			return ret_;
		}
		HAL_Delay(100);                   // Delay 100ms before retrying
		rx_count++;                       // Increment retry counter
	}

	DEBUG_PRINT("Transmit Failed\n");        // Print error message if all retries fail

	return ret_;                          // Return the last status from HAL function
}

/* This function is a wrapper for HAL_I2C_Master_Receive,
 * it adds retry logic and a print statement in case of failure
 */
HAL_StatusTypeDef ReadIcm20948::Icm20948Read(uint8_t MemAddress,
							   uint8_t *pData, uint16_t Size, uint32_t Timeout){
	uint8_t rx_count = 0;                 // Initialize retry counter to 0
	HAL_StatusTypeDef ret_;               // Variable to store the return status of HAL function

	while(rx_count < 5){                  // Retry loop, allowing up to 5 attempts
		ret_ = HAL_I2C_Mem_Read(icm20948_i2c_,
				ICM20948_ADDR << 1,      // Shift the 7-bit address left by 1 to create 8-bit address
				MemAddress,              // Memory address (register) to read from
				I2C_MEMADD_SIZE_8BIT,    // Size of memory address (8 bits)
				pData,                   // Data buffer to store the received data
				Size,                    // Size of data to be received
				Timeout);                // Timeout duration for I2C communication
		if(ret_ == HAL_OK){               // If reception was successful, return HAL_OK
			return ret_;
		}
		HAL_Delay(100);                   // Delay 100ms before retrying
		rx_count++;                       // Increment retry counter
	}
	DEBUG_PRINT("Receive Failed\n");     // Print error message if all retries fail

	return ret_;                          // Return the last status from HAL function
}

/* Change the register bank
 */
bool ReadIcm20948::Icm20948ChangeRegBank(uint8_t usr_bank){
	HAL_StatusTypeDef ret_;
	// If the usr_bank is already current don't do anything
	if(active_usr_bank_ != usr_bank){
		ICM_20948_USER_BANK_t user_bank_reg;
		ret_ = Icm20948Read(USRBANK_SEL_REG, (uint8_t *)&user_bank_reg, 1, icm_i2c_wait_time_ms_);

		user_bank_reg.USER_BANK = usr_bank;
		ret_ = Icm20948Write(USRBANK_SEL_REG, (uint8_t *)&user_bank_reg, 1, icm_i2c_wait_time_ms_);

		active_usr_bank_ = usr_bank;
		if(ret_ == HAL_OK){
			return true;
		}
	}else{
		return true;
	}
	return false;
}

/* This function queries the device ID to test that the connection between the MCU and the
 * ICM20948 is up and active
 */
bool ReadIcm20948::Icm20948TestConnection(){
	HAL_StatusTypeDef ret_;
	uint8_t tmp;

	bool change_bank_flag_ = Icm20948ChangeRegBank(0);
	if(change_bank_flag_){
		ret_ = Icm20948Read(UB0_WHOAMI_REG, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
		if(ret_ == HAL_OK){
			if(tmp == who_am_i_val_){
				return true;
			}else{
				return false;
			}
		}
	}else{
		return false;
	}

	return false;
}

/*Resets the internal registers and restores the default settings. Then wakes up the sensor
*/
bool ReadIcm20948::Icm20948SwReset(){
	HAL_StatusTypeDef ret_;
	ICM_20948_UB0_PWR_MGMT_1_t pwr_mgmt_1_reg;

	bool change_bank_flag_ = Icm20948ChangeRegBank(0);
	if(change_bank_flag_){
		// Reset the internal registers
		Icm20948Read(UB0_PWR_MGMT_1, (uint8_t *)&pwr_mgmt_1_reg, 1, icm_i2c_wait_time_ms_);

		pwr_mgmt_1_reg.DEVICE_RESET = 1;
		ret_ = Icm20948Write(UB0_PWR_MGMT_1, (uint8_t *)&pwr_mgmt_1_reg, 1, icm_i2c_wait_time_ms_);

		HAL_Delay(50);

		// Wake up and autoselect available clock
		Icm20948Read(UB0_PWR_MGMT_1, (uint8_t *)&pwr_mgmt_1_reg, 1, icm_i2c_wait_time_ms_);
		pwr_mgmt_1_reg.SLEEP = 0;
		pwr_mgmt_1_reg.CLKSEL = 1;
		ret_ = Icm20948Write(UB0_PWR_MGMT_1, (uint8_t *)&pwr_mgmt_1_reg, 1, icm_i2c_wait_time_ms_);

		if(ret_ == HAL_OK){
			return true;
		}else{
			return false;
		}

	}else{
		return false;
	}

	return false;
}

/* Config Accels
 */
bool ReadIcm20948::Icm20948ConfigAccels(){
	HAL_StatusTypeDef ret_;
	ICM_20948_UB2_ACCEL_CONFIG_t accel_config_reg;

	bool change_bank_flag_ = Icm20948ChangeRegBank(2);
	if(change_bank_flag_){
		// Set Accel SMPLRT to 1
		uint8_t tmp = 0x03;
		ret_ = Icm20948Write(UB2_ACCEL_SMPLRT_DIV_2, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);

		ret_ = Icm20948Read(UB2_ACCEL_CONFIG, (uint8_t *)&accel_config_reg, 1, icm_i2c_wait_time_ms_);

		accel_config_reg.ACCEL_DLPFCFG = 2;//3DB BW 111.4Hz, NBW 136.0Hz, ODR 1125/(1+ACCEL_SMPLRT_DIV) = 562.5 Hz
		accel_config_reg.ACCEL_FCHOICE = 1;
		accel_config_reg.ACCEL_FS_SEL = 3;

		ret_ = Icm20948Write(UB2_ACCEL_CONFIG, (uint8_t *)&accel_config_reg, 1, icm_i2c_wait_time_ms_);

		if(ret_ == HAL_OK){
			return true;
		}else{
			return false;
		}
	}

	return false;
}

/* Config Gyros
 */
bool ReadIcm20948::Icm20948ConfigGyros(){
	HAL_StatusTypeDef ret_;
	ICM_20948_UB2_ACCEL_UB2_GYRO_CONFIG_1_t gyro_config_1_reg;

	bool change_bank_flag_ = Icm20948ChangeRegBank(2);
	if(change_bank_flag_){
		// Set Gyro SMPLRT to 1
		uint8_t tmp = 0x03;
		ret_ = Icm20948Write(UB2_GYRO_SMPLRT_DIV, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);

		ret_ = Icm20948Read(UB2_GYRO_CONFIG_1, (uint8_t *)&gyro_config_1_reg, 1, icm_i2c_wait_time_ms_);

		gyro_config_1_reg.GYRO_DLPFCFG = 1;//2000dps, 3DB BW 151.8Hz, NBW 187.6Hz, ODR 1125/(1+ACCEL_SMPLRT_DIV) = 562.5Hz
		gyro_config_1_reg.GYRO_FCHOICE = 1;
		gyro_config_1_reg.GYRO_FS_SEL = 3;

		ret_ = Icm20948Write(UB2_GYRO_CONFIG_1, (uint8_t *)&gyro_config_1_reg, 1, icm_i2c_wait_time_ms_);

		if(ret_ == HAL_OK){
			return true;
		}else{
			return false;
		}
	}

	return false;
}

/* Disable I2C Master bypass feature, in this mode ICM20948 drives the mag
 *
 */
bool ReadIcm20948::Ic20948DisableI2cMstPassThrough(){
	HAL_StatusTypeDef ret_;
	bool success = true;
	ICM_20948_UB0_INT_PIN_CFG_t int_pin_cfg_reg;

	success &= Icm20948ChangeRegBank(0);
	ret_ = Icm20948Read(UB0_INT_PIN_CFG, (uint8_t *)&int_pin_cfg_reg, 1, icm_i2c_wait_time_ms_);

	int_pin_cfg_reg.BYPASS_EN = 0;

	ret_ = Icm20948Write(UB0_INT_PIN_CFG, (uint8_t *)&int_pin_cfg_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(5);

	return success;
}

/* Enables ICM20948 I2C Master
 *
 */
bool ReadIcm20948::Ic20948EnableI2cMst(){
	HAL_StatusTypeDef ret_;
	bool success = true;
	ICM_20948_UB0_USER_CTRL_t user_ctrl_reg;

	success &= Icm20948ChangeRegBank(0);
	ret_ = Icm20948Read(UB0_USER_CTRL, (uint8_t *)&user_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	user_ctrl_reg.I2C_MST_EN = 1;

	ret_ = Icm20948Write(UB0_USER_CTRL, (uint8_t *)&user_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(5);

	return success;
}

/* Resets ICM20948 I2C Master
 *
 */
bool ReadIcm20948::Ic20948ResetI2cMst(){
	HAL_StatusTypeDef ret_;
	bool success = true;
	ICM_20948_UB0_USER_CTRL_t user_ctrl_reg;

	success &= Icm20948ChangeRegBank(0);
	ret_ = Icm20948Read(UB0_USER_CTRL, (uint8_t *)&user_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	user_ctrl_reg.I2C_MST_RST = 1;

	ret_ = Icm20948Write(UB0_USER_CTRL, (uint8_t *)&user_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(5);

	return success;
}

/* Configures the mag
 *
 */
bool ReadIcm20948::Icm20948ConfigMag(){
	HAL_StatusTypeDef ret_;
	bool success = true;
	uint8_t tmp;

	// Disable I2C Master Passthrough
	success &= Ic20948DisableI2cMstPassThrough();

	//Enable I2C Master
	success &= Ic20948EnableI2cMst();

	ICM_20948_UB3_I2C_MST_CTRL_t i2c_mst_ctrl_reg;

	//Reset I2C Master
	success &= Ic20948ResetI2cMst();

	// Configure I2C Master
	tmp = 0x17;
	Icm20948ChangeRegBank(3);
	ret_ = Icm20948Read(UB3_I2C_MST_CTRL, (uint8_t *)&i2c_mst_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	i2c_mst_ctrl_reg.I2C_MST_P_NSR = 1;
	i2c_mst_ctrl_reg.I2C_MST_CLK = 7;

	ret_ = Icm20948Write(UB3_I2C_MST_CTRL, (uint8_t *)&i2c_mst_ctrl_reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	// Verify MAG ID
	uint8_t mag_id_chk_count = 0;
	while(tmp != 0x09 && (mag_id_chk_count < 10)){
		//Reset Master
		success &= Ic20948ResetI2cMst();
		HAL_Delay(5);
		//Reset Mag
		tmp = 0x01;
		success &= Icm20948Slv4ReadWriteByte(AK09916_CNTL3, false, (uint8_t *)&tmp);
		HAL_Delay(5);
		success &= Icm20948Slv4ReadWriteByte(AK09916_WIA2, true, (uint8_t *)&tmp);
		mag_id_chk_count++;
	}

	if(mag_id_chk_count == 10){
		return false;
	}

	// Setup mag to read continuously at 100Hz
	tmp = 0x08;
	success &= Icm20948Slv4ReadWriteByte(AK09916_CNTL2, false, (uint8_t *)&tmp);
	HAL_Delay(5);
	return success;

}

/* Sets up SLV4 peripheral to do initial mag setup
 *
 */
bool ReadIcm20948::Icm20948Slv4ReadWriteByte(uint8_t reg, bool read, uint8_t *pData){
	uint8_t tmp;
	bool success = true;
	HAL_StatusTypeDef ret_;

	// Write mag address to SLV4_ADDR
	tmp = (((read) ? I2CMASTER_READ_MASK : I2CMASTER_WRITE_MASK) | AK09916_ADDR);
	Icm20948ChangeRegBank(3);
	ret_ = Icm20948Write(UB3_I2C_SLV4_ADDR, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	// Write mag register to write to and the data to write;
	ret_ = Icm20948Write(UB3_I2C_SLV4_REG, (uint8_t *)&reg, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	if(!read){
		ret_ = Icm20948Write(UB3_I2C_SLV4_DO, pData, 1, icm_i2c_wait_time_ms_);
		success &= (ret_ == HAL_OK);
	}

	// Start the txn
	tmp = 0x80;
	ret_ = Icm20948Write(UB3_I2C_SLV4_CTRL, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(10);

	Icm20948ChangeRegBank(0);
	ret_ = Icm20948Read(UB0_I2C_MST_STATUS, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);

	//Make sure the txn was complete
	if(tmp != 0x40){
		return false;
	}

	if(read){
		Icm20948ChangeRegBank(3);
		ret_ = Icm20948Read(UB3_I2C_SLV4_DI, pData, 1, icm_i2c_wait_time_ms_);
		success &= (ret_ == HAL_OK);
	}

	return success;
}

/* Sets up SLV0 peripheral to read from the mag
 *
 */
bool ReadIcm20948::Icm20948StartSlv0MagRead(){
	uint8_t tmp;
	HAL_StatusTypeDef ret_;
	bool success = true;

	// Write mag address to SLV4_ADDR with read mack
	tmp = (I2CMASTER_READ_MASK | AK09916_ADDR);
	Icm20948ChangeRegBank(3);
	ret_ = Icm20948Write(UB3_I2C_SLV0_ADDR, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(1);

	// Start reading MAG at ST1 register
	tmp = AK09916_ST1;
	ret_ = Icm20948Write(UB3_I2C_SLV0_REG, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(1);

	// Start the txn
	tmp = 0x89;
	ret_ = Icm20948Write(UB3_I2C_SLV0_CTRL, (uint8_t *)&tmp, 1, icm_i2c_wait_time_ms_);
	success &= (ret_ == HAL_OK);
	HAL_Delay(10);


	return success;
}

/* Sets the sample mode of I2C master, Accel and Gyro
 */
bool ReadIcm20948::Icm20948SetSampleMode(){
	HAL_StatusTypeDef ret_;
	ICM_20948_UB0_LP_CONFIG_t lp_config_reg;

	bool change_bank_flag_ = Icm20948ChangeRegBank(0);
	if(change_bank_flag_){
		// Keep the I2C Master in Duty Cycle mode, it seems like MAG only works in this mode
		ret_ = Icm20948Read(UB0_LP_CONFIG, (uint8_t *)&lp_config_reg, 1, icm_i2c_wait_time_ms_);
		lp_config_reg.I2C_MST_CYCLE = 1;

		ret_ = Icm20948Write(UB0_LP_CONFIG, (uint8_t *)&lp_config_reg, 1, icm_i2c_wait_time_ms_);
		if(ret_ == HAL_OK){
			return true;
		}else{
			return false;
		}

	}else{
		return false;
	}

	return false;
}

/*Enable raw data ready interrupt from any sensor to propagate to interrupt
pin 1
*/
//bool ReadIcm20948::Icm20948EnableDataRdyInt(){
//	HAL_StatusTypeDef ret_;
//	bool success = true;
//	ICM_20948_UB0_INT_ENABLE_1_t int_enable_1_reg;
//
//	success &= Icm20948ChangeRegBank(0);
//	ret_ = Icm20948Read(UB0_INT_ENABLE_1, (uint8_t *)&int_enable_1_reg, 1, icm_i2c_wait_time_ms_);
//	success &= (ret_ == HAL_OK);
//
//	int_enable_1_reg.RAW_DATA_0_RDY_EN = 1;
//	ret_ = Icm20948Write(UB0_INT_ENABLE_1, (uint8_t *)&int_enable_1_reg, 1, icm_i2c_wait_time_ms_);
//	success &= (ret_ == HAL_OK);
//
//	return success;
//}

/* Initialize all the sensors
 *
 */
bool ReadIcm20948::Icm20948Init(){
	osDelay(200);
	bool success = true;
	if(Icm20948TestConnection()){
		success = success & Icm20948SwReset();
		DEBUG_PRINT("IcmSwReset: %d\n", success);
		success = success & Icm20948ConfigAccels();
		DEBUG_PRINT("IcmConfigAccels: %d\n", success);
		success = success & Icm20948ConfigGyros();
		DEBUG_PRINT("IcmConfigGyros: %d\n", success);
		success = success & Icm20948ConfigMag();
		DEBUG_PRINT("IcmConfigMag: %d\n", success);
		success = success & Icm20948StartSlv0MagRead();
		DEBUG_PRINT("IcmStartSlv0MagRead: %d\n", success);
		success = success & Icm20948SetSampleMode();
		DEBUG_PRINT("IcmSetSampleMode: %d\n", success);

		if(success){
			return true;
		}else{
			return false;
		}
	}

	return false;
}


void ReadIcm20948::Run() {
	Icm20948Init();
	ImuData imu_data;
	Publisher<ImuData> icm20948_pub(TopicID::ICM20948);

	read_icm20948_task_handle_ = xTaskGetCurrentTaskHandle();
	int blink_counter = 0;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	osDelay(500);
    /* Infinite loop */
    for (;;) {
    	// Blink LED every 100 iterations
		if (++blink_counter >= 100) {
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 1068 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//				   used, highWaterMark * sizeof(StackType_t), 1068);
		}
    	if(HAL_I2C_GetState(icm20948_i2c_) == HAL_I2C_STATE_READY){
    		HAL_I2C_Mem_Read_DMA(icm20948_i2c_, ICM20948_ADDR << 1, UB0_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,
    				icm20948_raw_buf_, 23);
    	}

    	// Wait for DMA completion using task notification
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		Icm20948GetData(&imu_data);

		icm20948_pub.publish(imu_data);

//		DEBUG_PRINT("Ax = %g, Ay = %g, Az = %g\n", imu_data.accel_mps2[0], imu_data.accel_mps2[1], imu_data.accel_mps2[2]);
//		DEBUG_PRINT("Gx = %g, Gy = %g, Gz = %g\n", imu_data.gyro_radps[0]/DEG2RAD, imu_data.gyro_radps[1]/DEG2RAD, imu_data.gyro_radps[2]/DEG2RAD);
//		DEBUG_PRINT("Mx = %g, My = %g, Mz = %g\n", imu_data.mag_ut[0], imu_data.mag_ut[1], imu_data.mag_ut[2]);
//		DEBUG_PRINT("-----------------------------------------------------\n");

		// Wait until the next cycle
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

void ReadIcm20948::DmaCompleteCallback(I2C_HandleTypeDef *hi2c) {
    if (read_icm20948_instance_->read_icm20948_task_handle_ != nullptr) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(read_icm20948_instance_->read_icm20948_task_handle_, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
	if(hi2c->Instance == ICM20948I2C && ReadIcm20948::read_icm20948_instance_ != nullptr){
		ReadIcm20948::DmaCompleteCallback(hi2c);
	}
}

/* Read all the sensor data
 *
 */
void ReadIcm20948::Icm20948GetData(ImuData *icm20948_data){
	int16_t data_buf[9] ={0};

		  data_buf[0] = ((icm20948_raw_buf_[0] << 8) | (icm20948_raw_buf_[1] & 0xFF));
		  data_buf[1] = ((icm20948_raw_buf_[2] << 8) | (icm20948_raw_buf_[3] & 0xFF));
		  data_buf[2] = ((icm20948_raw_buf_[4] << 8) | (icm20948_raw_buf_[5] & 0xFF));

		  data_buf[3] = ((icm20948_raw_buf_[6] << 8) | (icm20948_raw_buf_[7] & 0xFF));
		  data_buf[4] = ((icm20948_raw_buf_[8] << 8) | (icm20948_raw_buf_[9] & 0xFF));
		  data_buf[5] = ((icm20948_raw_buf_[10] << 8) | (icm20948_raw_buf_[11] & 0xFF));
		  data_buf[6] = ((icm20948_raw_buf_[12] << 8) | (icm20948_raw_buf_[13] & 0xFF));

		  icm20948_data->temp_degc = (((float)data_buf[6] - 21) / 333.87) + 21;

		  data_buf[7] = ((icm20948_raw_buf_[16] << 8) | (icm20948_raw_buf_[15] & 0xFF)); //Mag data is read little endian
		  data_buf[8] = ((icm20948_raw_buf_[18] << 8) | (icm20948_raw_buf_[17] & 0xFF));
		  data_buf[9] = ((icm20948_raw_buf_[20] << 8) | (icm20948_raw_buf_[19] & 0xFF));

		  icm20948_data->mag_ut[0] = (((float)data_buf[7]) * 0.15);
		  icm20948_data->mag_ut[1] = (((float)data_buf[8]) * 0.15);
		  icm20948_data->mag_ut[2] = (((float)data_buf[9]) * 0.15);

		  icm20948_data->mag_st1 = icm20948_raw_buf_[14] & 0x03;
		  icm20948_data->mag_st2 = icm20948_raw_buf_[22] & 0x08;

		  icm20948_data->accel_mps2[0] = (((float)data_buf[0]) / 2048);
		  icm20948_data->accel_mps2[1] = -(((float)data_buf[1]) / 2048);
		  icm20948_data->accel_mps2[2] = -(((float)data_buf[2]) / 2048);

//		  icm20948_data->accel_mps2[0] = (((float)data_buf[0]) / 2048)*9.81;
//		  icm20948_data->accel_mps2[1] = -(((float)data_buf[1]) / 2048)*9.81;
//		  icm20948_data->accel_mps2[2] = -(((float)data_buf[2]) / 2048)*9.81;

		  icm20948_data->gyro_radps[0] = (((float)data_buf[3]) / 16.4)*DEG2RAD;
		  icm20948_data->gyro_radps[1] = -(((float)data_buf[4]) / 16.4)*DEG2RAD;
		  icm20948_data->gyro_radps[2] = -(((float)data_buf[5]) / 16.4)*DEG2RAD;
}

//bool ReadIcm20948::GetDataReadyFlag(){
//	if(data_ready_to_read_flag_){
//		data_ready_to_read_flag_ = false;
//		return true;
//	}else{
//		return false;
//	}
//}

//void ReadIcm20948::StartI2cDmaRead(){
//	data_ready_to_read_flag_ = false;
//	if(HAL_I2C_GetState(icm20948_i2c_) == HAL_I2C_STATE_READY){
//		HAL_I2C_Mem_Read_DMA(icm20948_i2c_, ICM20948_ADDR << 1, UB0_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,
//				icm20948_raw_buf_, 23);
//	}else{
//		__NOP();
//	}
//}

//void ReadIcm20948::SetDataReadyFlag(){
//	data_ready_to_read_flag_ = true;
//	timer_val_ = __HAL_TIM_GET_COUNTER(htim_);
//}
//
//void ReadIcm20948::DataReadyPinIsrHandler(uint8_t id){
//	if (icm20948_instance_handles_[id] != nullptr) {
//		icm20948_instance_handles_[id]->StartI2cDmaRead();
//	}
//}

//void ReadIcm20948::DmaRxCpltIsrHandler(uint8_t id){
//	if (icm20948_instance_handles_[id] != nullptr){
//		icm20948_instance_handles_[id]->SetDataReadyFlag();
//	}
//}



