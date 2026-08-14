/*
 * state_estimator.h
 *
 *  Created on: Apr 9, 2025
 *      Author: tanmay
 */

#pragma once

#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "messages/imu_data.h"
#include "messages/baro_data.h"
#include "messages/gps_data.h"
#include "messages/mtf01p_data.h"
#include "messages/magnetometer_data.h"
#include "messages/ekf_data.h"
#include "messages/state_estimator_debug_data.h"
#include "constants.h"
#include "sensor_config.h"
#include "pubsub/subscriber.h"
#include "debug.h"

#include "stateEstimatorAutocode/stateEstimatorEskfAutocode.h"

class StateEstimator : public TaskBase {
public:
	explicit StateEstimator(bool register_task = true);
    void Run() override;
    void InitializeEstimator();
    bool StepOnce(TickType_t scheduled_start_tick, EkfData* output = nullptr);
#if RTOS_METRICS_ENABLE
    uint32_t MaxInputPrepCycles() const { return max_input_prep_cycles_; }
    uint32_t MaxAutocodeStepCycles() const { return max_autocode_step_cycles_; }
    uint32_t MaxOutputPublishCycles() const {
      return max_output_publish_cycles_;
    }
#endif

private:
    stateEstimatorAutocode stateEstimatorAutocodeObj_;
    stateEstimatorAutocode::ExtU_stateEstimatorEskfAutoco_T state_estimator_autocode_u_ = {0};

    Subscriber<ImuData> imu_sub_ = Subscriber<ImuData>(TopicID::ICM20948);
    Subscriber<MagnetometerData> ist8310_mag_sub_ =
        Subscriber<MagnetometerData>(TopicID::IST8310);
    Subscriber<BaroData> baro_sub_ = Subscriber<BaroData>(TopicID::BMP390L);
    Subscriber<GpsData> gps_sub_ = Subscriber<GpsData>(TopicID::UBLOXM9N);
    Subscriber<Mtf01pData> mtf01p_sub_ = Subscriber<Mtf01pData>(TopicID::MTF01P);

    ImuData imu_data_ = {};
    BaroData baro_data_ = {};
    GpsData gps_data_ = {};
    Mtf01pData mtf01p_data_ = {};
    EkfData ekf_data_ = {};
    StateEstimatorDebugData state_estimator_debug_data_ = {};

    float prev_imu_time_s = 0.0;
    int gps_fix_count_ = 0;
    int blink_counter_ = 0;
    bool first_iteration_ = true;
    static constexpr float MAX_ALLOWED_IMU_DT_S = 0.05;

    static constexpr float OF_SCALE = 0.01;
    static constexpr float OF_DIST_SCALE = 0.001;

    static constexpr uint16_t READ_INTERVAL_MS = 4; // 250Hz
    static constexpr uint32_t kAllowedStartLatenessTicks = 0;
#if STATE_ESTIMATOR_FAST_DEBUG_LOG_ENABLE
    static constexpr uint32_t kDebugPublishDecimation = 1U;
#else
    static constexpr uint32_t kDebugPublishDecimation = 10U; // 25Hz at 250Hz
#endif

#if RTOS_METRICS_ENABLE
    uint32_t max_input_prep_cycles_ = 0U;
    uint32_t max_autocode_step_cycles_ = 0U;
    uint32_t max_output_publish_cycles_ = 0U;

    static void UpdateMax(uint32_t* maximum, uint32_t candidate) {
      if ((maximum != nullptr) && (candidate > *maximum)) {
        *maximum = candidate;
      }
    }
#endif
};
