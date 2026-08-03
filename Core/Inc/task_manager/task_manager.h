/*
 * task_manager.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_base.h"
#include <cstdio>

class TaskManager {
public:
    static TaskManager& Instance() {
        static TaskManager instance;
        return instance;
    }

    void StartAll() {
        const std::size_t task_count = TaskBase::GetTaskCount();
        if (task_count == 0U) {
//            printf("No tasks to start.\n");
            return;
        }

        for (std::size_t i = 0U; i < task_count; ++i) {
            auto* const task = TaskBase::GetTask(i);
            if (task == nullptr) {
                continue;
            }
//            printf("Starting task: %s\n", task->GetTaskName());
            task->Start();
        }
    }

private:
    TaskManager() = default;
};
