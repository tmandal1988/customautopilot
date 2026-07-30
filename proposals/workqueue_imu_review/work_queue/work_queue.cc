#include "work_queue/work_queue.h"

#include "stm32h7xx.h"

namespace work_queue {

WorkQueue::WorkQueue(
    const WorkQueueConfig& config,
    WorkEvent* event_storage,
    size_t event_capacity,
    StackType_t* task_stack,
    uint16_t task_stack_words,
    StaticTask_t* task_control_block,
    PeriodicEntry* periodic_entries,
    size_t max_periodic_entries)
    : config_(config),
      event_storage_(event_storage),
      event_capacity_(event_capacity),
      event_head_(0),
      event_tail_(0),
      event_count_(0),
      task_stack_(task_stack),
      task_stack_words_(task_stack_words),
      task_control_block_(task_control_block),
      task_handle_(nullptr),
      periodic_entries_(periodic_entries),
      max_periodic_entries_(max_periodic_entries),
      started_(false),
      queue_full_count_(0),
      periodic_overrun_count_(0) {
    for (size_t idx = 0; idx < max_periodic_entries_; ++idx) {
        periodic_entries_[idx] = PeriodicEntry{nullptr, 0, 0, false};
    }
}

bool WorkQueue::Start() {
    if (started_) {
        return true;
    }

    if ((event_storage_ == nullptr) ||
        (task_stack_ == nullptr) ||
        (task_control_block_ == nullptr) ||
        (periodic_entries_ == nullptr)) {
        return false;
    }

    if (event_capacity_ < config_.queue_depth) {
        return false;
    }

    if (task_stack_words_ < config_.stack_words) {
        return false;
    }

    task_handle_ = xTaskCreateStatic(
        TaskEntry,
        config_.name,
        config_.stack_words,
        this,
        ToFreeRtosPriority(config_.priority),
        task_stack_,
        task_control_block_);

    started_ = (task_handle_ != nullptr);
    return started_;
}

bool WorkQueue::Add(WorkItem& item, WorkReason reason) {
    if (task_handle_ == nullptr) {
        item.NoteScheduleFail();
        return false;
    }

    PushResult push_result = PushResult::kFailed;

    // Bounded metadata lock only: no WorkItem::Run(), HAL, DMA, or pub/sub here.
    taskENTER_CRITICAL();
    push_result = PushEventLocked(item, reason);
    taskEXIT_CRITICAL();

    if (push_result == PushResult::kPushed) {
        WakeWorker();
    }

    return push_result != PushResult::kFailed;
}

bool WorkQueue::AddFromISR(
    WorkItem& item,
    WorkReason reason,
    BaseType_t* higher_priority_task_woken) {
    if (task_handle_ == nullptr) {
        item.NoteScheduleFail();
        return false;
    }

    PushResult push_result = PushResult::kFailed;

    // ISR variant of the same bounded metadata lock.
    const UBaseType_t saved_interrupt_status = taskENTER_CRITICAL_FROM_ISR();
    push_result = PushEventLocked(item, reason);
    taskEXIT_CRITICAL_FROM_ISR(saved_interrupt_status);

    if (push_result == PushResult::kPushed) {
        WakeWorkerFromISR(higher_priority_task_woken);
    }

    return push_result != PushResult::kFailed;
}

bool WorkQueue::ScheduleOnInterval(
    WorkItem& item,
    TickType_t interval_ticks,
    TickType_t delay_ticks) {
    if ((interval_ticks == 0) || started_) {
        item.NoteScheduleFail();
        return false;
    }

    PeriodicEntry* entry = FindPeriodicEntry(item);

    if (entry == nullptr) {
        entry = FindFreePeriodicEntry();
    }

    if (entry == nullptr) {
        item.NoteScheduleFail();
        return false;
    }

    const TickType_t now_ticks = xTaskGetTickCount();
    entry->item = &item;
    entry->interval_ticks = interval_ticks;
    entry->next_release_ticks = now_ticks + ((delay_ticks > 0) ? delay_ticks : interval_ticks);
    entry->active = true;

    return true;
}

bool WorkQueue::ScheduleClear(WorkItem& item) {
    if (started_) {
        item.NoteScheduleFail();
        return false;
    }

    PeriodicEntry* entry = FindPeriodicEntry(item);

    if (entry != nullptr) {
        *entry = PeriodicEntry{nullptr, 0, 0, false};
    }

    return entry != nullptr;
}

void WorkQueue::TaskEntry(void* argument) {
    static_cast<WorkQueue*>(argument)->RunLoop();
}

UBaseType_t WorkQueue::ToFreeRtosPriority(osPriority_t priority) {
    const int32_t priority_value = static_cast<int32_t>(priority);

    if (priority_value <= 0) {
        return 0U;
    }

    if (priority_value >= static_cast<int32_t>(configMAX_PRIORITIES)) {
        return static_cast<UBaseType_t>(configMAX_PRIORITIES - 1U);
    }

    return static_cast<UBaseType_t>(priority_value);
}

bool WorkQueue::TickReached(TickType_t now_ticks, TickType_t target_ticks) {
    return static_cast<int32_t>(now_ticks - target_ticks) >= 0;
}

void WorkQueue::RunLoop() {
    for (;;) {
        DrainQueuedEvents();

        TickType_t now_ticks = xTaskGetTickCount();
        RunDuePeriodicItems(now_ticks);

        now_ticks = xTaskGetTickCount();
        const TickType_t wait_ticks = ComputeWaitTicks(now_ticks);

        (void)ulTaskNotifyTake(pdTRUE, wait_ticks);
    }
}

void WorkQueue::DrainQueuedEvents() {
    WorkEvent event{};

    while (PopEvent(event)) {
        if (event.item != nullptr) {
            RunOne(*event.item, event.reason);
        }
    }
}

void WorkQueue::RunDuePeriodicItems(TickType_t now_ticks) {
    for (size_t idx = 0; idx < max_periodic_entries_; ++idx) {
        PeriodicEntry& entry = periodic_entries_[idx];

        if (!entry.active || (entry.item == nullptr) || !TickReached(now_ticks, entry.next_release_ticks)) {
            continue;
        }

        uint32_t skipped_releases = 0;

        do {
            entry.next_release_ticks += entry.interval_ticks;
            ++skipped_releases;
        } while (TickReached(now_ticks, entry.next_release_ticks));

        if (skipped_releases > 1) {
            periodic_overrun_count_ += (skipped_releases - 1);
        }

        RunOne(*entry.item, WorkReason::kPeriodic);
    }
}

void WorkQueue::RunOne(WorkItem& item, WorkReason reason) {
    const uint32_t start_cycles = DWT->CYCCNT;
    item.RunPreamble();
    item.Run(reason);
    item.RunPostamble(start_cycles);
}

TickType_t WorkQueue::ComputeWaitTicks(TickType_t now_ticks) const {
    TickType_t min_wait_ticks = portMAX_DELAY;

    for (size_t idx = 0; idx < max_periodic_entries_; ++idx) {
        const PeriodicEntry& entry = periodic_entries_[idx];

        if (!entry.active || (entry.item == nullptr)) {
            continue;
        }

        if (TickReached(now_ticks, entry.next_release_ticks)) {
            return 0;
        }

        const TickType_t wait_ticks = entry.next_release_ticks - now_ticks;

        if (wait_ticks < min_wait_ticks) {
            min_wait_ticks = wait_ticks;
        }
    }

    return min_wait_ticks;
}

WorkQueue::PeriodicEntry* WorkQueue::FindPeriodicEntry(WorkItem& item) {
    for (size_t idx = 0; idx < max_periodic_entries_; ++idx) {
        PeriodicEntry& entry = periodic_entries_[idx];

        if (entry.active && (entry.item == &item)) {
            return &entry;
        }
    }

    return nullptr;
}

WorkQueue::PeriodicEntry* WorkQueue::FindFreePeriodicEntry() {
    for (size_t idx = 0; idx < max_periodic_entries_; ++idx) {
        PeriodicEntry& entry = periodic_entries_[idx];

        if (!entry.active) {
            return &entry;
        }
    }

    return nullptr;
}

bool WorkQueue::PopEvent(WorkEvent& event) {
    bool popped = false;

    // Bounded metadata lock only; the caller runs the item after this returns.
    taskENTER_CRITICAL();

    if (event_count_ > 0) {
        event = event_storage_[event_head_];
        event_head_ = (event_head_ + 1U) % event_capacity_;
        --event_count_;

        if (event.item != nullptr) {
            event.item->queued_ = false;
        }

        popped = true;
    }

    taskEXIT_CRITICAL();
    return popped;
}

WorkQueue::PushResult WorkQueue::PushEventLocked(WorkItem& item, WorkReason reason) {
    if (item.queued_) {
        item.NoteScheduleDuplicate();
        return PushResult::kAlreadyQueued;
    }

    if (event_count_ >= event_capacity_) {
        item.NoteScheduleFail();
        ++queue_full_count_;
        return PushResult::kFailed;
    }

    event_storage_[event_tail_] = WorkEvent{&item, reason};
    event_tail_ = (event_tail_ + 1U) % event_capacity_;
    ++event_count_;
    item.queued_ = true;

    return PushResult::kPushed;
}

void WorkQueue::WakeWorker() {
    (void)xTaskNotifyGive(task_handle_);
}

void WorkQueue::WakeWorkerFromISR(BaseType_t* higher_priority_task_woken) {
    vTaskNotifyGiveFromISR(task_handle_, higher_priority_task_woken);
}

}  // namespace work_queue
