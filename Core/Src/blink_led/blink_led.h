/*
 * blink_led.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "debug.h"
#include "usb_console/usb_console.h"

class BlinkLedTask : public TaskBase {
public:
    BlinkLedTask() : TaskBase("BlinkLedTask", 768, osPriorityNormal) {}

    void Run() override;

private:
    // The loop below ticks once per second.
    static constexpr uint32_t kHeapReportDelaySeconds = 5;
};

// Static instance for self-registration
BlinkLedTask blink_led_task_instance_;
