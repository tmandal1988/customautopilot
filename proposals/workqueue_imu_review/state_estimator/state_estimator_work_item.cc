#include "state_estimator/state_estimator_work_item.h"

#include <string.h>

#include "stm32h7xx.h"

#include "sensors/icm20948_work_item.h"

StateEstimatorWorkItem state_estimator_work_item;

StateEstimatorWorkItem::StateEstimatorWorkItem()
    : work_queue::WorkItem(
          "StateEstimator",
          work_queue::configurations::nav_and_control),
      prev_imu_time_s_(0.0f),
      gps_fix_count_(0),
      initialized_(false),
      imu_updated_this_run_(false),
      last_step_cycles_(0),
      max_step_cycles_(0),
      steps_executed_(0),
      stale_imu_runs_(0) {}

bool StateEstimatorWorkItem::Init() {
    pubSubManager().advertise<EkfData>(TopicID::EKF);

    autocode_.initialize();

    // Subscribe to the IMU's publish chain. From this point on, every time
    // ReadIcm20948WorkItem::CompleteDmaRead() finishes publishing ImuData it
    // will call ScheduleNow() on us, waking the wq:nav worker.
    if (!read_icm20948_work_item.RegisterDownstreamConsumer(*this)) {
        return false;
    }

    initialized_ = true;
    return true;
}

void StateEstimatorWorkItem::Run(work_queue::WorkReason /*reason*/) {
    if (!initialized_) {
        return;
    }

    CopyImuInputs();
    CopyAuxInputs();

    autocode_.setExternalInputs(&autocode_inputs_);

    if (imu_updated_this_run_) {
        const uint32_t start_cycles = DWT->CYCCNT;
        autocode_.step();
        last_step_cycles_ = DWT->CYCCNT - start_cycles;
        if (last_step_cycles_ > max_step_cycles_) {
            max_step_cycles_ = last_step_cycles_;
        }
        ++steps_executed_;
    } else {
        // Triggered by a downstream wake but the IMU topic had nothing new
        // for us — most likely the IMU coalesced two publishes between our
        // runs. Read the latest outputs anyway so the published EkfData stays
        // current, but skip the integration step.
        ++stale_imu_runs_;
    }

    autocode_outputs_ = autocode_.getExternalOutputs();
    PublishEkfOutputs();

    // Wake the next stage in the chain (FCS once migrated). Today there are
    // no downstream consumers registered, so this is a no-op.
    NotifyDownstreamConsumers();
}

void StateEstimatorWorkItem::CopyImuInputs() {
    imu_updated_this_run_ = imu_sub_.copy(imu_data_);

    if (imu_updated_this_run_) {
        for (size_t idx = 0; idx < 3; ++idx) {
            autocode_inputs_.imuData.bodyAccels_mps2[idx]  = imu_data_.accel_mps2[idx];
            autocode_inputs_.imuData.bodyRates_radps[idx]  = imu_data_.gyro_radps[idx];
            autocode_inputs_.magData.bodyMagVector_uT[idx] = imu_data_.mag_ut[idx];
        }

        // dt is derived from publish-time timestamps; identical to what the
        // original polling task computed because pubsub stamps the message at
        // publish time, not at copy time.
        const float curr_imu_time_s = static_cast<float>(imu_data_.timestamp_ms) * 0.001f;
        const float dt_imu_time_s   = curr_imu_time_s - prev_imu_time_s_;
        const bool  dt_in_range     = (dt_imu_time_s > 0.0f) &&
                                      (dt_imu_time_s <= kMaxAllowedImuDtS);

        autocode_inputs_.imuData.dtImuTime_s     = dt_imu_time_s;
        autocode_inputs_.imuData.isImuDataValid  = dt_in_range;
        autocode_inputs_.magData.isMagDataValid  = true;
        prev_imu_time_s_                         = curr_imu_time_s;
    } else {
        autocode_inputs_.imuData.isImuDataValid = false;
        autocode_inputs_.magData.isMagDataValid = false;
    }
}

void StateEstimatorWorkItem::CopyAuxInputs() {
    if (baro_sub_.copy(baro_data_)) {
        autocode_inputs_.baroData.pressure_pa     = baro_data_.press_pa;
        autocode_inputs_.baroData.isBaroDataValid = true;
    } else {
        autocode_inputs_.baroData.isBaroDataValid = false;
    }

    if (gps_sub_.copy(gps_data_)) {
        autocode_inputs_.gpsData.latLonAlt[0] = gps_data_.latitude_rad;
        autocode_inputs_.gpsData.latLonAlt[1] = gps_data_.longitude_rad;
        autocode_inputs_.gpsData.latLonAlt[2] = gps_data_.altitude_m;
        autocode_inputs_.gpsData.nedVel_mps[0] = gps_data_.vn_mps;
        autocode_inputs_.gpsData.nedVel_mps[1] = gps_data_.ve_mps;
        autocode_inputs_.gpsData.nedVel_mps[2] = gps_data_.vd_mps;

        const bool gps_fix_acquired = gps_data_.fix_type >= 2;
        autocode_inputs_.gpsData.isGpsDataValid = gps_fix_acquired;
        ekf_data_.is_gps_valid                  = gps_fix_acquired;

        if (gps_fix_acquired && (gps_fix_count_ < 10)) {
            ++gps_fix_count_;
        }
        autocode_inputs_.gpsData.isGpsInitialized = (gps_fix_count_ >= 10);
    } else {
        autocode_inputs_.gpsData.isGpsDataValid = false;
        ekf_data_.is_gps_valid                  = false;
    }

    if (mtf01p_sub_.copy(mtf01p_data_)) {
        autocode_inputs_.mtf01pData.distPrecision     = mtf01p_data_.precision;
        autocode_inputs_.mtf01pData.distStatus        = mtf01p_data_.dis_status;
        autocode_inputs_.mtf01pData.distStrength      = mtf01p_data_.strength;
        autocode_inputs_.mtf01pData.dist_m            =
            static_cast<float>(mtf01p_data_.distance) * kOfDistScale;
        autocode_inputs_.mtf01pData.flowX_radps       =
            static_cast<float>(mtf01p_data_.flow_vel_x) * kOfScale;
        autocode_inputs_.mtf01pData.flowY_radps       =
            static_cast<float>(mtf01p_data_.flow_vel_y) * kOfScale;
        autocode_inputs_.mtf01pData.flowQuality       = mtf01p_data_.flow_quality;
        autocode_inputs_.mtf01pData.flowStatus        = mtf01p_data_.flow_status;
        autocode_inputs_.mtf01pData.isMtf01pDataValid = true;
    } else {
        autocode_inputs_.mtf01pData.isMtf01pDataValid = false;
    }
}

void StateEstimatorWorkItem::PublishEkfOutputs() {
    for (size_t idx = 0; idx < 3; ++idx) {
        ekf_data_.euler_rad[idx] = autocode_outputs_.eulAng_rad[idx];

        ekf_data_.bias_corr_body_rates_radps[idx]  =
            autocode_inputs_.imuData.bodyRates_radps[idx] -
            autocode_outputs_.states[idx + 10];
        ekf_data_.bias_corr_body_accels_mps2[idx]  = autocode_outputs_.bodyAccels_mps2[idx];

        ekf_data_.dcm_ned_to_body[idx]     = autocode_outputs_.dcmNedToBody[idx];
        ekf_data_.dcm_ned_to_body[idx + 3] = autocode_outputs_.dcmNedToBody[idx + 3];
        ekf_data_.dcm_ned_to_body[idx + 6] = autocode_outputs_.dcmNedToBody[idx + 6];

        ekf_data_.dcm_ned_to_fep[idx]      = autocode_outputs_.dcmNedToFep[idx];
        ekf_data_.dcm_ned_to_fep[idx + 3]  = autocode_outputs_.dcmNedToFep[idx + 3];
        ekf_data_.dcm_ned_to_fep[idx + 6]  = autocode_outputs_.dcmNedToFep[idx + 6];

        ekf_data_.nedpos_m[idx]   = autocode_outputs_.states[idx + 4];
        ekf_data_.nedvel_mps[idx] = autocode_outputs_.states[idx + 7];
    }

    ekf_data_.is_mag_valid  = autocode_inputs_.magData.isMagDataValid;
    ekf_data_.is_baro_valid = autocode_inputs_.baroData.isBaroDataValid;

    memcpy(ekf_data_.states, autocode_outputs_.states, sizeof(float) * 23);

    ekf_data_.state_init_pct = autocode_outputs_.stateEstimatorDebug.stateEstInitPct;
    ekf_data_.sm_mode        = static_cast<uint8_t>(autocode_outputs_.stateEstimatorDebug.smMode);

    pubSubManager().publish<EkfData>(TopicID::EKF, ekf_data_);
}
