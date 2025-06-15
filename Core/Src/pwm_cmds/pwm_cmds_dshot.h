/*
 * pwm_cmds.h
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#ifndef SRC_PWM_CMDS_PWM_CMDS_DSHOT_H_
#define SRC_PWM_CMDS_PWM_CMDS_DSHOT_H_

#include "task_manager/task_base.h"
#include "messages/pwm_data.h"
#include "pubsub/subscriber.h"
#include "debug.h"
#include "pin_defines.h"

/* Timer To PWM Channel Map
* TIM1 -> CH1 - PWM1
* TIM1 -> CH2 - PWM2
* TIM1 -> CH3 - PWM3
* TIM1 -> CH4 - PWM4
* TIM2 -> CH1 - PWM5
* TIM2 -> CH3 - PWM6
* TIM2 -> CH4 - PWM7
* TIM3 -> CH1 - PWM8
*/

#define MODE_TEST
//#define MODE_PASSTHROUGH

#ifdef MODE_PASSTHROUGH
#include "messages/rc_channels.h"
#endif

class PwmCmdsDshot : public TaskBase {
public:
	PwmCmdsDshot(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2, TIM_HandleTypeDef* htim3, uint16_t dshot_speed);
    void Run() override;

	static PwmCmdsDshot* pwm_cmds_instance_handle_;
	void OnPwmDmaComplete(TIM_HandleTypeDef* htim);
private:
	static constexpr int kDshotBits = 16;
	static constexpr int kDshotBufferSize = kDshotBits + 1;
	TIM_HandleTypeDef* pwm_timer1_;  // PWM Timer 1 Handle
	TIM_HandleTypeDef* pwm_timer2_;  // PWM Timer 1 Handle
	TIM_HandleTypeDef* pwm_timer3_;  // PWM Timer 1 Handle

	static constexpr uint16_t LOOP_INTERVAL_MS = 10; // 100Hz
	static constexpr int kOneSecIntervalCount = 1000 / LOOP_INTERVAL_MS;

	Subscriber<PwmData> pwm_sub_ = Subscriber<PwmData>(TopicID::PWM);
	PwmData pwm_data_ = {0};

	uint32_t dma_buffer1_[kDshotBufferSize];
	uint32_t dma_buffer2_[kDshotBufferSize];
	uint32_t dma_buffer3_[kDshotBufferSize];
	uint32_t dma_buffer4_[kDshotBufferSize];

	uint16_t dshot_speed_ ;
	uint32_t timer_clock_hz_;
	uint16_t bit_total_ticks_;
	uint16_t bit1_high_ticks_;
	uint16_t bit0_high_ticks_;

	uint32_t GetTimerClockHz(TIM_HandleTypeDef* htim);
	void ComputeTiming(TIM_HandleTypeDef* htim);
	uint16_t CreatePacket(uint16_t cmd);
	void EncodePacket(uint16_t packet, TIM_HandleTypeDef* htim, uint32_t channel);
	void SendCmd(TIM_HandleTypeDef* htim, uint32_t channel, uint16_t cmd);
	uint32_t* GetDmaBuffer(TIM_HandleTypeDef* htim, uint32_t channel);
	const char* ChannelToStr(uint32_t ch);

	void ArmEsc(TIM_HandleTypeDef* tim, uint32_t ch);

	bool tim1_ch1_dma_cplt_ = true;

#ifdef MODE_PASSTHROUGH
	Subscriber<RcChannels> rcchannels_sub_ = Subscriber<RcChannels>(TopicID::RCCHANNELS);
	RcChannels rcchannels_data_;
#endif
};

#endif /* SRC_PWM_CMDS_PWM_CMDS_DSHOT_H_ */
