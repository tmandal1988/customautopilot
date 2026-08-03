/*
 * rc_sbus.cc
 *
 *  Created on: Oct 26, 2024
 *      Author: tanmay
 */


#include "rc_sbus.h"

// Definition of the static member variable
RcSbus* RcSbus::rc_sbus_instance_handle_ = nullptr;

extern UART_HandleTypeDef huart7;
RcSbus read_rcinputs_task_instance_(&huart7);

RcSbus::RcSbus(UART_HandleTypeDef* huart):
TaskBase("ReadRcInTask", 864, osPriorityAboveNormal),
rc_uart_(huart){
	rc_sbus_instance_handle_ = this;
}

#if RTOS_METRICS_ENABLE
bool RcSbus::CaptureDiagnostics(Diagnostics* output) {
	if ((output == nullptr) || (rc_sbus_instance_handle_ == nullptr)) {
		return false;
	}

	RcSbus* instance = rc_sbus_instance_handle_;
	output->ready_events = instance->rx_ready_event_count_;
	output->error_events = instance->rx_error_event_count_;
	output->valid_frames = instance->valid_frame_count_;
	output->stale_publishes = instance->stale_publish_count_;
	output->max_bytes_processed = instance->max_bytes_processed_;
	output->max_frames_per_dispatch = instance->max_frames_per_dispatch_;
	output->max_publish_cycles = instance->max_publish_cycles_;
	output->max_context_switch_delta = instance->max_context_switch_delta_;
	output->healthy_frames = instance->healthy_frame_count_;
	output->lost_frames = instance->lost_frame_count_;
	output->failsafe_frames = instance->failsafe_frame_count_;
	output->last_status = instance->last_frame_status_;
	return true;
}
#endif

void RcSbus::ReceivedNewSbusFrame(uint8_t id){
	(void)id;
	if (rc_sbus_instance_handle_ != nullptr) {
		rc_sbus_instance_handle_->NotifyTaskFromIsr(kRxReadyEvent);
	}
}

void RcSbus::Run(){
	Publisher<RcChannels> rcchannels_pub(TopicID::RCCHANNELS);
	task_handle_ = xTaskGetCurrentTaskHandle();
	osDelay(200);
	FlushUartDataRegister();

	TickType_t now = xTaskGetTickCount();
	last_valid_frame_tick_ = now;
	next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
	if (!StartRxDma()) {
		rx_restart_pending_ = true;
		next_rx_restart_due_ = now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
	}

	ConfigureEventMetrics();
	for(;;){
		now = xTaskGetTickCount();
		uint32_t events = 0U;
		(void)xTaskNotifyWait(0U, kAllTaskEvents, &events,
				ComputeWaitTicks(now));
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
		const uint32_t cycle_context_switch_start =
				rtos_metrics::ContextSwitchCount();
#endif
		BeginMetricsCycle();
		now = xTaskGetTickCount();

		if ((events & kRxErrorEvent) != 0U) {
#if RTOS_METRICS_ENABLE
			++rx_error_event_count_;
			SetAuxMetric(1U, rx_error_event_count_);
#endif
			rx_restart_pending_ = true;
			next_rx_restart_due_ = now;
		}

		RecoverRxDma(now);

		if (((events & kRxReadyEvent) != 0U) || rx_backlog_pending_) {
#if RTOS_METRICS_ENABLE
			if ((events & kRxReadyEvent) != 0U) {
				++rx_ready_event_count_;
				SetAuxMetric(0U, rx_ready_event_count_);
			}
#endif
			const bool decoded_new_frame = ProcessSbusBytes(kParseBudgetBytes);
			if (decoded_new_frame) {
				last_valid_frame_tick_ = now;
				next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
#if RTOS_METRICS_ENABLE
				const uint32_t publish_start_cycles =
						rtos_metrics::CyclesNow();
#endif
				rcchannels_pub.publish(rc_channels_);
#if RTOS_METRICS_ENABLE
				const uint32_t publish_cycles =
						rtos_metrics::CyclesNow() - publish_start_cycles;
				if (publish_cycles > max_publish_cycles_) {
					max_publish_cycles_ = publish_cycles;
				}
#endif
			}
		}

		PublishLostIfStale(rcchannels_pub, now);
//		DEBUG_PRINT("RC Roll: %d, RC Pitch: %d, RC Throttle: %d, RC Yaw: %d, RC Sw1: %d, RC Sw2: %d, RC Sw3: %d, RC Knob: %d, RC Sw4: %d\n",
//					rc_channels_.roll, rc_channels_.pitch, rc_channels_.throttle, rc_channels_.yaw, rc_channels_.sw1, rc_channels_.sw2, rc_channels_.sw3,
//					rc_channels_.knob, rc_channels_.sw4);
		EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
		const uint32_t context_switch_delta =
				rtos_metrics::ContextSwitchCount() -
				cycle_context_switch_start;
		if (context_switch_delta > max_context_switch_delta_) {
			max_context_switch_delta_ = context_switch_delta;
		}
		UpdateAuxMetricMaximum(4U, context_switch_delta);
#endif
	}

}

inline uint16_t RcSbus::ScaleToPwmRange(uint16_t channel_in) const{
	// Clamp input to the input range
	if (channel_in < kMinRcVal) channel_in = kMinRcVal;
	if (channel_in > kMaxRcVal) channel_in = kMaxRcVal;

	return static_cast<uint16_t>( (channel_in - kMinRcVal) * (kMaxPwmVal - kMinPwmVal) / (kMaxRcVal - kMinRcVal) + kMinPwmVal );
}

bool RcSbus::StartRxDma(){
	uint32_t discarded_events = 0U;
	if (task_handle_ != nullptr) {
		(void)xTaskNotifyWait(0U, kAllTaskEvents, &discarded_events, 0U);
	}

	rx_dma_last_position_.store(0U, std::memory_order_relaxed);
	rx_produced_bytes_.store(0U, std::memory_order_release);
	rx_consumed_bytes_ = 0U;
	rx_backlog_pending_ = false;

	const HAL_StatusTypeDef result = HAL_UARTEx_ReceiveToIdle_DMA(
			rc_uart_, rc_sbus_ring_buffer_, RING_BUFFER_SIZE);
	if ((result == HAL_OK) && (rc_uart_->hdmarx != nullptr)) {
		__HAL_DMA_DISABLE_IT(rc_uart_->hdmarx, DMA_IT_HT);
		rx_restart_pending_ = false;
		return true;
	}

	return false;
}

void RcSbus::RecoverRxDma(TickType_t now){
	if (!rx_restart_pending_ || !DeadlineReached(now, next_rx_restart_due_)) {
		return;
	}

	(void)HAL_UART_AbortReceive(rc_uart_);
	FlushUartDataRegister();
	if (StartRxDma()) {
		next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
		return;
	}

	next_rx_restart_due_ = now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
}

void RcSbus::FlushUartDataRegister(){
	while (__HAL_UART_GET_FLAG(rc_uart_, UART_FLAG_RXNE)) {
		volatile uint8_t tmp =
				static_cast<uint8_t>(rc_uart_->Instance->RDR & 0xFFU);
		(void)tmp;
	}
	__HAL_UART_CLEAR_OREFLAG(rc_uart_);
}

void RcSbus::NotifyTaskFromIsr(uint32_t event){
	if (task_handle_ == nullptr) {
		return;
	}

	BaseType_t higher_priority_task_woken = pdFALSE;
	(void)xTaskNotifyFromISR(task_handle_, event, eSetBits,
			&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

TickType_t RcSbus::ComputeWaitTicks(TickType_t now) const{
	if (rx_backlog_pending_) {
		return 0U;
	}

	TickType_t wait_ticks = TicksUntil(now, next_health_due_);
	if (rx_restart_pending_) {
		const TickType_t restart_wait =
				TicksUntil(now, next_rx_restart_due_);
		if (restart_wait < wait_ticks) {
			wait_ticks = restart_wait;
		}
	}
	return wait_ticks;
}

bool RcSbus::ProcessSbusBytes(uint16_t budget){
	uint32_t produced =
			rx_produced_bytes_.load(std::memory_order_acquire);
	if ((produced - rx_consumed_bytes_) > RING_BUFFER_SIZE) {
		rx_consumed_bytes_ = produced - RING_BUFFER_SIZE;
	}

	bool decoded_frame = false;
	uint16_t decoded_frames = 0U;
	uint16_t processed_bytes = 0U;
	while ((rx_consumed_bytes_ < produced) &&
			(processed_bytes < budget)) {
		const uint32_t available_bytes = produced - rx_consumed_bytes_;
		if (RingByte(rx_consumed_bytes_) != SBUS_START_BYTE) {
			++rx_consumed_bytes_;
			++processed_bytes;
			continue;
		}

		if (available_bytes < FRAME_SIZE) {
			break;
		}

		if (RingByte(rx_consumed_bytes_ + FRAME_SIZE - 1U) ==
				SBUS_END_BYTE) {
			if (DecodeSbusFrameAt(rx_consumed_bytes_)) {
				decoded_frame = true;
				++decoded_frames;
			}
			rx_consumed_bytes_ += FRAME_SIZE;
			processed_bytes += FRAME_SIZE;
		} else {
			++rx_consumed_bytes_;
			++processed_bytes;
		}
	}

	produced = rx_produced_bytes_.load(std::memory_order_acquire);
	rx_backlog_pending_ =
			((produced - rx_consumed_bytes_) >= FRAME_SIZE);
#if RTOS_METRICS_ENABLE
	if (processed_bytes > max_bytes_processed_) {
		max_bytes_processed_ = processed_bytes;
	}
	if (decoded_frames > max_frames_per_dispatch_) {
		max_frames_per_dispatch_ = decoded_frames;
	}
#endif
	return decoded_frame;
}

bool RcSbus::DecodeSbusFrameAt(uint32_t frame_start){
	uint8_t payload_[FRAME_SIZE - 2U] = {0};
	for(size_t rIdx = 0; rIdx < (FRAME_SIZE - 2U); rIdx++){
		payload_[rIdx] = RingByte(frame_start + 1U + rIdx);
	}

	// 16 channels of 11 bit data
	rc_channels_.roll  		= (uint16_t) ((payload_[0]    |payload_[1] <<8)                     & 0x07FF);
	rc_channels_.pitch  	= (uint16_t) ((payload_[1]>>3 |payload_[2] <<5)                     & 0x07FF);
	rc_channels_.throttle  	= (uint16_t) ((payload_[2]>>6 |payload_[3] <<2 |payload_[4]<<10)    & 0x07FF);
	rc_channels_.yaw  		= (uint16_t) ((payload_[4]>>1 |payload_[5] <<7)                     & 0x07FF);
	rc_channels_.sw1  		= (uint16_t) ((payload_[5]>>4 |payload_[6] <<4)                     & 0x07FF);
	rc_channels_.sw2  		= (uint16_t) ((payload_[6]>>7 |payload_[7] <<1 |payload_[8]<<9)     & 0x07FF);
	rc_channels_.sw3  		= (uint16_t) ((payload_[8]>>2 |payload_[9] <<6)                     & 0x07FF);
	rc_channels_.knob  		= (uint16_t) ((payload_[9]>>5 |payload_[10]<<3)                     & 0x07FF);
	rc_channels_.sw4  		= (uint16_t) ((payload_[11]   |payload_[12]<<8)                     & 0x07FF);

	rc_channels_.roll = ScaleToPwmRange(rc_channels_.roll);
	rc_channels_.pitch = ScaleToPwmRange(rc_channels_.pitch);
	rc_channels_.yaw = ScaleToPwmRange(rc_channels_.yaw);
	rc_channels_.throttle = ScaleToPwmRange(rc_channels_.throttle);
	rc_channels_.sw1 = ScaleToPwmRange(rc_channels_.sw1);
	rc_channels_.sw2 = ScaleToPwmRange(rc_channels_.sw2);
	rc_channels_.sw3 = ScaleToPwmRange(rc_channels_.sw3);
	rc_channels_.knob = ScaleToPwmRange(rc_channels_.knob);
	rc_channels_.sw4 = ScaleToPwmRange(rc_channels_.sw4);

	rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::HEALTHY);
	if (payload_[22] & (1 << 2)) {
		rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::LOST);
	}
	if (payload_[22] & (1 << 3)) {
		rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::FAILSAFE);
	}

	if (++led_toggle_divider_ >= kLedToggleFrameDivider) {
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		led_toggle_divider_ = 0U;
	}
#if RTOS_METRICS_ENABLE
	++valid_frame_count_;
	SetAuxMetric(2U, valid_frame_count_);
	last_frame_status_ = rc_channels_.status;
	if (rc_channels_.status ==
			static_cast<uint8_t>(RcChannels::RxStatus::HEALTHY)) {
		++healthy_frame_count_;
	} else if (rc_channels_.status ==
			static_cast<uint8_t>(RcChannels::RxStatus::LOST)) {
		++lost_frame_count_;
	} else if (rc_channels_.status ==
			static_cast<uint8_t>(RcChannels::RxStatus::FAILSAFE)) {
		++failsafe_frame_count_;
	}
#endif
	return true;
}

void RcSbus::PublishLostIfStale(Publisher<RcChannels>& rcchannels_pub,
		TickType_t now){
	if (!DeadlineReached(now, next_health_due_)) {
		return;
	}

	if (DeadlineReached(now,
			last_valid_frame_tick_ + pdMS_TO_TICKS(kLostCommTimeoutMs))) {
		rc_channels_.status =
				static_cast<uint8_t>(RcChannels::RxStatus::LOST);
#if RTOS_METRICS_ENABLE
		const uint32_t publish_start_cycles = rtos_metrics::CyclesNow();
#endif
		rcchannels_pub.publish(rc_channels_);
#if RTOS_METRICS_ENABLE
		const uint32_t publish_cycles =
				rtos_metrics::CyclesNow() - publish_start_cycles;
		if (publish_cycles > max_publish_cycles_) {
			max_publish_cycles_ = publish_cycles;
		}
		++stale_publish_count_;
		SetAuxMetric(3U, stale_publish_count_);
#endif
	}
	next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
}

uint8_t RcSbus::RingByte(uint32_t absolute_index) const{
	return rc_sbus_ring_buffer_[absolute_index & (RING_BUFFER_SIZE - 1U)];
}

bool RcSbus::DeadlineReached(TickType_t now, TickType_t deadline){
	return static_cast<int32_t>(now - deadline) >= 0;
}

TickType_t RcSbus::TicksUntil(TickType_t now, TickType_t deadline){
	return DeadlineReached(now, deadline) ? 0U : (deadline - now);
}

uint16_t RcSbus::NormalizeDmaPosition(uint16_t size){
	return (size == RING_BUFFER_SIZE) ? 0U : size;
}

void RcSbus::RxEvent(UART_HandleTypeDef* huart, uint16_t size){
	RcSbus* instance = rc_sbus_instance_handle_;
	if ((instance == nullptr) || (huart != instance->rc_uart_)) {
		return;
	}

	if ((size == 0U) || (size > RING_BUFFER_SIZE)) {
		instance->NotifyTaskFromIsr(kRxErrorEvent);
		return;
	}

	const uint16_t position = NormalizeDmaPosition(size);
	const uint16_t previous =
			instance->rx_dma_last_position_.load(std::memory_order_relaxed);
	const uint32_t new_bytes =
			(position >= previous)
					? static_cast<uint32_t>(position - previous)
					: static_cast<uint32_t>(RING_BUFFER_SIZE - previous +
							position);

	instance->rx_dma_last_position_.store(position,
			std::memory_order_relaxed);
	if (new_bytes == 0U) {
		return;
	}

	const uint32_t produced =
			instance->rx_produced_bytes_.load(std::memory_order_relaxed);
	instance->rx_produced_bytes_.store(produced + new_bytes,
			std::memory_order_release);
	instance->NotifyTaskFromIsr(kRxReadyEvent);
}

void RcSbus::UartError(UART_HandleTypeDef* huart){
	RcSbus* instance = rc_sbus_instance_handle_;
	if ((instance != nullptr) && (huart == instance->rc_uart_)) {
		instance->NotifyTaskFromIsr(kRxErrorEvent);
	}
}


//// DMA transfer complete callback
//extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
//    if (huart->Instance == SBUSRX) {
//        if (RcSbus::rc_sbus_instance_handle_ != nullptr) {
//            RcSbus::ReceivedNewSbusFrame(0);  // ID currently unused
//        }
//    } else {
//        __NOP();
//    }
//}
