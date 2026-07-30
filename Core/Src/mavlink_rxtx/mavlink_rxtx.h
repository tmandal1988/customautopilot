/*
 * mavlink_rxtx.h
 *
 *  Created on: May 10, 2025
 *      Author: tanmay
 */

#pragma once

#include <array>
#include <atomic>
#include <utility>

#include "task_manager/task_base.h"
#include "pubsub/subscriber.h"
#include "pubsub/publisher.h"
#include "messages/ekf_data.h"
#include "messages/gps_data.h"
#include "messages/fcs_debug_data.h"
#include "messages/mavlink_data.h"
#include "parameters/parameter_types.h"
#include "mavlink_transport_state.h"
#include "pin_defines.h"
#include "constants.h"

extern "C" {
#include "mavlink/common/mavlink.h"
}

class MavlinkRxTx : public TaskBase {
public:
	MavlinkRxTx(UART_HandleTypeDef* huart);
    void Run() override;

    static void RxEvent(UART_HandleTypeDef* huart, uint16_t size);
    static void UartError(UART_HandleTypeDef* huart);
    static void TxCompleted(UART_HandleTypeDef* huart);

	static MavlinkRxTx* mavlink_rxtx_instance_handle_;
private:
	UART_HandleTypeDef* mavlink_uart_;  // UART handle for Mavlink messages
    static constexpr uint16_t kTelemetryIntervalMs = 200U;  // 5 Hz
    static constexpr uint16_t kHeartbeatIntervalMs = 1000U;
    static constexpr uint16_t kParameterListIntervalMs = 50U;
    static constexpr uint16_t kTransportRetryIntervalMs = 10U;
    static constexpr uint8_t kSysId       = 1;
    static constexpr uint8_t kCompId      = MAV_COMP_ID_AUTOPILOT1;

    static constexpr size_t kMavBuffSize = 512;
    static constexpr size_t kRxParseBudget = kMavBuffSize / 2U;
    alignas(32) uint8_t uart4_dma_rx_buffer_[kMavBuffSize]{};

    // TX buffer constants
    static constexpr size_t kTxBufferSize = 1024;
    static constexpr size_t kPendingParameterReplyCapacity = 32;
    static constexpr size_t kMaxParameterValueFrameLength =
        MAVLINK_MSG_ID_PARAM_VALUE_LEN + MAVLINK_NUM_NON_PAYLOAD_BYTES +
        MAVLINK_SIGNATURE_BLOCK_LEN;
    static_assert((kMavBuffSize & (kMavBuffSize - 1U)) == 0U);
    static_assert(kRxParseBudget <= kMavBuffSize);
    static_assert(kTxBufferSize <= UINT16_MAX);
    static_assert(kMaxParameterValueFrameLength <= MAVLINK_MAX_PACKET_LEN);
    static_assert(std::atomic<uint32_t>::is_always_lock_free);

    static constexpr uint32_t kRxReadyEvent = 1UL << 0U;
    static constexpr uint32_t kRxErrorEvent = 1UL << 1U;
    static constexpr uint32_t kTxCompleteEvent = 1UL << 2U;
    static constexpr uint32_t kTxErrorEvent = 1UL << 3U;
    static constexpr uint32_t kAllTaskEvents =
        kRxReadyEvent | kRxErrorEvent | kTxCompleteEvent | kTxErrorEvent;

	enum class HomeState {
		NOTHOMED = 0,
		HOMED
	};

	HomeState home_state_ = HomeState::NOTHOMED;

    Subscriber<EkfData> ekf_sub_ = Subscriber<EkfData>(TopicID::EKF);
    EkfData ekf_data_ = {0};

    Subscriber<GpsData> gps_sub_ = Subscriber<GpsData>(TopicID::UBLOXM9N);
    GpsData gps_data_ = {0};

    Subscriber<FcsDebugData> fcs_debug_sub_ = Subscriber<FcsDebugData>(TopicID::FCSDEBUG);
    FcsDebugData fcs_debug_data_ = {0};

    double last_valid_lat_deg_ =  0.0;
    double last_valid_lon_deg_ = 0.0;
    double last_valid_wgs84_alt_m_ =  0.0;

    double home_lat_deg_ = 0.0;
    double home_lon_deg_ = 0.0;
    double home_wgs84_alt_m_ = 0.0;

    bool gps_valid_ = false;

    //Double buffering for MAVLink TX
    alignas(32) uint8_t tx_buffer_a_[kTxBufferSize]{};
    alignas(32) uint8_t tx_buffer_b_[kTxBufferSize]{};
    uint8_t* active_buffer_ = tx_buffer_a_;
    uint8_t* sending_buffer_ = tx_buffer_b_;
    size_t active_index_ = 0;
    size_t sending_length_ = 0;
    bool active_buffer_has_parameter_response_ = false;
    bool sending_buffer_has_parameter_response_ = false;
    bool tx_resend_pending_ = false;
    bool tx_replay_wait_pending_ = false;
    bool tx_retry_pending_ = false;
    TickType_t next_tx_retry_due_ = 0U;

    mavlink_message_t msg{};
    mavlink_status_t status{};
    TaskHandle_t mavlink_task_handle_ = nullptr;
    std::atomic<uint32_t> rx_dma_last_position_{0U};
    std::atomic<uint32_t> rx_produced_bytes_{0U};
    uint32_t rx_consumed_bytes_ = 0U;
    bool rx_backlog_pending_ = false;
    bool rx_restart_pending_ = false;
    TickType_t next_rx_restart_due_ = 0U;

    bool StartRxDma();
    void RecoverRxDma(TickType_t now);
    bool ParseReadyMavlinkMessages();
    void ResetMavlinkParser();
    void HandleMavlinkMessage(mavlink_message_t* msg);
    void NotifyTaskFromIsr(uint32_t event);

    void BuildHeartbeat();
    void BuildGlobalPosition(uint32_t now_ms);
    void BuildGps(uint32_t now_ms);
    void BuildAttitude(uint32_t now_ms);

    enum class ParameterTxResult : uint8_t {
      Queued = 0U,
      TxFull,
      InvalidParameter,
    };

    bool QueueMessage(const mavlink_message_t& msg);
    ParameterTxResult TryQueueParameterValue(uint16_t index, float value,
                                             uint16_t parameter_count);
    ParameterTxResult TryQueueCurrentParameterValue(
        uint16_t index, uint16_t parameter_count);
    bool EnqueueCurrentParameterReply(uint16_t index);
    void RequestParameterList(TickType_t now);
    void ProcessOneParameterResponse(TickType_t now);
    void SendBufferedDataIfReady(TickType_t now);
    TickType_t ComputeWaitTicks(TickType_t now,
                                TickType_t next_telemetry,
                                TickType_t next_heartbeat) const;

    /// Generic wrapper that packs a MAVLink message then queues it.
    template<typename PackFunc, typename... Args>
	inline bool PackAndQueue(PackFunc&& pack, Args&&... args)
	{
		pack(std::forward<Args>(args)...);
		return QueueMessage(tx_msg_);
	}

    uint8_t base_mode_ = MAV_MODE_MANUAL_DISARMED;
    mavlink_message_t tx_msg_;  // Reusable for all outgoing messages

    MavlinkData mavlink_data_ = {0};
    bool new_mavlink_data_ = false;

    uint32_t next_parameter_update_token_ = 1U;
    parameters::ParameterUpdateCompletion pending_parameter_completion_{};
    bool has_pending_parameter_completion_ = false;

    struct PendingParameterReply {
      float value;
      uint16_t index;
      uint16_t parameter_count;
    };
    static_assert(sizeof(PendingParameterReply) == 8U);
    static_assert((kPendingParameterReplyCapacity &
                   (kPendingParameterReplyCapacity - 1U)) == 0U);
    std::array<PendingParameterReply,
               kPendingParameterReplyCapacity> pending_parameter_replies_{};
    uint32_t pending_parameter_reply_head_ = 0U;
    uint32_t pending_parameter_reply_tail_ = 0U;
    mavlink_transport::ParameterListCursor parameter_list_cursor_{};

    uint32_t rx_overrun_count_ = 0U;
    uint32_t rx_dropped_byte_count_ = 0U;
    uint32_t rx_error_count_ = 0U;
    uint32_t rx_restart_count_ = 0U;
    uint32_t tx_start_error_count_ = 0U;
    uint32_t tx_dma_error_count_ = 0U;
    uint32_t tx_resend_count_ = 0U;
    uint32_t pending_parameter_reply_overflow_count_ = 0U;
    uint32_t parameter_response_error_count_ = 0U;
    uint32_t parameter_list_error_count_ = 0U;

};
