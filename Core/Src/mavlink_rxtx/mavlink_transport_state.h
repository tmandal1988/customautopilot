#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace mavlink_transport {

enum class DmaRxEvent : std::uint8_t {
  Idle = 0U,
  HalfTransfer,
  TransferComplete,
};

struct DmaRxProgress {
  std::uint16_t position;
  std::uint16_t new_bytes;
  bool valid;
};

// HAL reports a cumulative position in the circular buffer, not a byte count.
// Half/full-transfer interrupts guarantee at least one event per half-buffer.
// Duplicate IDLE/HT/TC callbacks at the same position contribute zero bytes.
template <std::size_t Capacity>
constexpr DmaRxProgress AdvanceDmaRxProgress(
    std::uint16_t previous_position, std::uint16_t event_size,
    DmaRxEvent event) noexcept {
  static_assert(Capacity > 0U);
  static_assert((Capacity & (Capacity - 1U)) == 0U);
  static_assert(Capacity <= std::numeric_limits<std::uint16_t>::max());

  if ((previous_position >= Capacity) || (event_size == 0U) ||
      (event_size > Capacity)) {
    return {previous_position, 0U, false};
  }

  const auto position = static_cast<std::uint16_t>(
      (event_size == Capacity) ? 0U : event_size);
  std::uint16_t new_bytes = static_cast<std::uint16_t>(
      (static_cast<std::size_t>(position) + Capacity -
       previous_position) & (Capacity - 1U));
  (void)event;
  return {position, new_bytes, true};
}

struct RxDrainPlan {
  std::uint32_t start;
  std::uint32_t dropped;
  std::uint16_t count;
  bool overrun;
};

// Producer and consumer are monotonic uint32 counters, so subtraction remains
// valid across wrap as long as the producer is less than 2^31 bytes ahead.
template <std::size_t Capacity, std::size_t Budget>
constexpr RxDrainPlan PlanRxDrain(std::uint32_t produced,
                                  std::uint32_t consumed) noexcept {
  static_assert(Capacity > 0U);
  static_assert((Capacity & (Capacity - 1U)) == 0U);
  static_assert(Budget > 0U);
  static_assert(Budget <= Capacity);
  static_assert(Budget <= std::numeric_limits<std::uint16_t>::max());

  const std::uint32_t available = produced - consumed;
  if (available >= Capacity) {
    // A live circular DMA at exact capacity is already at the overwrite
    // boundary. Drop the whole snapshot and reset the stream parser rather
    // than race the DMA writer or feed it a corrupted frame tail.
    return {produced, available, 0U, true};
  }

  const std::uint32_t count =
      (available < Budget) ? available : static_cast<std::uint32_t>(Budget);
  return {consumed, 0U, static_cast<std::uint16_t>(count), false};
}

constexpr bool DeadlineReached(std::uint32_t now,
                               std::uint32_t deadline) noexcept {
  return (now - deadline) < 0x80000000UL;
}

constexpr std::uint32_t TicksUntil(std::uint32_t now,
                                   std::uint32_t deadline) noexcept {
  return DeadlineReached(now, deadline) ? 0U : (deadline - now);
}

class ParameterListCursor final {
 public:
  enum class State : std::uint8_t {
    Idle = 0U,
    WaitingForReady,
    Streaming,
  };

  void Request(std::uint32_t now) noexcept {
    if (state_ == State::Idle) {
      state_ = State::WaitingForReady;
      next_due_ = now;
    } else if (state_ == State::Streaming) {
      // Finish the current pass before honoring one coalesced repeat request.
      restart_pending_ = true;
    }
  }

  bool ReadyToSend(std::uint32_t now, bool store_ready,
                   std::uint16_t parameter_count,
                   std::uint32_t retry_interval) noexcept {
    if ((state_ == State::Idle) || !DeadlineReached(now, next_due_)) {
      return false;
    }

    if (state_ == State::WaitingForReady) {
      if (!store_ready) {
        next_due_ = now + retry_interval;
        return false;
      }
      if (parameter_count == 0U) {
        Reset();
        return false;
      }
      count_ = parameter_count;
      next_ = 0U;
      state_ = State::Streaming;
    }
    return true;
  }

  void CommitQueued(std::uint32_t now,
                    std::uint32_t interval) noexcept {
    if (state_ != State::Streaming) {
      return;
    }

    ++next_;
    next_due_ = now + interval;
    if (next_ >= count_) {
      if (restart_pending_) {
        restart_pending_ = false;
        next_ = 0U;
      } else {
        Reset();
      }
    }
  }

  void Defer(std::uint32_t now, std::uint32_t interval) noexcept {
    if (state_ != State::Idle) {
      next_due_ = now + interval;
    }
  }

  void Abort() noexcept { Reset(); }

  State state() const noexcept { return state_; }
  std::uint16_t next() const noexcept { return next_; }
  std::uint16_t count() const noexcept { return count_; }
  bool restart_pending() const noexcept { return restart_pending_; }
  bool has_deadline() const noexcept { return state_ != State::Idle; }
  std::uint32_t next_due() const noexcept { return next_due_; }

 private:
  void Reset() noexcept {
    state_ = State::Idle;
    next_ = 0U;
    count_ = 0U;
    restart_pending_ = false;
    next_due_ = 0U;
  }

  State state_{State::Idle};
  std::uint16_t next_{0U};
  std::uint16_t count_{0U};
  bool restart_pending_{false};
  std::uint32_t next_due_{0U};
};

}  // namespace mavlink_transport
