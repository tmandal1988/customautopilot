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

#include <cstdio>
#include <cstring>

#include "mavlink_ftp_protocol.h"
#include "mavlink_parameter_protocol.h"
#include "fcsModel_types.h"
#include "parameter_metadata_blob.h"
#include "parameters/parameter_store.h"

namespace {

// The two Component Metadata files QGC downloads over MAVLink FTP. Both live
// in .rodata; the server is a bounds check and a copy, with no filesystem.
struct FtpFileEntry {
  const char* path;
  const uint8_t* data;
  uint32_t size;
};

constexpr FtpFileEntry kFtpFiles[] = {
    {parameters::generated::kComponentGeneralPath,
     parameters::generated::kComponentGeneralJson,
     static_cast<uint32_t>(parameters::generated::kComponentGeneralJsonSize)},
    {parameters::generated::kParameterMetadataPath,
     parameters::generated::kParameterMetadataJson,
     static_cast<uint32_t>(parameters::generated::kParameterMetadataJsonSize)},
};

const FtpFileEntry* FindFtpFile(const uint8_t* path,
                                size_t max_length) noexcept {
  if (path == nullptr) {
    return nullptr;
  }
  // The request path is not guaranteed NUL-terminated and QGC may prepend '/'.
  size_t start = 0U;
  if ((max_length > 0U) && (path[0] == '/')) {
    start = 1U;
  }
  size_t end = start;
  while ((end < max_length) && (path[end] != '\0')) {
    ++end;
  }
  const size_t path_length = end - start;
  for (const FtpFileEntry& entry : kFtpFiles) {
    if ((path_length == std::strlen(entry.path)) &&
        (std::memcmp(&path[start], entry.path, path_length) == 0)) {
      return &entry;
    }
  }
  return nullptr;
}

// Deliberately NOT MAV_AUTOPILOT_PX4: that identity also switches on QGC's
// PX4-specific Vehicle Setup pages (sensor calibration, safety/failsafe,
// tuning), all of which assume real PX4 parameters/protocol underneath and
// would silently hang or show misleading state on this firmware. Staying
// MAV_AUTOPILOT_GENERIC means QGC's mode indicator only ever shows
// "Custom:0x<n>", so custom_mode is kept equal to the plain enumFlightMode
// value (0-3) to keep that hex readable rather than encoding it PX4-style.
// The actual mode name is announced separately via STATUSTEXT (see
// AnnounceFlightModeIfChanged()) so it is visible without claiming PX4.
const char* FlightModeName(enumFlightMode mode) {
  switch (mode) {
    case enumFlightMode::ACRO:
      return "ACRO";
    case enumFlightMode::STABILIZE:
      return "STABILIZE";
    case enumFlightMode::ALT_CONTROL:
      return "ALT_CONTROL";
    case enumFlightMode::POS_CONTROL:
      return "POS_CONTROL";
  }
  return "UNKNOWN";
}

}  // namespace

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
    : TaskBase("MavlinkRxTx", 4096, osPriorityNormal),
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
  const size_t frame_length = mavlink_msg_get_send_buffer_length(&msg);
  if ((active_index_ > kTxBufferSize) || (frame_length == 0U) ||
      (frame_length > (kTxBufferSize - active_index_))) {
    return false;
  }
  active_index_ += mavlink_msg_to_send_buffer(&active_buffer_[active_index_], &msg);
  return true;
}

MavlinkRxTx::ParameterTxResult MavlinkRxTx::TryQueueCurrentParameterValue(
    uint16_t index, uint16_t parameter_count) {
  const auto& store = parameters::ParameterStore::Instance();
  parameters::ParameterValue value{};
  if (!store.ReadValue(index, &value)) {
    return ParameterTxResult::InvalidParameter;
  }

  return TryQueueParameterValue(index, value, parameter_count);
}

#if defined(__GNUC__) && !defined(__clang__)
// The project-wide -Ofast setting otherwise folds MAVLink finalization,
// signing, and SHA helpers into every new PARAM_VALUE call path. Keep this
// non-real-time packing implementation compact and shared by all call sites.
__attribute__((noinline, optimize("Os")))
#endif
MavlinkRxTx::ParameterTxResult MavlinkRxTx::TryQueueParameterValue(
    uint16_t index, parameters::ParameterValue value,
    uint16_t parameter_count) {
  const auto& store = parameters::ParameterStore::Instance();
  const parameters::ParameterDescriptor* descriptor = store.Descriptor(index);
  if (descriptor == nullptr) {
    return ParameterTxResult::InvalidParameter;
  }

  // Admission precedes packing so a TX-full retry does not consume another
  // MAVLink sequence number. This upper bound includes a MAVLink 2 signature.
  if ((active_index_ > kTxBufferSize) ||
      (kMaxParameterValueFrameLength > (kTxBufferSize - active_index_))) {
    return ParameterTxResult::TxFull;
  }

  if (!PackAndQueue(mavlink_msg_param_value_pack,
                    kSysId, kCompId, &tx_msg_,
                    descriptor->name,
                    mavlink_parameter_protocol::EncodeValue(value),
                    mavlink_parameter_protocol::ToWireType(descriptor->type),
                    parameter_count, index)) {
    return ParameterTxResult::TxFull;
  }
  active_buffer_has_parameter_response_ = true;
  return ParameterTxResult::Queued;
}

bool MavlinkRxTx::EnqueueCurrentParameterReply(uint16_t index) {
  const auto& store = parameters::ParameterStore::Instance();
  parameters::ParameterValue value{};
  if (!store.ReadValue(index, &value)) {
    return false;
  }

  // Enqueue and dequeue both run in this task. The fixed FIFO covers more than
  // one complete 512-byte RX window of minimum-length parameter requests.
  if ((pending_parameter_reply_head_ - pending_parameter_reply_tail_) >=
      kPendingParameterReplyCapacity) {
    ++pending_parameter_reply_overflow_count_;
    return false;
  }
  constexpr uint32_t kIndexMask = kPendingParameterReplyCapacity - 1U;
  pending_parameter_replies_[pending_parameter_reply_head_ & kIndexMask] =
      PendingParameterReply{value, index, store.Count()};
  ++pending_parameter_reply_head_;
  return true;
}

void MavlinkRxTx::RequestParameterList(TickType_t now) {
  parameter_list_cursor_.Request(static_cast<uint32_t>(now));
}

void MavlinkRxTx::ProcessOneParameterResponse(TickType_t now) {
  auto& store = parameters::ParameterStore::Instance();
  if (!has_pending_parameter_completion_) {
    if (store.PopOneCompletion(&pending_parameter_completion_)) {
      has_pending_parameter_completion_ = true;
      // PopOneCompletion records the owner timestamp exactly once before this
      // ACK can be retried.
    }
  }

  // One parameter response may be in the active or DMA-owned TX buffer. The
  // next completion is already logged/retained above, but its PARAM_VALUE ACK
  // waits for the existing UART credit.
  if (active_buffer_has_parameter_response_ ||
      sending_buffer_has_parameter_response_) {
    return;
  }

  if (has_pending_parameter_completion_) {
    // Use the owner-captured value; rereading could acknowledge a later write.
    const ParameterTxResult result = TryQueueParameterValue(
        pending_parameter_completion_.catalog_index,
        pending_parameter_completion_.value, store.Count());
    if (result == ParameterTxResult::Queued) {
      has_pending_parameter_completion_ = false;
    } else if (result == ParameterTxResult::InvalidParameter) {
      has_pending_parameter_completion_ = false;
      ++parameter_response_error_count_;
    }
    return;
  }

  if (pending_parameter_reply_tail_ != pending_parameter_reply_head_) {
    constexpr uint32_t kIndexMask = kPendingParameterReplyCapacity - 1U;
    const PendingParameterReply& reply =
        pending_parameter_replies_[pending_parameter_reply_tail_ & kIndexMask];
    const ParameterTxResult result = TryQueueParameterValue(
        reply.index, reply.value, reply.parameter_count);
    if (result == ParameterTxResult::Queued) {
      ++pending_parameter_reply_tail_;
    } else if (result == ParameterTxResult::InvalidParameter) {
      ++pending_parameter_reply_tail_;
      ++parameter_response_error_count_;
    }
    return;
  }

  const uint32_t now_ticks = static_cast<uint32_t>(now);
  const uint32_t list_interval =
      static_cast<uint32_t>(pdMS_TO_TICKS(kParameterListIntervalMs));
  if (!parameter_list_cursor_.ReadyToSend(
          now_ticks, store.IsReady(), store.Count(), list_interval)) {
    return;
  }

  const ParameterTxResult result = TryQueueCurrentParameterValue(
      parameter_list_cursor_.next(), parameter_list_cursor_.count());
  if (result == ParameterTxResult::Queued) {
    parameter_list_cursor_.CommitQueued(now_ticks, list_interval);
  } else if (result == ParameterTxResult::TxFull) {
    parameter_list_cursor_.Defer(now_ticks, list_interval);
  } else {
    parameter_list_cursor_.Abort();
    ++parameter_list_error_count_;
  }
}

/// @brief Send buffer over DMA when hardware ready.
void MavlinkRxTx::SendBufferedDataIfReady(TickType_t now) {
  const uint32_t now_ticks = static_cast<uint32_t>(now);
  if (tx_replay_wait_pending_) {
    return;
  }
  if (tx_retry_pending_ &&
      !mavlink_transport::DeadlineReached(
          now_ticks, static_cast<uint32_t>(next_tx_retry_due_))) {
    return;
  }

  // A nonzero sending length is task-owned DMA credit. It is released only
  // after this task consumes the corresponding completion notification. This
  // prevents a delayed completion bit from being applied to a newer transfer.
  if ((sending_length_ != 0U) && !tx_resend_pending_) {
    return;
  }
  if (!tx_resend_pending_ && (active_index_ == 0U)) {
    tx_retry_pending_ = false;
    return;
  }

  const auto schedule_retry = [this, now]() {
    tx_retry_pending_ = true;
    next_tx_retry_due_ =
        now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
  };
  if (mavlink_uart_->hdmatx == nullptr) {
    ++tx_start_error_count_;
    schedule_retry();
    return;
  }
  if ((mavlink_uart_->hdmatx->State != HAL_DMA_STATE_READY) ||
      (mavlink_uart_->gState != HAL_UART_STATE_READY)) {
    schedule_retry();
    return;
  }

  const bool is_resend = tx_resend_pending_;
  const uint8_t* const buffer =
      is_resend ? sending_buffer_ : active_buffer_;
  const size_t length = is_resend ? sending_length_ : active_index_;
  const HAL_StatusTypeDef result = HAL_UART_Transmit_DMA(
      mavlink_uart_, buffer, static_cast<uint16_t>(length));
  if (result == HAL_OK) {
    if (is_resend) {
      tx_resend_pending_ = false;
      ++tx_resend_count_;
    } else {
      // Swap ownership only after HAL accepts the transfer. Keep the exact
      // length until TxCplt so a DMA fault can replay these serialized bytes.
      sending_length_ = active_index_;
      std::swap(active_buffer_, sending_buffer_);
      active_index_ = 0U;
      sending_buffer_has_parameter_response_ =
          active_buffer_has_parameter_response_;
      active_buffer_has_parameter_response_ = false;
    }
    tx_retry_pending_ = false;
    return;
  }

  ++tx_start_error_count_;
  schedule_retry();
}

// -----------------------------------------------------------------------------
//  MESSAGE BUILDERS (easier to extend)
// -----------------------------------------------------------------------------
void MavlinkRxTx::ServicePendingReboot(TickType_t now) {
  if (!reboot_pending_) {
    return;
  }

  // Two conditions before resetting: any retained-parameter change must have
  // finished its debounced Flash commit (rebooting inside that window would
  // silently discard the operator's change), and the accepted-command ACK
  // must have drained out of both TX buffers (HAL raises TxCplt only after
  // the UART shift register empties). The deadline guarantees a wedged link
  // or a stuck commit cannot veto the reboot the operator asked for.
  const bool persistence_quiescent =
      parameters::ParameterStore::Instance().PersistenceQuiescent();
  const bool tx_idle = (sending_length_ == 0U) && (active_index_ == 0U) &&
                       !tx_resend_pending_;
  if ((!tx_idle || !persistence_quiescent) &&
      !mavlink_transport::DeadlineReached(
          static_cast<uint32_t>(now),
          static_cast<uint32_t>(reboot_deadline_))) {
    return;
  }

  // Small grace period for the telemetry radio's own buffer, then reset.
  vTaskDelay(pdMS_TO_TICKS(20U));
  NVIC_SystemReset();
}

void MavlinkRxTx::BuildHeartbeat() {
	if (fcs_debug_sub_.copy(fcs_debug_data_)){
		if(fcs_debug_data_.sm_mode >= 1){
			base_mode_ = MAV_MODE_MANUAL_ARMED;
		}else{
			base_mode_ = MAV_MODE_MANUAL_DISARMED;
		}

    // Flash admission observes the already-published FCS state here at 1 Hz,
    // outside the flight-control task. Unknown fails closed. Only INACTIVE
    // permits Flash: once motors are armed, takeoff can follow at any moment
    // and an in-progress sector erase cannot be aborted, so MTR_ARMED is
    // classified with INFLIGHT. A change made while armed stays dirty and
    // commits after the disarm transition notifies the worker.
    auto persistence_state =
        parameters::PersistenceFlightState::Unknown;
    switch (static_cast<enumStateMachine>(fcs_debug_data_.sm_mode)) {
      case enumStateMachine::INACTIVE:
        persistence_state =
            parameters::PersistenceFlightState::NotInFlight;
        break;
      case enumStateMachine::MTR_ARMED:
      case enumStateMachine::INFLIGHT:
        persistence_state = parameters::PersistenceFlightState::InFlight;
        break;
      default:
        break;
    }
    parameters::ParameterStore::Instance().ObservePersistenceFlightState(
        persistence_state);
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

	AnnounceFlightModeIfChanged();

	// MAV_AUTOPILOT_GENERIC (not PX4/ArduPilot): this is a from-scratch
	// autopilot, and claiming PX4 would switch on QGC's PX4-specific setup
	// pages (calibration, safety/failsafe, tuning) that assume real PX4
	// parameters/protocol underneath -- see AnnounceFlightModeIfChanged() for
	// how the mode name is still surfaced without that false identity.
	// MAV_MODE_FLAG_CUSTOM_MODE_ENABLED is still set so QGC's generic mode
	// indicator shows "Custom:0x<n>" with the plain enumFlightMode value
	// rather than nothing; it is OR'd in here only, so base_mode_'s
	// armed/disarmed comparisons elsewhere in this file stay on the plain
	// MAV_MODE_MANUAL_* values.
	PackAndQueue(mavlink_msg_heartbeat_pack,
			   kSysId,
			   kCompId,
			   &tx_msg_,
			   MAV_TYPE_QUADROTOR,
			   MAV_AUTOPILOT_GENERIC,
			   static_cast<uint8_t>(base_mode_ | MAV_MODE_FLAG_CUSTOM_MODE_ENABLED),
			   static_cast<uint32_t>(fcs_debug_data_.flt_mode),
			   MAV_STATE_STANDBY);
}

void MavlinkRxTx::AnnounceFlightModeIfChanged() {
	if (fcs_debug_data_.flt_mode == last_announced_flt_mode_) {
		return;
	}
	last_announced_flt_mode_ = fcs_debug_data_.flt_mode;

	char text[50] = {0};
	std::snprintf(text, sizeof(text), "Mode: %s",
			FlightModeName(static_cast<enumFlightMode>(fcs_debug_data_.flt_mode)));
	PackAndQueue(mavlink_msg_statustext_pack,
			   kSysId,
			   kCompId,
			   &tx_msg_,
			   MAV_SEVERITY_INFO,
			   text,
			   0,
			   0);
}

void MavlinkRxTx::BuildAutopilotVersion() {
  static constexpr uint8_t kZeroVersion[8]{};
  static constexpr uint8_t kZeroUid[18]{};
  // QGC gates every MAVLink FTP attempt on the FTP capability bit; without it
  // Component Metadata is never fetched regardless of the 397 response.
  constexpr uint64_t kCapabilities =
      mavlink_parameter_protocol::kParameterProtocolCapabilities |
      MAV_PROTOCOL_CAPABILITY_FTP |
      MAV_PROTOCOL_CAPABILITY_MAVLINK2;

  PackAndQueue(mavlink_msg_autopilot_version_pack,
               kSysId, kCompId, &tx_msg_, kCapabilities,
               0U, 0U, 0U, 0U,
               kZeroVersion, kZeroVersion, kZeroVersion,
               0U, 0U, 0ULL, kZeroUid);
}

void MavlinkRxTx::BuildComponentMetadata(uint32_t now_ms) {
  // Points QGC at the embedded general metadata file; QGC then downloads it
  // and the parameter metadata it references over FTP, caching both by CRC.
  // The packer copies the entire 100-byte wire field, so zero-pad the
  // NUL-terminated generated URI to full width first.
  char uri[MAVLINK_MSG_COMPONENT_METADATA_FIELD_URI_LEN]{};
  static_assert(sizeof(parameters::generated::kComponentGeneralUri) <=
                sizeof(uri));
  std::memcpy(uri, parameters::generated::kComponentGeneralUri,
              sizeof(parameters::generated::kComponentGeneralUri));
  PackAndQueue(mavlink_msg_component_metadata_pack,
               kSysId, kCompId, &tx_msg_, now_ms,
               parameters::generated::kComponentGeneralJsonCrc32, uri);
}

// -----------------------------------------------------------------------------
//  READ-ONLY MAVLINK FTP SERVER (Component Metadata files only)
// -----------------------------------------------------------------------------
bool MavlinkRxTx::TryQueueFtpPacket(const uint8_t* payload) {
  // Admission precedes packing, mirroring TryQueueParameterValue, so a
  // TX-full retry never consumes a MAVLink sequence number.
  if ((active_index_ > kTxBufferSize) ||
      (kMaxFtpFrameLength > (kTxBufferSize - active_index_))) {
    return false;
  }
  return PackAndQueue(mavlink_msg_file_transfer_protocol_pack,
                      kSysId, kCompId, &tx_msg_, 0U,
                      ftp_peer_system_, ftp_peer_component_, payload);
}

void MavlinkRxTx::QueueFtpNak(uint16_t request_seq, uint8_t session,
                              uint8_t request_opcode, uint8_t error) {
  using namespace mavlink_ftp_protocol;
  uint8_t payload[kPayloadSize]{};
  Header reply{};
  reply.seq_number = static_cast<uint16_t>(request_seq + 1U);
  reply.session = session;
  reply.opcode = Opcode::Nak;
  reply.size = 1U;
  reply.req_opcode = static_cast<Opcode>(request_opcode);
  EncodeHeader(reply, payload);
  payload[kHeaderSize] = error;
  if (!TryQueueFtpPacket(payload)) {
    ++ftp_tx_drop_count_;
  }
}

#if defined(__GNUC__) && !defined(__clang__)
__attribute__((noinline, optimize("Os")))
#endif
void MavlinkRxTx::HandleFtpRequest(const mavlink_message_t* msg) {
  using namespace mavlink_ftp_protocol;
  mavlink_file_transfer_protocol_t ftp;
  mavlink_msg_file_transfer_protocol_decode(msg, &ftp);
  if (!mavlink_parameter_protocol::IsTarget(
          ftp.target_system, ftp.target_component, kSysId, kCompId)) {
    return;
  }

  ftp_peer_system_ = msg->sysid;
  ftp_peer_component_ = msg->compid;

  const Header request = DecodeHeader(ftp.payload);
  const uint8_t* request_data = &ftp.payload[kHeaderSize];
  const uint8_t request_opcode = static_cast<uint8_t>(request.opcode);
  if (request.size > kMaxDataSize) {
    QueueFtpNak(request.seq_number, request.session, request_opcode,
                static_cast<uint8_t>(NakError::InvalidDataSize));
    return;
  }

  switch (request.opcode) {
    case Opcode::OpenFileRO: {
      const FtpFileEntry* entry = FindFtpFile(request_data, request.size);
      if (entry == nullptr) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::FileNotFound));
        return;
      }
      // Single-session server: a new open supersedes any previous session.
      ftp_file_data_ = entry->data;
      ftp_file_size_ = entry->size;
      ftp_session_open_ = true;
      ftp_burst_active_ = false;

      uint8_t payload[kPayloadSize]{};
      Header reply{};
      reply.seq_number = static_cast<uint16_t>(request.seq_number + 1U);
      reply.session = 0U;
      reply.opcode = Opcode::Ack;
      reply.size = 4U;
      reply.req_opcode = Opcode::OpenFileRO;
      EncodeHeader(reply, payload);
      payload[kHeaderSize + 0U] =
          static_cast<uint8_t>(entry->size & 0xFFU);
      payload[kHeaderSize + 1U] =
          static_cast<uint8_t>((entry->size >> 8U) & 0xFFU);
      payload[kHeaderSize + 2U] =
          static_cast<uint8_t>((entry->size >> 16U) & 0xFFU);
      payload[kHeaderSize + 3U] =
          static_cast<uint8_t>((entry->size >> 24U) & 0xFFU);
      if (!TryQueueFtpPacket(payload)) {
        ++ftp_tx_drop_count_;
      }
      return;
    }

    case Opcode::ReadFile: {
      if (!ftp_session_open_ || (request.session != 0U)) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::InvalidSession));
        return;
      }
      if (request.offset >= ftp_file_size_) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::Eof));
        return;
      }
      const uint32_t remaining = ftp_file_size_ - request.offset;
      uint32_t chunk = (remaining < kMaxDataSize)
                           ? remaining
                           : static_cast<uint32_t>(kMaxDataSize);
      if ((request.size != 0U) && (request.size < chunk)) {
        chunk = request.size;
      }

      uint8_t payload[kPayloadSize]{};
      Header reply{};
      reply.seq_number = static_cast<uint16_t>(request.seq_number + 1U);
      reply.session = 0U;
      reply.opcode = Opcode::Ack;
      reply.size = static_cast<uint8_t>(chunk);
      reply.req_opcode = Opcode::ReadFile;
      reply.offset = request.offset;
      EncodeHeader(reply, payload);
      std::memcpy(&payload[kHeaderSize], &ftp_file_data_[request.offset],
                  chunk);
      if (!TryQueueFtpPacket(payload)) {
        ++ftp_tx_drop_count_;
      }
      return;
    }

    case Opcode::BurstReadFile: {
      if (!ftp_session_open_ || (request.session != 0U)) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::InvalidSession));
        return;
      }
      if (request.offset >= ftp_file_size_) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::Eof));
        return;
      }
      ftp_burst_active_ = true;
      ftp_burst_offset_ = request.offset;
      ftp_next_seq_ = static_cast<uint16_t>(request.seq_number + 1U);
      ProcessFtpBurst();
      return;
    }

    case Opcode::TerminateSession: {
      if (!ftp_session_open_ || (request.session != 0U)) {
        QueueFtpNak(request.seq_number, request.session, request_opcode,
                    static_cast<uint8_t>(NakError::InvalidSession));
        return;
      }
      [[fallthrough]];
    }
    case Opcode::ResetSessions: {
      ftp_session_open_ = false;
      ftp_burst_active_ = false;

      uint8_t payload[kPayloadSize]{};
      Header reply{};
      reply.seq_number = static_cast<uint16_t>(request.seq_number + 1U);
      reply.session = request.session;
      reply.opcode = Opcode::Ack;
      reply.req_opcode = static_cast<Opcode>(request_opcode);
      EncodeHeader(reply, payload);
      if (!TryQueueFtpPacket(payload)) {
        ++ftp_tx_drop_count_;
      }
      return;
    }

    default:
      QueueFtpNak(request.seq_number, request.session, request_opcode,
                  static_cast<uint8_t>(NakError::UnknownCommand));
      return;
  }
}

#if defined(__GNUC__) && !defined(__clang__)
__attribute__((noinline, optimize("Os")))
#endif
void MavlinkRxTx::ProcessFtpBurst() {
  using namespace mavlink_ftp_protocol;
  // Bounded by TX buffer capacity: at most a handful of frames fit before
  // TryQueueFtpPacket reports full and the burst resumes on the next TxCplt.
  while (ftp_burst_active_) {
    if (!ftp_session_open_ || (ftp_burst_offset_ >= ftp_file_size_)) {
      ftp_burst_active_ = false;
      return;
    }
    const uint32_t remaining = ftp_file_size_ - ftp_burst_offset_;
    const uint32_t chunk = (remaining < kMaxDataSize)
                               ? remaining
                               : static_cast<uint32_t>(kMaxDataSize);
    const bool last = (ftp_burst_offset_ + chunk) >= ftp_file_size_;

    uint8_t payload[kPayloadSize]{};
    Header reply{};
    reply.seq_number = ftp_next_seq_;
    reply.session = 0U;
    reply.opcode = Opcode::Ack;
    reply.size = static_cast<uint8_t>(chunk);
    reply.req_opcode = Opcode::BurstReadFile;
    reply.burst_complete = last ? 1U : 0U;
    reply.offset = ftp_burst_offset_;
    EncodeHeader(reply, payload);
    std::memcpy(&payload[kHeaderSize], &ftp_file_data_[ftp_burst_offset_],
                chunk);
    if (!TryQueueFtpPacket(payload)) {
      return;  // TX full; state is retained and resumed after DMA drains.
    }
    ++ftp_next_seq_;
    ftp_burst_offset_ += chunk;
    if (last) {
      ftp_burst_active_ = false;
    }
  }
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
void MavlinkRxTx::ResetMavlinkParser() {
  msg = {};
  status = {};
  mavlink_reset_channel_status(MAVLINK_COMM_0);
}

bool MavlinkRxTx::StartRxDma() {
  rx_dma_last_position_.store(0U, std::memory_order_relaxed);
  rx_produced_bytes_.store(0U, std::memory_order_release);
  rx_consumed_bytes_ = 0U;
  rx_backlog_pending_ = false;
  ResetMavlinkParser();

  const HAL_StatusTypeDef result = HAL_UARTEx_ReceiveToIdle_DMA(
      mavlink_uart_, uart4_dma_rx_buffer_,
      static_cast<uint16_t>(kMavBuffSize));
  if (result == HAL_OK) {
    rx_restart_pending_ = false;
    return true;
  }
  return false;
}

void MavlinkRxTx::RecoverRxDma(TickType_t now) {
  if (!rx_restart_pending_ ||
      !mavlink_transport::DeadlineReached(
          static_cast<uint32_t>(now),
          static_cast<uint32_t>(next_rx_restart_due_))) {
    return;
  }

  const HAL_StatusTypeDef abort_result = HAL_UART_AbortReceive(mavlink_uart_);
  if ((abort_result == HAL_OK) && StartRxDma()) {
    ++rx_restart_count_;
    return;
  }

  ++rx_error_count_;
  rx_restart_pending_ = true;
  next_rx_restart_due_ =
      now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
}

TickType_t MavlinkRxTx::ComputeWaitTicks(
    TickType_t now, TickType_t next_telemetry,
    TickType_t next_heartbeat) const {
  if (rx_backlog_pending_) {
    return 0U;
  }

  const uint32_t now_ticks = static_cast<uint32_t>(now);
  uint32_t wait_ticks = mavlink_transport::TicksUntil(
      now_ticks, static_cast<uint32_t>(next_telemetry));
  const auto shorten_wait = [now_ticks, &wait_ticks](uint32_t deadline) {
    const uint32_t candidate =
        mavlink_transport::TicksUntil(now_ticks, deadline);
    if (candidate < wait_ticks) {
      wait_ticks = candidate;
    }
  };

  shorten_wait(static_cast<uint32_t>(next_heartbeat));
  if (parameter_list_cursor_.has_deadline() &&
      !active_buffer_has_parameter_response_ &&
      !sending_buffer_has_parameter_response_ &&
      !has_pending_parameter_completion_ &&
      (pending_parameter_reply_tail_ == pending_parameter_reply_head_)) {
    shorten_wait(parameter_list_cursor_.next_due());
  }
  if (rx_restart_pending_) {
    shorten_wait(static_cast<uint32_t>(next_rx_restart_due_));
  }
  if (tx_retry_pending_) {
    shorten_wait(static_cast<uint32_t>(next_tx_retry_due_));
  }
  if (reboot_pending_) {
    shorten_wait(static_cast<uint32_t>(reboot_deadline_));
  }
  return static_cast<TickType_t>(wait_ticks);
}

void MavlinkRxTx::Run() {
  Publisher<MavlinkData> mavlink_pub_(TopicID::MAVLINK);

  static_assert(sizeof(TickType_t) == sizeof(uint32_t));
  mavlink_task_handle_ = xTaskGetCurrentTaskHandle();
  TickType_t now = xTaskGetTickCount();
  TickType_t next_telemetry =
      now + pdMS_TO_TICKS(kTelemetryIntervalMs);
  TickType_t next_heartbeat =
      now + pdMS_TO_TICKS(kHeartbeatIntervalMs);

  if (!StartRxDma()) {
    ++rx_error_count_;
    rx_restart_pending_ = true;
    next_rx_restart_due_ =
        now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
  }

  ConfigureEventMetrics();

  for (;;) {
    now = xTaskGetTickCount();
    uint32_t events = 0U;
    const bool replay_wait_elapsed = tx_replay_wait_pending_;
    const TickType_t wait_ticks =
        ComputeWaitTicks(now, next_telemetry, next_heartbeat);
    (void)xTaskNotifyWait(0U, kAllTaskEvents, &events, wait_ticks);
    BeginMetricsCycle();
    now = xTaskGetTickCount();
    if (replay_wait_elapsed) {
      tx_replay_wait_pending_ = false;
    }

    const bool tx_completed = (events & kTxCompleteEvent) != 0U;
    if (tx_completed) {
      sending_length_ = 0U;
      sending_buffer_has_parameter_response_ = false;
      tx_resend_pending_ = false;
      tx_replay_wait_pending_ = false;
      tx_retry_pending_ = false;
    }
    if ((events & kRxErrorEvent) != 0U) {
      ++rx_error_count_;
      rx_restart_pending_ = true;
      next_rx_restart_due_ = now;
    }
    if (((events & kTxErrorEvent) != 0U) && !tx_completed &&
        (sending_length_ != 0U)) {
      ++tx_dma_error_count_;
      // Exceptional task-context recovery only. Preserve the DMA-owned buffer,
      // its parameter-response credit, and its exact length for replay.
      (void)HAL_UART_AbortTransmit(mavlink_uart_);
      tx_resend_pending_ = true;
      tx_replay_wait_pending_ = true;
      tx_retry_pending_ = true;
      // Leave one notification wait between abort and replay. If the original
      // transfer completed just before the abort took effect, its queued
      // TxCplt is consumed as that generation's authoritative result before a
      // new DMA generation can start.
      next_tx_retry_due_ =
          now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
    }
    RecoverRxDma(now);

    // Free the active buffer before parsing requests whenever the previous DMA
    // transmission has completed. HAL failure retains every unsent byte.
    SendBufferedDataIfReady(now);

    // Snapshot and parse no more than half a DMA buffer in this service pass.
    // A remaining backlog makes the next notification wait zero. HT/TC bound
    // the service horizon to one half-buffer (44.4 ms at 57,600 baud, 8N1).
    rx_backlog_pending_ = ParseReadyMavlinkMessages();

    const uint32_t now_ticks = static_cast<uint32_t>(now);
    const uint32_t now_ms = now_ticks * portTICK_PERIOD_MS;
    if (mavlink_transport::DeadlineReached(
            now_ticks, static_cast<uint32_t>(next_heartbeat))) {
      BuildHeartbeat();
      BuildGps(now_ms);
      // Skip missed releases in O(1); never emit a catch-up burst.
      next_heartbeat = now + pdMS_TO_TICKS(kHeartbeatIntervalMs);
    }
    if (mavlink_transport::DeadlineReached(
            now_ticks, static_cast<uint32_t>(next_telemetry))) {
      BuildAttitude(now_ms);
      next_telemetry = now + pdMS_TO_TICKS(kTelemetryIntervalMs);
    }

    // Completion ACKs, explicit reads/rejections, then the paced list cursor.
    // The response pipeline permits at most one PARAM_VALUE frame in flight.
    ProcessOneParameterResponse(now);
    // A suspended metadata burst refills whatever TX space parameters left;
    // it makes progress on every TxCplt wake-up until EOF.
    ProcessFtpBurst();
    SendBufferedDataIfReady(now);
    ServicePendingReboot(now);

    if(new_mavlink_data_){
    	new_mavlink_data_ = false;
    	mavlink_pub_.publish(mavlink_data_);
    }
    EndMetricsCycle();
  }
}

// -----------------------------------------------------------------------------
//  RECEIVE PATH
// -----------------------------------------------------------------------------
bool MavlinkRxTx::ParseReadyMavlinkMessages() {
  const uint32_t produced =
      rx_produced_bytes_.load(std::memory_order_acquire);
  const auto plan = mavlink_transport::PlanRxDrain<
      kMavBuffSize, kRxParseBudget>(produced, rx_consumed_bytes_);
  if (plan.overrun) {
    ++rx_overrun_count_;
    rx_dropped_byte_count_ += plan.dropped;
    rx_consumed_bytes_ = plan.start;
    ResetMavlinkParser();
    return false;
  }

  constexpr uint32_t kIndexMask = kMavBuffSize - 1U;
  for (uint16_t offset = 0U; offset < plan.count; ++offset) {
    const uint32_t index = (plan.start + offset) & kIndexMask;
    const uint8_t byte = uart4_dma_rx_buffer_[index];
    if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status)) {
      HandleMavlinkMessage(&msg);
    }
  }
  rx_consumed_bytes_ += plan.count;
  return (rx_produced_bytes_.load(std::memory_order_acquire) -
          rx_consumed_bytes_) > 0U;
}

void MavlinkRxTx::HandleMavlinkMessage(mavlink_message_t* msg) {
  switch (msg->msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT: {
      mavlink_heartbeat_t hb;  // decoded but unused (kept for future)
      mavlink_msg_heartbeat_decode(msg, &hb);
      break;
    }

    case MAVLINK_MSG_ID_COMMAND_LONG: {
      mavlink_command_long_t cmd;
      mavlink_msg_command_long_decode(msg, &cmd);
      if (!mavlink_parameter_protocol::IsTarget(
              cmd.target_system, cmd.target_component, kSysId, kCompId)) {
        break;
      }

      MAV_RESULT command_result = MAV_RESULT_UNSUPPORTED;
      if (cmd.command == MAV_CMD_COMPONENT_ARM_DISARM) {
    	  if(static_cast<int>(cmd.param1) == 1){
            // Arming is the last command boundary before takeoff, and a
            // sector erase already in progress cannot be aborted. Refusing
            // to arm during an active commit closes the check-then-act race
            // for command-initiated arming (PX4: "arming denied: parameter
            // save in progress"). Retrying a moment later succeeds.
            if (parameters::ParameterStore::Instance()
                    .FlashCommitInProgress()) {
              command_result = MAV_RESULT_TEMPORARILY_REJECTED;
            } else {
              mavlink_data_.arm_cmd_issued = 1U;
              new_mavlink_data_ = true;
              command_result = MAV_RESULT_ACCEPTED;
            }
    	  }else{
    		  mavlink_data_.arm_cmd_issued = 2U;
    		  new_mavlink_data_ = true;
    		  command_result = MAV_RESULT_ACCEPTED;
    	  }
      } else if ((cmd.command ==
                  MAV_CMD_REQUEST_AUTOPILOT_CAPABILITIES) &&
                 (static_cast<int>(cmd.param1) == 1)) {
        BuildAutopilotVersion();
        command_result = MAV_RESULT_ACCEPTED;
      } else if ((cmd.command == MAV_CMD_REQUEST_MESSAGE) &&
                 (static_cast<uint32_t>(cmd.param1) ==
                  MAVLINK_MSG_ID_AUTOPILOT_VERSION)) {
        BuildAutopilotVersion();
        command_result = MAV_RESULT_ACCEPTED;
      } else if ((cmd.command == MAV_CMD_REQUEST_MESSAGE) &&
                 (static_cast<uint32_t>(cmd.param1) ==
                  MAVLINK_MSG_ID_COMPONENT_METADATA)) {
        BuildComponentMetadata(
            static_cast<uint32_t>(xTaskGetTickCount()) *
            portTICK_PERIOD_MS);
        command_result = MAV_RESULT_ACCEPTED;
      } else if ((cmd.command == MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN) &&
                 (static_cast<int>(cmd.param1) == 1)) {
        // Reboot the autopilot (QGC: Parameters -> Tools -> Reboot Vehicle).
        // Shutdown/bootloader requests (param1 != 1) stay unsupported. Honor
        // the request only while the FCS state machine is INACTIVE; the copy
        // refreshes the latest published state and the member retains the
        // last sample when nothing new has been published.
        static_cast<void>(fcs_debug_sub_.copy(fcs_debug_data_));
        if (static_cast<enumStateMachine>(fcs_debug_data_.sm_mode) ==
            enumStateMachine::INACTIVE) {
          reboot_pending_ = true;
          reboot_deadline_ = xTaskGetTickCount() +
                             pdMS_TO_TICKS(kRebootDrainTimeoutMs);
          command_result = MAV_RESULT_ACCEPTED;
        } else {
          command_result = MAV_RESULT_DENIED;
        }
      }

      // ACK
      PackAndQueue(mavlink_msg_command_ack_pack,
                   kSysId, kCompId, &tx_msg_, cmd.command, command_result,
                   0, 0, msg->sysid, msg->compid);
      break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST: {
        mavlink_param_request_list_t req;
        mavlink_msg_param_request_list_decode(msg, &req);
        if (!mavlink_parameter_protocol::IsTarget(
                req.target_system, req.target_component, kSysId, kCompId)) {
            break;
        }

        RequestParameterList(xTaskGetTickCount());
        break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_READ: {
        mavlink_param_request_read_t req;
        mavlink_msg_param_request_read_decode(msg, &req);
        if (!mavlink_parameter_protocol::IsTarget(
                req.target_system, req.target_component, kSysId, kCompId)) {
            break;
        }

        const auto& store = parameters::ParameterStore::Instance();
        if (req.param_index >= 0) {
            EnqueueCurrentParameterReply(
                static_cast<uint16_t>(req.param_index));
        } else {
            uint16_t index = 0U;
            if (store.FindById(
                    req.param_id,
                    MAVLINK_MSG_PARAM_REQUEST_READ_FIELD_PARAM_ID_LEN,
                    &index)) {
                EnqueueCurrentParameterReply(index);
            }
        }
        break;
    }

    case MAVLINK_MSG_ID_PARAM_SET: {
        mavlink_param_set_t set;
        mavlink_msg_param_set_decode(msg, &set);
        if (!mavlink_parameter_protocol::IsTarget(
                set.target_system, set.target_component, kSysId, kCompId)) {
            break;
        }

        auto& store = parameters::ParameterStore::Instance();
        uint16_t index = 0U;
        if (!store.FindById(
                set.param_id, MAVLINK_MSG_PARAM_SET_FIELD_PARAM_ID_LEN,
                &index)) {
            // Legacy PARAM_VALUE cannot represent an acknowledgement for an
            // unknown parameter because there is no current value or index.
            break;
        }

        const parameters::ParameterDescriptor* descriptor =
                store.Descriptor(index);
        if ((descriptor == nullptr) ||
            !mavlink_parameter_protocol::IsCompatibleWireType(
                descriptor->type, set.param_type)) {
            EnqueueCurrentParameterReply(index);
            break;
        }

        const uint32_t token = next_parameter_update_token_++;
        parameters::ParameterUpdateCompletion immediate_completion{};
        const parameters::ParameterSubmitResult result =
                store.SubmitUpdate(
                    index,
                    mavlink_parameter_protocol::DecodeValue(set.param_value),
                    token, &immediate_completion);
        if (result == parameters::ParameterSubmitResult::Completed) {
            // OnReboot changes are staged synchronously in this protocol task;
            // ACK the configured value and record the change exactly once.
            static_cast<void>(store.RecordChange(immediate_completion));
            EnqueueCurrentParameterReply(index);
        } else if (result != parameters::ParameterSubmitResult::Queued) {
            // The legacy parameter protocol acknowledges every known write,
            // including rejection, by broadcasting the current value.
            EnqueueCurrentParameterReply(index);
        }

        break;
    }

    case MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL: {
        HandleFtpRequest(msg);
        break;
    }

    // This vehicle carries no onboard mission/fence/rally storage. QGC's
    // initial-connect sequence downloads all three unconditionally for any
    // MAVLink vehicle -- with no handler at all it retries MISSION_REQUEST_LIST
    // until it gives up ("Mission transfer failed... maximum retries exceeded").
    // Answering immediately with a zero-item count is the same response PX4
    // gives when no mission is stored, and completes that download instantly.
    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST: {
        mavlink_mission_request_list_t req;
        mavlink_msg_mission_request_list_decode(msg, &req);
        if (!mavlink_parameter_protocol::IsTarget(
                req.target_system, req.target_component, kSysId, kCompId)) {
            break;
        }

        PackAndQueue(mavlink_msg_mission_count_pack,
                     kSysId, kCompId, &tx_msg_, msg->sysid, msg->compid,
                     0U, req.mission_type, 0U);
        break;
    }

    // QGC's Plan view can send this independently of a fresh connect (e.g.
    // "Sync"/clear actions). Nothing is stored, so there is nothing to clear;
    // accept unconditionally rather than leaving QGC waiting on an ACK.
    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL: {
        mavlink_mission_clear_all_t req;
        mavlink_msg_mission_clear_all_decode(msg, &req);
        if (!mavlink_parameter_protocol::IsTarget(
                req.target_system, req.target_component, kSysId, kCompId)) {
            break;
        }

        PackAndQueue(mavlink_msg_mission_ack_pack,
                     kSysId, kCompId, &tx_msg_, msg->sysid, msg->compid,
                     MAV_MISSION_ACCEPTED, req.mission_type, 0U);
        break;
    }

    // Add more cases as needed...
  }
}

// -----------------------------------------------------------------------------
//  UART ISR HOOKS
// -----------------------------------------------------------------------------
void MavlinkRxTx::NotifyTaskFromIsr(uint32_t event) {
  if (mavlink_task_handle_ == nullptr) {
    return;
  }

  BaseType_t higher_priority_task_woken = pdFALSE;
  (void)xTaskNotifyFromISR(mavlink_task_handle_, event, eSetBits,
                          &higher_priority_task_woken);
  portYIELD_FROM_ISR(higher_priority_task_woken);
}

void MavlinkRxTx::RxEvent(UART_HandleTypeDef* huart, uint16_t size) {
  MavlinkRxTx* instance = mavlink_rxtx_instance_handle_;
  if ((instance == nullptr) || (huart != instance->mavlink_uart_)) {
    return;
  }

  mavlink_transport::DmaRxEvent event =
      mavlink_transport::DmaRxEvent::Idle;
  switch (HAL_UARTEx_GetRxEventType(huart)) {
    case HAL_UART_RXEVENT_HT:
      event = mavlink_transport::DmaRxEvent::HalfTransfer;
      break;
    case HAL_UART_RXEVENT_TC:
      event = mavlink_transport::DmaRxEvent::TransferComplete;
      break;
    case HAL_UART_RXEVENT_IDLE:
      break;
    default:
      instance->NotifyTaskFromIsr(kRxErrorEvent);
      return;
  }

  const uint32_t previous =
      instance->rx_dma_last_position_.load(std::memory_order_relaxed);
  const auto progress =
      mavlink_transport::AdvanceDmaRxProgress<kMavBuffSize>(
          static_cast<uint16_t>(previous), size, event);
  if (!progress.valid) {
    instance->NotifyTaskFromIsr(kRxErrorEvent);
    return;
  }

  instance->rx_dma_last_position_.store(progress.position,
                                         std::memory_order_relaxed);
  if (progress.new_bytes == 0U) {
    return;
  }
  const uint32_t produced =
      instance->rx_produced_bytes_.load(std::memory_order_relaxed);
  instance->rx_produced_bytes_.store(produced + progress.new_bytes,
                                      std::memory_order_release);
  instance->NotifyTaskFromIsr(kRxReadyEvent);
}

void MavlinkRxTx::UartError(UART_HandleTypeDef* huart) {
  MavlinkRxTx* instance = mavlink_rxtx_instance_handle_;
  if ((instance != nullptr) && (huart == instance->mavlink_uart_)) {
    constexpr uint32_t kReceiveErrorMask =
        HAL_UART_ERROR_PE | HAL_UART_ERROR_NE | HAL_UART_ERROR_FE |
        HAL_UART_ERROR_ORE | HAL_UART_ERROR_RTO;
    const bool dma_error =
        ((huart->ErrorCode & HAL_UART_ERROR_DMA) != 0U) ||
        ((huart->hdmarx != nullptr) &&
         (huart->hdmarx->ErrorCode != HAL_DMA_ERROR_NONE)) ||
        ((huart->hdmatx != nullptr) &&
         (huart->hdmatx->ErrorCode != HAL_DMA_ERROR_NONE));
    const bool tx_dma_was_active =
        (READ_BIT(huart->Instance->CR3, USART_CR3_DMAT) != 0U);

    uint32_t events = 0U;
    if ((huart->ErrorCode & kReceiveErrorMask) != 0U) {
      events |= kRxErrorEvent;
    }
    if (dma_error) {
      // UART_DMAError leaves DMAT set when it interrupted an active TX DMA.
      // Gate the TX event on that bit so an RX-only fault cannot be consumed
      // as the error generation of a newer transfer started by the task.
      events |= kRxErrorEvent;
      if (tx_dma_was_active) {
        events |= kTxErrorEvent;
      }
    }
    if (events == 0U) {
      events = kRxErrorEvent;
    }
    instance->NotifyTaskFromIsr(events);
  }
}

void MavlinkRxTx::TxCompleted(UART_HandleTypeDef* huart) {
  MavlinkRxTx* instance = mavlink_rxtx_instance_handle_;
  if ((instance != nullptr) && (huart == instance->mavlink_uart_)) {
    instance->NotifyTaskFromIsr(kTxCompleteEvent);
  }
}
