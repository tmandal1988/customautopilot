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
#include "sensors/ubloxm9n_rb/ubloxm9n_rb.h"
#include "sensors/mtf01p/mtf01p.h"
#include "mavlink_rxtx/mavlink_rxtx.h"

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart) {
    if ((huart != nullptr) && (huart->Instance == SBUSRX)) {
        return;
    }
//    else if (huart->Instance == GPSUART) {
//        if (ReadUbloxM9n::ubloxm9n_instance_handle_ != nullptr) {
//        	ReadUbloxM9n::ReceivedNewNavPvtFrame();
////        }
//        if (ReadUbloxM9n2::ubloxm9n2_instance_handle_ != nullptr) {
//        	ReadUbloxM9n2::ReceivedNewNavPvtFrame();
//        }
//        if (ReadUbloxM8n::ubloxm8n_instance_handle_ != nullptr) {
//			ReadUbloxM8n::ReceivedNewNavPvtFrame();
//		}
//    }
		else {
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

extern "C" void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart,
                                             uint16_t size) {
    if ((huart != nullptr) && (huart->Instance == TELEM)) {
        MavlinkRxTx::RxEvent(huart, size);
    } else if ((huart != nullptr) && (huart->Instance == SBUSRX)) {
        RcSbus::RxEvent(huart, size);
    } else if ((huart != nullptr) && (huart->Instance == GPSUART)) {
        ReadUbloxM9nRb::RxEvent(huart, size);
    } else if ((huart != nullptr) && (huart->Instance == MTF01PUART)) {
        ReadMtf01p::RxEvent(huart, size);
    }
}

extern "C" void HAL_UART_ErrorCallback(UART_HandleTypeDef* huart) {
    if ((huart != nullptr) && (huart->Instance == TELEM)) {
        MavlinkRxTx::UartError(huart);
    } else if ((huart != nullptr) && (huart->Instance == SBUSRX)) {
        RcSbus::UartError(huart);
    } else if ((huart != nullptr) && (huart->Instance == GPSUART)) {
        ReadUbloxM9nRb::UartError(huart);
    } else if ((huart != nullptr) && (huart->Instance == MTF01PUART)) {
        ReadMtf01p::UartError(huart);
    }
}

extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {
    if ((huart != nullptr) && (huart->Instance == TELEM)) {
        MavlinkRxTx::TxCompleted(huart);
    }
}
