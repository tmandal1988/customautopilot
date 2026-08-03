/*
 * state_estimator.cpp
 *
 *  Created on: Apr 9, 2025
 *      Author: tanmay
 */

#include "state_estimator.h"

#include <cstring>

StateEstimator::StateEstimator():
TaskBase("StateEstimatorTask", 11000, osPriorityAboveNormal){
//
}

void StateEstimator::Run(){
	ImuData imu_data = {0};
	BaroData baro_data = {0};
	GpsData gps_data = {0};
	Mtf01pData mtf01p_data = {0};
	EkfData ekf_data = {0};
	Publisher<EkfData> ekf_pub_(TopicID::EKF);

	int blink_counter = 0;
	DEBUG_PRINT("FreeRTOS heap remaining: %u bytes\n", xPortGetFreeHeapSize());

	// Initialize model
	stateEstimatorAutocodeObj_.initialize();
	int gps_fix_count = 0;

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(READ_INTERVAL_MS);

//	UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//	uint32_t used = 2700 - highWaterMark * sizeof(StackType_t);
//	DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//    used, highWaterMark * sizeof(StackType_t), 2700);
	osDelay(500);
	// Initialize the periodic schedule after the startup delay.
	xLastWakeTime = xTaskGetTickCount();
	ConfigurePeriodicMetrics(READ_INTERVAL_MS * 1000U,
			READ_INTERVAL_MS * 1000U);
	bool imu_updated = false;
	bool first_iteration = true;
//	uint32_t elapsedTicks = 0;
	for(;;){
		BeginMetricsCycle();
		const TickType_t actual_start_tick = xTaskGetTickCount();
		const int32_t start_lateness_ticks =
				static_cast<int32_t>(actual_start_tick - xLastWakeTime);

		++ekf_data.task_run_seq;
		if (!first_iteration && start_lateness_ticks >
				static_cast<int32_t>(kAllowedStartLatenessTicks)) {
			++ekf_data.late_start_count;
		}
		first_iteration = false;

		if (++blink_counter >= 100) {
			blink_counter = 0;
//			UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(NULL);
//			uint32_t used = 30000 - highWaterMark * sizeof(StackType_t);
//			DEBUG_PRINT("Used: %lu bytes, Free: %lu bytes (of %d total)\n",
//				   used, highWaterMark * sizeof(StackType_t), 10000);
//			DEBUG_PRINT("Ax: %g, Ay: %g, Az: %g\n", ekf_data.bias_corr_body_accels_mps2[0],
//					ekf_data.bias_corr_body_accels_mps2[1], ekf_data.bias_corr_body_accels_mps2[2]);
//			DEBUG_PRINT("Ax: %g, Ay: %g, Az: %g\n", imu_data.accel_mps2[0],
//					imu_data.accel_mps2[1], imu_data.accel_mps2[2]);

		}

		// Read the latest ICM20948 acceleration and angular-rate sample.
		if (imu_sub_.copy(imu_data)) {
			imu_updated = true;
			for (size_t idx = 0; idx < 3; ++idx) {
				state_estimator_autocode_u_.imuData.bodyAccels_mps2[idx] =
				    imu_data.accel_mps2[idx];
				state_estimator_autocode_u_.imuData.bodyRates_radps[idx] =
				    imu_data.gyro_radps[idx];
			}

			// Check delta time between the last and current IMU samples.
			const float curr_imu_time_s =
			    static_cast<float>(imu_data.timestamp_ms) * 0.001F;
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
			imu_updated = false;
			state_estimator_autocode_u_.imuData.isImuDataValid = false;
		}

		// Consume a magnetometer sample only on a cycle where the EKF will
		// actually step. This prevents losing an IST8310 update during a rare
		// cycle with no new IMU data, and prevents duplicate mag corrections.
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
					state_estimator_autocode_u_.magData.isMagDataValid = true;
				}
			} else {
				for (size_t idx = 0; idx < 3; ++idx) {
					state_estimator_autocode_u_.magData.bodyMagVector_uT[idx] =
					    imu_data.mag_ut[idx];
				}
				state_estimator_autocode_u_.magData.isMagDataValid = true;
			}
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
				ekf_data.is_gps_valid = true;
			}else{
				state_estimator_autocode_u_.gpsData.isGpsDataValid = false;
				ekf_data.is_gps_valid = false;
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
			ekf_data.is_gps_valid = false;
		}

		if(mtf01p_sub_.copy(mtf01p_data)){
			state_estimator_autocode_u_.mtf01pData.distPrecision = mtf01p_data.precision;
			state_estimator_autocode_u_.mtf01pData.distStatus = mtf01p_data.dis_status;
			state_estimator_autocode_u_.mtf01pData.distStrength = mtf01p_data.strength;
			state_estimator_autocode_u_.mtf01pData.dist_m = static_cast<float>(mtf01p_data.distance)*OF_DIST_SCALE;
			state_estimator_autocode_u_.mtf01pData.flowX_radps = static_cast<float>(mtf01p_data.flow_vel_x)*OF_SCALE;
			state_estimator_autocode_u_.mtf01pData.flowY_radps = static_cast<float>(mtf01p_data.flow_vel_y)*OF_SCALE;
			state_estimator_autocode_u_.mtf01pData.flowQuality = mtf01p_data.flow_quality;
			state_estimator_autocode_u_.mtf01pData.flowStatus = mtf01p_data.flow_status;
			state_estimator_autocode_u_.mtf01pData.isMtf01pDataValid = true;

		}else{
			state_estimator_autocode_u_.mtf01pData.isMtf01pDataValid = false;
		}


		// Run one step of the model
		stateEstimatorAutocodeObj_.setExternalInputs(&state_estimator_autocode_u_);
		if(imu_updated){
			stateEstimatorAutocodeObj_.step();
			++ekf_data.ekf_step_seq;
		}
		const auto& state_estimator_autocode_y =
				stateEstimatorAutocodeObj_.getExternalOutputs();
		for(size_t idx = 0; idx < 3; idx++){
			ekf_data.euler_rad[idx] = state_estimator_autocode_y.eulAng_rad[idx];

			ekf_data.bias_corr_body_rates_radps[idx] = state_estimator_autocode_u_.imuData.bodyRates_radps[idx] -
					state_estimator_autocode_y.states[idx + 10];

			ekf_data.bias_corr_body_accels_mps2[idx] = state_estimator_autocode_y.bodyAccels_mps2[idx];

			ekf_data.dcm_ned_to_body[idx] = state_estimator_autocode_y.dcmNedToBody[idx];
			ekf_data.dcm_ned_to_body[idx + 3] = state_estimator_autocode_y.dcmNedToBody[idx + 3];
			ekf_data.dcm_ned_to_body[idx + 6] = state_estimator_autocode_y.dcmNedToBody[idx + 6];

			ekf_data.dcm_ned_to_fep[idx] = state_estimator_autocode_y.dcmNedToFep[idx];
			ekf_data.dcm_ned_to_fep[idx + 3] = state_estimator_autocode_y.dcmNedToFep[idx + 3];
			ekf_data.dcm_ned_to_fep[idx + 6] = state_estimator_autocode_y.dcmNedToFep[idx + 6];

			ekf_data.nedpos_m[idx] = state_estimator_autocode_y.states[idx + 4];
			ekf_data.nedvel_mps[idx] = state_estimator_autocode_y.states[idx + 7];
		}
		ekf_data.is_mag_valid = state_estimator_autocode_u_.magData.isMagDataValid;
		ekf_data.is_baro_valid = state_estimator_autocode_u_.baroData.isBaroDataValid;

		std::memset(ekf_data.states, 0, sizeof(ekf_data.states));
		std::memcpy(ekf_data.states, state_estimator_autocode_y.states,
				sizeof(state_estimator_autocode_y.states));

		ekf_data.state_init_pct = state_estimator_autocode_y.stateEstimatorDebug.stateEstInitPct;
		ekf_data.sm_mode = static_cast<uint8_t>(state_estimator_autocode_y.stateEstimatorDebug.smMode);
		ekf_pub_.publish(ekf_data);
		// Wait until the next cycle
		EndMetricsCycle();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

}

StateEstimator state_estimator_task_instance_;
