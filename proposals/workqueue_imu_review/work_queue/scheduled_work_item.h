#pragma once

#include "work_queue/work_item.h"
#include "work_queue/work_queue.h"

namespace work_queue {

class ScheduledWorkItem : public WorkItem {
public:
    // Init-only: call before WorkQueueManager::StartAll(). Runtime work uses
    // ScheduleNow() / ScheduleNowFromISR() through the event ring.
    bool ScheduleOnInterval(TickType_t interval_ticks, TickType_t delay_ticks = 0) {
        WorkQueue* queue = AssignedQueue();
        return (queue != nullptr) && queue->ScheduleOnInterval(*this, interval_ticks, delay_ticks);
    }

    bool ScheduleClear() {
        WorkQueue* queue = AssignedQueue();
        return (queue != nullptr) && queue->ScheduleClear(*this);
    }

protected:
    ScheduledWorkItem(const char* name, const WorkQueueConfig& config)
        : WorkItem(name, config) {}
};

}  // namespace work_queue
