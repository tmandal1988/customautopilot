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
#include "messages/gps_data.h"
#include "messages/fcs_debug_data.h"
#include "messages/mavlink_data.h"
#include "messages/mavlink_params_data.h"
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
    static constexpr uint8_t kSysId       = 1;
    static constexpr uint8_t kCompId      = 1;

    static constexpr size_t kMavBuffSize = 512;
    uint8_t uart4_dma_rx_buffer_[kMavBuffSize];

    // TX buffer constants
    static constexpr size_t kTxBufferSize = 1024;

    static constexpr double kEpsilon = 1e-9;

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

    void BuildHeartbeat();
    void BuildGlobalPosition(uint32_t now_ms);
    void BuildGps(uint32_t now_ms);
    void BuildAttitude(uint32_t now_ms);

    bool QueueMessage(const mavlink_message_t& msg);
    void SendBufferedDataIfReady();

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

    // Parameters (example only)
    float velz_kp_ = 3.05f;
    float velz_ki_ = 1.0f;
    float velz_kff_ = 0.0f;
    float velz_kff2_ = 0.0f;
    float velz_accel_kfb_ = 0.0f;
    float posz_kp_ = 1.6f;
    float base_mass_kg_ = 2.5f;

    float velne_kp_ = 0.95f;
	float velne_ki_ = 0.3f;
	float velne_kff_ = 0.0f;
	float velne_kff2_ = 0.0f;
	float velne_accel_kfb_ = 0.03f;
	float posne_kp_ = 0.8f;

    struct MavlinkParam {
      const char* name;
      float* value;
      MAV_PARAM_TYPE type;
    };

    static constexpr int kParamCount = 13;

    MavlinkParam param_table_[kParamCount] = {
        {"VELZ_KP", &velz_kp_, MAV_PARAM_TYPE_REAL32},
        {"VELZ_KI", &velz_ki_, MAV_PARAM_TYPE_REAL32},
        {"VELZ_KFF", &velz_kff_, MAV_PARAM_TYPE_REAL32},
		{"VELZ_KFF2", &velz_kff2_, MAV_PARAM_TYPE_REAL32},
		{"VELZ_ACCEL_KFB", &velz_accel_kfb_, MAV_PARAM_TYPE_REAL32},
		{"POSZ_KP", &posz_kp_, MAV_PARAM_TYPE_REAL32},
		{"VELNE_KP", &velne_kp_, MAV_PARAM_TYPE_REAL32},
		{"VELNE_KI", &velne_ki_, MAV_PARAM_TYPE_REAL32},
		{"VELNE_KFF", &velne_kff_, MAV_PARAM_TYPE_REAL32},
		{"VELNE_KFF2", &velne_kff2_, MAV_PARAM_TYPE_REAL32},
		{"VELNE_ACCEL_KFB", &velne_accel_kfb_, MAV_PARAM_TYPE_REAL32},
		{"POSNE_KP", &posne_kp_, MAV_PARAM_TYPE_REAL32},
		{"BASE_MASS_KG", &base_mass_kg_, MAV_PARAM_TYPE_REAL32},
    };

    MavlinkParamsData mavlink_params_data_ = {0};
    bool new_mavlink_params_data_ = false;
};
