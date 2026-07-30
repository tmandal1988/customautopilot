#pragma once

#include <cstddef>
#include <cstdint>

namespace parameters {

inline constexpr std::size_t kParameterIdMaxLength = 16U;

enum class ParameterDomain : std::uint8_t {
  FlightControls = 0U,
};

enum class ParameterValueType : std::uint8_t {
  Real32 = 0U,
};

enum class ParameterUpdatePolicy : std::uint8_t {
  Always = 0U,
  DisarmedOnly,
  RebootRequired,
};

// Supplied by the domain owner at the exact update boundary. Unknown is
// deliberately distinct from Disarmed so safety-gated writes fail closed
// until an authoritative vehicle state has been observed.
enum class ParameterRuntimeState : std::uint8_t {
  Unknown = 0U,
  Disarmed,
  Armed,
};

struct ParameterDescriptor {
  // MAVLink PARAM_* packers read all 16 wire bytes. Owning zero-padded
  // storage keeps that fixed-width read inside the descriptor object while
  // retaining a trailing NUL for local string use.
  char name[kParameterIdMaxLength + 1U];
  ParameterDomain domain;
  ParameterValueType type;
  ParameterUpdatePolicy update_policy;
  float minimum_value;
  float maximum_value;
  std::uint16_t domain_index;
};

static_assert(sizeof(ParameterDescriptor) == 32U);

enum class ParameterSubmitResult : std::uint8_t {
  Queued = 0U,
  NotReady,
  InvalidParameter,
  UnsupportedType,
  WrongDomain,
  NonFinite,
  OutOfRange,
  RebootRequired,
  QueueFull,
};

enum class ParameterUpdateStatus : std::uint8_t {
  Applied = 0U,
  StagedForReboot,
  RejectedUnsafeState,
  RejectedNonFinite,
  RejectedOutOfRange,
  RejectedRebootRequired,
  InternalError,
};

struct ParameterUpdateRequest {
  float requested_value;
  std::uint32_t token;
  std::uint16_t catalog_index;
  std::uint16_t domain_index;
};

struct ParameterUpdateCompletion {
  float applied_value;
  std::uint32_t token;
  std::uint16_t catalog_index;
  ParameterUpdateStatus status;
  std::uint8_t reserved;
};

static_assert(sizeof(ParameterUpdateRequest) == 12U);
static_assert(sizeof(ParameterUpdateCompletion) == 12U);

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
