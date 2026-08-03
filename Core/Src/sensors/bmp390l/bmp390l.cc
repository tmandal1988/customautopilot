/*
 * bmp390l.cc
 *
 *  Created on: Apr 1, 2025
 *      Author: tanmay
 */
#include "bmp390l.h"

extern SPI_HandleTypeDef hspi5;
ReadBmp390l read_bmp390l_task_instance_(&hspi5);
ReadBmp390l* ReadBmp390l::instance_ = nullptr;


ReadBmp390l::ReadBmp390l(SPI_HandleTypeDef* hspi):
TaskBase("Bmp390lTask", 1062, osPriorityAboveNormal),
bmp390l_spi_(hspi){
	instance_ = this;
}

bool ReadBmp390l::WriteSingleRegister(uint8_t reg_addr, uint8_t value){
	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_RESET);
	// BMP390L uses MSB = 0 for write operations
	tx_buf_[0] = static_cast<uint8_t>(reg_addr & 0x7F);
	tx_buf_[1] = value;

	// Transmit register address + value
	HAL_StatusTypeDef status = HAL_SPI_Transmit(bmp390l_spi_, tx_buf_, 2, baro_spi_tx_delay_ms_);

	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);

	return (status == HAL_OK);
}

bool ReadBmp390l::ReadSingleRegister(uint8_t reg_addr){
	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_RESET);
	// BMP390L uses MSB = 0 for write operations
	tx_buf_[0] = static_cast<uint8_t>(reg_addr | 0x80);

	// Transmit register address to read and then read the data
	HAL_StatusTypeDef status_tx = HAL_SPI_Transmit(bmp390l_spi_, &tx_buf_[0], 1, baro_spi_tx_delay_ms_);
	HAL_StatusTypeDef status_rx = HAL_SPI_Receive(bmp390l_spi_, rx_buf_, 2, baro_spi_rx_delay_ms_);

	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);

	return (status_rx == HAL_OK && status_tx == HAL_OK);
}

bool ReadBmp390l::ReadMultipleRegisters(uint8_t reg_addr, uint8_t num_bytes){
	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_RESET);
	// BMP390L uses MSB = 0 for write operations
	tx_buf_[0] = static_cast<uint8_t>(reg_addr | 0x80);

	// Transmit register address to read and then read the data
	HAL_StatusTypeDef status_tx = HAL_SPI_Transmit(bmp390l_spi_, &tx_buf_[0], 1, baro_spi_tx_delay_ms_);
	HAL_StatusTypeDef status_rx = HAL_SPI_Receive(bmp390l_spi_, rx_buf_, num_bytes + 1, baro_spi_rx_delay_ms_);

	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);

	return (status_rx == HAL_OK && status_tx == HAL_OK);
//	return (status_tx == HAL_OK);
}

bool ReadBmp390l::Bmp390lInit(){
	bool init_status = true;
	bool read_status = false;

	//Confirm chip id value
	uint8_t r_idx = 0;
	while(!read_status && r_idx < spi_retry_){
		ReadSingleRegister(REG_ADDR_CHIP_ID);
		read_status = rx_buf_[1] == CHIP_ID;
		r_idx++;
		osDelay(1);
	}
	init_status &= read_status;
	if(init_status){
		DEBUG_PRINT("BM390L -> CHIP_ID Valid\n");
	}

	osDelay(1);

	//Set OSR val
	WriteSingleRegister(REG_ADDR_OSR, REG_VAL_OSR);
	//confirm
	r_idx = 0;
	read_status = false;
	while(!read_status && r_idx < spi_retry_){
		ReadSingleRegister(REG_ADDR_OSR);
		read_status = rx_buf_[1] == REG_VAL_OSR;
		r_idx++;
		osDelay(1);
	}

	init_status &= read_status;
	if(init_status){
		DEBUG_PRINT("BM390L -> OSR SET\n");
	}

	osDelay(1);

	//Set ODR val
	WriteSingleRegister(REG_ADDR_ODR, REG_VAL_ODR);
	//confirm
	r_idx = 0;
	read_status = false;
	while(!read_status && r_idx < spi_retry_){
		ReadSingleRegister(REG_ADDR_ODR);
		read_status = rx_buf_[1] == REG_VAL_ODR;
		r_idx++;
		osDelay(1);
	}
	init_status &= read_status;
	if(init_status){
		DEBUG_PRINT("BM390L -> ODR SET\n");
	}

	osDelay(1);

	//Set CONFIG val
	WriteSingleRegister(REG_ADDR_CONFIG, REG_VAL_CONFIG);
	//confirm
	r_idx = 0;
	read_status = false;
	while(!read_status && r_idx < spi_retry_){
		ReadSingleRegister(REG_ADDR_CONFIG);
		read_status = rx_buf_[1] == REG_VAL_CONFIG;
		r_idx++;
		osDelay(1);
	}
	init_status &= read_status;
	if(init_status){
		DEBUG_PRINT("BM390L -> CONFIG SET\n");
	}

	osDelay(1);

	//confirm
	r_idx = 0;
	read_status = false;
	while(!read_status && r_idx < spi_retry_){
		read_status = ReadMultipleRegisters(REG_ADDR_CALIB_DATA, 21);
		r_idx++;
		osDelay(1);
	}
	init_status &= read_status;

	if(init_status){
			DEBUG_PRINT("BM390L -> RECEIVED CALIB DATA\n");
	}

	NVM_PAR_T1 = (static_cast<uint16_t>(rx_buf_[2]) << 8) | static_cast<uint16_t>(rx_buf_[1]);
	NVM_PAR_T2 = (static_cast<uint16_t>(rx_buf_[4]) << 8) | static_cast<uint16_t>(rx_buf_[3]);
	NVM_PAR_T3 = static_cast<int8_t>(rx_buf_[5]);


	NVM_PAR_P1 = (static_cast<int16_t>(rx_buf_[7]) << 8) | static_cast<int16_t>(rx_buf_[6]);
	NVM_PAR_P2 = (static_cast<int16_t>(rx_buf_[9]) << 8) | static_cast<int16_t>(rx_buf_[8]);
	NVM_PAR_P3 = static_cast<int8_t>(rx_buf_[10]);
	NVM_PAR_P4 = static_cast<int8_t>(rx_buf_[11]);
	NVM_PAR_P5 = (static_cast<uint16_t>(rx_buf_[13]) << 8) | static_cast<uint16_t>(rx_buf_[12]);
	NVM_PAR_P6 = (static_cast<uint16_t>(rx_buf_[15]) << 8) | static_cast<uint16_t>(rx_buf_[14]);

	NVM_PAR_P7 = static_cast<int8_t>(rx_buf_[16]);
	NVM_PAR_P8 = static_cast<int8_t>(rx_buf_[17]);
	NVM_PAR_P9 = (static_cast<int16_t>(rx_buf_[19]) << 8) | static_cast<int16_t>(rx_buf_[18]);
	NVM_PAR_P10 = static_cast<int8_t>(rx_buf_[20]);
	NVM_PAR_P11 = static_cast<int8_t>(rx_buf_[21]);

	par_t1_ = NVM_PAR_T1/std::pow(2, -8);
	par_t2_ = NVM_PAR_T2/std::pow(2, 30);
	par_t3_ = NVM_PAR_T3/std::pow(2, 48);

	par_p1_ = (NVM_PAR_P1 - std::pow(2, 14))/std::pow(2, 20);
	par_p2_ = (NVM_PAR_P2 - std::pow(2, 14))/std::pow(2, 29);
	par_p3_ = NVM_PAR_P3/std::pow(2, 32);
	par_p4_ = NVM_PAR_P4/std::pow(2, 37);
	par_p5_ = NVM_PAR_P5/std::pow(2, -3);
	par_p6_ = NVM_PAR_P6/std::pow(2, 6);
	par_p7_ = NVM_PAR_P7/std::pow(2, 8);
	par_p8_ = NVM_PAR_P8/std::pow(2, 15);
	par_p9_ = NVM_PAR_P9/std::pow(2, 48);
	par_p10_ = NVM_PAR_P10/std::pow(2, 48);
	par_p11_ = NVM_PAR_P11/std::pow(2, 65);

	HAL_Delay(1);

	//Set PWR_CTRL val
	WriteSingleRegister(REG_ADDR_PWR_CTRL, REG_VAL_PWR_CTRL);
	//confirm
	ReadSingleRegister(REG_ADDR_PWR_CTRL);
//	r_idx = 0;
//	read_status = false;
//	while(!read_status && r_idx < spi_retry_){
//		ReadSingleRegister(REG_ADDR_PWR_CTRL);
//		read_status = rx_buf_[1] == REG_VAL_PWR_CTRL;
//		r_idx++;
//		osDelay(1);
//	}
//	init_status &= read_status;
	init_status &= rx_buf_[1] == REG_VAL_PWR_CTRL;
	if(init_status){
		DEBUG_PRINT("BM390L -> PWR_CTRL SET\n");
	}

	osDelay(1);


	return init_status;
}

void ReadBmp390l::Bmp390lGetPressAndTemp(){
	ReadMultipleRegisters(REG_ADDR_DATA, 6);
	CompensatePressureAndTemperature();
}

bool ReadBmp390l::StartPressureTemperatureRead(){
	if ((task_handle_ == nullptr) ||
	    (transfer_result_ != TransferResult::kIdle) ||
	    (HAL_SPI_GetState(bmp390l_spi_) != HAL_SPI_STATE_READY)) {
		return false;
	}

	// Clear any stale completion notification before arming a new transfer.
	(void)ulTaskNotifyTake(pdTRUE, 0U);

	tx_buf_[0] = static_cast<uint8_t>(REG_ADDR_DATA | 0x80);
	transfer_result_ = TransferResult::kPending;
	transfer_phase_ = TransferPhase::kAddress;
	HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_RESET);
	HAL_StatusTypeDef status =
			HAL_SPI_Transmit_IT(bmp390l_spi_, tx_buf_, 1U);
	if (status != HAL_OK) {
		HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);
		transfer_phase_ = TransferPhase::kIdle;
		transfer_result_ = TransferResult::kIdle;
		return false;
	}

	return true;
}

bool ReadBmp390l::CompletePressureTemperatureRead(BaroData* baro_data){
	if ((baro_data == nullptr) ||
	    (transfer_result_ != TransferResult::kComplete)) {
		return false;
	}

	CompensatePressureAndTemperature();
	baro_data->press_pa = press_;
	baro_data->temp_degc = temp_;
	return true;
}

void ReadBmp390l::CompensatePressureAndTemperature(){
	// Reconstruct raw pressure from 3 bytes (big-endian)
	raw_press_ = (static_cast<uint32_t>(rx_buf_[3]) << 16) |
	             (static_cast<uint32_t>(rx_buf_[2]) << 8)  |
	             static_cast<uint32_t>(rx_buf_[1]);

	// Reconstruct raw temperature from 3 bytes (big-endian)
	raw_temp_ = (static_cast<uint32_t>(rx_buf_[6]) << 16) |
	            (static_cast<uint32_t>(rx_buf_[5]) << 8)  |
	            static_cast<uint32_t>(rx_buf_[4]);

	// --- Temperature Compensation (double precision) ---
	const double diff = static_cast<double>(raw_temp_) - par_t1_;
	const double diff_sq = diff * diff;

	temp_ = diff * par_t2_ + diff_sq * par_t3_;

	// --- Pressure Compensation (double precision) ---

	const double temp_sq = temp_ * temp_;
	const double temp_cu = temp_sq * temp_;
	const double raw_press_d = static_cast<double>(raw_press_);
	const double raw_press_sq = raw_press_d * raw_press_d;
	const double raw_press_cu = raw_press_sq * raw_press_d;

	// partial_out1
	const double out1 = par_p5_ +
	                    par_p6_ * temp_ +
	                    par_p7_ * temp_sq +
	                    par_p8_ * temp_cu;

	// partial_out2
	const double p1_term = par_p1_ +
	                       par_p2_ * temp_ +
	                       par_p3_ * temp_sq +
	                       par_p4_ * temp_cu;

	const double out2 = raw_press_d * p1_term;

	// pressure nonlinear correction
	const double nonlinear_term = raw_press_sq * (par_p9_ + par_p10_ * temp_) +
	                               raw_press_cu * par_p11_;

	// Final compensated pressure
	press_ = out1 + out2 + nonlinear_term;
}

void ReadBmp390l::NotifyFromIsr(TransferResult result) {
	if ((task_handle_ == nullptr) ||
	    (transfer_result_ != TransferResult::kPending)) {
		return;
	}

	transfer_result_ = result;
	transfer_phase_ = TransferPhase::kIdle;

	BaseType_t higher_priority_task_woken = pdFALSE;
	vTaskNotifyGiveFromISR(task_handle_, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

void ReadBmp390l::StartDataReceiveFromIsr() {
	transfer_phase_ = TransferPhase::kData;
	HAL_StatusTypeDef status = HAL_SPI_Receive_IT(
			bmp390l_spi_, rx_buf_, kRuntimeReceiveLength);
	if (status != HAL_OK) {
		HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);
		NotifyFromIsr(TransferResult::kError);
	}
}

void ReadBmp390l::SpiTransmitCompleteCallback(SPI_HandleTypeDef* spi_handle) {
	if ((instance_ != nullptr) && (spi_handle == instance_->bmp390l_spi_)) {
		if ((instance_->transfer_result_ == TransferResult::kPending) &&
		    (instance_->transfer_phase_ == TransferPhase::kAddress)) {
			instance_->StartDataReceiveFromIsr();
		}
	}
}

void ReadBmp390l::SpiReceiveCompleteCallback(SPI_HandleTypeDef* spi_handle) {
	if ((instance_ != nullptr) && (spi_handle == instance_->bmp390l_spi_)) {
		if ((instance_->transfer_result_ == TransferResult::kPending) &&
		    (instance_->transfer_phase_ == TransferPhase::kData)) {
			HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);
			instance_->NotifyFromIsr(TransferResult::kComplete);
		}
	}
}

void ReadBmp390l::SpiTransferCompleteCallback(SPI_HandleTypeDef* spi_handle) {
	if ((instance_ != nullptr) && (spi_handle == instance_->bmp390l_spi_)) {
		if (instance_->transfer_result_ == TransferResult::kPending) {
			HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);
			instance_->NotifyFromIsr(TransferResult::kComplete);
		}
	}
}

void ReadBmp390l::SpiErrorCallback(SPI_HandleTypeDef* spi_handle) {
	if ((instance_ != nullptr) && (spi_handle == instance_->bmp390l_spi_)) {
		HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin, GPIO_PIN_SET);
		instance_->NotifyFromIsr(TransferResult::kError);
	}
}

bool ReadBmp390l::TickReached(TickType_t now, TickType_t deadline) {
	return static_cast<int32_t>(now - deadline) >= 0;
}

void ReadBmp390l::Run() {
	BaroData baro_data = {};
	bool status = Bmp390lInit();
	Publisher<BaroData> bmp390l_pub(TopicID::BMP390L);
//	int blink_counter = 0;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);
	const TickType_t runtime_transfer_timeout =
			pdMS_TO_TICKS(kRuntimeTransferTimeoutMs);
	osDelay(100);
	task_handle_ = xTaskGetCurrentTaskHandle();
	TickType_t next_release = xTaskGetTickCount();
	ConfigureEventMetrics();
    /* Infinite loop */
    for (;;) {
		if (transfer_result_ == TransferResult::kPending) {
			const BaseType_t notified =
					ulTaskNotifyTake(pdTRUE, runtime_transfer_timeout);
			BeginMetricsCycle();
			if (notified != 0U) {
				if (CompletePressureTemperatureRead(&baro_data)) {
					bmp390l_pub.publish(baro_data);
				}
				transfer_result_ = TransferResult::kIdle;
			} else if (transfer_result_ == TransferResult::kPending) {
				HAL_GPIO_WritePin(BARO_CS_GPIO_Port, BARO_CS_Pin,
						GPIO_PIN_SET);
				(void)HAL_SPI_Abort(bmp390l_spi_);
				transfer_phase_ = TransferPhase::kIdle;
				transfer_result_ = TransferResult::kIdle;
			}
			EndMetricsCycle();
			continue;
		}

		const TickType_t now = xTaskGetTickCount();
		if (!TickReached(now, next_release)) {
			vTaskDelay(next_release - now);
			continue;
		}

		BeginMetricsCycle();
    	if(status){
//    		if (++blink_counter >= 20) {
//				blink_counter = 0;
//				UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//				uint32_t used = 1062 - highWaterMark * sizeof(StackType_t);
//				DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//				used, highWaterMark * sizeof(StackType_t), 1062);
//    		}
    		(void)StartPressureTemperatureRead();
//    		DEBUG_PRINT("Press: %g, Temp: %g\n", press_, temp_);
    	}
		next_release = xTaskGetTickCount() + xFrequency;
		EndMetricsCycle();
    }
}

extern "C" void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef* hspi) {
	ReadBmp390l::SpiTransferCompleteCallback(hspi);
}

extern "C" void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
	ReadBmp390l::SpiTransmitCompleteCallback(hspi);
}

extern "C" void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
	ReadBmp390l::SpiReceiveCompleteCallback(hspi);
}

extern "C" void HAL_SPI_ErrorCallback(SPI_HandleTypeDef* hspi) {
	ReadBmp390l::SpiErrorCallback(hspi);
}
