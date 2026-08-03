/*
 * ist8310.cc
 *
 * The IST8310 has no continuous-measurement mode. This task waits from the
 * actual trigger time, checks DRDY, reads one completed single-shot with DMA,
 * then retriggers it.
 */

#include "ist8310.h"

#include "messages/magnetometer_data.h"
#include "pubsub/publisher.h"

#if IST8310_ENABLE_DIAGNOSTICS
#include "debug.h"
#endif

ReadIst8310* ReadIst8310::instance_ = nullptr;

extern I2C_HandleTypeDef hi2c1;
ReadIst8310 read_ist8310_task_instance_(&hi2c1);

ReadIst8310::ReadIst8310(I2C_HandleTypeDef* i2c_handle)
    : TaskBase("Ist8310Task", 1536, osPriorityNormal),
      i2c_(i2c_handle) {
    instance_ = this;
}

HAL_StatusTypeDef ReadIst8310::ReadRegister(uint8_t reg, uint8_t* data,
                                            uint16_t size) {
    if ((i2c_ == nullptr) || (data == nullptr) || (size == 0U)) {
        return HAL_ERROR;
    }

    const HAL_StatusTypeDef status = HAL_I2C_Mem_Read(
        i2c_,
        static_cast<uint16_t>(device_address_) << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        size,
        kRegisterTimeoutMs);
    if (status != HAL_OK) {
        last_hal_error_ = HAL_I2C_GetError(i2c_);
    }
    return status;
}

HAL_StatusTypeDef ReadIst8310::WriteRegister(uint8_t reg, uint8_t value) {
    if (i2c_ == nullptr) {
        return HAL_ERROR;
    }

    const HAL_StatusTypeDef status = HAL_I2C_Mem_Write(
        i2c_,
        static_cast<uint16_t>(device_address_) << 1,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        &value,
        1,
        kRegisterTimeoutMs);
    if (status != HAL_OK) {
        last_hal_error_ = HAL_I2C_GetError(i2c_);
    }
    return status;
}

bool ReadIst8310::WriteAndVerifyRegister(uint8_t reg, uint8_t value,
                                         uint8_t verify_mask) {
    constexpr uint8_t kAttempts = 3;

    for (uint8_t attempt = 0; attempt < kAttempts; ++attempt) {
        uint8_t readback = 0;
        if ((WriteRegister(reg, value) == HAL_OK) &&
            (ReadRegister(reg, &readback, 1) == HAL_OK) &&
            ((readback & verify_mask) == (value & verify_mask))) {
            return true;
        }
        osDelay(1);
    }

    return false;
}

bool ReadIst8310::VerifyIdentity() {
    uint8_t who_am_i = 0;
    return (ReadRegister(kRegWhoAmI, &who_am_i, 1) == HAL_OK) &&
           (who_am_i == kWhoAmIValue);
}

bool ReadIst8310::SoftReset() {
    if (WriteRegister(kRegControl2, kControl2SoftReset) != HAL_OK) {
        return false;
    }

    const TickType_t reset_start = xTaskGetTickCount();
    const TickType_t reset_timeout = pdMS_TO_TICKS(kResetTimeoutMs);

    do {
        uint8_t control2 = kControl2SoftReset;
        osDelay(1);
        if ((ReadRegister(kRegControl2, &control2, 1) == HAL_OK) &&
            ((control2 & kControl2SoftReset) == 0U)) {
            return true;
        }
    } while ((xTaskGetTickCount() - reset_start) < reset_timeout);

    return false;
}

bool ReadIst8310::SelectAndResetDevice() {
    // Probe the default first, then every alternate CAD0/CAD1 strap. Identity
    // is checked before reset so a missing device is never written blindly.
    constexpr uint8_t kProbeOrder[] = {
        kDefaultI2cAddress, kFirstI2cAddress,
        static_cast<uint8_t>(kFirstI2cAddress + 1U), kLastI2cAddress};

    for (const uint8_t address : kProbeOrder) {
        device_address_ = address;
        if (VerifyIdentity() && SoftReset() && VerifyIdentity()) {
            return true;
        }
    }

    device_address_ = kDefaultI2cAddress;
    return false;
}

bool ReadIst8310::ConfigureSensor() {
    // 0x24 enables 16x averaging but requires at least 6 ms. Keep the reset
    // default averaging to retain the documented 5 ms maximum-rate mode.
    if (!WriteAndVerifyRegister(kRegAverageControl, kDefaultAveraging)) {
        return false;
    }

    if (!WriteAndVerifyRegister(kRegPulseDurationControl,
                                kPulseDurationValue)) {
        return false;
    }

    if (!WriteAndVerifyRegister(kRegControl2, kControl2RunValue,
                                kControl2DataReadyEnable |
                                    kControl2DataReadyActiveHigh)) {
        return false;
    }

    return true;
}

bool ReadIst8310::InitializeSensor() {
    if (i2c_ == nullptr) {
        return false;
    }

    if (HAL_I2C_GetState(i2c_) != HAL_I2C_STATE_READY) {
        if (!ReinitializeI2cPeripheral()) {
            return false;
        }
    }

    return SelectAndResetDevice() && ConfigureSensor();
}

bool ReadIst8310::ReinitializeI2cPeripheral() {
    if ((i2c_ == nullptr) || (i2c_->Instance != I2C1)) {
        return false;
    }

    transfer_result_ = TransferResult::kIdle;
    transfer_phase_ = TransferPhase::kIdle;
    measurement_state_ = MeasurementState::kNeedsTrigger;
    first_status_poll_pending_ = false;
    if (task_handle_ != nullptr) {
        (void)ulTaskNotifyTake(pdTRUE, 0);
    }

    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
    HAL_NVIC_DisableIRQ(DMA2_Stream4_IRQn);

    CLEAR_BIT(i2c_->Instance->CR1, I2C_CR1_RXDMAEN | I2C_CR1_TXDMAEN);
    if ((i2c_->hdmarx != nullptr) &&
        (HAL_DMA_GetState(i2c_->hdmarx) == HAL_DMA_STATE_BUSY)) {
        (void)HAL_DMA_Abort(i2c_->hdmarx);
    }

    // DeInit releases the alternate-function pins. Force-resetting the kernel
    // also clears a peripheral state machine stalled by a noisy edge.
    (void)HAL_I2C_DeInit(i2c_);
    __HAL_RCC_I2C1_FORCE_RESET();
    __HAL_RCC_I2C1_RELEASE_RESET();

    HAL_NVIC_ClearPendingIRQ(I2C1_EV_IRQn);
    HAL_NVIC_ClearPendingIRQ(I2C1_ER_IRQn);
    HAL_NVIC_ClearPendingIRQ(DMA2_Stream4_IRQn);

    const bool bus_released = ClearI2cBus();
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

    if (HAL_I2C_Init(i2c_) != HAL_OK) {
        HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
        return false;
    }
    if (HAL_I2CEx_ConfigAnalogFilter(i2c_, I2C_ANALOGFILTER_ENABLE) !=
        HAL_OK) {
        HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
        return false;
    }
    if (HAL_I2CEx_ConfigDigitalFilter(i2c_, 0) != HAL_OK) {
        HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
        return false;
    }

    HAL_NVIC_ClearPendingIRQ(DMA2_Stream4_IRQn);
    HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
#if IST8310_ENABLE_DIAGNOSTICS
    ++bus_recovery_count_;
#endif
    return bus_released;
}

bool ReadIst8310::ClearI2cBus() {
    constexpr uint16_t kSclPin = GPIO_PIN_8;
    constexpr uint16_t kSdaPin = GPIO_PIN_9;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Preload both open-drain outputs high so changing GPIO mode cannot create
    // an accidental START condition.
    HAL_GPIO_WritePin(GPIOB, kSclPin | kSdaPin, GPIO_PIN_SET);

    GPIO_InitTypeDef gpio = {};
    gpio.Pin = kSclPin | kSdaPin;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);
    osDelay(kBusClearHalfPeriodMs);

    // Clock a slave out of a partial byte. Stop early once it releases SDA.
    for (uint8_t pulse = 0;
         (pulse < kBusClearClockPulses) &&
         (HAL_GPIO_ReadPin(GPIOB, kSdaPin) == GPIO_PIN_RESET);
         ++pulse) {
        HAL_GPIO_WritePin(GPIOB, kSclPin, GPIO_PIN_RESET);
        osDelay(kBusClearHalfPeriodMs);
        HAL_GPIO_WritePin(GPIOB, kSclPin, GPIO_PIN_SET);
        osDelay(kBusClearHalfPeriodMs);

        if (HAL_GPIO_ReadPin(GPIOB, kSclPin) == GPIO_PIN_RESET) {
            break;
        }
    }

    // Generate a STOP: SDA low while SCL is low, release SCL, then release SDA.
    HAL_GPIO_WritePin(GPIOB, kSclPin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, kSdaPin, GPIO_PIN_RESET);
    osDelay(kBusClearHalfPeriodMs);
    HAL_GPIO_WritePin(GPIOB, kSclPin, GPIO_PIN_SET);
    osDelay(kBusClearHalfPeriodMs);
    HAL_GPIO_WritePin(GPIOB, kSdaPin, GPIO_PIN_SET);
    osDelay(kBusClearHalfPeriodMs);

    return (HAL_GPIO_ReadPin(GPIOB, kSclPin) == GPIO_PIN_SET) &&
           (HAL_GPIO_ReadPin(GPIOB, kSdaPin) == GPIO_PIN_SET);
}

bool ReadIst8310::StartMeasurement() {
    return WriteRegister(kRegControl1, kControl1SingleMeasurement) == HAL_OK;
}

bool ReadIst8310::ArmMeasurement(uint8_t max_attempts) {
    measurement_state_ = MeasurementState::kNeedsTrigger;
    first_status_poll_pending_ = false;

    for (uint8_t attempt = 0; attempt < max_attempts; ++attempt) {
        if (StartMeasurement()) {
            measurement_started_tick_ = xTaskGetTickCount();
            measurement_state_ = MeasurementState::kWaitingForReady;
            first_status_poll_pending_ = true;
            return true;
        }

        if ((attempt + 1U) < max_attempts) {
            osDelay(kTriggerRetryDelayMs);
        }
    }

    return false;
}

bool ReadIst8310::TickReached(TickType_t now, TickType_t deadline) {
    return static_cast<int32_t>(now - deadline) >= 0;
}

void ReadIst8310::NoteFirstPollLateness(TickType_t now) {
    if (!first_status_poll_pending_) {
        return;
    }

    // Only the first poll after a trigger sits inside the protected conversion
    // window; later DRDY polls are already outside it.
    first_status_poll_pending_ = false;

#if IST8310_ENABLE_DIAGNOSTICS
    const TickType_t allowed = pdMS_TO_TICKS(kMeasurementReadDelayMs) +
                               pdMS_TO_TICKS(kAllowedReadLatenessMs);
    if ((now - measurement_started_tick_) > allowed) {
        ++first_read_late_count_;
    }
#else
    static_cast<void>(now);
#endif
}

bool ReadIst8310::PrepareTransfer() {
    if ((task_handle_ == nullptr) || (i2c_ == nullptr) ||
        (HAL_I2C_GetState(i2c_) != HAL_I2C_STATE_READY)) {
        return false;
    }

    // A notification always belongs to the transfer started next. Dropping a
    // stale one prevents a late callback from retiring the new transaction.
    (void)ulTaskNotifyTake(pdTRUE, 0);
    transfer_result_ = TransferResult::kPending;
    return true;
}

void ReadIst8310::RecordStartFailure(FailureStage stage) {
    last_hal_error_ =
        (i2c_ != nullptr) ? HAL_I2C_GetError(i2c_) : HAL_I2C_ERROR_NONE;
    last_failure_stage_ = stage;
    transfer_result_ = TransferResult::kIdle;
    transfer_phase_ = TransferPhase::kIdle;
#if RTOS_METRICS_ENABLE
    ++metrics_transfer_start_failures_;
    SetAuxMetric(0U, metrics_transfer_start_failures_);
#endif
}

bool ReadIst8310::StartTriggerWrite() {
    if (!PrepareTransfer()) {
        RecordStartFailure(FailureStage::kAcquisitionTrigger);
        return false;
    }

    trigger_command_ = kControl1SingleMeasurement;
    transfer_phase_ = TransferPhase::kTriggerWrite;
    if (HAL_I2C_Mem_Write_IT(i2c_,
                             static_cast<uint16_t>(device_address_) << 1,
                             kRegControl1,
                             I2C_MEMADD_SIZE_8BIT,
                             &trigger_command_,
                             1) != HAL_OK) {
        RecordStartFailure(FailureStage::kAcquisitionTrigger);
        return false;
    }

    return true;
}

bool ReadIst8310::StartStatusRead() {
    if (!PrepareTransfer()) {
        RecordStartFailure(FailureStage::kStatusReadStart);
        return false;
    }

    transfer_phase_ = TransferPhase::kStatusRead;
    if (HAL_I2C_Mem_Read_IT(i2c_,
                            static_cast<uint16_t>(device_address_) << 1,
                            kRegStatus1,
                            I2C_MEMADD_SIZE_8BIT,
                            status_buffer_,
                            1) != HAL_OK) {
        RecordStartFailure(FailureStage::kStatusReadStart);
        return false;
    }

    return true;
}

bool ReadIst8310::StartAxisRead() {
    if (!PrepareTransfer()) {
        RecordStartFailure(FailureStage::kAxesDmaStart);
        return false;
    }

    transfer_phase_ = TransferPhase::kAxisRead;
    if (HAL_I2C_Mem_Read_DMA(i2c_,
                             static_cast<uint16_t>(device_address_) << 1,
                             kRegDataXLow,
                             I2C_MEMADD_SIZE_8BIT,
                             dma_buffer_,
                             kAxisDataLength) != HAL_OK) {
        RecordStartFailure(FailureStage::kAxesDmaStart);
        return false;
    }

    return true;
}

void ReadIst8310::AbortTransfer() {
    // Clearing ownership first stops a callback that lands during the abort
    // from posting a notification for a transaction the task has given up on.
    taskENTER_CRITICAL();
    transfer_result_ = TransferResult::kIdle;
    taskEXIT_CRITICAL();
    transfer_phase_ = TransferPhase::kIdle;

    if (i2c_ != nullptr) {
        (void)HAL_I2C_Master_Abort_IT(
            i2c_, static_cast<uint16_t>(device_address_) << 1);
    }
    (void)ulTaskNotifyTake(pdTRUE, 0);
}

ReadIst8310::FailureStage ReadIst8310::TransferErrorStage(TransferPhase phase) {
    switch (phase) {
        case TransferPhase::kTriggerWrite:
            return FailureStage::kTriggerWriteTransfer;
        case TransferPhase::kStatusRead:
            return FailureStage::kStatusReadTransfer;
        case TransferPhase::kAxisRead:
            return FailureStage::kAxesDmaTransfer;
        case TransferPhase::kIdle:
        default:
            return FailureStage::kInvalidState;
    }
}

ReadIst8310::FailureStage ReadIst8310::TransferTimeoutStage(
    TransferPhase phase) {
    switch (phase) {
        case TransferPhase::kTriggerWrite:
            return FailureStage::kTriggerWriteTimeout;
        case TransferPhase::kStatusRead:
            return FailureStage::kStatusReadTimeout;
        case TransferPhase::kAxisRead:
            return FailureStage::kAxesDmaTimeout;
        case TransferPhase::kIdle:
        default:
            return FailureStage::kInvalidState;
    }
}

int16_t ReadIst8310::DecodeLittleEndian(const uint8_t* bytes) {
    const uint16_t value = static_cast<uint16_t>(bytes[0]) |
                           (static_cast<uint16_t>(bytes[1]) << 8);
    return static_cast<int16_t>(value);
}

#if IST8310_ENABLE_DIAGNOSTICS
uint32_t ReadIst8310::Magnitude(int32_t value) {
    return static_cast<uint32_t>((value < 0) ? -value : value);
}

const char* ReadIst8310::FailureStageName(FailureStage stage) {
    switch (stage) {
        case FailureStage::kInvalidState:
            return "invalid-state";
        case FailureStage::kInitialization:
            return "initialization";
        case FailureStage::kInitialTrigger:
            return "initial-trigger";
        case FailureStage::kStatusReadStart:
            return "status-read-start";
        case FailureStage::kStatusReadTransfer:
            return "status-read-transfer";
        case FailureStage::kStatusReadTimeout:
            return "status-read-timeout";
        case FailureStage::kAxesDmaStart:
            return "axes-dma-start";
        case FailureStage::kAxesDmaTransfer:
            return "axes-dma-transfer";
        case FailureStage::kAxesDmaTimeout:
            return "axes-dma-timeout";
        case FailureStage::kAcquisitionTrigger:
            return "acquisition-trigger";
        case FailureStage::kTriggerWriteTransfer:
            return "trigger-write-transfer";
        case FailureStage::kTriggerWriteTimeout:
            return "trigger-write-timeout";
        case FailureStage::kPostSampleTrigger:
            return "post-sample-trigger";
        case FailureStage::kConversionTimeout:
            return "conversion-timeout";
        case FailureStage::kNone:
        default:
            return "none";
    }
}

void ReadIst8310::PrintSample(const RawSample& sample) const {
    // Reset-default 14-bit data is 0.3 uT/LSB. Keeping tenths as integers
    // avoids costly floating-point formatting in this real-time task. Apply
    // the same Yaw270 sensor-to-body rotation used by the published topic:
    // body X = sensor Y, body Y = -sensor X, body Z = sensor Z.
    const int32_t x_tenths = static_cast<int32_t>(sample.y) * 3;
    const int32_t y_tenths = static_cast<int32_t>(sample.x) * 3;
    const int32_t z_tenths = static_cast<int32_t>(sample.z) * 3;
    const uint32_t x_magnitude = Magnitude(x_tenths);
    const uint32_t y_magnitude = Magnitude(y_tenths);
    const uint32_t z_magnitude = Magnitude(z_tenths);

    DEBUG_PRINT(
        "IST8310 body FRD Yaw270 [uT]: "
        "X=%s%lu.%01lu, Y=%s%lu.%01lu, Z=%s%lu.%01lu\n",
        (x_tenths < 0) ? "-" : "",
        static_cast<unsigned long>(x_magnitude / 10U),
        static_cast<unsigned long>(x_magnitude % 10U),
        (y_tenths < 0) ? "-" : "",
        static_cast<unsigned long>(y_magnitude / 10U),
        static_cast<unsigned long>(y_magnitude % 10U),
        (z_tenths < 0) ? "-" : "",
        static_cast<unsigned long>(z_magnitude / 10U),
        static_cast<unsigned long>(z_magnitude % 10U));
}

void ReadIst8310::QueueSampleForPrint(const RawSample& sample) {
    if (print_task_handle_ == nullptr) {
        return;
    }

    taskENTER_CRITICAL();
    pending_print_sample_ = sample;
    ++pending_print_sequence_;
    taskEXIT_CRITICAL();
    xTaskNotifyGive(print_task_handle_);
}

void ReadIst8310::PrintTaskEntry(void* argument) {
    auto* const instance = static_cast<ReadIst8310*>(argument);
    if (instance != nullptr) {
        instance->PrintTask();
    }
    vTaskDelete(nullptr);
}

void ReadIst8310::PrintTask() {
    uint32_t last_printed_sequence = 0;

    for (;;) {
        (void)ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        RawSample sample = {};
        uint32_t sequence = 0;
        taskENTER_CRITICAL();
        sample = pending_print_sample_;
        sequence = pending_print_sequence_;
        taskEXIT_CRITICAL();

        if (sequence != last_printed_sequence) {
            PrintSample(sample);
            last_printed_sequence = sequence;
        }
    }
}
#endif

void ReadIst8310::NotifyFromIsr(TransferResult result, uint32_t hal_error) {
    if ((task_handle_ == nullptr) ||
        (transfer_result_ != TransferResult::kPending)) {
        return;
    }

    // The task owns transfer_phase_ and classifies the failing stage itself,
    // so the ISR only records the transport error.
    if (result == TransferResult::kError) {
        last_hal_error_ = hal_error;
    }
    transfer_result_ = result;

    BaseType_t higher_priority_task_woken = pdFALSE;
    vTaskNotifyGiveFromISR(task_handle_, &higher_priority_task_woken);
    portYIELD_FROM_ISR(higher_priority_task_woken);
}

void ReadIst8310::DmaCompleteCallback(I2C_HandleTypeDef* i2c_handle) {
    if ((instance_ != nullptr) && (i2c_handle == instance_->i2c_)) {
        instance_->NotifyFromIsr(TransferResult::kComplete,
                                 HAL_I2C_ERROR_NONE);
    }
}

void ReadIst8310::MemTxCompleteCallback(I2C_HandleTypeDef* i2c_handle) {
    if ((instance_ != nullptr) && (i2c_handle == instance_->i2c_)) {
        instance_->NotifyFromIsr(TransferResult::kComplete,
                                 HAL_I2C_ERROR_NONE);
    }
}

void ReadIst8310::ErrorCallback(I2C_HandleTypeDef* i2c_handle) {
    if ((instance_ != nullptr) && (i2c_handle == instance_->i2c_)) {
        instance_->NotifyFromIsr(TransferResult::kError,
                                 HAL_I2C_GetError(i2c_handle));
    }
}

void ReadIst8310::Run() {
    task_handle_ = xTaskGetCurrentTaskHandle();
    ConfigureEventMetrics();
    Publisher<MagnetometerData> mag_pub(TopicID::IST8310);

#if IST8310_ENABLE_DIAGNOSTICS
    print_task_handle_ = xTaskCreateStatic(
        PrintTaskEntry,
        "Ist8310Print",
        kPrintTaskStackDepth,
        this,
        tskIDLE_PRIORITY + 1U,
        print_task_stack_,
        &print_task_control_block_);
    if (print_task_handle_ == nullptr) {
        ERROR_PRINT("IST8310 print task creation failed\n");
    }

    uint32_t initialization_failures = 0;
#endif

    for (;;) {
        last_hal_error_ = HAL_I2C_ERROR_NONE;
        last_failure_stage_ = FailureStage::kNone;

        const bool initialized = InitializeSensor();
        const bool initially_armed =
            initialized && ArmMeasurement(kTriggerAttempts);
        if (!initialized || !initially_armed) {
            last_failure_stage_ = initialized
                                      ? FailureStage::kInitialTrigger
                                      : FailureStage::kInitialization;
#if IST8310_ENABLE_DIAGNOSTICS
            ++initialization_failures;
            const uint32_t initialization_hal_error =
                last_hal_error_;
            const FailureStage initialization_stage = last_failure_stage_;
#endif
            (void)ReinitializeI2cPeripheral();

#if IST8310_ENABLE_DIAGNOSTICS
            if ((initialization_failures == 1U) ||
                ((initialization_failures % kInitErrorPrintDivider) == 0U)) {
                ERROR_PRINT(
                    "IST8310 initialization failed on I2C1; retrying "
                    "(stage=%s, HAL error 0x%08lx)\n",
                    FailureStageName(initialization_stage),
                    static_cast<unsigned long>(initialization_hal_error));
            }
#endif

            osDelay(kRecoveryDelayMs);
            continue;
        }

#if IST8310_ENABLE_DIAGNOSTICS
        initialization_failures = 0;
#endif
        last_hal_error_ = HAL_I2C_ERROR_NONE;
        last_failure_stage_ = FailureStage::kNone;
#if IST8310_ENABLE_DIAGNOSTICS
        DEBUG_PRINT(
            "IST8310 initialized at I2C address 0x%02x "
            "(first DRDY poll target %lu ticks)\n",
            device_address_,
            static_cast<unsigned long>(
                pdMS_TO_TICKS(kMeasurementReadDelayMs)));
#endif

        uint32_t consecutive_failures = 0;
        uint32_t consecutive_conversion_timeouts = 0;

        transfer_result_ = TransferResult::kIdle;
        transfer_phase_ = TransferPhase::kIdle;
        next_action_tick_ =
            measurement_started_tick_ + pdMS_TO_TICKS(kMeasurementReadDelayMs);

        while ((consecutive_failures < kMaxConsecutiveFailures) &&
               (consecutive_conversion_timeouts <
                kMaxConsecutiveConversionTimeouts)) {
            if (transfer_phase_ == TransferPhase::kIdle) {
                const TickType_t now = xTaskGetTickCount();
                if (!TickReached(now, next_action_tick_)) {
                    // Conversion latency and the DRDY back-off are sensor time,
                    // not task work. Block for them outside a metrics cycle.
                    vTaskDelay(next_action_tick_ - now);
                    continue;
                }

#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
                const uint32_t idle_context_switch_start =
                    rtos_metrics::ContextSwitchCount();
#endif
                BeginMetricsCycle();
                if (measurement_state_ == MeasurementState::kNeedsTrigger) {
                    if (!StartTriggerWrite()) {
                        ++consecutive_failures;
                        next_action_tick_ =
                            now + pdMS_TO_TICKS(kTriggerRetryDelayMs);
                    }
                } else {
                    NoteFirstPollLateness(now);
                    if (!StartStatusRead()) {
                        measurement_state_ = MeasurementState::kNeedsTrigger;
                        ++consecutive_failures;
                        next_action_tick_ =
                            now + pdMS_TO_TICKS(kTriggerRetryDelayMs);
                    }
                }
                EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
                UpdateAuxMetricMaximum(4U,
                                       rtos_metrics::ContextSwitchCount() -
                                           idle_context_switch_start);
#endif
                continue;
            }

            // A transaction owns the bus. Everything up to its completion
            // notification is I2C wire time during which this task is blocked;
            // keeping it outside the cycle is what makes the reported execution
            // time the work this task actually performs.
#if RTOS_METRICS_ENABLE
            const uint32_t wait_start_cycles = rtos_metrics::CyclesNow();
#endif
            const uint32_t notified =
                ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(kTransferTimeoutMs));
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
            const uint32_t cycle_context_switch_start =
                rtos_metrics::ContextSwitchCount();
#endif
            BeginMetricsCycle();
#if RTOS_METRICS_ENABLE
            UpdateAuxMetricMaximum(
                2U, rtos_metrics::CyclesNow() - wait_start_cycles);
#endif

            // Retire ownership atomically. At the timeout boundary an I2C error
            // ISR may have completed just after ulTaskNotifyTake() returned
            // zero; its result must win over a synthetic timeout.
            taskENTER_CRITICAL();
            const TransferResult result = transfer_result_;
            if (result != TransferResult::kPending) {
                transfer_result_ = TransferResult::kIdle;
            }
            taskEXIT_CRITICAL();

            const TransferPhase phase = transfer_phase_;

            if (result != TransferResult::kComplete) {
                if ((notified == 0U) && (result == TransferResult::kPending)) {
                    last_hal_error_ = HAL_I2C_ERROR_TIMEOUT;
                    last_failure_stage_ = TransferTimeoutStage(phase);
#if RTOS_METRICS_ENABLE
                    ++metrics_transfer_timeouts_;
                    SetAuxMetric(1U, metrics_transfer_timeouts_);
#endif
                } else {
                    last_failure_stage_ = TransferErrorStage(phase);
                }

                // A transport error leaves the state of DRDY/data consumption
                // ambiguous, but a single glitched byte on a long, unshielded
                // I2C1 cable does not by itself mean the link is broken.
                // Tolerate a short run of consecutive errors before paying for
                // a full peripheral + bus reset.
                AbortTransfer();
                measurement_state_ = MeasurementState::kNeedsTrigger;
                ++consecutive_failures;
                // HAL_I2C_Master_Abort_IT() finishes asynchronously; retrying in
                // this same tick would only fail the peripheral-ready check.
                next_action_tick_ =
                    xTaskGetTickCount() + pdMS_TO_TICKS(kTriggerRetryDelayMs);
            } else {
                transfer_phase_ = TransferPhase::kIdle;

                switch (phase) {
                    case TransferPhase::kTriggerWrite:
                        // The conversion starts when CNTL1 lands, so time the
                        // DRDY poll from the completion of this write.
                        measurement_started_tick_ = xTaskGetTickCount();
                        measurement_state_ =
                            MeasurementState::kWaitingForReady;
                        first_status_poll_pending_ = true;
                        next_action_tick_ =
                            measurement_started_tick_ +
                            pdMS_TO_TICKS(kMeasurementReadDelayMs);
                        break;

                    case TransferPhase::kStatusRead: {
                        const uint8_t status = status_buffer_[0];
                        if ((status & kStatusDataReady) == 0U) {
#if IST8310_ENABLE_DIAGNOSTICS
                            ++not_ready_count_;
#endif
                            const TickType_t conversion_deadline =
                                measurement_started_tick_ +
                                pdMS_TO_TICKS(kConversionReadyTimeoutMs);
                            if (TickReached(xTaskGetTickCount(),
                                            conversion_deadline)) {
                                // The trigger was lost or the conversion
                                // stalled. Leave for a full reset instead of
                                // writing CNTL1 while the sensor may still be
                                // in an uncertain conversion state.
#if IST8310_ENABLE_DIAGNOSTICS
                                ++conversion_timeout_count_;
#endif
                                last_hal_error_ = HAL_I2C_ERROR_NONE;
                                last_failure_stage_ =
                                    FailureStage::kConversionTimeout;
                                measurement_state_ =
                                    MeasurementState::kNeedsTrigger;
                                consecutive_conversion_timeouts =
                                    kMaxConsecutiveConversionTimeouts;
                            } else {
                                next_action_tick_ =
                                    xTaskGetTickCount() +
                                    pdMS_TO_TICKS(kNotReadyPollIntervalMs);
                            }
                            break;
                        }

                        if ((status & kStatusDataOverrun) != 0U) {
#if IST8310_ENABLE_DIAGNOSTICS
                            ++data_overrun_count_;
#endif
                        }

                        // Follow the documented sequence: collect the completed
                        // measurement while the device is in standby.
                        if (!StartAxisRead()) {
                            measurement_state_ =
                                MeasurementState::kNeedsTrigger;
                            ++consecutive_failures;
                            next_action_tick_ = xTaskGetTickCount() +
                                pdMS_TO_TICKS(kTriggerRetryDelayMs);
                        }
                        break;
                    }

                    case TransferPhase::kAxisRead: {
                        // Reading the data registers already cleared DRDY, so
                        // this conversion is consumed either way. Re-arm first
                        // so the CNTL1 write overlaps the decode and publish
                        // below instead of following them.
                        measurement_state_ = MeasurementState::kNeedsTrigger;
                        const bool retriggered = StartTriggerWrite();

#if RTOS_METRICS_ENABLE
                        const uint32_t publish_start_cycles =
                            rtos_metrics::CyclesNow();
#endif
                        RawSample sample = {};
                        sample.x = DecodeLittleEndian(&dma_buffer_[0]);
                        sample.y = DecodeLittleEndian(&dma_buffer_[2]);
                        sample.z = DecodeLittleEndian(&dma_buffer_[4]);

                        MagnetometerData mag_data = {};
                        mag_data.mag_ut[0] =
                            static_cast<float>(sample.y) * kMicroteslaPerLsb;
                        mag_data.mag_ut[1] =
                            static_cast<float>(sample.x) * kMicroteslaPerLsb;
                        mag_data.mag_ut[2] =
                            static_cast<float>(sample.z) * kMicroteslaPerLsb;
                        mag_pub.publish(mag_data);
#if RTOS_METRICS_ENABLE
                        UpdateAuxMetricMaximum(
                            3U, rtos_metrics::CyclesNow() -
                                    publish_start_cycles);
#endif

#if IST8310_ENABLE_DIAGNOSTICS
                        ++valid_sample_count_;
                        if ((valid_sample_count_ % kPrintEveryNSamples) == 0U) {
                            QueueSampleForPrint(sample);
                        }
#endif

                        consecutive_failures = 0;
                        consecutive_conversion_timeouts = 0;
                        if (!retriggered) {
                            last_failure_stage_ =
                                FailureStage::kPostSampleTrigger;
                            ++consecutive_failures;
                            next_action_tick_ = xTaskGetTickCount() +
                                pdMS_TO_TICKS(kTriggerRetryDelayMs);
                        }
                        break;
                    }

                    case TransferPhase::kIdle:
                    default:
                        last_failure_stage_ = FailureStage::kInvalidState;
                        ++consecutive_failures;
                        next_action_tick_ = xTaskGetTickCount() +
                            pdMS_TO_TICKS(kTriggerRetryDelayMs);
                        break;
                }
            }

            EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
            UpdateAuxMetricMaximum(4U, rtos_metrics::ContextSwitchCount() -
                                           cycle_context_switch_start);
#endif
        }

        // Preserve the root cause because peripheral recovery resets the HAL
        // and DMA state before optional diagnostic output is allowed to block.
#if IST8310_ENABLE_DIAGNOSTICS
        taskENTER_CRITICAL();
        const uint32_t recovery_hal_error = last_hal_error_;
        const FailureStage recovery_stage = last_failure_stage_;
        taskEXIT_CRITICAL();
#endif

        // Stop/reset I2C and DMA before retrying. This also makes a timed-out
        // DMA buffer safe to reuse.
        (void)ReinitializeI2cPeripheral();

#if IST8310_ENABLE_DIAGNOSTICS
        ERROR_PRINT(
            "IST8310 acquisition recovery: stage=%s, HAL=0x%08lx, "
            "not-ready=%lu, "
            "conversion-timeout=%lu, overrun=%lu, bus-recovery=%lu, "
            "first-read-late=%lu\n",
            FailureStageName(recovery_stage),
            static_cast<unsigned long>(recovery_hal_error),
            static_cast<unsigned long>(not_ready_count_),
            static_cast<unsigned long>(conversion_timeout_count_),
            static_cast<unsigned long>(data_overrun_count_),
            static_cast<unsigned long>(bus_recovery_count_),
            static_cast<unsigned long>(first_read_late_count_));
#endif

        osDelay(kRecoveryDelayMs);
    }
}
