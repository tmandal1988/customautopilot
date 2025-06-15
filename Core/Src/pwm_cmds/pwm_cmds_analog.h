/*
 * pwm_cmds_analog.h
 *
 *  Created on: Jun 10, 2025
 *      Author: tanmay
 */

#ifndef SRC_PWM_CMDS_PWM_CMDS_ANALOG_H_
#define SRC_PWM_CMDS_PWM_CMDS_ANALOG_H_

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

//#define MODE_TEST
//#define MODE_PASSTHROUGH

#ifdef MODE_PASSTHROUGH
#include "messages/rc_channels.h"
#endif

class PwmCmds : public TaskBase {
public:
	PwmCmds(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2, TIM_HandleTypeDef* htim3);
    void Run() override;

	static PwmCmds* pwm_cmds_instance_handle_;
private:
	TIM_HandleTypeDef* pwm_timer1_;  // PWM Timer 1 Handle
	TIM_HandleTypeDef* pwm_timer2_;  // PWM Timer 1 Handle
	TIM_HandleTypeDef* pwm_timer3_;  // PWM Timer 1 Handle

	static constexpr uint16_t LOOP_INTERVAL_MS = 1; // 1000Hz

	static constexpr uint16_t kMinPwmVal = 24000;
	static constexpr uint16_t kMaxPwmVal = 48000;

//#ifndef MODE_PASSTHROUGH
	static constexpr uint16_t kMaxRcThrottle = 2000;
	static constexpr uint16_t kValidMinRcThrottle = 1000;
	static constexpr uint16_t kMinRcThrottle = 950;
//#else
//	static constexpr uint16_t kMaxRcThrottle = 1811;
//	static constexpr uint16_t kValidMinRcThrottle = 172;
//	static constexpr uint16_t kMinRcThrottle = 172;
//#endif

	Subscriber<PwmData> pwm_sub_ = Subscriber<PwmData>(TopicID::PWM);
	PwmData pwm_data_ = {0};

	const char* ChannelToStr(uint32_t ch);
	inline uint16_t PwmToOneShot(uint16_t throttle) const;

#ifdef MODE_PASSTHROUGH
	Subscriber<RcChannels> rcchannels_sub_ = Subscriber<RcChannels>(TopicID::RCCHANNELS);
	RcChannels rcchannels_data_;
#endif
};

#endif /* SRC_PWM_CMDS_PWM_CMDS_ANALOG_H_ */
