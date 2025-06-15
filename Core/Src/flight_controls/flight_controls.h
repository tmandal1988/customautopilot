/*
 * flight_controls.h
 *
 *  Created on: Jun 7, 2025
 *      Author: tanmay
 */

#ifndef SRC_FLIGHT_CONTROLS_FLIGHT_CONTROLS_H_
#define SRC_FLIGHT_CONTROLS_FLIGHT_CONTROLS_H_

#include "task_manager/task_base.h"
#include "pin_defines.h"
#include "messages/baro_data.h"
#include "messages/gps_data.h"
#include "messages/ekf_data.h"
#include "messages/rc_channels.h"
#include "messages/pwm_data.h"
#include "messages/fcs_debug_data.h"
#include "pubsub/subscriber.h"
#include "pubsub/publisher.h"
#include "debug.h"

#include "fcsModelAutocode/fcsModel.h"
#include "fcsModelAutocode/fcs_params.h"

class FlightControls : public TaskBase {
public:
	FlightControls();
    void Run() override;

private:
    fcsModel fcsModelObj_;

    static const uint16_t kMinPwmThreshold = 982; //Lowest pwm command
    static const uint16_t kMinPwmCheckThreshold = 1005; //Below this RC Throttle command value motor will follow RC Throttle command
    static constexpr uint16_t LOOP_INTERVAL_MS = 4; // 250Hz
    static constexpr int kOneSecIntervalCount = 1000 / LOOP_INTERVAL_MS;

    fcsModel::ExtU_fcsModel_T fcs_model_autocode_u_ = {0};
    fcsModel::ExtY_fcsModel_T fcs_model_autocode_y_ = {0};

    Subscriber<BaroData> baro_sub_ = Subscriber<BaroData>(TopicID::BMP390L);
    BaroData baro_data_ = {0};

    Subscriber<GpsData> gps_sub_ = Subscriber<GpsData>(TopicID::UBLOXM9N);
    GpsData gps_data_ = {0};

    Subscriber<EkfData> ekf_sub_ = Subscriber<EkfData>(TopicID::EKF);
    EkfData ekf_data_ = {0};

    Subscriber<RcChannels> rcchannels_sub_ = Subscriber<RcChannels>(TopicID::RCCHANNELS);
    RcChannels rcchannels_data_;

    PwmData pwm_data_ = {0};
    FcsDebugData fcs_debug_data_ = {0};
};



#endif /* SRC_FLIGHT_CONTROLS_FLIGHT_CONTROLS_H_ */
