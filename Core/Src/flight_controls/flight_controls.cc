/*
 * flight_controls.cc
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#include "flight_controls.h"

FlightControls::FlightControls():
TaskBase("FlightControlsTask", 2000, osPriorityAboveNormal){
//
}

void FlightControls::Run(){
	osDelay(500);
	DEBUG_PRINT("Starting Flight Controls Module");
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(LOOP_INTERVAL_MS);

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

	// PWM publisher
	Publisher<PwmData> pwm_pub_(TopicID::PWM);
	Publisher<FcsDebugData> fcs_debug_pub_(TopicID::FCSDEBUG);

	fcsModelObj_.initialize();

	// Assign gains to the controller
	fcs_model_autocode_u_.ctrlParams =  AssignFcsCtrlParams();

	pwm_data_.pwm_cmds[0] = 1000;
	pwm_data_.pwm_cmds[1] = 1000;
	pwm_data_.pwm_cmds[2] = 1000;
	pwm_data_.pwm_cmds[3] = 1000;

	uint32_t end_cycles = 0;
	uint32_t start_cycles = 0;
	uint16_t heartbeat_counter = 0;
	for(;;){
		if(ekf_sub_.copy(ekf_data_)){
			if (++heartbeat_counter >= kOneSecIntervalCount) {
				heartbeat_counter = 0;
//				DEBUG_PRINT("FCS State: %d, Flight Mode: %d, FCS Step Executed In: %g\n",fcs_debug_data_.sm_mode,
//						fcs_debug_data_.flt_mode, (float)(end_cycles - start_cycles) / (SystemCoreClock / 1e6));
//				DEBUG_PRINT("Roll: %d, Pitch: %d, Yaw: %d, Throttle: %d, SW1: %d\n",fcs_model_autocode_u_.rcCmdsIn.joystickXCmd_nd,
//						fcs_model_autocode_u_.rcCmdsIn.joystickYCmd_nd, fcs_model_autocode_u_.rcCmdsIn.joystickZCmd_nd,
//						fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd, fcs_model_autocode_u_.rcCmdsIn.rcSwitch1_nd);
//				DEBUG_PRINT("-----------------------------------\n");
			}

			start_cycles = DWT->CYCCNT;
			/****************** Assign Values To The Flight Controller***************************/
			for(size_t idx = 0; idx < 3; idx++){
				fcs_model_autocode_u_.stateEstimate.attitude_rad[idx] = ekf_data_.euler_rad[idx];

				fcs_model_autocode_u_.stateEstimate.bodyAngRates_radps[idx] = ekf_data_.bias_corr_body_rates_radps[idx];
				fcs_model_autocode_u_.stateEstimate.bodyAccels_mps2[idx] = ekf_data_.bias_corr_body_accels_mps2[idx];

				fcs_model_autocode_u_.stateEstimate.nedPos_m[idx] = ekf_data_.nedpos_m[idx];
				fcs_model_autocode_u_.stateEstimate.nedVel_mps[idx] = ekf_data_.nedvel_mps[idx];
			}

			for(size_t dIdx = 0; dIdx < 9; dIdx++){
				fcs_model_autocode_u_.stateEstimate.ned2BodyDcm_nd[dIdx] = ekf_data_.dcm_ned_to_body[dIdx];
				fcs_model_autocode_u_.stateEstimate.ned2FepDcm_nd[dIdx] = ekf_data_.dcm_ned_to_fep[dIdx];
			}
			fcs_model_autocode_u_.stateEstimate.aglEst_m = -ekf_data_.states[6];
			fcs_model_autocode_u_.stateEstimate.climbRateEst_mps = -ekf_data_.states[9];
			if(baro_sub_.copy(baro_data_)){
				fcs_model_autocode_u_.stateEstimate.pressure_mbar = baro_data_.press_pa;
				fcs_model_autocode_u_.stateEstimate.temp_c = baro_data_.temp_degc;
			}

			if(gps_sub_.copy(gps_data_)){
				fcs_model_autocode_u_.stateEstimate.geodeticPos.lat_rad = gps_data_.latitude_rad;
				fcs_model_autocode_u_.stateEstimate.geodeticPos.lon_rad = gps_data_.longitude_rad;
				fcs_model_autocode_u_.stateEstimate.geodeticPos.alt_m = gps_data_.altitude_m;
			}

			if(rcchannels_sub_.copy(rcchannels_data_)){
				fcs_model_autocode_u_.rcCmdsIn.joystickXCmd_nd = rcchannels_data_.roll;
				fcs_model_autocode_u_.rcCmdsIn.joystickYCmd_nd = rcchannels_data_.pitch;
				fcs_model_autocode_u_.rcCmdsIn.joystickZCmd_nd = rcchannels_data_.yaw;
				fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd = rcchannels_data_.throttle;
				fcs_model_autocode_u_.rcCmdsIn.rcSwitch1_nd = rcchannels_data_.sw1;
				fcs_model_autocode_u_.rcCmdsIn.rcSwitch2_nd = rcchannels_data_.sw2;
				fcs_model_autocode_u_.rcCmdsIn.rcSwitch3_nd = rcchannels_data_.sw3;;
			}

			if(mavlink_sub_.copy(mavlink_data_)){
				fcs_model_autocode_u_.externalCmds.armCmdIssued = mavlink_data_.arm_cmd_issued;
			}else{
				fcs_model_autocode_u_.externalCmds.armCmdIssued = 0U;
			}

			if(mavlink_params_sub_.copy(mavlink_params_data_)){
				//Vel Z
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray[2] = mavlink_params_data_.velz_accel_kfb;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ffGainsArray[2] = mavlink_params_data_.velz_kff;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ff2GainsArray[2] = mavlink_params_data_.velz_kff2;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[2].Kp = mavlink_params_data_.velz_kp;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[2].Ki = mavlink_params_data_.velz_ki;
				//Pos Z
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray[2].Kp = mavlink_params_data_.posz_kp;
				//Base Mass
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.baseMass_kg = mavlink_params_data_.base_mass_kg;

				//Vel N
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray[0] = mavlink_params_data_.velne_accel_kfb;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ffGainsArray[0] = mavlink_params_data_.velne_kff;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ff2GainsArray[0] = mavlink_params_data_.velne_kff2;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[0].Kp = mavlink_params_data_.velne_kp;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[0].Ki = mavlink_params_data_.velne_ki;
				//Pos N
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray[0].Kp = mavlink_params_data_.posne_kp;

				//Vel N
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray[1] = mavlink_params_data_.velne_accel_kfb;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ffGainsArray[1] = mavlink_params_data_.velne_kff;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ff2GainsArray[1] = mavlink_params_data_.velne_kff2;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[1].Kp = mavlink_params_data_.velne_kp;
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray[1].Ki = mavlink_params_data_.velne_ki;
				//Pos N
				fcs_model_autocode_u_.ctrlParams.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray[1].Kp = mavlink_params_data_.posne_kp;
			}

			// Run one step of the model
			fcsModelObj_.setExternalInputs(&fcs_model_autocode_u_);
			fcsModelObj_.step();

			// Get controller outputs
			fcs_model_autocode_y_ = fcsModelObj_.getExternalOutputs();

			if(fcs_model_autocode_y_.fcsDebug.state != enumStateMachine::INACTIVE){
				if (fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd <= kMinPwmCheckThreshold){
					pwm_data_.pwm_cmds[0] = fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
					pwm_data_.pwm_cmds[1] = fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
					pwm_data_.pwm_cmds[2] = fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
					pwm_data_.pwm_cmds[3] = fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
				}else{
//					DEBUG_PRINT("Flying\n");
					pwm_data_.pwm_cmds[0] = fcs_model_autocode_y_.actuatorsPwmCmds[0];
					pwm_data_.pwm_cmds[1] = fcs_model_autocode_y_.actuatorsPwmCmds[1];
					pwm_data_.pwm_cmds[2] = fcs_model_autocode_y_.actuatorsPwmCmds[2];
					pwm_data_.pwm_cmds[3] = fcs_model_autocode_y_.actuatorsPwmCmds[3];
				}
			}else{
				pwm_data_.pwm_cmds[0] = kMinPwmThreshold;
				pwm_data_.pwm_cmds[1] = kMinPwmThreshold;
				pwm_data_.pwm_cmds[2] = kMinPwmThreshold;
				pwm_data_.pwm_cmds[3] = kMinPwmThreshold;
			}
			pwm_pub_.publish(pwm_data_);

			fcs_debug_data_.sm_mode = static_cast<uint8_t>(fcs_model_autocode_y_.fcsDebug.state);
			fcs_debug_data_.flt_mode = static_cast<uint8_t>(fcs_model_autocode_y_.fcsDebug.flightMode);

			fcs_debug_data_.p_cmd_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[0];
			fcs_debug_data_.p_meas_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[0];
			fcs_debug_data_.p_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[0].proportionalOutput;
			fcs_debug_data_.p_ki_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[0].integralOutput;

			fcs_debug_data_.q_cmd_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[1];
			fcs_debug_data_.q_meas_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[1];
			fcs_debug_data_.q_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[1].proportionalOutput;
			fcs_debug_data_.q_ki_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[1].integralOutput;

			fcs_debug_data_.r_cmd_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[2];
			fcs_debug_data_.r_meas_radps = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[2];
			fcs_debug_data_.r_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[2].proportionalOutput;
			fcs_debug_data_.r_ki_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[2].integralOutput;

			fcs_debug_data_.phi_cmd_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[0];
			fcs_debug_data_.phi_meas_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[0];
			fcs_debug_data_.phi_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[0].proportionalOutput;

			fcs_debug_data_.theta_cmd_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[1];
			fcs_debug_data_.theta_meas_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[1];
			fcs_debug_data_.theta_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[1].proportionalOutput;

			fcs_debug_data_.psi_cmd_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[2];
			fcs_debug_data_.psi_meas_rad = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[2];
			fcs_debug_data_.psi_kp_out = fcs_model_autocode_y_.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[2].proportionalOutput;

			fcs_debug_data_.vn_cmd_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.cmd[0];
			fcs_debug_data_.vn_meas_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.meas[0];
			fcs_debug_data_.vn_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[0].proportionalOutput;
			fcs_debug_data_.vn_ki_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[0].integralOutput;

			fcs_debug_data_.ve_cmd_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.cmd[1];
			fcs_debug_data_.ve_meas_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.meas[1];
			fcs_debug_data_.ve_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[1].proportionalOutput;
			fcs_debug_data_.ve_ki_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[1].integralOutput;

			fcs_debug_data_.vd_cmd_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.cmd[2];
			fcs_debug_data_.vd_meas_mps = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.meas[2];
			fcs_debug_data_.vd_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[2].proportionalOutput;
			fcs_debug_data_.vd_ki_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[2].integralOutput;
			// Vz control feedforward
			fcs_debug_data_.vd_ff_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.velCtrlDebug.velCtrlFf[2];
			// Estimated hover thrust
			fcs_debug_data_.hover_thrust_est = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.zAccelCtrlDebug.pidDebug.output;
			fcs_debug_data_.vd_meas_thrust_est = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.zAccelCtrlDebug.cmd;
			fcs_debug_data_.ad_meas_thrust_est = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.zAccelCtrlDebug.meas;
			fcs_debug_data_.alt_ctrl_trigger = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.zAccelCtrlDebug.pidDebug.proportionalOutput;

			fcs_debug_data_.pn_cmd_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.cmd[0];
			fcs_debug_data_.pn_meas_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.meas[0];
			fcs_debug_data_.pn_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[0].proportionalOutput;

			fcs_debug_data_.pe_cmd_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.cmd[1];
			fcs_debug_data_.pe_meas_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.meas[1];
			fcs_debug_data_.pe_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[1].proportionalOutput;

			fcs_debug_data_.pd_cmd_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.cmd[2];
			fcs_debug_data_.pd_meas_m = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.meas[2];
			fcs_debug_data_.pd_kp_out = fcs_model_autocode_y_.fcsDebug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[2].proportionalOutput;

			fcs_debug_data_.thrust_cmd_N = fcs_model_autocode_y_.fcsDebug.allocDebug.thrustCmd_N;
			fcs_debug_data_.xmom_cmd_Nm = fcs_model_autocode_y_.fcsDebug.allocDebug.xMomCmd_Nm;
			fcs_debug_data_.ymom_cmd_Nm = fcs_model_autocode_y_.fcsDebug.allocDebug.yMomCmd_Nm;
			fcs_debug_data_.zmom_cmd_Nm = fcs_model_autocode_y_.fcsDebug.allocDebug.zMomCmd_Nm;

			fcs_debug_data_.chirp_debug = fcs_model_autocode_y_.fcsDebug.sysIdDebug.chirpSignal;
			fcs_debug_data_.chirp_trigger = static_cast<uint8_t>(fcs_model_autocode_y_.fcsDebug.sysIdDebug.chirpTrigger);
			fcs_debug_data_.chirp_type = static_cast<uint8_t>(fcs_model_autocode_y_.fcsDebug.sysIdDebug.chirpType);

			fcs_debug_pub_.publish(fcs_debug_data_);
			end_cycles = DWT->CYCCNT;
		}

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

FlightControls flight_controls_task_instance_;


