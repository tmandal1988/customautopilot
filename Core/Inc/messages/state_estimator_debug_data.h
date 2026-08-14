#pragma once

#include <cstddef>
#include <cstdint>

#define PACKED __attribute__((__packed__))

struct PACKED StateEstimatorFifoDebugData {
  uint8_t status;
  uint16_t count;
};

struct PACKED StateEstimatorDebugData {
  uint8_t is_mag_used;
  uint8_t is_gps_used;
  uint8_t is_baro_used;
  uint8_t is_lidar_used;
  uint8_t is_flow_used;

  float dh_states[23];
  StateEstimatorFifoDebugData status_fifo;
  StateEstimatorFifoDebugData imu_fifo;
  StateEstimatorFifoDebugData mag_fifo;
  StateEstimatorFifoDebugData gps_fifo;
  StateEstimatorFifoDebugData baro_fifo;
  StateEstimatorFifoDebugData lidar_fifo;
  StateEstimatorFifoDebugData flow_fifo;
  uint64_t timestamp_ms;
};

static_assert(sizeof(StateEstimatorDebugData) == 126U,
              "State estimator debug log size changed");
static_assert(offsetof(StateEstimatorDebugData, dh_states) == 5U,
              "Unexpected state estimator debug layout");
static_assert(sizeof(StateEstimatorFifoDebugData) == 3U,
              "FIFO debug log size changed");
