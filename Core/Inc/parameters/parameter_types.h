#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "parameter_domains.h"

namespace parameters {

inline constexpr std::size_t kParameterIdMaxLength = 16U;

enum class ParameterValueType : std::uint8_t {
  Real32 = 0U,
  Int32,
  Bool,
};

// Activation and persistence are deliberately independent. Live values are
// handed to their owning task through a fixed queue; OnReboot values update
// only the configured bank and become active when that module starts again.
enum class ParameterActivationPolicy : std::uint8_t {
  Live = 0U,
  OnReboot,
};

// Persistence is independent of live application. Volatile parameters always
// restart from their module-supplied defaults; retained overrides may overlay
// those defaults during boot and are the only values admitted to Flash.
enum class ParameterPersistencePolicy : std::uint8_t {
  Volatile = 0U,
  RetainedOverride,
};

// Supplied by the FCS owner from the model's authoritative state-machine
// output. Unknown deliberately fails closed for Flash operations. InFlight
// covers every armed state, not only INFLIGHT: an in-progress sector erase
// cannot be aborted, so Flash may only start while the vehicle is INACTIVE.
enum class PersistenceFlightState : std::uint8_t {
  Unknown = 0U,
  NotInFlight,
  InFlight,
};

struct ParameterDescriptor {
  // MAVLink PARAM_* packers read all 16 wire bytes. Owning zero-padded
  // storage keeps that fixed-width read inside the descriptor object while
  // retaining a trailing NUL for local string use.
  char name[kParameterIdMaxLength + 1U];
  ParameterDomain domain;
  ParameterValueType type;
  ParameterActivationPolicy activation_policy;
  // Bounds retain the parameter's native 32-bit representation. This avoids
  // lossy integer-to-float metadata conversions while preserving a compact,
  // cache-friendly descriptor.
  std::uint32_t minimum_bits;
  std::uint32_t maximum_bits;
  std::uint16_t domain_index;
  ParameterPersistencePolicy persistence_policy;
};

static_assert(sizeof(ParameterDescriptor) == 32U);

enum class ParameterSubmitResult : std::uint8_t {
  Queued = 0U,
  Completed,
  NotReady,
  InvalidParameter,
  UnsupportedType,
  WrongDomain,
  NonFinite,
  OutOfRange,
  PersistenceUnavailable,
  QueueFull,
};

enum class ParameterUpdateStatus : std::uint8_t {
  Applied = 0U,
  StagedForReboot,
  RejectedNonFinite,
  RejectedOutOfRange,
  InternalError,
};

struct ParameterValue {
  std::uint32_t bits;
};

static_assert(sizeof(float) == sizeof(std::uint32_t));
static_assert(sizeof(std::int32_t) == sizeof(std::uint32_t));
static_assert(std::numeric_limits<float>::is_iec559,
              "Parameters require IEEE-754 binary32");

constexpr ParameterValue MakeReal32Value(float value) noexcept {
  return ParameterValue{std::bit_cast<std::uint32_t>(value)};
}

constexpr ParameterValue MakeInt32Value(std::int32_t value) noexcept {
  return ParameterValue{std::bit_cast<std::uint32_t>(value)};
}

constexpr ParameterValue MakeBoolValue(bool value) noexcept {
  return ParameterValue{value ? 1U : 0U};
}

constexpr float AsReal32(ParameterValue value) noexcept {
  return std::bit_cast<float>(value.bits);
}

constexpr std::int32_t AsInt32(ParameterValue value) noexcept {
  return std::bit_cast<std::int32_t>(value.bits);
}

constexpr bool AsBool(ParameterValue value) noexcept {
  return value.bits != 0U;
}

struct ParameterUpdateRequest {
  ParameterValue requested_value;
  std::uint32_t token;
  // Acquired only after an owner pops a real update. It classifies the
  // application atomically against a Logger START/STOP boundary; the empty
  // owner path never touches the logging-session counters.
  std::uint32_t application_session_epoch;
  std::uint16_t catalog_index;
  std::uint16_t domain_index;
};

struct ParameterUpdateCompletion {
  std::uint64_t timestamp_ms;
  ParameterValue value;
  ParameterValue previous_value;
  std::uint32_t token;
  std::uint32_t state_revision;
  std::uint32_t application_session_epoch;
  std::uint16_t catalog_index;
  ParameterUpdateStatus status;
  // One means RecordChange() owns a session lease and must release it after
  // the event has either entered the fixed queue or received a gap sequence.
  std::uint8_t change_record_lease;
};

static_assert(sizeof(ParameterUpdateRequest) == 16U);
static_assert(sizeof(ParameterUpdateCompletion) == 32U);

template <std::size_t N>
consteval bool IsValidParameterId(const char (&name)[N]) noexcept {
  if constexpr ((N <= 1U) || ((N - 1U) > kParameterIdMaxLength)) {
    return false;
  }

  for (std::size_t i = 0U; i < (N - 1U); ++i) {
    const char c = name[i];
    const bool is_uppercase = (c >= 'A') && (c <= 'Z');
    const bool is_digit = (c >= '0') && (c <= '9');
    if (!is_uppercase && !is_digit && (c != '_')) {
      return false;
    }
  }
  return true;
}

template <std::size_t N>
consteval std::uint32_t ParameterIdHash(const char (&name)[N]) noexcept {
  std::uint32_t hash = 2166136261UL;
  for (std::size_t i = 0U; i < (N - 1U); ++i) {
    hash ^= static_cast<std::uint8_t>(name[i]);
    hash *= 16777619UL;
  }
  return hash;
}

inline std::uint32_t ParameterIdHash(const char* name,
                                     std::size_t max_length) noexcept {
  if (name == nullptr) {
    return 0U;
  }

  const std::size_t length_limit =
      (max_length < kParameterIdMaxLength) ? max_length
                                           : kParameterIdMaxLength;
  std::uint32_t hash = 2166136261UL;
  for (std::size_t i = 0U; i < length_limit; ++i) {
    const char c = name[i];
    if (c == '\0') {
      break;
    }
    hash ^= static_cast<std::uint8_t>(c);
    hash *= 16777619UL;
  }
  return hash;
}

inline bool ParameterIdEquals(const char* received,
                              std::size_t received_max_length,
                              const char* expected) noexcept {
  if ((received == nullptr) || (expected == nullptr)) {
    return false;
  }

  const std::size_t length_limit =
      (received_max_length < kParameterIdMaxLength)
          ? received_max_length
          : kParameterIdMaxLength;
  for (std::size_t i = 0U; i < length_limit; ++i) {
    if (received[i] != expected[i]) {
      return false;
    }
    if (expected[i] == '\0') {
      return true;
    }
  }

  return expected[length_limit] == '\0';
}

}  // namespace parameters
