/*
 * ekf_data.h
 *
 *  Created on: Apr 10, 2025
 *      Author: tanmay
 */

#pragma once
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
  uint8_t sm_mode;
  uint64_t timestamp_us;
};

static_assert(sizeof(EkfData) == 241, "EKF Data size should be 241 bytes!");
