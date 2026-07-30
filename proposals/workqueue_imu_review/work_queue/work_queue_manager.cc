#include "work_queue/work_queue_manager.h"

namespace work_queue {
namespace {

uint32_t requested_queue_mask = 0;
bool     manager_started      = false;

constexpr uint32_t QueueBit(QueueId id) {
    return 1UL << static_cast<uint8_t>(id);
}

bool QueueRequested(QueueId id) {
    return (requested_queue_mask & QueueBit(id)) != 0;
}

// Each queue gets a single static slot. The slot owns everything FreeRTOS
// needs: event-ring storage, task stack, TCB, and the periodic-entry table.
// No heap is touched at runtime.
template <size_t QueueDepth, uint16_t StackWords, size_t MaxPeriodicItems>
struct StaticWorkQueueSlot {
    WorkQueue::WorkEvent     event_storage[QueueDepth]{};
    StaticTask_t             task_control_block{};
    StackType_t              task_stack[StackWords]{};
    WorkQueue::PeriodicEntry periodic_entries[MaxPeriodicItems]{};

    WorkQueue queue;

    explicit StaticWorkQueueSlot(const WorkQueueConfig& config)
        : queue(config,
                event_storage, QueueDepth,
                task_stack, StackWords, &task_control_block,
                periodic_entries, MaxPeriodicItems) {}
};

auto& ImuSlot() {
    static StaticWorkQueueSlot<
        configurations::imu_bus.queue_depth,
        configurations::imu_bus.stack_words,
        configurations::imu_bus.max_periodic_items>
        slot(configurations::imu_bus);
    return slot;
}

auto& NavSlot() {
    static StaticWorkQueueSlot<
        configurations::nav_and_control.queue_depth,
        configurations::nav_and_control.stack_words,
        configurations::nav_and_control.max_periodic_items>
        slot(configurations::nav_and_control);
    return slot;
}

auto& HpDefaultSlot() {
    static StaticWorkQueueSlot<
        configurations::hp_default.queue_depth,
        configurations::hp_default.stack_words,
        configurations::hp_default.max_periodic_items>
        slot(configurations::hp_default);
    return slot;
}

auto& LpDefaultSlot() {
    static StaticWorkQueueSlot<
        configurations::lp_default.queue_depth,
        configurations::lp_default.stack_words,
        configurations::lp_default.max_periodic_items>
        slot(configurations::lp_default);
    return slot;
}

}  // namespace

WorkQueue* WorkQueueManager::FindOrCreate(const WorkQueueConfig& config) {
    requested_queue_mask |= QueueBit(config.id);

    WorkQueue* queue = Find(config.id);

    // If a new module registers after StartAll() has run (e.g. dynamically
    // brought-up subsystem), bring the queue up immediately. Otherwise it
    // will be started later by StartAll().
    if ((queue != nullptr) && manager_started) {
        (void)queue->Start();
    }

    return queue;
}

WorkQueue* WorkQueueManager::Find(QueueId id) {
    switch (id) {
        case QueueId::kImu:           return &ImuSlot().queue;
        case QueueId::kNavAndControl: return &NavSlot().queue;
        case QueueId::kHpDefault:     return &HpDefaultSlot().queue;
        case QueueId::kLpDefault:     return &LpDefaultSlot().queue;
    }
    return nullptr;
}

bool WorkQueueManager::StartAll() {
    manager_started = true;

    bool ok = true;

    // Only start queues that at least one module has requested. Adding new
    // queue configurations here does not spawn idle workers.
    if (QueueRequested(QueueId::kImu))           ok &= ImuSlot().queue.Start();
    if (QueueRequested(QueueId::kNavAndControl)) ok &= NavSlot().queue.Start();
    if (QueueRequested(QueueId::kHpDefault))     ok &= HpDefaultSlot().queue.Start();
    if (QueueRequested(QueueId::kLpDefault))     ok &= LpDefaultSlot().queue.Start();

    return ok;
}

}  // namespace work_queue
