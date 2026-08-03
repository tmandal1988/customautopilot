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
	HAL_StatusTypeDef ret_ = HAL_ERROR;   // Variable to store the return status of HAL function
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

	DEBUG_PRINT("ICM20948 write failed: reg=0x%02x status=%d error=0x%08lx state=%lu\n",
			MemAddress, ret_,
			static_cast<unsigned long>(HAL_I2C_GetError(icm20948_i2c_)),
			static_cast<unsigned long>(HAL_I2C_GetState(icm20948_i2c_)));

	return ret_;                          // Return the last status from HAL function
}

/* This function is a wrapper for HAL_I2C_Master_Receive,
 * it adds retry logic and a print statement in case of failure
 */
HAL_StatusTypeDef ReadIcm20948::Icm20948Read(uint8_t MemAddress,
							   uint8_t *pData, uint16_t Size, uint32_t Timeout){
	uint8_t rx_count = 0;                 // Initialize retry counter to 0
	HAL_StatusTypeDef ret_ = HAL_ERROR;   // Variable to store the return status of HAL function

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
	DEBUG_PRINT("ICM20948 read failed: reg=0x%02x status=%d error=0x%08lx state=%lu\n",
			MemAddress, ret_,
			static_cast<unsigned long>(HAL_I2C_GetError(icm20948_i2c_)),
			static_cast<unsigned long>(HAL_I2C_GetState(icm20948_i2c_)));

	return ret_;                          // Return the last status from HAL function
}

/* Change the register bank
 */
bool ReadIcm20948::Icm20948ChangeRegBank(uint8_t usr_bank){
	HAL_StatusTypeDef ret_;
	// If the usr_bank is already current don't do anything
	if(active_usr_bank_ != usr_bank){
		ICM_20948_USER_BANK_t user_bank_reg{};
		ret_ = Icm20948Read(USRBANK_SEL_REG, (uint8_t *)&user_bank_reg, 1, icm_i2c_wait_time_ms_);
		if(ret_ != HAL_OK){
			active_usr_bank_ = 37U;
			return false;
		}

		user_bank_reg.USER_BANK = usr_bank;
		ret_ = Icm20948Write(USRBANK_SEL_REG, (uint8_t *)&user_bank_reg, 1, icm_i2c_wait_time_ms_);

		if(ret_ == HAL_OK){
			active_usr_bank_ = usr_bank;
			return true;
		}
		active_usr_bank_ = 37U;
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
bool ReadIcm20948::RecoverI2cPeripheral() {
	transfer_result_ = TransferResult::kIdle;
	(void)HAL_I2C_Master_Abort_IT(icm20948_i2c_, ICM20948_ADDR << 1);
	if (read_icm20948_task_handle_ != nullptr) {
		(void)ulTaskNotifyTake(pdTRUE, 0);
	}

	(void)HAL_I2C_DeInit(icm20948_i2c_);
	osDelay(2);
	active_usr_bank_ = 37U;

	if (HAL_I2C_Init(icm20948_i2c_) != HAL_OK) {
		DEBUG_PRINT("ICM20948 I2C recovery: HAL_I2C_Init failed\n");
		return false;
	}
	if (HAL_I2CEx_ConfigAnalogFilter(icm20948_i2c_,
			I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
		DEBUG_PRINT("ICM20948 I2C recovery: analog filter config failed\n");
		return false;
	}
	if (HAL_I2CEx_ConfigDigitalFilter(icm20948_i2c_, 0) != HAL_OK) {
		DEBUG_PRINT("ICM20948 I2C recovery: digital filter config failed\n");
		return false;
	}

	return true;
}

void ReadIcm20948::RecordRuntimeTransferFailure(const char* reason) {
	if (consecutive_runtime_failures_ < UINT8_MAX) {
		++consecutive_runtime_failures_;
	}

	if (consecutive_runtime_failures_ >= kRuntimeFailureLogThreshold) {
		DEBUG_PRINT("ICM20948 runtime transfer failures: %s, count=%u; no runtime reinit\n",
				(reason != nullptr) ? reason : "unknown",
				static_cast<unsigned int>(consecutive_runtime_failures_));
		consecutive_runtime_failures_ = 0U;
	}
}

void ReadIcm20948::ClearRuntimeTransferFailures() {
	consecutive_runtime_failures_ = 0U;
}

bool ReadIcm20948::StartInertialRead() {
	if ((read_icm20948_task_handle_ == nullptr) ||
			(transfer_result_ != TransferResult::kIdle)) {
		return false;
	}

	if (HAL_I2C_GetState(icm20948_i2c_) != HAL_I2C_STATE_READY) {
#if RTOS_METRICS_ENABLE
		++metrics_i2c_busy_skips_;
		SetAuxMetric(0U, metrics_i2c_busy_skips_);
#endif
		RecordRuntimeTransferFailure("I2C not ready");
		return false;
	}

	// A notification always belongs to the DMA transaction started below.
	// Discarding a stale notification prevents a late callback from making a
	// new sample appear complete immediately.
	(void)ulTaskNotifyTake(pdTRUE, 0);
	transfer_result_ = TransferResult::kPending;
	const HAL_StatusTypeDef dma_start_status = HAL_I2C_Mem_Read_DMA(
			icm20948_i2c_, ICM20948_ADDR << 1, UB0_ACCEL_XOUT_H,
			I2C_MEMADD_SIZE_8BIT, icm20948_raw_buf_, kRawReadSize);
	if (dma_start_status == HAL_OK) {
		return true;
	}

	transfer_result_ = TransferResult::kIdle;
#if RTOS_METRICS_ENABLE
	++metrics_dma_start_failures_;
	SetAuxMetric(1U, metrics_dma_start_failures_);
#endif
	RecordRuntimeTransferFailure("DMA start failed");
	return false;
}

bool ReadIcm20948::CompleteInertialRead(ImuData* icm20948_data) {
	if (icm20948_data == nullptr) {
		return false;
	}

	ClearRuntimeTransferFailures();
	Icm20948GetData(icm20948_data);
	return true;
}

void ReadIcm20948::AbortRuntimeTransfer() {
	(void)HAL_I2C_Master_Abort_IT(icm20948_i2c_, ICM20948_ADDR << 1);
	transfer_result_ = TransferResult::kIdle;
}

bool ReadIcm20948::TickReached(TickType_t now, TickType_t deadline) {
	return static_cast<int32_t>(now - deadline) >= 0;
}

TickType_t ReadIcm20948::AdvanceRelease(TickType_t previous_release,
		TickType_t now, TickType_t period) {
	const TickType_t next_release = previous_release + period;
	if (TickReached(now, next_release)) {
		return now + period;
	}
	return next_release;
}

bool ReadIcm20948::Icm20948Init(){
	osDelay(200);
	active_usr_bank_ = 37U;
	if(!Icm20948TestConnection()){
		DEBUG_PRINT("IcmTestConnection: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmTestConnection: 1\n");

	if(!Icm20948SwReset()){
		DEBUG_PRINT("IcmSwReset: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmSwReset: 1\n");

	if(!Icm20948ConfigAccels()){
		DEBUG_PRINT("IcmConfigAccels: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmConfigAccels: 1\n");

	if(!Icm20948ConfigGyros()){
		DEBUG_PRINT("IcmConfigGyros: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmConfigGyros: 1\n");

	if(!Icm20948ConfigMag()){
		DEBUG_PRINT("IcmConfigMag: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmConfigMag: 1\n");

	if(!Icm20948StartSlv0MagRead()){
		DEBUG_PRINT("IcmStartSlv0MagRead: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmStartSlv0MagRead: 1\n");

	if(!Icm20948SetSampleMode()){
		DEBUG_PRINT("IcmSetSampleMode: 0\n");
		return false;
	}
	DEBUG_PRINT("IcmSetSampleMode: 1\n");

	return true;
}


void ReadIcm20948::Run() {
	ImuData imu_data = {};
	Publisher<ImuData> icm20948_pub(TopicID::ICM20948);

	read_icm20948_task_handle_ = xTaskGetCurrentTaskHandle();
	int blink_counter = 0;

	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);
	const TickType_t runtime_transfer_timeout =
			pdMS_TO_TICKS(kDmaCompletionTimeoutMs);
	TickType_t next_release = xTaskGetTickCount();
	bool first_iteration = true;
	bool sensor_initialized = false;
    /* Infinite loop */
    for (;;) {
		if (!sensor_initialized) {
			if (!Icm20948Init()) {
				DEBUG_PRINT("ICM20948 init failed, recovering I2C and retrying\n");
				(void)RecoverI2cPeripheral();
				osDelay(kInitRetryDelayMs);
				next_release = xTaskGetTickCount();
				continue;
			}

			ClearRuntimeTransferFailures();
			transfer_result_ = TransferResult::kIdle;
			(void)ulTaskNotifyTake(pdTRUE, 0);
			osDelay(500);
			next_release = xTaskGetTickCount();
			ConfigureEventMetrics();
			first_iteration = true;
			sensor_initialized = true;
			continue;
		}

		if (transfer_result_ != TransferResult::kIdle) {
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
			const uint32_t cycle_context_switch_start =
					rtos_metrics::ContextSwitchCount();
#endif
#if RTOS_METRICS_ENABLE
			const uint32_t dma_wait_start_cycles = rtos_metrics::CyclesNow();
#endif
			uint32_t notify_count = 0U;
			if (transfer_result_ == TransferResult::kPending) {
				notify_count = ulTaskNotifyTake(pdTRUE,
						runtime_transfer_timeout);
			} else {
				notify_count = ulTaskNotifyTake(pdTRUE, 0U);
			}
			BeginMetricsCycle();
#if RTOS_METRICS_ENABLE
			UpdateAuxMetricMaximum(2U,
					rtos_metrics::CyclesNow() - dma_wait_start_cycles);
#endif
			const TransferResult transfer_result = transfer_result_;
			if ((notify_count == 0U) &&
					(transfer_result == TransferResult::kPending)) {
				AbortRuntimeTransfer();
				RecordRuntimeTransferFailure("DMA completion timeout");
			} else {
				transfer_result_ = TransferResult::kIdle;
				if (transfer_result == TransferResult::kComplete) {
#if RTOS_METRICS_ENABLE
					const uint32_t parse_publish_start_cycles =
							rtos_metrics::CyclesNow();
#endif
					if (CompleteInertialRead(&imu_data)) {
						++imu_data.publish_seq;
						icm20948_pub.publish(imu_data);
					}
#if RTOS_METRICS_ENABLE
					UpdateAuxMetricMaximum(3U,
							rtos_metrics::CyclesNow() -
							parse_publish_start_cycles);
#endif
				} else if (transfer_result == TransferResult::kError) {
					AbortRuntimeTransfer();
					RecordRuntimeTransferFailure("DMA transfer error");
				} else {
					RecordRuntimeTransferFailure("DMA notification mismatch");
				}
			}

			EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
			UpdateAuxMetricMaximum(4U,
					rtos_metrics::ContextSwitchCount() -
					cycle_context_switch_start);
#endif
			continue;
		}

		TickType_t now = xTaskGetTickCount();
		if (!TickReached(now, next_release)) {
			vTaskDelay(next_release - now);
			continue;
		}

#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
		const uint32_t cycle_context_switch_start =
				rtos_metrics::ContextSwitchCount();
#endif
		BeginMetricsCycle();
		now = xTaskGetTickCount();
		const int32_t start_lateness_ticks =
				static_cast<int32_t>(now - next_release);

		++imu_data.task_run_seq;
		if (!first_iteration && start_lateness_ticks > 0) {
			++imu_data.late_start_count;
		}
		first_iteration = false;

		// Blink LED every 100 iterations
		if (++blink_counter >= 100) {
			HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 1068 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//				   used, highWaterMark * sizeof(StackType_t), 1068);
		}

		(void)StartInertialRead();
		next_release = AdvanceRelease(next_release, now, xFrequency);
//		DEBUG_PRINT("Ax = %g, Ay = %g, Az = %g\n", imu_data.accel_mps2[0], imu_data.accel_mps2[1], imu_data.accel_mps2[2]);
//		DEBUG_PRINT("Gx = %g, Gy = %g, Gz = %g\n", imu_data.gyro_radps[0]/DEG2RAD, imu_data.gyro_radps[1]/DEG2RAD, imu_data.gyro_radps[2]/DEG2RAD);
//		DEBUG_PRINT("Mx = %g, My = %g, Mz = %g\n", imu_data.mag_ut[0], imu_data.mag_ut[1], imu_data.mag_ut[2]);
//		DEBUG_PRINT("-----------------------------------------------------\n");

		EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
		UpdateAuxMetricMaximum(4U,
				rtos_metrics::ContextSwitchCount() -
				cycle_context_switch_start);
#endif
    }
}

void ReadIcm20948::DmaCompleteCallback(I2C_HandleTypeDef *hi2c) {
    ReadIcm20948* instance = read_icm20948_instance_;
	if ((instance != nullptr) && (hi2c == instance->icm20948_i2c_) &&
		(instance->read_icm20948_task_handle_ != nullptr) &&
		(instance->transfer_result_ == TransferResult::kPending)) {
		instance->transfer_result_ = TransferResult::kComplete;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(instance->read_icm20948_task_handle_,
                              &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void ReadIcm20948::ErrorCallback(I2C_HandleTypeDef *hi2c) {
    ReadIcm20948* instance = read_icm20948_instance_;
	if ((instance != nullptr) && (hi2c == instance->icm20948_i2c_) &&
		(instance->read_icm20948_task_handle_ != nullptr) &&
		(instance->transfer_result_ == TransferResult::kPending)) {
		instance->transfer_result_ = TransferResult::kError;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        vTaskNotifyGiveFromISR(instance->read_icm20948_task_handle_,
                              &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

/* Read all the sensor data
 *
 */
void ReadIcm20948::Icm20948GetData(ImuData *icm20948_data){
	int16_t data_buf[10] ={0};

		  data_buf[0] = ((icm20948_raw_buf_[0] << 8) | (icm20948_raw_buf_[1] & 0xFF));
		  data_buf[1] = ((icm20948_raw_buf_[2] << 8) | (icm20948_raw_buf_[3] & 0xFF));
		  data_buf[2] = ((icm20948_raw_buf_[4] << 8) | (icm20948_raw_buf_[5] & 0xFF));

		  data_buf[3] = ((icm20948_raw_buf_[6] << 8) | (icm20948_raw_buf_[7] & 0xFF));
		  data_buf[4] = ((icm20948_raw_buf_[8] << 8) | (icm20948_raw_buf_[9] & 0xFF));
		  data_buf[5] = ((icm20948_raw_buf_[10] << 8) | (icm20948_raw_buf_[11] & 0xFF));
		  data_buf[6] = ((icm20948_raw_buf_[12] << 8) | (icm20948_raw_buf_[13] & 0xFF));

		  icm20948_data->temp_degc = (((float)data_buf[6] - 21) / 333.87) + 21;

		  if constexpr (kStateEstimatorMagnetometerSource ==
				  MagnetometerSource::kIcm20948) {
			  data_buf[7] = ((icm20948_raw_buf_[16] << 8) | (icm20948_raw_buf_[15] & 0xFF)); //Mag data is read little endian
			  data_buf[8] = ((icm20948_raw_buf_[18] << 8) | (icm20948_raw_buf_[17] & 0xFF));
			  data_buf[9] = ((icm20948_raw_buf_[20] << 8) | (icm20948_raw_buf_[19] & 0xFF));

			  icm20948_data->mag_ut[0] = (((float)data_buf[7]) * 0.15);
			  icm20948_data->mag_ut[1] = (((float)data_buf[8]) * 0.15);
			  icm20948_data->mag_ut[2] = (((float)data_buf[9]) * 0.15);

			  icm20948_data->mag_st1 = icm20948_raw_buf_[14] & 0x03;
			  icm20948_data->mag_st2 = icm20948_raw_buf_[22] & 0x08;
		  } else {
			  icm20948_data->mag_ut[0] = 0.0F;
			  icm20948_data->mag_ut[1] = 0.0F;
			  icm20948_data->mag_ut[2] = 0.0F;
			  icm20948_data->mag_st1 = 0U;
			  icm20948_data->mag_st2 = 0U;
		  }

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
