/*
 * pin_defines.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#ifndef INC_PIN_DEFINES_H_
#define INC_PIN_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

#define ICM20948_SDA_Pin GPIO_PIN_0
#define ICM20948_SDA_GPIO_Port GPIOF
#define ICM20948_SCL_Pin GPIO_PIN_1
#define ICM20948_SCL_GPIO_Port GPIOF
#define ICM20948_INT_Pin GPIO_PIN_2
#define ICM20948_INT_GPIO_Port GPIOF
#define ICM20948I2C I2C2

#define BARO_SCK_Pin GPIO_PIN_7
#define BARO_SCK_GPIO_Port GPIOF
#define BARO_MISO_Pin GPIO_PIN_8
#define BARO_MISO_GPIO_Port GPIOF
#define BARO_MOSI_Pin GPIO_PIN_9
#define BARO_MOSI_GPIO_Port GPIOF
#define BARO_CS_Pin GPIO_PIN_10
#define BARO_CS_GPIO_Port GPIOF

#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOD
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOD
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOD

#define uSD_DETECT_Pin GPIO_PIN_2
#define uSD_DETECT_GPIO_Port GPIOG
#define uSD_D0_Pin GPIO_PIN_8
#define uSD_D0_GPIO_Port GPIOC
#define uSD_D1_Pin GPIO_PIN_9
#define uSD_D1_GPIO_Port GPIOC
#define uSD_D2_Pin GPIO_PIN_10
#define uSD_D2_GPIO_Port GPIOC
#define uSD_D3_Pin GPIO_PIN_11
#define uSD_D3_GPIO_Port GPIOC
#define uSD_CK_Pin GPIO_PIN_12
#define uSD_CK_GPIO_Port GPIOC
#define uSD_CMD_Pin GPIO_PIN_2
#define uSD_CMD_GPIO_Port GPIOD

#define GPSUART USART2
#define SBUSRX UART7
#define TELEM UART4
#define LIOF USART5

//#define LD1_Pin GPIO_PIN_0
//#define LD1_GPIO_Port GPIOB
//
//#define ICM20948_INT_Pin GPIO_PIN_2
//#define ICM20948_INT_GPIO_Port GPIOF
//#define ICM20948_INT_EXTI_IRQn EXTI2_IRQn
//
//#define BLUE_PB_Pin GPIO_PIN_13
//#define BLUE_PB_GPIO_Port GPIOC
//#define BLUE_PB_EXTI_IRQn EXTI15_10_IRQn
//
//#define LD3_Pin GPIO_PIN_14
//#define LD3_GPIO_Port GPIOB
//
//#define STLINK_RX_Pin GPIO_PIN_8
//#define STLINK_RX_GPIO_Port GPIOD
//
//#define STLINK_TX_Pin GPIO_PIN_9
//#define STLINK_TX_GPIO_Port GPIOD
//
//#define USB_OTG_FS_PWR_EN_Pin GPIO_PIN_10
//#define USB_OTG_FS_PWR_EN_GPIO_Port GPIOD
//
//#define USB_OTG_FS_OVCR_Pin GPIO_PIN_7
//#define USB_OTG_FS_OVCR_GPIO_Port GPIOG
//
//#define LD2_Pin GPIO_PIN_1
//#define LD2_GPIO_Port GPIOE
//
//#define GPSUART USART2
//#define GPSI2C I2C1
//
//#define uSD_Detect_Pin GPIO_PIN_2
//#define uSD_Detect_GPIO_Port GPIOG
//
//#define IMUI2C I2C2
//
//#define IOUART USART3
//
//#define RCUART UART7
//
//// ID of the ICM20948 on i2c2 port
//#define ICM20948_ID0 0
//
//// ID of the UBLOXM9N on uart2 and i2c1 port
//#define UBLOXM9N_ID0 0
//
//// ID of the RC SBUS on uart7
//#define RCSBUS_ID0 0

#ifdef __cplusplus
}
#endif

#endif /* INC_PIN_DEFINES_H_ */
