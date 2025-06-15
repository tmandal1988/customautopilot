/*
 * pwm_data.h
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#pragma once
#define PACKED __attribute__((__packed__))

struct PACKED PwmData {
  uint16_t pwm_cmds[4];
  uint64_t timestamp_us;
};

static_assert(sizeof(PwmData) == 16, "PWM Data size should be 16 bytes!");



