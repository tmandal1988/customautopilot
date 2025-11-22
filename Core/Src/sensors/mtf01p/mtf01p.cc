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
TaskBase("ReadMtf01pTask", 4000, osPriorityAboveNormal),
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

void ReadMtf01p::Run() {
	Publisher<Mtf01pData> mtf01p_pub(TopicID::MTF01P);
	osDelay(3000);
	FlushUartDataRegister();
	DEBUG_PRINT("Reading MTF01P Sensor\n");
	HAL_StatusTypeDef result = HAL_UART_Receive_DMA(mtf01_uart_, rx_buffer_, MAX_BUFF_SIZE);
	if (result != HAL_OK) {
	    DEBUG_PRINT("UART DMA start failed with code: %d\n", result);
	}
	TickType_t xLastWakeTime = xTaskGetTickCount();
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);
	int hb_counter = 0;
	osDelay(500);
	for(;;){
//		HAL_UART_Receive(mtf01_uart_, rx_buffer_, 1, 10);
//		DEBUG_PRINT("Byte Received: %02X\n", rx_buffer_[0]);
		ProcessMicrolinkFrame();
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

			mtf01p_pub.publish(mtf01p_data_);
		}

		if(restart_comm_){
			restart_comm_ = false;
			HAL_UART_DMAStop(mtf01_uart_);
			FlushUartDataRegister();
			HAL_StatusTypeDef result = HAL_UART_Receive_DMA(mtf01_uart_, rx_buffer_, MAX_BUFF_SIZE);
			if (result != HAL_OK) {
				DEBUG_PRINT("UART DMA re-start failed with code: %d\n", result);
			}
			DEBUG_PRINT("Restarted MTP01 Comm\n");
			osDelay(100);
		}

		if (++hb_counter >= kHeartbeatIntervalCount) {
			hb_counter = 0;
//			DEBUG_PRINT("Time[ms]: %ld, Dist[mm]: %ld, Flow Vel X: %d, Flow Vel  Y: %d\n", sensor_payload_.time_ms, sensor_payload_.distance,
//								sensor_payload_.flow_vel_x, sensor_payload_.flow_vel_y);
//			DEBUG_PRINT("Dist Status: %d, Flow Status: %d, Flow Quality: %d\n",sensor_payload_.dis_status, sensor_payload_.flow_status, sensor_payload_.flow_quality);
//			DEBUG_PRINT("Strength: %d, Distance Precision: %d\n", sensor_payload_.strength, sensor_payload_.precision);
//			DEBUG_PRINT("################################################\n");
		}
		// Wait until the next cycle
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

}

void ReadMtf01p::ProcessMicrolinkFrame(){
	size_t current_write_index = MAX_BUFF_SIZE - __HAL_DMA_GET_COUNTER(mtf01_uart_->hdmarx);
	size_t available_bytes = 0;
	if (current_write_index >= last_read_index_) {
		available_bytes = current_write_index - last_read_index_;
	} else {
		available_bytes = MAX_BUFF_SIZE - last_read_index_ + current_write_index;
	}

	if (available_bytes == 0){
		lost_comm_count_++;
		if(lost_comm_count_ >= kLostCommIntervalCount){
			restart_comm_ = true;
			lost_comm_count_ = 0;
		}

	}else{
		lost_comm_count_ = 0;
	}
	size_t bytes_processed = 0;
	while (bytes_processed < available_bytes) {
		size_t buffer_index = (last_read_index_ + bytes_processed) % MAX_BUFF_SIZE;
		if(ParseChar(rx_buffer_[buffer_index])){
			memcpy(&sensor_payload_, msg_.payload, msg_.len);
			new_sensor_frame_ = true;
		}
		++bytes_processed;
	}
	last_read_index_ = (last_read_index_ + bytes_processed) % MAX_BUFF_SIZE;
}

bool ReadMtf01p::ParseChar(uint8_t data)
{
    switch(msg_.status)
    {
    case 0:
        if(data == MTF_SYNC_CHAR)
        {
           msg_.head = data;
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
           msg_.status = 0;
        else
           msg_.status++;
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
       msg_.status = 0;
        if(ComputeCheckSum())
        {
            return true;
        }

    default:
       msg_.status = 0;
       msg_.payload_cnt = 0;
        break;
    }

    return false;
}

bool ReadMtf01p::ComputeCheckSum()
{
    uint8_t length = msg_.len + 6;
    uint8_t temp[MICOLINK_MAX_LEN];
    uint8_t checksum = 0;

    memcpy(temp, &msg_, length);

    for(uint8_t i=0; i<length; i++)
    {
        checksum += temp[i];
    }

    if(checksum == msg_.checksum)
        return true;
    else
        return false;
}

