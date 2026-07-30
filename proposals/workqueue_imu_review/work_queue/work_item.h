#pragma once

#include "FreeRTOS.h"
#include "work_queue/work_queue_config.h"

#include <stddef.h>
#include <stdint.h>

namespace work_queue {

class WorkQueue;

enum class WorkReason : uint8_t {
    kEvent,
    kPeriodic,
};

class WorkItem {
public:
    WorkItem() = delete;
    WorkItem(const WorkItem&) = delete;
    WorkItem& operator=(const WorkItem&) = delete;
    WorkItem(WorkItem&&) = delete;
    WorkItem& operator=(WorkItem&&) = delete;
    virtual ~WorkItem() = default;

    const char* ItemName() const { return item_name_; }

    bool ScheduleNow();
    bool ScheduleNowFromISR(BaseType_t* higher_priority_task_woken);

    // Register a downstream work item that should be scheduled whenever this
    // item finishes producing new data. Used to build event-driven processing
    // chains (e.g. IMU -> EKF -> FCS -> PWM) across queues without polling.
    //
    // Init-only: call before WorkQueueManager::StartAll(). Returns false once
    // either the producer queue or consumer queue has started.
    bool RegisterDownstreamConsumer(WorkItem& consumer);

    uint32_t RunCount() const               { return run_count_; }
    uint32_t ScheduleDuplicateCount() const { return schedule_duplicate_count_; }
    uint32_t ScheduleFailCount() const      { return schedule_fail_count_; }
    uint32_t LastRuntimeCycles() const      { return last_runtime_cycles_; }
    uint32_t MaxRuntimeCycles() const       { return max_runtime_cycles_; }

protected:
    WorkItem(const char* name, const WorkQueueConfig& config);

    virtual void Run(WorkReason reason) = 0;

    // Call from the derived Run() implementation right after publishing fresh
    // data. Walks the registered downstream list and schedules each consumer
    // on its own queue. Schedule failures are counted against the consumer,
    // never thrown — the producer is unaffected.
    void NotifyDownstreamConsumers();

    WorkQueue* AssignedQueue() const { return work_queue_; }

private:
    friend class WorkQueue;

    static constexpr size_t kMaxDownstreamConsumers = 4;

    void NoteScheduleDuplicate();
    void NoteScheduleFail();
    void RunPreamble();
    void RunPostamble(uint32_t start_cycles);

    const char*            item_name_;
    const WorkQueueConfig& config_;
    WorkQueue*             work_queue_;
    bool                   queued_;

    WorkItem* downstream_consumers_[kMaxDownstreamConsumers];
    uint8_t   downstream_consumer_count_;

    uint32_t run_count_;
    uint32_t schedule_duplicate_count_;
    uint32_t schedule_fail_count_;
    uint32_t last_runtime_cycles_;
    uint32_t max_runtime_cycles_;
};

}  // namespace work_queue
