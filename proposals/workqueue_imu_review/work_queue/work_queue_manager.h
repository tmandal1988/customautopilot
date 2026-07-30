#pragma once

#include "work_queue/work_queue.h"
#include "work_queue/work_queue_config.h"

namespace work_queue {

class WorkQueueManager {
public:
    static WorkQueue* FindOrCreate(const WorkQueueConfig& config);
    static WorkQueue* Find(QueueId id);
    static bool StartAll();
};

}  // namespace work_queue

