/*
 * mavlink_rxtx_refactored.cc
 *
 *  Original  : May 10, 2025 – Tanmay
 *  Refactored: Jun 15, 2025 – ChatGPT
 *
 *  Goal: Preserve existing functionality while reducing repetition and
 *        making it easier to add/maintain future message types.
 */

#include "mavlink_rxtx.h"

// -----------------------------------------------------------------------------
//  STATIC DATA
// -----------------------------------------------------------------------------
MavlinkRxTx* MavlinkRxTx::mavlink_rxtx_instance_handle_ = nullptr;

extern UART_HandleTypeDef huart4;
MavlinkRxTx mavlink_rxtx_task_instance_{&huart4};

// -----------------------------------------------------------------------------
//  CTOR / DTOR
// -----------------------------------------------------------------------------
MavlinkRxTx::MavlinkRxTx(UART_HandleTypeDef* huart)
    : TaskBase("MavlinkRxTxTask", 4096, osPriorityNormal),
      mavlink_uart_{huart} {
  mavlink_rxtx_instance_handle_ = this;
}

// -----------------------------------------------------------------------------
//  PRIVATE HELPERS
// -----------------------------------------------------------------------------
/**
 * @brief Try to append a MAVLink message to the active TX buffer.
 * @return true if the message was queued, false if the buffer is full.
 */
bool MavlinkRxTx::QueueMessage(const mavlink_message_t& msg) {
  if (active_index_ + MAVLINK_MAX_PACKET_LEN >= kTxBufferSize) {
    return false;  // Not enough space – drop message (or consider flushing)
  }
  active_index_ += mavlink_msg_to_send_buffer(&active_buffer_[active_index_], &msg);
  return true;
}

/// @brief Send buffer over DMA when hardware ready.
void MavlinkRxTx::SendBufferedDataIfReady() {
  if (active_index_ == 0) { return; }
  if (mavlink_uart_->hdmatx->State != HAL_DMA_STATE_READY) { return; }
  if (!__HAL_UART_GET_FLAG(mavlink_uart_, UART_FLAG_TC)) { return; }

  const size_t len = active_index_;
  SwapBuffers();  // Resets active_index_ to 0
  HAL_UART_Transmit_DMA(mavlink_uart_, sending_buffer_, len);
}

// -----------------------------------------------------------------------------
//  PUBLIC API
// -----------------------------------------------------------------------------
void MavlinkRxTx::FlushUartDataRegister() {
  while (__HAL_UART_GET_FLAG(mavlink_uart_, UART_FLAG_RXNE)) {
    volatile uint8_t tmp = static_cast<uint8_t>(mavlink_uart_->Instance->RDR & 0xFF);
    (void)tmp;  // suppress "unused" warning
  }
  __HAL_UART_CLEAR_OREFLAG(mavlink_uart_);
}

void MavlinkRxTx::SwapBuffers() {
  std::swap(active_buffer_, sending_buffer_);
  active_index_ = 0;
}

// -----------------------------------------------------------------------------
//  MESSAGE BUILDERS (easier to extend)
// -----------------------------------------------------------------------------
void MavlinkRxTx::BuildHeartbeat() {
	if (fcs_debug_sub_.copy(fcs_debug_data_)){
		if(fcs_debug_data_.sm_mode >= 1){
			base_mode_ = MAV_MODE_MANUAL_ARMED;
		}else{
			base_mode_ = MAV_MODE_MANUAL_DISARMED;
		}
	}

	switch (home_state_){
		case HomeState::NOTHOMED:
			if(base_mode_ == MAV_MODE_MANUAL_ARMED){
				home_state_ = HomeState::HOMED;
				if(gps_valid_){
					home_lat_deg_ = last_valid_lat_deg_;
					home_lon_deg_ = last_valid_lon_deg_;
					home_wgs84_alt_m_ = last_valid_wgs84_alt_m_;
				}
			}
			break;

		case HomeState::HOMED:
			if(base_mode_ == MAV_MODE_MANUAL_DISARMED){
				home_state_ = HomeState::NOTHOMED;
			}

		default:
			break;
	}

	PackAndQueue(mavlink_msg_heartbeat_pack,
			   kSysId,
			   kCompId,
			   &tx_msg_,
			   MAV_TYPE_QUADROTOR,
			   MAV_AUTOPILOT_GENERIC,
			   base_mode_,
			   0,
			   MAV_STATE_STANDBY);
}

void MavlinkRxTx::BuildGps(uint32_t now_ms)
{
	if (!gps_sub_.copy(gps_data_)) return;

	gps_valid_ = true;

	last_valid_lat_deg_ =   gps_data_.latitude_rad * RAD2DEG;
	last_valid_lon_deg_ =  gps_data_.longitude_rad * RAD2DEG;
	last_valid_wgs84_alt_m_ =   gps_data_.altitude_m;

    const uint64_t  now_us = static_cast<uint64_t>(now_ms) * 1000ULL;

    float rel_alt_m = 0.0;

    if(home_state_ == HomeState::HOMED){
    	rel_alt_m = last_valid_wgs84_alt_m_ - home_wgs84_alt_m_;
    }

    PackAndQueue(mavlink_msg_global_position_int_pack,
   		kSysId,
   		kCompId,
   		&tx_msg_,
   		now_ms,
   		static_cast<int32_t>(last_valid_lat_deg_ * 1e7),     // Latitude (degrees × 1e7)
   		static_cast<int32_t>(last_valid_lon_deg_ * 1e7),     // Longitude (degrees × 1e7)
   		static_cast<int32_t>(last_valid_wgs84_alt_m_ * 1000),// Altitude above MSL (mm)

   		static_cast<int32_t>(rel_alt_m * 1000U),
   		static_cast<int16_t>(gps_data_.vn_mps * 1e2), // North Velocity cm/s
   		static_cast<int16_t>(gps_data_.ve_mps * 1e2), // East Velocity cm/s
   		static_cast<int16_t>(gps_data_.vd_mps * 1e2), // Down Velocity cm/s
   		static_cast<uint16_t>(gps_data_.head_veh_deg * 100) // vehicle heading (centidegrees)
   	);

    PackAndQueue(
        mavlink_msg_gps_raw_int_pack,
        kSysId,
        kCompId,
        &tx_msg_,

        now_us,                                      // Timestamp (µs since boot or UNIX epoch)
        gps_data_.fix_type,                          // GPS fix type (0: no fix, 3: 3D fix, etc.)

        static_cast<int32_t>(last_valid_lat_deg_ * 1e7),     // Latitude (degrees × 1e7)
        static_cast<int32_t>(last_valid_lon_deg_ * 1e7),     // Longitude (degrees × 1e7)
        static_cast<int32_t>(last_valid_wgs84_alt_m_ * 1000),// Altitude above MSL (mm)

        static_cast<uint16_t>(gps_data_.p_dop * 1e2),        // Horizontal PDOP (unitless * 100)
        UINT16_MAX,        // Vertical accuracy (cm × 100)

        static_cast<uint16_t>(gps_data_.g_speed_mps * 100),  // Ground speed (cm/s)
        static_cast<uint16_t>(gps_data_.cog_deg * 100),      // Course over ground (centidegrees)

        gps_data_.num_sv,                                    // Number of satellites used

        static_cast<int32_t>(last_valid_wgs84_alt_m_ * 1000),// Altitude above ellipsoid (mm)
        static_cast<uint32_t>(gps_data_.hacc_m * 1000),       // Horizontal accuracy (mm)
        static_cast<uint32_t>(gps_data_.vacc_m * 1000),       // Vertical accuracy (mm)
        static_cast<uint32_t>(gps_data_.s_acc_mps * 1000),   // Speed accuracy (mm/s)
        static_cast<uint32_t>(gps_data_.heading_acc_deg * 1e5), // Heading accuracy (degE5)
		static_cast<uint16_t>(gps_data_.head_veh_deg * 100) // vehicle heading (centidegrees)
    );
}

void MavlinkRxTx::BuildAttitude(uint32_t now_ms)
{
    if (!ekf_sub_.copy(ekf_data_)) return;

    const float roll     = ekf_data_.euler_rad[0];
    const float pitch    = ekf_data_.euler_rad[1];
    const float yaw      = ekf_data_.euler_rad[2];

    PackAndQueue(mavlink_msg_attitude_pack,
                 kSysId, kCompId, &tx_msg_, now_ms,
                 roll, pitch, yaw,
				 static_cast<float>(ekf_data_.bias_corr_body_rates_radps[0]),
				 static_cast<float>(ekf_data_.bias_corr_body_rates_radps[1]),
				 static_cast<float>(ekf_data_.bias_corr_body_rates_radps[2]));
}


// -----------------------------------------------------------------------------
//  MAIN TASK
// -----------------------------------------------------------------------------
void MavlinkRxTx::Run() {
  DEBUG_PRINT("Starting Mavlink Module\n");
  Publisher<MavlinkData> mavlink_pub_(TopicID::MAVLINK);
  Publisher<MavlinkParamsData> mavlink_params_pub_(TopicID::MAVLINKPARAMS);

  TickType_t last_wake   = xTaskGetTickCount();
  int         hb_counter = 0;

  HAL_UART_Receive_DMA(mavlink_uart_, uart4_dma_rx_buffer_, kMavBuffSize);
  osDelay(200);

  for (;;) {
    const uint32_t now_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;

    // ---------------------------------------------------------------------
    //  Periodic messages
    // ---------------------------------------------------------------------
    if (++hb_counter >= kHeartbeatIntervalCount) {  // 1 Hz
      hb_counter = 0;
      BuildHeartbeat();
      BuildGps(now_ms);
//      DEBUG_PRINT("Velz Kp = %g\n", velz_kp_);
    }

    // Event‑driven / subscription‑based messages
    BuildAttitude(now_ms);

    // ---------------------------------------------------------------------
    //  Transmit + Parse incoming
    // ---------------------------------------------------------------------
    SendBufferedDataIfReady();
    ParseReadyMavlinkMessages();

    if(new_mavlink_data_){
    	new_mavlink_data_ = false;
    	mavlink_pub_.publish(mavlink_data_);
    }

    if(new_mavlink_params_data_){
    	new_mavlink_params_data_ = false;
    	mavlink_params_pub_.publish(mavlink_params_data_);
    }

    vTaskDelayUntil(&last_wake, READ_INTERVAL_MS);
  }
}

// -----------------------------------------------------------------------------
//  RECEIVE PATH
// -----------------------------------------------------------------------------
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
	printf("Msg Id received: %d\n", msg->msgid);
  switch (msg->msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: {
      mavlink_heartbeat_t hb;  // decoded but unused (kept for future)
      mavlink_msg_heartbeat_decode(msg, &hb);
      break;
    }

    case MAVLINK_MSG_ID_COMMAND_LONG: {
      mavlink_command_long_t cmd;
      mavlink_msg_command_long_decode(msg, &cmd);

      if (cmd.command == MAV_CMD_COMPONENT_ARM_DISARM) {
//        base_mode_ = (static_cast<int>(cmd.param1) == 1) ?
//                         MAV_MODE_MANUAL_ARMED : MAV_MODE_MANUAL_DISARMED;
    	  if(static_cast<int>(cmd.param1) == 1){
    		  mavlink_data_.arm_cmd_issued = 1U;
    	  }else{
    		  mavlink_data_.arm_cmd_issued = 2U;
    	  }

    	  new_mavlink_data_ = true;
      }


      // ACK
      PackAndQueue(mavlink_msg_command_ack_pack,
                   kSysId, kCompId, &tx_msg_, cmd.command, MAV_RESULT_ACCEPTED,
                   0, 0, msg->sysid, msg->compid);
      break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
        for (uint16_t i = 0; i < kParamCount; ++i) {
            PackAndQueue(mavlink_msg_param_value_pack,
                         kSysId, kCompId, &tx_msg_,
                         param_table_[i].name,
                         *(param_table_[i].value),
                         param_table_[i].type,
                         kParamCount,
                         i);
        }
        break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_READ: {
        mavlink_param_request_read_t req;
        mavlink_msg_param_request_read_decode(msg, &req);

        for (uint16_t i = 0; i < kParamCount; ++i) {
            if ((req.param_index == i) ||
                (strncmp(param_table_[i].name, req.param_id, MAVLINK_MSG_PARAM_REQUEST_READ_FIELD_PARAM_ID_LEN) == 0)) {

                PackAndQueue(mavlink_msg_param_value_pack,
                             kSysId, kCompId, &tx_msg_,
                             param_table_[i].name,
                             *(param_table_[i].value),
                             param_table_[i].type,
                             kParamCount,
                             i);
                break;
            }
        }
        break;
    }

    case MAVLINK_MSG_ID_PARAM_SET: {
        mavlink_param_set_t set;
        mavlink_msg_param_set_decode(msg, &set);

        // Ignore messages not addressed to us
//        if (msg->target_system != kSysId || msg->target_component != kCompId) break;

        for (uint16_t i = 0; i < kParamCount; ++i) {
            if (strncmp(param_table_[i].name, set.param_id, MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN) == 0) {
                *(param_table_[i].value) = set.param_value;

                PackAndQueue(mavlink_msg_param_value_pack,
                             kSysId, kCompId, &tx_msg_,
                             param_table_[i].name,
                             *(param_table_[i].value),
                             param_table_[i].type,
                             kParamCount,
                             i);
                break;
            }
        }

        mavlink_params_data_.velz_kp = velz_kp_;
        mavlink_params_data_.velz_ki = velz_ki_;
        mavlink_params_data_.velz_kff = velz_kff_;
        mavlink_params_data_.velz_kff2 = velz_kff2_;
        mavlink_params_data_.velz_accel_kfb = velz_accel_kfb_;
        mavlink_params_data_.posz_kp = posz_kp_;

        mavlink_params_data_.velne_kp = velne_kp_;
		mavlink_params_data_.velne_ki = velne_ki_;
		mavlink_params_data_.velne_kff = velne_kff_;
		mavlink_params_data_.velne_kff2 = velne_kff2_;
		mavlink_params_data_.velne_accel_kfb = velne_accel_kfb_;
		mavlink_params_data_.posne_kp = posne_kp_;
        mavlink_params_data_.base_mass_kg = base_mass_kg_;
        new_mavlink_params_data_ = true;

        break;
    }

    // Add more cases as needed...
  }
}

// -----------------------------------------------------------------------------
//  TX COMPLETE ISR HOOK
// -----------------------------------------------------------------------------
void MavlinkRxTx::SetTxReadyFlag() { tx_complete_ = true; }

void MavlinkRxTx::TxCompleted() {
  if (mavlink_rxtx_instance_handle_) {
    mavlink_rxtx_instance_handle_->SetTxReadyFlag();
  }
}
