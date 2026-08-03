#include "rtos_metrics/rtos_metrics.h"

#include <cstring>
#include <limits>

namespace {
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
void* g_rtos_previous_tcb = nullptr;
std::atomic<std::uint32_t> g_rtos_context_switch_count{0U};
#endif
}  // namespace

extern "C" void RtosMetricsTraceTaskSwitchedIn(void* next_tcb) noexcept {
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
  void* const previous_tcb = g_rtos_previous_tcb;
  if (previous_tcb != next_tcb) {
    // Do not count the scheduler's initial selection as a context switch.
    if (previous_tcb != nullptr) {
      g_rtos_context_switch_count.store(
          g_rtos_context_switch_count.load(std::memory_order_relaxed) + 1U,
          std::memory_order_relaxed);
    }
    g_rtos_previous_tcb = next_tcb;
  }
#else
  static_cast<void>(next_tcb);
#endif
}

namespace rtos_metrics {

#if RTOS_METRICS_ENABLE
bool g_dwt_ready = false;
namespace {
std::uint32_t g_dwt_read_overhead_cycles = 0U;

std::uint32_t MicrosecondsToCycles(std::uint32_t microseconds) noexcept {
  const std::uint64_t cycles =
      (static_cast<std::uint64_t>(SystemCoreClock) * microseconds) /
      1000000ULL;
  if (cycles == 0U) {
    return 1U;
  }
  if (cycles > std::numeric_limits<std::uint32_t>::max()) {
    return std::numeric_limits<std::uint32_t>::max();
  }
  return static_cast<std::uint32_t>(cycles);
}
}  // namespace
#endif

void Initialize() noexcept {
#if RTOS_METRICS_ENABLE
  g_dwt_ready = false;
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->LAR = 0xC5ACCE55UL;
  DWT->CYCCNT = 0U;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
  __DSB();
  __ISB();

  if (((DWT->CTRL & DWT_CTRL_NOCYCCNT_Msk) == 0U) &&
      ((DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk) != 0U)) {
    const std::uint32_t first = CyclesNow();
    __NOP();
    const std::uint32_t second = CyclesNow();
    g_dwt_read_overhead_cycles = second - first;
    g_dwt_ready = second != first;
  }
#endif
}

std::uint32_t ContextSwitchCount() noexcept {
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
  return g_rtos_context_switch_count.load(std::memory_order_relaxed);
#else
  return 0U;
#endif
}

std::uint32_t DwtReadOverheadCycles() noexcept {
#if RTOS_METRICS_ENABLE
  return g_dwt_read_overhead_cycles;
#else
  return 0U;
#endif
}

TaskMetrics::TaskMetrics(const char* task_name,
                         std::uint32_t stack_size_bytes,
                         osPriority_t priority) noexcept
#if RTOS_METRICS_ENABLE
    : task_name_(task_name),
      stack_size_bytes_(stack_size_bytes),
      priority_(priority)
#endif
{
#if !RTOS_METRICS_ENABLE
  static_cast<void>(task_name);
  static_cast<void>(stack_size_bytes);
  static_cast<void>(priority);
#endif
}

void TaskMetrics::ConfigurePeriodic(std::uint32_t period_us,
                                    std::uint32_t deadline_us) noexcept {
#if RTOS_METRICS_ENABLE
  const std::uint32_t applied_deadline_us =
      (deadline_us == 0U) ? period_us : deadline_us;
  period_us_.store(period_us, std::memory_order_relaxed);
  deadline_us_.store(applied_deadline_us, std::memory_order_relaxed);
  period_cycles_ = MicrosecondsToCycles(period_us);
  deadline_cycles_ = MicrosecondsToCycles(applied_deadline_us);
  phase_valid_ = false;
  initial_warmup_pending_ = true;
  constexpr std::uint32_t kMaximumSignedDelta =
      static_cast<std::uint32_t>(
          std::numeric_limits<std::int32_t>::max());
  const bool timing_range_supported =
      (period_cycles_ <= kMaximumSignedDelta) &&
      (deadline_cycles_ <= kMaximumSignedDelta);
  periodic_mode_.store(timing_range_supported ? kPeriodicTimingValid
                                              : kPeriodicTimingInvalid,
                       std::memory_order_release);
#else
  static_cast<void>(period_us);
  static_cast<void>(deadline_us);
#endif
}

void TaskMetrics::ConfigureEventDriven() noexcept {
#if RTOS_METRICS_ENABLE
  period_us_.store(0U, std::memory_order_relaxed);
  deadline_us_.store(0U, std::memory_order_relaxed);
  period_cycles_ = 0U;
  deadline_cycles_ = 0U;
  phase_valid_ = false;
  initial_warmup_pending_ = false;
  periodic_mode_.store(kEventDriven, std::memory_order_release);
#endif
}

void TaskMetrics::BeginCycle() noexcept {
#if RTOS_METRICS_ENABLE
  if (!g_dwt_ready) {
    return;
  }

  const std::uint32_t now = CyclesNow();
  active_start_cycles_ = now;
  active_expected_release_cycles_ = now;

  if (periodic_mode_.load(std::memory_order_relaxed) ==
      kPeriodicTimingValid) {
    if (!phase_valid_ && initial_warmup_pending_) {
      // Current loops run once immediately before their first
      // vTaskDelayUntil(). Measure that dispatch against its own start, then
      // establish the DWT phase on the first genuinely scheduled wake.
      initial_warmup_pending_ = false;
    } else {
      if (!phase_valid_) {
        expected_release_cycles_ = now;
        phase_valid_ = true;
      }

      const std::int32_t jitter =
          static_cast<std::int32_t>(now - expected_release_cycles_);
      const std::uint32_t positive_jitter =
          (jitter > 0) ? static_cast<std::uint32_t>(jitter) : 0U;
      const std::uint32_t absolute_jitter =
          (jitter < 0) ? (0U - static_cast<std::uint32_t>(jitter))
                       : static_cast<std::uint32_t>(jitter);

      last_release_jitter_cycles_.store(jitter, std::memory_order_relaxed);
      UpdateMaximum(max_abs_release_jitter_cycles_, absolute_jitter);
      UpdateMaximum(max_release_lateness_cycles_, positive_jitter);

      // Do not silently rebase after an overrun: vTaskDelayUntil() retains
      // its phase, so doing so here would hide deadline misses. Recovery paths
      // that intentionally reset their RTOS schedule mark the discontinuity.
      active_expected_release_cycles_ = expected_release_cycles_;
      expected_release_cycles_ += period_cycles_;
    }
  }

  Increment(release_count_);
#endif
}

void TaskMetrics::CompleteCycle() noexcept {
#if RTOS_METRICS_ENABLE
  if (!g_dwt_ready) {
    return;
  }

  const std::uint32_t finish = CyclesNow();
  const std::uint32_t elapsed = finish - active_start_cycles_;
  last_elapsed_cycles_.store(elapsed, std::memory_order_relaxed);
  UpdateMaximum(max_elapsed_cycles_, elapsed);

  if (periodic_mode_.load(std::memory_order_relaxed) ==
      kPeriodicTimingValid) {
    const std::int32_t signed_response = static_cast<std::int32_t>(
        finish - active_expected_release_cycles_);
    // A job can start slightly before the DWT phase established by the first
    // tick (for example, if the first dispatch was more heavily preempted).
    // Clamp an early completion instead of interpreting it as uint32 wrap.
    const std::uint32_t response =
        (signed_response > 0) ? static_cast<std::uint32_t>(signed_response)
                              : 0U;
    UpdateMaximum(max_response_cycles_, response);

    if ((signed_response > 0) &&
        (static_cast<std::uint32_t>(signed_response) > deadline_cycles_)) {
      Increment(deadline_miss_count_);
    }
  }
#endif
}

void TaskMetrics::ResetPeriodicPhase() noexcept {
#if RTOS_METRICS_ENABLE
  phase_valid_ = false;
  initial_warmup_pending_ = false;
#endif
}

void TaskMetrics::MarkScheduleDiscontinuity() noexcept {
#if RTOS_METRICS_ENABLE
  Increment(schedule_discontinuity_count_);
  phase_valid_ = false;
  initial_warmup_pending_ = false;
#endif
}

void TaskMetrics::RefreshStackHighWaterMark(
    osThreadId_t task_handle) noexcept {
#if RTOS_METRICS_ENABLE
  if (task_handle != nullptr) {
    const std::uint32_t sampled_free_bytes =
        osThreadGetStackSpace(task_handle);
    // The fill-pattern scan is deliberately preemptible. Preserve the lowest
    // result ever seen so a later raced scan can never make the reported
    // headroom look safer than an earlier sample. Blink is the only writer,
    // so this needs no compare/exchange loop.
    if (sampled_free_bytes <
        stack_min_free_bytes_.load(std::memory_order_relaxed)) {
      stack_min_free_bytes_.store(sampled_free_bytes,
                                  std::memory_order_relaxed);
    }
  }
#else
  static_cast<void>(task_handle);
#endif
}

void TaskMetrics::SetAuxMetric(std::uint8_t index,
                               std::uint32_t value) noexcept {
#if RTOS_METRICS_ENABLE
  if (index < 5U) {
    aux_metrics_[index].store(value, std::memory_order_relaxed);
  }
#else
  static_cast<void>(index);
  static_cast<void>(value);
#endif
}

void TaskMetrics::UpdateAuxMetricMaximum(
    std::uint8_t index, std::uint32_t candidate) noexcept {
#if RTOS_METRICS_ENABLE
  if (index < 5U) {
    UpdateMaximum(aux_metrics_[index], candidate);
  }
#else
  static_cast<void>(index);
  static_cast<void>(candidate);
#endif
}

void TaskMetrics::Capture(
    std::uint8_t task_index, std::uint8_t task_count,
    std::uint32_t snapshot_sequence, std::uint32_t context_switch_count,
    std::uint32_t context_switch_delta, std::uint32_t context_interval_ms,
    RtosMetricsData* output) const noexcept {
  if (output == nullptr) {
    return;
  }
  std::memset(output, 0, sizeof(*output));
  output->schema_version = RtosMetricsData::kSchemaVersion;
  output->task_index = task_index;
  output->task_count = task_count;

#if RTOS_METRICS_ENABLE
  output->task_priority = static_cast<std::int8_t>(priority_);
  std::strncpy(output->task_name, task_name_, sizeof(output->task_name) - 1U);
  output->flags = g_dwt_ready ? kDwtAvailable : 0U;
#if RTOS_CONTEXT_SWITCH_METRICS_ENABLE
  output->flags |= kContextSwitchCounterEnabled;
#endif
#if RTOS_STACK_WATERMARK_METRICS_ENABLE
  output->flags |= kStackWatermarkEnabled;
#endif
  const std::uint32_t periodic_mode =
      periodic_mode_.load(std::memory_order_acquire);
  if (periodic_mode != kEventDriven) {
    output->flags |= kPeriodicExecutor;
  }
  if (periodic_mode == kPeriodicTimingInvalid) {
    output->flags |= kPeriodicTimingUnsupported;
  }
  if (release_count_.load(std::memory_order_relaxed) != 0U) {
    output->flags |= kHasExecutionSample;
  }
  const std::uint32_t aux0 = aux_metrics_[0].load(std::memory_order_relaxed);
  const std::uint32_t aux1 = aux_metrics_[1].load(std::memory_order_relaxed);
  const std::uint32_t aux2 = aux_metrics_[2].load(std::memory_order_relaxed);
  const std::uint32_t aux3 = aux_metrics_[3].load(std::memory_order_relaxed);
  const std::uint32_t aux4 = aux_metrics_[4].load(std::memory_order_relaxed);
  if ((aux0 | aux1 | aux2 | aux3 | aux4) != 0U) {
    output->flags |= kHasAuxMetrics;
    output->reserved[0] = aux0;
    output->reserved[1] = aux1;
    output->reserved[2] = aux2;
    output->reserved[3] = aux3;
    output->reserved[4] = aux4;
  }
  const std::uint32_t stack_free =
      stack_min_free_bytes_.load(std::memory_order_relaxed);
  if (stack_free != UINT32_MAX) {
    output->flags |= kStackSampled;
    output->stack_min_free_bytes = stack_free;
  }

  output->snapshot_sequence = snapshot_sequence;
  output->core_clock_hz = SystemCoreClock;
  output->period_us = period_us_.load(std::memory_order_relaxed);
  output->deadline_us = deadline_us_.load(std::memory_order_relaxed);
  output->release_count = release_count_.load(std::memory_order_relaxed);
  output->last_elapsed_cycles =
      last_elapsed_cycles_.load(std::memory_order_relaxed);
  output->max_elapsed_cycles =
      max_elapsed_cycles_.load(std::memory_order_relaxed);
  output->last_release_jitter_cycles =
      last_release_jitter_cycles_.load(std::memory_order_relaxed);
  output->max_abs_release_jitter_cycles =
      max_abs_release_jitter_cycles_.load(std::memory_order_relaxed);
  output->max_release_lateness_cycles =
      max_release_lateness_cycles_.load(std::memory_order_relaxed);
  output->max_response_cycles =
      max_response_cycles_.load(std::memory_order_relaxed);
  output->deadline_miss_count =
      deadline_miss_count_.load(std::memory_order_relaxed);
  output->schedule_discontinuity_count =
      schedule_discontinuity_count_.load(std::memory_order_relaxed);
  output->context_switch_count = context_switch_count;
  output->context_switch_delta = context_switch_delta;
  output->context_sample_interval_ms = context_interval_ms;
  output->stack_size_bytes = stack_size_bytes_;
  output->dwt_read_overhead_cycles = DwtReadOverheadCycles();
#else
  static_cast<void>(snapshot_sequence);
  static_cast<void>(context_switch_count);
  static_cast<void>(context_switch_delta);
  static_cast<void>(context_interval_ms);
#endif
}

}  // namespace rtos_metrics
