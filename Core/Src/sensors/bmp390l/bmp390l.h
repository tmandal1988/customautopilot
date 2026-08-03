/*
 * bmp390l.h
 *
 *  Created on: Apr 1, 2025
 *      Author: tanmay
 */

#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "messages/baro_data.h"
#include "pubsub/publisher.h"
#include "debug.h"
#include "task.h"

class ReadBmp390l : public TaskBase {
public:
	ReadBmp390l(SPI_HandleTypeDef* spiHandle);
	bool Bmp390lInit();

	void Run() override;

	// Called only by the central HAL SPI callback dispatcher.
	static void SpiTransmitCompleteCallback(SPI_HandleTypeDef* spi_handle);
	static void SpiReceiveCompleteCallback(SPI_HandleTypeDef* spi_handle);
	static void SpiTransferCompleteCallback(SPI_HandleTypeDef* spi_handle);
	static void SpiErrorCallback(SPI_HandleTypeDef* spi_handle);

private:
	enum class TransferResult : uint8_t {
		kIdle,
		kPending,
		kComplete,
		kError,
	};

	enum class TransferPhase : uint8_t {
		kIdle,
		kAddress,
		kData,
	};

	static constexpr uint16_t READ_INTERVAL_MS = 16; // Go slightly faster than 50Hz
	// Tx and Rx max delay (ms) in polling mode
	static constexpr uint8_t baro_spi_tx_delay_ms_ = 1;
	static constexpr uint8_t baro_spi_rx_delay_ms_ = 1;
	static constexpr uint8_t kRuntimeReceiveLength = 7;
	static constexpr uint32_t kRuntimeTransferTimeoutMs = 2;
	static constexpr uint8_t spi_retry_ = 5;
	// Valid CHIP ID
	static constexpr uint8_t CHIP_ID = 0x60;

	// This SPI is used to communicate with BMP390L
	SPI_HandleTypeDef* bmp390l_spi_;

	//Member functions for single read write
	bool WriteSingleRegister(uint8_t reg_addr, uint8_t value);
	bool ReadSingleRegister(uint8_t reg_addr);
	bool ReadMultipleRegisters(uint8_t reg_addr, uint8_t num_bytes);
	void Bmp390lGetPressAndTemp();
	bool StartPressureTemperatureRead();
	bool CompletePressureTemperatureRead(BaroData* baro_data);
	void CompensatePressureAndTemperature();
	void StartDataReceiveFromIsr();
	void NotifyFromIsr(TransferResult result);
	static bool TickReached(TickType_t now, TickType_t deadline);

	// Tx and Rx Buffer for Single read/write
	uint8_t tx_buf_[30];
	uint8_t rx_buf_[30];

	////////////////IMPORTANT REGISTERS///////////////////
	static constexpr uint8_t REG_ADDR_CHIP_ID = 0x00;
	static constexpr uint8_t REG_ADDR_PWR_CTRL = 0x1B;
	static constexpr uint8_t REG_ADDR_OSR = 0x1C;
	static constexpr uint8_t REG_ADDR_ODR = 0x1D;
	static constexpr uint8_t REG_ADDR_CONFIG = 0x1F;
	static constexpr uint8_t REG_ADDR_CALIB_DATA = 0x31;
	static constexpr uint8_t REG_ADDR_DATA = 0x04;
	///////////////REG Value//////////////////////////////
	//For Drone Normal Standard	Resolution x8 x1 2 50 ////
	static constexpr uint8_t REG_VAL_PWR_CTRL = 0b00110011; // Enable both pressure and temperature and normal mode
	static constexpr uint8_t REG_VAL_OSR = 0b00000011; //osr_p x8, osr_t x1
	static constexpr uint8_t REG_VAL_ODR = 0x03; //50 Hz
	static constexpr uint8_t REG_VAL_CONFIG = 0b00000100; // Filter coefficient 2

	//Calibration Params from BMP390L NVM
	uint16_t NVM_PAR_T1;
	double par_t1_;
	uint16_t NVM_PAR_T2;
	double par_t2_;
	int8_t NVM_PAR_T3;
	double par_t3_;

	int16_t NVM_PAR_P1;
	double par_p1_;
	int16_t NVM_PAR_P2;
	double par_p2_;
	int8_t NVM_PAR_P3;
	double par_p3_;
	int8_t NVM_PAR_P4;
	double par_p4_;
	uint16_t NVM_PAR_P5;
	double par_p5_;
	uint16_t NVM_PAR_P6;
	double par_p6_;
	int8_t NVM_PAR_P7;
	double par_p7_;
	int8_t NVM_PAR_P8;
	double par_p8_;
	int16_t NVM_PAR_P9;
	double par_p9_;
	int8_t NVM_PAR_P10;
	double par_p10_;
	int8_t NVM_PAR_P11;
	double par_p11_;

	uint32_t raw_press_;
	uint32_t raw_temp_;

	double press_;
	double temp_;

	static ReadBmp390l* instance_;

	TaskHandle_t task_handle_ = nullptr;
	volatile TransferResult transfer_result_ = TransferResult::kIdle;
	volatile TransferPhase transfer_phase_ = TransferPhase::kIdle;
};
