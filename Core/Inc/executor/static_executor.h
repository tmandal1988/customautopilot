#pragma once

#include "executor/work_item.h"
#include "task_manager/task_base.h"

#include "FreeRTOS.h"
#include "task.h"

#include <cstddef>
#include <cstdint>

namespace executor {

struct PeriodicWorkConfig {
    WorkItem* item = nullptr;
    TickType_t period_ticks = 0U;
    TickType_t phase_ticks = 0U;
};

struct EventWorkConfig {
    WorkItem* item = nullptr;
    std::uint32_t notification_bit = 0U;
};

template <std::size_t MaxPeriodicItems, std::size_t MaxEventItems>
class StaticExecutor final : public TaskBase {
public:
    StaticExecutor(const char* name, std::uint16_t stack_size_bytes,
                   osPriority_t priority, TickType_t idle_wait_ticks) noexcept
        : TaskBase(name, stack_size_bytes, priority),
          idle_wait_ticks_(idle_wait_ticks) {
        ConfigureEventMetrics();
    }

    bool RegisterPeriodic(const PeriodicWorkConfig& config) noexcept {
        if ((config.item == nullptr) || (config.period_ticks == 0U) ||
            (periodic_count_ >= MaxPeriodicItems)) {
            return false;
        }

        periodic_items_[periodic_count_++] = PeriodicState{
            .item = config.item,
            .period_ticks = config.period_ticks,
            .next_release_tick = config.phase_ticks,
        };
        return true;
    }

    bool RegisterEvent(const EventWorkConfig& config) noexcept {
        if ((config.item == nullptr) || (config.notification_bit == 0U) ||
            (event_count_ >= MaxEventItems)) {
            return false;
        }

        event_items_[event_count_++] = config;
        return true;
    }

    void Run() override {
        if ((periodic_count_ == 0U) && (event_count_ == 0U)) {
            vTaskSuspend(nullptr);
        }

        TickType_t now = xTaskGetTickCount();
        for (std::size_t i = 0U; i < periodic_count_; ++i) {
            periodic_items_[i].next_release_tick += now;
        }

        for (;;) {
            const TickType_t wait_ticks = ComputeWaitTicks(now);
            std::uint32_t notification_bits = 0U;
            static_cast<void>(xTaskNotifyWait(
                0U, UINT32_MAX, &notification_bits, wait_ticks));

            now = xTaskGetTickCount();
            auto metrics_scope = MeasureMetricsScope();
            ServiceEvents(notification_bits);
            ServicePeriodic(now);
        }
    }

private:
    struct PeriodicState {
        WorkItem* item = nullptr;
        TickType_t period_ticks = 0U;
        TickType_t next_release_tick = 0U;
    };

    static bool TickReached(TickType_t now, TickType_t deadline) noexcept {
        return static_cast<BaseType_t>(now - deadline) >= 0;
    }

    TickType_t ComputeWaitTicks(TickType_t now) const noexcept {
        if (periodic_count_ == 0U) {
            return idle_wait_ticks_;
        }

        TickType_t wait_ticks = idle_wait_ticks_;
        for (std::size_t i = 0U; i < periodic_count_; ++i) {
            const auto& periodic = periodic_items_[i];
            if (TickReached(now, periodic.next_release_tick)) {
                return 0U;
            }

            const TickType_t candidate =
                periodic.next_release_tick - now;
            if (candidate < wait_ticks) {
                wait_ticks = candidate;
            }
        }
        return wait_ticks;
    }

    void ServiceEvents(std::uint32_t notification_bits) noexcept {
        if (notification_bits == 0U) {
            return;
        }

        for (std::size_t i = 0U; i < event_count_; ++i) {
            const auto& event = event_items_[i];
            if ((notification_bits & event.notification_bit) != 0U) {
                event.item->Run();
            }
        }
    }

    void ServicePeriodic(TickType_t now) noexcept {
        for (std::size_t i = 0U; i < periodic_count_; ++i) {
            auto& periodic = periodic_items_[i];
            if (!TickReached(now, periodic.next_release_tick)) {
                continue;
            }

            periodic.item->Run();

            do {
                periodic.next_release_tick += periodic.period_ticks;
            } while (TickReached(now, periodic.next_release_tick));
        }
    }

    TickType_t idle_wait_ticks_;
    PeriodicState periodic_items_[MaxPeriodicItems] = {};
    EventWorkConfig event_items_[MaxEventItems] = {};
    std::size_t periodic_count_ = 0U;
    std::size_t event_count_ = 0U;
};

}  // namespace executor
