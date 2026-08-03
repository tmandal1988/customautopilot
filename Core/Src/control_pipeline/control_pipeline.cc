#include "control_pipeline.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

ControlPipeline::ControlPipeline(TIM_HandleTypeDef* htim1_handle,
                                 TIM_HandleTypeDef* htim2_handle,
                                 TIM_HandleTypeDef* htim3_handle)
    : TaskBase("ControlPipeline", 24000, osPriorityHigh),
      htim1_(htim1_handle),
      htim2_(htim2_handle),
      htim3_(htim3_handle) {}

void ControlPipeline::ConstructModules() {
  if (estimator_ == nullptr) {
    estimator_ = new (estimator_storage_) StateEstimator(false);
  }
  if (flight_controls_ == nullptr) {
    flight_controls_ =
        new (flight_controls_storage_) FlightControls(false);
  }
  if (pwm_output_ == nullptr) {
    pwm_output_ =
        new (pwm_output_storage_) PwmCmds(htim1_, htim2_, htim3_, false);
  }
}

void ControlPipeline::Run() {
  DEBUG_PRINT("Starting deterministic control pipeline\n");
  ConstructModules();

  pwm_output_->InitializeOutputs();
  osDelay(2000);

  estimator_->InitializeEstimator();
  if (!flight_controls_->InitializeController()) {
    vTaskSuspend(nullptr);
    return;
  }

  TickType_t last_wake_time = xTaskGetTickCount();
  const TickType_t frequency = pdMS_TO_TICKS(kLoopIntervalMs);
  ConfigurePeriodicMetrics(kLoopIntervalMs * 1000U, kLoopIntervalMs * 1000U);

  for (;;) {
    BeginMetricsCycle();

#if RTOS_METRICS_ENABLE
    const std::uint32_t estimator_start_cycles = rtos_metrics::CyclesNow();
#endif
    const bool estimator_stepped =
        estimator_->StepOnce(last_wake_time, &ekf_data_);
#if RTOS_METRICS_ENABLE
    UpdateAuxMetricMaximum(
        0U, rtos_metrics::CyclesNow() - estimator_start_cycles);
    UpdateAuxMetricMaximum(1U, estimator_->MaxInputPrepCycles());
    UpdateAuxMetricMaximum(2U, estimator_->MaxAutocodeStepCycles());
    UpdateAuxMetricMaximum(3U, estimator_->MaxOutputPublishCycles());
#endif
    if (estimator_stepped) {
#if RTOS_METRICS_ENABLE
      const std::uint32_t fcs_start_cycles = rtos_metrics::CyclesNow();
#endif
      const bool controls_stepped =
          flight_controls_->StepOnce(&ekf_data_, &pwm_data_, last_wake_time);
#if RTOS_METRICS_ENABLE
      UpdateAuxMetricMaximum(4U,
                             rtos_metrics::CyclesNow() - fcs_start_cycles);
#endif
      if (controls_stepped) {
        pwm_output_->ApplyPwmData(pwm_data_);
      }
    }

    EndMetricsCycle();
    vTaskDelayUntil(&last_wake_time, frequency);
  }
}

#if CONTROL_PIPELINE_ENABLE
ControlPipeline control_pipeline_task_instance_(&htim1, &htim2, &htim3);
#endif
