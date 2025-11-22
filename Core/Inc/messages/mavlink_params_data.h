/*
 * mavalink_param_data.h
 *
 *  Created on: Jun 22, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_MAVLINK_PARAMS_DATA_H_
#define INC_MESSAGES_MAVLINK_PARAMS_DATA_H_

struct PACKED MavlinkParamsData {
  float velz_kp;
  float velz_ki;
  float velz_kff;
  float velz_kff2;
  float velz_accel_kfb;
  float posz_kp;
  float velne_kp;
  float velne_ki;
  float velne_kff;
  float velne_kff2;
  float velne_accel_kfb;
  float posne_kp;
  float base_mass_kg;
  uint64_t timestamp_ms;
};

static_assert(sizeof(MavlinkParamsData) == 60, "Mavlink Params size should be 60 bytes!");



#endif /* INC_MESSAGES_MAVALINK_PARAM_DATA_H_ */
