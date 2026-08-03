/*
 * rc_sbus.h
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

#include <atomic>
#include <stdint.h>
#include <cstring>

class RcSbus : public TaskBase {
public:
    RcSbus(UART_HandleTypeDef* huart);
    virtual ~RcSbus(){}

#if RTOS_METRICS_ENABLE
    struct Diagnostics {
        uint32_t ready_events = 0U;
        uint32_t error_events = 0U;
        uint32_t valid_frames = 0U;
        uint32_t stale_publishes = 0U;
        uint32_t max_bytes_processed = 0U;
        uint32_t max_frames_per_dispatch = 0U;
        uint32_t max_publish_cycles = 0U;
        uint32_t max_context_switch_delta = 0U;
        uint32_t healthy_frames = 0U;
        uint32_t lost_frames = 0U;
        uint32_t failsafe_frames = 0U;
        uint8_t last_status = 0U;
    };
    static bool CaptureDiagnostics(Diagnostics* output);
#endif

    static void ReceivedNewSbusFrame(uint8_t id);
    static void RxEvent(UART_HandleTypeDef* huart, uint16_t size);
    static void UartError(UART_HandleTypeDef* huart);

    static RcSbus* rc_sbus_instance_handle_;
    void Run() override;

private:
    UART_HandleTypeDef* rc_uart_;

    static constexpr uint16_t READ_INTERVAL_MS = 20; // 50Hz

    static constexpr uint8_t FRAME_SIZE     = 25;
    static constexpr uint16_t RING_BUFFER_SIZE = 128;
    static constexpr uint16_t kParseBudgetBytes = FRAME_SIZE * 3U;
    static constexpr uint32_t kLostCommTimeoutMs = 100U;
    static constexpr uint32_t kTransportRetryIntervalMs = 20U;
    static constexpr uint8_t kLedToggleFrameDivider = 4U;

    static constexpr uint8_t SBUS_START_BYTE = 0x0F;
    static constexpr uint8_t SBUS_END_BYTE   = 0x00;
    static constexpr uint32_t kRxReadyEvent = 1UL << 0;
    static constexpr uint32_t kRxErrorEvent = 1UL << 1;
    static constexpr uint32_t kAllTaskEvents =
            kRxReadyEvent | kRxErrorEvent;

    static constexpr uint16_t kMinPwmVal = 985;
    static constexpr uint16_t kMaxPwmVal = 2000;

    static constexpr uint16_t kMinRcVal = 172;
    static constexpr uint16_t kMaxRcVal = 1811;

    uint8_t rc_sbus_ring_buffer_[RING_BUFFER_SIZE] = {0};
    std::atomic<uint32_t> rx_produced_bytes_{0U};
    std::atomic<uint16_t> rx_dma_last_position_{0U};
    uint32_t rx_consumed_bytes_ = 0U;
    bool rx_backlog_pending_ = false;
    bool rx_restart_pending_ = false;
    uint8_t led_toggle_divider_ = 0U;
    TickType_t next_rx_restart_due_ = 0U;
    TickType_t next_health_due_ = 0U;
    TickType_t last_valid_frame_tick_ = 0U;
#if RTOS_METRICS_ENABLE
    uint32_t rx_ready_event_count_ = 0U;
    uint32_t rx_error_event_count_ = 0U;
    uint32_t valid_frame_count_ = 0U;
    uint32_t stale_publish_count_ = 0U;
    uint32_t max_bytes_processed_ = 0U;
    uint32_t max_frames_per_dispatch_ = 0U;
    uint32_t max_publish_cycles_ = 0U;
    uint32_t max_context_switch_delta_ = 0U;
    uint32_t healthy_frame_count_ = 0U;
    uint32_t lost_frame_count_ = 0U;
    uint32_t failsafe_frame_count_ = 0U;
    uint8_t last_frame_status_ =
            static_cast<uint8_t>(RcChannels::RxStatus::INACTIVE);
#endif

    RcChannels rc_channels_;
    TaskHandle_t task_handle_ = nullptr;

    bool StartRxDma();
    void RecoverRxDma(TickType_t now);
    void FlushUartDataRegister();
    void NotifyTaskFromIsr(uint32_t event);
    TickType_t ComputeWaitTicks(TickType_t now) const;
    bool ProcessSbusBytes(uint16_t budget);
    bool DecodeSbusFrameAt(uint32_t frame_start);
    void PublishLostIfStale(Publisher<RcChannels>& rcchannels_pub,
            TickType_t now);
    uint8_t RingByte(uint32_t absolute_index) const;
    static bool DeadlineReached(TickType_t now, TickType_t deadline);
    static TickType_t TicksUntil(TickType_t now, TickType_t deadline);
    static uint16_t NormalizeDmaPosition(uint16_t size);
    inline uint16_t ScaleToPwmRange(uint16_t channel_in) const;
};

//#ifdef __cplusplus
//extern "C" {
//#endif
//    void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);
//#ifdef __cplusplus
//}
//#endif
