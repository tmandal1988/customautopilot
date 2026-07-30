#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace parameters {

// Fixed-capacity, task-to-task single-producer/single-consumer ring. The
// producer and consumer must each be unique; this is intentionally not an ISR
// or MPMC queue.
template <typename T, std::size_t Capacity>
class SpscRing final {
  static_assert(Capacity > 0U);
  static_assert((Capacity & (Capacity - 1U)) == 0U,
                "SPSC capacity must be a power of two");
  static_assert(Capacity <= 0x7FFFFFFFU,
                "SPSC capacity must fit monotonic-index arithmetic");
  static_assert(std::is_trivially_copyable_v<T>);
  static_assert(std::atomic<std::uint32_t>::is_always_lock_free,
                "The real-time SPSC queue requires lock-free 32-bit atomics");

 public:
  bool TryPush(const T& value) noexcept {
    const std::uint32_t head = head_.load(std::memory_order_relaxed);
    const std::uint32_t tail = tail_.load(std::memory_order_acquire);
    if ((head - tail) >= Capacity) {
      return false;
    }

    slots_[head & kIndexMask] = value;
    head_.store(head + 1U, std::memory_order_release);
    return true;
  }

  bool TryPop(T& value) noexcept {
    const std::uint32_t tail = tail_.load(std::memory_order_relaxed);
    const std::uint32_t head = head_.load(std::memory_order_acquire);
    if (tail == head) {
      return false;
    }

    value = slots_[tail & kIndexMask];
    tail_.store(tail + 1U, std::memory_order_release);
    return true;
  }

 private:
  static constexpr std::uint32_t kIndexMask =
      static_cast<std::uint32_t>(Capacity - 1U);

  std::array<T, Capacity> slots_{};
  std::atomic<std::uint32_t> head_{0U};
  std::atomic<std::uint32_t> tail_{0U};
};

}  // namespace parameters
