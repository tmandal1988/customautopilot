#pragma once

#include <bit>
#include <cstdint>

#include "parameters/parameter_types.h"

extern "C" {
#include "mavlink/common/mavlink.h"
}

namespace mavlink_parameter_protocol {

// MAVLink routing rules: system zero is a network broadcast; component zero
// addresses every component of a matching system.
constexpr bool IsTarget(std::uint8_t target_system,
                        std::uint8_t target_component,
                        std::uint8_t local_system,
                        std::uint8_t local_component) noexcept {
  return (target_system == 0U) ||
         ((target_system == local_system) &&
          ((target_component == MAV_COMP_ID_ALL) ||
           (target_component == local_component)));
}

constexpr MAV_PARAM_TYPE ToWireType(
    parameters::ParameterValueType type) noexcept {
  switch (type) {
    case parameters::ParameterValueType::Real32:
      return MAV_PARAM_TYPE_REAL32;
    case parameters::ParameterValueType::Int32:
      return MAV_PARAM_TYPE_INT32;
    case parameters::ParameterValueType::Bool:
      return MAV_PARAM_TYPE_UINT8;
  }
  return MAV_PARAM_TYPE_ENUM_END;
}

constexpr bool IsCompatibleWireType(
    parameters::ParameterValueType expected_type,
    std::uint8_t received_type) noexcept {
  return received_type ==
         static_cast<std::uint8_t>(ToWireType(expected_type));
}

// This implementation uses the MAVLink standard byte-wise encoding. Integer
// bytes occupy the legacy float field unchanged; they must never be converted
// numerically through float, which would lose INT32 precision.
constexpr float EncodeValue(parameters::ParameterValue value) noexcept {
  return std::bit_cast<float>(value.bits);
}

constexpr parameters::ParameterValue DecodeValue(float wire_value) noexcept {
  return parameters::ParameterValue{
      std::bit_cast<std::uint32_t>(wire_value)};
}

inline constexpr std::uint64_t kParameterProtocolCapabilities =
    MAV_PROTOCOL_CAPABILITY_PARAM_ENCODE_BYTEWISE;

static_assert(DecodeValue(EncodeValue(
                  parameters::MakeInt32Value(INT32_MIN))).bits ==
              parameters::MakeInt32Value(INT32_MIN).bits);
static_assert(DecodeValue(EncodeValue(
                  parameters::MakeInt32Value(INT32_MAX))).bits ==
              parameters::MakeInt32Value(INT32_MAX).bits);
static_assert(DecodeValue(EncodeValue(
                  parameters::MakeBoolValue(false))).bits == 0U);
static_assert(DecodeValue(EncodeValue(
                  parameters::MakeBoolValue(true))).bits == 1U);
static_assert((kParameterProtocolCapabilities &
               MAV_PROTOCOL_CAPABILITY_PARAM_ENCODE_BYTEWISE) != 0U);

}  // namespace mavlink_parameter_protocol
