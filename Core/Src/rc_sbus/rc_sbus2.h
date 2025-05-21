/*
 * rc_sbus2.h
 *
 *  Created on: Apr 7, 2025
 *      Author: tanmay
 */

#pragma once

#include "debug.h"
#include "pin_defines.h"
#include "task_manager/task_base.h"
#include "pubsub/publisher.h"
#include "messages/rc_channels.h"

#include <stdint.h>
#include <cstring>

class RcSbus2 : public TaskBase {
public:
    RcSbus2(UART_HandleTypeDef* huart);
    virtual ~RcSbus2(){}

    static void ReceivedNewSbusFrame(uint8_t id);

    static RcSbus2* rc_sbus2_instance_handle_;
    void Run() override;

private:
    UART_HandleTypeDef* rc_uart_;

    static constexpr uint16_t READ_INTERVAL_MS = 20; // 50Hz

    static constexpr uint8_t FRAME_SIZE     = 25;
    static constexpr uint8_t DMA_RX_SIZE    = (FRAME_SIZE * 2);
    static constexpr uint16_t RING_BUFFER_SIZE = (DMA_RX_SIZE * 2);

    static constexpr uint8_t SBUS_START_BYTE = 0x0F;
    static constexpr uint8_t SBUS_END_BYTE   = 0x00;

    bool new_rc_sbus2_frame_ = false;
    uint8_t rc_sbus2_ring_buffer_[RING_BUFFER_SIZE] = {0};
    uint16_t write_pos_ = 0;
    uint16_t read_pos_ = 0;

    RcChannels rc_channels_;

    void RcSbusInit();
    void RestartDma();
    void DecodeSbusFrame();
};

//#ifdef __cplusplus
//extern "C" {
//#endif
//    void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
//#ifdef __cplusplus
//}
//#endif
