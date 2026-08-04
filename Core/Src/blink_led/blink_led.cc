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

    // Topics are advertised lazily, when each task constructs its Publisher.
    // Report the heap once every task has had time to reach its loop, so the
    // figure reflects the steady-state requirement rather than an early
    // snapshot. Everything here is allocated during start-up and never freed,
    // so this one report is the whole story.
    uint32_t heap_report_countdown = kHeapReportDelaySeconds;

    /* Infinite loop */
    for (;;) {
		BeginMetricsCycle();
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);  // Toggle LED1

		if (heap_report_countdown != 0U) {
			--heap_report_countdown;
			if (heap_report_countdown == 0U) {
				DEBUG_PRINT(
					"FreeRTOS heap: %u free now, %u minimum ever, of %u total; "
					"static stack pool: %u B free\n",
					static_cast<unsigned>(xPortGetFreeHeapSize()),
					static_cast<unsigned>(xPortGetMinimumEverFreeHeapSize()),
					static_cast<unsigned>(configTOTAL_HEAP_SIZE),
					static_cast<unsigned>(StaticStackPoolFreeBytes()));
			}
		}

		EndMetricsCycle();
		osDelay(1000);
    }
}
