#include "work_queue/work_item.h"

#include "stm32h7xx.h"
#include "task.h"
#include "work_queue/work_queue.h"
#include "work_queue/work_queue_manager.h"

namespace work_queue {

WorkItem::WorkItem(const char* name, const WorkQueueConfig& config)
    : item_name_(name),
      config_(config),
      work_queue_(WorkQueueManager::FindOrCreate(config)),
      queued_(false),
      downstream_consumers_{},
      downstream_consumer_count_(0),
      run_count_(0),
      schedule_duplicate_count_(0),
      schedule_fail_count_(0),
      last_runtime_cycles_(0),
      max_runtime_cycles_(0) {}

bool WorkItem::ScheduleNow() {
    if (work_queue_ == nullptr) {
        NoteScheduleFail();
        return false;
    }
    return work_queue_->Add(*this, WorkReason::kEvent);
}

bool WorkItem::ScheduleNowFromISR(BaseType_t* higher_priority_task_woken) {
    if (work_queue_ == nullptr) {
        NoteScheduleFail();
        return false;
    }
    return work_queue_->AddFromISR(*this, WorkReason::kEvent, higher_priority_task_woken);
}

bool WorkItem::RegisterDownstreamConsumer(WorkItem& consumer) {
    const bool producer_started = (work_queue_ != nullptr) && work_queue_->Started();
    const bool consumer_started =
        (consumer.work_queue_ != nullptr) && consumer.work_queue_->Started();

    if (producer_started || consumer_started ||
        (downstream_consumer_count_ >= kMaxDownstreamConsumers)) {
        NoteScheduleFail();
        return false;
    }

    downstream_consumers_[downstream_consumer_count_++] = &consumer;
    return true;
}

void WorkItem::NotifyDownstreamConsumers() {
    // Downstream registration is init-only, so this list is immutable while
    // producers are running.
    const uint8_t count = downstream_consumer_count_;
    for (uint8_t idx = 0; idx < count; ++idx) {
        WorkItem* consumer = downstream_consumers_[idx];
        if (consumer != nullptr) {
            (void)consumer->ScheduleNow();
        }
    }
}

void WorkItem::NoteScheduleDuplicate() { ++schedule_duplicate_count_; }
void WorkItem::NoteScheduleFail()      { ++schedule_fail_count_; }

void WorkItem::RunPreamble() { ++run_count_; }

void WorkItem::RunPostamble(uint32_t start_cycles) {
    last_runtime_cycles_ = DWT->CYCCNT - start_cycles;
    if (last_runtime_cycles_ > max_runtime_cycles_) {
        max_runtime_cycles_ = last_runtime_cycles_;
    }
}

}  // namespace work_queue
