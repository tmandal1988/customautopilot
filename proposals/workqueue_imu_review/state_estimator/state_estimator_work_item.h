#pragma once

#include "main.h"
#include "constants.h"
#include "debug.h"
#include "messages/baro_data.h"
#include "messages/ekf_data.h"
#include "messages/gps_data.h"
#include "messages/imu_data.h"
#include "messages/mtf01p_data.h"
#include "pubsub/pubsub_manager.h"
#include "pubsub/subscriber.h"
#include "stateEstimatorAutocode/stateEstimatorEskfAutocode.h"
#include "work_queue/work_item.h"

// Event-driven state estimator on wq:nav.
//
// Runs whenever the IMU work item publishes a fresh ImuData sample (the IMU
// registers this work item as a downstream consumer in StateEstimatorWorkItem::
// Init()). Secondary topics (baro, GPS, optical flow) are copied opportunistic-
// ally with the same valid-this-cycle gating the polling implementation used,
// so the autocode sees identical inputs without any periodic timer.
//
// Not a ScheduledWorkItem: there is no fallback periodic schedule. If the IMU
// stops publishing, the EKF stops too, which is the intended behavior — there
// is nothing useful to integrate from. Add a watchdog later if needed.
class StateEstimatorWorkItem : public work_queue::WorkItem {
public:
    StateEstimatorWorkItem();

    // Advertise EkfData and subscribe to the IMU publish chain. Must be
    // called before the IMU work item starts producing (i.e. before
    // read_icm20948_work_item.Init()), so the downstream registration is in
    // place by the time the first IMU sample lands.
    bool Init();

    // Driver-specific diagnostics. Generic WorkItem stats (run count, max
    // runtime, schedule duplicates/fails) come from the base class.
    uint32_t StepCycles() const     { return last_step_cycles_; }
    uint32_t MaxStepCycles() const  { return max_step_cycles_; }
    uint32_t StepsExecuted() const  { return steps_executed_; }
    uint32_t StaleImuRuns() const   { return stale_imu_runs_; }

private:
    void Run(work_queue::WorkReason reason) override;

    void CopyImuInputs();
    void CopyAuxInputs();
    void PublishEkfOutputs();

    // Match the constants the production StateEstimator task uses verbatim so
    // the autocode input scaling does not change on migration.
    static constexpr float kMaxAllowedImuDtS = 0.05f;
    static constexpr float kOfScale          = 0.01f;
    static constexpr float kOfDistScale      = 0.001f;

    Subscriber<ImuData>    imu_sub_{TopicID::ICM20948};
    Subscriber<BaroData>   baro_sub_{TopicID::BMP390L};
    Subscriber<GpsData>    gps_sub_{TopicID::UBLOXM9N};
    Subscriber<Mtf01pData> mtf01p_sub_{TopicID::MTF01P};

    stateEstimatorAutocode autocode_;
    stateEstimatorAutocode::ExtU_stateEstimatorEskfAutoco_T autocode_inputs_{};
    stateEstimatorAutocode::ExtY_stateEstimatorEskfAutoco_T autocode_outputs_{};

    ImuData    imu_data_{};
    BaroData   baro_data_{};
    GpsData    gps_data_{};
    Mtf01pData mtf01p_data_{};
    EkfData    ekf_data_{};

    float prev_imu_time_s_;
    int   gps_fix_count_;

    bool initialized_;
    bool imu_updated_this_run_;

    uint32_t last_step_cycles_;
    uint32_t max_step_cycles_;
    uint32_t steps_executed_;
    uint32_t stale_imu_runs_;
};

extern StateEstimatorWorkItem state_estimator_work_item;
