#include "sensors/icm20948_work_item.h"

ReadIcm20948WorkItem* ReadIcm20948WorkItem::instance = nullptr;

extern I2C_HandleTypeDef hi2c2;
ReadIcm20948WorkItem read_icm20948_work_item(&hi2c2);

ReadIcm20948WorkItem::ReadIcm20948WorkItem(I2C_HandleTypeDef* i2c_handle)
    : work_queue::ScheduledWorkItem(
          "Icm20948",
          work_queue::configurations::imu_bus),
      icm20948_i2c_(i2c_handle),
      icm20948_raw_buf_{},
      imu_data_{},
      initialized_(false),
      dma_pending_(false),
      dma_complete_(false),
      dma_start_count_(0),
      dma_complete_count_(0),
      dma_start_busy_count_(0),
      dma_start_error_count_(0),
      parse_publish_count_(0),
      last_parse_cycles_(0) {
    instance = this;
}

bool ReadIcm20948WorkItem::Init() {
    pubSubManager().advertise<ImuData>(TopicID::ICM20948);

    // Enable the cycle counter so RunPostamble() and CompleteDmaRead() can
    // measure execution cost. Idempotent: safe if other modules also touch it.
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    initialized_ = Icm20948Init();
    if (initialized_) {
        (void)ScheduleOnInterval(pdMS_TO_TICKS(kReadIntervalMs));
    }

    return initialized_;
}

void ReadIcm20948WorkItem::Run(work_queue::WorkReason reason) {
    if (!initialized_) {
        return;
    }

    // Always drain a pending DMA result first so the parse/publish path runs
    // even on a kPeriodic firing that happens to coincide with a completion.
    if (dma_complete_) {
        CompleteDmaRead();
    }

    if (reason == work_queue::WorkReason::kPeriodic) {
        StartDmaReadIfReady();
    }
}

void ReadIcm20948WorkItem::OnDmaCompleteFromISR(
    BaseType_t* higher_priority_task_woken) {
    dma_complete_ = true;
    dma_pending_  = false;
    ++dma_complete_count_;

    (void)ScheduleNowFromISR(higher_priority_task_woken);
}

void ReadIcm20948WorkItem::StartDmaReadIfReady() {
    if (dma_pending_) {
        ++dma_start_busy_count_;
        return;
    }

    if (HAL_I2C_GetState(icm20948_i2c_) != HAL_I2C_STATE_READY) {
        ++dma_start_busy_count_;
        return;
    }

    dma_pending_ = true;
    const HAL_StatusTypeDef status = HAL_I2C_Mem_Read_DMA(
        icm20948_i2c_,
        ICM20948_ADDR << 1,
        UB0_ACCEL_XOUT_H,
        I2C_MEMADD_SIZE_8BIT,
        icm20948_raw_buf_,
        kRawBufferSize);

    if (status == HAL_OK) {
        ++dma_start_count_;
    } else {
        dma_pending_ = false;
        ++dma_start_error_count_;
    }
}

void ReadIcm20948WorkItem::CompleteDmaRead() {
    dma_complete_ = false;

    const uint32_t start_cycles = DWT->CYCCNT;
    Icm20948GetData(&imu_data_);
    pubSubManager().publish<ImuData>(TopicID::ICM20948, imu_data_);

    // Wake every downstream consumer (currently the EKF on wq:nav). Each
    // ScheduleNow() pushes onto the consumer's queue and signals its worker;
    // failures are accounted against the consumer, never against us.
    NotifyDownstreamConsumers();

    last_parse_cycles_ = DWT->CYCCNT - start_cycles;
    ++parse_publish_count_;
}

void ReadIcm20948WorkItem::Icm20948GetData(ImuData* icm20948_data) {
    const int16_t accel_x = ReadBe16(&icm20948_raw_buf_[0]);
    const int16_t accel_y = ReadBe16(&icm20948_raw_buf_[2]);
    const int16_t accel_z = ReadBe16(&icm20948_raw_buf_[4]);

    const int16_t gyro_x = ReadBe16(&icm20948_raw_buf_[6]);
    const int16_t gyro_y = ReadBe16(&icm20948_raw_buf_[8]);
    const int16_t gyro_z = ReadBe16(&icm20948_raw_buf_[10]);

    const int16_t temp_raw = ReadBe16(&icm20948_raw_buf_[12]);

    const int16_t mag_x = ReadLe16(&icm20948_raw_buf_[15]);
    const int16_t mag_y = ReadLe16(&icm20948_raw_buf_[17]);
    const int16_t mag_z = ReadLe16(&icm20948_raw_buf_[19]);

    icm20948_data->temp_degc = ((static_cast<float>(temp_raw) - 21.0f) / 333.87f) + 21.0f;

    icm20948_data->mag_ut[0] = static_cast<float>(mag_x) * 0.15f;
    icm20948_data->mag_ut[1] = static_cast<float>(mag_y) * 0.15f;
    icm20948_data->mag_ut[2] = static_cast<float>(mag_z) * 0.15f;

    icm20948_data->mag_st1 = icm20948_raw_buf_[14] & 0x03;
    icm20948_data->mag_st2 = icm20948_raw_buf_[22] & 0x08;

    icm20948_data->accel_mps2[0] =  static_cast<float>(accel_x) / 2048.0f;
    icm20948_data->accel_mps2[1] = -static_cast<float>(accel_y) / 2048.0f;
    icm20948_data->accel_mps2[2] = -static_cast<float>(accel_z) / 2048.0f;

    icm20948_data->gyro_radps[0] =  (static_cast<float>(gyro_x) / 16.4f) * DEG2RAD;
    icm20948_data->gyro_radps[1] = -(static_cast<float>(gyro_y) / 16.4f) * DEG2RAD;
    icm20948_data->gyro_radps[2] = -(static_cast<float>(gyro_z) / 16.4f) * DEG2RAD;
}

extern "C" void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    if ((hi2c->Instance == ICM20948I2C) && (ReadIcm20948WorkItem::instance != nullptr)) {
        BaseType_t higher_priority_task_woken = pdFALSE;
        ReadIcm20948WorkItem::instance->OnDmaCompleteFromISR(&higher_priority_task_woken);
        portYIELD_FROM_ISR(higher_priority_task_woken);
    }
}
