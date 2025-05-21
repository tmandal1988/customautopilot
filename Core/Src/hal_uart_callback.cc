/*
 * hal_uart_callback.cc
 *
 *  Created on: Apr 8, 2025
 *      Author: tanmay
 */

#include "rc_sbus/rc_sbus.h"
//#include "sensors/ubloxm9n/ubloxm9n.h"
#include "sensors/ubloxm9n2/ubloxm9n2.h"
//#include "sensors/ubloxm8n/ubloxm8n.h"
#include "mavlink_rxtx/mavlink_rxtx.h"

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if (huart->Instance == SBUSRX) {
        if (RcSbus::rc_sbus_instance_handle_ != nullptr) {
            RcSbus::ReceivedNewSbusFrame(0);
        }
    } else if (huart->Instance == GPSUART) {
//        if (ReadUbloxM9n::ubloxm9n_instance_handle_ != nullptr) {
//        	ReadUbloxM9n::ReceivedNewNavPvtFrame();
//        }
        if (ReadUbloxM9n2::ubloxm9n2_instance_handle_ != nullptr) {
        	ReadUbloxM9n2::ReceivedNewNavPvtFrame();
        }
//        if (ReadUbloxM8n::ubloxm8n_instance_handle_ != nullptr) {
//			ReadUbloxM8n::ReceivedNewNavPvtFrame();
//		}
    } else {
        __NOP();
    }
//	if (huart->Instance == GPSUART) {
////        if (ReadUbloxM9n::ubloxm9n_instance_handle_ != nullptr) {
////        	ReadUbloxM9n::ReceivedNewNavPvtFrame();
////        }
////        if (ReadUbloxM9n2::ubloxm9n2_instance_handle_ != nullptr) {
////        	ReadUbloxM9n2::ReceivedNewNavPvtFrame();
////        }
//        if (ReadUbloxM8n::ubloxm8n_instance_handle_ != nullptr) {
//			ReadUbloxM8n::ReceivedNewNavPvtFrame();
//		}
//    } else {
//        __NOP();
//    }
}

//extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
//    if (huart->Instance == TELEM) {
//        if (MavlinkRxTx::mavlink_rxtx_instance_handle_ != nullptr) {
//        	MavlinkRxTx::TxCompleted();
//        }
//    }else {
//        __NOP();
//    }
//}
