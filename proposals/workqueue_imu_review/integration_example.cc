// Review-only integration sketch. Do not add this file to the build as-is.
//
// Inside main.cc, replace the single TaskManager::Instance().StartAll() with
// the bring-up sequence below to wire the IMU and the EKF onto the new
// work-queue framework:
//
//   /* 1. Init the EKF *first* so it registers itself as a downstream
//    *    consumer of the IMU before the IMU starts publishing. After this
//    *    call, every IMU publish will ScheduleNow() the EKF on wq:nav. */
//   if (!state_estimator_work_item.Init()) {
//       ERROR_PRINT("StateEstimator init failed\n");
//   }
//
//   /* 2. Init the IMU before starting the queues. This performs the I2C
//    *    register configuration and installs the 2 ms periodic schedule on
//    *    wq:imu. Periodic table mutation is init-only in this proposal. */
//   if (!read_icm20948_work_item.Init()) {
//       ERROR_PRINT("ICM20948 init failed\n");
//   }
//
//   /* 3. Start the worker tasks after all periodic entries are installed.
//    *    The IMU and EKF constructors already requested wq:imu and wq:nav
//    *    at static-init time, so only those two queues will actually spawn
//    *    workers. wq:hp and wq:lp stay dormant until another module migrates. */
//   if (!work_queue::WorkQueueManager::StartAll()) {
//       ERROR_PRINT("Failed to start work queues\n");
//   }
//
//   /* 4. Anything still on the old TaskBase framework (PWM, FCS, baro, GPS,
//    *    MTF01P, RC SBUS, MAVLink, logger, blink LED) starts the way it
//    *    always did. As each one migrates to a WorkItem the entry below
//    *    naturally shrinks. */
//   TaskManager::Instance().StartAll();
//
// Remove the old TaskBase instances for the IMU and the StateEstimator from
// task registration (just delete the global ReadIcm20948 and StateEstimator
// objects in their .cc files). The pubsub topics they advertised (ICM20948,
// EKF) are kept, so downstream consumers — including the FCS task still
// running under TaskBase — continue to work without change.
//
// Wiring diagram once IMU and EKF are migrated:
//
//   I2C2 DMA-done ISR
//        |
//        | AddFromISR(kEvent) + xTaskNotifyGiveFromISR
//        v
//   wq:imu  ReadIcm20948WorkItem::Run(kEvent)
//                CompleteDmaRead()
//                    parse buffer (23 bytes)
//                    publish ImuData
//                    NotifyDownstreamConsumers()
//                        |
//                        | ScheduleNow() + xTaskNotifyGive
//                        v
//                   wq:nav  StateEstimatorWorkItem::Run(kEvent)
//                                CopyImuInputs() / CopyAuxInputs()
//                                autocode_.step()
//                                publish EkfData
//                                NotifyDownstreamConsumers()  --> (future FCS -> PWM)
//
//   Independent periodic schedule on wq:imu:
//     every 2 ms: ReadIcm20948WorkItem::Run(kPeriodic)
//                     StartDmaReadIfReady()
//
// When FCS migrates, repeat the EKF pattern verbatim:
//   * Derive from work_queue::WorkItem (no ScheduledWorkItem needed).
//   * Bind to work_queue::configurations::nav_and_control.
//   * In FcsWorkItem::Init(), call
//         state_estimator_work_item.RegisterDownstreamConsumer(*this);
//   * In FcsWorkItem::Run(), copy EkfData, run fcsModelObj_.step(), publish
//     PwmData, then NotifyDownstreamConsumers() to wake PwmWorkItem.
//
// All three (EKF, FCS, PWM) then share wq:nav and run back-to-back inside a
// single task context per IMU sample — no inter-stage context switches.
