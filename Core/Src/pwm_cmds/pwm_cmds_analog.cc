/*
 * pwm_cmds_analog.cc
 *
 *  Created on: Jun 10, 2025
 *      Author: tanmay
 */

#include "pwm_cmds_analog.h"

// Definition of the static member variable
PwmCmds* PwmCmds::pwm_cmds_instance_handle_ = nullptr;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

#if !CONTROL_PIPELINE_ENABLE
PwmCmds pwm_cmds_task_instance_(&htim1, &htim2, &htim3);
#endif

PwmCmds::PwmCmds(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2, TIM_HandleTypeDef* htim3,
                 bool register_task):
TaskBase("PwmOut250Hz", 1024, osPriorityHigh, register_task),
pwm_timer1_(htim1),
pwm_timer2_(htim2),
pwm_timer3_(htim3){
	pwm_cmds_instance_handle_ = this;
}

const char* PwmCmds::ChannelToStr(uint32_t ch) {
  switch (ch) {
    case TIM_CHANNEL_1: return "CH1";
    case TIM_CHANNEL_2: return "CH2";
    case TIM_CHANNEL_3: return "CH3";
    case TIM_CHANNEL_4: return "CH4";
    default: return "UNKNOWN";
  }
}

inline uint16_t PwmCmds::PwmToOneShot(uint16_t throttle) const {
    // Clamp throttle
    uint16_t clamped = std::max(kMinRcThrottle, std::min(throttle, kMaxRcThrottle));

    // Scale and shift
    float pwm = (clamped - kValidMinRcThrottle) *
                (kMaxPwmVal - kMinPwmVal) / static_cast<float>(kMaxRcThrottle - kValidMinRcThrottle) +
                kMinPwmVal;

    return static_cast<uint16_t>(pwm);
}

void PwmCmds::InitializeOutputs() {
#ifndef MODE_PASSTHROUGH
	 __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, kMinPwmVal);
	 __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, kMinPwmVal);
	 __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, kMinPwmVal);
	 __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, kMinPwmVal);
#endif
}

void PwmCmds::ApplyPwmData(const PwmData& pwm_data) {
	uint16_t pwm_oneshot42_cmd = PwmToOneShot(pwm_data.pwm_cmds[0]);
	__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, pwm_oneshot42_cmd);

	pwm_oneshot42_cmd = PwmToOneShot(pwm_data.pwm_cmds[1]);
	__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, pwm_oneshot42_cmd);

	pwm_oneshot42_cmd = PwmToOneShot(pwm_data.pwm_cmds[2]);
	__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, pwm_oneshot42_cmd);

	pwm_oneshot42_cmd = PwmToOneShot(pwm_data.pwm_cmds[3]);
	__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, pwm_oneshot42_cmd);
}

void PwmCmds::Run() {
	DEBUG_PRINT("Starting PWM CMDS Module\n");

	InitializeOutputs();
	osDelay(2);

#ifndef MODE_TEST
	const TickType_t loop_frequency = pdMS_TO_TICKS(LOOP_INTERVAL_MS);  // 1000Hz
#endif

#ifdef MODE_TEST
	uint16_t delta_pwm_val = 2000;
	uint16_t current_pwm_val = kMinPwmVal;
	uint8_t current_mtr_idx = 0;
#endif
	osDelay(2000);
#ifndef MODE_TEST
	TickType_t xLastWakeTime = xTaskGetTickCount();
	ConfigurePeriodicMetrics(LOOP_INTERVAL_MS * 1000U,
			LOOP_INTERVAL_MS * 1000U);
#endif
	while(1){
#ifndef MODE_TEST
		BeginMetricsCycle();
#endif
#ifdef MODE_TEST
		//Sequence through each motors
//		DEBUG_PRINT("Current Mtr Idx: %d, Current PWM Val: %d\n", current_mtr_idx, current_pwm_val);
		//MOTOR 1
		if(current_mtr_idx == 0){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, current_pwm_val);
			current_pwm_val += delta_pwm_val;
			if (current_pwm_val > kMaxPwmVal){
				osDelay(500);
				current_pwm_val = kMinPwmVal;
				current_mtr_idx += 1;
				__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, current_pwm_val);
				osDelay(2000);
				continue;
			}else{
				osDelay(500);
			}
		}

		//MOTOR 2
		if(current_mtr_idx == 1){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, current_pwm_val);
			current_pwm_val += delta_pwm_val;
			if (current_pwm_val > kMaxPwmVal){
				osDelay(500);
				current_pwm_val = kMinPwmVal;
				current_mtr_idx += 1;
				__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, current_pwm_val);
				osDelay(2000);
				continue;
			}else{
				osDelay(500);
			}
		}

		//MOTOR 3
		if(current_mtr_idx == 2){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, current_pwm_val);
			current_pwm_val += delta_pwm_val;
			if (current_pwm_val > kMaxPwmVal){
				osDelay(500);
				current_pwm_val = kMinPwmVal;
				current_mtr_idx += 1;
				__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, current_pwm_val);
				osDelay(2000);
				continue;
			}else{
				osDelay(500);
			}
		}

		//MOTOR 3
		if(current_mtr_idx == 3){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, current_pwm_val);
			current_pwm_val += delta_pwm_val;
			if (current_pwm_val > kMaxPwmVal){
				osDelay(500);
				current_pwm_val = kMinPwmVal;
				current_mtr_idx = 0;
				__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, current_pwm_val);
				osDelay(5000);
				continue;
			}else{
				osDelay(500);
			}
		}
#elif defined MODE_PASSTHROUGH
		if (rcchannels_sub_.copy(rcchannels_data_)) {
			uint16_t pwm_oneshot42_cmd = PwmToOneShot(rcchannels_data_.throttle);
			DEBUG_PRINT("RC Throttle: %d, PWM One Shot: %d\n", rcchannels_data_.throttle, pwm_oneshot42_cmd);
		    __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, static_cast<uint16_t>(pwm_oneshot42_cmd));
		    __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, static_cast<uint16_t>(pwm_oneshot42_cmd));
		    __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, static_cast<uint16_t>(pwm_oneshot42_cmd));
		    __HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, static_cast<uint16_t>(pwm_oneshot42_cmd));
		}

		EndMetricsCycle();
		vTaskDelayUntil(&xLastWakeTime, loop_frequency);

#else
		if(pwm_sub_.copy(pwm_data_)){
			ApplyPwmData(pwm_data_);
		}
		EndMetricsCycle();
		vTaskDelayUntil(&xLastWakeTime, loop_frequency);
#endif
	}
}
