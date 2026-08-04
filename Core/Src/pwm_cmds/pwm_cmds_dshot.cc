/*
 * pwm_cmds_dshot.cc
 *
 * DShot300 output on TIM1 CH1..CH4. See pwm_cmds_dshot.h for the protocol
 * description and the ESC start-up sequence.
 *
 * The whole translation unit compiles away unless MOTOR_PROTOCOL_DSHOT is 1,
 * so an analog build is bit-for-bit unaffected by this file existing.
 */

#include "pwm_cmds_dshot.h"

#if MOTOR_PROTOCOL_DSHOT

#include "pubsub/publisher.h"

PwmCmdsDshot* PwmCmdsDshot::instance_ = nullptr;

PwmCmdsDshot::PwmCmdsDshot(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2,
                           TIM_HandleTypeDef* htim3, bool register_task)
    : TaskBase("MotorDshot250", 1024, osPriorityHigh, register_task),
      pwm_timer1_(htim1),
      pwm_timer2_(htim2),
      pwm_timer3_(htim3) {
    instance_ = this;
}

// TIM1 sits on APB2. When the APB2 prescaler is not 1 the timer kernel clock
// is twice PCLK2, which is the case in this clock tree (480 MHz SYSCLK ->
// 240 MHz HCLK -> 120 MHz PCLK2 -> 240 MHz timer clock). Deriving it at run
// time rather than hard-coding keeps the bit timing correct if the clock tree
// is ever retuned.
uint32_t PwmCmdsDshot::TimerKernelClockHz() {
    const uint32_t pclk2 = HAL_RCC_GetPCLK2Freq();
    const uint32_t apb2_prescaler =
        (RCC->D2CFGR & RCC_D2CFGR_D2PPRE2) >> RCC_D2CFGR_D2PPRE2_Pos;
    // Bit 2 of the field set means a divider greater than 1 is in use.
    return ((apb2_prescaler & 0x4U) != 0U) ? (pclk2 * 2U) : pclk2;
}

uint32_t PwmCmdsDshot::ChannelForIndex(uint8_t index) {
    switch (index) {
        case 0U:  return TIM_CHANNEL_1;
        case 1U:  return TIM_CHANNEL_2;
        case 2U:  return TIM_CHANNEL_3;
        default:  return TIM_CHANNEL_4;
    }
}

void PwmCmdsDshot::ConfigureTimebase() {
    const uint32_t timer_hz = TimerKernelClockHz();

    // One timer period per DShot bit. Prescaler 0 keeps the counter at full
    // rate so the 75 % / 37.5 % duty split has maximum resolution: at
    // 240 MHz / 300 kbit/s that is 800 ticks per bit.
    bit_period_ticks_ = timer_hz / kDshotBitrateHz;
    bit1_high_ticks_ = (bit_period_ticks_ * 3U) / 4U;    // 75 %
    bit0_high_ticks_ = (bit_period_ticks_ * 3U) / 8U;    // 37.5 %

    // main() starts these channels in plain PWM mode; they must be stopped
    // before the timebase changes and before DMA drives them.
    for (uint8_t i = 0U; i < kMotorCount; ++i) {
        (void)HAL_TIM_PWM_Stop(pwm_timer1_, ChannelForIndex(i));
    }

    __HAL_TIM_SET_PRESCALER(pwm_timer1_, 0U);
    __HAL_TIM_SET_AUTORELOAD(pwm_timer1_, bit_period_ticks_ - 1U);
    for (uint8_t i = 0U; i < kMotorCount; ++i) {
        __HAL_TIM_SET_COMPARE(pwm_timer1_, ChannelForIndex(i), 0U);
    }
    // Load the new prescaler/ARR immediately instead of at the next update.
    pwm_timer1_->Instance->EGR = TIM_EGR_UG;

    DEBUG_PRINT(
        "DShot%lu: timer %lu Hz, %lu ticks/bit (1=%lu, 0=%lu), frame %lu us\n",
        static_cast<unsigned long>(kDshotBitrateHz / 1000U),
        static_cast<unsigned long>(timer_hz),
        static_cast<unsigned long>(bit_period_ticks_),
        static_cast<unsigned long>(bit1_high_ticks_),
        static_cast<unsigned long>(bit0_high_ticks_),
        static_cast<unsigned long>(
            (kBufferLength * 1000000UL) / kDshotBitrateHz));
}

uint16_t PwmCmdsDshot::BuildFrame(uint16_t value11, bool telemetry_request) {
    const uint16_t packet = static_cast<uint16_t>(
        ((value11 & 0x07FFU) << 1) | (telemetry_request ? 1U : 0U));
    const uint16_t crc = static_cast<uint16_t>(
        (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x000FU);
    return static_cast<uint16_t>((packet << 4) | crc);
}

// Maps the FCS throttle command onto the DShot value space.
//
// Resolution note: pwm_cmds[] is a uint16_t carrying 1000..2000, i.e. 1001
// distinct commands. The DShot throttle range 48..2047 provides 1999 steps,
// so every distinct command maps to a distinct DShot value - roughly two
// DShot counts per input step. This mapping cannot coarsen the commanded
// throttle; the quantisation limit is the uint16_t in PwmData, which is
// common to both output protocols.
uint16_t PwmCmdsDshot::ThrottleToDshot(uint16_t rc_throttle) const {
    // At or below the FCS idle command the motors are commanded to stop.
    // This matches the analog driver, where 1000 us is the calibrated zero.
    if (rc_throttle <= kValidMinRcThrottle) {
        return kDshotMotorStop;
    }

    const uint16_t clamped =
        (rc_throttle > kMaxRcThrottle) ? kMaxRcThrottle : rc_throttle;
    const uint32_t span =
        static_cast<uint32_t>(clamped) - kValidMinRcThrottle;  // 1..1000

    // Rounded rather than truncated so no input step is lost to integer
    // division.
    const uint32_t scaled =
        ((span * (kDshotMaxThrottle - kDshotMinThrottle)) +
         (kRcThrottleSpan / 2U)) / kRcThrottleSpan;

    return static_cast<uint16_t>(kDshotMinThrottle + scaled);
}

void PwmCmdsDshot::FillBuffer(uint8_t motor_index, uint16_t frame) {
    uint32_t* const buffer = dshot_buffer_[motor_index];
    for (uint8_t bit = 0U; bit < kFrameBits; ++bit) {
        // MSB first.
        const bool is_one =
            (frame & static_cast<uint16_t>(0x8000U >> bit)) != 0U;
        buffer[bit] = is_one ? bit1_high_ticks_ : bit0_high_ticks_;
    }
    for (uint8_t i = kFrameBits; i < kBufferLength; ++i) {
        buffer[i] = 0U;
    }
}

void PwmCmdsDshot::TransmitAll() {
    for (uint8_t i = 0U; i < kMotorCount; ++i) {
        const uint8_t bit = static_cast<uint8_t>(1U << i);

        // A transfer still in flight means the previous frame has not drained.
        // Skipping is the safe response: the ESC simply holds its last valid
        // command, which is exactly what a dropped frame should do.
        if ((transfer_active_mask_ & bit) != 0U) {
            ++dropped_frame_count_;
            continue;
        }

        transfer_active_mask_ =
            static_cast<uint8_t>(transfer_active_mask_ | bit);
        if (HAL_TIM_PWM_Start_DMA(pwm_timer1_, ChannelForIndex(i),
                                  dshot_buffer_[i],
                                  kBufferLength) != HAL_OK) {
            transfer_active_mask_ =
                static_cast<uint8_t>(transfer_active_mask_ & ~bit);
            ++dropped_frame_count_;
        }
    }
}

void PwmCmdsDshot::ApplyPwmData(const PwmData& pwm_data) {
    for (uint8_t i = 0U; i < kMotorCount; ++i) {
        const uint16_t value = ThrottleToDshot(pwm_data.pwm_cmds[i]);
        FillBuffer(i, BuildFrame(value, false));
    }
    TransmitAll();
}

void PwmCmdsDshot::SendIdleFrames(uint32_t frame_count) {
    for (uint8_t i = 0U; i < kMotorCount; ++i) {
        FillBuffer(i, BuildFrame(kDshotMotorStop, false));
    }
    for (uint32_t frame = 0U; frame < frame_count; ++frame) {
        TransmitAll();
        osDelay(kIdleFramePeriodMs);
    }
}

void PwmCmdsDshot::InitializeOutputs() {
    ConfigureTimebase();

    // The ESC classifies the input protocol from the signal present after it
    // powers up, then waits for a run of zero-throttle frames before arming.
    // Both requirements are satisfied by streaming motor-stop frames here.
    DEBUG_PRINT("DShot: streaming idle frames for ESC detect/arm\n");
    SendIdleFrames(kIdleFrameCount);
    DEBUG_PRINT("DShot: idle sequence complete (%lu frames dropped)\n",
                static_cast<unsigned long>(dropped_frame_count_));
}

void PwmCmdsDshot::OnPulseFinished(TIM_HandleTypeDef* htim) {
    if (htim != pwm_timer1_) {
        return;
    }

    uint8_t index;
    uint32_t channel;
    switch (htim->Channel) {
        case HAL_TIM_ACTIVE_CHANNEL_1: index = 0U; channel = TIM_CHANNEL_1; break;
        case HAL_TIM_ACTIVE_CHANNEL_2: index = 1U; channel = TIM_CHANNEL_2; break;
        case HAL_TIM_ACTIVE_CHANNEL_3: index = 2U; channel = TIM_CHANNEL_3; break;
        case HAL_TIM_ACTIVE_CHANNEL_4: index = 3U; channel = TIM_CHANNEL_4; break;
        default: return;
    }

    // Normal-mode DMA is one-shot: stop it so the next frame can re-arm.
    (void)HAL_TIM_PWM_Stop_DMA(htim, channel);
    transfer_active_mask_ = static_cast<uint8_t>(
        transfer_active_mask_ & ~static_cast<uint8_t>(1U << index));
}

void PwmCmdsDshot::PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    if (instance_ != nullptr) {
        instance_->OnPulseFinished(htim);
    }
}

// Standalone-task entry point, used only when ControlPipeline is disabled.
// Under ControlPipeline this object is a sub-module and ApplyPwmData() is
// driven directly by the 250 Hz loop.
void PwmCmdsDshot::Run() {
    DEBUG_PRINT("Starting DShot motor output module\n");
    InitializeOutputs();

    ConfigurePeriodicMetrics(LOOP_INTERVAL_MS * 1000U,
                             LOOP_INTERVAL_MS * 1000U);
    const TickType_t loop_frequency = pdMS_TO_TICKS(LOOP_INTERVAL_MS);
    TickType_t last_wake_time = xTaskGetTickCount();

    for (;;) {
        BeginMetricsCycle();
        if (pwm_sub_.copy(pwm_data_)) {
            ApplyPwmData(pwm_data_);
        }
        EndMetricsCycle();
        vTaskDelayUntil(&last_wake_time, loop_frequency);
    }
}

// TIM1 is the only timer driven by DMA in this build, so this override is
// unambiguous. It replaces the HAL's weak no-op only when DShot is selected.
extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef* htim) {
    PwmCmdsDshot::PulseFinishedCallback(htim);
}

#if !CONTROL_PIPELINE_ENABLE
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
PwmCmdsDshot pwm_cmds_task_instance_(&htim1, &htim2, &htim3);
#endif

#endif  // MOTOR_PROTOCOL_DSHOT
