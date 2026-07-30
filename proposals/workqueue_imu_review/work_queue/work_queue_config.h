#pragma once

#include "cmsis_os.h"

#include <stddef.h>
#include <stdint.h>

namespace work_queue {

// Logical queue identifiers. The integer value is also the bit index used by
// WorkQueueManager to track which queues modules actually requested at static
// init time. Keep contiguous and starting at 0.
enum class QueueId : uint8_t {
    kImu,             // PX4 wq:I2C2/wq:SPIn - dedicated to a single sensor bus
    kNavAndControl,   // PX4 wq:nav_and_controllers - EKF, controllers, PWM
    kHpDefault,       // PX4 wq:hp_default - secondary sensors
    kLpDefault,       // PX4 wq:lp_default - logging, telemetry
};

struct WorkQueueConfig {
    QueueId       id;
    const char*   name;                 // must fit configMAX_TASK_NAME_LEN (16)
    osPriority_t  priority;
    uint16_t      stack_words;          // 4 bytes per word on Cortex-M
    uint8_t       queue_depth;          // event ring capacity
    uint8_t       max_periodic_items;   // periodic-schedule slot count
};

namespace configurations {

// IMU read sits on its own worker at the highest priority any application
// thread should ever take. Only the ICM20948 driver lives here, so the stack
// budget is tight; the worker only ever runs CompleteDmaRead() (parse +
// publish, ~hundreds of cycles) and StartDmaReadIfReady() (a HAL call).
inline constexpr WorkQueueConfig imu_bus{
    /* id                 */ QueueId::kImu,
    /* name               */ "wq:imu",
    /* priority           */ osPriorityRealtime,    // 48
    /* stack_words        */ 1024,                  // 4 KB
    /* queue_depth        */ 8,
    /* max_periodic_items */ 2,
};

// Everything in the IMU -> EKF -> controller -> motor chain runs here in a
// single sequential pass per IMU sample. Items execute back-to-back on the
// same worker, so the stack only needs to fit the largest single item.
// StateEstimator's autocode currently demands ~10 KB; 4096 words = 16 KB
// leaves headroom for FCS step locals when it migrates.
inline constexpr WorkQueueConfig nav_and_control{
    /* id                 */ QueueId::kNavAndControl,
    /* name               */ "wq:nav",
    /* priority           */ osPriorityHigh,        // 40
    /* stack_words        */ 4096,                  // 16 KB
    /* queue_depth        */ 16,
    /* max_periodic_items */ 4,
};

// Auxiliary sensors (baro, GPS, optical flow, RC SBUS). Each module is
// periodic at <= 100 Hz and their parse routines are short.
inline constexpr WorkQueueConfig hp_default{
    /* id                 */ QueueId::kHpDefault,
    /* name               */ "wq:hp",
    /* priority           */ osPriorityAboveNormal, // 32
    /* stack_words        */ 1536,                  // 6 KB
    /* queue_depth        */ 16,
    /* max_periodic_items */ 8,
};

// Logging, telemetry, MAVLink, anything that must never delay the control
// chain. Preemptable by every queue above.
inline constexpr WorkQueueConfig lp_default{
    /* id                 */ QueueId::kLpDefault,
    /* name               */ "wq:lp",
    /* priority           */ osPriorityNormal,      // 24
    /* stack_words        */ 1536,                  // 6 KB
    /* queue_depth        */ 16,
    /* max_periodic_items */ 8,
};

}  // namespace configurations

}  // namespace work_queue
