/*
 * pwm_cmds.cc
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#include "pwm_cmds_dshot.h"

// Definition of the static member variable
PwmCmdsDshot* PwmCmdsDshot::pwm_cmds_instance_handle_ = nullptr;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

//PwmCmdsDshot pwm_cmds_task_instance_(&htim1, &htim2, &htim3, 300);

PwmCmdsDshot::PwmCmdsDshot(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2, TIM_HandleTypeDef* htim3, uint16_t dshot_speed):
TaskBase("PwmCmdsTask", 1024, osPriorityNormal),
pwm_timer1_(htim1),
pwm_timer2_(htim2),
pwm_timer3_(htim3),
dshot_speed_(dshot_speed){
	pwm_cmds_instance_handle_ = this;
}

uint32_t PwmCmdsDshot::GetTimerClockHz(TIM_HandleTypeDef* htim) {
  RCC_ClkInitTypeDef clk_config;
  uint32_t flash_latency;
  HAL_RCC_GetClockConfig(&clk_config, &flash_latency);

  // TIM1 and TIM8 are on APB2
  if (htim->Instance == TIM1 || htim->Instance == TIM8) {
    uint32_t pclk = HAL_RCC_GetPCLK2Freq();

    // APB2 prescaler multiplier
    return (clk_config.APB2CLKDivider == RCC_HCLK_DIV1) ? pclk : (pclk * 2);
  }

  // TIM2 to TIM7 are on APB1
  if (htim->Instance == TIM2 || htim->Instance == TIM3 ||
      htim->Instance == TIM4 || htim->Instance == TIM5 ||
      htim->Instance == TIM6 || htim->Instance == TIM7) {
    uint32_t pclk = HAL_RCC_GetPCLK1Freq();

    // APB1 prescaler multiplier
    return (clk_config.APB1CLKDivider == RCC_HCLK_DIV1) ? pclk : (pclk * 2);
  }

  // Default fallback
  return 0;
}

void PwmCmdsDshot::ComputeTiming(TIM_HandleTypeDef* htim) {
  uint32_t timer_input_clk = GetTimerClockHz(htim);
  uint32_t prescaler = htim->Instance->PSC;
  uint32_t timer_tick_hz = timer_input_clk / (prescaler + 1U);

  bit_total_ticks_ = timer_tick_hz / (dshot_speed_ * 1000U);
  bit1_high_ticks_ = static_cast<uint16_t>(
      (static_cast<uint32_t>(bit_total_ticks_) * 75U) / 100U);
  bit0_high_ticks_ = static_cast<uint16_t>(
      (static_cast<uint32_t>(bit_total_ticks_) * 37U) / 100U);

  bit0_high_ticks_ = 300;
  DEBUG_PRINT("Total Tick: %d, High Tick: %d, Low Tick: %d\n", bit_total_ticks_, bit1_high_ticks_, bit0_high_ticks_);

  __HAL_TIM_SET_AUTORELOAD(htim, bit_total_ticks_ - 1);
}

void PwmCmdsDshot::ArmEsc(TIM_HandleTypeDef* tim, uint32_t ch)
{
	DEBUG_PRINT("Arming...............\n");
    const uint16_t idle = 0;                 /* zero-throttle value   */
    const int      frames = 1000;              /* BLHeli wants ~100     */
//    TickType_t last_send = xTaskGetTickCount();
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (int i = 0; i < frames; ++i) {
    	vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(LOOP_INTERVAL_MS));  // 1ms loop
//    	TickType_t now = xTaskGetTickCount();
//    	if(tim1_ch1_dma_cplt_){
//    		tim1_ch1_dma_cplt_ = false;
//    		last_send = now;
    		SendCmd(tim, ch, idle);               /* one D-Shot frame      */
//    	}
//        osDelay(1);                           /* 2 ms gap  → 500 Hz    */
    }
    DEBUG_PRINT("Arming Complete\n");
}

uint16_t PwmCmdsDshot::CreatePacket(uint16_t cmd) {
	uint16_t packet;

	packet = (cmd << 1) | 0;

	// compute checksum
	uint8_t csum = 0;
	uint16_t csum_data = packet;

	for (uint8_t i = 0; i < 3; i++)
	{
		csum ^= csum_data; // xor data by nibbles
		csum_data >>= 4;
	}

	csum &= 0xf;

	packet = (packet << 4) | csum;

	return packet;
}

void PwmCmdsDshot::EncodePacket(uint16_t packet, TIM_HandleTypeDef* htim, uint32_t channel) {
  uint32_t* buffer = GetDmaBuffer(htim, channel);
  if (!buffer) return;

  for (int i = 0; i < kDshotBits; ++i) {
//	bool bit_set = (packet & (1U << (15 - i))) != 0;
//    buffer[i] = bit_set ? bit1_high_ticks_ : bit0_high_ticks_;
	  buffer[i] = (packet & 0x8000) ? bit1_high_ticks_ : bit0_high_ticks_;
	  		packet <<= 1;
  }

//   Trailing zeros to bring line low
  buffer[kDshotBits] = 0;
//  buffer[kDshotBits + 1] = 0;
//  buffer[kDshotBits + 2] = 0;

//  DEBUG_PRINT("Encoded buffer for TIM1 CH%lu: \n", channel);
//  for (int i = 0; i < kDshotBits; ++i) {
//    DEBUG_PRINT("%lu \n", buffer[i]);
//  }
//  DEBUG_PRINT("\n");
}

void PwmCmdsDshot::SendCmd(TIM_HandleTypeDef* htim,
                      uint32_t           channel,
                      uint16_t           cmd)
{
    /* look-ups ------------------------------------------------------------ */
    DMA_HandleTypeDef* dma =
        htim->hdma[TIM_DMA_ID_CC1 + (channel >> 2)];
    if (!dma) return;                                        /* safety */

    /* skip if stream is still busy --------------------------------------- */
    if (dma->State != HAL_DMA_STATE_READY)
        return;

    uint32_t* buf = GetDmaBuffer(htim, channel);
    if (!buf) return;

    /* build a fresh DShot frame ------------------------------------------ */
    EncodePacket(CreatePacket(cmd), htim, channel);

    /* flush cache (H7) ---------------------------------------------------- */
//    SCB_CleanDCache_by_Addr(buf, kDshotBufferSize * sizeof(uint32_t));

    /* start DMA + PWM in one call ---------------------------------------- */
    HAL_StatusTypeDef stat =
        HAL_TIM_PWM_Start_DMA(htim, channel, buf, kDshotBufferSize);

    if (stat != HAL_OK) {
        /* print both channel and HAL status code */
        DEBUG_PRINT("❌ HAL_TIM_PWM_Start_DMA failed on CH%lu  (status=%d)\n",
                    channel, stat);
    }
}

const char* PwmCmdsDshot::ChannelToStr(uint32_t ch) {
  switch (ch) {
    case TIM_CHANNEL_1: return "CH1";
    case TIM_CHANNEL_2: return "CH2";
    case TIM_CHANNEL_3: return "CH3";
    case TIM_CHANNEL_4: return "CH4";
    default: return "UNKNOWN";
  }
}

uint32_t* PwmCmdsDshot::GetDmaBuffer(TIM_HandleTypeDef* htim, uint32_t channel) {
  if (htim->Instance != TIM1) {
    DEBUG_PRINT("Unsupported TIM instance: 0x%x\n", reinterpret_cast<uintptr_t>(htim->Instance));
    return nullptr;
  }

  switch (channel) {
    case TIM_CHANNEL_1: return dma_buffer1_;
    case TIM_CHANNEL_2: return dma_buffer2_;
    case TIM_CHANNEL_3: return dma_buffer3_;
    case TIM_CHANNEL_4: return dma_buffer4_;
    default:
      DEBUG_PRINT("Invalid TIM1 channel: %lu\n", channel);
      return nullptr;
  }
}


void PwmCmdsDshot::Run() {
	osDelay(5000);
	DEBUG_PRINT("Starting PWM CMDS Module\n");
	ComputeTiming(pwm_timer1_);
//	ArmEsc(pwm_timer1_, TIM_CHANNEL_1);
//	HAL_TIM_PWM_Start(pwm_timer1_, TIM_CHANNEL_1);

	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t loop_frequency = pdMS_TO_TICKS(LOOP_INTERVAL_MS);  // 1000Hz
	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

#ifdef MODE_TEST
	uint16_t min_pwm_val = 24000;
	uint16_t max_pwm_val = 48000;
	uint16_t delta_pwm_val = 2000;
	uint16_t current_pwm_val = min_pwm_val;
	uint8_t current_mtr_idx = 0;

	uint16_t min_dshot_cmd = 500;
	uint16_t max_dshot_cmd = 2047;
	uint16_t delta_dshot_cmd = 0;//100;
	uint16_t current_dshot_cmd = min_dshot_cmd;
	uint16_t heartbeat_counter = 0;
//	uint8_t current_mtr_idx = 0;
#endif
//	TickType_t last_send = xTaskGetTickCount();
	ArmEsc(pwm_timer1_, TIM_CHANNEL_1);
	while(1){
#ifdef MODE_TEST
		//Sequence through each motors
		DEBUG_PRINT("Current Mtr Idx: %d, Current PWM Val: %d\n", current_mtr_idx, current_dshot_cmd);
		if (++heartbeat_counter >= kOneSecIntervalCount) {
				heartbeat_counter = 0;
				current_dshot_cmd += delta_dshot_cmd;
		}
		//MOTOR 1
//		TickType_t now = xTaskGetTickCount();
		if(current_mtr_idx == 0){
//			if(tim1_ch1_dma_cplt_){
//				tim1_ch1_dma_cplt_ = false;
//				last_send = now;
				SendCmd(pwm_timer1_, TIM_CHANNEL_1, current_dshot_cmd);
//			}
			if (current_dshot_cmd > max_dshot_cmd){
				current_dshot_cmd = min_dshot_cmd;
			}
		}

//		//MOTOR 2
//		if(current_mtr_idx == 1){
////			if (pwm_timer1_->hdma[TIM_DMA_ID_CC2]->State == HAL_DMA_STATE_READY){
////				SendCmd(pwm_timer1_, TIM_CHANNEL_2, current_dshot_cmd);
////			}
//			current_dshot_cmd += delta_dshot_cmd;
//			if (current_dshot_cmd > max_dshot_cmd){
//				osDelay(10);
//				current_mtr_idx += 1;
//				current_dshot_cmd = min_dshot_cmd;
//				if (pwm_timer1_->hdma[TIM_DMA_ID_CC2]->State == HAL_DMA_STATE_READY){
////					SendCmd(pwm_timer1_, TIM_CHANNEL_2, current_dshot_cmd);
//				}
//				osDelay(2000);
//				continue;
//			}else{
//				osDelay(500);
//			}
//		}
//
//		//MOTOR 3
//		if(current_mtr_idx == 2){
//			if (pwm_timer1_->hdma[TIM_DMA_ID_CC3]->State == HAL_DMA_STATE_READY){
////				SendCmd(pwm_timer1_, TIM_CHANNEL_3, current_dshot_cmd);
//			}
//			current_dshot_cmd += delta_dshot_cmd;
//			if (current_dshot_cmd > max_dshot_cmd){
//				osDelay(10);
//				current_mtr_idx += 1;
//				current_dshot_cmd = min_dshot_cmd;
//				if (pwm_timer1_->hdma[TIM_DMA_ID_CC3]->State == HAL_DMA_STATE_READY){
////					SendCmd(pwm_timer1_, TIM_CHANNEL_3, current_dshot_cmd);
//				}
//				osDelay(2000);
//				continue;
//			}else{
//				osDelay(1);
//			}
//		}
//
//		//MOTOR 4
//		if(current_mtr_idx == 3){
//			if (pwm_timer1_->hdma[TIM_DMA_ID_CC4]->State == HAL_DMA_STATE_READY){
////				SendCmd(pwm_timer1_, TIM_CHANNEL_4, current_dshot_cmd);
//			}
//			current_dshot_cmd += delta_dshot_cmd;
//			if (current_dshot_cmd > max_dshot_cmd){
//				osDelay(10);
//				current_mtr_idx = 0;
//				current_dshot_cmd = min_dshot_cmd;
//				if (pwm_timer1_->hdma[TIM_DMA_ID_CC4]->State == HAL_DMA_STATE_READY){
////					SendCmd(pwm_timer1_, TIM_CHANNEL_4, current_dshot_cmd);
//				}
//				osDelay(5000);
//				continue;
//			}else{
//				osDelay(1);
//			}
//		}
		vTaskDelayUntil(&xLastWakeTime, loop_frequency);
#elif defined MODE_PASSTHROUGH
		if(rcchannels_sub_.copy(rcchannels_data_)){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, static_cast<uint16_t>(24 *rcchannels_data_.throttle));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, static_cast<uint16_t>(24 *rcchannels_data_.throttle));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, static_cast<uint16_t>(24 *rcchannels_data_.throttle));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, static_cast<uint16_t>(24 *rcchannels_data_.throttle));
		}
		vTaskDelayUntil(&xLastWakeTime, loop_frequency);

#else
		if(pwm_sub_.copy(pwm_data_)){
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_1, static_cast<uint16_t>(24 * pwm_data_.pwm_cmds[0]));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_2, static_cast<uint16_t>(24 * pwm_data_.pwm_cmds[1]));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_3, static_cast<uint16_t>(24 * pwm_data_.pwm_cmds[2]));
			__HAL_TIM_SET_COMPARE(pwm_timer1_, TIM_CHANNEL_4, static_cast<uint16_t>(24 * pwm_data_.pwm_cmds[3]));
		}
		vTaskDelayUntil(&xLastWakeTime, loop_frequency);
#endif
	}
}

void PwmCmdsDshot::OnPwmDmaComplete(TIM_HandleTypeDef* htim) {
  if (htim->Instance == TIM1) {
    if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
      tim1_ch1_dma_cplt_ = true;
      HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
      // Set a flag, notify task, or chain another DMA if needed
    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
    } else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
    }
  }
}

extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if (PwmCmdsDshot::pwm_cmds_instance_handle_ != nullptr) {
    PwmCmdsDshot::pwm_cmds_instance_handle_->OnPwmDmaComplete(htim);
  }
}
