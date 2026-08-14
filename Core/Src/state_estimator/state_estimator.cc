/*
 * state_estimator.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: tanmay
 */

#include "state_estimator.h"

#include "pubsub/pubsub_manager.h"

#include <cstring>

namespace {

// Simulink represents uint64 values as two 32-bit words for this embedded
// target. Keep the conversion explicit so the generated timestamp inputs use
// the same low-word-first layout as the autocode's multiword helpers.
void SetAutocodeTimestampMs(uint64m_T& destination, uint64_t timestamp_ms) {
  destination.chunks[0] = static_cast<uint32_T>(timestamp_ms);
  destination.chunks[1] = static_cast<uint32_T>(timestamp_ms >> 32U);
}

uint8_t ToLogFifoStatus(enumDhFifoStatus status) {
  return static_cast<uint8_t>(status);
}

void CopyFifoDebug(const busFifoDebug& source,
                   StateEstimatorFifoDebugData* destination) {
  if (destination == nullptr) {
    return;
  }
  destination->status = ToLogFifoStatus(source.status);
  destination->count = source.count;
}

}  // namespace

StateEstimator::StateEstimator(bool register_task)
    : TaskBase("EkfEst250Hz", 11000, osPriorityAboveNormal,
               register_task) {}

void StateEstimator::InitializeEstimator() {
  DEBUG_PRINT("FreeRTOS heap remaining: %u bytes\n", xPortGetFreeHeapSize());
  pubSubManager().advertise<EkfData>(TopicID::EKF);
  pubSubManager().advertise<StateEstimatorDebugData>(
      TopicID::STATE_ESTIMATOR_DEBUG);
  stateEstimatorAutocodeObj_.initialize();
  gps_fix_count_ = 0;
  blink_counter_ = 0;
  first_iteration_ = true;
  prev_imu_time_s = 0.0F;
  ekf_data_ = {};
  state_estimator_debug_data_ = {};
}

bool StateEstimator::StepOnce(TickType_t scheduled_start_tick,
                              EkfData* output) {
  const TickType_t actual_start_tick = xTaskGetTickCount();
  const int32_t start_lateness_ticks =
      static_cast<int32_t>(actual_start_tick - scheduled_start_tick);

  ++ekf_data_.task_run_seq;
  if (!first_iteration_ &&
      start_lateness_ticks > static_cast<int32_t>(kAllowedStartLatenessTicks)) {
    ++ekf_data_.late_start_count;
  }
  first_iteration_ = false;

  if (++blink_counter_ >= 100) {
    blink_counter_ = 0;
  }

#if RTOS_METRICS_ENABLE
  const uint32_t input_prep_start_cycles = rtos_metrics::CyclesNow();
#endif

  bool imu_updated = false;
  if (imu_sub_.copy(imu_data_)) {
    imu_updated = true;
    for (size_t idx = 0; idx < 3; ++idx) {
      state_estimator_autocode_u_.imuData.bodyAccels_mps2[idx] =
          imu_data_.accel_mps2[idx];
      state_estimator_autocode_u_.imuData.bodyRates_radps[idx] =
          imu_data_.gyro_radps[idx];
    }
    SetAutocodeTimestampMs(state_estimator_autocode_u_.imuData.timestamp_ms,
                           imu_data_.timestamp_ms);

    const float curr_imu_time_s =
        static_cast<float>(imu_data_.timestamp_ms) * 0.001F;
    const float dt_imu_time_s = curr_imu_time_s - prev_imu_time_s;
    if ((dt_imu_time_s > 0.0F) &&
        (dt_imu_time_s <= MAX_ALLOWED_IMU_DT_S)) {
      state_estimator_autocode_u_.imuData.dtImuTime_s = dt_imu_time_s;
      state_estimator_autocode_u_.imuData.isImuDataValid = true;
    } else {
      state_estimator_autocode_u_.imuData.isImuDataValid = false;
    }
    prev_imu_time_s = curr_imu_time_s;
  } else {
    state_estimator_autocode_u_.imuData.isImuDataValid = false;
  }

  state_estimator_autocode_u_.magData.isMagDataValid = false;
  if (imu_updated) {
    if constexpr (kStateEstimatorMagnetometerSource ==
                  MagnetometerSource::kIst8310) {
      MagnetometerData mag_data = {};
      if (ist8310_mag_sub_.copy(mag_data)) {
        for (size_t idx = 0; idx < 3; ++idx) {
          state_estimator_autocode_u_.magData.bodyMagVector_uT[idx] =
              mag_data.mag_ut[idx];
        }
        SetAutocodeTimestampMs(
            state_estimator_autocode_u_.magData.timestamp_ms,
            mag_data.timestamp_ms);
        state_estimator_autocode_u_.magData.isMagDataValid = true;
      }
    } else {
      for (size_t idx = 0; idx < 3; ++idx) {
        state_estimator_autocode_u_.magData.bodyMagVector_uT[idx] =
            imu_data_.mag_ut[idx];
      }
      SetAutocodeTimestampMs(state_estimator_autocode_u_.magData.timestamp_ms,
                             imu_data_.timestamp_ms);
      state_estimator_autocode_u_.magData.isMagDataValid = true;
    }
  }

  if (baro_sub_.copy(baro_data_)) {
    state_estimator_autocode_u_.baroData.pressure_pa = baro_data_.press_pa;
    SetAutocodeTimestampMs(state_estimator_autocode_u_.baroData.timestamp_ms,
                           baro_data_.timestamp_ms);
    state_estimator_autocode_u_.baroData.isBaroDataValid = true;
  } else {
    state_estimator_autocode_u_.baroData.isBaroDataValid = false;
  }

  if (gps_sub_.copy(gps_data_)) {
    state_estimator_autocode_u_.gpsData.latLonAlt[0] = gps_data_.latitude_rad;
    state_estimator_autocode_u_.gpsData.latLonAlt[1] = gps_data_.longitude_rad;
    state_estimator_autocode_u_.gpsData.latLonAlt[2] = gps_data_.altitude_m;
    state_estimator_autocode_u_.gpsData.nedVel_mps[0] = gps_data_.vn_mps;
    state_estimator_autocode_u_.gpsData.nedVel_mps[1] = gps_data_.ve_mps;
    state_estimator_autocode_u_.gpsData.nedVel_mps[2] = gps_data_.vd_mps;
    SetAutocodeTimestampMs(state_estimator_autocode_u_.gpsData.timestamp_ms,
                           gps_data_.timestamp_ms);

    if (gps_data_.fix_type >= 2) {
      state_estimator_autocode_u_.gpsData.isGpsDataValid = true;
      ekf_data_.is_gps_valid = true;
    } else {
      state_estimator_autocode_u_.gpsData.isGpsDataValid = false;
      ekf_data_.is_gps_valid = false;
    }

    if (gps_fix_count_ < 10) {
      if (gps_data_.fix_type >= 2) {
        ++gps_fix_count_;
      }
    }
    state_estimator_autocode_u_.gpsData.isGpsInitialized =
        gps_fix_count_ >= 10;
  } else {
    state_estimator_autocode_u_.gpsData.isGpsDataValid = false;
    ekf_data_.is_gps_valid = false;
  }

  if (mtf01p_sub_.copy(mtf01p_data_)) {
    state_estimator_autocode_u_.mtf01pData.distPrecision =
        mtf01p_data_.precision;
    state_estimator_autocode_u_.mtf01pData.distStatus =
        mtf01p_data_.dis_status;
    state_estimator_autocode_u_.mtf01pData.distStrength =
        mtf01p_data_.strength;
    state_estimator_autocode_u_.mtf01pData.dist_m =
        static_cast<float>(mtf01p_data_.distance) * OF_DIST_SCALE;
    state_estimator_autocode_u_.mtf01pData.flowX_radps =
        static_cast<float>(mtf01p_data_.flow_vel_x) * OF_SCALE;
    state_estimator_autocode_u_.mtf01pData.flowY_radps =
        static_cast<float>(mtf01p_data_.flow_vel_y) * OF_SCALE;
    state_estimator_autocode_u_.mtf01pData.flowQuality =
        mtf01p_data_.flow_quality;
    state_estimator_autocode_u_.mtf01pData.flowStatus =
        mtf01p_data_.flow_status;
    SetAutocodeTimestampMs(state_estimator_autocode_u_.mtf01pData.timestamp_ms,
                           mtf01p_data_.timestamp_ms);
    state_estimator_autocode_u_.mtf01pData.isMtf01pDataValid = true;
  } else {
    state_estimator_autocode_u_.mtf01pData.isMtf01pDataValid = false;
  }

#if RTOS_METRICS_ENABLE
  UpdateMax(&max_input_prep_cycles_,
            rtos_metrics::CyclesNow() - input_prep_start_cycles);
  const uint32_t autocode_step_start_cycles = rtos_metrics::CyclesNow();
#endif

  stateEstimatorAutocodeObj_.setExternalInputs(&state_estimator_autocode_u_);
  if (imu_updated) {
    stateEstimatorAutocodeObj_.step();
    ++ekf_data_.ekf_step_seq;
  }

#if RTOS_METRICS_ENABLE
  UpdateMax(&max_autocode_step_cycles_,
            rtos_metrics::CyclesNow() - autocode_step_start_cycles);
  const uint32_t output_publish_start_cycles = rtos_metrics::CyclesNow();
#endif

  const auto& y = stateEstimatorAutocodeObj_.getExternalOutputs();
  for (size_t idx = 0; idx < 3; idx++) {
    ekf_data_.euler_rad[idx] = y.eulAng_rad[idx];
    ekf_data_.bias_corr_body_rates_radps[idx] =
        state_estimator_autocode_u_.imuData.bodyRates_radps[idx] -
        y.states[idx + 10];
    ekf_data_.bias_corr_body_accels_mps2[idx] = y.bodyAccels_mps2[idx];
    ekf_data_.dcm_ned_to_body[idx] = y.dcmNedToBody[idx];
    ekf_data_.dcm_ned_to_body[idx + 3] = y.dcmNedToBody[idx + 3];
    ekf_data_.dcm_ned_to_body[idx + 6] = y.dcmNedToBody[idx + 6];
    ekf_data_.dcm_ned_to_fep[idx] = y.dcmNedToFep[idx];
    ekf_data_.dcm_ned_to_fep[idx + 3] = y.dcmNedToFep[idx + 3];
    ekf_data_.dcm_ned_to_fep[idx + 6] = y.dcmNedToFep[idx + 6];
    ekf_data_.nedpos_m[idx] = y.states[idx + 4];
    ekf_data_.nedvel_mps[idx] = y.states[idx + 7];
  }
  ekf_data_.is_mag_valid =
      state_estimator_autocode_u_.magData.isMagDataValid;
  ekf_data_.is_baro_valid =
      state_estimator_autocode_u_.baroData.isBaroDataValid;

  std::memset(ekf_data_.states, 0, sizeof(ekf_data_.states));
  std::memcpy(ekf_data_.states, y.states, sizeof(y.states));
  ekf_data_.state_init_pct =
      y.stateEstimatorDebug.stateEstInitPct;
  ekf_data_.sm_mode = static_cast<uint8_t>(y.stateEstimatorDebug.smMode);

  if (imu_updated &&
      ((ekf_data_.ekf_step_seq % kDebugPublishDecimation) == 0U)) {
    const auto& debug = y.stateEstimatorDebug;
    const auto& ekf_debug = debug.ekfDebugData;
    const auto& buffer_debug = ekf_debug.dhBufferDebugData;

    state_estimator_debug_data_.is_mag_used =
        ekf_debug.isAidingUsed.isMagUsed;
    state_estimator_debug_data_.is_gps_used =
        ekf_debug.isAidingUsed.isGpsUsed;
    state_estimator_debug_data_.is_baro_used =
        ekf_debug.isAidingUsed.isBaroUsed;
    state_estimator_debug_data_.is_lidar_used =
        ekf_debug.isAidingUsed.isLidarUsed;
    state_estimator_debug_data_.is_flow_used =
        ekf_debug.isAidingUsed.isFlowUsed;

    std::memcpy(state_estimator_debug_data_.dh_states,
                ekf_debug.dhStates,
                sizeof(state_estimator_debug_data_.dh_states));
    CopyFifoDebug(buffer_debug.statusFifoDebugData,
                  &state_estimator_debug_data_.status_fifo);
    CopyFifoDebug(buffer_debug.imuFifoDebugData,
                  &state_estimator_debug_data_.imu_fifo);
    CopyFifoDebug(buffer_debug.magFifoDebugData,
                  &state_estimator_debug_data_.mag_fifo);
    CopyFifoDebug(buffer_debug.gpsFifoDebugData,
                  &state_estimator_debug_data_.gps_fifo);
    CopyFifoDebug(buffer_debug.baroFifoDebugData,
                  &state_estimator_debug_data_.baro_fifo);
    CopyFifoDebug(buffer_debug.lidarFifoDebugData,
                  &state_estimator_debug_data_.lidar_fifo);
    CopyFifoDebug(buffer_debug.flowFifoDebugData,
                  &state_estimator_debug_data_.flow_fifo);
    pubSubManager().publish<StateEstimatorDebugData>(
        TopicID::STATE_ESTIMATOR_DEBUG, state_estimator_debug_data_);
  }

  pubSubManager().publish<EkfData>(TopicID::EKF, ekf_data_);
  if (output != nullptr) {
    *output = ekf_data_;
  }
#if RTOS_METRICS_ENABLE
  UpdateMax(&max_output_publish_cycles_,
            rtos_metrics::CyclesNow() - output_publish_start_cycles);
#endif
  return imu_updated;
}

void StateEstimator::Run() {
  osDelay(500);
  InitializeEstimator();

  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);
  ConfigurePeriodicMetrics(READ_INTERVAL_MS * 1000U,
                           READ_INTERVAL_MS * 1000U);

  for (;;) {
    BeginMetricsCycle();
    static_cast<void>(StepOnce(xLastWakeTime));
    EndMetricsCycle();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

#if !CONTROL_PIPELINE_ENABLE
StateEstimator state_estimator_task_instance_;
#endif
