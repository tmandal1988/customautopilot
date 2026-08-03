/*
 * task_base.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */
#pragma once

#include "cmsis_os.h"
#include "rtos_metrics/rtos_metrics.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>

class TaskBase {
public:
    TaskBase(const char* name, uint16_t requestedStackSize,
             osPriority_t priority, bool register_task = true)
        : taskName(name),
          stackSize(RoundUpStackSize(EffectiveStackSize(requestedStackSize))),
          priority(priority),
          taskHandle(nullptr),
          stackMemory(register_task ? AllocateStack(stackSize) : nullptr),
          stackDepthWords(register_task
                              ? static_cast<uint16_t>(stackSize /
                                                      sizeof(StackType_t))
                              : 0U),
          taskControlBlock{}
#if RTOS_METRICS_ENABLE
          ,
          metrics_(name,
                   (static_cast<uint32_t>(stackSize) /
                    sizeof(StackType_t)) * sizeof(StackType_t),
                   priority)
#endif
    {
        if (register_task) {
            RegisterTask(this);
        }
    }

    virtual ~TaskBase() {}

    virtual void Run() = 0;

    void Start() {
        osThreadAttr_t threadAttributes = {};
        threadAttributes.name = taskName;
        threadAttributes.stack_size = stackSize;
        threadAttributes.priority = priority;
        threadAttributes.cb_mem = &taskControlBlock;
        threadAttributes.cb_size = sizeof(taskControlBlock);
        threadAttributes.stack_mem = stackMemory;

        taskHandle = osThreadNew(TaskFunctionWrapper, this, &threadAttributes);
        if (taskHandle == nullptr) {
            printf("Failed to create task: %s\n", taskName);
        }
    }

    static void TaskFunctionWrapper(void* argument) {
        auto* task = static_cast<TaskBase*>(argument);
        task->Run();
    }

    class TaskListView {
    public:
        bool empty() const noexcept {
            return taskCount == 0U;
        }

        std::size_t size() const noexcept {
            return taskCount;
        }

        TaskBase* operator[](std::size_t index) const noexcept {
            return TaskBase::GetTask(index);
        }
    };

    static TaskListView GetTaskList() noexcept {
        return {};
    }

    static std::size_t GetTaskCount() noexcept {
        return taskCount;
    }

    static TaskBase* GetTask(std::size_t index) noexcept {
        return (index < taskCount) ? taskList[index] : nullptr;
    }

    static void RegisterTask(TaskBase* task) {
        if (taskCount < kMaxRegisteredTasks) {
            taskList[taskCount++] = task;
        } else {
            printf("Task registry full, dropping task: %s\n", task->taskName);
        }
    }

    const char* GetTaskName() const {
        return taskName;
    }

    void RefreshStackHighWaterMark() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.RefreshStackHighWaterMark(taskHandle);
#endif
    }

    void CaptureMetrics(std::uint8_t task_index, std::uint8_t task_count,
                        std::uint32_t snapshot_sequence,
                        std::uint32_t context_switch_count,
                        std::uint32_t context_switch_delta,
                        std::uint32_t context_interval_ms,
                        RtosMetricsData* output) const noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.Capture(task_index, task_count, snapshot_sequence,
                         context_switch_count, context_switch_delta,
                         context_interval_ms, output);
#else
        static_cast<void>(task_index);
        static_cast<void>(task_count);
        static_cast<void>(snapshot_sequence);
        static_cast<void>(context_switch_count);
        static_cast<void>(context_switch_delta);
        static_cast<void>(context_interval_ms);
        static_cast<void>(output);
#endif
    }

protected:
    static constexpr uint16_t RoundUpStackSize(
        uint16_t requested_stack_size) noexcept {
        constexpr uint16_t kStackAlignment =
            static_cast<uint16_t>(sizeof(StackType_t));
        return static_cast<uint16_t>(
            (requested_stack_size + (kStackAlignment - 1U)) &
            ~(kStackAlignment - 1U));
    }

    static constexpr uint16_t EffectiveStackSize(
        uint16_t requested_stack_size) noexcept {
#if RTOS_METRICS_ENABLE
        // Bench-only instrumentation must not consume a marginal production
        // stack. This allocation-only reserve adds no steady-state CPU work.
        constexpr uint16_t kDiagnosticReserveBytes = 256U;
        return (requested_stack_size <=
                (UINT16_MAX - kDiagnosticReserveBytes))
                   ? static_cast<uint16_t>(requested_stack_size +
                                           kDiagnosticReserveBytes)
                   : requested_stack_size;
#else
        return requested_stack_size;
#endif
    }

    static StackType_t* AllocateStack(uint16_t stack_size_bytes) noexcept {
        const std::size_t requested_words =
            stack_size_bytes / sizeof(StackType_t);
        const std::size_t next_used = staticStackPoolUsedWords + requested_words;
        if (next_used > kStaticStackPoolWords) {
            return nullptr;
        }

        StackType_t* const allocation =
            &staticStackPool[staticStackPoolUsedWords];
        staticStackPoolUsedWords = next_used;
        return allocation;
    }

    class MetricsCycleScope {
    public:
        explicit MetricsCycleScope(TaskBase* owner) noexcept
            : owner_(owner) {
            owner_->BeginMetricsCycle();
        }

        ~MetricsCycleScope() {
            Complete();
        }

        void Complete() noexcept {
            if (owner_ != nullptr) {
                owner_->EndMetricsCycle();
                owner_ = nullptr;
            }
        }

        MetricsCycleScope(const MetricsCycleScope&) = delete;
        MetricsCycleScope& operator=(const MetricsCycleScope&) = delete;

    private:
        TaskBase* owner_;
    };

    void ConfigurePeriodicMetrics(std::uint32_t period_us,
                                  std::uint32_t deadline_us) noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.ConfigurePeriodic(period_us, deadline_us);
#else
        static_cast<void>(period_us);
        static_cast<void>(deadline_us);
#endif
    }

    void ConfigureEventMetrics() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.ConfigureEventDriven();
#endif
    }

    void BeginMetricsCycle() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.BeginCycle();
#endif
    }

    [[nodiscard]] MetricsCycleScope MeasureMetricsScope() noexcept {
        return MetricsCycleScope(this);
    }

    void EndMetricsCycle() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.CompleteCycle();
#endif
    }

    void SetAuxMetric(std::uint8_t index, std::uint32_t value) noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.SetAuxMetric(index, value);
#else
        static_cast<void>(index);
        static_cast<void>(value);
#endif
    }

    void UpdateAuxMetricMaximum(std::uint8_t index,
                                std::uint32_t candidate) noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.UpdateAuxMetricMaximum(index, candidate);
#else
        static_cast<void>(index);
        static_cast<void>(candidate);
#endif
    }

    void ResetPeriodicMetricsPhase() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.ResetPeriodicPhase();
#endif
    }

    void MarkMetricsScheduleDiscontinuity() noexcept {
#if RTOS_METRICS_ENABLE
        metrics_.MarkScheduleDiscontinuity();
#endif
    }

    const char* taskName;
    uint16_t stackSize;
    osPriority_t priority;
    osThreadId_t taskHandle;
    StackType_t* stackMemory;
    uint16_t stackDepthWords;
    StaticTask_t taskControlBlock;
#if RTOS_METRICS_ENABLE
    rtos_metrics::TaskMetrics metrics_;
#endif

private:
    static constexpr std::size_t kMaxRegisteredTasks = 24U;
    static constexpr std::size_t kStaticStackPoolBytes = 64U * 1024U;
    static constexpr std::size_t kStaticStackPoolWords =
        kStaticStackPoolBytes / sizeof(StackType_t);

    inline static TaskBase* taskList[kMaxRegisteredTasks] = {};
    inline static std::size_t taskCount = 0U;
    inline static StackType_t staticStackPool[kStaticStackPoolWords] = {};
    inline static std::size_t staticStackPoolUsedWords = 0U;
};
