/*
 * Compile-time sensor selections used by application modules.
 */

#pragma once

#include <cstdint>

enum class MagnetometerSource : uint8_t {
    kIcm20948,
    kIst8310,
};

// Change only this line to route the ICM20948 magnetometer back into the EKF.
inline constexpr MagnetometerSource kStateEstimatorMagnetometerSource =
    MagnetometerSource::kIst8310;
