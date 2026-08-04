/*
 * topic.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

#include <cstdint>
#include "FreeRTOS.h"
#include "semphr.h"

// Milliseconds since boot, used to stamp every published sample.
uint64_t getCurrentTimeMs();

// Topic management structure
template<typename T>
struct Topic {
    T data;
    uint32_t updated_mask = 0; // Bitmask for subscriber updates
    SemaphoreHandle_t mutex;
};
