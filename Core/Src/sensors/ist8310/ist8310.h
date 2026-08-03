/*
 * ist8310.h
 *
 * Raw IST8310 magnetometer acquisition on I2C1.
 */

#pragma once

#include <cstdint>

#include "FreeRTOS.h"
#include "pin_defines.h"
#include "task.h"
#include "task_manager/task_base.h"
#include "ist8310_config.h"

class ReadIst8310 final : public TaskBase {
public:
    explicit ReadIst8310(I2C_HandleTypeDef* i2c_handle);

    void Run() override;

    // Called only by the central HAL I2C callback dispatcher.
    static void DmaCompleteCallback(I2C_HandleTypeDef* i2c_handle);
    static void MemTxCompleteCallback(I2C_HandleTypeDef* i2c_handle);
    static void ErrorCallback(I2C_HandleTypeDef* i2c_handle);

private:
    struct RawSample {
        int16_t x;
        int16_t y;
        int16_t z;
    };

    enum class TransferResult : uint8_t {
        kIdle,
        kPending,
        kComplete,
        kError,
    };

    // Which non-blocking I2C transaction, if any, currently owns the bus. Only
    // the task writes this; the ISR path keys off transfer_result_ alone.
    enum class TransferPhase : uint8_t {
        kIdle,
        kTriggerWrite,
        kStatusRead,
        kAxisRead,
    };

    enum class MeasurementState : uint8_t {
        kNeedsTrigger,
        kWaitingForReady,
    };

    enum class FailureStage : uint8_t {
        kNone,
        kInvalidState,
        kInitialization,
        kInitialTrigger,
        kStatusReadStart,
        kStatusReadTransfer,
        kStatusReadTimeout,
        kAxesDmaStart,
        kAxesDmaTransfer,
        kAxesDmaTimeout,
        kAcquisitionTrigger,
        kTriggerWriteTransfer,
        kTriggerWriteTimeout,
        kPostSampleTrigger,
        kConversionTimeout,
    };

    // Schedule the first DRDY poll 5 RTOS ticks after each trigger. Axis data
    // is read only after the sensor asserts DRDY.
    static constexpr uint32_t kMeasurementReadDelayMs = 5;
    static constexpr uint32_t kNotReadyPollIntervalMs = 1;
    static constexpr uint32_t kAllowedReadLatenessMs = 1;
    // Covers the longest runtime transaction (a 6-byte register read is under
    // 300 us on this 400 kHz bus) with room for preemption.
    static constexpr uint32_t kTransferTimeoutMs = 5;
    static constexpr uint32_t kRegisterTimeoutMs = 10;
    static constexpr uint32_t kConversionReadyTimeoutMs = 10;
    static constexpr uint32_t kResetTimeoutMs = 50;
    static constexpr uint32_t kRecoveryDelayMs = 250;
    static constexpr uint32_t kTriggerRetryDelayMs = 1;
    static constexpr uint32_t kBusClearHalfPeriodMs = 1;
#if IST8310_ENABLE_DIAGNOSTICS
    static constexpr uint32_t kPrintEveryNSamples = 20;  // Nominal 10 Hz output.
    static constexpr uint32_t kPrintTaskStackDepth = 256;  // 1024 bytes.
#endif
    static constexpr uint32_t kMaxConsecutiveFailures = 3;
    static constexpr uint32_t kMaxConsecutiveConversionTimeouts = 1;
#if IST8310_ENABLE_DIAGNOSTICS
    static constexpr uint32_t kInitErrorPrintDivider = 20;
#endif
    static constexpr uint8_t kTriggerAttempts = 3;
    static constexpr uint8_t kBusClearClockPulses = 9;
    static constexpr float kMicroteslaPerLsb = 0.3F;

    static constexpr uint8_t kFirstI2cAddress = 0x0C;
    static constexpr uint8_t kLastI2cAddress = 0x0F;
    static constexpr uint8_t kDefaultI2cAddress = 0x0E;
    static constexpr uint8_t kWhoAmIValue = 0x10;

    static constexpr uint8_t kRegWhoAmI = 0x00;
    static constexpr uint8_t kRegStatus1 = 0x02;
    static constexpr uint8_t kRegDataXLow = 0x03;
    static constexpr uint8_t kRegControl1 = 0x0A;
    static constexpr uint8_t kRegControl2 = 0x0B;
    static constexpr uint8_t kRegAverageControl = 0x41;
    static constexpr uint8_t kRegPulseDurationControl = 0x42;

    static constexpr uint8_t kStatusDataReady = 1U << 0;
    static constexpr uint8_t kStatusDataOverrun = 1U << 1;
    static constexpr uint8_t kControl1SingleMeasurement = 0x01;
    static constexpr uint8_t kControl2SoftReset = 1U << 0;
    static constexpr uint8_t kControl2DataReadyEnable = 1U << 3;
    static constexpr uint8_t kControl2DataReadyActiveHigh = 1U << 2;
    static constexpr uint8_t kControl2RunValue =
        kControl2DataReadyEnable | kControl2DataReadyActiveHigh;
    static constexpr uint8_t kDefaultAveraging = 0x00;
    static constexpr uint8_t kPulseDurationValue = 0xC0;

    static constexpr uint16_t kAxisDataLength = 6;

    bool InitializeSensor();
    bool SelectAndResetDevice();
    bool SoftReset();
    bool VerifyIdentity();
    bool ConfigureSensor();
    bool ReinitializeI2cPeripheral();
    bool ClearI2cBus();
    bool StartMeasurement();
    bool ArmMeasurement(uint8_t max_attempts);

    // Runtime acquisition uses interrupt/DMA transfers exclusively, so the task
    // is blocked - not spinning inside the HAL - for every byte on the wire.
    bool PrepareTransfer();
    void RecordStartFailure(FailureStage stage);
    bool StartTriggerWrite();
    bool StartStatusRead();
    bool StartAxisRead();
    void AbortTransfer();
    void NoteFirstPollLateness(TickType_t now);

    static FailureStage TransferErrorStage(TransferPhase phase);
    static FailureStage TransferTimeoutStage(TransferPhase phase);
    static bool TickReached(TickType_t now, TickType_t deadline);

    HAL_StatusTypeDef ReadRegister(uint8_t reg, uint8_t* data,
                                   uint16_t size);
    HAL_StatusTypeDef WriteRegister(uint8_t reg, uint8_t value);
    bool WriteAndVerifyRegister(uint8_t reg, uint8_t value,
                                uint8_t verify_mask = 0xFF);

    void NotifyFromIsr(TransferResult result, uint32_t hal_error);
#if IST8310_ENABLE_DIAGNOSTICS
    void QueueSampleForPrint(const RawSample& sample);
    void PrintSample(const RawSample& sample) const;

    static void PrintTaskEntry(void* argument);
    void PrintTask();
#endif

    static int16_t DecodeLittleEndian(const uint8_t* bytes);
#if IST8310_ENABLE_DIAGNOSTICS
    static uint32_t Magnitude(int32_t value);
    static const char* FailureStageName(FailureStage stage);
#endif

    static ReadIst8310* instance_;

    I2C_HandleTypeDef* const i2c_;
    TaskHandle_t task_handle_ = nullptr;
#if IST8310_ENABLE_DIAGNOSTICS
    TaskHandle_t print_task_handle_ = nullptr;
#endif
    uint8_t device_address_ = kDefaultI2cAddress;

    // I2C1 DMA is in normal mode. D-cache is currently disabled in this project.
    alignas(4) uint8_t dma_buffer_[kAxisDataLength] = {};
    // Separate from dma_buffer_ so a retrigger can be launched before the axis
    // bytes are decoded.
    alignas(4) uint8_t status_buffer_[1] = {};
    uint8_t trigger_command_ = kControl1SingleMeasurement;
    volatile TransferResult transfer_result_ = TransferResult::kIdle;
    TransferPhase transfer_phase_ = TransferPhase::kIdle;
    volatile uint32_t last_hal_error_ = HAL_I2C_ERROR_NONE;
    volatile FailureStage last_failure_stage_ = FailureStage::kNone;
    MeasurementState measurement_state_ = MeasurementState::kNeedsTrigger;
    TickType_t measurement_started_tick_ = 0;
    TickType_t next_action_tick_ = 0;
    bool first_status_poll_pending_ = false;

#if RTOS_METRICS_ENABLE
    // Aux metric slots reported through Topic 12:
    //   0 = transfer start failures, 1 = transfer timeouts,
    //   2 = max I2C wait (cycles), 3 = max decode+publish (cycles),
    //   4 = max context switches per cycle.
    uint32_t metrics_transfer_start_failures_ = 0;
    uint32_t metrics_transfer_timeouts_ = 0;
#endif

#if IST8310_ENABLE_DIAGNOSTICS
    // The optional low-priority printer consumes only the latest decimated
    // sample, so USB backpressure cannot grow a queue or block sensor reads.
    StaticTask_t print_task_control_block_ = {};
    StackType_t print_task_stack_[kPrintTaskStackDepth] = {};
    RawSample pending_print_sample_ = {};
    uint32_t pending_print_sequence_ = 0;

    uint32_t valid_sample_count_ = 0;
    uint32_t data_overrun_count_ = 0;
    uint32_t not_ready_count_ = 0;
    uint32_t conversion_timeout_count_ = 0;
    uint32_t bus_recovery_count_ = 0;
    uint32_t first_read_late_count_ = 0;
#endif
};
