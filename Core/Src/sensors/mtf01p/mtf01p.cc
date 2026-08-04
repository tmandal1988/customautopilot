/*
 * mtf01p.cc
 *
 *  Created on: Jul 26, 2025
 *      Author: tanmay
 */

#include "mtf01p.h"

// Definition of the static member variable
ReadMtf01p* ReadMtf01p::read_mtf01p_instance_handle_ = nullptr;

extern UART_HandleTypeDef huart1;
//extern DMA_HandleTypeDef hdma_uart5_rx;

ReadMtf01p read_mtf01p_task_instance_(&huart1);

ReadMtf01p::ReadMtf01p(UART_HandleTypeDef* huart):
TaskBase("FlowMtf01pRx", 4000, osPriorityNormal),
mtf01_uart_(huart){
	read_mtf01p_instance_handle_ = this;
}

// -----------------------------------------------------------------------------
//  PUBLIC API
// -----------------------------------------------------------------------------
void ReadMtf01p::FlushUartDataRegister() {
  while (__HAL_UART_GET_FLAG(mtf01_uart_, UART_FLAG_RXNE)) {
    volatile uint8_t tmp = static_cast<uint8_t>(mtf01_uart_->Instance->RDR & 0xFF);
    (void)tmp;  // suppress "unused" warning
  }
  __HAL_UART_CLEAR_OREFLAG(mtf01_uart_);
}

bool ReadMtf01p::StartRxDma() {
	uint32_t discarded_events = 0U;
	if (task_handle_ != nullptr) {
		(void)xTaskNotifyWait(0U, kAllTaskEvents, &discarded_events, 0U);
	}

	rx_dma_last_position_.store(0U, std::memory_order_relaxed);
	rx_produced_bytes_.store(0U, std::memory_order_release);
	rx_consumed_bytes_ = 0U;
	last_read_index_ = 0U;
	rx_backlog_pending_ = false;
	ResetParser();

	const HAL_StatusTypeDef result = HAL_UARTEx_ReceiveToIdle_DMA(
			mtf01_uart_, rx_buffer_, MAX_BUFF_SIZE);
	if (result == HAL_OK) {
		rx_restart_pending_ = false;
		return true;
	}

	return false;
}

void ReadMtf01p::RecoverRxDma(TickType_t now) {
	if (!rx_restart_pending_ || !DeadlineReached(now, next_rx_restart_due_)) {
		return;
	}

	(void)HAL_UART_AbortReceive(mtf01_uart_);
	FlushUartDataRegister();

	if (StartRxDma()) {
		last_health_produced_ =
				rx_produced_bytes_.load(std::memory_order_acquire);
		next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
		return;
	}

	next_rx_restart_due_ = now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
}

TickType_t ReadMtf01p::ComputeWaitTicks(TickType_t now) const {
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

void ReadMtf01p::Run() {
	Publisher<Mtf01pData> mtf01p_pub(TopicID::MTF01P);
	osDelay(3000);
	FlushUartDataRegister();
	DEBUG_PRINT("Reading MTF01P Sensor\n");
	task_handle_ = xTaskGetCurrentTaskHandle();

	TickType_t now = xTaskGetTickCount();
	next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
#if MTF01P_DEBUG_PRINT_ENABLE
	next_debug_print_due_ = now;
	DEBUG_PRINT("MTF01P debug enabled: UART instance=0x%08lx, baud=115200\n",
			static_cast<unsigned long>(
					reinterpret_cast<uintptr_t>(mtf01_uart_->Instance)));
#endif
	if (!StartRxDma()) {
		rx_restart_pending_ = true;
		next_rx_restart_due_ = now + pdMS_TO_TICKS(kTransportRetryIntervalMs);
#if MTF01P_DEBUG_PRINT_ENABLE
		DEBUG_PRINT("MTF01P RX DMA start failed: state=%lu error=0x%08lx\n",
				static_cast<unsigned long>(mtf01_uart_->RxState),
				static_cast<unsigned long>(mtf01_uart_->ErrorCode));
#endif
	}

	last_health_produced_ =
			rx_produced_bytes_.load(std::memory_order_acquire);
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
			rx_restart_pending_ = true;
			next_rx_restart_due_ = now;
		}
		RecoverRxDma(now);

		if (((events & kRxReadyEvent) != 0U) || rx_backlog_pending_) {
			rx_backlog_pending_ = ProcessMicrolinkFrame(kParseBudgetBytes);
		}

		if(new_sensor_frame_){
			new_sensor_frame_ =  false;
			mtf01p_data_.time_ms = sensor_payload_.time_ms;
			mtf01p_data_.strength = sensor_payload_.strength;
			mtf01p_data_.distance = sensor_payload_.distance;
			mtf01p_data_.dis_status = sensor_payload_.dis_status;
			mtf01p_data_.precision = sensor_payload_.precision;
			mtf01p_data_.flow_vel_x = sensor_payload_.flow_vel_x;
			mtf01p_data_.flow_vel_y = sensor_payload_.flow_vel_y;
			mtf01p_data_.flow_quality = sensor_payload_.flow_quality;
			mtf01p_data_.flow_status = sensor_payload_.flow_status;

#if MTF01P_DEBUG_PRINT_ENABLE
			DebugPrintData(mtf01p_data_, now);
#endif
			mtf01p_pub.publish(mtf01p_data_);
		}

		if (DeadlineReached(now, next_health_due_)) {
			const uint32_t produced =
					rx_produced_bytes_.load(std::memory_order_acquire);
			if (produced == last_health_produced_) {
				rx_restart_pending_ = true;
				next_rx_restart_due_ = now;
#if MTF01P_DEBUG_PRINT_ENABLE
				DebugPrintNoRxBytes(now);
#endif
			}
			last_health_produced_ = produced;
			next_health_due_ = now + pdMS_TO_TICKS(kLostCommTimeoutMs);
		}

		EndMetricsCycle();
#if RTOS_METRICS_ENABLE && RTOS_CONTEXT_SWITCH_METRICS_ENABLE
		UpdateAuxMetricMaximum(0U,
				rtos_metrics::ContextSwitchCount() -
				cycle_context_switch_start);
#endif
	}

}

bool ReadMtf01p::ProcessMicrolinkFrame(uint16_t budget){
	const uint32_t produced =
			rx_produced_bytes_.load(std::memory_order_acquire);
	const uint32_t available_bytes = produced - rx_consumed_bytes_;

	if (available_bytes >= MAX_BUFF_SIZE) {
		rx_consumed_bytes_ = produced;
		last_read_index_ = rx_dma_last_position_.load(
				std::memory_order_relaxed);
		ResetParser();
		return false;
	}

	if (available_bytes == 0){
		return false;
	}

#if RTOS_METRICS_ENABLE
	UpdateAuxMetricMaximum(1U, available_bytes);
#endif
	const uint32_t bytes_to_process =
			(available_bytes < budget) ? available_bytes : budget;
#if RTOS_METRICS_ENABLE
	UpdateAuxMetricMaximum(2U, bytes_to_process);
#endif
	uint32_t bytes_processed = 0U;
	while (bytes_processed < bytes_to_process) {
		if(ParseChar(rx_buffer_[last_read_index_])){
			if (IsExpectedSensorFrame()) {
				memcpy(&sensor_payload_, msg_.payload, sizeof(sensor_payload_));
				new_sensor_frame_ = true;
			}
		}
		++bytes_processed;
		last_read_index_ = (last_read_index_ + 1U) & (MAX_BUFF_SIZE - 1U);
	}
	rx_consumed_bytes_ += bytes_processed;

	return (produced - rx_consumed_bytes_) != 0U;
}

bool ReadMtf01p::ParseChar(uint8_t data)
{
    switch(msg_.status)
    {
    case 0:
        if(data == MTF_SYNC_CHAR)
        {
           msg_.head = data;
           msg_.payload_cnt = 0;
           msg_.status++;
        }
        break;

    case 1:     // device id
       msg_.dev_id = data;
       msg_.status++;
        break;

    case 2:     // system id
       msg_.sys_id = data;
       msg_.status++;
        break;

    case 3:     // message id
       msg_.msg_id = data;
       msg_.status++;
        break;

    case 4:     //
       msg_.seq = data;
       msg_.status++;
        break;

    case 5:     // payload length
       msg_.len = data;
        if(msg_.len == 0)
           msg_.status += 2;
        else if(msg_.len > MICOLINK_MAX_PAYLOAD_LEN)
           ResetParser();
        else {
           msg_.payload_cnt = 0;
           msg_.status++;
        }
        break;

    case 6:     // payload receive
       msg_.payload[msg_.payload_cnt++] = data;
        if(msg_.payload_cnt ==msg_.len)
        {
           msg_.payload_cnt = 0;
           msg_.status++;
        }
        break;

    case 7:     // check sum
       msg_.checksum = data;
       ResetParser();
       return ComputeCheckSum();

    default:
       ResetParser();
        break;
    }

    return false;
}

bool ReadMtf01p::ComputeCheckSum()
{
    uint8_t checksum = 0;

    checksum += msg_.head;
    checksum += msg_.dev_id;
    checksum += msg_.sys_id;
    checksum += msg_.msg_id;
    checksum += msg_.seq;
    checksum += msg_.len;
    for(uint8_t i=0; i<msg_.len; i++)
    {
        checksum += msg_.payload[i];
    }

    if(checksum == msg_.checksum)
        return true;
    else
        return false;
}

bool ReadMtf01p::IsExpectedSensorFrame() const {
	return (msg_.dev_id == MTF_DEV_ID) &&
	       (msg_.sys_id == MTF_SYS_ID) &&
	       (msg_.msg_id == MTF_MSG_ID) &&
	       (msg_.len == MTF_MSG_SIZE);
}

void ReadMtf01p::ResetParser() {
	msg_.status = 0U;
	msg_.payload_cnt = 0U;
}

void ReadMtf01p::NotifyTaskFromIsr(uint32_t event) {
	if (task_handle_ == nullptr) {
		return;
	}

	BaseType_t higher_priority_task_woken = pdFALSE;
	(void)xTaskNotifyFromISR(task_handle_, event, eSetBits,
			&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

void ReadMtf01p::RxEvent(UART_HandleTypeDef* huart, uint16_t size) {
	ReadMtf01p* instance = read_mtf01p_instance_handle_;
	if ((instance == nullptr) || (huart != instance->mtf01_uart_)) {
		return;
	}

	if ((size == 0U) || (size > MAX_BUFF_SIZE)) {
		instance->NotifyTaskFromIsr(kRxErrorEvent);
		return;
	}

	const uint16_t position = NormalizeDmaPosition(size);
	const uint32_t previous =
			instance->rx_dma_last_position_.load(std::memory_order_relaxed);
	const uint32_t new_bytes =
			(static_cast<uint32_t>(position) + MAX_BUFF_SIZE - previous) &
			(MAX_BUFF_SIZE - 1U);

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

void ReadMtf01p::UartError(UART_HandleTypeDef* huart) {
	ReadMtf01p* instance = read_mtf01p_instance_handle_;
	if ((instance != nullptr) && (huart == instance->mtf01_uart_)) {
		instance->NotifyTaskFromIsr(kRxErrorEvent);
	}
}

bool ReadMtf01p::DeadlineReached(TickType_t now, TickType_t deadline) {
	return static_cast<int32_t>(now - deadline) >= 0;
}

TickType_t ReadMtf01p::TicksUntil(TickType_t now, TickType_t deadline) {
	return DeadlineReached(now, deadline) ? 0U : (deadline - now);
}

uint16_t ReadMtf01p::NormalizeDmaPosition(uint16_t size) {
	return (size == MAX_BUFF_SIZE) ? 0U : size;
}

#if MTF01P_DEBUG_PRINT_ENABLE
void ReadMtf01p::DebugPrintData(const Mtf01pData& data, TickType_t now) {
	if (!DeadlineReached(now, next_debug_print_due_)) {
		return;
	}

	next_debug_print_due_ = now + pdMS_TO_TICKS(kDebugPrintIntervalMs);
	DEBUG_PRINT(
			"MTF01P data: t=%lu dist_mm=%lu strength=%u precision=%u "
			"dist_status=%u flow_x=%d flow_y=%d flow_q=%u flow_status=%u\n",
			static_cast<unsigned long>(data.time_ms),
			static_cast<unsigned long>(data.distance),
			static_cast<unsigned int>(data.strength),
			static_cast<unsigned int>(data.precision),
			static_cast<unsigned int>(data.dis_status),
			static_cast<int>(data.flow_vel_x),
			static_cast<int>(data.flow_vel_y),
			static_cast<unsigned int>(data.flow_quality),
			static_cast<unsigned int>(data.flow_status));
}

void ReadMtf01p::DebugPrintNoRxBytes(TickType_t now) {
	static_cast<void>(now);
	DEBUG_PRINT(
			"MTF01P no RX bytes in %lu ms: UART instance=0x%08lx "
			"rx_state=%lu error=0x%08lx dma_ndtr=%lu\n",
			static_cast<unsigned long>(kLostCommTimeoutMs),
			static_cast<unsigned long>(
					reinterpret_cast<uintptr_t>(mtf01_uart_->Instance)),
			static_cast<unsigned long>(mtf01_uart_->RxState),
			static_cast<unsigned long>(mtf01_uart_->ErrorCode),
			(mtf01_uart_->hdmarx != nullptr)
					? static_cast<unsigned long>(
							__HAL_DMA_GET_COUNTER(mtf01_uart_->hdmarx))
					: 0UL);
}
#endif
