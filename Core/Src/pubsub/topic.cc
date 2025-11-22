/*
 * topic.cc
 *
 *  Created on: Apr 1, 2025
 *      Author: tanmay
 */

#include "pubsub/topic.h"

uint64_t getCurrentTimeMs() {
    TickType_t ticks = xTaskGetTickCount();
    return static_cast<uint64_t>(ticks) * 1000UL / configTICK_RATE_HZ;
}


