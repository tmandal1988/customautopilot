/*
 * mavlink_rxtx.cc
 *
 *  Created on: May 10, 2025
 *      Author: tanmay
 */

#include "mavlink_rxtx.h"

// Definition of the static member variable
MavlinkRxTx* MavlinkRxTx::mavlink_rxtx_instance_handle_ = nullptr;

extern UART_HandleTypeDef huart4;

MavlinkRxTx mavlink_rxtx_task_instance_(&huart4);

MavlinkRxTx::MavlinkRxTx(UART_HandleTypeDef* huart):
TaskBase("MavlinkRxTxTask", 4096, osPriorityNormal),
mavlink_uart_(huart){
	mavlink_rxtx_instance_handle_ = this;
}

void MavlinkRxTx::FlushUartDataRegister(){
    // Check if there is data in the data register by reading the status register
    while (__HAL_UART_GET_FLAG(mavlink_uart_, UART_FLAG_RXNE)) {
        // Read the data register to clear the RXNE flag
        volatile uint8_t temp = (uint8_t)(mavlink_uart_->Instance->RDR & 0xFF);
        (void)temp; // Use (void) to prevent compiler warnings about unused variable
    }

    // Clear any other relevant flags (optional, depending on your error handling needs)
    __HAL_UART_CLEAR_OREFLAG(mavlink_uart_); // Clear overrun error flag, if set
}

void MavlinkRxTx::SwapBuffers() {
  std::swap(active_buffer_, sending_buffer_);
  active_index_ = 0;
}

void MavlinkRxTx::Run() {
  DEBUG_PRINT("Starting Mavlink Module\n");

  TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t loop_frequency = pdMS_TO_TICKS(READ_INTERVAL_MS);  // 1000Hz

  // Initialize the xLastWakeTime variable with the current time.
  last_wake_time = xTaskGetTickCount();

  int heartbeat_counter = 0;

  HAL_UART_Receive_DMA(mavlink_uart_, uart4_dma_rx_buffer_, kMavBuffSize);
  osDelay(200);
  for (;;) {
    // Periodic heartbeat message every 1 second
	if (++heartbeat_counter >= kHeartbeatIntervalCount) {
		heartbeat_counter = 0;
		mavlink_msg_heartbeat_pack(
			1, 1, &tx_msg_,
			MAV_TYPE_QUADROTOR,        // vehicle type (or FIXED_WING, GROUND_ROVER, etc.)
			MAV_AUTOPILOT_GENERIC,
			base_mode_,  // base_mode (see below)
			0,
			MAV_STATE_STANDBY);        // system_status

		if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
			active_index_ += mavlink_msg_to_send_buffer(
			&active_buffer_[active_index_], &tx_msg_);
		}

		mavlink_msg_global_position_int_pack(
			1, 	// system_id
			1,  //component_id
			&tx_msg_,
			xTaskGetTickCount() * portTICK_PERIOD_MS,  // timestamp in ms
			static_cast<int32_t>(37.34318394406419 * 1e7),
			static_cast<int32_t>(-122.06617112309152 * 1e7),
			static_cast<int32_t>(72.3 * 1000),
			0,
			0,
			0,
			0,
			UINT16_MAX);

		if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
			active_index_ += mavlink_msg_to_send_buffer(
			&active_buffer_[active_index_], &tx_msg_);
		}

		mavlink_msg_gps_raw_int_pack(
		    1, 1,  // system_id, component_id
		    &tx_msg_,
		    xTaskGetTickCount() * portTICK_PERIOD_MS,
		    3,  // fix_type
		    static_cast<int32_t>(37.34318394406419 * 1e7),
		    static_cast<int32_t>(-122.06617112309152 * 1e7),
		    static_cast<int32_t>(72.3 * 1000),
		    150,  // HDOP × 100
		    200,  // VDOP × 100
		    0,    // velocity
		    0,    // course over ground
		    25,   // satellites_visible
		    0,    // alt_ellipsoid
		    0,    // h_acc
		    0,    // v_acc
		    0,    // vel_acc
		    0,    // hdg_acc ← MISSING ARG ADDED HERE
		    0);   // yaw

		if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
		    active_index_ += mavlink_msg_to_send_buffer(
		        &active_buffer_[active_index_], &tx_msg_);
		}
	}

    // TODO: Add other messages (e.g., GPS, attitude)
    // mavlink_msg_xxx_pack(...);
    // if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
    //   active_index_ += mavlink_msg_to_send_buffer(&active_buffer_[active_index_], &msg);
    // }
	//Add attitude message
	if(ekf_sub_.copy(ekf_data)){
		mavlink_msg_attitude_pack(
		    1,  // system_id
		    1,  // component_id
		    &tx_msg_,
		    xTaskGetTickCount() * portTICK_PERIOD_MS,  // timestamp in ms
		    ekf_data.euler_rad[0],
			ekf_data.euler_rad[1],
			ekf_data.euler_rad[2],
		    ekf_data.bias_corr_body_rates_radps[0],
			ekf_data.bias_corr_body_rates_radps[1],
			ekf_data.bias_corr_body_rates_radps[2]);

		if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
		  active_index_ += mavlink_msg_to_send_buffer(
		      &active_buffer_[active_index_], &tx_msg_);
		}
	}

    // Transmit if DMA is ready and there's data in the buffer
	if(active_index_ > 0 && mavlink_uart_->hdmatx->State == HAL_DMA_STATE_READY &&
		__HAL_UART_GET_FLAG(mavlink_uart_, UART_FLAG_TC)){
		size_t send_len = active_index_;
		SwapBuffers();
		HAL_UART_Transmit_DMA(mavlink_uart_, sending_buffer_, send_len);
	}

    ParseReadyMavlinkMessages();
    vTaskDelayUntil(&last_wake_time, loop_frequency);
  }
}


void MavlinkRxTx::ParseReadyMavlinkMessages() {
	write_index_ = (kMavBuffSize - __HAL_DMA_GET_COUNTER(huart4.hdmarx)) % kMavBuffSize;
	while (read_index_ != write_index_) {
		uint8_t byte = uart4_dma_rx_buffer_[read_index_];

		if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status)) {
			HandleMavlinkMessage(&msg);
		}

		read_index_ = (read_index_ + 1) % kMavBuffSize;
	}
}


void MavlinkRxTx::HandleMavlinkMessage(mavlink_message_t* msg) {
    switch (msg->msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
            mavlink_heartbeat_t hb;
            mavlink_msg_heartbeat_decode(msg, &hb);

//            DEBUG_PRINT("Received HEARTBEAT from system %d, component %d\n", msg->sysid, msg->compid);
//            DEBUG_PRINT("  Type: %u\n", hb.type);
//            DEBUG_PRINT("  Autopilot: %u\n", hb.autopilot);
//            DEBUG_PRINT("  Base Mode: 0x%02X\n", hb.base_mode);
//            DEBUG_PRINT("  Custom Mode: %lu\n", static_cast<unsigned long>(hb.custom_mode));
//            DEBUG_PRINT("  System Status: %u\n", hb.system_status);
//            DEBUG_PRINT("  MAVLink Version: %u\n", hb.mavlink_version);
            break;
        }
        case MAVLINK_MSG_ID_COMMAND_LONG: {
        	mavlink_command_long_t cmd;
        	mavlink_msg_command_long_decode(msg, &cmd);
        	if(cmd.command == MAV_CMD_COMPONENT_ARM_DISARM){
        		if (static_cast<int>(cmd.param1) == 1){
        			base_mode_ = MAV_MODE_MANUAL_ARMED;
        		}else{
        			base_mode_ = MAV_MODE_MANUAL_DISARMED;
        		}
        	}

        	//Add ACK to the tx buffer
        	mavlink_msg_command_ack_pack(
        	1,
			1,
			&tx_msg_, cmd.command, MAV_RESULT_ACCEPTED, 0, 0, msg->sysid, msg->compid);
			if (active_index_ + MAVLINK_MAX_PACKET_LEN < kTxBufferSize) {
				active_index_ += mavlink_msg_to_send_buffer(
				&active_buffer_[active_index_], &tx_msg_);
			}
			break;
        }

        // Handle more messages...
    }
}

void MavlinkRxTx::SetTxReadyFlag(){
	tx_complete_ = true;
}

void MavlinkRxTx::TxCompleted(){
	if (mavlink_rxtx_instance_handle_ != nullptr){
		mavlink_rxtx_instance_handle_->SetTxReadyFlag();
	}
}
