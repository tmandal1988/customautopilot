/*
 * pwm_cmds_dshot.h
 *
 * DShot motor output on TIM1 CH1..CH4, one DMA stream per channel.
 *
 * ---------------------------------------------------------------------------
 * Frame format
 * ---------------------------------------------------------------------------
 * Every DShot frame is exactly 16 bits, sent MSB first:
 *
 *     bits 15..5 : 11-bit value      0        = motor stop / disarmed
 *                                    1..47    = reserved commands (beep,
 *                                               direction, save settings...)
 *                                    48..2047 = throttle, 0 % .. 100 %
 *     bit  4     : telemetry request (1 = ESC replies on its telemetry wire)
 *     bits 3..0  : CRC-4
 *
 * CRC is computed over the 12 bits formed by (value << 1 | telemetry):
 *     crc = (packet ^ (packet >> 4) ^ (packet >> 8)) & 0x0F
 * A frame failing CRC is discarded by the ESC, so line noise cannot become a
 * throttle command the way it can with analog PWM.
 *
 * ---------------------------------------------------------------------------
 * Bit encoding on the wire
 * ---------------------------------------------------------------------------
 * There is no separate clock. Each bit occupies one fixed-length period and
 * the bit's value is carried by the duty cycle of that period:
 *
 *     '1' -> line high for 75   % of the bit period
 *     '0' -> line high for 37.5 % of the bit period
 *
 * That maps directly onto a timer in PWM mode: ARR holds the bit period and
 * each bit is one CCR value. A 16-entry DMA burst into CCR therefore emits a
 * whole frame with no CPU involvement after the transfer is kicked off.
 *
 * Bit period is set by the DShot rate:
 *     DShot150 = 6.67 us, DShot300 = 3.33 us,
 *     DShot600 = 1.67 us, DShot1200 = 0.83 us
 *
 * ---------------------------------------------------------------------------
 * Start-up: how the ESC knows to expect DShot
 * ---------------------------------------------------------------------------
 * Nothing tells it explicitly. BLHeli_32 auto-detects the input protocol by
 * measuring the signal it sees after power-up: it classifies analog PWM,
 * OneShot125/42, Multishot and each DShot rate from the edge timing, then
 * locks that choice until the next power cycle.
 *
 * Two consequences drive the sequencing in InitializeOutputs():
 *   1. The flight controller must already be emitting valid frames when the
 *      ESC powers up, or shortly after. If the line is idle the ESC keeps
 *      waiting and never detects a protocol.
 *   2. The ESC only arms after a continuous run of zero-throttle frames.
 *      SendIdleFrames() supplies exactly that.
 *
 * Because detection happens once per power cycle, switching between analog
 * PWM and DShot always requires powering the ESCs down and back up.
 */

#pragma once

#include "motor_protocol_config.h"

#if MOTOR_PROTOCOL_DSHOT

#include <cstdint>

#include "debug.h"
#include "messages/pwm_data.h"
#include "pin_defines.h"
#include "pubsub/subscriber.h"
#include "task_manager/task_base.h"

class PwmCmdsDshot : public TaskBase {
public:
    PwmCmdsDshot(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2,
                 TIM_HandleTypeDef* htim3, bool register_task = true);

    void Run() override;

    // Reconfigures TIM1 for the DShot bit period and holds the ESCs at zero
    // throttle long enough for them to detect the protocol and arm.
    void InitializeOutputs();

    // Emits one 16-bit frame per motor. Called from the 250 Hz control loop.
    void ApplyPwmData(const PwmData& pwm_data);

    static PwmCmdsDshot* instance_;

    // Called only by HAL_TIM_PWM_PulseFinishedCallback.
    static void PulseFinishedCallback(TIM_HandleTypeDef* htim);

private:
    static constexpr uint16_t LOOP_INTERVAL_MS = 4;  // 250 Hz

    // DShot300 is the deliberate default. DShot600 halves the bit period and
    // becomes marginal over the long ESC signal leads typical of a large
    // multi-ESC airframe; 300 keeps generous edge margin and is still ~60 us
    // per frame against a 4 ms control period.
    static constexpr uint32_t kDshotBitrateHz = 300000U;

    static constexpr uint8_t kMotorCount = 4U;
    static constexpr uint8_t kFrameBits = 16U;
    // Two trailing zero-duty entries park the line low once the frame ends.
    // Without them CCR would retain the final bit's duty and hold the line
    // high, which the ESC reads as a framing error.
    static constexpr uint8_t kTrailingIdleEntries = 2U;
    static constexpr uint8_t kBufferLength = kFrameBits + kTrailingIdleEntries;

    // DShot value space.
    static constexpr uint16_t kDshotMotorStop = 0U;
    static constexpr uint16_t kDshotMinThrottle = 48U;
    static constexpr uint16_t kDshotMaxThrottle = 2047U;

    // Control-side throttle range, matching the analog driver exactly so the
    // two protocols present an identical command interface to the FCS.
    static constexpr uint16_t kMaxRcThrottle = 2000U;
    static constexpr uint16_t kValidMinRcThrottle = 1000U;
    static constexpr uint16_t kRcThrottleSpan =
        kMaxRcThrottle - kValidMinRcThrottle;

    // Arming: BLHeli_32 needs a sustained run of zero-throttle frames before
    // it will accept throttle. 2 s at 1 kHz is comfortably beyond the
    // requirement and covers ESC boot and tone playback.
    static constexpr uint32_t kIdleFrameCount = 2000U;
    static constexpr uint32_t kIdleFramePeriodMs = 1U;

    TIM_HandleTypeDef* pwm_timer1_;
    TIM_HandleTypeDef* pwm_timer2_;
    TIM_HandleTypeDef* pwm_timer3_;

    // Derived from the timer kernel clock in ConfigureTimebase().
    uint32_t bit_period_ticks_ = 0U;
    uint32_t bit1_high_ticks_ = 0U;
    uint32_t bit0_high_ticks_ = 0U;

    // DMA is configured word-wide (DMA_MDATAALIGN_WORD), so the buffer must
    // be uint32_t. D-cache is disabled project-wide, so no maintenance is
    // required around these transfers.
    alignas(4) uint32_t dshot_buffer_[kMotorCount][kBufferLength] = {};

    // Set when a channel's DMA is in flight, cleared by its completion
    // callback. Guards against re-arming a transfer that has not finished.
    volatile uint8_t transfer_active_mask_ = 0U;
    uint32_t dropped_frame_count_ = 0U;

    static uint32_t TimerKernelClockHz();
    static uint32_t ChannelForIndex(uint8_t index);

    void ConfigureTimebase();
    void SendIdleFrames(uint32_t frame_count);

    // Pure functions; unit-checkable on a host.
    static uint16_t BuildFrame(uint16_t value11, bool telemetry_request);
    uint16_t ThrottleToDshot(uint16_t rc_throttle) const;
    void FillBuffer(uint8_t motor_index, uint16_t frame);

    void TransmitAll();
    void OnPulseFinished(TIM_HandleTypeDef* htim);

    Subscriber<PwmData> pwm_sub_ = Subscriber<PwmData>(TopicID::PWM);
    PwmData pwm_data_ = {0};
};

#endif  // MOTOR_PROTOCOL_DSHOT
