# IMU + EKF Workqueue Review Proposal

This directory sits outside `Core/` so the existing CMake source glob ignores
it. The files here are review-only and do not change the running firmware.

## What this is

A PX4-style work-queue framework migrated for two modules — the ICM20948 IMU
driver and the state estimator. Together they exercise every piece of the
framework the rest of the codebase will need:

- a periodic schedule driven by a hardware timer + DMA-complete event (IMU)
- a pure event-driven schedule consuming an upstream publication (EKF)
- inter-queue handoff via `WorkItem::NotifyDownstreamConsumers()`
- per-item runtime statistics from `DWT->CYCCNT`
- static allocation throughout (no heap once the kernel is up)

Once this pattern is verified, the remaining modules (PWM, FCS, baro, GPS,
optical flow, RC SBUS, MAVLink, logger, blink LED) can be ported one at a time
without further framework changes.

## Queue layout

Four queues, mirroring PX4's per-bus / per-role partitioning:

| Queue id          | Name     | Priority (CMSIS-RTOS V2)      | Stack (words / bytes) | Modules                                            |
|-------------------|----------|-------------------------------|-----------------------|----------------------------------------------------|
| `kImu`            | `wq:imu` | `osPriorityRealtime` (48)     | 1024 / 4 KB           | `ReadIcm20948WorkItem` only                        |
| `kNavAndControl`  | `wq:nav` | `osPriorityHigh` (40)         | 4096 / 16 KB          | `StateEstimatorWorkItem` (+ future FCS, PWM)       |
| `kHpDefault`      | `wq:hp`  | `osPriorityAboveNormal` (32)  | 1536 / 6 KB           | future: baro, GPS, optical flow, RC SBUS           |
| `kLpDefault`      | `wq:lp`  | `osPriorityNormal` (24)       | 1536 / 6 KB           | future: logger, SD writer, MAVLink, blink LED      |

Rationale:

- **`wq:imu` is dedicated.** The IMU DMA kick is the hardest real-time deadline
  in the system. Putting the IMU on its own worker at the highest application
  priority guarantees that no EKF step, controller compute, or sensor parse
  can delay the next 2 ms transfer. Mirrors PX4's per-bus queues (`wq:I2C2`,
  `wq:SPI1`, ...).
- **`wq:nav` hosts the entire IMU -> control -> motor chain.** Every stage
  runs back-to-back on the same worker per IMU sample, so there are no
  inter-stage context switches. Stack is sized to the largest single item
  (StateEstimator's autocode peaks around 10 KB; 16 KB leaves headroom for the
  FCS step locals when it migrates).
- **`wq:hp` is sized for secondary sensors.** They publish at <= 100 Hz and
  their parse routines are short; one worker easily handles all four.
- **`wq:lp` is preemptable by everything above.** Logging cannot delay a
  control cycle even when SD I/O stalls.

`StartAll()` only spawns workers for queues that have at least one registered
module. Adding configurations to `work_queue_config.h` does not create idle
tasks.

## Event chain

```
I2C2 DMA-done ISR
        |
        | AddFromISR(kEvent) + xTaskNotifyGiveFromISR
        v
   wq:imu  ReadIcm20948WorkItem::Run(kEvent)
                CompleteDmaRead()
                    parse 23-byte buffer
                    publish ImuData
                    NotifyDownstreamConsumers()
                        |
                        | ScheduleNow() + xTaskNotifyGive
                        v
                   wq:nav  StateEstimatorWorkItem::Run(kEvent)
                                copy imu / baro / gps / mtf01p
                                autocode_.step()
                                publish EkfData
                                NotifyDownstreamConsumers()
                                    |
                                    +--> (future) FCS -> PWM
```

A separate 2 ms periodic schedule on `wq:imu` calls
`ReadIcm20948WorkItem::Run(kPeriodic)`, which starts the next DMA. The two
schedules ride independent legs of the worker loop, so the
"DMA done -> publish -> EKF step -> publish" round-trip never blocks the next
periodic DMA kick.

## Runtime enforcement rules

- `ScheduleOnInterval()`, `ScheduleClear()`, and
  `RegisterDownstreamConsumer()` are init-only in this proposal. They return
  false after their queues have started, so periodic-table and dependency-list
  mutation cannot enter the flight hot path.
- Module `Init()` runs before `WorkQueueManager::StartAll()`. This installs all
  periodic entries before any worker task can execute.
- Runtime scheduling uses only `Add()` / `AddFromISR()`, which touch the event
  ring in O(1): check pending flag, write one `WorkEvent`, advance one index,
  update one count.
- `WorkItem::Run()` always executes after the queue metadata lock is released.
  HAL calls, DMA work, parsing, pub/sub, EKF, and control code are not allowed
  inside critical sections.

## Why this is faster *and* more deterministic than the current task-per-module design

- **No fixed-rate gap between IMU and EKF.** The polling task wakes every 4 ms
  and reads whatever IMU sample happens to be latest, so the worst case puts
  the EKF up to ~4 ms behind a fresh sample. Here the EKF runs the moment the
  IMU publishes; the IMU-to-EKF latency is bounded by one ScheduleNow + one
  context switch from `wq:imu` to `wq:nav`.
- **One context switch per stage instead of two.** Today each task does
  `vTaskDelayUntil` + (for the IMU) `ulTaskNotifyTake`, so every stage costs a
  block + wake pair. Here a stage transition is one `xTaskNotifyGive` to a
  higher-priority worker that runs in the next scheduling slot.
- **Stages on the same queue do not context-switch at all.** Once FCS and PWM
  migrate to `wq:nav`, the chain EKF -> FCS -> PWM runs to completion inside
  one task context per IMU sample.
- **No same-priority round-robin.** Seven sensor/control tasks currently sit
  at `osPriorityAboveNormal` and time-slice at 1 ms tick boundaries. The new
  queues each occupy a distinct priority band.
- **Per-item runtime visibility.** Every work item tracks
  `last_runtime_cycles_` / `max_runtime_cycles_` from `DWT->CYCCNT`, and the
  EKF additionally records autocode-only step cycles. Periodic overruns are
  counted by the queue itself. None of this exists today.

## What this is *not* (yet)

- A full migration. Only IMU and EKF are ported here. Net task count change
  is +1 wq:imu, +1 wq:nav, -1 ReadIcm20948, -1 StateEstimator = +0. The
  context-switch reduction starts when other modules join their queues.
- A change to pubsub. The IMU and EKF still publish through the existing
  `PubSubManager`. Subscribers (including the polling FCS task) keep working
  unchanged.
- A replacement for the FreeRTOSConfig priority assignments. CMSIS-RTOS V2
  priority names are mapped onto FreeRTOS priorities by
  `WorkQueue::ToFreeRtosPriority()`.

## Files

work_queue framework:
- `work_queue/work_queue_config.h` — queue id enum + four PX4-style configs
- `work_queue/work_item.h` / `.cc` — work item base + downstream consumer registry
- `work_queue/scheduled_work_item.h` — periodic-schedule helpers (unchanged)
- `work_queue/work_queue.h` / `.cc` — static FreeRTOS-backed worker (unchanged)
- `work_queue/work_queue_manager.h` — static queue manager API (unchanged)
- `work_queue/work_queue_manager.cc` — slot storage for the four queues

Modules:
- `sensors/icm20948_work_item.h` / `.cc` — IMU driver on `wq:imu`
- `state_estimator/state_estimator_work_item.h` / `.cc` — EKF on `wq:nav`

Integration sketch:
- `integration_example.cc` — main.cc bring-up sequence and wiring diagram

## What is intentionally not duplicated here

- The ICM20948 register configuration helpers from production
  `Core/Src/sensors/icm20948/icm20948.cc`. Keep those unchanged on migration;
  only the FreeRTOS task loop / notification plumbing is replaced.
- A `CMakeLists.txt`. These files are review-only and do not need to compile
  inside this directory.
