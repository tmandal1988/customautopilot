/*
 * ReadMtf01p.h
 *
 *  Created on: Jul 26, 2025
 *      Author: tanmay
 */

#ifndef SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_
#define SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_

#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "pubsub/publisher.h"
#include "messages/mtf01p_data.h"
#include "constants.h"
#include "debug.h"

#include <cstring>  // Include this header for memcpy

class ReadMtf01p : public TaskBase {
public:
	ReadMtf01p(UART_HandleTypeDef* huart);
	virtual ~ReadMtf01p(){
	}

	void Run() override;

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

	static constexpr int kHeartbeatIntervalCount = 500 / READ_INTERVAL_MS;
	static constexpr int kLostCommIntervalCount = 5000 / READ_INTERVAL_MS;

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

	Mtf01pData mtf01p_data_;
	uint64_t lost_comm_count_ = {0};
	bool restart_comm_ = {false};

	void FlushUartDataRegister();
	void ProcessMicrolinkFrame();
	bool ParseChar(uint8_t data);
	bool ComputeCheckSum();

};




#endif /* SRC_SENSORS_ReadMtf01p_ReadMtf01p_H_ */
