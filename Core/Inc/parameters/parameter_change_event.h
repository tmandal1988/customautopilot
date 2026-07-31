#pragma once

#include <cstdint>

#include "parameters/parameter_types.h"

namespace parameters {

enum class ParameterChangeKind : std::uint8_t {
  AppliedLive = 0U,
  StagedForReboot,
  ConfiguredSnapshot,
  EventsDropped,
};

// Persisted as the payload of TopicID::PARAMETER_UPDATE. The stable name makes
// a flight log self-describing even if catalog ordering changes in a later
// firmware build. Values retain their native bits; `type` controls decoding.
struct ParameterChangeEvent {
  char name[kParameterIdMaxLength];
  std::uint64_t timestamp_ms;
  std::uint32_t previous_value_bits;
  std::uint32_t value_bits;
  // Effective module value at the event boundary. For OnReboot staging this
  // deliberately remains different from the new configured `value_bits`.
  std::uint32_t active_value_bits;
  // Monotonic producer order and the application epoch acquired at the owner
  // boundary. Together these make START/STOP boundaries and missing-event
  // markers exact; millisecond timestamp equality is never used for ordering.
  std::uint32_t sequence;
  std::uint32_t session_epoch;
  // Revision of the configured/active bank represented by this record. A
  // snapshot taken concurrently with an update can therefore state exactly
  // whether that update is already incorporated in each parameter row.
  std::uint32_t state_revision;
  std::uint16_t catalog_index;
  ParameterValueType type;
  ParameterChangeKind kind;
};

static_assert(sizeof(ParameterChangeEvent) == 56U);

}  // namespace parameters
