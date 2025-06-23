/*
 * fcs_debug_data.h
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#pragma once
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
	uint64_t timestamp_us;
};

static_assert(sizeof(FcsDebugData) == 208, "Fcs Debug Data size should be 208 bytes!");
