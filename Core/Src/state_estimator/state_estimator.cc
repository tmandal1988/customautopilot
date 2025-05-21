/*
 * state_estimator.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: tanmay
 */

#include "state_estimator.h"

StateEstimator::StateEstimator():
TaskBase("StateEstimatorTask", 10000, osPriorityAboveNormal){
//
}

void StateEstimator::Run(){
	ImuData imu_data = {0};
	BaroData baro_data = {0};
	GpsData gps_data = {0};
	EkfData ekf_data = {0};
	Publisher<EkfData> ekf_pub_(TopicID::EKF);

	// On Cortex-M3/M4/M7 with DWT support:
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	int blink_counter = 0;
	DEBUG_PRINT("FreeRTOS heap remaining: %u bytes\n", xPortGetFreeHeapSize());

	// Initialize model
	stateEstimatorAutocodeObj_.initialize();
	int gps_fix_count = 0;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

//	UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//	uint32_t used = 2700 - highWaterMark * sizeof(StackType_t);
//	DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//    used, highWaterMark * sizeof(StackType_t), 2700);
	osDelay(500);
	bool imu_updated = false;
	uint32_t end_cycles = 0;
	uint32_t start_cycles = 0;
//	uint32_t elapsedTicks = 0;
	for(;;){
		if (++blink_counter >= 100) {
			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 30000 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//				   used, highWaterMark * sizeof(StackType_t), 10000);
			DEBUG_PRINT("Roll: %g, Pitch: %g, Yaw: %g, Ex Time: %g\n", ekf_data.euler_rad[0]/DEG2RAD,
					ekf_data.euler_rad[1]/DEG2RAD, ekf_data.euler_rad[2]/DEG2RAD, (float)(end_cycles - start_cycles) / (SystemCoreClock / 1e6));
//			DEBUG_PRINT("Ax: %g, Ay: %g, Az: %g\n", ekf_data.bias_corr_body_accels_mps2[0],
//					ekf_data.bias_corr_body_accels_mps2[1], ekf_data.bias_corr_body_accels_mps2[2]);
//			DEBUG_PRINT("Ax: %g, Ay: %g, Az: %g\n", imu_data.accel_mps2[0],
//					imu_data.accel_mps2[1], imu_data.accel_mps2[2]);

		}

		start_cycles = DWT->CYCCNT;
		// Read before
		if(imu_sub_.copy(imu_data)){
			imu_updated = true;
			for(size_t idx = 0; idx < 3; idx++){
				state_estimator_autocode_u_.imuData.bodyAccels_mps2[idx] = imu_data.accel_mps2[idx];
				state_estimator_autocode_u_.imuData.bodyRates_radps[idx] = imu_data.gyro_radps[idx];
				state_estimator_autocode_u_.magData.bodyMagVector_uT[idx] = imu_data.mag_ut[idx];
				state_estimator_autocode_u_.magData.isMagDataValid = true;
			}
		}else{
			state_estimator_autocode_u_.magData.isMagDataValid = false;
			imu_updated = false;
		}

		if(baro_sub_.copy(baro_data)){
			state_estimator_autocode_u_.baroData.pressure_pa = baro_data.press_pa;
			state_estimator_autocode_u_.baroData.isBaroDataValid = true;
		}else{
			state_estimator_autocode_u_.baroData.isBaroDataValid = false;
		}

		if(gps_sub_.copy(gps_data)){
			state_estimator_autocode_u_.gpsData.latLonAlt[0] = gps_data.latitude_rad;
			state_estimator_autocode_u_.gpsData.latLonAlt[1] = gps_data.longitude_rad;
			state_estimator_autocode_u_.gpsData.latLonAlt[2] = gps_data.altitude_m;

			state_estimator_autocode_u_.gpsData.nedVel_mps[0] = gps_data.vn_mps;
			state_estimator_autocode_u_.gpsData.nedVel_mps[1] = gps_data.ve_mps;
			state_estimator_autocode_u_.gpsData.nedVel_mps[2] = gps_data.vd_mps;

			if(gps_data.fix_type >= 2){
				state_estimator_autocode_u_.gpsData.isGpsDataValid = true;
			}else{
				state_estimator_autocode_u_.gpsData.isGpsDataValid = false;
			}

			if(gps_fix_count < 10){
				if(gps_data.fix_type >= 2)
					gps_fix_count++;
			}
			if(gps_fix_count >= 10){
				state_estimator_autocode_u_.gpsData.isGpsInitialized = true;
			}else{
				state_estimator_autocode_u_.gpsData.isGpsInitialized = false;
			}

		}else{
			state_estimator_autocode_u_.gpsData.isGpsDataValid = false;
		}

		// Run one step of the model
		stateEstimatorAutocodeObj_.setExternalInputs(&state_estimator_autocode_u_);
		if(imu_updated){
			stateEstimatorAutocodeObj_.step();
		}
		state_estimator_autocode_y_ = stateEstimatorAutocodeObj_.getExternalOutputs();
		for(size_t idx = 0; idx < 3; idx++){
			ekf_data.euler_rad[idx] = state_estimator_autocode_y_.eulAng_rad[idx];

			ekf_data.bias_corr_body_rates_radps[idx] = state_estimator_autocode_u_.imuData.bodyRates_radps[idx] -
					state_estimator_autocode_y_.states[idx + 11];

			ekf_data.bias_corr_body_accels_mps2[idx] = state_estimator_autocode_y_.bodyAccels_mps2[idx];

			ekf_data.dcm_ned_to_body[idx] = state_estimator_autocode_y_.dcmNedToBody[idx];
			ekf_data.dcm_ned_to_body[idx + 3] = state_estimator_autocode_y_.dcmNedToBody[idx + 3];
			ekf_data.dcm_ned_to_body[idx + 6] = state_estimator_autocode_y_.dcmNedToBody[idx + 6];

			ekf_data.dcm_ned_to_fep[idx] = state_estimator_autocode_y_.dcmNedToFep[idx];
			ekf_data.dcm_ned_to_fep[idx + 3] = state_estimator_autocode_y_.dcmNedToFep[idx + 3];
			ekf_data.dcm_ned_to_fep[idx + 6] = state_estimator_autocode_y_.dcmNedToFep[idx + 6];
		}
		ekf_data.is_mag_valid = state_estimator_autocode_u_.magData.isMagDataValid;
		ekf_data.is_baro_valid = state_estimator_autocode_u_.baroData.isBaroDataValid;
		ekf_data.is_gps_valid = state_estimator_autocode_u_.gpsData.isGpsDataValid;

		memcpy(ekf_data.states, state_estimator_autocode_y_.states, sizeof(float) * 23);

		ekf_data.state_init_pct = state_estimator_autocode_y_.stateEstimatorDebug.stateEstInitPct;
		ekf_data.sm_mode = static_cast<uint8_t>(state_estimator_autocode_y_.stateEstimatorDebug.smMode);
		ekf_pub_.publish(ekf_data);
		end_cycles = DWT->CYCCNT;
		// Wait until the next cycle
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

}

StateEstimator state_estimator_task_instance_;
