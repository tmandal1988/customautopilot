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
#include "constants.h"
#include "sensor_config.h"
#include "pubsub/subscriber.h"
#include "pubsub/publisher.h"
#include "debug.h"

#include "stateEstimatorAutocode/stateEstimatorEskfAutocode.h"

class StateEstimator : public TaskBase {
public:
	StateEstimator();
    void Run() override;

private:
    stateEstimatorAutocode stateEstimatorAutocodeObj_;
    stateEstimatorAutocode::ExtU_stateEstimatorEskfAutoco_T state_estimator_autocode_u_ = {0};

	    Subscriber<ImuData> imu_sub_ = Subscriber<ImuData>(TopicID::ICM20948);
	    Subscriber<MagnetometerData> ist8310_mag_sub_ =
	        Subscriber<MagnetometerData>(TopicID::IST8310);
	    Subscriber<BaroData> baro_sub_ = Subscriber<BaroData>(TopicID::BMP390L);
    Subscriber<GpsData> gps_sub_ = Subscriber<GpsData>(TopicID::UBLOXM9N);
    Subscriber<Mtf01pData> mtf01p_sub_ = Subscriber<Mtf01pData>(TopicID::MTF01P);

    float prev_imu_time_s = 0.0;
    static constexpr float MAX_ALLOWED_IMU_DT_S = 0.05;

    static constexpr float OF_SCALE = 0.01;
    static constexpr float OF_DIST_SCALE = 0.001;

    static constexpr uint16_t READ_INTERVAL_MS = 4; // 250Hz
    static constexpr uint32_t kAllowedStartLatenessTicks = 0;
};
