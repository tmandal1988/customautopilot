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
        auto& tasks = TaskBase::GetTaskList();
        if (tasks.empty()) {
//            printf("No tasks to start.\n");
            return;
        }

        for (auto task : tasks) {
//            printf("Starting task: %s\n", task->GetTaskName());
            task->Start();
        }
    }

private:
    TaskManager() = default;
};
