#pragma once

#include <cstddef>
#include <cstdint>

// Persisted TopicID::RTOS_METRICS payload. Keep this structure exactly 128
// bytes: the binary log framing has no payload-length field. An incompatible
// future layout must use a new TopicID rather than changing this size.
struct __attribute__((__packed__)) RtosMetricsData {
  static constexpr std::uint8_t kSchemaVersion = 1U;

  std::uint8_t schema_version;
  std::uint8_t task_index;
  std::uint8_t task_count;
  std::int8_t task_priority;
  char task_name[16];

  std::uint32_t flags;
  std::uint32_t snapshot_sequence;
  std::uint32_t core_clock_hz;
  std::uint32_t period_us;
  std::uint32_t deadline_us;
  std::uint32_t release_count;
  std::uint32_t last_elapsed_cycles;
  std::uint32_t max_elapsed_cycles;
  std::int32_t last_release_jitter_cycles;
  std::uint32_t max_abs_release_jitter_cycles;
  std::uint32_t max_release_lateness_cycles;
  std::uint32_t max_response_cycles;
  std::uint32_t deadline_miss_count;
  std::uint32_t schedule_discontinuity_count;
  std::uint32_t context_switch_count;
  std::uint32_t context_switch_delta;
  std::uint32_t context_sample_interval_ms;
  std::uint32_t stack_size_bytes;
  std::uint32_t stack_min_free_bytes;
  std::uint32_t dwt_read_overhead_cycles;

  // Reserved now so future additive metrics do not alter the record size.
  std::uint32_t reserved[5];
  std::uint64_t timestamp_ms;
};

static_assert(sizeof(RtosMetricsData) == 128U,
              "RTOS metrics log payload size is persistent ABI");
static_assert(offsetof(RtosMetricsData, flags) == 20U);
static_assert(offsetof(RtosMetricsData, last_release_jitter_cycles) == 52U);
static_assert(offsetof(RtosMetricsData, timestamp_ms) == 120U);

