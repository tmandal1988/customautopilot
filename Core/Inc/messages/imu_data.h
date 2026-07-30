/*
 * imu_data.h
 *
 *  Created on: Mar 30, 2025
 *      Author: tanmay
 */

#ifndef INC_MESSAGES_IMU_DATA_H_
#define INC_MESSAGES_IMU_DATA_H_

#include <cstddef>
#include <cstdint>

// Instruct the compiler not to add any padding between the members of a structure
#define PACKED __attribute__((__packed__))

struct PACKED ImuData{
	float accel_mps2[3];
	float gyro_radps[3];
	float mag_ut[3];
	float temp_degc;
	uint8_t mag_st1;
	uint8_t mag_st2;

	// Minimal cumulative producer telemetry, matching the task/step/late pattern
	// used by the state estimator and flight controls. Comparing task and publish
	// deltas exposes any iteration that failed to produce an IMU sample.
	uint32_t task_run_seq;
	uint32_t publish_seq;
	uint32_t late_start_count;

	uint64_t timestamp_ms;
};

static_assert(sizeof(ImuData) == 62, "IMU data size should be 62 bytes");
static_assert(offsetof(ImuData, task_run_seq) == 42,
			"Unexpected IMU task_run_seq offset");
static_assert(offsetof(ImuData, publish_seq) == 46,
			"Unexpected IMU publish_seq offset");
static_assert(offsetof(ImuData, late_start_count) == 50,
			"Unexpected IMU late_start_count offset");
static_assert(offsetof(ImuData, timestamp_ms) == 54,
			"Unexpected IMU timestamp offset");


#endif /* INC_MESSAGES_IMU_DATA_H_ */
