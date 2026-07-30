#pragma once

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
  }
  return MAV_PARAM_TYPE_ENUM_END;
}

constexpr bool IsCompatibleWireType(
    parameters::ParameterValueType expected_type,
    std::uint8_t received_type) noexcept {
  return received_type ==
         static_cast<std::uint8_t>(ToWireType(expected_type));
}

}  // namespace mavlink_parameter_protocol
