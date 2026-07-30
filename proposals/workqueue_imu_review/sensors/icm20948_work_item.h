#pragma once

#include "main.h"
#include "pin_defines.h"
#include "constants.h"
#include "debug.h"
#include "messages/imu_data.h"
#include "pubsub/pubsub_manager.h"
#include "work_queue/scheduled_work_item.h"

// ICM20948 IMU driver running on wq:imu.
//
// Two trigger paths into Run():
//   * kPeriodic (every 2 ms): start a new DMA read of the ACCEL/GYRO/MAG
//     register block. The HAL state check guards against starting a second
//     transfer before the previous one completes.
//   * kEvent (from I2C MemRx ISR): parse the just-completed transfer,
//     publish ImuData, and wake every downstream consumer (e.g. the EKF on
//     wq:nav) via WorkItem::NotifyDownstreamConsumers().
//
// The two schedules ride independent legs of the worker loop, so the
// "DMA done -> publish -> downstream wake" path never blocks the next
// periodic DMA kick.
class ReadIcm20948WorkItem : public work_queue::ScheduledWorkItem {
public:
    explicit ReadIcm20948WorkItem(I2C_HandleTypeDef* i2c_handle);

    // Advertise the topic, init the device over I2C, and arm the 2 ms
    // periodic schedule. Returns false if the device fails to respond.
    bool Init();

    // Invoked from HAL_I2C_MemRxCpltCallback. Marks the buffer ready and
    // schedules this work item as a kEvent so CompleteDmaRead() runs on the
    // worker thread (out of ISR context).
    void OnDmaCompleteFromISR(BaseType_t* higher_priority_task_woken);

    // Static back-pointer used by the HAL C callback. Set in the constructor;
    // never reassigned. Single-instance driver.
    static ReadIcm20948WorkItem* instance;

private:
    static constexpr uint16_t kReadIntervalMs = 2;     // 500 Hz
    static constexpr uint8_t  kRawBufferSize  = 23;

    static constexpr uint8_t  ICM20948_ADDR             = 0x68;
    static constexpr uint8_t  UB0_ACCEL_XOUT_H          = 0x2D;
    static constexpr uint8_t  who_am_i_val_             = 0xEA;
    static constexpr uint32_t icm_i2c_wait_time_ms_     = HAL_MAX_DELAY;

    void StartDmaReadIfReady();
    void CompleteDmaRead();
    void Icm20948GetData(ImuData* icm20948_data);

    void Run(work_queue::WorkReason reason) override;

    static int16_t ReadBe16(const uint8_t* data) {
        return static_cast<int16_t>(
            (static_cast<uint16_t>(data[0]) << 8) |
            static_cast<uint16_t>(data[1]));
    }

    static int16_t ReadLe16(const uint8_t* data) {
        return static_cast<int16_t>(
            (static_cast<uint16_t>(data[1]) << 8) |
            static_cast<uint16_t>(data[0]));
    }

    bool Icm20948Init();

    HAL_StatusTypeDef Icm20948Write(uint8_t mem_address, uint8_t* data,
                                    uint16_t size, uint32_t timeout);
    HAL_StatusTypeDef Icm20948Read(uint8_t mem_address, uint8_t* data,
                                   uint16_t size, uint32_t timeout);

    // Register configuration helpers from the production icm20948.cc are
    // intentionally not duplicated here. Keep those unchanged during the
    // migration; only the FreeRTOS task loop plumbing is replaced.

    I2C_HandleTypeDef* icm20948_i2c_;
    uint8_t            icm20948_raw_buf_[kRawBufferSize];
    ImuData            imu_data_;

    volatile bool initialized_;
    volatile bool dma_pending_;
    volatile bool dma_complete_;

    // Diagnostic counters. Visible via the public WorkItem stats plus these
    // driver-specific ones (DMA accounting, parse cost in cycles).
    uint32_t dma_start_count_;
    uint32_t dma_complete_count_;
    uint32_t dma_start_busy_count_;
    uint32_t dma_start_error_count_;
    uint32_t parse_publish_count_;
    uint32_t last_parse_cycles_;
};

extern ReadIcm20948WorkItem read_icm20948_work_item;
