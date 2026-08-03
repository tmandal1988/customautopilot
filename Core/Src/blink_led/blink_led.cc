/*
 * blink_led.cc
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#include "blink_led.h"

void BlinkLedTask::Run() {
	DEBUG_PRINT("Hello from FreeRTOS via USB CDC!\r\n");
    ConfigureEventMetrics();
    /* Infinite loop */
    for (;;) {
		BeginMetricsCycle();
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);  // Toggle LED1
		EndMetricsCycle();
		osDelay(1000);
    }
}
