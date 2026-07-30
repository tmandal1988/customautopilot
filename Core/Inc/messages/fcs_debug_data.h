/*
 * fcs_debug_data.h
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#pragma once

#include <cstddef>
#include <cstdint>

#define PACKED __attribute__((__packed__))

struct PACKED FcsDebugData {
	float thrust_cmd_N;
	float xmom_cmd_Nm;
	float ymom_cmd_Nm;
	float zmom_cmd_Nm;

	float p_cmd_radps;
	float p_meas_radps;
	float p_kp_out;
	float p_ki_out;

	float q_cmd_radps;
	float q_meas_radps;
	float q_kp_out;
	float q_ki_out;

	float r_cmd_radps;
	float r_meas_radps;
	float r_kp_out;
	float r_ki_out;

	float phi_cmd_rad;
	float phi_meas_rad;
	float phi_kp_out;

	float theta_cmd_rad;
	float theta_meas_rad;
	float theta_kp_out;

	float psi_cmd_rad;
	float psi_meas_rad;
	float psi_kp_out;

	float vn_cmd_mps;
	float vn_meas_mps;
	float vn_kp_out;
	float vn_ki_out;

	float ve_cmd_mps;
	float ve_meas_mps;
	float ve_kp_out;
	float ve_ki_out;

	float vd_cmd_mps;
	float vd_meas_mps;
	float vd_kp_out;
	float vd_ki_out;
	float vd_ff_out;
	float hover_thrust_est;
	float vd_meas_thrust_est;
	float ad_meas_thrust_est;
	float alt_ctrl_trigger;

	float pn_cmd_m;
	float pn_meas_m;
	float pn_kp_out;

	float pe_cmd_m;
	float pe_meas_m;
	float pe_kp_out;

	float pd_cmd_m;
	float pd_meas_m;
	float pd_kp_out;

	float chirp_debug;

	uint8_t chirp_trigger;
	uint8_t chirp_type;
	uint8_t flt_mode;
	uint8_t sm_mode;

	// Cumulative task timing telemetry. These counters are never reset when
	// data is published, so logger decimation cannot hide task activity or
	// late starts between recorded samples.
	uint32_t task_run_seq;
	uint32_t fcs_step_seq;
	uint32_t late_start_count;

	uint64_t timestamp_ms;
};

static_assert(sizeof(FcsDebugData) == 232, "Fcs Debug Data size should be 232 bytes!");
static_assert(offsetof(FcsDebugData, task_run_seq) == 212,
		"Unexpected task_run_seq offset");
static_assert(offsetof(FcsDebugData, fcs_step_seq) == 216,
		"Unexpected fcs_step_seq offset");
static_assert(offsetof(FcsDebugData, late_start_count) == 220,
		"Unexpected late_start_count offset");
static_assert(offsetof(FcsDebugData, timestamp_ms) == 224,
		"Unexpected FCS timestamp offset");
