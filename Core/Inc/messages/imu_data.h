/*
 * imu_data.h
 *
 *  Created on: Mar 30, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_IMU_DATA_H_
#define INC_MESSAGES_IMU_DATA_H_

// Instruct the compiler not to add any padding between the members of a structure
#define PACKED __attribute__((__packed__))

struct PACKED ImuData{
	float accel_mps2[3];
	float gyro_radps[3];
	float mag_ut[3];
	float temp_degc;
	uint8_t mag_st1;
	uint8_t mag_st2;
	uint64_t timestamp_us;
};


#endif /* INC_MESSAGES_IMU_DATA_H_ */
