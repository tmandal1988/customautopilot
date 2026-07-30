#pragma once

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task.h"
#include "work_queue/work_item.h"
#include "work_queue/work_queue_config.h"

#include <stddef.h>
#include <stdint.h>

namespace work_queue {

class WorkQueue {
public:
    struct WorkEvent {
        WorkItem* item;
        WorkReason reason;
    };

    struct PeriodicEntry {
        WorkItem* item;
        TickType_t interval_ticks;
        TickType_t next_release_ticks;
        bool active;
    };

    WorkQueue(
        const WorkQueueConfig& config,
        WorkEvent* event_storage,
        size_t event_capacity,
        StackType_t* task_stack,
        uint16_t task_stack_words,
        StaticTask_t* task_control_block,
        PeriodicEntry* periodic_entries,
        size_t max_periodic_entries);

    bool Start();
    bool Add(WorkItem& item, WorkReason reason);
    bool AddFromISR(WorkItem& item, WorkReason reason, BaseType_t* higher_priority_task_woken);
    bool ScheduleOnInterval(WorkItem& item, TickType_t interval_ticks, TickType_t delay_ticks = 0);
    bool ScheduleClear(WorkItem& item);

    const WorkQueueConfig& Config() const {
        return config_;
    }

    bool Started() const {
        return started_;
    }

    uint32_t QueueFullCount() const {
        return queue_full_count_;
    }

    uint32_t PeriodicOverrunCount() const {
        return periodic_overrun_count_;
    }

private:
    enum class PushResult : uint8_t {
        kFailed,
        kAlreadyQueued,
        kPushed,
    };

    static void TaskEntry(void* argument);
    static UBaseType_t ToFreeRtosPriority(osPriority_t priority);
    static bool TickReached(TickType_t now_ticks, TickType_t target_ticks);

    void RunLoop();
    void DrainQueuedEvents();
    void RunDuePeriodicItems(TickType_t now_ticks);
    void RunOne(WorkItem& item, WorkReason reason);
    TickType_t ComputeWaitTicks(TickType_t now_ticks) const;
    PeriodicEntry* FindPeriodicEntry(WorkItem& item);
    PeriodicEntry* FindFreePeriodicEntry();
    bool PopEvent(WorkEvent& event);
    PushResult PushEventLocked(WorkItem& item, WorkReason reason);
    void WakeWorker();
    void WakeWorkerFromISR(BaseType_t* higher_priority_task_woken);

    const WorkQueueConfig& config_;

    WorkEvent* event_storage_;
    size_t event_capacity_;
    size_t event_head_;
    size_t event_tail_;
    size_t event_count_;

    StackType_t* task_stack_;
    uint16_t task_stack_words_;
    StaticTask_t* task_control_block_;
    TaskHandle_t task_handle_;

    PeriodicEntry* periodic_entries_;
    size_t max_periodic_entries_;

    bool started_;
    uint32_t queue_full_count_;
    uint32_t periodic_overrun_count_;
};

}  // namespace work_queue
