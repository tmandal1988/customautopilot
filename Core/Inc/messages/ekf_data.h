/*
 * ekf_data.h
 *
 *  Created on: Apr 10, 2025
 *      Author: tanmay
 */

#pragma once

#include <cstddef>
#include <cstdint>

#define PACKED __attribute__((__packed__))

struct PACKED EkfData {
  float euler_rad[3];
  float nedpos_m[3];
  float nedvel_mps[3];
  float bias_corr_body_accels_mps2[3];
  float bias_corr_body_rates_radps[3];
  float dcm_ned_to_body[9];
  float dcm_ned_to_fep[9];
  uint8_t is_mag_valid;
  uint8_t is_baro_valid;
  uint8_t is_gps_valid;
  uint8_t is_lidar_valid;
  float states[23];
  float state_init_pct;

  // Cumulative task timing telemetry. These counters are never reset when
  // data is published, so logger decimation cannot hide task activity or
  // late starts between recorded samples.
  uint32_t task_run_seq;
  uint32_t ekf_step_seq;
  uint32_t late_start_count;

  uint8_t sm_mode;
  uint64_t timestamp_ms;
};

static_assert(sizeof(EkfData) == 253, "EKF Data size should be 253 bytes!");
static_assert(offsetof(EkfData, task_run_seq) == 232,
		"Unexpected task_run_seq offset");
static_assert(offsetof(EkfData, ekf_step_seq) == 236,
		"Unexpected ekf_step_seq offset");
static_assert(offsetof(EkfData, late_start_count) == 240,
		"Unexpected late_start_count offset");
static_assert(offsetof(EkfData, sm_mode) == 244,
		"Unexpected EKF sm_mode offset");
static_assert(offsetof(EkfData, timestamp_ms) == 245,
		"Unexpected EKF timestamp offset");
