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

void RcSbus::RcSbusInit(){
//	FlushUartDataRegister(rc_uart_);
	HAL_UART_Receive_DMA(rc_uart_, rc_sbus_ring_buffer_, RING_BUFFER_SIZE);
	HAL_Delay(100);
}

void RcSbus::ReceivedNewSbusFrame(uint8_t id){
	if (rc_sbus_instance_handle_ != nullptr){
		rc_sbus_instance_handle_->RestartDma();
	}
}

void RcSbus::Run(){
	Publisher<RcChannels> rcchannels_pub(TopicID::RCCHANNELS);
	RcSbusInit();
//	int blink_counter = 0;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);

	osDelay(500);
	// Initialize the periodic schedule after the startup delay.
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
//		if (++blink_counter >= 20) {
//			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 864 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//			used, highWaterMark * sizeof(StackType_t), 864);
//		}
		if(read_pos_ != write_pos_){
			DecodeSbusFrame();
		}
		rcchannels_pub.publish(rc_channels_);
//		DEBUG_PRINT("RC Roll: %d, RC Pitch: %d, RC Throttle: %d, RC Yaw: %d, RC Sw1: %d, RC Sw2: %d, RC Sw3: %d, RC Knob: %d, RC Sw4: %d\n",
//					rc_channels_.roll, rc_channels_.pitch, rc_channels_.throttle, rc_channels_.yaw, rc_channels_.sw1, rc_channels_.sw2, rc_channels_.sw3,
//					rc_channels_.knob, rc_channels_.sw4);
		// Wait until the next cycle
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

}

inline uint16_t RcSbus::ScaleToPwmRange(uint16_t channel_in) const{
	// Clamp input to the input range
	if (channel_in < kMinRcVal) channel_in = kMinRcVal;
	if (channel_in > kMaxRcVal) channel_in = kMaxRcVal;

	return static_cast<uint16_t>( (channel_in - kMinRcVal) * (kMaxPwmVal - kMinPwmVal) / (kMaxRcVal - kMinRcVal) + kMinPwmVal );
}

void RcSbus::DecodeSbusFrame(){
	// Process data from the ring buffer to find valid frames
	while (read_pos_ != write_pos_) {
		if (rc_sbus_ring_buffer_[read_pos_] == SBUS_START_BYTE) {

			// Ensure there are enough bytes for a frame
			uint16_t bytes_available;
			if (write_pos_ >= read_pos_) {
				bytes_available = write_pos_ - read_pos_;
			} else {
				bytes_available = (RING_BUFFER_SIZE - read_pos_) + write_pos_;
			}

			if (bytes_available >= FRAME_SIZE) {
				// Check if the frame ends correctly with 0x00
				uint16_t frame_end = (read_pos_ + FRAME_SIZE - 1) % RING_BUFFER_SIZE;
				if (rc_sbus_ring_buffer_[frame_end] == SBUS_END_BYTE) {
					// Valid frame found, process it
					uint8_t payload_[23] = {0};
					for(size_t rIdx = 0; rIdx < (FRAME_SIZE - 2); rIdx++){
						payload_[rIdx] = rc_sbus_ring_buffer_[(read_pos_ + 1 + rIdx) % RING_BUFFER_SIZE];
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
					//					rc_channels_[9]  = (uint16_t) ((payload_[12]>>3|payload_[13]<<5)                     & 0x07FF);
					//					rc_channels_[10] = (uint16_t) ((payload_[13]>>6|payload_[14]<<2 |payload_[15]<<10)   & 0x07FF);
					//					rc_channels_[11] = (uint16_t) ((payload_[15]>>1|payload_[16]<<7)                     & 0x07FF);
					//					rc_channels_[12] = (uint16_t) ((payload_[16]>>4|payload_[17]<<4)                     & 0x07FF);
					//					rc_channels_[13] = (uint16_t) ((payload_[17]>>7|payload_[18]<<1 |payload_[19]<<9)    & 0x07FF);
					//					rc_channels_[14] = (uint16_t) ((payload_[19]>>2|payload_[20]<<6)                     & 0x07FF);
					//					rc_channels_[15] = (uint16_t) ((payload_[20]>>5|payload_[21]<<3)                     & 0x07FF);
					//
					//					if (payload_[22] & (1 << 0)) {
					//						Channel No [16] = 1;
					//					} else {
					//						Channel No [16] = 0;
					//					}
					//
					//					if (payload_[22]  & (1 << 1)) {
					//						channel No [17] = 1;
					//					} else {
					//						Channel No [17] = 0;
					//					}

					// Failsafe
					rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::HEALTHY);
					if (payload_[22] & (1 << 2)) {
						rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::LOST);
					}

					if (payload_[22] & (1 << 3)) {
						rc_channels_.status = static_cast<uint8_t>(RcChannels::RxStatus::FAILSAFE);
					}
//					HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);  // Toggle LED3
					// Move the read position past the processed frame
					read_pos_ = (read_pos_ + FRAME_SIZE) % RING_BUFFER_SIZE;
				} else {
					// If invalid frame, just skip the current byte
					read_pos_ = (read_pos_ + 1) % RING_BUFFER_SIZE;
				}
			}else {
				  // Not enough data to process a complete frame yet
				  break;
			  }
		} else {
			// If no valid start byte, skip current byte
			read_pos_ = (read_pos_ + 1) % RING_BUFFER_SIZE;
		}
	}
}

void RcSbus::RestartDma(){
	// Increment the write position by the number of received bytes
	write_pos_ = (write_pos_ + DMA_RX_SIZE) % RING_BUFFER_SIZE;
	// Restart DMA reception for the next 50 bytes from the current position
	HAL_UART_Receive_DMA(rc_uart_, rc_sbus_ring_buffer_ + write_pos_, DMA_RX_SIZE);
	HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);  // Toggle LED3
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
