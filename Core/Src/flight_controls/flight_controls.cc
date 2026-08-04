/*
 * flight_controls.cc
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#include "flight_controls.h"

#include "parameters/parameter_store.h"
#include "pubsub/pubsub_manager.h"

FlightControls::FlightControls(bool register_task)
    : TaskBase("FcsInner250Hz", 2000, osPriorityAboveNormal,
               register_task),
      fcsModelObj_(&fcs_model_autocode_u_,
                   parameters::ParameterStore::Instance()
                       .FcsModelParameters()) {}

bool FlightControls::InitializeController() {
  auto& parameter_store = parameters::ParameterStore::Instance();
  if (!parameter_store.IsReady()) {
    ERROR_PRINT("Flight-control parameter store was not initialized\n");
    return false;
  }

  DEBUG_PRINT("Starting Flight Controls Module");
  pubSubManager().advertise<PwmData>(TopicID::PWM);
  pubSubManager().advertise<FcsDebugData>(TopicID::FCSDEBUG);
  fcsModelObj_.initialize();
  pwm_data_.pwm_cmds[0] = 1000;
  pwm_data_.pwm_cmds[1] = 1000;
  pwm_data_.pwm_cmds[2] = 1000;
  pwm_data_.pwm_cmds[3] = 1000;
  heartbeat_counter_ = 0;
  first_iteration_ = true;
  fcs_debug_data_ = {};
  return true;
}

bool FlightControls::StepOnce(const EkfData* direct_ekf,
                              PwmData* direct_pwm_output,
                              TickType_t scheduled_start_tick) {
  auto& parameter_store = parameters::ParameterStore::Instance();
  const TickType_t reference_tick =
      (scheduled_start_tick != 0U) ? scheduled_start_tick
                                   : scheduled_start_tick_;
  const TickType_t actual_start_tick = xTaskGetTickCount();
  const int32_t start_lateness_ticks =
      static_cast<int32_t>(actual_start_tick - reference_tick);

  ++fcs_debug_data_.task_run_seq;
  if (!first_iteration_ &&
      start_lateness_ticks > static_cast<int32_t>(kAllowedStartLatenessTicks)) {
    ++fcs_debug_data_.late_start_count;
  }
  first_iteration_ = false;

  const bool has_ekf =
      (direct_ekf != nullptr) ? ((ekf_data_ = *direct_ekf), true)
                              : ekf_sub_.copy(ekf_data_);
  if (!has_ekf) {
    parameter_store.ApplyOneFcsUpdate();
    return false;
  }

  if (++heartbeat_counter_ >= kOneSecIntervalCount) {
    heartbeat_counter_ = 0;
  }

  for (size_t idx = 0; idx < 3; idx++) {
    fcs_model_autocode_u_.stateEstimate.attitude_rad[idx] =
        ekf_data_.euler_rad[idx];
    fcs_model_autocode_u_.stateEstimate.bodyAngRates_radps[idx] =
        ekf_data_.bias_corr_body_rates_radps[idx];
    fcs_model_autocode_u_.stateEstimate.bodyAccels_mps2[idx] =
        ekf_data_.bias_corr_body_accels_mps2[idx];
    fcs_model_autocode_u_.stateEstimate.nedPos_m[idx] =
        ekf_data_.nedpos_m[idx];
    fcs_model_autocode_u_.stateEstimate.nedVel_mps[idx] =
        ekf_data_.nedvel_mps[idx];
  }

  for (size_t dIdx = 0; dIdx < 9; dIdx++) {
    fcs_model_autocode_u_.stateEstimate.ned2BodyDcm_nd[dIdx] =
        ekf_data_.dcm_ned_to_body[dIdx];
    fcs_model_autocode_u_.stateEstimate.ned2FepDcm_nd[dIdx] =
        ekf_data_.dcm_ned_to_fep[dIdx];
  }
  fcs_model_autocode_u_.stateEstimate.aglEst_m = -ekf_data_.states[6];
  fcs_model_autocode_u_.stateEstimate.climbRateEst_mps = -ekf_data_.states[9];

  if (baro_sub_.copy(baro_data_)) {
    fcs_model_autocode_u_.stateEstimate.pressure_mbar = baro_data_.press_pa;
    fcs_model_autocode_u_.stateEstimate.temp_c = baro_data_.temp_degc;
  }

  if (gps_sub_.copy(gps_data_)) {
    fcs_model_autocode_u_.stateEstimate.geodeticPos.lat_rad =
        gps_data_.latitude_rad;
    fcs_model_autocode_u_.stateEstimate.geodeticPos.lon_rad =
        gps_data_.longitude_rad;
    fcs_model_autocode_u_.stateEstimate.geodeticPos.alt_m =
        gps_data_.altitude_m;
  }

  if (rcchannels_sub_.copy(rcchannels_data_)) {
    fcs_model_autocode_u_.rcCmdsIn.joystickXCmd_nd = rcchannels_data_.roll;
    fcs_model_autocode_u_.rcCmdsIn.joystickYCmd_nd = rcchannels_data_.pitch;
    fcs_model_autocode_u_.rcCmdsIn.joystickZCmd_nd = rcchannels_data_.yaw;
    fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd = rcchannels_data_.throttle;
    fcs_model_autocode_u_.rcCmdsIn.rcSwitch1_nd = rcchannels_data_.sw1;
    fcs_model_autocode_u_.rcCmdsIn.rcSwitch2_nd = rcchannels_data_.sw2;
    fcs_model_autocode_u_.rcCmdsIn.rcSwitch3_nd = rcchannels_data_.sw3;
  }

  if (mavlink_sub_.copy(mavlink_data_)) {
    fcs_model_autocode_u_.externalCmds.armCmdIssued =
        mavlink_data_.arm_cmd_issued;
  } else {
    fcs_model_autocode_u_.externalCmds.armCmdIssued = 0U;
  }

  fcsModelObj_.step();
  ++fcs_debug_data_.fcs_step_seq;

  const auto& outputs = fcsModelObj_.getExternalOutputs();
  const auto& model_debug = outputs.fcsDebug;
  if (model_debug.state != enumStateMachine::INACTIVE) {
    if (fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd <=
        kMinPwmCheckThreshold) {
      pwm_data_.pwm_cmds[0] =
          fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
      pwm_data_.pwm_cmds[1] =
          fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
      pwm_data_.pwm_cmds[2] =
          fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
      pwm_data_.pwm_cmds[3] =
          fcs_model_autocode_u_.rcCmdsIn.throttleCmd_nd;
    } else {
      pwm_data_.pwm_cmds[0] = outputs.actuatorsPwmCmds[0];
      pwm_data_.pwm_cmds[1] = outputs.actuatorsPwmCmds[1];
      pwm_data_.pwm_cmds[2] = outputs.actuatorsPwmCmds[2];
      pwm_data_.pwm_cmds[3] = outputs.actuatorsPwmCmds[3];
    }
  } else {
    pwm_data_.pwm_cmds[0] = kMinPwmThreshold;
    pwm_data_.pwm_cmds[1] = kMinPwmThreshold;
    pwm_data_.pwm_cmds[2] = kMinPwmThreshold;
    pwm_data_.pwm_cmds[3] = kMinPwmThreshold;
  }
  pubSubManager().publish<PwmData>(TopicID::PWM, pwm_data_);
  if (direct_pwm_output != nullptr) {
    *direct_pwm_output = pwm_data_;
  }

  fcs_debug_data_.sm_mode = static_cast<uint8_t>(model_debug.state);
  fcs_debug_data_.flt_mode = static_cast<uint8_t>(model_debug.flightMode);
  fcs_debug_data_.p_cmd_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[0];
  fcs_debug_data_.p_meas_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.meas[0];
  fcs_debug_data_.p_kp_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[0]
                                 .proportionalOutput;
  fcs_debug_data_.p_ki_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[0]
                                 .integralOutput;
  fcs_debug_data_.q_cmd_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[1];
  fcs_debug_data_.q_meas_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.meas[1];
  fcs_debug_data_.q_kp_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[1]
                                 .proportionalOutput;
  fcs_debug_data_.q_ki_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[1]
                                 .integralOutput;
  fcs_debug_data_.r_cmd_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[2];
  fcs_debug_data_.r_meas_radps =
      model_debug.innerLoopCtrlDebug.angRateCtrlDebug.meas[2];
  fcs_debug_data_.r_kp_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[2]
                                 .proportionalOutput;
  fcs_debug_data_.r_ki_out = model_debug.innerLoopCtrlDebug
                                 .angRateCtrlDebug.pidDebug[2]
                                 .integralOutput;
  fcs_debug_data_.phi_cmd_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.cmd[0];
  fcs_debug_data_.phi_meas_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.meas[0];
  fcs_debug_data_.phi_kp_out =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[0]
          .proportionalOutput;
  fcs_debug_data_.theta_cmd_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.cmd[1];
  fcs_debug_data_.theta_meas_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.meas[1];
  fcs_debug_data_.theta_kp_out =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[1]
          .proportionalOutput;
  fcs_debug_data_.psi_cmd_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.cmd[2];
  fcs_debug_data_.psi_meas_rad =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.meas[2];
  fcs_debug_data_.psi_kp_out =
      model_debug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[2]
          .proportionalOutput;
  fcs_debug_data_.vn_cmd_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.cmd[0];
  fcs_debug_data_.vn_meas_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.meas[0];
  fcs_debug_data_.vn_kp_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[0]
          .proportionalOutput;
  fcs_debug_data_.vn_ki_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[0].integralOutput;
  fcs_debug_data_.ve_cmd_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.cmd[1];
  fcs_debug_data_.ve_meas_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.meas[1];
  fcs_debug_data_.ve_kp_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[1]
          .proportionalOutput;
  fcs_debug_data_.ve_ki_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[1].integralOutput;
  fcs_debug_data_.vd_cmd_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.cmd[2];
  fcs_debug_data_.vd_meas_mps =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.meas[2];
  fcs_debug_data_.vd_kp_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[2]
          .proportionalOutput;
  fcs_debug_data_.vd_ki_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.pidDebug[2].integralOutput;
  fcs_debug_data_.vd_ff_out =
      model_debug.outerLoopCtrlDebug.velCtrlDebug.velCtrlFf[2];
  fcs_debug_data_.hover_thrust_est =
      model_debug.outerLoopCtrlDebug.zAccelCtrlDebug.pidDebug.output;
  fcs_debug_data_.vd_meas_thrust_est =
      model_debug.outerLoopCtrlDebug.zAccelCtrlDebug.cmd;
  fcs_debug_data_.ad_meas_thrust_est =
      model_debug.outerLoopCtrlDebug.zAccelCtrlDebug.meas;
  fcs_debug_data_.alt_ctrl_trigger =
      model_debug.outerLoopCtrlDebug.zAccelCtrlDebug.pidDebug
          .proportionalOutput;
  fcs_debug_data_.pn_cmd_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.cmd[0];
  fcs_debug_data_.pn_meas_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.meas[0];
  fcs_debug_data_.pn_kp_out =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[0]
          .proportionalOutput;
  fcs_debug_data_.pe_cmd_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.cmd[1];
  fcs_debug_data_.pe_meas_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.meas[1];
  fcs_debug_data_.pe_kp_out =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[1]
          .proportionalOutput;
  fcs_debug_data_.pd_cmd_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.cmd[2];
  fcs_debug_data_.pd_meas_m =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.meas[2];
  fcs_debug_data_.pd_kp_out =
      model_debug.outerLoopCtrlDebug.posCtrlDebug.pidDebug[2]
          .proportionalOutput;
  fcs_debug_data_.thrust_cmd_N = model_debug.allocDebug.thrustCmd_N;
  fcs_debug_data_.xmom_cmd_Nm = model_debug.allocDebug.xMomCmd_Nm;
  fcs_debug_data_.ymom_cmd_Nm = model_debug.allocDebug.yMomCmd_Nm;
  fcs_debug_data_.zmom_cmd_Nm = model_debug.allocDebug.zMomCmd_Nm;
  fcs_debug_data_.chirp_debug = model_debug.sysIdDebug.chirpSignal;
  fcs_debug_data_.chirp_trigger =
      static_cast<uint8_t>(model_debug.sysIdDebug.chirpTrigger);
  fcs_debug_data_.chirp_type =
      static_cast<uint8_t>(model_debug.sysIdDebug.chirpType);

  pubSubManager().publish<FcsDebugData>(TopicID::FCSDEBUG, fcs_debug_data_);
  parameter_store.ApplyOneFcsUpdate();
  return true;
}

void FlightControls::Run() {
  osDelay(500);
  if (!InitializeController()) {
    vTaskSuspend(nullptr);
    return;
  }

  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(LOOP_INTERVAL_MS);
  ConfigurePeriodicMetrics(LOOP_INTERVAL_MS * 1000U,
                           LOOP_INTERVAL_MS * 1000U);

  for (;;) {
    BeginMetricsCycle();
    scheduled_start_tick_ = xLastWakeTime;
    static_cast<void>(StepOnce());
    EndMetricsCycle();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

#if !CONTROL_PIPELINE_ENABLE
FlightControls flight_controls_task_instance_;
#endif
