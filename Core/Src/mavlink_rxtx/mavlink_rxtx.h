/*
 * mavlink_rxtx.h
 *
 *  Created on: May 10, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "pubsub/subscriber.h"
#include "pubsub/publisher.h"
#include "messages/ekf_data.h"
#include "debug.h"
#include "pin_defines.h"
#include "constants.h"

extern "C" {
#include "mavlink/common/mavlink.h"
}

class MavlinkRxTx : public TaskBase {
public:
	MavlinkRxTx(UART_HandleTypeDef* huart);
    void Run() override;

    // Sets the tx_complete_ flag
	static void TxCompleted();

	static MavlinkRxTx* mavlink_rxtx_instance_handle_;
private:
	UART_HandleTypeDef* mavlink_uart_;  // UART handle for Mavlink messages
    static constexpr uint16_t READ_INTERVAL_MS = 200; // 5Hz
    static constexpr int kHeartbeatIntervalCount = 1000 / READ_INTERVAL_MS;

    static constexpr size_t kMavBuffSize = 512;
    uint8_t uart4_dma_rx_buffer_[kMavBuffSize];

    // TX buffer constants
    static constexpr size_t kTxBufferSize = 1024;

    Subscriber<EkfData> ekf_sub_ = Subscriber<EkfData>(TopicID::EKF);
    EkfData ekf_data = {0};

    //Double buffering for MAVLink TX
    uint8_t tx_buffer_a_[kTxBufferSize];
    uint8_t tx_buffer_b_[kTxBufferSize];
    uint8_t* active_buffer_ = tx_buffer_a_;
    uint8_t* sending_buffer_ = tx_buffer_b_;
    size_t active_index_ = 0;

    mavlink_message_t msg;
    mavlink_status_t status;
    size_t read_index_ = 0;

    size_t write_index_ = 0;
    bool tx_complete_ = true;
    void SetTxReadyFlag();
    void ParseReadyMavlinkMessages();
    void HandleMavlinkMessage(mavlink_message_t* msg);
    void FlushUartDataRegister();
    void SwapBuffers();

    uint8_t base_mode_ = MAV_MODE_MANUAL_DISARMED;
    mavlink_message_t tx_msg_;  // Reusable for all outgoing messages
};
