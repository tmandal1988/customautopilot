/*
 * rc_channels.h
 *
 *  Created on: Apr 6, 2025
 *      Author: tanmay
 */

#pragma once
#define PACKED __attribute__((__packed__))

struct PACKED RcChannels {
  uint16_t roll;
  uint16_t pitch;
  uint16_t throttle;
  uint16_t yaw;
  uint16_t sw1;
  uint16_t sw2;
  uint16_t sw3;
  uint16_t knob;
  uint16_t sw4;
  uint8_t status; // store enum as raw uint8_t
  uint64_t timestamp_us;

  enum class RxStatus : uint8_t {
    INACTIVE,
    HEALTHY,
    LOST,
    FAILSAFE
  };

  RcChannels()
      : roll(992), pitch(992), throttle(172), yaw(992),
        sw1(172), sw2(172), sw3(172), knob(172), sw4(172),
        status(static_cast<uint8_t>(RxStatus::INACTIVE)),
        timestamp_us(0) {}
};

static_assert(sizeof(RcChannels) == 27, "RcChannels size should be 27 bytes!");

