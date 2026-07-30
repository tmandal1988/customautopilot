/*
 * Dedicated three-axis magnetometer topic payload.
 */

#pragma once

#include <cstddef>
#include <cstdint>

struct __attribute__((__packed__)) MagnetometerData {
    float mag_ut[3];
    uint64_t timestamp_ms;
};

static_assert(offsetof(MagnetometerData, timestamp_ms) == 12U,
              "MagnetometerData layout changed");
static_assert(sizeof(MagnetometerData) == 20U,
              "MagnetometerData must remain compatible with the log decoder");
