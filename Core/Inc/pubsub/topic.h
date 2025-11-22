/*
 * topic.h
 *
 *  Created on: Mar 29, 2025
 *      Author: tanmay
 */

#pragma once

#include <cstdint>
#include <chrono>
#include "FreeRTOS.h"
#include "semphr.h"

// Get current time in microseconds
uint64_t getCurrentTimeMs();

// Topic management structure
template<typename T>
struct Topic {
    T data;
    uint32_t updated_mask = 0; // Bitmask for subscriber updates
    SemaphoreHandle_t mutex;
};

//{
//    auto now = std::chrono::steady_clock::now();
//    return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
//    return 1;
//}
