#pragma once

#include <atomic>
#include <cstdint>

#include "cmsis_os.h"
#include "messages/rtos_metrics_data.h"
#include "stm32h7xx.h"

#ifndef RTOS_METRICS_ENABLE
#define RTOS_METRICS_ENABLE 0
#endif
#ifndef RTOS_CONTEXT_SWITCH_METRICS_ENABLE
#define RTOS_CONTEXT_SWITCH_METRICS_ENABLE 0
#endif
#ifndef RTOS_METRICS_LOGGING_ENABLE
#define RTOS_METRICS_LOGGING_ENABLE 0
#endif
#ifndef RTOS_STACK_WATERMARK_METRICS_ENABLE
#define RTOS_STACK_WATERMARK_METRICS_ENABLE 0
#endif

namespace rtos_metrics {

static_assert(std::atomic<std::uint32_t>::is_always_lock_free,
              "metrics counters must not call an atomic runtime helper");
static_assert(std::atomic<std::int32_t>::is_always_lock_free,
              "metrics counters must not call an atomic runtime helper");

enum MetricFlags : std::uint32_t {
  kDwtAvailable = 1U << 0,
  kPeriodicExecutor = 1U << 1,
  kHasExecutionSample = 1U << 2,
  kStackSampled = 1U << 3,
  kPeriodicTimingUnsupported = 1U << 4,
  kContextSwitchCounterEnabled = 1U << 5,
  kStackWatermarkEnabled = 1U << 6,
};

// Called once after SystemClock_Config() and before any task can run.
void Initialize() noexcept;
std::uint32_t ContextSwitchCount() noexcept;
std::uint32_t DwtReadOverheadCycles() noexcept;

#if RTOS_METRICS_ENABLE
extern bool g_dwt_ready;

inline std::uint32_t CyclesNow() noexcept {
  // A compiler barrier prevents code motion across the sample. DSB/ISB are
  // intentionally not used here because their cost would perturb every job.
  __asm volatile("" ::: "memory");
  const std::uint32_t cycles = DWT->CYCCNT;
  __asm volatile("" ::: "memory");
  return cycles;
}
#endif

class TaskMetrics {
 public:
  TaskMetrics(const char* task_name, std::uint32_t stack_size_bytes,
              osPriority_t priority) noexcept;

  void ConfigurePeriodic(std::uint32_t period_us,
                         std::uint32_t deadline_us) noexcept;
  void ConfigureEventDriven() noexcept;
  void ResetPeriodicPhase() noexcept;
  void MarkScheduleDiscontinuity() noexcept;

  // Kept out-of-line deliberately. Inlining the bookkeeping into every
  // executor made its local frame live across the entire task body, reducing
  // stack headroom at the deepest driver/control calls.
  [[gnu::noinline]] void BeginCycle() noexcept;
  [[gnu::noinline]] void CompleteCycle() noexcept;

  void RefreshStackHighWaterMark(osThreadId_t task_handle) noexcept;
  void Capture(std::uint8_t task_index, std::uint8_t task_count,
               std::uint32_t snapshot_sequence,
               std::uint32_t context_switch_count,
               std::uint32_t context_switch_delta,
               std::uint32_t context_interval_ms,
               RtosMetricsData* output) const noexcept;

 private:
#if RTOS_METRICS_ENABLE
  static constexpr std::uint32_t kEventDriven = 0U;
  static constexpr std::uint32_t kPeriodicTimingValid = 1U;
  static constexpr std::uint32_t kPeriodicTimingInvalid = 2U;

  static inline void Increment(std::atomic<std::uint32_t>& value) noexcept {
    value.store(value.load(std::memory_order_relaxed) + 1U,
                std::memory_order_relaxed);
  }

  static inline void UpdateMaximum(std::atomic<std::uint32_t>& maximum,
                                   std::uint32_t candidate) noexcept {
    if (candidate > maximum.load(std::memory_order_relaxed)) {
      maximum.store(candidate, std::memory_order_relaxed);
    }
  }

  const char* task_name_;
  std::uint32_t stack_size_bytes_;
  osPriority_t priority_;
  std::atomic<std::uint32_t> period_us_{0U};
  std::atomic<std::uint32_t> deadline_us_{0U};
  std::uint32_t period_cycles_ = 0U;
  std::uint32_t deadline_cycles_ = 0U;
  std::uint32_t expected_release_cycles_ = 0U;
  std::uint32_t active_start_cycles_ = 0U;
  std::uint32_t active_expected_release_cycles_ = 0U;
  // DWT signed phase deltas are unambiguous only below half a 32-bit wrap.
  // Mode 2 retains elapsed-time samples but suppresses misleading periodic
  // jitter/response/deadline results for an out-of-range future executor.
  std::atomic<std::uint32_t> periodic_mode_{kEventDriven};
  bool phase_valid_ = false;
  bool initial_warmup_pending_ = false;

  std::atomic<std::uint32_t> release_count_{0U};
  std::atomic<std::uint32_t> last_elapsed_cycles_{0U};
  std::atomic<std::uint32_t> max_elapsed_cycles_{0U};
  std::atomic<std::int32_t> last_release_jitter_cycles_{0};
  std::atomic<std::uint32_t> max_abs_release_jitter_cycles_{0U};
  std::atomic<std::uint32_t> max_release_lateness_cycles_{0U};
  std::atomic<std::uint32_t> max_response_cycles_{0U};
  std::atomic<std::uint32_t> deadline_miss_count_{0U};
  std::atomic<std::uint32_t> schedule_discontinuity_count_{0U};
  std::atomic<std::uint32_t> stack_min_free_bytes_{UINT32_MAX};
#endif
};

}  // namespace rtos_metrics
