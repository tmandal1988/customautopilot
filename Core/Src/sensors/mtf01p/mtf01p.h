/*
 * ReadMtf01p.h
 *
 *  Created on: Jul 26, 2025
 *      Author: tanmay
 */

#ifndef SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_
#define SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include "FreeRTOS.h"
#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "pubsub/publisher.h"
#include "messages/mtf01p_data.h"
#include "constants.h"
#include "debug.h"
#include "task.h"

#ifndef MTF01P_DEBUG_PRINT_ENABLE
#define MTF01P_DEBUG_PRINT_ENABLE 0
#endif

class ReadMtf01p : public TaskBase {
public:
	ReadMtf01p(UART_HandleTypeDef* huart);
	virtual ~ReadMtf01p(){
	}

	void Run() override;

	static void RxEvent(UART_HandleTypeDef* huart, uint16_t size);
	static void UartError(UART_HandleTypeDef* huart);

	static ReadMtf01p* read_mtf01p_instance_handle_;

private:
	static constexpr uint16_t READ_INTERVAL_MS 				= 10; // 100Hz
	static constexpr uint16_t MAX_BUFF_SIZE 				= 512;
	static constexpr uint8_t MTF_SYNC_CHAR					= 0xEF;
	static constexpr uint8_t MTF_DEV_ID 					= 0x0F;
	static constexpr uint8_t MTF_SYS_ID 					= 0x00;
	static constexpr uint8_t MTF_MSG_ID 					= 0x51;
	static constexpr uint8_t MTF_MSG_SIZE 		 			= 20;
	static constexpr uint8_t MICOLINK_MAX_PAYLOAD_LEN     	= 64;
	static constexpr uint8_t MICOLINK_MAX_LEN             	= MICOLINK_MAX_PAYLOAD_LEN + 7;

	static constexpr uint16_t kParseBudgetBytes = 64;
	static constexpr uint32_t kLostCommTimeoutMs = 5000;
	static constexpr uint32_t kTransportRetryIntervalMs = 1000;
	static constexpr uint32_t kRxReadyEvent = 1U << 0;
	static constexpr uint32_t kRxErrorEvent = 1U << 1;
	static constexpr uint32_t kAllTaskEvents =
			kRxReadyEvent | kRxErrorEvent;
#if MTF01P_DEBUG_PRINT_ENABLE
	static constexpr uint32_t kDebugPrintIntervalMs = 100;
#endif
	static_assert((MAX_BUFF_SIZE & (MAX_BUFF_SIZE - 1U)) == 0U,
			"MTF01P DMA ring size must be a power of two");

	uint8_t rx_buffer_[MAX_BUFF_SIZE] = {0};
	size_t last_read_index_ = 0;

	bool new_sensor_frame_ = false;

	UART_HandleTypeDef* mtf01_uart_;  // UART handle to receive data from GPS

    // Message Structure Definition

	struct PACKED MicroLinkMsg{
		uint8_t head;
		uint8_t dev_id;
		uint8_t sys_id;
		uint8_t msg_id;
		uint8_t seq;
		uint8_t len;
		uint8_t payload[MICOLINK_MAX_PAYLOAD_LEN];
		uint8_t checksum;

		uint8_t status;
		uint8_t payload_cnt;
	}msg_;

	struct PACKED SensorPayload
	{
	    uint32_t  time_ms;		    // System time in ms
	    uint32_t  distance;		    // distance(mm), 0 Indicates unavailable
	    uint8_t   strength;	        // signal strength
	    uint8_t   precision;	    // distance precision
	    uint8_t   dis_status;	    // distance status
	    uint8_t   reserved1;	    // reserved
	    int16_t   flow_vel_x;	    // optical flow velocity in x
	    int16_t   flow_vel_y;	    // optical flow velocity in y
	    uint8_t   flow_quality;	    // optical flow quality
	    uint8_t   flow_status;	    // optical flow status
	    uint16_t  reserved2;	    // reserved
	}sensor_payload_;
	static_assert(sizeof(SensorPayload) == MTF_MSG_SIZE,
			"MTF01P payload size must match the protocol frame length");

	Mtf01pData mtf01p_data_;
	TaskHandle_t task_handle_ = nullptr;
	std::atomic<uint32_t> rx_dma_last_position_{0U};
	std::atomic<uint32_t> rx_produced_bytes_{0U};
	uint32_t rx_consumed_bytes_ = 0U;
	uint32_t last_health_produced_ = 0U;
	bool rx_backlog_pending_ = false;
	bool rx_restart_pending_ = false;
	TickType_t next_rx_restart_due_ = 0U;
	TickType_t next_health_due_ = 0U;
#if MTF01P_DEBUG_PRINT_ENABLE
	TickType_t next_debug_print_due_ = 0U;
#endif

	void FlushUartDataRegister();
	bool StartRxDma();
	void RecoverRxDma(TickType_t now);
	TickType_t ComputeWaitTicks(TickType_t now) const;
	bool ProcessMicrolinkFrame(uint16_t budget);
	bool ParseChar(uint8_t data);
	bool ComputeCheckSum();
	bool IsExpectedSensorFrame() const;
	void ResetParser();
	void NotifyTaskFromIsr(uint32_t event);
	static bool DeadlineReached(TickType_t now, TickType_t deadline);
	static TickType_t TicksUntil(TickType_t now, TickType_t deadline);
	static uint16_t NormalizeDmaPosition(uint16_t size);
#if MTF01P_DEBUG_PRINT_ENABLE
	void DebugPrintData(const Mtf01pData& data, TickType_t now);
	void DebugPrintNoRxBytes(TickType_t now);
#endif

};




#endif /* SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_ */
