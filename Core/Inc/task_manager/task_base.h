/*
 * task_base.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */
#pragma once

#include "cmsis_os.h"
#include <vector>
#include <string>

class TaskBase {
public:
    TaskBase(const char* name, uint16_t stackSize, osPriority_t priority)
        : taskName(name),
          stackSize(stackSize),
          priority(priority),
          taskHandle(nullptr) // Ensure the order matches the declaration
    {
        RegisterTask(this);
    }

    virtual ~TaskBase() {}

    virtual void Run() = 0;

    void Start() {
        osThreadAttr_t threadAttributes = {};
        threadAttributes.name = taskName;
        threadAttributes.stack_size = stackSize;
        threadAttributes.priority = priority;

        taskHandle = osThreadNew(TaskFunctionWrapper, this, &threadAttributes);
        if (taskHandle == nullptr) {
            printf("Failed to create task: %s\n", taskName);
        }
    }

    static void TaskFunctionWrapper(void* argument) {
        auto* task = static_cast<TaskBase*>(argument);
        task->Run();
    }

    static std::vector<TaskBase*>& GetTaskList() {
        static std::vector<TaskBase*> taskList;
        return taskList;
    }

    static void RegisterTask(TaskBase* task) {
        GetTaskList().push_back(task);
    }

    const char* GetTaskName() const {
        return taskName;
    }

protected:
    const char* taskName;
    uint16_t stackSize;
    osPriority_t priority;
    osThreadId_t taskHandle;
};


