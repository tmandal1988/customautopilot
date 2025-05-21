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
#include "messages/ekf_data.h"
#include "constants.h"
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
    stateEstimatorAutocode::ExtY_stateEstimatorEskfAutoco_T state_estimator_autocode_y_ = {0};

    Subscriber<ImuData> imu_sub_ = Subscriber<ImuData>(TopicID::ICM20948);
    Subscriber<BaroData> baro_sub_ = Subscriber<BaroData>(TopicID::BMP390L);
    Subscriber<GpsData> gps_sub_ = Subscriber<GpsData>(TopicID::UBLOXM9N);

    static constexpr uint16_t READ_INTERVAL_MS = 4; // 250Hz
};
