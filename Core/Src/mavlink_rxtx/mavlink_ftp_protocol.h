#pragma once

#include <cstddef>
#include <cstdint>

extern "C" {
#include "mavlink/common/mavlink.h"
}

namespace mavlink_ftp_protocol {

// MAVLink FTP (https://mavlink.io/en/services/ftp.html) rides inside
// FILE_TRANSFER_PROTOCOL.payload: a fixed 12-byte little-endian header
// followed by up to 239 data bytes.
inline constexpr std::size_t kPayloadSize =
    MAVLINK_MSG_FILE_TRANSFER_PROTOCOL_FIELD_PAYLOAD_LEN;
inline constexpr std::size_t kHeaderSize = 12U;
inline constexpr std::size_t kMaxDataSize = kPayloadSize - kHeaderSize;

static_assert(kPayloadSize == 251U);
static_assert(kMaxDataSize == 239U);

enum class Opcode : std::uint8_t {
  None = 0U,
  TerminateSession = 1U,
  ResetSessions = 2U,
  ListDirectory = 3U,
  OpenFileRO = 4U,
  ReadFile = 5U,
  CreateFile = 6U,
  WriteFile = 7U,
  RemoveFile = 8U,
  CreateDirectory = 9U,
  RemoveDirectory = 10U,
  OpenFileWO = 11U,
  TruncateFile = 12U,
  Rename = 13U,
  CalcFileCRC32 = 14U,
  BurstReadFile = 15U,
  Ack = 128U,
  Nak = 129U,
};

enum class NakError : std::uint8_t {
  None = 0U,
  Fail = 1U,
  FailErrno = 2U,
  InvalidDataSize = 3U,
  InvalidSession = 4U,
  NoSessionsAvailable = 5U,
  Eof = 6U,
  UnknownCommand = 7U,
  FileExists = 8U,
  FileProtected = 9U,
  FileNotFound = 10U,
};

struct Header {
  std::uint16_t seq_number{0U};
  std::uint8_t session{0U};
  Opcode opcode{Opcode::None};
  std::uint8_t size{0U};
  Opcode req_opcode{Opcode::None};
  std::uint8_t burst_complete{0U};
  std::uint32_t offset{0U};
};

inline Header DecodeHeader(const std::uint8_t* payload) noexcept {
  Header header{};
  header.seq_number = static_cast<std::uint16_t>(
      payload[0] | (static_cast<std::uint16_t>(payload[1]) << 8U));
  header.session = payload[2];
  header.opcode = static_cast<Opcode>(payload[3]);
  header.size = payload[4];
  header.req_opcode = static_cast<Opcode>(payload[5]);
  header.burst_complete = payload[6];
  header.offset = static_cast<std::uint32_t>(payload[8]) |
                  (static_cast<std::uint32_t>(payload[9]) << 8U) |
                  (static_cast<std::uint32_t>(payload[10]) << 16U) |
                  (static_cast<std::uint32_t>(payload[11]) << 24U);
  return header;
}

inline void EncodeHeader(const Header& header, std::uint8_t* payload) noexcept {
  payload[0] = static_cast<std::uint8_t>(header.seq_number & 0xFFU);
  payload[1] = static_cast<std::uint8_t>(header.seq_number >> 8U);
  payload[2] = header.session;
  payload[3] = static_cast<std::uint8_t>(header.opcode);
  payload[4] = header.size;
  payload[5] = static_cast<std::uint8_t>(header.req_opcode);
  payload[6] = header.burst_complete;
  payload[7] = 0U;
  payload[8] = static_cast<std::uint8_t>(header.offset & 0xFFU);
  payload[9] = static_cast<std::uint8_t>((header.offset >> 8U) & 0xFFU);
  payload[10] = static_cast<std::uint8_t>((header.offset >> 16U) & 0xFFU);
  payload[11] = static_cast<std::uint8_t>((header.offset >> 24U) & 0xFFU);
}

}  // namespace mavlink_ftp_protocol
