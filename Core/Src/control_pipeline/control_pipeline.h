#pragma once

#include "flight_controls.h"
#include "messages/ekf_data.h"
#include "messages/pwm_data.h"
#include "motor_output.h"
#include "state_estimator.h"
#include "task_manager/task_base.h"

#include <cstdint>
#include <new>

class ControlPipeline final : public TaskBase {
public:
    ControlPipeline(TIM_HandleTypeDef* htim1, TIM_HandleTypeDef* htim2,
                    TIM_HandleTypeDef* htim3);
    void Run() override;

private:
    static constexpr uint16_t kLoopIntervalMs = 4U;

    StateEstimator* estimator_ = nullptr;
    FlightControls* flight_controls_ = nullptr;
    MotorOutput* pwm_output_ = nullptr;

    TIM_HandleTypeDef* htim1_;
    TIM_HandleTypeDef* htim2_;
    TIM_HandleTypeDef* htim3_;

    alignas(StateEstimator) std::uint8_t
        estimator_storage_[sizeof(StateEstimator)] = {};
    alignas(FlightControls) std::uint8_t
        flight_controls_storage_[sizeof(FlightControls)] = {};
    alignas(MotorOutput) std::uint8_t
        pwm_output_storage_[sizeof(MotorOutput)] = {};

    EkfData ekf_data_ = {};
    PwmData pwm_data_ = {};

    void ConstructModules();
};
