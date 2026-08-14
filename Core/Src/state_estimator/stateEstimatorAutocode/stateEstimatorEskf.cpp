//
// File: stateEstimatorEskf.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.53
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Fri Aug 14 07:31:18 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#include "stateEstimatorEskf.h"
#include "stateEstimatorEskf_types.h"
#include "rtwtypes.h"
#include "multiword_types.h"
#include <cstring>
#include <cmath>
#include "norm_NoMIKEmk.h"
#include "applyGpsPosAndVelCorr_wtI6aFgH.h"
#include "norm_94qjDDKI.h"
#include "computEskfMagMeasJac_ecmRY7bq.h"
#include "computeEskfStateJac_tpF0ZWTA.h"
#include "updateEskfCovP_Qn4XLGNE.h"
#include "updateQuatAndResetCovP_o483nOwE.h"
#include "quatMultiply_UkcBdhzN.h"
#include "mrdiv_7fpDxZtR.h"
#include "mrdiv_9ppBIzmt.h"
#include "quatToDcm_smxwJjrc.h"
#include "stateEstimatorEskf_private.h"
#include "uMultiWordEq.h"
#include "uMultiWordGe.h"
#include "uMultiWord2MultiWord.h"
#include "MultiWordSub.h"
#include "sMultiWord2uMultiWordSat.h"
#include "uMultiWordLe.h"
#include "uMultiWordGt.h"
#include "uMultiWordLt.h"

// Named constants for Chart: '<Root>/estimatorStateMachine'
const uint8_T stateEstima_IN_RUN_GPS_NOT_INIT{ 4U };

const uint8_T stateEstimatorE_IN_RUN_GPS_LOST{ 3U };

const uint8_T stateEstimatorE_IN_RUN_INIT_GPS{ 5U };

const uint8_T stateEstimatorEsk_IN_INITIALIZE{ 1U };

const uint8_T stateEstimatorEskf_IN_RUN{ 2U };

// Function for Chart: '<Root>/estimatorStateMachine'
void stateEstimatorEskf::stateEstimatorEskf_INITIALIZE(enumStateEstimateMode
  *mode, real_T latLonAltOut[3], const real32_T *Divide1, const real32_T
  Product[3], const real32_T Divide[3], const busMagData *rtu_magData, const
  busGpsData *rtu_gpsData, const busBaroData *rtu_baroData, const
  busStateEstSmParams *rtu_stateEstSmParams)
{
  *mode = enumStateEstimateMode::INITIALIZE;
  stateEstimatorEskf_DW.resetStates = true;

  // During 'INITIALIZE': '<S5>:1'
  // '<S5>:44:1' sf_internal_predicateOutput = 0 | (isAttInitialized && isPosInitialized ... 
  // '<S5>:44:2'  && isBaroInitialized);
  if (static_cast<boolean_T>(static_cast<boolean_T>
       (stateEstimatorEskf_DW.isAttInitialized &
        stateEstimatorEskf_DW.isPosInitialized) &
       stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S5>:44'
    stateEstimatorEskf_DW.durationCounter_1 = 0U;
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf = stateEstimatorEskf_IN_RUN;

    // Entry 'RUN': '<S5>:43'
    // FULL EKF WITH GPS IS RUNNING
    // '<S5>:43:4' resetStates = false;
    stateEstimatorEskf_DW.resetStates = false;

    // '<S5>:43:5' mode = enumStateEstimateMode.RUN;
    *mode = enumStateEstimateMode::RUN;

    // Chart: '<Root>/estimatorStateMachine'
    // '<S5>:43:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S5>:43:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S5>:43:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S5>:43:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // Product: '<S4>/Product'
    // '<S5>:43:10' latLonAltOut = latLonAltIn;
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];

    // SignalConversion generated from: '<S5>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];

    // Product: '<S9>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[0] = rtu_gpsData->latLonAlt[0];

    // Product: '<S4>/Product'
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];

    // SignalConversion generated from: '<S5>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];

    // Product: '<S9>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[1] = rtu_gpsData->latLonAlt[1];

    // Product: '<S4>/Product'
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];

    // SignalConversion generated from: '<S5>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];

    // Product: '<S9>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

    // '<S5>:43:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S5>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // Chart: '<Root>/estimatorStateMachine'
    // '<S5>:43:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //

    // Chart: '<Root>/estimatorStateMachine' incorporates:
    //   Product: '<S4>/Product'
    //   Product: '<S9>/Divide'
    //   SignalConversion generated from: '<S5>/ SFunction '

    // '<S5>:63:1' sf_internal_predicateOutput = 0 | (isAttInitialized && ~isGpsInitialized &&  isBaroInitialized); 
  } else if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<boolean_T>
               (static_cast<int32_T>(rtu_gpsData->isGpsInitialized) ^ 1) &
               stateEstimatorEskf_DW.isAttInitialized) &
              stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S5>:63'
    stateEstimatorEskf_DW.durationCounter_1_n = 0U;
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
      stateEstima_IN_RUN_GPS_NOT_INIT;

    // Entry 'RUN_GPS_NOT_INIT': '<S5>:62'
    // EKF STARTED RUNNING WITHOUT GPS
    // '<S5>:62:4' resetStates = false;
    stateEstimatorEskf_DW.resetStates = false;

    // '<S5>:62:5' isPosInitialized = false;
    stateEstimatorEskf_DW.isPosInitialized = false;

    // '<S5>:62:6' gpsValidCount = 0;
    stateEstimatorEskf_DW.gpsValidCount = 0U;

    // '<S5>:62:7' mode = enumStateEstimateMode.RUN_GPS_NOT_INIT;
    *mode = enumStateEstimateMode::RUN_GPS_NOT_INIT;

    // '<S5>:62:8' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S5>:62:9' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S5>:62:10' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S5>:62:11' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S5>:62:12' latLonAltOut = latLonAltIn;
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
    latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
    latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
    latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

    // '<S5>:62:13' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S5>:62:14' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // '<S5>:62:15' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //
  } else {
    real32_T imuDelta_idx_0;
    real32_T imuDelta_idx_1;
    real32_T imuDelta_idx_2;
    real32_T imuDelta_idx_3;
    real32_T imuDelta_idx_4;
    real32_T imuDelta_idx_5;

    // Take first n(user specified) data to find the initial attitude, accel and gyro biases and NED 
    // origin lat, lon and alt
    //
    // Compute running mean and bias of IMU data
    // '<S5>:1:48' if ( imuIdx < max(stateEstSmParams.imuInitCount, 1) )
    if (stateEstimatorEskf_DW.imuIdx < std::fmax
        (rtu_stateEstSmParams->imuInitCount, 1.0F)) {
      // '<S5>:1:49' imuIdx = imuIdx + 1;
      stateEstimatorEskf_DW.imuIdx++;

      // '<S5>:1:50' imuDelta = [filtBodyAccelsIn_mps2; filtBodyRatesIn_radps] -  ... 
      // '<S5>:1:51'         imuMean;
      imuDelta_idx_0 = Product[0] - stateEstimatorEskf_DW.imuMean[0];
      imuDelta_idx_3 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0]
        - stateEstimatorEskf_DW.imuMean[3];
      imuDelta_idx_1 = Product[1] - stateEstimatorEskf_DW.imuMean[1];
      imuDelta_idx_4 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1]
        - stateEstimatorEskf_DW.imuMean[4];
      imuDelta_idx_2 = Product[2] - stateEstimatorEskf_DW.imuMean[2];
      imuDelta_idx_5 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2]
        - stateEstimatorEskf_DW.imuMean[5];

      // '<S5>:1:52' imuMean = imuMean + imuDelta / imuIdx;
      stateEstimatorEskf_DW.imuMean[0] += imuDelta_idx_0 /
        stateEstimatorEskf_DW.imuIdx;
      stateEstimatorEskf_DW.imuMean[1] += imuDelta_idx_1 /
        stateEstimatorEskf_DW.imuIdx;
      stateEstimatorEskf_DW.imuMean[2] += imuDelta_idx_2 /
        stateEstimatorEskf_DW.imuIdx;
      stateEstimatorEskf_DW.imuMean[3] += imuDelta_idx_3 /
        stateEstimatorEskf_DW.imuIdx;
      stateEstimatorEskf_DW.imuMean[4] += imuDelta_idx_4 /
        stateEstimatorEskf_DW.imuIdx;
      stateEstimatorEskf_DW.imuMean[5] += imuDelta_idx_5 /
        stateEstimatorEskf_DW.imuIdx;

      // '<S5>:1:53' imuM2 = imuM2 + imuDelta .* ( [filtBodyAccelsIn_mps2; filtBodyRatesIn_radps] - ... 
      // '<S5>:1:54'         imuMean);
      stateEstimatorEskf_DW.imuM2[0] += (Product[0] -
        stateEstimatorEskf_DW.imuMean[0]) * imuDelta_idx_0;
      stateEstimatorEskf_DW.imuM2[1] += (Product[1] -
        stateEstimatorEskf_DW.imuMean[1]) * imuDelta_idx_1;
      stateEstimatorEskf_DW.imuM2[2] += (Product[2] -
        stateEstimatorEskf_DW.imuMean[2]) * imuDelta_idx_2;
      stateEstimatorEskf_DW.imuM2[3] +=
        (stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0] -
         stateEstimatorEskf_DW.imuMean[3]) * imuDelta_idx_3;
      stateEstimatorEskf_DW.imuM2[4] +=
        (stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1] -
         stateEstimatorEskf_DW.imuMean[4]) * imuDelta_idx_4;
      stateEstimatorEskf_DW.imuM2[5] +=
        (stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2] -
         stateEstimatorEskf_DW.imuMean[5]) * imuDelta_idx_5;
    }

    //
    // if (imuIdx >= stateEstSmParams.imuInitCount)
    // if (stateEstSmParams.imuInitCount > 1)
    // compute accel and gyro biases
    // accelBias_mps2 = sqrt( imuM2(1 : 3) / (stateEstSmParams.imuInitCount - 1) ); 
    // gyroBias_radps = sqrt( imuM2(4 : 6) / (stateEstSmParams.imuInitCount - 1) ); 
    // else
    // accelBias_mps2 = [0; 0; 0];
    // gyroBias_radps = [0; 0; 0];
    // end
    //
    // Compute running mean and bias of MAG data
    // '<S5>:1:69' if (isMagDataValid)
    if (rtu_magData->isMagDataValid) {
      // '<S5>:1:70' if( magIdx < max(stateEstSmParams.magInitCount, 1) )
      if (stateEstimatorEskf_DW.magIdx < std::fmax
          (rtu_stateEstSmParams->magInitCount, 1.0F)) {
        // '<S5>:1:71' magIdx = magIdx + 1;
        stateEstimatorEskf_DW.magIdx++;

        // '<S5>:1:72' magDelta = (normMagVecIn_nd - magMean);
        // '<S5>:1:73' magMean = magMean + magDelta / magIdx;
        // '<S5>:1:74' magM2 = magM2 + magDelta .* (normMagVecIn_nd - magMean);
        imuDelta_idx_0 = Divide[0] - stateEstimatorEskf_DW.magMean[0];
        imuDelta_idx_3 = imuDelta_idx_0 / stateEstimatorEskf_DW.magIdx +
          stateEstimatorEskf_DW.magMean[0];
        stateEstimatorEskf_DW.magMean[0] = imuDelta_idx_3;
        stateEstimatorEskf_DW.magM2[0] += (Divide[0] - imuDelta_idx_3) *
          imuDelta_idx_0;
        imuDelta_idx_0 = Divide[1] - stateEstimatorEskf_DW.magMean[1];
        imuDelta_idx_3 = imuDelta_idx_0 / stateEstimatorEskf_DW.magIdx +
          stateEstimatorEskf_DW.magMean[1];
        stateEstimatorEskf_DW.magMean[1] = imuDelta_idx_3;
        stateEstimatorEskf_DW.magM2[1] += (Divide[1] - imuDelta_idx_3) *
          imuDelta_idx_0;
        imuDelta_idx_0 = Divide[2] - stateEstimatorEskf_DW.magMean[2];
        imuDelta_idx_3 = imuDelta_idx_0 / stateEstimatorEskf_DW.magIdx +
          stateEstimatorEskf_DW.magMean[2];
        stateEstimatorEskf_DW.magMean[2] = imuDelta_idx_3;
        stateEstimatorEskf_DW.magM2[2] += (Divide[2] - imuDelta_idx_3) *
          imuDelta_idx_0;
      }

      //
      // '<S5>:1:77' if(magIdx >= stateEstSmParams.magInitCount)
      if (stateEstimatorEskf_DW.magIdx >= rtu_stateEstSmParams->magInitCount) {
        real32_T initialQuat_tmp;
        real32_T initialQuat_tmp_0;

        // if(stateEstSmParams.magInitCount > 1)
        // compute mag bias
        // magBias_nd = sqrt( magM2 / (stateEstSmParams.magInitCount - 1) );
        // else
        // magBias_nd = [0; 0; 0];
        //  end
        // '<S5>:1:84' [initialQuat, nedMagVecNorm_nd] = computeInitialAttitude(imuMean(1:3), magMean, stateEstSmParams.initMagDec_rad); 
        // COMPUTEINITIALATTITUDE Computes the IMU attitude using IMU and mag data 
        //
        // Inputs:
        // bodyAccels_mps2:    3x1 array of sum of accel_[x, y, z] readings
        // magVecNorm_nd:      3x1 unit vector obtained from magnetometer strapped to 
        // the body
        // magDec_rad:         Magnetic Declination
        //
        // Outputs:
        // quat:               4x1 array of attitude represented as
        // quaterion with first entry being the real entry
        // nedMagVecNorm_nd:   Mag unit vector in NED frame
        // 'computeInitialAttitude:14' roll_rad = atan2( -bodyAccels_mps2(2), -bodyAccels_mps2(3) ); 
        imuDelta_idx_3 = std::atan2(-stateEstimatorEskf_DW.imuMean[1],
          -stateEstimatorEskf_DW.imuMean[2]);

        // 'computeInitialAttitude:15' pitch_rad = atan2( -bodyAccels_mps2(1), norm(bodyAccels_mps2(2:3)) ); 
        imuDelta_idx_0 = 1.29246971E-26F;
        imuDelta_idx_1 = std::abs(stateEstimatorEskf_DW.imuMean[1]);
        if (imuDelta_idx_1 > 1.29246971E-26F) {
          imuDelta_idx_2 = 1.0F;
          imuDelta_idx_0 = imuDelta_idx_1;
        } else {
          imuDelta_idx_4 = imuDelta_idx_1 / 1.29246971E-26F;
          imuDelta_idx_2 = imuDelta_idx_4 * imuDelta_idx_4;
        }

        imuDelta_idx_1 = std::abs(stateEstimatorEskf_DW.imuMean[2]);
        if (imuDelta_idx_1 > imuDelta_idx_0) {
          imuDelta_idx_4 = imuDelta_idx_0 / imuDelta_idx_1;
          imuDelta_idx_2 = imuDelta_idx_2 * imuDelta_idx_4 * imuDelta_idx_4 +
            1.0F;
          imuDelta_idx_0 = imuDelta_idx_1;
        } else {
          imuDelta_idx_4 = imuDelta_idx_1 / imuDelta_idx_0;
          imuDelta_idx_2 += imuDelta_idx_4 * imuDelta_idx_4;
        }

        imuDelta_idx_2 = std::atan2(-stateEstimatorEskf_DW.imuMean[0],
          imuDelta_idx_0 * std::sqrt(imuDelta_idx_2));

        //  Compute corrected magnetometer readings
        // 'computeInitialAttitude:18' cPhi = cos(roll_rad);
        imuDelta_idx_0 = std::cos(imuDelta_idx_3);

        // 'computeInitialAttitude:19' sPhi = sin(roll_rad);
        imuDelta_idx_1 = std::sin(imuDelta_idx_3);

        // 'computeInitialAttitude:20' sTheta = sin(pitch_rad);
        imuDelta_idx_4 = std::sin(imuDelta_idx_2);

        // 'computeInitialAttitude:21' cTheta = cos(pitch_rad);
        // 'computeInitialAttitude:22' yaw_rad = atan2(-magVecNorm_nd(2) * cPhi + magVecNorm_nd(3) * sPhi, ... 
        // 'computeInitialAttitude:23'                     magVecNorm_nd(1) * cTheta + magVecNorm_nd(2) * sPhi * sTheta + ... 
        // 'computeInitialAttitude:24'                     magVecNorm_nd(3) * cPhi * sTheta) + magDec_rad; 
        imuDelta_idx_1 = std::atan2(-stateEstimatorEskf_DW.magMean[1] *
          imuDelta_idx_0 + stateEstimatorEskf_DW.magMean[2] * imuDelta_idx_1,
          (stateEstimatorEskf_DW.magMean[1] * imuDelta_idx_1 * imuDelta_idx_4 +
           stateEstimatorEskf_DW.magMean[0] * std::cos(imuDelta_idx_2)) +
          stateEstimatorEskf_DW.magMean[2] * imuDelta_idx_0 * imuDelta_idx_4) +
          rtu_stateEstSmParams->initMagDec_rad;

        //  Compute half-angles
        // 'computeInitialAttitude:27' cHalfPhi= cos(roll_rad/2);
        imuDelta_idx_3 /= 2.0F;
        imuDelta_idx_0 = std::cos(imuDelta_idx_3);

        // 'computeInitialAttitude:28' sHalfPhi = sin(roll_rad/2);
        imuDelta_idx_3 = std::sin(imuDelta_idx_3);

        // 'computeInitialAttitude:32' cHalfTheta = cos(pitch_rad/2);
        imuDelta_idx_2 /= 2.0F;
        imuDelta_idx_4 = std::cos(imuDelta_idx_2);

        // 'computeInitialAttitude:33' sHalfTheta = sin(pitch_rad/2);
        imuDelta_idx_2 = std::sin(imuDelta_idx_2);

        // 'computeInitialAttitude:35' cHalfPsi = cos(yaw_rad/2);
        imuDelta_idx_1 /= 2.0F;
        imuDelta_idx_5 = std::cos(imuDelta_idx_1);

        // 'computeInitialAttitude:36' sHalfPsi = sin(yaw_rad/2);
        imuDelta_idx_1 = std::sin(imuDelta_idx_1);

        //  Compute quaternion components
        // 'computeInitialAttitude:39' q0 = cHalfPsi * cHalfTheta * cHalfPhi + sHalfPsi * sHalfTheta * sHalfPhi; 
        // 'computeInitialAttitude:40' q1 = cHalfPsi * cHalfTheta * sHalfPhi - sHalfPsi * sHalfTheta * cHalfPhi; 
        // 'computeInitialAttitude:41' q2 = cHalfPsi * sHalfTheta * cHalfPhi + sHalfPsi * cHalfTheta * sHalfPhi; 
        // 'computeInitialAttitude:42' q3 = sHalfPsi * cHalfTheta * cHalfPhi - cHalfPsi * sHalfTheta * sHalfPhi; 
        // 'computeInitialAttitude:43' quat = [q0; q1; q2; q3];
        //  Direction Cosine Matrix (DCM) from body cooridinates to NED coordinates 
        //  expressed using quaternions.
        // 'computeInitialAttitude:47' C_b2ned=[1-2*(q2^2+q3^2), 2*(q1*q2-q3*q0), 2*(q1*q3+q2*q0); 
        // 'computeInitialAttitude:48'          2*(q1*q2+q3*q0), 1-2*(q1^2+q3^2), 2*(q2*q3-q1*q0); 
        // 'computeInitialAttitude:49'          2*(q1*q3-q2*q0), 2*(q2*q3+q1*q0), 1-2*(q1^2+q2^2)]; 
        // 'computeInitialAttitude:50' nedMagVecNorm_nd = C_b2ned * magVecNorm_nd; 
        initialQuat_tmp = imuDelta_idx_5 * imuDelta_idx_4;
        initialQuat_tmp_0 = imuDelta_idx_1 * imuDelta_idx_2;
        stateEstimatorEskf_DW.initialQuat[0] = initialQuat_tmp * imuDelta_idx_0
          + initialQuat_tmp_0 * imuDelta_idx_3;
        stateEstimatorEskf_DW.initialQuat[1] = initialQuat_tmp * imuDelta_idx_3
          - initialQuat_tmp_0 * imuDelta_idx_0;
        initialQuat_tmp = imuDelta_idx_1 * imuDelta_idx_4;
        initialQuat_tmp_0 = imuDelta_idx_5 * imuDelta_idx_2;
        stateEstimatorEskf_DW.initialQuat[2] = initialQuat_tmp_0 *
          imuDelta_idx_0 + initialQuat_tmp * imuDelta_idx_3;
        stateEstimatorEskf_DW.initialQuat[3] = initialQuat_tmp * imuDelta_idx_0
          - initialQuat_tmp_0 * imuDelta_idx_3;

        // '<S5>:1:85' isAttInitialized = true;
        stateEstimatorEskf_DW.isAttInitialized = true;
      }
    }

    //
    // Compute running mean of GPS data for NED origin Lat, Lon and Alt
    // '<S5>:1:90' if (isGpsDataValid && isGpsInitialized)
    if (static_cast<boolean_T>(rtu_gpsData->isGpsDataValid &
         rtu_gpsData->isGpsInitialized)) {
      // '<S5>:1:91' if( gpsIdx < max(stateEstSmParams.gpsInitCount, 1) )
      if (stateEstimatorEskf_DW.gpsIdx < std::fmax
          (rtu_stateEstSmParams->gpsInitCount, 1.0F)) {
        // '<S5>:1:92' gpsIdx = gpsIdx + 1;
        stateEstimatorEskf_DW.gpsIdx++;

        // '<S5>:1:93' llhDelta = (latLonAltIn - refLatLonAlt);
        // '<S5>:1:94' refLatLonAlt = refLatLonAlt + llhDelta/gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[0] += (rtu_gpsData->latLonAlt[0] -
          stateEstimatorEskf_DW.refLatLonAlt[0]) / stateEstimatorEskf_DW.gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[1] += (rtu_gpsData->latLonAlt[1] -
          stateEstimatorEskf_DW.refLatLonAlt[1]) / stateEstimatorEskf_DW.gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[2] += (rtu_gpsData->latLonAlt[2] -
          stateEstimatorEskf_DW.refLatLonAlt[2]) / stateEstimatorEskf_DW.gpsIdx;
      }

      //
      // '<S5>:1:97' if(gpsIdx >= stateEstSmParams.gpsInitCount)
      if (stateEstimatorEskf_DW.gpsIdx >= rtu_stateEstSmParams->gpsInitCount) {
        // '<S5>:1:98' isPosInitialized = true;
        stateEstimatorEskf_DW.isPosInitialized = true;
      }
    }

    //
    // Compute running mean and bias of baro data
    // '<S5>:1:103' if (isBaroDataValid)
    if (rtu_baroData->isBaroDataValid) {
      // '<S5>:1:104' if( baroIdx < max(stateEstSmParams.baroInitCount, 1) )
      if (stateEstimatorEskf_DW.baroIdx < std::fmax
          (rtu_stateEstSmParams->baroInitCount, 1.0F)) {
        // '<S5>:1:105' baroIdx = baroIdx + 1;
        stateEstimatorEskf_DW.baroIdx++;

        // '<S5>:1:106' baroInitAltDelta = (baroPressAlt_m - baroInitAltMean);
        imuDelta_idx_3 = *Divide1 - stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:1:107' baroInitAltMean = baroInitAltMean + baroInitAltDelta / baroIdx; 
        stateEstimatorEskf_DW.baroInitAltMean += imuDelta_idx_3 /
          stateEstimatorEskf_DW.baroIdx;

        // '<S5>:1:108' baroInitAltM2 = baroInitAltM2 + baroInitAltDelta .* (baroPressAlt_m - baroInitAltMean); 
        stateEstimatorEskf_DW.baroInitAltM2 += (*Divide1 -
          stateEstimatorEskf_DW.baroInitAltMean) * imuDelta_idx_3;
      }

      //
      // '<S5>:1:111' if(baroIdx >= stateEstSmParams.baroInitCount)
      if (stateEstimatorEskf_DW.baroIdx >= rtu_stateEstSmParams->baroInitCount)
      {
        // if(stateEstSmParams.baroInitCount > 1)
        // compute baro bias
        // baroBias_m = sqrt( baroInitAltM2 / (stateEstSmParams.baroInitCount - 1) ); 
        // else
        // baroBias_m = 0;
        //  end
        // '<S5>:1:118' isBaroInitialized = true;
        stateEstimatorEskf_DW.isBaroInitialized = true;
      }
    }

    // '<S5>:1:121' if(isGpsInitialized)
    if (rtu_gpsData->isGpsInitialized) {
      // Status of the initialization
      // '<S5>:1:123' stateEstInitPct = (imuIdx + magIdx + gpsIdx + baroIdx) / (stateEstSmParams.imuInitCount + ... 
      // '<S5>:1:124'         stateEstSmParams.magInitCount + stateEstSmParams.gpsInitCount +  ... 
      // '<S5>:1:125'         stateEstSmParams.baroInitCount) * 100;
      stateEstimatorEskf_DW.stateEstInitPct = (((stateEstimatorEskf_DW.imuIdx +
        stateEstimatorEskf_DW.magIdx) + static_cast<real32_T>
        (stateEstimatorEskf_DW.gpsIdx)) + stateEstimatorEskf_DW.baroIdx) /
        (((rtu_stateEstSmParams->imuInitCount +
           rtu_stateEstSmParams->magInitCount) +
          rtu_stateEstSmParams->gpsInitCount) +
         rtu_stateEstSmParams->baroInitCount) * 100.0F;
    } else {
      // '<S5>:1:126' else
      // Status of the initialization
      // '<S5>:1:128' stateEstInitPct = (imuIdx + magIdx + baroIdx) / (stateEstSmParams.imuInitCount + ... 
      // '<S5>:1:129'         stateEstSmParams.magInitCount + stateEstSmParams.baroInitCount) * 100; 
      stateEstimatorEskf_DW.stateEstInitPct = ((stateEstimatorEskf_DW.imuIdx +
        stateEstimatorEskf_DW.magIdx) + stateEstimatorEskf_DW.baroIdx) /
        ((rtu_stateEstSmParams->imuInitCount +
          rtu_stateEstSmParams->magInitCount) +
         rtu_stateEstSmParams->baroInitCount) * 100.0F;
    }

    //
    // '<S5>:1:133' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S5>:1:134' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S5>:1:135' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S5>:1:136' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S5>:1:137' latLonAltOut = latLonAltIn;
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
    latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
    latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
    latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

    // '<S5>:1:138' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S5>:1:139' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S5>:1:140' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //
    // '<S5>:1:143' initialStates = [initialQuat; 0; 0; 0; 0; 0; 0; gyroBias_radps; accelBias_mps2; magBias_nd; baroBias_m]; 
    stateEstimatorEskf_DW.initialStates[0] = stateEstimatorEskf_DW.initialQuat[0];
    stateEstimatorEskf_DW.initialStates[1] = stateEstimatorEskf_DW.initialQuat[1];
    stateEstimatorEskf_DW.initialStates[2] = stateEstimatorEskf_DW.initialQuat[2];
    stateEstimatorEskf_DW.initialStates[3] = stateEstimatorEskf_DW.initialQuat[3];
    stateEstimatorEskf_DW.initialStates[4] = 0.0F;
    stateEstimatorEskf_DW.initialStates[5] = 0.0F;
    stateEstimatorEskf_DW.initialStates[6] = 0.0F;
    stateEstimatorEskf_DW.initialStates[7] = 0.0F;
    stateEstimatorEskf_DW.initialStates[8] = 0.0F;
    stateEstimatorEskf_DW.initialStates[9] = 0.0F;
    stateEstimatorEskf_DW.initialStates[10] =
      stateEstimatorEskf_DW.gyroBias_radps[0];
    stateEstimatorEskf_DW.initialStates[13] =
      stateEstimatorEskf_DW.accelBias_mps2[0];
    stateEstimatorEskf_DW.initialStates[16] = stateEstimatorEskf_DW.magBias_nd[0];
    stateEstimatorEskf_DW.initialStates[11] =
      stateEstimatorEskf_DW.gyroBias_radps[1];
    stateEstimatorEskf_DW.initialStates[14] =
      stateEstimatorEskf_DW.accelBias_mps2[1];
    stateEstimatorEskf_DW.initialStates[17] = stateEstimatorEskf_DW.magBias_nd[1];
    stateEstimatorEskf_DW.initialStates[12] =
      stateEstimatorEskf_DW.gyroBias_radps[2];
    stateEstimatorEskf_DW.initialStates[15] =
      stateEstimatorEskf_DW.accelBias_mps2[2];
    stateEstimatorEskf_DW.initialStates[18] = stateEstimatorEskf_DW.magBias_nd[2];
    stateEstimatorEskf_DW.initialStates[19] = stateEstimatorEskf_DW.baroBias_m;

    // Compute Body To NED DCM
    // '<S5>:1:145' initialDcmBodyToNed = quatToDcm_function(initialStates(1:4)); 
    // Quaternions
    // 'quatToDcm_function:3' q0 = quat(1);
    // 'quatToDcm_function:4' q1 = quat(2);
    // 'quatToDcm_function:5' q2 = quat(3);
    // 'quatToDcm_function:6' q3 = quat(4);
    //  Direction Cosine Matrix (DCM) from body cooridinates to NED coordinates
    //  expressed using quaternions.
    // 'quatToDcm_function:10' dcmBodyToNed = [1-2*(q2^2+q3^2), 2*(q1*q2-q3*q0), 2*(q1*q3+q2*q0); 
    // 'quatToDcm_function:11'     2*(q1*q2+q3*q0), 1-2*(q1^2+q3^2), 2*(q2*q3-q1*q0); 
    // 'quatToDcm_function:12'     2*(q1*q3-q2*q0), 2*(q2*q3+q1*q0), 1-2*(q1^2+q2^2)]; 
    imuDelta_idx_0 = stateEstimatorEskf_DW.initialStates[3] *
      stateEstimatorEskf_DW.initialStates[3];
    imuDelta_idx_3 = stateEstimatorEskf_DW.initialStates[2] *
      stateEstimatorEskf_DW.initialStates[2];
    stateEstimatorEskf_DW.initialDcmBodyToNed[0] = 1.0F - (imuDelta_idx_3 +
      imuDelta_idx_0) * 2.0F;
    imuDelta_idx_1 = stateEstimatorEskf_DW.initialStates[1] *
      stateEstimatorEskf_DW.initialStates[2];
    imuDelta_idx_4 = stateEstimatorEskf_DW.initialStates[0] *
      stateEstimatorEskf_DW.initialStates[3];
    stateEstimatorEskf_DW.initialDcmBodyToNed[3] = (imuDelta_idx_1 -
      imuDelta_idx_4) * 2.0F;
    imuDelta_idx_2 = stateEstimatorEskf_DW.initialStates[1] *
      stateEstimatorEskf_DW.initialStates[3];
    imuDelta_idx_5 = stateEstimatorEskf_DW.initialStates[0] *
      stateEstimatorEskf_DW.initialStates[2];
    stateEstimatorEskf_DW.initialDcmBodyToNed[6] = (imuDelta_idx_2 +
      imuDelta_idx_5) * 2.0F;
    stateEstimatorEskf_DW.initialDcmBodyToNed[1] = (imuDelta_idx_1 +
      imuDelta_idx_4) * 2.0F;
    imuDelta_idx_1 = stateEstimatorEskf_DW.initialStates[1] *
      stateEstimatorEskf_DW.initialStates[1];
    stateEstimatorEskf_DW.initialDcmBodyToNed[4] = 1.0F - (imuDelta_idx_1 +
      imuDelta_idx_0) * 2.0F;
    imuDelta_idx_0 = stateEstimatorEskf_DW.initialStates[2] *
      stateEstimatorEskf_DW.initialStates[3];
    imuDelta_idx_4 = stateEstimatorEskf_DW.initialStates[0] *
      stateEstimatorEskf_DW.initialStates[1];
    stateEstimatorEskf_DW.initialDcmBodyToNed[7] = (imuDelta_idx_0 -
      imuDelta_idx_4) * 2.0F;
    stateEstimatorEskf_DW.initialDcmBodyToNed[2] = (imuDelta_idx_2 -
      imuDelta_idx_5) * 2.0F;
    stateEstimatorEskf_DW.initialDcmBodyToNed[5] = (imuDelta_idx_0 +
      imuDelta_idx_4) * 2.0F;
    stateEstimatorEskf_DW.initialDcmBodyToNed[8] = 1.0F - (imuDelta_idx_1 +
      imuDelta_idx_3) * 2.0F;
  }
}

// Function for Chart: '<Root>/estimatorStateMachine'
void stateEstimatorEskf::state_enter_atomic_RUN_INIT_GPS(enumStateEstimateMode
  *mode, real_T latLonAltOut[3], const real32_T *Divide1, const real32_T
  Product[3], const real32_T Divide[3], const busMagData *rtu_magData, const
  busGpsData *rtu_gpsData, const busBaroData *rtu_baroData)
{
  // Entry 'RUN_INIT_GPS': '<S5>:64'
  // EKF RUNNING WITHOUT GPS BUT WE HAVE HEALTHY GPS SIGNAL
  // START INITIALIZING GPS
  // '<S5>:64:5' gpsValidCount = 0;
  stateEstimatorEskf_DW.gpsValidCount = 0U;

  //  Index to keep track of how many gps readings we have summed
  //  so far
  // '<S5>:64:8' gpsIdx = 0;
  stateEstimatorEskf_DW.gpsIdx = 0.0;

  // '<S5>:64:9' refLatLonAlt = [0; 0; 0];
  // '<S5>:64:10' isPosInitialized = false;
  stateEstimatorEskf_DW.isPosInitialized = false;

  // '<S5>:64:11' mode = enumStateEstimateMode.RUN_INIT_GPS;
  *mode = enumStateEstimateMode::RUN_INIT_GPS;

  // Chart: '<Root>/estimatorStateMachine'
  // '<S5>:64:12' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
  // '<S5>:64:13' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
  // '<S5>:64:14' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
  // '<S5>:64:15' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
    rtu_magData->isMagDataValid;

  // '<S5>:64:16' latLonAltOut = latLonAltIn;
  stateEstimatorEskf_DW.refLatLonAlt[0] = 0.0;

  // Product: '<S4>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];

  // SignalConversion generated from: '<S5>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];

  // Product: '<S9>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
  stateEstimatorEskf_DW.refLatLonAlt[1] = 0.0;

  // Product: '<S4>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];

  // SignalConversion generated from: '<S5>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];

  // Product: '<S9>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
  stateEstimatorEskf_DW.refLatLonAlt[2] = 0.0;

  // Product: '<S4>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];

  // SignalConversion generated from: '<S5>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];

  // Product: '<S9>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

  // '<S5>:64:17' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
    rtu_gpsData->isGpsDataValid;

  // '<S5>:64:18' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
  stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
    stateEstimatorEskf_DW.baroInitAltMean;

  // Chart: '<Root>/estimatorStateMachine'
  // '<S5>:64:19' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
    rtu_baroData->isBaroDataValid;

  //
}

//
// Function for MATLAB Function: '<S13>/DelayedHorizonBufferManager'
// function [magReady, magTimeOut_ms, magOut_uT, status, countOut] = ...
//     magFifo(isMagValid, magTimeIn_ms, magIn_uT, fusionTime_ms, ...
//             reset, magFifoParams)
//
//  Magnetometer fixed-memory FIFO for delayed-horizon ESKF fusion.
//
//  Runtime inputs:
//    isMagValid      : boolean pulse. True for one model step when a NEW
//                      valid magnetometer measurement is available.
//    magTimeIn_ms    : uint64 corrected magnetometer measurement epoch [ms].
//    magIn_uT        : single [3x1] magnetic field [uT]
//                      [Bx; By; Bz]
//    fusionTime_ms   : uint64 delayed ESKF fusion horizon [ms].
//    reset           : boolean. Clears FIFO and timestamp history.
//    magFifoParams   : read-only configuration struct.
//
//  Required magFifoParams fields:
//    magCapacity              : uint16
//    magMaxAge_ms             : uint64
//    magMinObsInterval_ms     : uint64
//    magTimeResetThreshold_ms : uint64
//
//  Outputs:
//    magReady       : true when a magnetometer measurement is returned.
//    magTimeOut_ms  : timestamp of returned measurement.
//    magOut_uT      : returned [Bx; By; Bz] magnetic field [uT].
//    status         : diagnostic status code.
//    countOut       : number of measurements remaining in FIFO.
//
//  FIFO behavior:
//    - Static allocation, no dynamic memory.
//    - Newest measurement overwrites oldest when FIFO is full.
//    - Duplicate/small backward timestamps are rejected.
//    - Large backward timestamp jump resets the magnetometer time epoch.
//    - Measurements arriving too quickly can be throttled.
//    - POP returns the newest measurement satisfying:
//
//          magTime <= fusionTime_ms
//
//    - Older eligible measurements are discarded.
//    - Selected measurements older than magMaxAge_ms are discarded.
//
//  Important:
//    isMagValid must behave as a NEW-DATA pulse. It must not remain true
//    continuously while the same magnetometer sample is held at the inputs.
//
void stateEstimatorEskf::stateEstimatorEskf_magFifo(boolean_T isMagValid, const
  uint64m_T magTimeIn_ms, const real32_T magIn_uT[3], const uint64m_T
  fusionTime_ms, boolean_T reset, uint16_T magFifoParams_capacity, const
  uint64m_T magFifoParams_minInterval_ms, const uint64m_T
  magFifoParams_maxAge_ms, const uint64m_T magFifoParams_resetThreshold_ms,
  boolean_T *magReady, uint64m_T *magTimeOut_ms, real32_T magOut_uT[3], uint8_T *
  status, uint16_T *countOut)
{
  int96m_T tmp_0;
  int96m_T tmp_1;
  int96m_T tmp_2;
  uint64m_T tmp;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  static const uint64m_T tmp_5{ { 0U, 0U }// chunks
  };

  // 'magFifo:48' MAX_SIZE = 64;
  // 'magFifo:50' STATUS_OK             = uint8(0);
  // 'magFifo:51' STATUS_BAD_TIMESTAMP  = uint8(1);
  // 'magFifo:52' STATUS_OVERWRITE      = uint8(2);
  // 'magFifo:53' STATUS_TIMEBASE_RESET = uint8(3);
  // 'magFifo:54' STATUS_STALE_DISCARD  = uint8(4);
  // 'magFifo:55' STATUS_BAD_CAPACITY   = uint8(5);
  //  Static FIFO storage/state initialization.
  // 'magFifo:62' if isempty(head)
  //  Default outputs.
  // 'magFifo:73' magReady = false;
  *magReady = false;

  // 'magFifo:74' magTimeOut_ms = uint64(0);
  *magTimeOut_ms = tmp_5;

  // 'magFifo:75' magOut_uT = zeros(3,1,'single');
  magOut_uT[0] = 0.0F;
  magOut_uT[1] = 0.0F;
  magOut_uT[2] = 0.0F;

  // 'magFifo:76' status = STATUS_OK;
  *status = 0U;

  // 'magFifo:77' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_b;

  //  Read-only configuration aliases.
  // 'magFifo:80' capacity = magFifoParams.capacity;
  // 'magFifo:81' maxAge_ms = magFifoParams.maxAge_ms;
  // 'magFifo:82' minObsInterval_ms = magFifoParams.minInterval_ms;
  // 'magFifo:83' timeResetThreshold_ms = magFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'magFifo:86' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((magFifoParams_capacity < 1) |
       (magFifoParams_capacity > 64))) {
    // 'magFifo:87' status = STATUS_BAD_CAPACITY;
    *status = 5U;

    //  Explicit estimator/FIFO reset.
    // 'magFifo:92' if reset
  } else if (reset) {
    // 'magFifo:93' head = uint16(1);
    stateEstimatorEskf_DW.head_m = 1U;

    // 'magFifo:94' tail = uint16(1);
    stateEstimatorEskf_DW.tail_n = 1U;

    // 'magFifo:95' count = uint16(0);
    stateEstimatorEskf_DW.count_b = 0U;

    // 'magFifo:96' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms = tmp_5;

    // 'magFifo:97' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime = false;

    // 'magFifo:98' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_o_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW MAGNETOMETER MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'magFifo:105' if isMagValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isMagValid) {
      //  Magnetometer timestamps must normally increase monotonically.
      // 'magFifo:107' if haveLastPushTime && magTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime &
           uMultiWordLe(&magTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms.chunks[0U], 2)))
      {
        // 'magFifo:108' backwardsJump_ms = lastPushTime_ms - magTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'magFifo:111' if timeResetThreshold_ms > 0 && ...
        // 'magFifo:112'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&magTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&magFifoParams_resetThreshold_ms.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &magFifoParams_resetThreshold_ms.chunks[0U], 2))) {
          // 'magFifo:113' head = uint16(1);
          stateEstimatorEskf_DW.head_m = 1U;

          // 'magFifo:114' tail = uint16(1);
          stateEstimatorEskf_DW.tail_n = 1U;

          // 'magFifo:115' count = uint16(0);
          stateEstimatorEskf_DW.count_b = 0U;

          // 'magFifo:116' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms = tmp_5;

          // 'magFifo:117' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime = false;

          // 'magFifo:118' status = STATUS_TIMEBASE_RESET;
          *status = 3U;
          guard3 = true;
        } else {
          // 'magFifo:119' else
          //  Duplicate timestamp or small backward jump.
          // 'magFifo:121' status = STATUS_BAD_TIMESTAMP;
          *status = 1U;
        }
      } else {
        guard3 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      //  Optional rate throttling.
      //  minObsInterval_ms == 0 disables throttling.
      // 'magFifo:128' if haveLastPushTime && ...
      // 'magFifo:129'             (magTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&magTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms.chunks[0U], 2,
                           &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime &
           uMultiWordLt(&tmp_3.chunks[0U], &magFifoParams_minInterval_ms.chunks
                        [0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'magFifo:134' bufferWasFull = (count == capacity);
        //  Store complete 3-axis magnetic-field observation.
        //  Each column contains [Bx; By; Bz] in uT.
        // 'magFifo:138' timeBuf_ms(head) = magTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.head_m - 1] =
          magTimeIn_ms;

        // 'magFifo:139' dataBuf(:,head) = magIn_uT;
        dataBuf_o_tmp = (stateEstimatorEskf_DW.head_m - 1) * 3;
        stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp] = magIn_uT[0];
        stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp + 1] = magIn_uT[1];
        stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp + 2] = magIn_uT[2];

        //  Advance write location.
        // 'magFifo:142' if head == capacity
        if (stateEstimatorEskf_DW.head_m == magFifoParams_capacity) {
          // 'magFifo:143' head = uint16(1);
          stateEstimatorEskf_DW.head_m = 1U;
        } else {
          // 'magFifo:144' else
          // 'magFifo:145' head = head + 1;
          qY = stateEstimatorEskf_DW.head_m + 1U;
          if (stateEstimatorEskf_DW.head_m + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_m = static_cast<uint16_T>(qY);
        }

        // 'magFifo:148' if bufferWasFull
        if (stateEstimatorEskf_DW.count_b == magFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'magFifo:151' if tail == capacity
          if (stateEstimatorEskf_DW.tail_n == magFifoParams_capacity) {
            // 'magFifo:152' tail = uint16(1);
            stateEstimatorEskf_DW.tail_n = 1U;
          } else {
            // 'magFifo:153' else
            // 'magFifo:154' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_n + 1U;
            if (stateEstimatorEskf_DW.tail_n + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_n = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'magFifo:158' count = capacity;
          stateEstimatorEskf_DW.count_b = magFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'magFifo:161' if status == STATUS_OK
          if (*status == 0) {
            // 'magFifo:162' status = STATUS_OVERWRITE;
            *status = 2U;
          }
        } else {
          // 'magFifo:164' else
          //  One new measurement was added without replacing anything.
          // 'magFifo:166' count = count + 1;
          qY = stateEstimatorEskf_DW.count_b + 1U;
          if (stateEstimatorEskf_DW.count_b + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_b = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'magFifo:170' lastPushTime_ms = magTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms = magTimeIn_ms;

        // 'magFifo:171' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime = true;

        // 'magFifo:172' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_b;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST MAGNETOMETER MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'magFifo:180' if count == 0
      if ((stateEstimatorEskf_DW.count_b == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_n - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'magFifo:186' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ magTime | magTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'magFifo:197' for k = 1:(MAX_SIZE-1)
        dataBuf_o_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_o_tmp < 63) &
                 (stateEstimatorEskf_DW.count_b != 1)))) {
          uint16_T nextIdx;

          // 'magFifo:198' if count == 1
          // 'magFifo:202' if tail == capacity
          if (stateEstimatorEskf_DW.tail_n == magFifoParams_capacity) {
            // 'magFifo:203' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'magFifo:204' else
            // 'magFifo:205' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_n + 1U;
            if (stateEstimatorEskf_DW.tail_n + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'magFifo:208' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_c[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'magFifo:213' tail = nextIdx;
            stateEstimatorEskf_DW.tail_n = nextIdx;

            // 'magFifo:214' count = count - 1;
            qY = stateEstimatorEskf_DW.count_b -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_b - 1U >
                stateEstimatorEskf_DW.count_b) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_b = static_cast<uint16_T>(qY);
            dataBuf_o_tmp++;
          }
        }

        //  Tail now points to the newest eligible magnetometer measurement.
        // 'magFifo:218' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'magFifo:222' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&magFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_n -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &magFifoParams_maxAge_ms.chunks[0U],
                           2)) {
            // 'magFifo:223' if tail == capacity
            if (stateEstimatorEskf_DW.tail_n == magFifoParams_capacity) {
              // 'magFifo:224' tail = uint16(1);
              stateEstimatorEskf_DW.tail_n = 1U;
            } else {
              // 'magFifo:225' else
              // 'magFifo:226' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_n + 1U;
              if (stateEstimatorEskf_DW.tail_n + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_n = static_cast<uint16_T>(qY);
            }

            // 'magFifo:229' count = count - 1;
            qY = stateEstimatorEskf_DW.count_b -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_b - 1U >
                stateEstimatorEskf_DW.count_b) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_b = static_cast<uint16_T>(qY);

            // 'magFifo:230' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_b;

            // 'magFifo:231' status = STATUS_STALE_DISCARD;
            *status = 4U;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      //  Return selected magnetic-field measurement.
      // 'magFifo:236' magTimeOut_ms = timeBuf_ms(tail);
      *magTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_n - 1];

      // 'magFifo:237' magOut_uT = dataBuf(:,tail);
      dataBuf_o_tmp = (stateEstimatorEskf_DW.tail_n - 1) * 3;
      magOut_uT[0] = stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp];
      magOut_uT[1] = stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp + 1];
      magOut_uT[2] = stateEstimatorEskf_DW.dataBuf_o[dataBuf_o_tmp + 2];

      // 'magFifo:238' magReady = true;
      *magReady = true;

      //  Consume returned measurement.
      // 'magFifo:241' if tail == capacity
      if (stateEstimatorEskf_DW.tail_n == magFifoParams_capacity) {
        // 'magFifo:242' tail = uint16(1);
        stateEstimatorEskf_DW.tail_n = 1U;
      } else {
        // 'magFifo:243' else
        // 'magFifo:244' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_n + 1U;
        if (stateEstimatorEskf_DW.tail_n + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_n = static_cast<uint16_T>(qY);
      }

      // 'magFifo:247' count = count - 1;
      qY = stateEstimatorEskf_DW.count_b -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_b - 1U > stateEstimatorEskf_DW.count_b) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_b = static_cast<uint16_T>(qY);

      // 'magFifo:248' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_b;
    }
  }
}

//
// Function for MATLAB Function: '<S13>/DelayedHorizonBufferManager'
// function [gpsReady, gpsTimeOut_ms, posVelOut, status, countOut] = ...
//     gpsFifo(isGpsValid, gpsTimeIn_ms, posVelIn, fusionTime_ms, ...
//             reset, gpsFifoParams)
//
//  GPS fixed-memory FIFO for delayed-horizon ESKF fusion.
//
//  Runtime inputs:
//    isGpsValid      : boolean pulse. True for one model step when a NEW
//                      valid GPS measurement is available.
//    gpsTimeIn_ms    : uint64 corrected GPS measurement epoch [ms].
//    posVelIn        : single [6x1]
//                      [pN; pE; pD; vN; vE; vD]
//    fusionTime_ms   : uint64 delayed ESKF fusion horizon [ms].
//    reset           : boolean. Clears FIFO and timestamp history.
//    gpsFifoParams   : read-only configuration struct.
//
//  Required gpsFifoParams fields:
//    gpsCapacity              : uint16
//    gpsMaxAge_ms             : uint64
//    gpsMinObsInterval_ms     : uint64
//    gpsTimeResetThreshold_ms : uint64
//
//  Outputs:
//    gpsReady       : true when a GPS measurement is returned for fusion.
//    gpsTimeOut_ms  : timestamp of returned measurement.
//    posVelOut      : returned [pN;pE;pD;vN;vE;vD].
//    status         : diagnostic status code.
//    countOut       : number of measurements remaining in FIFO.
//
//  FIFO behavior:
//    - Static allocation, no dynamic memory.
//    - Newest measurement overwrites oldest when FIFO is full.
//    - Duplicate/small backward timestamps are rejected.
//    - Large backward timestamp jump resets the GPS time epoch.
//    - Measurements arriving too quickly can be throttled.
//    - POP returns the newest measurement satisfying:
//
//          gpsTime <= fusionTime_ms
//
//    - Older eligible measurements are discarded.
//    - Selected measurements older than gpsMaxAge_ms are discarded.
//
//  Important:
//    isGpsValid must behave as a NEW-DATA pulse. It must not remain true
//    continuously while the same GPS sample is held at the inputs.
//
void stateEstimatorEskf::stateEstimatorEskf_gpsFifo(boolean_T isGpsValid, const
  uint64m_T gpsTimeIn_ms, const real32_T posVelIn[6], const uint64m_T
  fusionTime_ms, boolean_T reset, uint16_T gpsFifoParams_capacity, const
  uint64m_T gpsFifoParams_minInterval_ms, const uint64m_T
  gpsFifoParams_maxAge_ms, const uint64m_T gpsFifoParams_resetThreshold_ms,
  boolean_T *gpsReady, uint64m_T *gpsTimeOut_ms, real32_T posVelOut[6], uint8_T *
  status, uint16_T *countOut)
{
  int96m_T tmp_0;
  int96m_T tmp_1;
  int96m_T tmp_2;
  uint64m_T tmp;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  static const uint64m_T tmp_5{ { 0U, 0U }// chunks
  };

  // 'gpsFifo:48' MAX_SIZE = 16;
  // 'gpsFifo:50' STATUS_OK             = uint8(0);
  // 'gpsFifo:51' STATUS_BAD_TIMESTAMP  = uint8(1);
  // 'gpsFifo:52' STATUS_OVERWRITE      = uint8(2);
  // 'gpsFifo:53' STATUS_TIMEBASE_RESET = uint8(3);
  // 'gpsFifo:54' STATUS_STALE_DISCARD  = uint8(4);
  // 'gpsFifo:55' STATUS_BAD_CAPACITY   = uint8(5);
  //  Static FIFO storage/state initialization.
  // 'gpsFifo:62' if isempty(head)
  //  Default outputs.
  // 'gpsFifo:73' gpsReady = false;
  *gpsReady = false;

  // 'gpsFifo:74' gpsTimeOut_ms = uint64(0);
  *gpsTimeOut_ms = tmp_5;

  // 'gpsFifo:75' posVelOut = zeros(6,1,'single');
  posVelOut[0] = 0.0F;
  posVelOut[1] = 0.0F;
  posVelOut[2] = 0.0F;
  posVelOut[3] = 0.0F;
  posVelOut[4] = 0.0F;
  posVelOut[5] = 0.0F;

  // 'gpsFifo:76' status = STATUS_OK;
  *status = 0U;

  // 'gpsFifo:77' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_f;

  //  Read-only configuration aliases.
  // 'gpsFifo:80' capacity = gpsFifoParams.capacity;
  // 'gpsFifo:81' maxAge_ms = gpsFifoParams.maxAge_ms;
  // 'gpsFifo:82' minObsInterval_ms = gpsFifoParams.minInterval_ms;
  // 'gpsFifo:83' timeResetThreshold_ms = gpsFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'gpsFifo:86' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((gpsFifoParams_capacity < 1) |
       (gpsFifoParams_capacity > 16))) {
    // 'gpsFifo:87' status = STATUS_BAD_CAPACITY;
    *status = 5U;

    //  Explicit estimator/FIFO reset.
    // 'gpsFifo:92' if reset
  } else if (reset) {
    // 'gpsFifo:93' head = uint16(1);
    stateEstimatorEskf_DW.head_k = 1U;

    // 'gpsFifo:94' tail = uint16(1);
    stateEstimatorEskf_DW.tail_o = 1U;

    // 'gpsFifo:95' count = uint16(0);
    stateEstimatorEskf_DW.count_f = 0U;

    // 'gpsFifo:96' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_h = tmp_5;

    // 'gpsFifo:97' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_h = false;

    // 'gpsFifo:98' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_ot_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW GPS MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'gpsFifo:105' if isGpsValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isGpsValid) {
      //  GPS timestamps must normally increase monotonically.
      // 'gpsFifo:107' if haveLastPushTime && gpsTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_h &
           uMultiWordLe(&gpsTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_h.chunks[0U], 2)))
      {
        // 'gpsFifo:108' backwardsJump_ms = lastPushTime_ms - gpsTimeIn_ms;
        //  Large backward jump indicates a GPS time-base discontinuity.
        // 'gpsFifo:111' if timeResetThreshold_ms > 0 && ...
        // 'gpsFifo:112'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_h.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&gpsTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&gpsFifoParams_resetThreshold_ms.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &gpsFifoParams_resetThreshold_ms.chunks[0U], 2))) {
          // 'gpsFifo:113' head = uint16(1);
          stateEstimatorEskf_DW.head_k = 1U;

          // 'gpsFifo:114' tail = uint16(1);
          stateEstimatorEskf_DW.tail_o = 1U;

          // 'gpsFifo:115' count = uint16(0);
          stateEstimatorEskf_DW.count_f = 0U;

          // 'gpsFifo:116' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_h = tmp_5;

          // 'gpsFifo:117' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_h = false;

          // 'gpsFifo:118' status = STATUS_TIMEBASE_RESET;
          *status = 3U;
          guard3 = true;
        } else {
          // 'gpsFifo:119' else
          //  Duplicate timestamp or small backward jump.
          // 'gpsFifo:121' status = STATUS_BAD_TIMESTAMP;
          *status = 1U;
        }
      } else {
        guard3 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      //  Optional rate throttling.
      //  minObsInterval_ms == 0 disables throttling.
      // 'gpsFifo:128' if haveLastPushTime && ...
      // 'gpsFifo:129'             (gpsTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&gpsTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_h.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_h &
           uMultiWordLt(&tmp_3.chunks[0U], &gpsFifoParams_minInterval_ms.chunks
                        [0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'gpsFifo:134' bufferWasFull = (count == capacity);
        //  Store complete GPS observation at current write location.
        // 'gpsFifo:137' timeBuf_ms(head) = gpsTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.head_k - 1] =
          gpsTimeIn_ms;

        // 'gpsFifo:138' dataBuf(:,head) = posVelIn;
        dataBuf_ot_tmp = (stateEstimatorEskf_DW.head_k - 1) * 6;
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp] = posVelIn[0];
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 1] = posVelIn[1];
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 2] = posVelIn[2];
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 3] = posVelIn[3];
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 4] = posVelIn[4];
        stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 5] = posVelIn[5];

        //  Advance write location.
        // 'gpsFifo:141' if head == capacity
        if (stateEstimatorEskf_DW.head_k == gpsFifoParams_capacity) {
          // 'gpsFifo:142' head = uint16(1);
          stateEstimatorEskf_DW.head_k = 1U;
        } else {
          // 'gpsFifo:143' else
          // 'gpsFifo:144' head = head + 1;
          qY = stateEstimatorEskf_DW.head_k + 1U;
          if (stateEstimatorEskf_DW.head_k + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_k = static_cast<uint16_T>(qY);
        }

        // 'gpsFifo:147' if bufferWasFull
        if (stateEstimatorEskf_DW.count_f == gpsFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'gpsFifo:150' if tail == capacity
          if (stateEstimatorEskf_DW.tail_o == gpsFifoParams_capacity) {
            // 'gpsFifo:151' tail = uint16(1);
            stateEstimatorEskf_DW.tail_o = 1U;
          } else {
            // 'gpsFifo:152' else
            // 'gpsFifo:153' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_o + 1U;
            if (stateEstimatorEskf_DW.tail_o + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_o = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'gpsFifo:157' count = capacity;
          stateEstimatorEskf_DW.count_f = gpsFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'gpsFifo:160' if status == STATUS_OK
          if (*status == 0) {
            // 'gpsFifo:161' status = STATUS_OVERWRITE;
            *status = 2U;
          }
        } else {
          // 'gpsFifo:163' else
          //  One new measurement was added without replacing anything.
          // 'gpsFifo:165' count = count + 1;
          qY = stateEstimatorEskf_DW.count_f + 1U;
          if (stateEstimatorEskf_DW.count_f + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_f = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'gpsFifo:169' lastPushTime_ms = gpsTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_h = gpsTimeIn_ms;

        // 'gpsFifo:170' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_h = true;

        // 'gpsFifo:171' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_f;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST GPS MEASUREMENT ELIGIBLE AT THE FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'gpsFifo:179' if count == 0
      if ((stateEstimatorEskf_DW.count_f == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_o - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Because measurements are chronologically ordered, if the oldest
        //  measurement is newer than the fusion horizon, nothing is ready.
        // 'gpsFifo:185' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ gpsTime | gpsTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'gpsFifo:196' for k = 1:(MAX_SIZE-1)
        dataBuf_ot_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_ot_tmp < 15) &
                 (stateEstimatorEskf_DW.count_f != 1)))) {
          uint16_T nextIdx;

          // 'gpsFifo:197' if count == 1
          // 'gpsFifo:201' if tail == capacity
          if (stateEstimatorEskf_DW.tail_o == gpsFifoParams_capacity) {
            // 'gpsFifo:202' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'gpsFifo:203' else
            // 'gpsFifo:204' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_o + 1U;
            if (stateEstimatorEskf_DW.tail_o + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'gpsFifo:207' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_k[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible sample exists; discard current tail.
            // 'gpsFifo:212' tail = nextIdx;
            stateEstimatorEskf_DW.tail_o = nextIdx;

            // 'gpsFifo:213' count = count - 1;
            qY = stateEstimatorEskf_DW.count_f -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_f - 1U >
                stateEstimatorEskf_DW.count_f) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_f = static_cast<uint16_T>(qY);
            dataBuf_ot_tmp++;
          }
        }

        //  Tail now points to the newest eligible GPS measurement.
        // 'gpsFifo:217' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'gpsFifo:221' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&gpsFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_o -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &gpsFifoParams_maxAge_ms.chunks[0U],
                           2)) {
            // 'gpsFifo:222' if tail == capacity
            if (stateEstimatorEskf_DW.tail_o == gpsFifoParams_capacity) {
              // 'gpsFifo:223' tail = uint16(1);
              stateEstimatorEskf_DW.tail_o = 1U;
            } else {
              // 'gpsFifo:224' else
              // 'gpsFifo:225' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_o + 1U;
              if (stateEstimatorEskf_DW.tail_o + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_o = static_cast<uint16_T>(qY);
            }

            // 'gpsFifo:228' count = count - 1;
            qY = stateEstimatorEskf_DW.count_f -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_f - 1U >
                stateEstimatorEskf_DW.count_f) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_f = static_cast<uint16_T>(qY);

            // 'gpsFifo:229' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_f;

            // 'gpsFifo:230' status = STATUS_STALE_DISCARD;
            *status = 4U;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      //  Return selected GPS measurement.
      // 'gpsFifo:235' gpsTimeOut_ms = timeBuf_ms(tail);
      *gpsTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_o - 1];

      // 'gpsFifo:236' posVelOut = dataBuf(:,tail);
      dataBuf_ot_tmp = (stateEstimatorEskf_DW.tail_o - 1) * 6;
      posVelOut[0] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp];
      posVelOut[1] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 1];
      posVelOut[2] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 2];
      posVelOut[3] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 3];
      posVelOut[4] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 4];
      posVelOut[5] = stateEstimatorEskf_DW.dataBuf_ot[dataBuf_ot_tmp + 5];

      // 'gpsFifo:237' gpsReady = true;
      *gpsReady = true;

      //  Consume returned measurement.
      // 'gpsFifo:240' if tail == capacity
      if (stateEstimatorEskf_DW.tail_o == gpsFifoParams_capacity) {
        // 'gpsFifo:241' tail = uint16(1);
        stateEstimatorEskf_DW.tail_o = 1U;
      } else {
        // 'gpsFifo:242' else
        // 'gpsFifo:243' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_o + 1U;
        if (stateEstimatorEskf_DW.tail_o + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_o = static_cast<uint16_T>(qY);
      }

      // 'gpsFifo:246' count = count - 1;
      qY = stateEstimatorEskf_DW.count_f -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_f - 1U > stateEstimatorEskf_DW.count_f) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_f = static_cast<uint16_T>(qY);

      // 'gpsFifo:247' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_f;
    }
  }
}

//
// Function for MATLAB Function: '<S13>/DelayedHorizonBufferManager'
// function [lidarReady, lidarTimeOut_ms, aglOut_m, status, countOut] = ...
//     lidarFifo(isLidarValid, lidarTimeIn_ms, aglIn_m, fusionTime_ms, ...
//               reset, lidarFifoParams)
//
//  LiDAR fixed-memory FIFO for delayed-horizon ESKF fusion.
//
//  Runtime inputs:
//    isLidarValid      : boolean pulse. True for one model step when a NEW
//                        valid LiDAR measurement is available.
//    lidarTimeIn_ms    : uint64 corrected LiDAR measurement epoch [ms].
//    aglIn_m           : single scalar altitude above ground level [m].
//    fusionTime_ms     : uint64 delayed ESKF fusion horizon [ms].
//    reset             : boolean. Clears FIFO and timestamp history.
//    lidarFifoParams   : read-only configuration struct.
//
//  Required lidarFifoParams fields:
//    lidarCapacity              : uint16
//    lidarMaxAge_ms             : uint64
//    lidarMinObsInterval_ms     : uint64
//    lidarTimeResetThreshold_ms : uint64
//
//  Outputs:
//    lidarReady       : true when a LiDAR measurement is returned.
//    lidarTimeOut_ms  : timestamp of returned measurement.
//    aglOut_m         : returned AGL measurement [m].
//    status           : diagnostic status code.
//    countOut         : number of measurements remaining in FIFO.
//
//  FIFO behavior:
//    - Static allocation, no dynamic memory.
//    - Newest measurement overwrites oldest when FIFO is full.
//    - Duplicate/small backward timestamps are rejected.
//    - Large backward timestamp jump resets the LiDAR time epoch.
//    - Measurements arriving too quickly can be throttled.
//    - POP returns the newest measurement satisfying:
//
//          lidarTime <= fusionTime_ms
//
//    - Older eligible measurements are discarded.
//    - Selected measurements older than lidarMaxAge_ms are discarded.
//
//  Important:
//    isLidarValid must behave as a NEW-DATA pulse. It must not remain true
//    continuously while the same LiDAR sample is held at the inputs.
//
void stateEstimatorEskf::stateEstimatorEskf_lidarFifo(boolean_T isLidarValid,
  const uint64m_T lidarTimeIn_ms, real32_T aglIn_m, const uint64m_T
  fusionTime_ms, boolean_T reset, uint16_T lidarFifoParams_capacity, const
  uint64m_T lidarFifoParams_minInterval_ms, const uint64m_T
  lidarFifoParams_maxAge_ms, const uint64m_T lidarFifoParams_resetThreshold_,
  boolean_T *lidarReady, uint64m_T *lidarTimeOut_ms, real32_T *aglOut_m, uint8_T
  *status, uint16_T *countOut)
{
  int96m_T tmp_0;
  int96m_T tmp_1;
  int96m_T tmp_2;
  uint64m_T tmp;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  static const uint64m_T tmp_5{ { 0U, 0U }// chunks
  };

  // 'lidarFifo:47' MAX_SIZE = 32;
  // 'lidarFifo:49' STATUS_OK             = uint8(0);
  // 'lidarFifo:50' STATUS_BAD_TIMESTAMP  = uint8(1);
  // 'lidarFifo:51' STATUS_OVERWRITE      = uint8(2);
  // 'lidarFifo:52' STATUS_TIMEBASE_RESET = uint8(3);
  // 'lidarFifo:53' STATUS_STALE_DISCARD  = uint8(4);
  // 'lidarFifo:54' STATUS_BAD_CAPACITY   = uint8(5);
  //  Static FIFO storage/state initialization.
  // 'lidarFifo:61' if isempty(head)
  //  Default outputs.
  // 'lidarFifo:72' lidarReady = false;
  *lidarReady = false;

  // 'lidarFifo:73' lidarTimeOut_ms = uint64(0);
  *lidarTimeOut_ms = tmp_5;

  // 'lidarFifo:74' aglOut_m = single(0);
  *aglOut_m = 0.0F;

  // 'lidarFifo:75' status = STATUS_OK;
  *status = 0U;

  // 'lidarFifo:76' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_n;

  //  Read-only configuration aliases.
  // 'lidarFifo:79' capacity = lidarFifoParams.capacity;
  // 'lidarFifo:80' maxAge_ms = lidarFifoParams.maxAge_ms;
  // 'lidarFifo:81' minObsInterval_ms = lidarFifoParams.minInterval_ms;
  // 'lidarFifo:82' timeResetThreshold_ms = lidarFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'lidarFifo:85' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((lidarFifoParams_capacity < 1) |
       (lidarFifoParams_capacity > 32))) {
    // 'lidarFifo:86' status = STATUS_BAD_CAPACITY;
    *status = 5U;

    //  Explicit estimator/FIFO reset.
    // 'lidarFifo:91' if reset
  } else if (reset) {
    // 'lidarFifo:92' head = uint16(1);
    stateEstimatorEskf_DW.head_b = 1U;

    // 'lidarFifo:93' tail = uint16(1);
    stateEstimatorEskf_DW.tail_c = 1U;

    // 'lidarFifo:94' count = uint16(0);
    stateEstimatorEskf_DW.count_n = 0U;

    // 'lidarFifo:95' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_gh = tmp_5;

    // 'lidarFifo:96' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_l = false;

    // 'lidarFifo:97' countOut = count;
    *countOut = 0U;
  } else {
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW LIDAR MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'lidarFifo:104' if isLidarValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isLidarValid) {
      //  LiDAR timestamps must normally increase monotonically.
      // 'lidarFifo:106' if haveLastPushTime && lidarTimeIn_ms <= lastPushTime_ms 
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_l &
           uMultiWordLe(&lidarTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_gh.chunks[0U], 2)))
      {
        // 'lidarFifo:107' backwardsJump_ms = lastPushTime_ms - lidarTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'lidarFifo:110' if timeResetThreshold_ms > 0 && ...
        // 'lidarFifo:111'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_gh.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&lidarTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&lidarFifoParams_resetThreshold_.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &lidarFifoParams_resetThreshold_.chunks[0U], 2))) {
          // 'lidarFifo:112' head = uint16(1);
          stateEstimatorEskf_DW.head_b = 1U;

          // 'lidarFifo:113' tail = uint16(1);
          stateEstimatorEskf_DW.tail_c = 1U;

          // 'lidarFifo:114' count = uint16(0);
          stateEstimatorEskf_DW.count_n = 0U;

          // 'lidarFifo:115' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_gh = tmp_5;

          // 'lidarFifo:116' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_l = false;

          // 'lidarFifo:117' status = STATUS_TIMEBASE_RESET;
          *status = 3U;
          guard3 = true;
        } else {
          // 'lidarFifo:118' else
          //  Duplicate timestamp or small backward jump.
          // 'lidarFifo:120' status = STATUS_BAD_TIMESTAMP;
          *status = 1U;
        }
      } else {
        guard3 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      //  Optional rate throttling.
      //  minObsInterval_ms == 0 disables throttling.
      // 'lidarFifo:127' if haveLastPushTime && ...
      // 'lidarFifo:128'             (lidarTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&lidarTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_gh.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_l &
           uMultiWordLt(&tmp_3.chunks[0U],
                        &lidarFifoParams_minInterval_ms.chunks[0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'lidarFifo:133' bufferWasFull = (count == capacity);
        //  Store AGL measurement and timestamp.
        // 'lidarFifo:136' timeBuf_ms(head) = lidarTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_l[stateEstimatorEskf_DW.head_b - 1] =
          lidarTimeIn_ms;

        // 'lidarFifo:137' aglBuf_m(head) = aglIn_m;
        stateEstimatorEskf_DW.aglBuf_m[stateEstimatorEskf_DW.head_b - 1] =
          aglIn_m;

        //  Advance write location.
        // 'lidarFifo:140' if head == capacity
        if (stateEstimatorEskf_DW.head_b == lidarFifoParams_capacity) {
          // 'lidarFifo:141' head = uint16(1);
          stateEstimatorEskf_DW.head_b = 1U;
        } else {
          // 'lidarFifo:142' else
          // 'lidarFifo:143' head = head + 1;
          qY = stateEstimatorEskf_DW.head_b + 1U;
          if (stateEstimatorEskf_DW.head_b + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_b = static_cast<uint16_T>(qY);
        }

        // 'lidarFifo:146' if bufferWasFull
        if (stateEstimatorEskf_DW.count_n == lidarFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'lidarFifo:149' if tail == capacity
          if (stateEstimatorEskf_DW.tail_c == lidarFifoParams_capacity) {
            // 'lidarFifo:150' tail = uint16(1);
            stateEstimatorEskf_DW.tail_c = 1U;
          } else {
            // 'lidarFifo:151' else
            // 'lidarFifo:152' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_c + 1U;
            if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'lidarFifo:156' count = capacity;
          stateEstimatorEskf_DW.count_n = lidarFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'lidarFifo:159' if status == STATUS_OK
          if (*status == 0) {
            // 'lidarFifo:160' status = STATUS_OVERWRITE;
            *status = 2U;
          }
        } else {
          // 'lidarFifo:162' else
          //  One new measurement was added without replacing anything.
          // 'lidarFifo:164' count = count + 1;
          qY = stateEstimatorEskf_DW.count_n + 1U;
          if (stateEstimatorEskf_DW.count_n + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_n = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'lidarFifo:168' lastPushTime_ms = lidarTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_gh = lidarTimeIn_ms;

        // 'lidarFifo:169' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_l = true;

        // 'lidarFifo:170' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_n;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST LIDAR MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'lidarFifo:178' if count == 0
      if ((stateEstimatorEskf_DW.count_n == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_l[stateEstimatorEskf_DW.tail_c - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        int32_T k;
        boolean_T exitg1;

        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'lidarFifo:184' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ lidarTime | lidarTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'lidarFifo:195' for k = 1:(MAX_SIZE-1)
        k = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((k < 31) &
                 (stateEstimatorEskf_DW.count_n != 1)))) {
          uint16_T nextIdx;

          // 'lidarFifo:196' if count == 1
          // 'lidarFifo:200' if tail == capacity
          if (stateEstimatorEskf_DW.tail_c == lidarFifoParams_capacity) {
            // 'lidarFifo:201' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'lidarFifo:202' else
            // 'lidarFifo:203' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_c + 1U;
            if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'lidarFifo:206' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_l[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'lidarFifo:211' tail = nextIdx;
            stateEstimatorEskf_DW.tail_c = nextIdx;

            // 'lidarFifo:212' count = count - 1;
            qY = stateEstimatorEskf_DW.count_n -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_n - 1U >
                stateEstimatorEskf_DW.count_n) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_n = static_cast<uint16_T>(qY);
            k++;
          }
        }

        //  Tail now points to the newest eligible LiDAR measurement.
        // 'lidarFifo:216' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail); 
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'lidarFifo:220' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&lidarFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_l[stateEstimatorEskf_DW.tail_c -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &lidarFifoParams_maxAge_ms.chunks
                           [0U], 2)) {
            // 'lidarFifo:221' if tail == capacity
            if (stateEstimatorEskf_DW.tail_c == lidarFifoParams_capacity) {
              // 'lidarFifo:222' tail = uint16(1);
              stateEstimatorEskf_DW.tail_c = 1U;
            } else {
              // 'lidarFifo:223' else
              // 'lidarFifo:224' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_c + 1U;
              if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
            }

            // 'lidarFifo:227' count = count - 1;
            qY = stateEstimatorEskf_DW.count_n -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_n - 1U >
                stateEstimatorEskf_DW.count_n) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_n = static_cast<uint16_T>(qY);

            // 'lidarFifo:228' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_n;

            // 'lidarFifo:229' status = STATUS_STALE_DISCARD;
            *status = 4U;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      //  Return selected AGL measurement.
      // 'lidarFifo:234' lidarTimeOut_ms = timeBuf_ms(tail);
      *lidarTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_l[stateEstimatorEskf_DW.tail_c - 1];

      // 'lidarFifo:235' aglOut_m = aglBuf_m(tail);
      *aglOut_m = stateEstimatorEskf_DW.aglBuf_m[stateEstimatorEskf_DW.tail_c -
        1];

      // 'lidarFifo:236' lidarReady = true;
      *lidarReady = true;

      //  Consume returned measurement.
      // 'lidarFifo:239' if tail == capacity
      if (stateEstimatorEskf_DW.tail_c == lidarFifoParams_capacity) {
        // 'lidarFifo:240' tail = uint16(1);
        stateEstimatorEskf_DW.tail_c = 1U;
      } else {
        // 'lidarFifo:241' else
        // 'lidarFifo:242' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_c + 1U;
        if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
      }

      // 'lidarFifo:245' count = count - 1;
      qY = stateEstimatorEskf_DW.count_n -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_n - 1U > stateEstimatorEskf_DW.count_n) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_n = static_cast<uint16_T>(qY);

      // 'lidarFifo:246' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_n;
    }
  }
}

//
// Function for MATLAB Function: '<S13>/DelayedHorizonBufferManager'
// function [flowReady, flowTimeOut_ms, velNEOut, status, countOut] = ...
//     flowFifo(isFlowValid, flowTimeIn_ms, velNEIn, fusionTime_ms, ...
//              reset, flowFifoParams)
//
//  Optical-flow fixed-memory FIFO for delayed-horizon ESKF fusion.
//
//  Runtime inputs:
//    isFlowValid      : boolean pulse. True for one model step when a NEW
//                       valid optical-flow measurement is available.
//    flowTimeIn_ms    : uint64 corrected optical-flow measurement epoch [ms].
//    velNEIn          : single [2x1]
//                       [vN; vE]
//    fusionTime_ms    : uint64 delayed ESKF fusion horizon [ms].
//    reset            : boolean. Clears FIFO and timestamp history.
//    flowFifoParams   : read-only configuration struct.
//
//  Required flowFifoParams fields:
//    flowCapacity              : uint16
//    flowMaxAge_ms             : uint64
//    flowMinObsInterval_ms     : uint64
//    flowTimeResetThreshold_ms : uint64
//
//  Outputs:
//    flowReady       : true when an optical-flow measurement is returned.
//    flowTimeOut_ms  : timestamp of returned measurement.
//    velNEOut        : returned [vN; vE].
//    status          : diagnostic status code.
//    countOut        : number of measurements remaining in FIFO.
//
//  FIFO behavior:
//    - Static allocation, no dynamic memory.
//    - Newest measurement overwrites oldest when FIFO is full.
//    - Duplicate/small backward timestamps are rejected.
//    - Large backward timestamp jump resets the flow time epoch.
//    - Measurements arriving too quickly can be throttled.
//    - POP returns the newest measurement satisfying:
//
//          flowTime <= fusionTime_ms
//
//    - Older eligible measurements are discarded.
//    - Selected measurements older than flowMaxAge_ms are discarded.
//
//  Important:
//    isFlowValid must behave as a NEW-DATA pulse. It must not remain true
//    continuously while the same optical-flow sample is held at the inputs.
//
void stateEstimatorEskf::stateEstimatorEskf_flowFifo(boolean_T isFlowValid,
  const uint64m_T flowTimeIn_ms, const real32_T velNEIn[2], const uint64m_T
  fusionTime_ms, boolean_T reset, uint16_T flowFifoParams_capacity, const
  uint64m_T flowFifoParams_minInterval_ms, const uint64m_T
  flowFifoParams_maxAge_ms, const uint64m_T flowFifoParams_resetThreshold_m,
  boolean_T *flowReady, uint64m_T *flowTimeOut_ms, real32_T velNEOut[2], uint8_T
  *status, uint16_T *countOut)
{
  int96m_T tmp_0;
  int96m_T tmp_1;
  int96m_T tmp_2;
  uint64m_T tmp;
  uint64m_T tmp_3;
  uint64m_T tmp_4;
  static const uint64m_T tmp_5{ { 0U, 0U }// chunks
  };

  // 'flowFifo:48' MAX_SIZE = 24;
  // 'flowFifo:50' STATUS_OK             = uint8(0);
  // 'flowFifo:51' STATUS_BAD_TIMESTAMP  = uint8(1);
  // 'flowFifo:52' STATUS_OVERWRITE      = uint8(2);
  // 'flowFifo:53' STATUS_TIMEBASE_RESET = uint8(3);
  // 'flowFifo:54' STATUS_STALE_DISCARD  = uint8(4);
  // 'flowFifo:55' STATUS_BAD_CAPACITY   = uint8(5);
  //  Static FIFO storage/state initialization.
  // 'flowFifo:62' if isempty(head)
  //  Default outputs.
  // 'flowFifo:73' flowReady = false;
  *flowReady = false;

  // 'flowFifo:74' flowTimeOut_ms = uint64(0);
  *flowTimeOut_ms = tmp_5;

  // 'flowFifo:75' velNEOut = zeros(2,1,'single');
  velNEOut[0] = 0.0F;
  velNEOut[1] = 0.0F;

  // 'flowFifo:76' status = STATUS_OK;
  *status = 0U;

  // 'flowFifo:77' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_i;

  //  Read-only configuration aliases.
  // 'flowFifo:80' capacity = flowFifoParams.capacity;
  // 'flowFifo:81' maxAge_ms = flowFifoParams.maxAge_ms;
  // 'flowFifo:82' minObsInterval_ms = flowFifoParams.minInterval_ms;
  // 'flowFifo:83' timeResetThreshold_ms = flowFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'flowFifo:86' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((flowFifoParams_capacity < 1) |
       (flowFifoParams_capacity > 24))) {
    // 'flowFifo:87' status = STATUS_BAD_CAPACITY;
    *status = 5U;

    //  Explicit estimator/FIFO reset.
    // 'flowFifo:92' if reset
  } else if (reset) {
    // 'flowFifo:93' head = uint16(1);
    stateEstimatorEskf_DW.head_i = 1U;

    // 'flowFifo:94' tail = uint16(1);
    stateEstimatorEskf_DW.tail_l = 1U;

    // 'flowFifo:95' count = uint16(0);
    stateEstimatorEskf_DW.count_i = 0U;

    // 'flowFifo:96' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_d = tmp_5;

    // 'flowFifo:97' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_n = false;

    // 'flowFifo:98' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_i_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW OPTICAL-FLOW MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'flowFifo:105' if isFlowValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isFlowValid) {
      //  Optical-flow timestamps must normally increase monotonically.
      // 'flowFifo:107' if haveLastPushTime && flowTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_n &
           uMultiWordLe(&flowTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_d.chunks[0U], 2)))
      {
        // 'flowFifo:108' backwardsJump_ms = lastPushTime_ms - flowTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'flowFifo:111' if timeResetThreshold_ms > 0 && ...
        // 'flowFifo:112'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_d.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&flowTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&flowFifoParams_resetThreshold_m.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &flowFifoParams_resetThreshold_m.chunks[0U], 2))) {
          // 'flowFifo:113' head = uint16(1);
          stateEstimatorEskf_DW.head_i = 1U;

          // 'flowFifo:114' tail = uint16(1);
          stateEstimatorEskf_DW.tail_l = 1U;

          // 'flowFifo:115' count = uint16(0);
          stateEstimatorEskf_DW.count_i = 0U;

          // 'flowFifo:116' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_d = tmp_5;

          // 'flowFifo:117' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_n = false;

          // 'flowFifo:118' status = STATUS_TIMEBASE_RESET;
          *status = 3U;
          guard3 = true;
        } else {
          // 'flowFifo:119' else
          //  Duplicate timestamp or small backward jump.
          // 'flowFifo:121' status = STATUS_BAD_TIMESTAMP;
          *status = 1U;
        }
      } else {
        guard3 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      //  Optional rate throttling.
      //  minObsInterval_ms == 0 disables throttling.
      // 'flowFifo:128' if haveLastPushTime && ...
      // 'flowFifo:129'             (flowTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&flowTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_d.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_n &
           uMultiWordLt(&tmp_3.chunks[0U],
                        &flowFifoParams_minInterval_ms.chunks[0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'flowFifo:134' bufferWasFull = (count == capacity);
        //  Store complete optical-flow observation at current write location.
        //  Each column contains [vN; vE].
        // 'flowFifo:138' timeBuf_ms(head) = flowTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_e[stateEstimatorEskf_DW.head_i - 1] =
          flowTimeIn_ms;

        // 'flowFifo:139' dataBuf(:,head) = velNEIn;
        dataBuf_i_tmp = (stateEstimatorEskf_DW.head_i - 1) << 1;
        stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp] = velNEIn[0];
        stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 1] = velNEIn[1];

        //  Advance write location.
        // 'flowFifo:142' if head == capacity
        if (stateEstimatorEskf_DW.head_i == flowFifoParams_capacity) {
          // 'flowFifo:143' head = uint16(1);
          stateEstimatorEskf_DW.head_i = 1U;
        } else {
          // 'flowFifo:144' else
          // 'flowFifo:145' head = head + 1;
          qY = stateEstimatorEskf_DW.head_i + 1U;
          if (stateEstimatorEskf_DW.head_i + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_i = static_cast<uint16_T>(qY);
        }

        // 'flowFifo:148' if bufferWasFull
        if (stateEstimatorEskf_DW.count_i == flowFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'flowFifo:151' if tail == capacity
          if (stateEstimatorEskf_DW.tail_l == flowFifoParams_capacity) {
            // 'flowFifo:152' tail = uint16(1);
            stateEstimatorEskf_DW.tail_l = 1U;
          } else {
            // 'flowFifo:153' else
            // 'flowFifo:154' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_l + 1U;
            if (stateEstimatorEskf_DW.tail_l + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_l = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'flowFifo:158' count = capacity;
          stateEstimatorEskf_DW.count_i = flowFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'flowFifo:161' if status == STATUS_OK
          if (*status == 0) {
            // 'flowFifo:162' status = STATUS_OVERWRITE;
            *status = 2U;
          }
        } else {
          // 'flowFifo:164' else
          //  One new measurement was added without replacing anything.
          // 'flowFifo:166' count = count + 1;
          qY = stateEstimatorEskf_DW.count_i + 1U;
          if (stateEstimatorEskf_DW.count_i + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_i = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'flowFifo:170' lastPushTime_ms = flowTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_d = flowTimeIn_ms;

        // 'flowFifo:171' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_n = true;

        // 'flowFifo:172' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_i;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST OPTICAL-FLOW MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'flowFifo:180' if count == 0
      if ((stateEstimatorEskf_DW.count_i == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_e[stateEstimatorEskf_DW.tail_l - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Because measurements are chronologically ordered, if the oldest
        //  measurement is newer than the fusion horizon, nothing is ready.
        // 'flowFifo:186' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ flowTime | flowTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'flowFifo:197' for k = 1:(MAX_SIZE-1)
        dataBuf_i_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_i_tmp < 23) &
                 (stateEstimatorEskf_DW.count_i != 1)))) {
          uint16_T nextIdx;

          // 'flowFifo:198' if count == 1
          // 'flowFifo:202' if tail == capacity
          if (stateEstimatorEskf_DW.tail_l == flowFifoParams_capacity) {
            // 'flowFifo:203' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'flowFifo:204' else
            // 'flowFifo:205' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_l + 1U;
            if (stateEstimatorEskf_DW.tail_l + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'flowFifo:208' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_e[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible sample exists; discard current tail.
            // 'flowFifo:213' tail = nextIdx;
            stateEstimatorEskf_DW.tail_l = nextIdx;

            // 'flowFifo:214' count = count - 1;
            qY = stateEstimatorEskf_DW.count_i -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_i - 1U >
                stateEstimatorEskf_DW.count_i) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_i = static_cast<uint16_T>(qY);
            dataBuf_i_tmp++;
          }
        }

        //  Tail now points to the newest eligible optical-flow measurement.
        // 'flowFifo:218' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'flowFifo:222' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&flowFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_e[stateEstimatorEskf_DW.tail_l -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &flowFifoParams_maxAge_ms.chunks
                           [0U], 2)) {
            // 'flowFifo:223' if tail == capacity
            if (stateEstimatorEskf_DW.tail_l == flowFifoParams_capacity) {
              // 'flowFifo:224' tail = uint16(1);
              stateEstimatorEskf_DW.tail_l = 1U;
            } else {
              // 'flowFifo:225' else
              // 'flowFifo:226' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_l + 1U;
              if (stateEstimatorEskf_DW.tail_l + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_l = static_cast<uint16_T>(qY);
            }

            // 'flowFifo:229' count = count - 1;
            qY = stateEstimatorEskf_DW.count_i -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_i - 1U >
                stateEstimatorEskf_DW.count_i) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_i = static_cast<uint16_T>(qY);

            // 'flowFifo:230' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_i;

            // 'flowFifo:231' status = STATUS_STALE_DISCARD;
            *status = 4U;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      //  Return selected optical-flow measurement.
      // 'flowFifo:236' flowTimeOut_ms = timeBuf_ms(tail);
      *flowTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_e[stateEstimatorEskf_DW.tail_l - 1];

      // 'flowFifo:237' velNEOut = dataBuf(:,tail);
      dataBuf_i_tmp = (stateEstimatorEskf_DW.tail_l - 1) << 1;
      velNEOut[0] = stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp];
      velNEOut[1] = stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 1];

      // 'flowFifo:238' flowReady = true;
      *flowReady = true;

      //  Consume returned measurement.
      // 'flowFifo:241' if tail == capacity
      if (stateEstimatorEskf_DW.tail_l == flowFifoParams_capacity) {
        // 'flowFifo:242' tail = uint16(1);
        stateEstimatorEskf_DW.tail_l = 1U;
      } else {
        // 'flowFifo:243' else
        // 'flowFifo:244' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_l + 1U;
        if (stateEstimatorEskf_DW.tail_l + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_l = static_cast<uint16_T>(qY);
      }

      // 'flowFifo:247' count = count - 1;
      qY = stateEstimatorEskf_DW.count_i -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_i - 1U > stateEstimatorEskf_DW.count_i) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_i = static_cast<uint16_T>(qY);

      // 'flowFifo:248' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_i;
    }
  }
}

// System initialize for referenced model: 'stateEstimatorEskf'
void stateEstimatorEskf::init(busStateEstimatorDebug *rty_stateEstimatorDebug)
{
  int32_T i;
  int32_T k;
  int32_T tmp;
  int8_T b_I[256];
  static const uint64m_T tmp_0{ { 0U, 0U }// chunks
  };

  static const int8_T tmp_1[20]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0 };

  // InitializeConditions for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] = 1.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[2] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] = 1.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[5] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[6] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[7] = 0.0F;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[8] = 1.0F;

  // InitializeConditions for Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = true;

  // InitializeConditions for Delay: '<S1>/Delay2'
  stateEstimatorEskf_DW.icLoad_g = true;
  for (i = 0; i < 20; i++) {
    // InitializeConditions for UnitDelay: '<Root>/Unit Delay'
    stateEstimatorEskf_DW.UnitDelay_DSTATE_e[i] =
      rtCP_UnitDelay_InitialConditi_m[i];

    // SystemInitialize for Chart: '<Root>/estimatorStateMachine' incorporates:
    //   UnitDelay: '<Root>/Unit Delay'

    stateEstimatorEskf_DW.initialStates[i] = tmp_1[i];
  }

  // SystemInitialize for Chart: '<Root>/estimatorStateMachine'
  stateEstimatorEskf_DW.initialDcmBodyToNed[0] = 1.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[1] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[2] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[3] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[4] = 1.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[5] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[6] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[7] = 0.0F;
  stateEstimatorEskf_DW.initialDcmBodyToNed[8] = 1.0F;
  stateEstimatorEskf_DW.sensorDataOut.dtImuTime_s = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.localNedUnitMag_nd[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.pos_m[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.vel_mps[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.localNedUnitMag_nd[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.pos_m[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.vel_mps[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.localNedUnitMag_nd[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.pos_m[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.nedPosAndVel.vel_mps[2] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.lidarAgl_m = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.ofNeVel_mps[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.ofNeVel_mps[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isImuValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isLidarValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isOfValid = false;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.imuTimestamp_ms = tmp_0;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.magTimestamp_ms = tmp_0;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.gpsTimestamp_ms = tmp_0;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.baroTimestamp_ms = tmp_0;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.lidarTimestamp_ms = tmp_0;
  stateEstimatorEskf_DW.sensorDataOut.sensorTimestamp.ofTimestamp_ms = tmp_0;

  // SystemInitialize for BusCreator: '<Root>/Bus Creator' incorporates:
  //   Chart: '<Root>/estimatorStateMachine'

  rty_stateEstimatorDebug->stateEstInitPct = 0.0F;
  rty_stateEstimatorDebug->smMode = enumStateEstimateMode::NONE;

  // SystemInitialize for MATLAB Function: '<S13>/DelayedHorizonBufferManager'
  // 'stateFifo:109' modeBuf = repmat(enumStateEstimateMode.NONE,1,MAX_SIZE);
  // 'stateFifo:111' head = uint16(1);
  stateEstimatorEskf_DW.head = 1U;

  //  Next write location
  // 'stateFifo:112' tail = uint16(1);
  stateEstimatorEskf_DW.tail = 1U;

  //  Oldest valid mode
  // 'stateFifo:113' count = uint16(0);
  //  Number of valid stored modes
  // 'imuFifo:88' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'imuFifo:89' dataBuf = zeros(7,MAX_SIZE,'single');
  // 'imuFifo:91' head = uint16(1);
  stateEstimatorEskf_DW.head_e = 1U;

  //  Next write location
  // 'imuFifo:92' tail = uint16(1);
  stateEstimatorEskf_DW.tail_g = 1U;

  //  Oldest valid IMU sample
  // 'imuFifo:93' count = uint16(0);
  //  Number of valid samples
  // 'magFifo:63' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 64; i++) {
    stateEstimatorEskf_DW.timeBuf_ms[i] = tmp_0;
    stateEstimatorEskf_DW.timeBuf_ms_c[i] = tmp_0;
  }

  // 'magFifo:64' dataBuf = zeros(3,MAX_SIZE,'single');
  // 'magFifo:65' head = uint16(1);
  stateEstimatorEskf_DW.head_m = 1U;

  //  Next write location
  // 'magFifo:66' tail = uint16(1);
  stateEstimatorEskf_DW.tail_n = 1U;

  //  Oldest valid measurement
  // 'magFifo:67' count = uint16(0);
  //  Number of valid measurements
  // 'magFifo:68' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms = tmp_0;

  //  Last accepted magnetometer timestamp
  // 'magFifo:69' haveLastPushTime = false;
  // 'gpsFifo:63' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 16; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_k[i] = tmp_0;
  }

  // 'gpsFifo:64' dataBuf = zeros(6,MAX_SIZE,'single');
  // 'gpsFifo:65' head = uint16(1);
  stateEstimatorEskf_DW.head_k = 1U;

  //  Next write location
  // 'gpsFifo:66' tail = uint16(1);
  stateEstimatorEskf_DW.tail_o = 1U;

  //  Oldest valid measurement
  // 'gpsFifo:67' count = uint16(0);
  //  Number of valid measurements
  // 'gpsFifo:68' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_h = tmp_0;

  //  Last accepted GPS timestamp
  // 'gpsFifo:69' haveLastPushTime = false;
  // 'baroFifo:62' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'baroFifo:63' altitudeBuf_m = zeros(1,MAX_SIZE,'single');
  for (i = 0; i < 24; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_o[i] = tmp_0;
  }

  // 'baroFifo:64' head = uint16(1);
  stateEstimatorEskf_DW.head_c = 1U;

  //  Next write location
  // 'baroFifo:65' tail = uint16(1);
  stateEstimatorEskf_DW.tail_i = 1U;

  //  Oldest valid measurement
  // 'baroFifo:66' count = uint16(0);
  //  Number of valid measurements
  // 'baroFifo:67' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_g = tmp_0;

  //  Last accepted barometer timestamp
  // 'baroFifo:68' haveLastPushTime = false;
  // 'lidarFifo:62' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'lidarFifo:63' aglBuf_m = zeros(1,MAX_SIZE,'single');
  for (i = 0; i < 32; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_l[i] = tmp_0;
  }

  // 'lidarFifo:64' head = uint16(1);
  stateEstimatorEskf_DW.head_b = 1U;

  //  Next write location
  // 'lidarFifo:65' tail = uint16(1);
  stateEstimatorEskf_DW.tail_c = 1U;

  //  Oldest valid measurement
  // 'lidarFifo:66' count = uint16(0);
  //  Number of valid measurements
  // 'lidarFifo:67' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_gh = tmp_0;

  //  Last accepted LiDAR timestamp
  // 'lidarFifo:68' haveLastPushTime = false;
  // 'flowFifo:63' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 24; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_e[i] = tmp_0;
  }

  // 'flowFifo:64' dataBuf = zeros(2,MAX_SIZE,'single');
  // 'flowFifo:65' head = uint16(1);
  stateEstimatorEskf_DW.head_i = 1U;

  //  Next write location
  // 'flowFifo:66' tail = uint16(1);
  stateEstimatorEskf_DW.tail_l = 1U;

  //  Oldest valid measurement
  // 'flowFifo:67' count = uint16(0);
  //  Number of valid measurements
  // 'flowFifo:68' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_d = tmp_0;

  // End of SystemInitialize for MATLAB Function: '<S13>/DelayedHorizonBufferManager' 

  // SystemInitialize for MATLAB Function: '<S1>/EKF'
  //  Last accepted flow timestamp
  // 'flowFifo:69' haveLastPushTime = false;
  // 'errorStateEkf_function2:66' I3 = eye(3, 'single');
  stateEstimatorEskf_DW.I3[1] = 0.0F;
  stateEstimatorEskf_DW.I3[2] = 0.0F;
  stateEstimatorEskf_DW.I3[3] = 0.0F;
  stateEstimatorEskf_DW.I3[5] = 0.0F;
  stateEstimatorEskf_DW.I3[6] = 0.0F;
  stateEstimatorEskf_DW.I3[7] = 0.0F;
  stateEstimatorEskf_DW.I3[0] = 1.0F;
  stateEstimatorEskf_DW.I3[4] = 1.0F;
  stateEstimatorEskf_DW.I3[8] = 1.0F;

  // Nominal state Jacobian wrt to error state
  // 'errorStateEkf_function2:68' xErrorJac = zeros(20, 19, 'single');
  std::memset(&stateEstimatorEskf_DW.xErrorJac[0], 0, 380U * sizeof(real32_T));

  // 'errorStateEkf_function2:69' xErrorJac(5:20, 4:19) = eye(16, 'single');
  std::memset(&b_I[0], 0, sizeof(int8_T) << 8U);
  k = 0;
  for (i = 0; i < 16; i++) {
    b_I[k] = 1;
    k += 17;
  }

  i = 0;
  tmp = 0;
  for (k = 0; k < 16; k++) {
    for (int32_T i_0{0}; i_0 < 16; i_0++) {
      stateEstimatorEskf_DW.xErrorJac[(i_0 + i) + 64] = b_I[i_0 + tmp];
    }

    i += 20;
    tmp += 16;
  }

  // End of SystemInitialize for MATLAB Function: '<S1>/EKF'
  // Error state jacobian
  //      errorStateJac = eye(19, 'single');
  //      errorStateJac(1:3, 10:12) = -I3*sampleTime_s;
}

// Output and update for referenced model: 'stateEstimatorEskf'
void stateEstimatorEskf::step(const busImuData *rtu_imuData, const busMagData
  *rtu_magData, const busGpsData *rtu_gpsData, const busBaroData *rtu_baroData,
  const busLidarData *rtu_lidarData, const busMtf01pData *rtu_mtf01pData, const
  busImuNtchFiltParams *rtu_imuNotchFiltParams, const busAccelParams
  *rtu_accelParams, const busMagParams *rtu_magParams, const busLidarParams
  *rtu_lidarParams, const busMtf01pParams *rtu_mtf01pParams, const
  busStateEstSmParams *rtu_stateEstSmParams, const real32_T rtu_processNoiseQ
  [361], const real32_T rtu_measNoiseR[225], const real32_T rtu_initCovP[361],
  const real32_T *rtu_gEarth_mps2, real32_T rty_states[20], real32_T
  rty_eulAng_rad[3], real32_T rty_dcmNedToBody[9], real32_T rty_dcmNedToFep[9],
  real32_T rty_bodyAccels_mps2[3], busStateEstimatorDebug
  *rty_stateEstimatorDebug)
{
  int96m_T tmp_6;
  int96m_T tmp_7;
  int96m_T tmp_8;
  uint64m_T baroTimeIn_ms;
  uint64m_T dhSensorIn_sensorTimestamp_imuT;
  uint64m_T dhSensorIn_sensorTimestamp_magT;
  uint64m_T fusionTime_ms;
  uint64m_T gpsTimeIn_ms;
  uint64m_T lidarTimeIn_ms;
  uint64m_T magTimeIn_ms;
  uint64m_T ofTimeIn_ms;
  uint64m_T tmp_9;
  uint64m_T tmp_a;
  uint64m_T tmp_b;
  real_T tmp[19];
  real_T tmp_0[16];
  real_T tmp_1[16];
  real_T tmp_4[9];
  real_T latLonAltOut[3];
  real_T n;
  real_T nRef;
  real_T n_idx_0;
  real_T n_idx_0_tmp;
  real_T n_idx_0_tmp_0;
  real_T rtb_nedPos_m_idx_0;
  real_T rtb_nedPos_m_idx_2;
  real_T rtb_nedPos_m_tmp;
  real_T rtb_nedPos_m_tmp_0;
  real_T rtb_nedPos_m_tmp_1;
  int32_T covP_tmp;
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  int32_T i_2;
  int32_T rtb_VectorConcatenate1_tmp;
  int32_T tmp_c;
  real32_T b_covP[361];
  real32_T covP[361];
  real32_T tmp_5[225];
  real32_T measJac[60];
  real32_T H[57];
  real32_T b_K[57];
  real32_T tmp1[57];
  real32_T K[45];
  real32_T tmp1_0[45];
  real32_T rtb_states[20];
  real32_T b_errorStateHat[19];
  real32_T d_K[19];
  real32_T errorStateHat[19];
  real32_T K_0[15];
  real32_T tmp_2[15];
  real32_T rtb_states_0[12];
  real32_T rtb_VectorConcatenate_m[9];
  real32_T imuOut[7];
  real32_T nedPosAndVel[6];
  real32_T rtb_CastToSingle_0[6];
  real32_T rtb_ElementProduct[6];
  real32_T qDelayed[4];
  real32_T qError[4];
  real32_T tmp_3[4];
  real32_T Divide[3];
  real32_T Product[3];
  real32_T imuOut_0[3];
  real32_T rtb_CastToSingle[3];
  real32_T rtb_VectorConcatenate_i[2];
  real32_T rtb_VectorConcatenate_k2[2];
  real32_T q2q3;
  real32_T rtb_Merge_idx_0;
  real32_T rtb_Merge_idx_0_tmp;
  real32_T rtb_Merge_idx_0_tmp_0;
  real32_T rtb_Merge_idx_0_tmp_1;
  real32_T rtb_Merge_idx_0_tmp_2;
  real32_T rtb_Merge_idx_1;
  real32_T rtb_Product1_b;
  real32_T rtb_Product2_c;
  real32_T rtb_Sum2_idx_1;
  real32_T rtb_Sum2_idx_2;
  real32_T rtb_UnitDelay_g;
  real32_T rtb_XAxis;
  real32_T rtb_XAxis1;
  real32_T rtb_XAxis2;
  real32_T rtb_dcmBodyToNed_idx_2;
  real32_T rtb_dcmBodyToNed_idx_5;
  real32_T rtb_dcmBodyToNed_idx_7;
  real32_T rtb_dcmBodyToNed_idx_8;
  uint32_T qY;
  uint16_T nextIdx;
  int8_T c;
  uint8_T a__4;
  boolean_T dhSensorIn_sensorValidity_isBar;
  boolean_T dhSensorIn_sensorValidity_isGps;
  boolean_T dhSensorIn_sensorValidity_isLid;
  boolean_T dhSensorIn_sensorValidity_isMag;
  boolean_T dhSensorIn_sensorValidity_isOfV;
  boolean_T gpsLossFlag;
  boolean_T rtb_Compare;
  boolean_T stateModeReady;
  enumStateEstimateMode delayedEstSmMode;
  enumStateEstimateMode mode;
  static const uint64m_T dhSensorIn_sensorTimestamp_ma_0{ { 0U, 0U }// chunks
  };

  static const int8_T c_0[15]{ 0, 1, 2, 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18 };

  static const int8_T e[12]{ 6, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

  static const int8_T f[12]{ 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };

  real32_T tmp_d[46];
  boolean_T exitg1;
  boolean_T guard1;
  boolean_T guard2;
  boolean_T guard3;

  // DiscreteTransferFcn: '<S61>/X Axis'
  stateEstimatorEskf_DW.XAxis_tmp = (rtu_imuData->bodyAccels_mps2[0] -
    stateEstimatorEskf_DW.XAxis_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[1]) -
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[2];
  rtb_UnitDelay_g = (rtu_imuNotchFiltParams->accelNtchFilt.xNum[0] *
                     stateEstimatorEskf_DW.XAxis_tmp +
                     stateEstimatorEskf_DW.XAxis_states[0] *
                     rtu_imuNotchFiltParams->accelNtchFilt.xNum[1]) +
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xNum[2];

  // DiscreteTransferFcn: '<S61>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_tmp = (rtu_imuData->bodyAccels_mps2[1] -
    stateEstimatorEskf_DW.XAxis1_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[1]) -
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[2];
  rtb_Product2_c = (rtu_imuNotchFiltParams->accelNtchFilt.yNum[0] *
                    stateEstimatorEskf_DW.XAxis1_tmp +
                    stateEstimatorEskf_DW.XAxis1_states[0] *
                    rtu_imuNotchFiltParams->accelNtchFilt.yNum[1]) +
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yNum[2];

  // DiscreteTransferFcn: '<S61>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_tmp = (rtu_imuData->bodyAccels_mps2[2] -
    stateEstimatorEskf_DW.XAxis2_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[1]) -
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[2];
  rtb_Product1_b = (rtu_imuNotchFiltParams->accelNtchFilt.zNum[0] *
                    stateEstimatorEskf_DW.XAxis2_tmp +
                    stateEstimatorEskf_DW.XAxis2_states[0] *
                    rtu_imuNotchFiltParams->accelNtchFilt.zNum[1]) +
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zNum[2];

  // Product: '<S4>/Product' incorporates:
  //   Product: '<S4>/Matrix Multiply'
  //   SignalConversion generated from: '<S4>/Matrix Multiply'
  //   Sum: '<S4>/Sum'

  Product[0] = (((rtu_accelParams->scaleAlignMat_nd[0] * rtb_UnitDelay_g +
                  rtu_accelParams->scaleAlignMat_nd[3] * rtb_Product2_c) +
                 rtu_accelParams->scaleAlignMat_nd[6] * rtb_Product1_b) +
                rtu_accelParams->offset_nd[0]) * *rtu_gEarth_mps2;
  Product[1] = (((rtu_accelParams->scaleAlignMat_nd[1] * rtb_UnitDelay_g +
                  rtu_accelParams->scaleAlignMat_nd[4] * rtb_Product2_c) +
                 rtu_accelParams->scaleAlignMat_nd[7] * rtb_Product1_b) +
                rtu_accelParams->offset_nd[1]) * *rtu_gEarth_mps2;
  Product[2] = (((rtu_accelParams->scaleAlignMat_nd[2] * rtb_UnitDelay_g +
                  rtu_accelParams->scaleAlignMat_nd[5] * rtb_Product2_c) +
                 rtu_accelParams->scaleAlignMat_nd[8] * rtb_Product1_b) +
                rtu_accelParams->offset_nd[2]) * *rtu_gEarth_mps2;

  // DiscreteTransferFcn: '<S62>/X Axis'
  stateEstimatorEskf_DW.XAxis_tmp_o = (rtu_imuData->bodyRates_radps[0] -
    stateEstimatorEskf_DW.XAxis_states_e[0] *
    rtu_imuNotchFiltParams->gyroNtchFilt.xDen[1]) -
    stateEstimatorEskf_DW.XAxis_states_e[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.xDen[2];
  rtb_XAxis = (rtu_imuNotchFiltParams->gyroNtchFilt.xNum[0] *
               stateEstimatorEskf_DW.XAxis_tmp_o +
               stateEstimatorEskf_DW.XAxis_states_e[0] *
               rtu_imuNotchFiltParams->gyroNtchFilt.xNum[1]) +
    stateEstimatorEskf_DW.XAxis_states_e[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.xNum[2];

  // DiscreteTransferFcn: '<S62>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_tmp_l = (rtu_imuData->bodyRates_radps[1] -
    stateEstimatorEskf_DW.XAxis1_states_a[0] *
    rtu_imuNotchFiltParams->gyroNtchFilt.yDen[1]) -
    stateEstimatorEskf_DW.XAxis1_states_a[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.yDen[2];
  rtb_XAxis1 = (rtu_imuNotchFiltParams->gyroNtchFilt.yNum[0] *
                stateEstimatorEskf_DW.XAxis1_tmp_l +
                stateEstimatorEskf_DW.XAxis1_states_a[0] *
                rtu_imuNotchFiltParams->gyroNtchFilt.yNum[1]) +
    stateEstimatorEskf_DW.XAxis1_states_a[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.yNum[2];

  // DiscreteTransferFcn: '<S62>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_tmp_o = (rtu_imuData->bodyRates_radps[2] -
    stateEstimatorEskf_DW.XAxis2_states_j[0] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zDen[1]) -
    stateEstimatorEskf_DW.XAxis2_states_j[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zDen[2];
  rtb_XAxis2 = (rtu_imuNotchFiltParams->gyroNtchFilt.zNum[0] *
                stateEstimatorEskf_DW.XAxis2_tmp_o +
                stateEstimatorEskf_DW.XAxis2_states_j[0] *
                rtu_imuNotchFiltParams->gyroNtchFilt.zNum[1]) +
    stateEstimatorEskf_DW.XAxis2_states_j[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zNum[2];

  // Sum: '<S9>/Sum'
  rtb_Product1_b = rtu_magData->bodyMagVector_uT[0] - rtu_magParams->offset_uT[0];
  rtb_Product2_c = rtu_magData->bodyMagVector_uT[1] - rtu_magParams->offset_uT[1];
  rtb_Sum2_idx_2 = rtu_magData->bodyMagVector_uT[2] - rtu_magParams->offset_uT[2];

  // Product: '<S9>/Matrix Multiply' incorporates:
  //   Sum: '<S10>/Sum2'

  rtb_UnitDelay_g = (rtu_magParams->scaleAlignMat_nd[0] * rtb_Product1_b +
                     rtu_magParams->scaleAlignMat_nd[3] * rtb_Product2_c) +
    rtu_magParams->scaleAlignMat_nd[6] * rtb_Sum2_idx_2;
  rtb_Sum2_idx_1 = (rtu_magParams->scaleAlignMat_nd[1] * rtb_Product1_b +
                    rtu_magParams->scaleAlignMat_nd[4] * rtb_Product2_c) +
    rtu_magParams->scaleAlignMat_nd[7] * rtb_Sum2_idx_2;
  rtb_Sum2_idx_2 = (rtu_magParams->scaleAlignMat_nd[2] * rtb_Product1_b +
                    rtu_magParams->scaleAlignMat_nd[5] * rtb_Product2_c) +
    rtu_magParams->scaleAlignMat_nd[8] * rtb_Sum2_idx_2;

  // MinMax: '<S9>/Max' incorporates:
  //   Constant: '<S9>/Constant2'
  //   Math: '<S9>/Transpose'
  //   Product: '<S9>/Matrix Multiply1'
  //   Sqrt: '<S9>/Sqrt'
  //   Sum: '<S10>/Sum2'

  rtb_Product1_b = std::fmax(std::sqrt((rtb_UnitDelay_g * rtb_UnitDelay_g +
    rtb_Sum2_idx_1 * rtb_Sum2_idx_1) + rtb_Sum2_idx_2 * rtb_Sum2_idx_2), 1.0E-7F);

  // Product: '<S9>/Divide' incorporates:
  //   Sum: '<S10>/Sum2'

  Divide[0] = rtb_UnitDelay_g / rtb_Product1_b;
  Divide[1] = rtb_Sum2_idx_1 / rtb_Product1_b;
  Divide[2] = rtb_Sum2_idx_2 / rtb_Product1_b;

  // Product: '<S11>/Divide1' incorporates:
  //   Constant: '<S11>/Constant'
  //   Constant: '<S11>/Constant2'
  //   Constant: '<S11>/Constant3'
  //   Math: '<S11>/Power'
  //   Product: '<S11>/Divide'
  //   Sum: '<S11>/Sum'

  rtb_Product1_b = (1.0F - std::pow(rtu_baroData->pressure_pa / 101325.0F,
    0.190294951F)) * 44330.0F;

  // SignalConversion generated from: '<S5>/ SFunction ' incorporates:
  //   Chart: '<Root>/estimatorStateMachine'

  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0] = rtb_XAxis;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1] = rtb_XAxis1;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2] = rtb_XAxis2;

  // Chart: '<Root>/estimatorStateMachine' incorporates:
  //   Product: '<S4>/Product'
  //   Product: '<S9>/Divide'
  //   SignalConversion generated from: '<S5>/ SFunction '

  // Gateway: estimatorStateMachine
  // During: estimatorStateMachine
  if (stateEstimatorEskf_DW.is_active_c3_stateEstimatorEskf == 0) {
    // Entry: estimatorStateMachine
    stateEstimatorEskf_DW.is_active_c3_stateEstimatorEskf = 1U;

    // Entry Internal: estimatorStateMachine
    // Transition: '<S5>:2'
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
      stateEstimatorEsk_IN_INITIALIZE;

    // Entry 'INITIALIZE': '<S5>:1'
    // Variables to set on entry
    // '<S5>:1:4' mode = enumStateEstimateMode.INITIALIZE;
    mode = enumStateEstimateMode::INITIALIZE;

    // '<S5>:1:5' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S5>:1:6' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S5>:1:7' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S5>:1:8' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S5>:1:9' latLonAltOut = latLonAltIn;
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
    latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
    latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
    latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

    // '<S5>:1:10' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S5>:1:11' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S5>:1:12' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    // '<S5>:1:13' resetStates = true;
    stateEstimatorEskf_DW.resetStates = true;

    //  Index to keep track of how many imu readings we have summed
    //  so far
    // '<S5>:1:16' imuIdx = 0;
    stateEstimatorEskf_DW.imuIdx = 0.0F;

    // '<S5>:1:17' imuMean = [0; 0; 0; 0; 0; 0];
    stateEstimatorEskf_DW.imuMean[0] = 0.0F;
    stateEstimatorEskf_DW.imuMean[1] = 0.0F;
    stateEstimatorEskf_DW.imuMean[2] = 0.0F;
    stateEstimatorEskf_DW.imuMean[3] = 0.0F;
    stateEstimatorEskf_DW.imuMean[4] = 0.0F;
    stateEstimatorEskf_DW.imuMean[5] = 0.0F;

    // '<S5>:1:18' accelBias_mps2 = [0; 0; 0];
    // '<S5>:1:19' gyroBias_radps = [0; 0; 0];
    stateEstimatorEskf_DW.accelBias_mps2[0] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[0] = 0.0F;
    stateEstimatorEskf_DW.accelBias_mps2[1] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[1] = 0.0F;
    stateEstimatorEskf_DW.accelBias_mps2[2] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[2] = 0.0F;

    // '<S5>:1:20' imuM2 = [0; 0; 0; 0; 0; 0];
    stateEstimatorEskf_DW.imuM2[0] = 0.0F;
    stateEstimatorEskf_DW.imuM2[1] = 0.0F;
    stateEstimatorEskf_DW.imuM2[2] = 0.0F;
    stateEstimatorEskf_DW.imuM2[3] = 0.0F;
    stateEstimatorEskf_DW.imuM2[4] = 0.0F;
    stateEstimatorEskf_DW.imuM2[5] = 0.0F;

    // '<S5>:1:21' initialQuat = [1; 0; 0; 0];
    stateEstimatorEskf_DW.initialQuat[0] = 1.0F;
    stateEstimatorEskf_DW.initialQuat[1] = 0.0F;
    stateEstimatorEskf_DW.initialQuat[2] = 0.0F;
    stateEstimatorEskf_DW.initialQuat[3] = 0.0F;

    // '<S5>:1:22' isAttInitialized = false;
    stateEstimatorEskf_DW.isAttInitialized = false;

    //  Index to keep track of how many mag readings we have summed
    //  so far
    // '<S5>:1:25' magIdx = 0;
    stateEstimatorEskf_DW.magIdx = 0.0F;

    // '<S5>:1:26' magMean = [0; 0; 0];
    // '<S5>:1:27' magM2 = [0; 0; 0];
    // '<S5>:1:28' magBias_nd = [0; 0; 0];
    // '<S5>:1:29' nedMagVecNorm_nd = [0; 0; 0];
    //  Index to keep track of how many gps readings we have summed
    //  so far
    // '<S5>:1:32' gpsIdx = 0;
    stateEstimatorEskf_DW.gpsIdx = 0.0;

    // '<S5>:1:33' refLatLonAlt = [0; 0; 0];
    stateEstimatorEskf_DW.magMean[0] = 0.0F;
    stateEstimatorEskf_DW.magM2[0] = 0.0F;
    stateEstimatorEskf_DW.magBias_nd[0] = 0.0F;
    stateEstimatorEskf_DW.refLatLonAlt[0] = 0.0;
    stateEstimatorEskf_DW.magMean[1] = 0.0F;
    stateEstimatorEskf_DW.magM2[1] = 0.0F;
    stateEstimatorEskf_DW.magBias_nd[1] = 0.0F;
    stateEstimatorEskf_DW.refLatLonAlt[1] = 0.0;
    stateEstimatorEskf_DW.magMean[2] = 0.0F;
    stateEstimatorEskf_DW.magM2[2] = 0.0F;
    stateEstimatorEskf_DW.magBias_nd[2] = 0.0F;
    stateEstimatorEskf_DW.refLatLonAlt[2] = 0.0;

    // '<S5>:1:34' isPosInitialized = false;
    stateEstimatorEskf_DW.isPosInitialized = false;

    //  Index to keep track of how many baro readings we have summed
    //  so far
    // '<S5>:1:37' baroIdx = 0;
    stateEstimatorEskf_DW.baroIdx = 0.0F;

    // '<S5>:1:38' baroInitAltMean = 0;
    stateEstimatorEskf_DW.baroInitAltMean = 0.0F;

    // '<S5>:1:39' baroInitAltM2 = 0;
    stateEstimatorEskf_DW.baroInitAltM2 = 0.0F;

    // '<S5>:1:40' baroBias_m = 0;
    stateEstimatorEskf_DW.baroBias_m = 0.0F;

    // '<S5>:1:41' isBaroInitialized = false;
    stateEstimatorEskf_DW.isBaroInitialized = false;

    // End of entry stage
  } else {
    switch (stateEstimatorEskf_DW.is_c3_stateEstimatorEskf) {
     case stateEstimatorEsk_IN_INITIALIZE:
      stateEstimatorEskf_INITIALIZE(&mode, latLonAltOut, &rtb_Product1_b,
        Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData,
        rtu_stateEstSmParams);
      break;

     case stateEstimatorEskf_IN_RUN:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN;

      // During 'RUN': '<S5>:43'
      // '<S5>:68:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
      // '<S5>:68:2' stateEstSmParams.gpsLossCheckDuration_s);
      if (rtu_gpsData->isGpsDataValid) {
        stateEstimatorEskf_DW.durationCounter_1 = 0U;
      }

      if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1) >=
          rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
        // Transition: '<S5>:68'
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_GPS_LOST;

        // Entry 'RUN_GPS_LOST': '<S5>:67'
        // GPS WAS LOST
        // '<S5>:67:4' gpsValidCount = 0;
        stateEstimatorEskf_DW.gpsValidCount = 0U;

        // '<S5>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
        mode = enumStateEstimateMode::RUN_GPS_LOST;

        // '<S5>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S5>:67:10' latLonAltOut = latLonAltIn;
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
        latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
        latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
        latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

        // '<S5>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // FULL EKF WITH GPS IS RUNNING
        // '<S5>:43:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:43:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:43:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:43:19' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S5>:43:20' latLonAltOut = latLonAltIn;
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
        latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
        latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
        latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

        // '<S5>:43:21' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:43:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:43:23' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;
      }
      break;

     case stateEstimatorE_IN_RUN_GPS_LOST:
      mode = enumStateEstimateMode::RUN_GPS_LOST;

      // During 'RUN_GPS_LOST': '<S5>:67'
      // '<S5>:71:1' sf_internal_predicateOutput = 0 | (gpsValidCount >= stateEstSmParams.desValidGpsCount); 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S5>:71'
        stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_Product1_b,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // GPS WAS LOST
        // '<S5>:67:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:67:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:67:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:67:19' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S5>:67:20' latLonAltOut = latLonAltIn;
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
        latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
        latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
        latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

        // '<S5>:67:21' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:67:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:67:23' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
        // '<S5>:67:25' if(isGpsDataValid)
        if (rtu_gpsData->isGpsDataValid) {
          // '<S5>:67:26' gpsValidCount = gpsValidCount + 1;
          stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
            (stateEstimatorEskf_DW.gpsValidCount + 1);
        }
      }
      break;

     case stateEstima_IN_RUN_GPS_NOT_INIT:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN_GPS_NOT_INIT;

      // During 'RUN_GPS_NOT_INIT': '<S5>:62'
      // '<S5>:65:1' sf_internal_predicateOutput = 0 | (gpsValidCount >= stateEstSmParams.desValidGpsCount); 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S5>:65'
        stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_Product1_b,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // '<S5>:73:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
        // '<S5>:73:2' stateEstSmParams.gpsLossCheckDuration_s);
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_n = 0U;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_n) >=
            rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
          // Transition: '<S5>:73'
          stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
            stateEstimatorE_IN_RUN_GPS_LOST;

          // Entry 'RUN_GPS_LOST': '<S5>:67'
          // GPS WAS LOST
          // '<S5>:67:4' gpsValidCount = 0;
          stateEstimatorEskf_DW.gpsValidCount = 0U;

          // '<S5>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
          mode = enumStateEstimateMode::RUN_GPS_LOST;

          // '<S5>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
          // '<S5>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
          // '<S5>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S5>:67:10' latLonAltOut = latLonAltIn;
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
          latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
          latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
          latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

          // '<S5>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF STARTED RUNNING WITHOUT GPS
          // '<S5>:62:18' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S5>:62:19' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S5>:62:20' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:62:21' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S5>:62:22' latLonAltOut = latLonAltIn;
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
          latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
          latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
          latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

          // '<S5>:62:23' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:62:24' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:62:25' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
          // '<S5>:62:27' if(isGpsDataValid)
          if (rtu_gpsData->isGpsDataValid) {
            // '<S5>:62:28' gpsValidCount = gpsValidCount + 1;
            stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
              (stateEstimatorEskf_DW.gpsValidCount + 1);
          }
        }
      }
      break;

     default:
      mode = enumStateEstimateMode::RUN_INIT_GPS;

      // During 'RUN_INIT_GPS': '<S5>:64'
      // '<S5>:66:1' sf_internal_predicateOutput = 0 | (isPosInitialized);
      if (stateEstimatorEskf_DW.isPosInitialized) {
        // Transition: '<S5>:66'
        stateEstimatorEskf_DW.durationCounter_1 = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorEskf_IN_RUN;

        // Entry 'RUN': '<S5>:43'
        // FULL EKF WITH GPS IS RUNNING
        // '<S5>:43:4' resetStates = false;
        stateEstimatorEskf_DW.resetStates = false;

        // '<S5>:43:5' mode = enumStateEstimateMode.RUN;
        mode = enumStateEstimateMode::RUN;

        // '<S5>:43:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:43:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:43:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:43:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S5>:43:10' latLonAltOut = latLonAltIn;
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
        latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
        latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
        stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
          stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
        stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
        latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

        // '<S5>:43:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:43:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // '<S5>:70:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
        // '<S5>:70:2' stateEstSmParams.gpsLossCheckDuration_s);
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_g) >=
            rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
          // Transition: '<S5>:70'
          stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
            stateEstimatorE_IN_RUN_GPS_LOST;

          // Entry 'RUN_GPS_LOST': '<S5>:67'
          // GPS WAS LOST
          // '<S5>:67:4' gpsValidCount = 0;
          stateEstimatorEskf_DW.gpsValidCount = 0U;

          // '<S5>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
          mode = enumStateEstimateMode::RUN_GPS_LOST;

          // '<S5>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
          // '<S5>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
          // '<S5>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S5>:67:10' latLonAltOut = latLonAltIn;
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
          latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
          latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
          latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

          // '<S5>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF RUNNING WITHOUT GPS BUT WE HAVE HEALTHY GPS SIGNAL
          // START INITIALIZING GPS
          // '<S5>:64:22' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S5>:64:23' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S5>:64:24' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:64:25' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S5>:64:26' latLonAltOut = latLonAltIn;
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];
          latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];
          latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
          stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];
          stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
            stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];
          stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];
          latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

          // '<S5>:64:27' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:64:28' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product1_b -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:64:29' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
          // Compute running mean of GPS data for NED origin Lat, Lon and Alt
          // '<S5>:64:32' if (isGpsDataValid)
          if (rtu_gpsData->isGpsDataValid) {
            // '<S5>:64:33' if( gpsIdx < max(stateEstSmParams.gpsInitCount, 1) ) 
            if (stateEstimatorEskf_DW.gpsIdx < std::fmax
                (rtu_stateEstSmParams->gpsInitCount, 1.0F)) {
              // '<S5>:64:34' gpsIdx = gpsIdx + 1;
              stateEstimatorEskf_DW.gpsIdx++;

              // '<S5>:64:35' llhDelta = (latLonAltIn - refLatLonAlt);
              // '<S5>:64:36' refLatLonAlt = refLatLonAlt + llhDelta/gpsIdx;
              stateEstimatorEskf_DW.refLatLonAlt[0] += (rtu_gpsData->latLonAlt[0]
                - stateEstimatorEskf_DW.refLatLonAlt[0]) /
                stateEstimatorEskf_DW.gpsIdx;
              stateEstimatorEskf_DW.refLatLonAlt[1] += (rtu_gpsData->latLonAlt[1]
                - stateEstimatorEskf_DW.refLatLonAlt[1]) /
                stateEstimatorEskf_DW.gpsIdx;
              stateEstimatorEskf_DW.refLatLonAlt[2] += (rtu_gpsData->latLonAlt[2]
                - stateEstimatorEskf_DW.refLatLonAlt[2]) /
                stateEstimatorEskf_DW.gpsIdx;
            }

            //
            // '<S5>:64:39' if(gpsIdx >= stateEstSmParams.gpsInitCount)
            if (stateEstimatorEskf_DW.gpsIdx >=
                rtu_stateEstSmParams->gpsInitCount) {
              // '<S5>:64:40' isPosInitialized = true;
              stateEstimatorEskf_DW.isPosInitialized = true;
            }
          }
        }
      }
      break;
    }
  }

  if (static_cast<boolean_T>(static_cast<int32_T>(rtu_gpsData->isGpsDataValid) ^
       1)) {
    stateEstimatorEskf_DW.durationCounter_1++;
    stateEstimatorEskf_DW.durationCounter_1_n++;
    stateEstimatorEskf_DW.durationCounter_1_g++;
  } else {
    stateEstimatorEskf_DW.durationCounter_1 = 0U;
    stateEstimatorEskf_DW.durationCounter_1_n = 0U;
    stateEstimatorEskf_DW.durationCounter_1_g = 0U;
  }

  // Product: '<S8>/Product' incorporates:
  //   Trigonometry: '<S8>/Cos'
  //   Trigonometry: '<S8>/Cos2'
  //   UnitDelay: '<Root>/Unit Delay1'

  rtb_Product2_c = std::cos(stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) * std::
    cos(stateEstimatorEskf_DW.UnitDelay1_DSTATE[1]);

  // RelationalOperator: '<S63>/Compare' incorporates:
  //   Constant: '<S63>/Constant'

  rtb_Compare = (mode == enumStateEstimateMode::RUN);

  // MATLAB Function: '<S7>/convertLlhToNedPos'
  // MATLAB Function 'latLonAltToNedPos/convertLlhToNedPos': '<S65>:1'
  // '<S65>:1:3' nedPos_m = convertLlhToNedPos_function(latLonAlt, refLatLonAlt, isGpsValid); 
  //  CONVERTLLHTONEDPOS_FUNCTION converts Latitude, Longitude, and Height (LLH) to 
  //  North-East-Down (nedPos_m) coordinates with the origin at refLlh.
  //
  //  Inputs:
  //  llh: [latRad, lonRad, height] in radians and meters
  //  refLlh: reference [latRad, lonRad, height] in radians and meters
  //  isGpsValid: Flag that indicates if gps is valid or not
  //
  //  Outputs:
  //  nedPos_m: [north, east, down] position in meters
  // 'convertLlhToNedPos_function:13' if(~isGpsValid)
  if (static_cast<boolean_T>(static_cast<int32_T>(rtb_Compare) ^ 1)) {
    // 'convertLlhToNedPos_function:14' nedPos_m = double([0; 0; 0]);
    rtb_nedPos_m_idx_0 = 0.0;
    nRef = 0.0;
    rtb_nedPos_m_idx_2 = 0.0;
  } else {
    //  Constants
    // 'convertLlhToNedPos_function:19' semiMajorAxis = double(6378137.0);
    //  WGS84 semi-major axis (meters)
    // 'convertLlhToNedPos_function:20' flattening = double(1 / 298.257223563);
    //  WGS84 flattening
    // 'convertLlhToNedPos_function:21' eccentricitySquared = 2 * flattening - flattening^2; 
    //  Square of the eccentricity
    //  Extract input values
    // 'convertLlhToNedPos_function:24' latRad = llh(1);
    // 'convertLlhToNedPos_function:25' lonRad = llh(2);
    // 'convertLlhToNedPos_function:26' height = llh(3);
    // 'convertLlhToNedPos_function:28' refLatRad = refLlh(1);
    // 'convertLlhToNedPos_function:29' refLonRad = refLlh(2);
    // 'convertLlhToNedPos_function:30' refHeight = refLlh(3);
    //  Calculate the prime vertical radius of curvature at the reference point
    // 'convertLlhToNedPos_function:33' nRef = semiMajorAxis / sqrt(1 - eccentricitySquared * sin(refLatRad)^2); 
    rtb_nedPos_m_idx_2 = std::sin(stateEstimatorEskf_DW.refLatLonAlt[0]);
    nRef = 6.378137E+6 / std::sqrt(1.0 - rtb_nedPos_m_idx_2 * rtb_nedPos_m_idx_2
      * 0.0066943799901413165);

    //  Calculate ECEF coordinates of the reference point
    // 'convertLlhToNedPos_function:36' refX = (nRef + refHeight) * cos(refLatRad) * cos(refLonRad); 
    // 'convertLlhToNedPos_function:37' refY = (nRef + refHeight) * cos(refLatRad) * sin(refLonRad); 
    // 'convertLlhToNedPos_function:38' refZ = ((nRef * (1 - eccentricitySquared)) + refHeight) * sin(refLatRad); 
    //  Calculate the prime vertical radius of curvature at the current point
    // 'convertLlhToNedPos_function:41' n = semiMajorAxis / sqrt(1 - eccentricitySquared * sin(latRad)^2); 
    rtb_nedPos_m_idx_0 = std::sin(latLonAltOut[0]);
    n = 6.378137E+6 / std::sqrt(1.0 - rtb_nedPos_m_idx_0 * rtb_nedPos_m_idx_0 *
      0.0066943799901413165);

    //  Calculate ECEF coordinates of the current point
    // 'convertLlhToNedPos_function:44' x = (n + height) * cos(latRad) * cos(lonRad); 
    // 'convertLlhToNedPos_function:45' y = (n + height) * cos(latRad) * sin(lonRad); 
    // 'convertLlhToNedPos_function:46' z = ((n * (1 - eccentricitySquared)) + height) * sin(latRad); 
    //  Compute the ECEF displacement from the reference point
    // 'convertLlhToNedPos_function:49' dx = x - refX;
    // 'convertLlhToNedPos_function:50' dy = y - refY;
    // 'convertLlhToNedPos_function:51' dz = z - refZ;
    //  Compute the rotation matrix from ECEF to nedPos_m
    // 'convertLlhToNedPos_function:54' R = [-sin(refLatRad) * cos(refLonRad), -sin(refLatRad) * sin(refLonRad), cos(refLatRad); 
    // 'convertLlhToNedPos_function:55'      -sin(refLonRad), cos(refLonRad), 0; 
    // 'convertLlhToNedPos_function:56'      -cos(refLatRad) * cos(refLonRad), -cos(refLatRad) * sin(refLonRad), -sin(refLatRad)]; 
    //  Rotate the displacement vector to nedPos_m coordinates
    // 'convertLlhToNedPos_function:59' nedPos_m = R * [dx; dy; dz];
    rtb_nedPos_m_tmp = std::sin(stateEstimatorEskf_DW.refLatLonAlt[1]);
    rtb_nedPos_m_tmp_0 = std::cos(stateEstimatorEskf_DW.refLatLonAlt[1]);
    rtb_nedPos_m_tmp_1 = std::cos(stateEstimatorEskf_DW.refLatLonAlt[0]);
    n_idx_0_tmp = (n + latLonAltOut[2]) * std::cos(latLonAltOut[0]);
    n_idx_0_tmp_0 = (nRef + stateEstimatorEskf_DW.refLatLonAlt[2]) *
      rtb_nedPos_m_tmp_1;
    n_idx_0 = n_idx_0_tmp * std::cos(latLonAltOut[1]) - n_idx_0_tmp_0 *
      rtb_nedPos_m_tmp_0;
    n_idx_0_tmp = n_idx_0_tmp * std::sin(latLonAltOut[1]) - n_idx_0_tmp_0 *
      rtb_nedPos_m_tmp;
    n = (n * 0.99330562000985867 + latLonAltOut[2]) * rtb_nedPos_m_idx_0 - (nRef
      * 0.99330562000985867 + stateEstimatorEskf_DW.refLatLonAlt[2]) *
      rtb_nedPos_m_idx_2;
    nRef = -rtb_nedPos_m_tmp * n_idx_0 + rtb_nedPos_m_tmp_0 * n_idx_0_tmp;
    rtb_nedPos_m_idx_0 = (-rtb_nedPos_m_idx_2 * rtb_nedPos_m_tmp_0 * n_idx_0 +
                          -rtb_nedPos_m_idx_2 * rtb_nedPos_m_tmp * n_idx_0_tmp)
      + rtb_nedPos_m_tmp_1 * n;
    rtb_nedPos_m_idx_2 = (-rtb_nedPos_m_tmp_1 * rtb_nedPos_m_tmp_0 * n_idx_0 +
                          -rtb_nedPos_m_tmp_1 * rtb_nedPos_m_tmp * n_idx_0_tmp)
      + -rtb_nedPos_m_idx_2 * n;

    // 'convertLlhToNedPos_function:61' for idx = 1:3
    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(rtb_nedPos_m_idx_0) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      rtb_nedPos_m_idx_0 = 0.0;
    }

    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(nRef) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      nRef = 0.0;
    }

    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(rtb_nedPos_m_idx_2) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      rtb_nedPos_m_idx_2 = 0.0;
    }
  }

  // End of MATLAB Function: '<S7>/convertLlhToNedPos'

  // Sum: '<S10>/Sum2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay'

  rtb_UnitDelay_g = rtb_XAxis - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[10];
  rtb_Sum2_idx_1 = rtb_XAxis1 - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[11];
  rtb_Sum2_idx_2 = rtb_XAxis2 - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[12];

  // Product: '<S10>/Product1'
  rtb_Product1_b = rtu_mtf01pData->dist_m * rtb_Product2_c;

  // Product: '<S10>/Matrix Multiply1' incorporates:
  //   Product: '<S10>/Matrix Multiply'
  //   Product: '<S10>/Product'
  //   SignalConversion generated from: '<S10>/Vector Concatenate'
  //   Sum: '<S10>/Sum'
  //   Sum: '<S10>/Sum1'
  //   UnitDelay: '<Root>/Unit Delay2'
  //
  rtb_XAxis1 = ((rtu_mtf01pParams->sensorToBodyRot[0] *
                 rtu_mtf01pData->flowX_radps + rtu_mtf01pParams->
                 sensorToBodyRot[2] * rtu_mtf01pData->flowY_radps) -
                rtb_Sum2_idx_1) * rtb_Product1_b;
  rtb_XAxis2 = stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] * rtb_XAxis1;
  rtb_XAxis = stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] * rtb_XAxis1;
  rtb_XAxis1 = ((rtu_mtf01pParams->sensorToBodyRot[1] *
                 rtu_mtf01pData->flowX_radps + rtu_mtf01pParams->
                 sensorToBodyRot[3] * rtu_mtf01pData->flowY_radps) +
                rtb_UnitDelay_g) * rtb_Product1_b;

  // MATLAB Function: '<S71>/Compute Filter Numerator And Denominator' incorporates:
  //   MATLAB Function: '<S70>/Compute Filter Numerator And Denominator'

  //  Call the main function
  // MATLAB Function 'Discrete First Order Filter/Compute Filter Numerator And Denominator': '<S73>:1' 
  // '<S73>:1:4' [num, den] = computeFirstOrderFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
  //  This function computes the numerator and denominator of the discrete
  //  first order filter
  //
  // Inputs:
  // filterBandwidth_radps: Bandwidth of the filter
  // sampleTime_s: sampling time
  //
  // Outputs:
  // num: Numerator array for the discrete transfer function
  // den: Denominator array for the discrete transfer function
  // 'computeFirstOrderFilterNumAndDen_function:13' B0 = filterBandwidth_radps;
  // 'computeFirstOrderFilterNumAndDen_function:14' B1 = 0;
  // 'computeFirstOrderFilterNumAndDen_function:16' A0 = B0;
  // 'computeFirstOrderFilterNumAndDen_function:17' A1 = 1;
  // 'computeFirstOrderFilterNumAndDen_function:18' K = 2/sampleTime_s;
  // 'computeFirstOrderFilterNumAndDen_function:20' [num, den] = computeDiscreteTFNumAndDen_function([B0, B1], [A0, A1], K); 
  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  //  For 1st order system
  // 'computeDiscreteTFNumAndDen_function:23' normalizer = A(1) + A(2)*K;
  // 'computeDiscreteTFNumAndDen_function:24' b0 = (B(1) + B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:25' b1 = (B(1) - B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:27' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:28' a1 = (A(1) - A(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:29' num = [b0, b1];
  // 'computeDiscreteTFNumAndDen_function:30' den = [a0, a1];
  rtb_Product1_b = (rtu_mtf01pParams->filterBw_radps - 500.0F) /
    (rtu_mtf01pParams->filterBw_radps + 500.0F);

  // DiscreteTransferFcn: '<S71>/Discrete Transfer Fcn' incorporates:
  //   MATLAB Function: '<S71>/Compute Filter Numerator And Denominator'
  //   Product: '<S10>/Matrix Multiply1'
  //   Product: '<S67>/Element Product'
  //   Sum: '<S10>/Sum3'
  //   Sum: '<S67>/Sum'
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.DiscreteTransferFcn_tmp =
    ((stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] * rtb_XAxis1 + rtb_XAxis2) +
     (rtb_Sum2_idx_1 * rtu_mtf01pParams->posVector_m[2] -
      rtu_mtf01pParams->posVector_m[1] * rtb_Sum2_idx_2)) - rtb_Product1_b *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;

  // MATLAB Function: '<S71>/Compute Filter Numerator And Denominator' incorporates:
  //   MATLAB Function: '<S70>/Compute Filter Numerator And Denominator'

  rtb_XAxis2 = rtu_mtf01pParams->filterBw_radps /
    (rtu_mtf01pParams->filterBw_radps + 500.0F);

  // DiscreteTransferFcn: '<S71>/Discrete Transfer Fcn' incorporates:
  //   MATLAB Function: '<S71>/Compute Filter Numerator And Denominator'

  rtb_VectorConcatenate_i[0] = rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp + rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;

  // DiscreteTransferFcn: '<S70>/Discrete Transfer Fcn' incorporates:
  //   Product: '<S10>/Matrix Multiply1'
  //   Product: '<S67>/Element Product'
  //   Sum: '<S10>/Sum3'
  //   Sum: '<S67>/Sum'
  //   UnitDelay: '<Root>/Unit Delay2'

  //  Call the main function
  // MATLAB Function 'Discrete First Order Filter/Compute Filter Numerator And Denominator': '<S72>:1' 
  // '<S72>:1:4' [num, den] = computeFirstOrderFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
  //  This function computes the numerator and denominator of the discrete
  //  first order filter
  //
  // Inputs:
  // filterBandwidth_radps: Bandwidth of the filter
  // sampleTime_s: sampling time
  //
  // Outputs:
  // num: Numerator array for the discrete transfer function
  // den: Denominator array for the discrete transfer function
  // 'computeFirstOrderFilterNumAndDen_function:13' B0 = filterBandwidth_radps;
  // 'computeFirstOrderFilterNumAndDen_function:14' B1 = 0;
  // 'computeFirstOrderFilterNumAndDen_function:16' A0 = B0;
  // 'computeFirstOrderFilterNumAndDen_function:17' A1 = 1;
  // 'computeFirstOrderFilterNumAndDen_function:18' K = 2/sampleTime_s;
  // 'computeFirstOrderFilterNumAndDen_function:20' [num, den] = computeDiscreteTFNumAndDen_function([B0, B1], [A0, A1], K); 
  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  //  For 1st order system
  // 'computeDiscreteTFNumAndDen_function:23' normalizer = A(1) + A(2)*K;
  // 'computeDiscreteTFNumAndDen_function:24' b0 = (B(1) + B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:25' b1 = (B(1) - B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:27' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:28' a1 = (A(1) - A(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:29' num = [b0, b1];
  // 'computeDiscreteTFNumAndDen_function:30' den = [a0, a1];
  stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b =
    ((stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] * rtb_XAxis1 + rtb_XAxis) +
     (rtu_mtf01pParams->posVector_m[0] * rtb_Sum2_idx_2 - rtb_UnitDelay_g *
      rtu_mtf01pParams->posVector_m[2])) - rtb_Product1_b *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;
  rtb_VectorConcatenate_i[1] = rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b + rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;

  // BusCreator: '<Root>/Bus Creator'
  // This function generates a validity flag based to inform EKF when to not
  // use optical flow data
  // MATLAB Function 'opticalFlowToNeVel/getMtf01pValidity': '<S69>:1'
  // '<S69>:1:4' mtf01pValidityFlag = true;
  // Update flag based distance health
  // '<S69>:1:7' mtf01pValidityFlag = mtf01pValidityFlag & (mtf01pData.distPrecision <= mtf01pParams.distPrecisionThr) & ... 
  // '<S69>:1:8'     (mtf01pData.distStatus == 1) & (mtf01pData.dist_m > mtf01pParams.distLimit_m(1)) & (mtf01pData.dist_m < mtf01pParams.distLimit_m(2)); 
  // Update flag based on flow health
  // '<S69>:1:11' mtf01pValidityFlag = mtf01pValidityFlag & (mtf01pData.flowStatus == 1); 
  rty_stateEstimatorDebug->stateEstInitPct =
    stateEstimatorEskf_DW.stateEstInitPct;
  rty_stateEstimatorDebug->smMode = mode;

  // Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = stateEstimatorEskf_DW.resetStates |
    stateEstimatorEskf_DW.icLoad;
  if (stateEstimatorEskf_DW.icLoad) {
    std::memcpy(&stateEstimatorEskf_DW.Delay_DSTATE[0],
                &stateEstimatorEskf_DW.initialStates[0], 20U * sizeof(real32_T));
  }

  // Delay: '<S1>/Delay2'
  stateEstimatorEskf_DW.icLoad_g = stateEstimatorEskf_DW.resetStates |
    stateEstimatorEskf_DW.icLoad_g;
  if (stateEstimatorEskf_DW.icLoad_g) {
    stateEstimatorEskf_DW.Delay2_DSTATE[0] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[0];
    stateEstimatorEskf_DW.Delay2_DSTATE[1] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[1];
    stateEstimatorEskf_DW.Delay2_DSTATE[2] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[2];
    stateEstimatorEskf_DW.Delay2_DSTATE[3] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[3];
    stateEstimatorEskf_DW.Delay2_DSTATE[4] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[4];
    stateEstimatorEskf_DW.Delay2_DSTATE[5] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[5];
    stateEstimatorEskf_DW.Delay2_DSTATE[6] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[6];
    stateEstimatorEskf_DW.Delay2_DSTATE[7] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[7];
    stateEstimatorEskf_DW.Delay2_DSTATE[8] =
      stateEstimatorEskf_DW.initialDcmBodyToNed[8];
  }

  // MATLAB Function: '<S13>/DelayedHorizonBufferManager' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   BusCreator: '<Root>/Bus Creator1'
  //   Concatenate: '<S68>/Vector Concatenate'
  //   DataTypeConversion: '<S7>/Cast To Single'
  //   Gain: '<S8>/Gain'
  //   Logic: '<S10>/AND'
  //   Logic: '<S8>/AND'
  //   Logic: '<S8>/AND1'
  //   Logic: '<S8>/NOT'
  //   Logic: '<S8>/OR'
  //   MATLAB Function: '<S10>/getMtf01pValidity'
  //   Product: '<S8>/Product1'
  //   Product: '<S8>/Product2'
  //   Product: '<S8>/Product3'
  //   RelationalOperator: '<S8>/Less Than'
  //   RelationalOperator: '<S8>/Less Than1'
  //   Sum: '<S7>/Sum'
  //   Sum: '<S8>/Sum'
  //   Sum: '<S8>/Sum1'
  //   Trigonometry: '<S8>/Cos1'
  //   UnitDelay: '<Root>/Unit Delay1'
  //   UnitDelay: '<S7>/Unit Delay'

  // MATLAB Function 'EKF/DelayedHorizonPassthrough/DelayedHorizonBufferManager': '<S17>:1' 
  // '<S17>:1:2' [dhSensorIn, estSmModeOut] = delayedHorizonBufferManager_function(sensorIn, estSmMode, reset, ekfParams); 
  // DELAYEDHORIZONBUFFERMANAGER manages sensor buffers so the ESKF operates
  // at the delayed IMU horizon.
  // 'delayedHorizonBufferManager_function:7' dhSensorIn = sensorIn;
  // 'delayedHorizonBufferManager_function:9' [stateModeReady, delayedEstSmMode, ~] = ... 
  // 'delayedHorizonBufferManager_function:10'     stateFifo(estSmMode, ekfParams.stateFifoParams); 
  //
  //  Fixed-memory estimator-state delay FIFO.
  //
  //  Delays enumStateEstimateMode by the configured fixed GPS delay so that
  //  GPS processing can use the estimator state-machine mode corresponding
  //  to the GPS measurement epoch rather than the current-time mode.
  //
  //  Input:
  //
  //    currentMode:
  //        enumStateEstimateMode
  //
  //        Current estimator state-machine mode generated internally by the
  //        Simulink model.
  //
  //    stateFifoParams:
  //        Read-only configuration struct.
  //
  //  Required stateFifoParams field:
  //
  //    gpsDelaySamples : uint16
  //
  //        Number of estimator execution INTERVALS corresponding to the
  //        configured GPS delay.
  //
  //        Example:
  //
  //            estimator rate  = 250 Hz
  //            estimator dt    = 4 ms
  //            GPS delay       = 200 ms
  //
  //            gpsDelaySamples = 50
  //
  //  Outputs:
  //
  //    modeReady:
  //        True when a mode corresponding to the GPS-delayed epoch is
  //        available.
  //
  //    delayedMode:
  //        enumStateEstimateMode corresponding to the GPS-delayed epoch.
  //
  //    countOut:
  //        Number of modes remaining in the FIFO after this execution.
  //
  //  OPERATION
  //  ---------
  //
  //  During startup:
  //
  //        PUSH current mode every estimator execution.
  //
  //        Do not return a delayed mode until gpsDelaySamples intervals of
  //        history have accumulated.
  //
  //  Example for gpsDelaySamples = 50:
  //
  //        sample epochs:
  //
  //            0, 4, 8, ..., 196 ms
  //
  //        contain 50 samples but only 49 intervals.
  //
  //        After receiving the mode at 200 ms:
  //
  //            0, 4, 8, ..., 196, 200 ms
  //
  //        there are now 50 intervals between the oldest and newest modes.
  //
  //        The mode from 0 ms can therefore be returned.
  //
  //  Once primed:
  //
  //        PUSH current mode
  //        POP oldest delayed mode
  //
  //        Exactly one mode enters and one delayed mode leaves on every
  //        estimator execution.
  //
  //  ASSUMPTIONS
  //  -----------
  //
  //    - State estimator executes synchronously at a fixed rate.
  //    - currentMode is generated once per estimator execution.
  //    - GPS delay is fixed and represented by gpsDelaySamples.
  //    - No timestamp search is required.
  //    - Buffer is sized correctly for the configured delay.
  //
  //  Full-buffer behavior:
  //
  //    If the physical buffer becomes full, the newest mode overwrites the
  //    oldest mode. With correct configuration and normal synchronous
  //    operation this should not occur before the oldest mode is consumed.
  // 'stateFifo:100' MAX_SIZE = 64;
  //  -------------------------------------------------------------------------
  //  STATIC INITIALIZATION
  //  -------------------------------------------------------------------------
  // 'stateFifo:108' if isempty(head)
  //  Default outputs.
  // 'stateFifo:117' modeReady = false;
  stateModeReady = false;

  //  Hold current mode on the output until delayed history becomes available.
  //  modeReady indicates whether delayedMode should actually be used.
  // 'stateFifo:121' delayedMode = currentMode;
  delayedEstSmMode = mode;

  // 'stateFifo:123' countOut = count;
  //  Read-only configuration alias.
  // 'stateFifo:126' delaySamples = stateFifoParams.gpsDelaySamples;
  //  A delay of N intervals requires temporary storage for N+1 sampled modes
  //  because the newest mode is pushed before the oldest delayed mode is
  //  returned.
  //
  //  Therefore:
  //
  //        delaySamples <= MAX_SIZE - 1
  // 'stateFifo:135' if delaySamples >= MAX_SIZE
  //  -------------------------------------------------------------------------
  //  PUSH CURRENT ESTIMATOR MODE
  //  -------------------------------------------------------------------------
  // 'stateFifo:143' bufferWasFull = (count == MAX_SIZE);
  // 'stateFifo:145' modeBuf(head) = currentMode;
  stateEstimatorEskf_DW.modeBuf[stateEstimatorEskf_DW.head - 1] = mode;

  //  Advance write pointer.
  // 'stateFifo:148' if head == MAX_SIZE
  if (stateEstimatorEskf_DW.head == 64) {
    // 'stateFifo:149' head = uint16(1);
    stateEstimatorEskf_DW.head = 1U;
  } else {
    // 'stateFifo:150' else
    // 'stateFifo:151' head = head + 1;
    qY = stateEstimatorEskf_DW.head + 1U;
    if (stateEstimatorEskf_DW.head + 1U > 65535U) {
      qY = 65535U;
    }

    stateEstimatorEskf_DW.head = static_cast<uint16_T>(qY);
  }

  // 'stateFifo:154' if bufferWasFull
  if (stateEstimatorEskf_DW.count == 64) {
    //  New mode replaced the oldest stored mode.
    // 'stateFifo:156' if tail == MAX_SIZE
    if (stateEstimatorEskf_DW.tail == 64) {
      // 'stateFifo:157' tail = uint16(1);
      stateEstimatorEskf_DW.tail = 1U;
    } else {
      // 'stateFifo:158' else
      // 'stateFifo:159' tail = tail + 1;
      qY = stateEstimatorEskf_DW.tail + 1U;
      if (stateEstimatorEskf_DW.tail + 1U > 65535U) {
        qY = 65535U;
      }

      stateEstimatorEskf_DW.tail = static_cast<uint16_T>(qY);
    }

    // 'stateFifo:162' count = uint16(MAX_SIZE);
  } else {
    // 'stateFifo:163' else
    // 'stateFifo:164' count = count + 1;
    qY = stateEstimatorEskf_DW.count + 1U;
    if (stateEstimatorEskf_DW.count + 1U > 65535U) {
      qY = 65535U;
    }

    stateEstimatorEskf_DW.count = static_cast<uint16_T>(qY);
  }

  //  -------------------------------------------------------------------------
  //  DELAY-LINE PRIMING
  //  -------------------------------------------------------------------------
  //  gpsDelaySamples represents time INTERVALS, not the number of stored
  //  endpoints.
  //
  //  Therefore, for gpsDelaySamples = 50:
  //
  //        count = 1 ... 50  -> no delayed output
  //        count = 51        -> first delayed output
  // 'stateFifo:178' if count <= delaySamples
  if (stateEstimatorEskf_DW.count > 50) {
    //  ------------------------------------------------------------------------- 
    //  POP GPS-DELAYED ESTIMATOR MODE
    //  ------------------------------------------------------------------------- 
    //  Tail is the estimator mode corresponding to the GPS-delayed epoch.
    // 'stateFifo:188' delayedMode = modeBuf(tail);
    delayedEstSmMode = stateEstimatorEskf_DW.modeBuf[stateEstimatorEskf_DW.tail
      - 1];

    // 'stateFifo:189' modeReady = true;
    stateModeReady = true;

    //  Consume returned delayed mode.
    // 'stateFifo:192' if tail == MAX_SIZE
    if (stateEstimatorEskf_DW.tail == 64) {
      // 'stateFifo:193' tail = uint16(1);
      stateEstimatorEskf_DW.tail = 1U;
    } else {
      // 'stateFifo:194' else
      // 'stateFifo:195' tail = tail + 1;
      qY = stateEstimatorEskf_DW.tail + 1U;
      if (stateEstimatorEskf_DW.tail + 1U > 65535U) {
        qY = 65535U;
      }

      stateEstimatorEskf_DW.tail = static_cast<uint16_T>(qY);
    }

    // 'stateFifo:198' count = count - 1;
    qY = stateEstimatorEskf_DW.count -
      /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
      /*MW:OvSatOk*/ 1U;
    if (stateEstimatorEskf_DW.count - 1U > stateEstimatorEskf_DW.count) {
      qY = 0U;
    }

    stateEstimatorEskf_DW.count = static_cast<uint16_T>(qY);

    // 'stateFifo:199' countOut = count;
  } else {
    // 'stateFifo:179' countOut = count;
  }

  // 'delayedHorizonBufferManager_function:12' if stateModeReady
  if (stateModeReady) {
    // 'delayedHorizonBufferManager_function:13' estSmModeOut = delayedEstSmMode; 
    mode = delayedEstSmMode;
  } else {
    // 'delayedHorizonBufferManager_function:14' else
    // 'delayedHorizonBufferManager_function:15' estSmModeOut = estSmMode;
  }

  // 'delayedHorizonBufferManager_function:18' [imuReady, imuTimeOut_ms, imuOut, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:19'     imuFifo(sensorIn.sensorValidity.isImuValid, ... 
  // 'delayedHorizonBufferManager_function:20'             sensorIn.sensorTimestamp.imuTimestamp_ms, ... 
  // 'delayedHorizonBufferManager_function:21'             [sensorIn.bodyAccels_mps2; ... 
  // 'delayedHorizonBufferManager_function:22'              sensorIn.bodyRates_radps; ... 
  // 'delayedHorizonBufferManager_function:23'              sensorIn.dtImuTime_s], ... 
  // 'delayedHorizonBufferManager_function:24'             ekfParams.imuFifoParams); 
  //
  //  Fixed-memory IMU delay FIFO for delayed-horizon ESKF propagation.
  //
  //  Runtime inputs:
  //    isImuValid    : boolean pulse. True for one model step when a NEW
  //                    IMU measurement is available.
  //
  //    imuTimeIn_ms  : uint64 timestamp of current IMU measurement [ms].
  //
  //    imuIn         : single [7x1]
  //                    [ax; ay; az; wx; wy; wz]
  //
  //                    ax,ay,az : body-frame acceleration [m/s^2]
  //                    wx,wy,wz : body-frame angular rate [rad/s]
  //                    dt: delta time between imu readings in sec
  //
  //    imuFifoParams : read-only configuration struct.
  //
  //  Required imuFifoParams fields:
  //    imuPeriod_ms  : uint64 nominal IMU period [ms].
  //                    Example at 250 Hz: 4 ms.
  //
  //    delaySamples  : uint16 number of IMU intervals between current time
  //                    and delayed ESKF fusion horizon.
  //
  //                    Example:
  //                        imuPeriod_ms = 4
  //                        delaySamples = 50
  //
  //                    Corresponding delay horizon:
  //
  //                        50 * 4 ms = 200 ms
  //
  //  Outputs:
  //    imuReady       : true when one delayed IMU sample is returned.
  //
  //    imuTimeOut_ms  : timestamp of returned delayed IMU measurement.
  //                     This defines the delayed ESKF fusion time.
  //
  //    imuOut         : single [7x1]
  //                     [ax; ay; az; wx; wy; wz; dt]
  //
  //    status         : diagnostic status code.
  //
  //    countOut       : number of samples retained after this call.
  //
  //  Assumptions:
  //    - IMU timestamps are monotonically increasing.
  //    - isImuValid is a one-step NEW-DATA pulse.
  //    - Producer and estimator execute at the same nominal rate.
  //    - Every delayed IMU sample is consumed once.
  //    - Buffer is sized sufficiently for the configured delay.
  //
  //  FIFO operation:
  //
  //    During startup:
  //
  //        push current IMU samples
  //        do not output until delaySamples intervals of history exist
  //
  //    Once primed:
  //
  //        push newest current-time IMU sample
  //        pop oldest delayed IMU sample
  //
  //    In normal steady-state operation exactly one sample enters and one
  //    sample leaves on every valid IMU step.
  //
  //  Full-buffer behavior:
  //    If the FIFO is full, the newest sample overwrites the oldest sample.
  //    With correct sizing and normal estimator execution this should never
  //    occur before the oldest sample has already been consumed.
  // 'imuFifo:77' MAX_SIZE = 64;
  // 'imuFifo:79' STATUS_OK           = uint8(0);
  // 'imuFifo:80' STATUS_OVERWRITE    = uint8(1);
  // 'imuFifo:81' STATUS_BAD_CONFIG   = uint8(2);
  //  Static FIFO allocation/state initialization.
  // 'imuFifo:87' if isempty(head)
  //  Default outputs.
  // 'imuFifo:97' imuReady = false;
  stateModeReady = false;

  // 'imuFifo:98' imuTimeOut_ms = uint64(0);
  fusionTime_ms = dhSensorIn_sensorTimestamp_ma_0;

  // 'imuFifo:99' imuOut = zeros(7,1,'single');
  imuOut[0] = 0.0F;
  imuOut[1] = 0.0F;
  imuOut[2] = 0.0F;
  imuOut[3] = 0.0F;
  imuOut[4] = 0.0F;
  imuOut[5] = 0.0F;
  imuOut[6] = 0.0F;

  // 'imuFifo:100' status = STATUS_OK;
  // 'imuFifo:101' countOut = count;
  //  Read-only configuration aliases.
  // 'imuFifo:104' imuPeriod_ms = imuFifoParams.imuPeriod_ms;
  // 'imuFifo:105' delaySamples = imuFifoParams.delaySamples;
  //  imuPeriod_ms is supplied as a system configuration parameter. It is not
  //  required by the FIFO indexing itself, but retaining it here keeps the
  //  configured temporal horizon explicit:
  //
  //    delayHorizon_ms = delaySamples * imuPeriod_ms
  //
  //  A delay of N intervals requires storage for N+1 timestamped samples
  //  before the oldest sample can first be released.
  // 'imuFifo:115' if imuPeriod_ms == 0 || delaySamples >= MAX_SIZE
  if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<int32_T>
        (uMultiWordEq
         (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.imuFifoParams.imuPeriod_ms.chunks
          [0], &dhSensorIn_sensorTimestamp_ma_0.chunks[0U], 2)) ^ 1) &
       rtu_imuData->isImuDataValid)) {
    //  No new IMU measurement this estimator step.
    // 'imuFifo:121' if ~isImuValid
    //  ------------------------------------------------------------------------- 
    //  PUSH CURRENT-TIME IMU MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'imuFifo:129' bufferWasFull = (count == MAX_SIZE);
    //  Store complete IMU observation at current write location.
    // 'imuFifo:132' timeBuf_ms(head) = imuTimeIn_ms;
    stateEstimatorEskf_DW.timeBuf_ms[stateEstimatorEskf_DW.head_e - 1] =
      rtu_imuData->timestamp_ms;

    // 'imuFifo:133' dataBuf(:,head) = imuIn;
    rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.head_e - 1) * 7;
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp] =
      stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 3] =
      stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 1] =
      stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 4] =
      stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 2] =
      stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 5] =
      stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2];
    stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 6] =
      rtu_imuData->dtImuTime_s;

    //  Advance write pointer.
    // 'imuFifo:136' if head == MAX_SIZE
    if (stateEstimatorEskf_DW.head_e == 64) {
      // 'imuFifo:137' head = uint16(1);
      stateEstimatorEskf_DW.head_e = 1U;
    } else {
      // 'imuFifo:138' else
      // 'imuFifo:139' head = head + 1;
      qY = stateEstimatorEskf_DW.head_e + 1U;
      if (stateEstimatorEskf_DW.head_e + 1U > 65535U) {
        qY = 65535U;
      }

      stateEstimatorEskf_DW.head_e = static_cast<uint16_T>(qY);
    }

    // 'imuFifo:142' if bufferWasFull
    if (stateEstimatorEskf_DW.count_g == 64) {
      //  New sample replaced the oldest stored IMU sample.
      //
      //  Under correctly sized steady-state operation this should not occur
      //  before the oldest sample has already been consumed.
      // 'imuFifo:147' if tail == MAX_SIZE
      if (stateEstimatorEskf_DW.tail_g == 64) {
        // 'imuFifo:148' tail = uint16(1);
        stateEstimatorEskf_DW.tail_g = 1U;
      } else {
        // 'imuFifo:149' else
        // 'imuFifo:150' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_g + 1U;
        if (stateEstimatorEskf_DW.tail_g + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_g = static_cast<uint16_T>(qY);
      }

      // 'imuFifo:153' count = uint16(MAX_SIZE);
      // 'imuFifo:154' status = STATUS_OVERWRITE;
    } else {
      // 'imuFifo:155' else
      // 'imuFifo:156' count = count + 1;
      qY = stateEstimatorEskf_DW.count_g + 1U;
      if (stateEstimatorEskf_DW.count_g + 1U > 65535U) {
        qY = 65535U;
      }

      stateEstimatorEskf_DW.count_g = static_cast<uint16_T>(qY);
    }

    //  ------------------------------------------------------------------------- 
    //  DELAY-LINE PRIMING
    //  ------------------------------------------------------------------------- 
    //  delaySamples represents the number of IMU intervals between the delayed
    //  fusion horizon and current time.
    //
    //  Example:
    //
    //    imuPeriod_ms = 4
    //    delaySamples = 50
    //
    //  The FIFO must receive 51 timestamped samples before the first sample can 
    //  be released:
    //
    //    0, 4, 8, ..., 196, 200 ms
    //
    //  There are 50 intervals between 0 ms and 200 ms.
    // 'imuFifo:177' if count <= delaySamples
    if (stateEstimatorEskf_DW.count_g > 50) {
      //  ------------------------------------------------------------------------- 
      //  POP OLDEST DELAYED IMU MEASUREMENT
      //  ------------------------------------------------------------------------- 
      //  Tail points to the oldest valid IMU measurement.
      // 'imuFifo:187' imuTimeOut_ms = timeBuf_ms(tail);
      fusionTime_ms =
        stateEstimatorEskf_DW.timeBuf_ms[stateEstimatorEskf_DW.tail_g - 1];

      // 'imuFifo:188' imuOut = dataBuf(:,tail);
      rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.tail_g - 1) * 7;
      imuOut[0] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp];
      imuOut[1] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 1];
      imuOut[2] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 2];
      imuOut[3] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 3];
      imuOut[4] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 4];
      imuOut[5] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 5];
      imuOut[6] = stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 6];

      // 'imuFifo:189' imuReady = true;
      stateModeReady = true;

      //  Consume returned measurement.
      // 'imuFifo:192' if tail == MAX_SIZE
      if (stateEstimatorEskf_DW.tail_g == 64) {
        // 'imuFifo:193' tail = uint16(1);
        stateEstimatorEskf_DW.tail_g = 1U;
      } else {
        // 'imuFifo:194' else
        // 'imuFifo:195' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_g + 1U;
        if (stateEstimatorEskf_DW.tail_g + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_g = static_cast<uint16_T>(qY);
      }

      // 'imuFifo:198' count = count - 1;
      qY = stateEstimatorEskf_DW.count_g -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_g - 1U > stateEstimatorEskf_DW.count_g) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_g = static_cast<uint16_T>(qY);

      // 'imuFifo:199' countOut = count;
    } else {
      // 'imuFifo:178' countOut = count;
    }
  } else {
    // 'imuFifo:116' status = STATUS_BAD_CONFIG;
  }

  dhSensorIn_sensorTimestamp_imuT = fusionTime_ms;

  // 'delayedHorizonBufferManager_function:26' dhSensorIn.sensorValidity.isImuValid = imuReady; 
  // 'delayedHorizonBufferManager_function:27' dhSensorIn.sensorTimestamp.imuTimestamp_ms = imuTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:28' dhSensorIn.bodyAccels_mps2 = imuOut(1:3); 
  // 'delayedHorizonBufferManager_function:29' dhSensorIn.bodyRates_radps = imuOut(4:6); 
  // 'delayedHorizonBufferManager_function:30' dhSensorIn.dtImuTime_s = imuOut(7); 
  // 'delayedHorizonBufferManager_function:32' if imuReady
  if (static_cast<boolean_T>(static_cast<int32_T>(stateModeReady) ^ 1)) {
    // 'delayedHorizonBufferManager_function:34' else
    // 'delayedHorizonBufferManager_function:35' fusionTime_ms = uint64(0);
    fusionTime_ms = dhSensorIn_sensorTimestamp_ma_0;
  } else {
    // 'delayedHorizonBufferManager_function:33' fusionTime_ms = imuTimeOut_ms;
  }

  // 'delayedHorizonBufferManager_function:38' magDelay_ms   = ekfParams.magDelay_ms; 
  // 'delayedHorizonBufferManager_function:39' gpsDelay_ms   = ekfParams.gpsDelay_ms; 
  // 'delayedHorizonBufferManager_function:40' baroDelay_ms  = ekfParams.baroDelay_ms; 
  // 'delayedHorizonBufferManager_function:41' lidarDelay_ms = ekfParams.lidarDelay_ms; 
  // 'delayedHorizonBufferManager_function:42' ofDelay_ms    = ekfParams.ofDelay_ms; 
  // 'delayedHorizonBufferManager_function:44' if sensorIn.sensorTimestamp.magTimestamp_ms >= magDelay_ms 
  if (uMultiWordGe(&rtu_magData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:45' magTimeIn_ms = sensorIn.sensorTimestamp.magTimestamp_ms - magDelay_ms; 
    uMultiWord2MultiWord(&rtu_magData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &magTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:46' else
    // 'delayedHorizonBufferManager_function:47' magTimeIn_ms = uint64(0);
    magTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:50' if sensorIn.sensorTimestamp.gpsTimestamp_ms >= gpsDelay_ms 
  if (uMultiWordGe(&rtu_gpsData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:51' gpsTimeIn_ms = sensorIn.sensorTimestamp.gpsTimestamp_ms - gpsDelay_ms; 
    uMultiWord2MultiWord(&rtu_gpsData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &gpsTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:52' else
    // 'delayedHorizonBufferManager_function:53' gpsTimeIn_ms = uint64(0);
    gpsTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:56' if sensorIn.sensorTimestamp.baroTimestamp_ms >= baroDelay_ms 
  if (uMultiWordGe(&rtu_baroData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:57' baroTimeIn_ms = sensorIn.sensorTimestamp.baroTimestamp_ms - baroDelay_ms; 
    uMultiWord2MultiWord(&rtu_baroData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &baroTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:58' else
    // 'delayedHorizonBufferManager_function:59' baroTimeIn_ms = uint64(0);
    baroTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:62' if sensorIn.sensorTimestamp.lidarTimestamp_ms >= lidarDelay_ms 
  if (uMultiWordGe(&rtu_lidarData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:63' lidarTimeIn_ms = sensorIn.sensorTimestamp.lidarTimestamp_ms - lidarDelay_ms; 
    uMultiWord2MultiWord(&rtu_lidarData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &lidarTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:64' else
    // 'delayedHorizonBufferManager_function:65' lidarTimeIn_ms = uint64(0);
    lidarTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:68' if sensorIn.sensorTimestamp.ofTimestamp_ms >= ofDelay_ms 
  if (uMultiWordGe(&rtu_mtf01pData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.ofDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:69' ofTimeIn_ms = sensorIn.sensorTimestamp.ofTimestamp_ms - ofDelay_ms; 
    uMultiWord2MultiWord(&rtu_mtf01pData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.ofDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &ofTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:70' else
    // 'delayedHorizonBufferManager_function:71' ofTimeIn_ms = uint64(0);
    ofTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:74' [magReady, magTimeOut_ms, magOut_uT, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:75'     magFifo(sensorIn.sensorValidity.isMagValid, ... 
  // 'delayedHorizonBufferManager_function:76'             magTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:77'             sensorIn.normMagVec_nd, ... 
  // 'delayedHorizonBufferManager_function:78'             fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:79'             ekfParams.magFifoParams); 
  stateEstimatorEskf_magFifo
    (stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid, magTimeIn_ms,
     stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd, fusionTime_ms,
     stateEstimatorEskf_DW.resetStates,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.capacity,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.minInterval_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.maxAge_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.resetThreshold_ms,
     &dhSensorIn_sensorValidity_isMag, &dhSensorIn_sensorTimestamp_magT, Divide,
     &a__4, &nextIdx);

  // 'delayedHorizonBufferManager_function:81' dhSensorIn.sensorValidity.isMagValid = magReady; 
  // 'delayedHorizonBufferManager_function:82' dhSensorIn.sensorTimestamp.magTimestamp_ms = magTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:83' dhSensorIn.normMagVec_nd = magOut_uT; 
  // 'delayedHorizonBufferManager_function:85' [gpsReady, gpsTimeOut_ms, posVelOut, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:86'     gpsFifo(sensorIn.sensorValidity.isGpsValid, ... 
  // 'delayedHorizonBufferManager_function:87'             gpsTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:88'             [sensorIn.nedPosAndVel.pos_m; ... 
  // 'delayedHorizonBufferManager_function:89'              sensorIn.nedPosAndVel.vel_mps], ... 
  // 'delayedHorizonBufferManager_function:90'             fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:91'             ekfParams.gpsFifoParams); 
  rtb_CastToSingle_0[0] = static_cast<real32_T>(rtb_nedPos_m_idx_0);
  rtb_CastToSingle_0[1] = static_cast<real32_T>(nRef);
  rtb_CastToSingle_0[2] = static_cast<real32_T>(rtb_nedPos_m_idx_2) +
    stateEstimatorEskf_DW.UnitDelay_DSTATE;
  rtb_CastToSingle_0[3] = rtu_gpsData->nedVel_mps[0];
  rtb_CastToSingle_0[4] = rtu_gpsData->nedVel_mps[1];
  rtb_CastToSingle_0[5] = rtu_gpsData->nedVel_mps[2];
  stateEstimatorEskf_gpsFifo
    (stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid, gpsTimeIn_ms,
     rtb_CastToSingle_0, fusionTime_ms, stateEstimatorEskf_DW.resetStates,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsFifoParams.capacity,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsFifoParams.minInterval_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsFifoParams.maxAge_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsFifoParams.resetThreshold_ms,
     &dhSensorIn_sensorValidity_isGps, &magTimeIn_ms, rtb_ElementProduct, &a__4,
     &nextIdx);

  // 'delayedHorizonBufferManager_function:93' dhSensorIn.sensorValidity.isGpsValid = gpsReady; 
  // 'delayedHorizonBufferManager_function:94' dhSensorIn.sensorTimestamp.gpsTimestamp_ms = gpsTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:95' dhSensorIn.nedPosAndVel.pos_m = posVelOut(1:3); 
  // 'delayedHorizonBufferManager_function:96' dhSensorIn.nedPosAndVel.vel_mps = posVelOut(4:6); 
  // 'delayedHorizonBufferManager_function:98' [baroReady, baroTimeOut_ms, baroAltOut_m, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:99'     baroFifo(sensorIn.sensorValidity.isBaroValid, ... 
  // 'delayedHorizonBufferManager_function:100'              baroTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:101'              sensorIn.baroAlt_m, ... 
  // 'delayedHorizonBufferManager_function:102'              fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:103'              ekfParams.baroFifoParams); 
  //
  //  Barometer fixed-memory FIFO for delayed-horizon ESKF fusion.
  //
  //  Runtime inputs:
  //    isBaroValid      : boolean pulse. True for one model step when a NEW
  //                       valid barometer measurement is available.
  //    baroTimeIn_ms    : uint64 corrected barometer measurement epoch [ms].
  //    altitudeIn_m     : single scalar barometric altitude [m].
  //    fusionTime_ms    : uint64 delayed ESKF fusion horizon [ms].
  //    reset            : boolean. Clears FIFO and timestamp history.
  //    baroFifoParams   : read-only configuration struct.
  //
  //  Required baroFifoParams fields:
  //    baroCapacity              : uint16
  //    baroMaxAge_ms             : uint64
  //    baroMinObsInterval_ms     : uint64
  //    baroTimeResetThreshold_ms : uint64
  //
  //  Outputs:
  //    baroReady       : true when a barometer measurement is returned.
  //    baroTimeOut_ms  : timestamp of returned measurement.
  //    altitudeOut_m   : returned barometric altitude [m].
  //    status          : diagnostic status code.
  //    countOut        : number of measurements remaining in FIFO.
  //
  //  FIFO behavior:
  //    - Static allocation, no dynamic memory.
  //    - Newest measurement overwrites oldest when FIFO is full.
  //    - Duplicate/small backward timestamps are rejected.
  //    - Large backward timestamp jump resets the barometer time epoch.
  //    - Measurements arriving too quickly can be throttled.
  //    - POP returns the newest measurement satisfying:
  //
  //          baroTime <= fusionTime_ms
  //
  //    - Older eligible measurements are discarded.
  //    - Selected measurements older than baroMaxAge_ms are discarded.
  //
  //  Important:
  //    isBaroValid must behave as a NEW-DATA pulse. It must not remain true
  //    continuously while the same barometer sample is held at the inputs.
  // 'baroFifo:47' MAX_SIZE = 24;
  // 'baroFifo:49' STATUS_OK             = uint8(0);
  // 'baroFifo:50' STATUS_BAD_TIMESTAMP  = uint8(1);
  // 'baroFifo:51' STATUS_OVERWRITE      = uint8(2);
  // 'baroFifo:52' STATUS_TIMEBASE_RESET = uint8(3);
  // 'baroFifo:53' STATUS_STALE_DISCARD  = uint8(4);
  // 'baroFifo:54' STATUS_BAD_CAPACITY   = uint8(5);
  //  Static FIFO storage/state initialization.
  // 'baroFifo:61' if isempty(head)
  //  Default outputs.
  // 'baroFifo:72' baroReady = false;
  dhSensorIn_sensorValidity_isBar = false;

  // 'baroFifo:73' baroTimeOut_ms = uint64(0);
  // 'baroFifo:74' altitudeOut_m = single(0);
  rtb_XAxis = 0.0F;

  // 'baroFifo:75' status = STATUS_OK;
  // 'baroFifo:76' countOut = count;
  //  Read-only configuration aliases.
  // 'baroFifo:79' capacity = baroFifoParams.capacity;
  // 'baroFifo:80' maxAge_ms = baroFifoParams.maxAge_ms;
  // 'baroFifo:81' minObsInterval_ms = baroFifoParams.minInterval_ms;
  // 'baroFifo:82' timeResetThreshold_ms = baroFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'baroFifo:85' if capacity < 1 || capacity > MAX_SIZE
  //  Explicit estimator/FIFO reset.
  // 'baroFifo:91' if reset
  if (stateEstimatorEskf_DW.resetStates) {
    // 'baroFifo:92' head = uint16(1);
    stateEstimatorEskf_DW.head_c = 1U;

    // 'baroFifo:93' tail = uint16(1);
    stateEstimatorEskf_DW.tail_i = 1U;

    // 'baroFifo:94' count = uint16(0);
    stateEstimatorEskf_DW.count_k = 0U;

    // 'baroFifo:95' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_g = dhSensorIn_sensorTimestamp_ma_0;

    // 'baroFifo:96' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_p = false;

    // 'baroFifo:97' countOut = count;
  } else {
    //  ------------------------------------------------------------------------- 
    //  PUSH NEW BAROMETER MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'baroFifo:104' if isBaroValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid) {
      //  Barometer timestamps must normally increase monotonically.
      // 'baroFifo:106' if haveLastPushTime && baroTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_p &
           uMultiWordLe(&baroTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_g.chunks[0U], 2)))
      {
        // 'baroFifo:107' backwardsJump_ms = lastPushTime_ms - baroTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'baroFifo:110' if timeResetThreshold_ms > 0 && ...
        // 'baroFifo:111'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_g.chunks[0U],
                             2, &tmp_7.chunks[0U], 3);
        uMultiWord2MultiWord(&baroTimeIn_ms.chunks[0U], 2, &tmp_8.chunks[0U], 3);
        MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &tmp_9.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.resetThreshold_ms.chunks
              [0], &dhSensorIn_sensorTimestamp_ma_0.chunks[0U], 2) &
             uMultiWordGe(&tmp_9.chunks[0U],
                          &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.resetThreshold_ms.chunks
                          [0], 2))) {
          // 'baroFifo:112' head = uint16(1);
          stateEstimatorEskf_DW.head_c = 1U;

          // 'baroFifo:113' tail = uint16(1);
          stateEstimatorEskf_DW.tail_i = 1U;

          // 'baroFifo:114' count = uint16(0);
          stateEstimatorEskf_DW.count_k = 0U;

          // 'baroFifo:115' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_g =
            dhSensorIn_sensorTimestamp_ma_0;

          // 'baroFifo:116' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_p = false;

          // 'baroFifo:117' status = STATUS_TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'baroFifo:118' else
          //  Duplicate timestamp or small backward jump.
          // 'baroFifo:120' status = STATUS_BAD_TIMESTAMP;
        }
      } else {
        guard3 = true;
      }
    } else {
      guard2 = true;
    }

    if (guard3) {
      //  Optional rate throttling.
      //  minObsInterval_ms == 0 disables throttling.
      // 'baroFifo:127' if haveLastPushTime && ...
      // 'baroFifo:128'             (baroTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&baroTimeIn_ms.chunks[0U], 2, &tmp_7.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_g.chunks[0U],
                           2, &tmp_8.chunks[0U], 3);
      MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &tmp_a.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_p &
           uMultiWordLt(&tmp_a.chunks[0U],
                        &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.minInterval_ms.chunks
                        [0], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'baroFifo:133' bufferWasFull = (count == capacity);
        //  Store barometric altitude and its measurement timestamp.
        // 'baroFifo:136' timeBuf_ms(head) = baroTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.head_c - 1] =
          baroTimeIn_ms;

        // 'baroFifo:137' altitudeBuf_m(head) = altitudeIn_m;
        stateEstimatorEskf_DW.altitudeBuf_m[stateEstimatorEskf_DW.head_c - 1] =
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m;

        //  Advance write location.
        // 'baroFifo:140' if head == capacity
        if (stateEstimatorEskf_DW.head_c == 16) {
          // 'baroFifo:141' head = uint16(1);
          stateEstimatorEskf_DW.head_c = 1U;
        } else {
          // 'baroFifo:142' else
          // 'baroFifo:143' head = head + 1;
          qY = stateEstimatorEskf_DW.head_c + 1U;
          if (stateEstimatorEskf_DW.head_c + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_c = static_cast<uint16_T>(qY);
        }

        // 'baroFifo:146' if bufferWasFull
        if (stateEstimatorEskf_DW.count_k == 16) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'baroFifo:149' if tail == capacity
          if (stateEstimatorEskf_DW.tail_i == 16) {
            // 'baroFifo:150' tail = uint16(1);
            stateEstimatorEskf_DW.tail_i = 1U;
          } else {
            // 'baroFifo:151' else
            // 'baroFifo:152' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_i + 1U;
            if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'baroFifo:156' count = capacity;
          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'baroFifo:159' if status == STATUS_OK
        } else {
          // 'baroFifo:162' else
          //  One new measurement was added without replacing anything.
          // 'baroFifo:164' count = count + 1;
          qY = stateEstimatorEskf_DW.count_k + 1U;
          if (stateEstimatorEskf_DW.count_k + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'baroFifo:168' lastPushTime_ms = baroTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_g = baroTimeIn_ms;

        // 'baroFifo:169' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_p = true;

        // 'baroFifo:170' countOut = count;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST BAROMETER MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'baroFifo:178' if count == 0
      if ((stateEstimatorEskf_DW.count_k == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.tail_i - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'baroFifo:184' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ baroTime | baroTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'baroFifo:195' for k = 1:(MAX_SIZE-1)
        rtb_VectorConcatenate1_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((rtb_VectorConcatenate1_tmp <
                  23) & (stateEstimatorEskf_DW.count_k != 1)))) {
          // 'baroFifo:196' if count == 1
          // 'baroFifo:200' if tail == capacity
          if (stateEstimatorEskf_DW.tail_i == 16) {
            // 'baroFifo:201' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'baroFifo:202' else
            // 'baroFifo:203' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_i + 1U;
            if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'baroFifo:206' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_o[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'baroFifo:211' tail = nextIdx;
            stateEstimatorEskf_DW.tail_i = nextIdx;

            // 'baroFifo:212' count = count - 1;
            qY = stateEstimatorEskf_DW.count_k -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_k - 1U >
                stateEstimatorEskf_DW.count_k) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);
            rtb_VectorConcatenate1_tmp++;
          }
        }

        //  Tail now points to the newest eligible barometer measurement.
        // 'baroFifo:216' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'baroFifo:220' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt
            (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.maxAge_ms.chunks
             [0], &dhSensorIn_sensorTimestamp_ma_0.chunks[0U], 2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_7.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.tail_i -
             1].chunks[0U], 2, &tmp_8.chunks[0U], 3);
          MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &tmp_b.chunks[0U], 2);
          if (uMultiWordGe(&tmp_b.chunks[0U],
                           &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.maxAge_ms.chunks
                           [0], 2)) {
            // 'baroFifo:221' if tail == capacity
            if (stateEstimatorEskf_DW.tail_i == 16) {
              // 'baroFifo:222' tail = uint16(1);
              stateEstimatorEskf_DW.tail_i = 1U;
            } else {
              // 'baroFifo:223' else
              // 'baroFifo:224' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_i + 1U;
              if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
            }

            // 'baroFifo:227' count = count - 1;
            qY = stateEstimatorEskf_DW.count_k -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_k - 1U >
                stateEstimatorEskf_DW.count_k) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);

            // 'baroFifo:228' countOut = count;
            // 'baroFifo:229' status = STATUS_STALE_DISCARD;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      //  Return selected barometric altitude.
      // 'baroFifo:234' baroTimeOut_ms = timeBuf_ms(tail);
      // 'baroFifo:235' altitudeOut_m = altitudeBuf_m(tail);
      rtb_XAxis =
        stateEstimatorEskf_DW.altitudeBuf_m[stateEstimatorEskf_DW.tail_i - 1];

      // 'baroFifo:236' baroReady = true;
      dhSensorIn_sensorValidity_isBar = true;

      //  Consume returned measurement.
      // 'baroFifo:239' if tail == capacity
      if (stateEstimatorEskf_DW.tail_i == 16) {
        // 'baroFifo:240' tail = uint16(1);
        stateEstimatorEskf_DW.tail_i = 1U;
      } else {
        // 'baroFifo:241' else
        // 'baroFifo:242' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_i + 1U;
        if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
      }

      // 'baroFifo:245' count = count - 1;
      qY = stateEstimatorEskf_DW.count_k -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_k - 1U > stateEstimatorEskf_DW.count_k) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);

      // 'baroFifo:246' countOut = count;
    }
  }

  // 'delayedHorizonBufferManager_function:105' dhSensorIn.sensorValidity.isBaroValid = baroReady; 
  // 'delayedHorizonBufferManager_function:106' dhSensorIn.sensorTimestamp.baroTimestamp_ms = baroTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:107' dhSensorIn.baroAlt_m = baroAltOut_m; 
  // 'delayedHorizonBufferManager_function:109' [lidarReady, lidarTimeOut_ms, lidarAglOut_m, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:110'     lidarFifo(sensorIn.sensorValidity.isLidarValid, ... 
  // 'delayedHorizonBufferManager_function:111'               lidarTimeIn_ms, ... 
  // 'delayedHorizonBufferManager_function:112'               sensorIn.lidarAgl_m, ... 
  // 'delayedHorizonBufferManager_function:113'               fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:114'               ekfParams.lidarFifoParams); 
  stateEstimatorEskf_lidarFifo(static_cast<boolean_T>(static_cast<boolean_T>
    (rtu_lidarData->isLidarDataValid & rtu_lidarData->isLidarInitialized) &
    static_cast<boolean_T>((rtu_lidarData->range_m >=
    rtu_lidarParams->validRange_m[0]) & (rtu_lidarData->range_m <=
    rtu_lidarParams->validRange_m[1]))), lidarTimeIn_ms, rtb_Product2_c *
    rtu_lidarData->range_m - (rtu_lidarParams->yMntOff_m * -std::sin
    (stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) + rtu_lidarParams->zMntOff_m *
    rtb_Product2_c), fusionTime_ms, stateEstimatorEskf_DW.resetStates,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.capacity,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.minInterval_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.maxAge_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.resetThreshold_ms,
    &dhSensorIn_sensorValidity_isLid, &gpsTimeIn_ms, &rtb_Product1_b, &a__4,
    &nextIdx);

  // 'delayedHorizonBufferManager_function:116' dhSensorIn.sensorValidity.isLidarValid = lidarReady; 
  // 'delayedHorizonBufferManager_function:117' dhSensorIn.sensorTimestamp.lidarTimestamp_ms = lidarTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:118' dhSensorIn.lidarAgl_m = lidarAglOut_m; 
  // 'delayedHorizonBufferManager_function:120' [ofReady, ofTimeOut_ms, ofNeVelOut_mps, ~, ~] = ... 
  // 'delayedHorizonBufferManager_function:121'     flowFifo(sensorIn.sensorValidity.isOfValid, ... 
  // 'delayedHorizonBufferManager_function:122'              ofTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:123'              sensorIn.ofNeVel_mps, ... 
  // 'delayedHorizonBufferManager_function:124'              fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:125'              ekfParams.flowFifoParams); 
  stateEstimatorEskf_flowFifo(static_cast<boolean_T>(static_cast<boolean_T>(
    static_cast<boolean_T>(static_cast<boolean_T>(static_cast<boolean_T>(
    static_cast<boolean_T>((rtu_mtf01pData->distPrecision <=
    rtu_mtf01pParams->distPrecisionThr) & (rtu_mtf01pData->distStatus == 1)) &
    (rtu_mtf01pData->dist_m > rtu_mtf01pParams->distLimit_m[0])) &
    (rtu_mtf01pData->dist_m < rtu_mtf01pParams->distLimit_m[1])) &
    (rtu_mtf01pData->flowStatus == 1)) & rtu_mtf01pData->isMtf01pDataValid) &
    rtu_stateEstSmParams->useOpticalFlow), ofTimeIn_ms, rtb_VectorConcatenate_i,
    fusionTime_ms, stateEstimatorEskf_DW.resetStates,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.capacity,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.minInterval_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.maxAge_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.resetThreshold_ms,
    &dhSensorIn_sensorValidity_isOfV, &lidarTimeIn_ms, rtb_VectorConcatenate_k2,
    &a__4, &nextIdx);

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   Constant: '<Root>/localNedMag_nd'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay2'
  //   MATLAB Function: '<S13>/DelayedHorizonBufferManager'

  // 'delayedHorizonBufferManager_function:127' dhSensorIn.sensorValidity.isOfValid = ofReady; 
  // 'delayedHorizonBufferManager_function:128' dhSensorIn.sensorTimestamp.ofTimestamp_ms = ofTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:129' dhSensorIn.ofNeVel_mps = ofNeVelOut_mps; 
  // MATLAB Function 'EKF/EKF': '<S14>:1'
  // '<S14>:1:5' if isempty(covP) || resetStates
  if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<int32_T>
        (stateEstimatorEskf_DW.covP_not_empty) ^ 1) |
       stateEstimatorEskf_DW.resetStates)) {
    // '<S14>:1:6' covP = initCovP;
    std::memcpy(&stateEstimatorEskf_DW.covP[0], &rtu_initCovP[0], 361U * sizeof
                (real32_T));
    stateEstimatorEskf_DW.covP_not_empty = true;
  }

  // '<S14>:1:9' [states, covP, ekfDebug] = errorStateEkf_function2(sensorIn, prevStates, covP, prevDcmBodyToNed, estSmMode, ... 
  // '<S14>:1:10'     processNoiseQ, measNoiseR, gEarth_mps2, ekfParams, sampleTime_s); 
  // EKF runs an EKF to estimate required states
  //
  // Inputs:
  // SensorIn:                      Struct with sensor data
  // bodyAccels_mps2:           Body accels measured usig accelerometer
  // bodyRates_radps:           Body angular rates measured using Gyro
  // normMagVec_nd:             Mag data from onboard magnetometer as unit
  // localNedUnitMag_nd:        Local unit NED mag vector
  // isMagValid:                Boolean flag to indicate if mag data is valid
  // nedPosAndVel:              Measured Vehicle NED position and velocity
  // isGpsValid:                Boolean flag to indicate if GPS position is
  // valid
  // baroAlt_m:                 Baro altitude
  // isBaroValid:               Boolean flag to indicate if baro data is valid
  // lidarAgl_m:                Lidar AGL
  // isLidarValid:              Boolean flag to indicate if lidar data is valid
  // ofNeVel_mps:               Optical flow converted to NE velocity
  // isOfvalid:                 Boolean flag to indicate if optical flow data
  // is valid
  // prevStates:                    Previous state estimate
  // prevCovP:                      Previous covariance
  // prevDcmBodyToNed:              Body to NED DCM computed using prevStates
  // quaternion
  // estSmMode:                     Estimator state machine mode
  // processNoiseQ:                 Process Noise Matrix
  // measNoiseR:                    Meas Noise Matrix
  // gEarth_mps2:                   Acceleration due to gravity
  // ekfParams:                     Various parameters for EKF runs
  // sampleTime_s:                  Sample Time
  //
  // Ouputs:
  // states:                        Current states
  // cov:                           Current covariance
  // dcmBodyToNed:                  Body to NED DCM computed using current states 
  // quaternion
  // ekfDebug:                      EKF debug data
  // States
  // 1:4                            quaternions
  // 5:7                            NED Positions
  // 8:10                           NED Velocities
  // 11:13                          Gyro biases
  // 14:16                          Accel biases
  // 17:19                          Mag biases
  // 20                             Baro biases
  // Error States
  // 1:3                            angle error vector
  // 4:6                            NED position error
  // 7:9                            NED velocity error
  // 10:12                          Gyro bias error
  // 13:15                          Accel bias error
  // 16:18                          Mag bias error
  // 19                             Baro bias error
  //  persistent errorStateJac;
  // 'errorStateEkf_function2:65' if isempty(I3)
  // 'errorStateEkf_function2:75' ekfDebug.isBaroUsed = false;
  // Propagate state
  // 'errorStateEkf_function2:77' if (estSmMode == enumStateEstimateMode.INITIALIZE) 
  if (mode == enumStateEstimateMode::INITIALIZE) {
    // 'errorStateEkf_function2:78' states = prevStates;
    std::memcpy(&rtb_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                sizeof(real32_T));
  } else {
    // ErrorStateHat
    // 'errorStateEkf_function2:83' errorStateHat = zeros(19, 1, 'single');
    std::memset(&errorStateHat[0], 0, 19U * sizeof(real32_T));

    // 'errorStateEkf_function2:85' isImuValid = sensorIn.sensorValidity.isImuValid; 
    //  if(isImuValid)
    // Get all the sensor data
    // 'errorStateEkf_function2:89' bodyAccels_mps2 = sensorIn.bodyAccels_mps2;
    // 'errorStateEkf_function2:90' bodyRates_radps = sensorIn.bodyRates_radps;
    // 'errorStateEkf_function2:91' dtImuTime_s = sensorIn.dtImuTime_s;
    // 'errorStateEkf_function2:93' normMagVec_nd = sensorIn.normMagVec_nd;
    // 'errorStateEkf_function2:94' localNedUnitMag_nd = sensorIn.localNedUnitMag_nd; 
    // 'errorStateEkf_function2:95' isMagValid = sensorIn.sensorValidity.isMagValid; 
    // 'errorStateEkf_function2:96' nedPosAndVel =  [sensorIn.nedPosAndVel.pos_m; sensorIn.nedPosAndVel.vel_mps]; 
    nedPosAndVel[0] = rtb_ElementProduct[0];
    nedPosAndVel[3] = rtb_ElementProduct[3];
    nedPosAndVel[1] = rtb_ElementProduct[1];
    nedPosAndVel[4] = rtb_ElementProduct[4];
    nedPosAndVel[2] = rtb_ElementProduct[2];
    nedPosAndVel[5] = rtb_ElementProduct[5];

    // 'errorStateEkf_function2:97' isGpsValid = sensorIn.sensorValidity.isGpsValid; 
    // 'errorStateEkf_function2:98' baroAlt_m = sensorIn.baroAlt_m;
    // 'errorStateEkf_function2:99' isBaroValid = sensorIn.sensorValidity.isBaroValid; 
    // 'errorStateEkf_function2:100' lidarAgl_m = sensorIn.lidarAgl_m;
    // 'errorStateEkf_function2:101' isLidarValid = sensorIn.sensorValidity.isLidarValid; 
    // 'errorStateEkf_function2:102' ofNeVel_mps = sensorIn.ofNeVel_mps;
    // 'errorStateEkf_function2:103' isOfValid = sensorIn.sensorValidity.isOfValid; 
    // We don't update error states because initial value of error states are all 
    // zero
    // errorStates = errorStateJac*errorStates + inputPerturbJac*processNoiseCov 
    // Valid nominal state indices when GPS is avalable
    //      idxNs = 1:20;
    // 'errorStateEkf_function2:111' idxNs2 = 5:20;
    // Without quaternion
    // Valid error state indices when GPS is avalable
    // 'errorStateEkf_function2:113' idxEs = 1:19;
    // 'errorStateEkf_function2:114' idxEs2 = 4:19;
    // Without angle error
    // Valid nominal state indices when there is no GPS avalable
    //      idxNs = [1:4, 7, 10:20];
    // 'errorStateEkf_function2:118' idxNoGpsNs2 = [7, 10:20];
    // Without quaternion
    // Valid error state indices when there is no GPS avalable
    // 'errorStateEkf_function2:120' idxNoGpsEs = [1:3, 6, 9:19];
    // 'errorStateEkf_function2:121' idxNoGpsEs2 = [6, 9:19];
    // Without angle error
    //  %Apply Rodrigues formula to the angleVector to get the rotation vector
    //  dR = I3 + sin(dThetaNorm)*skew3(dThetaUnit) + dThetaUnit*dThetaUnit'*(1 - cos(dThetaNorm)); 
    //  %
    //  errorStateJac(1:3, 1:3) = dR';
    // 'errorStateEkf_function2:128' if estSmMode == enumStateEstimateMode.RUN
    if (mode == enumStateEstimateMode::RUN) {
      //      Compute the skew matrix for accel - accelbias
      //      accelSkew = skew3(bodyAccels_mps2 - prevStates(14:16));
      //      errorStateJac(4:6, 7:9) = I3*sampleTime_s;
      //      errorStateJac(7:9, 1:3) = -dcmBodyToNed * ...
      //          accelSkew*sampleTime_s;
      //      errorStateJac(7:9, 13:15) = -dcmBodyToNed*sampleTime_s;
      //  covP = errorStateJac*covP*errorStateJac' + processNoiseQ;
      // 'errorStateEkf_function2:136' gpsLossFlag = false;
      gpsLossFlag = false;
    } else {
      // 'errorStateEkf_function2:137' else
      //      errorStateJac(6, 9) = sampleTime_s;
      //      errorStateJac(7:9, 1:3) = single(0);
      //      errorStateJac(7:9, 13:15) = single(0);
      //      %Propogate covariances
      //      covP(idxEs, idxEs) = errorStateJac(idxEs, idxEs) * covP(idxEs, idxEs) * ... 
      //          errorStateJac(idxEs, idxEs)' + processNoiseQ(idxEs, idxEs);
      // 'errorStateEkf_function2:144' gpsLossFlag = true;
      gpsLossFlag = true;
    }

    // Propagate nominal states and Compute Jacobian and propagate covariance only if IMU data is valid 
    // 'errorStateEkf_function2:148' if(isImuValid)
    if (stateModeReady) {
      // 'errorStateEkf_function2:149' [states, dThetaNorm, dThetaUnit] = updateEskfStates(prevStates, bodyAccels_mps2, bodyRates_radps, ... 
      // 'errorStateEkf_function2:150'             dcmBodyToNed, estSmMode, isOfValid, dtImuTime_s, gEarth_mps2); 
      std::memcpy(&rtb_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                  sizeof(real32_T));

      // UPDATEESKFSTATES propogates the state
      //
      // Inputs:
      // states:                    Previous nominal state estimate
      // bodyAccels_mps2:           Body accels measured usig accelerometer
      // bodyRates_radps:           Body angular rates measured using Gyro
      // dcmBodyToNed:              Body to NED DCM computed using states
      // quaternion
      // baroData:                  Baro altitude
      // estSmMode:                 State Estimator State
      // isOfValid:                 OF validity flag
      // sampleTime_s:              Sample time for integration
      // gEarth_mps2:               Accel due to gravity
      // ekfParams:                 EKF Parameters
      //
      // Ouputs:
      // states:                    Current states
      // dThetaNorm:                Norm of delta angle by which nominal quat was 
      // propagated
      // dThetaUnit:                Unit vector along with the delt angle occured 
      // Only update NE position and velocity if we have valid GPS
      // 'updateEskfStates:27' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
           dhSensorIn_sensorValidity_isOfV)) {
        // 'updateEskfStates:28' stateDot = [ [states(8); states(9); states(10)]; ...                            % Derivative of [pN; pE; pD] 
        // 'updateEskfStates:29'     dcmBodyToNed * (bodyAccels_mps2 - [states(14);states(15);states(16)]) + ... 
        // 'updateEskfStates:30'     [0; 0; gEarth_mps2]];
        //                             % Derivative of [pN; pE; pD]
        //                                                        % Derivative of [vN; vE; vD] 
        // 'updateEskfStates:31' states(5:10) = states(5:10) + sampleTime_s * stateDot; 
        rtb_Product2_c = imuOut[0] - stateEstimatorEskf_DW.Delay_DSTATE[13];
        rtb_XAxis1 = imuOut[1] - stateEstimatorEskf_DW.Delay_DSTATE[14];
        rtb_XAxis2 = imuOut[2] - stateEstimatorEskf_DW.Delay_DSTATE[15];
        rtb_states[4] = imuOut[6] * stateEstimatorEskf_DW.Delay_DSTATE[7] +
          stateEstimatorEskf_DW.Delay_DSTATE[4];
        rtb_states[5] = imuOut[6] * stateEstimatorEskf_DW.Delay_DSTATE[8] +
          stateEstimatorEskf_DW.Delay_DSTATE[5];
        rtb_states[6] = imuOut[6] * stateEstimatorEskf_DW.Delay_DSTATE[9] +
          stateEstimatorEskf_DW.Delay_DSTATE[6];
        rtb_states[7] = ((stateEstimatorEskf_DW.Delay2_DSTATE[0] *
                          rtb_Product2_c + stateEstimatorEskf_DW.Delay2_DSTATE[3]
                          * rtb_XAxis1) + stateEstimatorEskf_DW.Delay2_DSTATE[6]
                         * rtb_XAxis2) * imuOut[6] +
          stateEstimatorEskf_DW.Delay_DSTATE[7];
        rtb_states[8] = ((stateEstimatorEskf_DW.Delay2_DSTATE[1] *
                          rtb_Product2_c + stateEstimatorEskf_DW.Delay2_DSTATE[4]
                          * rtb_XAxis1) + stateEstimatorEskf_DW.Delay2_DSTATE[7]
                         * rtb_XAxis2) * imuOut[6] +
          stateEstimatorEskf_DW.Delay_DSTATE[8];
        rtb_states[9] = (((stateEstimatorEskf_DW.Delay2_DSTATE[2] *
                           rtb_Product2_c + stateEstimatorEskf_DW.Delay2_DSTATE
                           [5] * rtb_XAxis1) +
                          stateEstimatorEskf_DW.Delay2_DSTATE[8] * rtb_XAxis2) +
                         *rtu_gEarth_mps2) * imuOut[6] +
          stateEstimatorEskf_DW.Delay_DSTATE[9];
      } else {
        // 'updateEskfStates:32' else
        // 'updateEskfStates:33' stateDot = states(10);
        //  %Derivative of down position
        // 'updateEskfStates:35' states(7) = states(7) + sampleTime_s * stateDot; 
        rtb_states[6] = imuOut[6] * stateEstimatorEskf_DW.Delay_DSTATE[9] +
          stateEstimatorEskf_DW.Delay_DSTATE[6];
      }

      // 'updateEskfStates:38' dTheta = (bodyRates_radps - states(11:13))*sampleTime_s; 
      rtb_CastToSingle[0] = (imuOut[3] - rtb_states[10]) * imuOut[6];
      rtb_CastToSingle[1] = (imuOut[4] - rtb_states[11]) * imuOut[6];
      rtb_CastToSingle[2] = (imuOut[5] - rtb_states[12]) * imuOut[6];

      // 'updateEskfStates:39' dThetaNorm = norm(dTheta);
      rtb_Product2_c = norm_94qjDDKI(rtb_CastToSingle);

      // 'updateEskfStates:41' if dThetaNorm > 1e-7
      if (rtb_Product2_c > 1.0E-7) {
        // Propagate the quaternion part of the state
        // 'updateEskfStates:43' dThetaUnit = dTheta/dThetaNorm;
        // 'updateEskfStates:44' states(1:4) = quatMultiply(states(1:4), [cos(dThetaNorm*0.5); sin(dThetaNorm*0.5)*dThetaUnit]); 
        rtb_XAxis2 = rtb_Product2_c * 0.5F;
        rtb_XAxis1 = std::sin(rtb_XAxis2);
        tmp_3[0] = std::cos(rtb_XAxis2);
        rtb_XAxis2 = rtb_CastToSingle[0] / rtb_Product2_c;
        rtb_CastToSingle[0] = rtb_XAxis2;
        tmp_3[1] = rtb_XAxis1 * rtb_XAxis2;
        rtb_XAxis2 = rtb_CastToSingle[1] / rtb_Product2_c;
        rtb_CastToSingle[1] = rtb_XAxis2;
        tmp_3[2] = rtb_XAxis1 * rtb_XAxis2;
        rtb_XAxis2 = rtb_CastToSingle[2] / rtb_Product2_c;
        rtb_CastToSingle[2] = rtb_XAxis2;
        tmp_3[3] = rtb_XAxis1 * rtb_XAxis2;
        quatMultiply_UkcBdhzN(&rtb_states[0], tmp_3, qDelayed);
        rtb_states[0] = qDelayed[0];
        rtb_states[1] = qDelayed[1];
        rtb_states[2] = qDelayed[2];
        rtb_states[3] = qDelayed[3];

        // Normalize the quaternion
        // 'updateEskfStates:46' nQuat = norm(states(1:4));
        rtb_XAxis1 = norm_NoMIKEmk(&rtb_states[0]);

        // 'updateEskfStates:47' if nQuat > 1e-7
        if (rtb_XAxis1 > 1.0E-7) {
          // 'updateEskfStates:48' states(1:4) = states(1:4)/nQuat;
          rtb_states[0] = qDelayed[0] / rtb_XAxis1;
          rtb_states[1] = qDelayed[1] / rtb_XAxis1;
          rtb_states[2] = qDelayed[2] / rtb_XAxis1;
          rtb_states[3] = qDelayed[3] / rtb_XAxis1;
        }
      } else {
        // 'updateEskfStates:50' else
        // 'updateEskfStates:51' dThetaUnit = single([0; 0; 0]);
        rtb_CastToSingle[0] = 0.0F;
        rtb_CastToSingle[1] = 0.0F;
        rtb_CastToSingle[2] = 0.0F;
      }

      // 'errorStateEkf_function2:151' errorStateJac = computeEskfStateJac(prevStates, dcmBodyToNed, bodyAccels_mps2, ... 
      // 'errorStateEkf_function2:152'             dThetaNorm, dThetaUnit, gpsLossFlag, isOfValid, dtImuTime_s); 
      // 'errorStateEkf_function2:153' covP = updateEskfCovP(covP, errorStateJac, processNoiseQ, dtImuTime_s); 
      computeEskfStateJac_tpF0ZWTA(stateEstimatorEskf_DW.Delay_DSTATE,
        stateEstimatorEskf_DW.Delay2_DSTATE, &imuOut[0], rtb_Product2_c,
        rtb_CastToSingle, gpsLossFlag, dhSensorIn_sensorValidity_isOfV, imuOut[6],
        tmp_d);
      updateEskfCovP_Qn4XLGNE(stateEstimatorEskf_DW.covP, tmp_d,
        rtu_processNoiseQ, imuOut[6]);
    } else {
      // 'errorStateEkf_function2:154' else
      // 'errorStateEkf_function2:155' states = prevStates;
      std::memcpy(&rtb_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                  sizeof(real32_T));
    }

    // Fuse Accel in Correction step if GPS is not available
    // 'errorStateEkf_function2:159' if estSmMode ~= enumStateEstimateMode.RUN && ~isOfValid && ... 
    // 'errorStateEkf_function2:160'             isImuValid
    if (static_cast<boolean_T>(static_cast<boolean_T>((mode !=
           enumStateEstimateMode::RUN) & static_cast<boolean_T>
          (static_cast<int32_T>(dhSensorIn_sensorValidity_isOfV) ^ 1)) &
         stateModeReady)) {
      // 'errorStateEkf_function2:161' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:401' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:402'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:403'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:404'     -states(3), states(2), states(1)];
      rtb_Product2_c = 0.5F * -rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_Product2_c;
      rtb_XAxis1 = 0.5F * -rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[20] = rtb_XAxis1;
      rtb_XAxis2 = 0.5F * -rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_XAxis2;
      rtb_UnitDelay_g = 0.5F * rtb_states[0];
      stateEstimatorEskf_DW.xErrorJac[1] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_Product2_c;
      stateEstimatorEskf_DW.xErrorJac[3] = rtb_XAxis1;
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = rtb_UnitDelay_g;

      // 'errorStateEkf_function2:163' measJac = computeEskfAccelMeasJac(states, gEarth_mps2); 
      // COMPUTEESKFACCELMEASJAC Computes Meas Jacobian for accelerometer
      // measurements
      //
      // Inputs:
      // states:                EKF states
      // gEarth_mps2:           Local accel due to gravity
      //
      // Outputs:
      // accelaccelMeasJac:            3x16 Accel Meas Jacobian
      // Initialize the Meas jacobian to zero
      // 'computeEskfAccelMeasJac:13' accelMeasJac = zeros(3, 20, 'single');
      std::memset(&measJac[0], 0, 60U * sizeof(real32_T));

      // Extract quat states
      // 'computeEskfAccelMeasJac:16' states(1) = states(1);
      // 'computeEskfAccelMeasJac:17' states(2) = states(2);
      // 'computeEskfAccelMeasJac:18' states(3) = states(3);
      // 'computeEskfAccelMeasJac:19' states(4) = states(4);
      // 'computeEskfAccelMeasJac:21' tmp1 = gEarth_mps2*2*states(3);
      rtb_XAxis2 = *rtu_gEarth_mps2 * 2.0F;
      rtb_Product2_c = rtb_XAxis2 * rtb_states[2];

      // 'computeEskfAccelMeasJac:22' tmp2 = -2*gEarth_mps2*states(4);
      rtb_XAxis1 = -2.0F * *rtu_gEarth_mps2 * rtb_states[3];

      // 'computeEskfAccelMeasJac:23' tmp3 = gEarth_mps2*2*states(1);
      rtb_XAxis2 *= rtb_states[0];

      // 'computeEskfAccelMeasJac:24' tmp4 = -gEarth_mps2*2*states(2);
      rtb_UnitDelay_g = -*rtu_gEarth_mps2 * 2.0F * rtb_states[1];

      // 'computeEskfAccelMeasJac:25' tmp5 = 4*gEarth_mps2;
      rtb_Sum2_idx_1 = 4.0F * *rtu_gEarth_mps2;

      // 'computeEskfAccelMeasJac:27' accelMeasJac(1, 1) = tmp1;
      measJac[0] = rtb_Product2_c;

      // 'computeEskfAccelMeasJac:28' accelMeasJac(1, 2) = tmp2;
      measJac[3] = rtb_XAxis1;

      // 'computeEskfAccelMeasJac:29' accelMeasJac(1, 3) = tmp3;
      measJac[6] = rtb_XAxis2;

      // 'computeEskfAccelMeasJac:30' accelMeasJac(1, 4) = tmp4;
      measJac[9] = rtb_UnitDelay_g;

      // 'computeEskfAccelMeasJac:31' accelMeasJac(1, 14) = 1;
      measJac[39] = 1.0F;

      // 'computeEskfAccelMeasJac:33' accelMeasJac(2, 1) = tmp4;
      measJac[1] = rtb_UnitDelay_g;

      // 'computeEskfAccelMeasJac:34' accelMeasJac(2, 2) = -tmp3;
      measJac[4] = -rtb_XAxis2;

      // 'computeEskfAccelMeasJac:35' accelMeasJac(2, 3) = tmp2;
      measJac[7] = rtb_XAxis1;

      // 'computeEskfAccelMeasJac:36' accelMeasJac(2, 4) = -tmp1;
      measJac[10] = -rtb_Product2_c;

      // 'computeEskfAccelMeasJac:37' accelMeasJac(2, 15) = 1;
      measJac[43] = 1.0F;

      // 'computeEskfAccelMeasJac:39' accelMeasJac(3, 2) = states(2)*tmp5;
      measJac[5] = rtb_states[1] * rtb_Sum2_idx_1;

      // 'computeEskfAccelMeasJac:40' accelMeasJac(3, 3) = states(3)*tmp5;
      measJac[8] = rtb_states[2] * rtb_Sum2_idx_1;

      // 'computeEskfAccelMeasJac:41' accelMeasJac(3, 16) = 1;
      measJac[47] = 1.0F;

      // 'errorStateEkf_function2:164' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:165' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      // 'errorStateEkf_function2:166' H(1:3, 13:15) = I3;
      rtb_VectorConcatenate1_tmp = 0;
      i_2 = 0;
      for (i = 0; i < 3; i++) {
        rtb_Product2_c = 0.0F;
        rtb_XAxis2 = 0.0F;
        rtb_UnitDelay_g = 0.0F;
        i_1 = 0;
        for (covP_tmp = 0; covP_tmp < 4; covP_tmp++) {
          rtb_XAxis1 = stateEstimatorEskf_DW.xErrorJac[covP_tmp +
            rtb_VectorConcatenate1_tmp];
          rtb_Product2_c += measJac[i_1] * rtb_XAxis1;
          rtb_XAxis2 += measJac[i_1 + 1] * rtb_XAxis1;
          rtb_UnitDelay_g += measJac[i_1 + 2] * rtb_XAxis1;
          i_1 += 3;
        }

        H[i_2 + 2] = rtb_UnitDelay_g;
        H[i_2 + 1] = rtb_XAxis2;
        H[i_2] = rtb_Product2_c;
        H[i_2 + 36] = stateEstimatorEskf_DW.I3[i_2];
        H[i_2 + 37] = stateEstimatorEskf_DW.I3[i_2 + 1];
        H[i_2 + 38] = stateEstimatorEskf_DW.I3[i_2 + 2];
        rtb_VectorConcatenate1_tmp += 20;
        i_2 += 3;
      }

      //      H = measJac(:, idxNs) * xErrorJac(idxNs, idxEs);
      // 'errorStateEkf_function2:170' C_ned2b  = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:171'             states(4));
      // NED gravity in body frame
      // 'errorStateEkf_function2:173' estGravityInBodyFrame = C_ned2b*[0; 0; -gEarth_mps2] + states(14:16); 
      // 'errorStateEkf_function2:175' tmp1 = covP(:, 13:15) + covP(:, 1:3) * H(:, 1:3).'; 
      rtb_Product2_c = H[3];
      rtb_XAxis2 = H[0];
      rtb_UnitDelay_g = H[6];
      rtb_XAxis1 = H[4];
      rtb_Sum2_idx_1 = H[1];
      rtb_Sum2_idx_2 = H[7];
      q2q3 = H[5];
      rtb_dcmBodyToNed_idx_7 = H[2];
      rtb_dcmBodyToNed_idx_2 = H[8];
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        rtb_dcmBodyToNed_idx_5 =
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 19];
        rtb_dcmBodyToNed_idx_8 =
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp];
        rtb_Merge_idx_0 = stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp
          + 38];
        tmp1[rtb_VectorConcatenate1_tmp] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_Product2_c + rtb_dcmBodyToNed_idx_8 * rtb_XAxis2) +
          rtb_Merge_idx_0 * rtb_UnitDelay_g) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 228];
        tmp1[rtb_VectorConcatenate1_tmp + 19] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_XAxis1 + rtb_dcmBodyToNed_idx_8 * rtb_Sum2_idx_1) +
          rtb_Merge_idx_0 * rtb_Sum2_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 247];
        tmp1[rtb_VectorConcatenate1_tmp + 38] = ((rtb_dcmBodyToNed_idx_5 * q2q3
          + rtb_dcmBodyToNed_idx_8 * rtb_dcmBodyToNed_idx_7) + rtb_Merge_idx_0 *
          rtb_dcmBodyToNed_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 266];
      }

      // covP(idxEs, idxEs)*H';
      // 'errorStateEkf_function2:176' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(12:14, 12:14)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
           rtb_VectorConcatenate1_tmp++) {
        for (i_2 = 0; i_2 < 15; i_2++) {
          tmp1_0[i_2 + 15 * rtb_VectorConcatenate1_tmp] = tmp1[19 *
            rtb_VectorConcatenate1_tmp + c_0[i_2]];
        }

        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis1 = 0.0F;
          for (i = 0; i < 15; i++) {
            c = c_0[i];
            rtb_XAxis1 += H[3 * c + rtb_VectorConcatenate1_tmp] * tmp1[19 * i_2
              + c];
          }

          rtb_VectorConcatenate_m[rtb_VectorConcatenate1_tmp + 3 * i_2] =
            rtu_measNoiseR[((i_2 + 11) * 15 + rtb_VectorConcatenate1_tmp) + 11]
            + rtb_XAxis1;
        }
      }

      mrdiv_9ppBIzmt(tmp1_0, rtb_VectorConcatenate_m, K);

      // 'errorStateEkf_function2:178' errorStateHat(idxNoGpsEs) = K*(bodyAccels_mps2 - estGravityInBodyFrame); 
      quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                         rtb_states[3], tmp_4);
      imuOut_0[0] = imuOut[0] - (static_cast<real32_T>(tmp_4[6]) *
        -*rtu_gEarth_mps2 + rtb_states[13]);
      imuOut_0[1] = imuOut[1] - (static_cast<real32_T>(tmp_4[7]) *
        -*rtu_gEarth_mps2 + rtb_states[14]);
      imuOut_0[2] = imuOut[2] - (static_cast<real32_T>(tmp_4[8]) *
        -*rtu_gEarth_mps2 + rtb_states[15]);
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        K_0[rtb_VectorConcatenate1_tmp] = 0.0F;
      }

      rtb_VectorConcatenate1_tmp = 0;
      for (i_2 = 0; i_2 < 3; i_2++) {
        rtb_XAxis1 = imuOut_0[i_2];
        for (i = 0; i < 15; i++) {
          K_0[i] += K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis1;
        }

        rtb_VectorConcatenate1_tmp += 15;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] =
          K_0[rtb_VectorConcatenate1_tmp];
      }

      // 'errorStateEkf_function2:180' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(13:15, idxNoGpsEs) + ... 
      // 'errorStateEkf_function2:181'             H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
           rtb_VectorConcatenate1_tmp++) {
        rtb_Product2_c = H[rtb_VectorConcatenate1_tmp + 3];
        rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp];
        rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp + 6];
        for (i_2 = 0; i_2 < 15; i_2++) {
          i = 19 * c_0[i_2];
          tmp1_0[rtb_VectorConcatenate1_tmp + 3 * i_2] =
            ((stateEstimatorEskf_DW.covP[i + 1] * rtb_Product2_c +
              stateEstimatorEskf_DW.covP[i] * rtb_XAxis2) +
             stateEstimatorEskf_DW.covP[i + 2] * rtb_UnitDelay_g) +
            stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 12];
        }
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        rtb_Product2_c = K[rtb_VectorConcatenate1_tmp + 15];
        rtb_XAxis1 = K[rtb_VectorConcatenate1_tmp];
        rtb_XAxis2 = K[rtb_VectorConcatenate1_tmp + 30];
        for (i_2 = 0; i_2 < 15; i_2++) {
          tmp_5[rtb_VectorConcatenate1_tmp + 15 * i_2] =
            stateEstimatorEskf_DW.covP[19 * c_0[i_2] +
            c_0[rtb_VectorConcatenate1_tmp]] - ((tmp1_0[3 * i_2 + 1] *
            rtb_Product2_c + tmp1_0[3 * i_2] * rtb_XAxis1) + tmp1_0[3 * i_2 + 2]
            * rtb_XAxis2);
        }
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        for (i_2 = 0; i_2 < 15; i_2++) {
          stateEstimatorEskf_DW.covP[c_0[i_2] + 19 *
            c_0[rtb_VectorConcatenate1_tmp]] = tmp_5[15 *
            rtb_VectorConcatenate1_tmp + i_2];
        }
      }

      // Update the nominal state
      // 'errorStateEkf_function2:184' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states_0[rtb_VectorConcatenate1_tmp] =
          rtb_states[e[rtb_VectorConcatenate1_tmp]] +
          errorStateHat[f[rtb_VectorConcatenate1_tmp]];
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states[e[rtb_VectorConcatenate1_tmp]] =
          rtb_states_0[rtb_VectorConcatenate1_tmp];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:187' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:188' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:190' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + covP(idxNoGpsEs, idxNoGpsEs)')/2; 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        for (i_2 = 0; i_2 < 15; i_2++) {
          covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
          stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
            c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) / 2.0F;
        }
      }
    }

    // Fuse Mag data if it is valid
    // 'errorStateEkf_function2:194' if(isMagValid)
    if (dhSensorIn_sensorValidity_isMag) {
      // 'errorStateEkf_function2:195' measJac = computEskfMagMeasJac(states, localNedUnitMag_nd); 
      // 'errorStateEkf_function2:197' C_ned2b = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:198'             states(4));
      // Rotate propogated mag states and add bias to estimate measurements
      // 'errorStateEkf_function2:201' estBodyMagUnitVec = C_ned2b*localNedUnitMag_nd + states(17:19); 
      // 'errorStateEkf_function2:203' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:401' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:402'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:403'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:404'     -states(3), states(2), states(1)];
      rtb_Product2_c = 0.5F * -rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_Product2_c;
      rtb_XAxis1 = 0.5F * -rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[20] = rtb_XAxis1;
      rtb_XAxis2 = 0.5F * -rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_XAxis2;
      rtb_UnitDelay_g = 0.5F * rtb_states[0];
      stateEstimatorEskf_DW.xErrorJac[1] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_Product2_c;
      stateEstimatorEskf_DW.xErrorJac[3] = rtb_XAxis1;
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = rtb_UnitDelay_g;

      // 'errorStateEkf_function2:205' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:206' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      computEskfMagMeasJac_ecmRY7bq(rtb_states, rtCP_localNedMag_nd_Value,
        measJac);

      // 'errorStateEkf_function2:207' H(1:3, 16:18) = I3;
      rtb_VectorConcatenate1_tmp = 0;
      i_2 = 0;
      for (i = 0; i < 3; i++) {
        rtb_Product2_c = 0.0F;
        rtb_XAxis2 = 0.0F;
        rtb_UnitDelay_g = 0.0F;
        i_1 = 0;
        for (covP_tmp = 0; covP_tmp < 4; covP_tmp++) {
          rtb_XAxis1 = stateEstimatorEskf_DW.xErrorJac[covP_tmp +
            rtb_VectorConcatenate1_tmp];
          rtb_Product2_c += measJac[i_1] * rtb_XAxis1;
          rtb_XAxis2 += measJac[i_1 + 1] * rtb_XAxis1;
          rtb_UnitDelay_g += measJac[i_1 + 2] * rtb_XAxis1;
          i_1 += 3;
        }

        H[i_2 + 2] = rtb_UnitDelay_g;
        H[i_2 + 1] = rtb_XAxis2;
        H[i_2] = rtb_Product2_c;
        H[i_2 + 45] = stateEstimatorEskf_DW.I3[i_2];
        H[i_2 + 46] = stateEstimatorEskf_DW.I3[i_2 + 1];
        H[i_2 + 47] = stateEstimatorEskf_DW.I3[i_2 + 2];
        rtb_VectorConcatenate1_tmp += 20;
        i_2 += 3;
      }

      // 'errorStateEkf_function2:208' tmp1 = covP(:, 16:18) + covP(:, 1:3) * H(:,1:3).'; 
      rtb_Product2_c = H[3];
      rtb_XAxis2 = H[0];
      rtb_UnitDelay_g = H[6];
      rtb_XAxis1 = H[4];
      rtb_Sum2_idx_1 = H[1];
      rtb_Sum2_idx_2 = H[7];
      q2q3 = H[5];
      rtb_dcmBodyToNed_idx_7 = H[2];
      rtb_dcmBodyToNed_idx_2 = H[8];
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        rtb_dcmBodyToNed_idx_5 =
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 19];
        rtb_dcmBodyToNed_idx_8 =
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp];
        rtb_Merge_idx_0 = stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp
          + 38];
        tmp1[rtb_VectorConcatenate1_tmp] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_Product2_c + rtb_dcmBodyToNed_idx_8 * rtb_XAxis2) +
          rtb_Merge_idx_0 * rtb_UnitDelay_g) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 285];
        tmp1[rtb_VectorConcatenate1_tmp + 19] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_XAxis1 + rtb_dcmBodyToNed_idx_8 * rtb_Sum2_idx_1) +
          rtb_Merge_idx_0 * rtb_Sum2_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 304];
        tmp1[rtb_VectorConcatenate1_tmp + 38] = ((rtb_dcmBodyToNed_idx_5 * q2q3
          + rtb_dcmBodyToNed_idx_8 * rtb_dcmBodyToNed_idx_7) + rtb_Merge_idx_0 *
          rtb_dcmBodyToNed_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 323];
      }

      // covP(idxEs, idxEs)*H(:, idxEs)';
      // 'errorStateEkf_function2:210' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      stateModeReady = (mode == enumStateEstimateMode::RUN) |
        dhSensorIn_sensorValidity_isOfV;
      if (stateModeReady) {
        // 'errorStateEkf_function2:211' K = tmp1(idxEs, :)/(H(:, idxEs) * tmp1(idxEs, :) + measNoiseR(1:3, 1:3)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          i_2 = 0;
          i = 0;
          i_1 = 0;
          for (covP_tmp = 0; covP_tmp < 3; covP_tmp++) {
            rtb_XAxis1 = 0.0F;
            tmp_c = 0;
            for (i_0 = 0; i_0 < 19; i_0++) {
              rtb_XAxis1 += H[tmp_c + rtb_VectorConcatenate1_tmp] * tmp1[i_0 +
                i_1];
              tmp_c += 3;
            }

            rtb_VectorConcatenate_m[i_2 + rtb_VectorConcatenate1_tmp] =
              rtu_measNoiseR[i + rtb_VectorConcatenate1_tmp] + rtb_XAxis1;
            i_2 += 3;
            i += 15;
            i_1 += 19;
          }
        }

        mrdiv_7fpDxZtR(tmp1, rtb_VectorConcatenate_m, b_K);

        // 'errorStateEkf_function2:212' errorStateHat(idxEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                           rtb_states[3], tmp_4);
        imuOut_0[0] = Divide[0] - (((static_cast<real32_T>(tmp_4[0]) * 0.4752F +
          static_cast<real32_T>(tmp_4[3]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[6]) * 0.873F) + rtb_states[16]);
        imuOut_0[1] = Divide[1] - (((static_cast<real32_T>(tmp_4[1]) * 0.4752F +
          static_cast<real32_T>(tmp_4[4]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[7]) * 0.873F) + rtb_states[17]);
        imuOut_0[2] = Divide[2] - (((static_cast<real32_T>(tmp_4[2]) * 0.4752F +
          static_cast<real32_T>(tmp_4[5]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[8]) * 0.873F) + rtb_states[18]);
        std::memset(&d_K[0], 0, 19U * sizeof(real32_T));
        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis1 = imuOut_0[i_2];
          for (i = 0; i < 19; i++) {
            d_K[i] += b_K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis1;
          }

          rtb_VectorConcatenate1_tmp += 19;
        }

        std::memcpy(&errorStateHat[0], &d_K[0], 19U * sizeof(real32_T));

        // 'errorStateEkf_function2:214' covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*(covP(16:18, idxEs) + ... 
        // 'errorStateEkf_function2:215'                 H(:, 1:3) * covP(1:3, idxEs)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          rtb_Product2_c = H[rtb_VectorConcatenate1_tmp + 3];
          rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp];
          rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp + 6];
          i_2 = 0;
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            tmp1[i_2 + rtb_VectorConcatenate1_tmp] =
              ((stateEstimatorEskf_DW.covP[i + 1] * rtb_Product2_c +
                stateEstimatorEskf_DW.covP[i] * rtb_XAxis2) +
               stateEstimatorEskf_DW.covP[i + 2] * rtb_UnitDelay_g) +
              stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 15];
            i_2 += 3;
            i += 19;
          }
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
             rtb_VectorConcatenate1_tmp++) {
          rtb_Product2_c = b_K[rtb_VectorConcatenate1_tmp + 19];
          rtb_XAxis1 = b_K[rtb_VectorConcatenate1_tmp];
          rtb_XAxis2 = b_K[rtb_VectorConcatenate1_tmp + 38];
          i_2 = 0;
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            covP_tmp = i_2 + rtb_VectorConcatenate1_tmp;
            stateEstimatorEskf_DW.covP[covP_tmp] -= (tmp1[i + 1] *
              rtb_Product2_c + tmp1[i] * rtb_XAxis1) + tmp1[i + 2] * rtb_XAxis2;
            i_2 += 19;
            i += 3;
          }
        }

        // Update the nominal state
        // 'errorStateEkf_function2:218' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
             rtb_VectorConcatenate1_tmp++) {
          rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
            errorStateHat[rtb_VectorConcatenate1_tmp + 3];
        }
      } else {
        // 'errorStateEkf_function2:219' else
        // 'errorStateEkf_function2:220' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(1:3, 1:3)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          for (i_2 = 0; i_2 < 15; i_2++) {
            tmp1_0[i_2 + 15 * rtb_VectorConcatenate1_tmp] = tmp1[19 *
              rtb_VectorConcatenate1_tmp + c_0[i_2]];
          }

          for (i_2 = 0; i_2 < 3; i_2++) {
            rtb_XAxis1 = 0.0F;
            for (i = 0; i < 15; i++) {
              c = c_0[i];
              rtb_XAxis1 += H[3 * c + rtb_VectorConcatenate1_tmp] * tmp1[19 *
                i_2 + c];
            }

            rtb_VectorConcatenate_m[rtb_VectorConcatenate1_tmp + 3 * i_2] =
              rtu_measNoiseR[15 * i_2 + rtb_VectorConcatenate1_tmp] + rtb_XAxis1;
          }
        }

        mrdiv_9ppBIzmt(tmp1_0, rtb_VectorConcatenate_m, K);

        // 'errorStateEkf_function2:221' errorStateHat(idxNoGpsEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                           rtb_states[3], tmp_4);
        imuOut_0[0] = Divide[0] - (((static_cast<real32_T>(tmp_4[0]) * 0.4752F +
          static_cast<real32_T>(tmp_4[3]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[6]) * 0.873F) + rtb_states[16]);
        imuOut_0[1] = Divide[1] - (((static_cast<real32_T>(tmp_4[1]) * 0.4752F +
          static_cast<real32_T>(tmp_4[4]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[7]) * 0.873F) + rtb_states[17]);
        imuOut_0[2] = Divide[2] - (((static_cast<real32_T>(tmp_4[2]) * 0.4752F +
          static_cast<real32_T>(tmp_4[5]) * 0.1096F) + static_cast<real32_T>
          (tmp_4[8]) * 0.873F) + rtb_states[18]);
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          K_0[rtb_VectorConcatenate1_tmp] = 0.0F;
        }

        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis1 = imuOut_0[i_2];
          for (i = 0; i < 15; i++) {
            K_0[i] += K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis1;
          }

          rtb_VectorConcatenate1_tmp += 15;
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] =
            K_0[rtb_VectorConcatenate1_tmp];
        }

        // 'errorStateEkf_function2:223' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(16:18, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:224'                 H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          rtb_Product2_c = H[rtb_VectorConcatenate1_tmp + 3];
          rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp];
          rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp + 6];
          for (i_2 = 0; i_2 < 15; i_2++) {
            i = 19 * c_0[i_2];
            tmp1_0[rtb_VectorConcatenate1_tmp + 3 * i_2] =
              ((stateEstimatorEskf_DW.covP[i + 1] * rtb_Product2_c +
                stateEstimatorEskf_DW.covP[i] * rtb_XAxis2) +
               stateEstimatorEskf_DW.covP[i + 2] * rtb_UnitDelay_g) +
              stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 15];
          }
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          rtb_Product2_c = K[rtb_VectorConcatenate1_tmp + 15];
          rtb_XAxis1 = K[rtb_VectorConcatenate1_tmp];
          rtb_XAxis2 = K[rtb_VectorConcatenate1_tmp + 30];
          for (i_2 = 0; i_2 < 15; i_2++) {
            tmp_5[rtb_VectorConcatenate1_tmp + 15 * i_2] =
              stateEstimatorEskf_DW.covP[19 * c_0[i_2] +
              c_0[rtb_VectorConcatenate1_tmp]] - ((tmp1_0[3 * i_2 + 1] *
              rtb_Product2_c + tmp1_0[3 * i_2] * rtb_XAxis1) + tmp1_0[3 * i_2 +
              2] * rtb_XAxis2);
          }
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          for (i_2 = 0; i_2 < 15; i_2++) {
            stateEstimatorEskf_DW.covP[c_0[i_2] + 19 *
              c_0[rtb_VectorConcatenate1_tmp]] = tmp_5[15 *
              rtb_VectorConcatenate1_tmp + i_2];
          }
        }

        // Update the nominal state
        // 'errorStateEkf_function2:227' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
             rtb_VectorConcatenate1_tmp++) {
          rtb_states_0[rtb_VectorConcatenate1_tmp] =
            rtb_states[e[rtb_VectorConcatenate1_tmp]] +
            errorStateHat[f[rtb_VectorConcatenate1_tmp]];
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
             rtb_VectorConcatenate1_tmp++) {
          rtb_states[e[rtb_VectorConcatenate1_tmp]] =
            rtb_states_0[rtb_VectorConcatenate1_tmp];
        }
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:231' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:232' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:233' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      if (stateModeReady) {
        // 'errorStateEkf_function2:234' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 19; i_2++) {
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            covP_tmp = i_1 + rtb_VectorConcatenate1_tmp;
            stateEstimatorEskf_DW.covP[covP_tmp] = (covP[i + i_2] +
              covP[covP_tmp]) * 0.5F;
            i += 19;
          }

          rtb_VectorConcatenate1_tmp += 19;
        }
      } else {
        // 'errorStateEkf_function2:235' else
        // 'errorStateEkf_function2:236' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:237'                 covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          for (i_2 = 0; i_2 < 15; i_2++) {
            covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
            stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
              c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
          }
        }

        // 'errorStateEkf_function2:238' states(8:9) = 0;
        rtb_states[7] = 0.0F;
        rtb_states[8] = 0.0F;
      }
    }

    // Integrate optical flow data
    // 'errorStateEkf_function2:243' if(isOfValid)
    if (dhSensorIn_sensorValidity_isOfV) {
      //          measJac = computeEskfOfMeasJac(states);
      //          C_ned2b = quatToDcm(states(1), states(2), states(3), ...
      //              states(4));
      //
      //          %Rotate propogated NED velocity states
      //          estBodyVel_mps = C_ned2b*states(8:10);
      //
      //          xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac);
      //
      //          H = zeros(2, 6, 'single');
      //          H(:, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3);
      //          H(:, 4:6) = measJac(:, 5:7);
      //          tmp1 = covP(idxEs, [1:3, 7:9]) * H';
      //
      //          K = tmp1(idxEs, :)/(H * tmp1([1:3, 7:9], :) + ...
      //              [measNoiseR(15, 15), 0; 0, measNoiseR(15, 15)]);
      //          errorStateHat(idxEs) = K*(ofNeVel_mps - estBodyVel_mps(1:2));
      //
      //          covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP([1:3, 7:9], :); 
      //
      //          %Update the nominal state
      //          states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2);
      //
      //          %Construct quaternion from the rotation vector and reset covP
      //          [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      //          states(1:4) = nomQuat;
      //          covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      // 'errorStateEkf_function2:272' [states, covP] = applyOfVelCorr(states, ofNeVel_mps, covP, 7, idxEs, ... 
      // 'errorStateEkf_function2:273'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      // 'errorStateEkf_function2:491' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      rtb_Product2_c = 1.0F / (stateEstimatorEskf_DW.covP[120] + rtu_measNoiseR
        [224]);

      // 'errorStateEkf_function2:492' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_VectorConcatenate_k2[0] - rtb_states[7];

      // 'errorStateEkf_function2:493' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:496' errorStateHat = zeros(19, 1, 'single');
      //  if NIS < innovGate
      // 'errorStateEkf_function2:499' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:500' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_XAxis2 = stateEstimatorEskf_DW.covP[i + 114] * rtb_Product2_c;
        d_K[i] = rtb_XAxis2;
        b_errorStateHat[i] = rtb_XAxis2 * rtb_XAxis1;
      }

      // 'errorStateEkf_function2:502' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
      rtb_VectorConcatenate1_tmp = 0;
      for (i_2 = 0; i_2 < 19; i_2++) {
        for (i = 0; i < 19; i++) {
          covP_tmp = i + rtb_VectorConcatenate1_tmp;
          covP[covP_tmp] = stateEstimatorEskf_DW.covP[covP_tmp] -
            stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 6] * d_K[i];
        }

        rtb_VectorConcatenate1_tmp += 19;
      }

      // Update the nominal state
      // 'errorStateEkf_function2:505' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
          b_errorStateHat[rtb_VectorConcatenate1_tmp + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:508' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      updateQuatAndResetCovP_o483nOwE(qDelayed, &b_errorStateHat[0], covP);

      // 'errorStateEkf_function2:509' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:511' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      //  end
      // 'errorStateEkf_function2:274' [states, covP] = applyOfVelCorr(states, ofNeVel_mps, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:275'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      // 'errorStateEkf_function2:491' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      // 'errorStateEkf_function2:492' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_VectorConcatenate_k2[1] - rtb_states[8];

      // 'errorStateEkf_function2:493' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:496' errorStateHat = zeros(19, 1, 'single');
      i = 0;
      for (i_1 = 0; i_1 < 19; i_1++) {
        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 19; i_2++) {
          covP_tmp = i_2 + i;
          b_covP[covP_tmp] = (covP[rtb_VectorConcatenate1_tmp + i_1] +
                              covP[covP_tmp]) * 0.5F;
          rtb_VectorConcatenate1_tmp += 19;
        }

        i += 19;
      }

      rtb_Product2_c = 1.0F / (b_covP[140] + rtu_measNoiseR[224]);

      //  if NIS < innovGate
      // 'errorStateEkf_function2:499' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:500' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_XAxis2 = b_covP[i + 133] * rtb_Product2_c;
        d_K[i] = rtb_XAxis2;
        b_errorStateHat[i] = rtb_XAxis2 * rtb_XAxis1;
      }

      // 'errorStateEkf_function2:502' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
      rtb_VectorConcatenate1_tmp = 0;
      for (i_2 = 0; i_2 < 19; i_2++) {
        for (i = 0; i < 19; i++) {
          covP_tmp = i + rtb_VectorConcatenate1_tmp;
          covP[covP_tmp] = b_covP[covP_tmp] - b_covP[rtb_VectorConcatenate1_tmp
            + 7] * d_K[i];
        }

        rtb_VectorConcatenate1_tmp += 19;
      }

      std::memcpy(&b_covP[0], &covP[0], 361U * sizeof(real32_T));

      // Update the nominal state
      // 'errorStateEkf_function2:505' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
          b_errorStateHat[rtb_VectorConcatenate1_tmp + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:508' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      updateQuatAndResetCovP_o483nOwE(qDelayed, &b_errorStateHat[0], b_covP);

      // 'errorStateEkf_function2:509' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:511' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      rtb_VectorConcatenate1_tmp = 0;
      for (i_2 = 0; i_2 < 19; i_2++) {
        i = 0;
        for (i_1 = 0; i_1 < 19; i_1++) {
          covP_tmp = i_1 + rtb_VectorConcatenate1_tmp;
          stateEstimatorEskf_DW.covP[covP_tmp] = (b_covP[i + i_2] +
            b_covP[covP_tmp]) * 0.5F;
          i += 19;
        }

        rtb_VectorConcatenate1_tmp += 19;
      }

      //  end
    }

    // Fuse Baro data if it is valid
    // 'errorStateEkf_function2:279' if(isBaroValid)
    if (dhSensorIn_sensorValidity_isBar) {
      // 'errorStateEkf_function2:280' iS = 1/(covP(19, 19) - covP(19, 6) + covP(6, 6) - covP(6, 19) + measNoiseR(10, 10)); 
      rtb_Product2_c = 1.0F / ((((stateEstimatorEskf_DW.covP[360] -
        stateEstimatorEskf_DW.covP[113]) + stateEstimatorEskf_DW.covP[100]) -
        stateEstimatorEskf_DW.covP[347]) + rtu_measNoiseR[144]);

      // 'errorStateEkf_function2:281' nu = baroAlt_m  + states(7) - states(20); 
      rtb_XAxis = (rtb_XAxis + rtb_states[6]) - rtb_states[19];

      // 'errorStateEkf_function2:282' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:284' if NIS < 3.68
      if (rtb_XAxis * rtb_XAxis * rtb_Product2_c < 3.68) {
        // 'errorStateEkf_function2:285' ekfDebug.isBaroUsed = true;
        // 'errorStateEkf_function2:286' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        stateModeReady = (mode == enumStateEstimateMode::RUN) |
          dhSensorIn_sensorValidity_isOfV;
        if (stateModeReady) {
          // 'errorStateEkf_function2:287' K = (covP(idxEs, 19) - covP(idxEs, 6)).*iS; 
          // 'errorStateEkf_function2:288' errorStateHat(idxEs) = K*nu;
          //            covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs); 
          // 'errorStateEkf_function2:290' covP(idxEs, idxEs) = covP(idxEs, idxEs) - ... 
          // 'errorStateEkf_function2:291'                     (-K * (covP(6, idxEs) - covP(19, idxEs))); 
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            rtb_XAxis2 = (stateEstimatorEskf_DW.covP[i_1 + 342] -
                          stateEstimatorEskf_DW.covP[i_1 + 95]) * rtb_Product2_c;
            errorStateHat[i_1] = rtb_XAxis2 * rtb_XAxis;
            d_K[i_1] = -rtb_XAxis2;
            b_errorStateHat[i_1] = stateEstimatorEskf_DW.covP[i + 5] -
              stateEstimatorEskf_DW.covP[i + 18];
            i += 19;
          }

          rtb_VectorConcatenate1_tmp = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            for (i = 0; i < 19; i++) {
              covP_tmp = i + rtb_VectorConcatenate1_tmp;
              stateEstimatorEskf_DW.covP[covP_tmp] -= d_K[i] *
                b_errorStateHat[i_2];
            }

            rtb_VectorConcatenate1_tmp += 19;
          }

          // Update the nominal state
          // 'errorStateEkf_function2:293' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
              errorStateHat[rtb_VectorConcatenate1_tmp + 3];
          }
        } else {
          // 'errorStateEkf_function2:294' else
          // 'errorStateEkf_function2:295' K = (covP(idxNoGpsEs, 19) - covP(idxNoGpsEs, 6)).*iS; 
          // 'errorStateEkf_function2:296' errorStateHat(idxNoGpsEs) = K*nu;
          //            covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs); 
          // 'errorStateEkf_function2:298' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - ... 
          // 'errorStateEkf_function2:299'                     (-K * (covP(6, idxNoGpsEs) - covP(19, idxNoGpsEs))); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            c = c_0[rtb_VectorConcatenate1_tmp];
            rtb_XAxis1 = (stateEstimatorEskf_DW.covP[c + 342] -
                          stateEstimatorEskf_DW.covP[c + 95]) * rtb_Product2_c;
            errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] = rtb_XAxis1 *
              rtb_XAxis;
            K_0[rtb_VectorConcatenate1_tmp] = -rtb_XAxis1;
            i_2 = 19 * c;
            tmp_2[rtb_VectorConcatenate1_tmp] = stateEstimatorEskf_DW.covP[i_2 +
              5] - stateEstimatorEskf_DW.covP[i_2 + 18];
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              tmp_5[i_2 + 15 * rtb_VectorConcatenate1_tmp] =
                stateEstimatorEskf_DW.covP[19 * c_0[rtb_VectorConcatenate1_tmp]
                + c_0[i_2]] - K_0[i_2] * tmp_2[rtb_VectorConcatenate1_tmp];
            }
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              stateEstimatorEskf_DW.covP[c_0[i_2] + 19 *
                c_0[rtb_VectorConcatenate1_tmp]] = tmp_5[15 *
                rtb_VectorConcatenate1_tmp + i_2];
            }
          }

          // Update the nominal state
          // 'errorStateEkf_function2:301' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states_0[rtb_VectorConcatenate1_tmp] =
              rtb_states[e[rtb_VectorConcatenate1_tmp]] +
              errorStateHat[f[rtb_VectorConcatenate1_tmp]];
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[e[rtb_VectorConcatenate1_tmp]] =
              rtb_states_0[rtb_VectorConcatenate1_tmp];
          }
        }

        // Construct quaternion from the rotation vector and reset covP
        // 'errorStateEkf_function2:305' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        qDelayed[0] = rtb_states[0];
        qDelayed[1] = rtb_states[1];
        qDelayed[2] = rtb_states[2];
        qDelayed[3] = rtb_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:306' states(1:4) = nomQuat;
        rtb_states[0] = qDelayed[0];
        rtb_states[1] = qDelayed[1];
        rtb_states[2] = qDelayed[2];
        rtb_states[3] = qDelayed[3];

        // 'errorStateEkf_function2:307' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (stateModeReady) {
          // 'errorStateEkf_function2:308' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
          rtb_VectorConcatenate1_tmp = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            i = 0;
            for (i_1 = 0; i_1 < 19; i_1++) {
              covP_tmp = i_1 + rtb_VectorConcatenate1_tmp;
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[i + i_2] +
                covP[covP_tmp]) * 0.5F;
              i += 19;
            }

            rtb_VectorConcatenate1_tmp += 19;
          }
        } else {
          // 'errorStateEkf_function2:309' else
          // 'errorStateEkf_function2:310' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:311'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
                c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:312' states(8:9) = 0;
          rtb_states[7] = 0.0F;
          rtb_states[8] = 0.0F;
        }
      }
    }

    // Fuse Lidar data if it is valid
    // 'errorStateEkf_function2:318' if(isLidarValid)
    if (dhSensorIn_sensorValidity_isLid) {
      // 'errorStateEkf_function2:319' iS = 1/(covP(6, 6) + measNoiseR(11, 11)); 
      rtb_Product2_c = 1.0F / (stateEstimatorEskf_DW.covP[100] + rtu_measNoiseR
        [160]);

      // 'errorStateEkf_function2:320' nu = lidarAgl_m  + states(7);
      rtb_XAxis = rtb_Product1_b + rtb_states[6];

      // 'errorStateEkf_function2:321' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:323' if NIS < 27
      if (rtb_XAxis * rtb_XAxis * rtb_Product2_c < 27.0F) {
        // 'errorStateEkf_function2:324' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        stateModeReady = (mode == enumStateEstimateMode::RUN) |
          dhSensorIn_sensorValidity_isOfV;
        if (stateModeReady) {
          // 'errorStateEkf_function2:325' K = -covP(idxEs, 6).*iS;
          // 'errorStateEkf_function2:326' errorStateHat(idxEs) = K*nu;
          // 'errorStateEkf_function2:328' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (-K*covP(6, idxEs)); 
          for (i = 0; i < 19; i++) {
            rtb_XAxis2 = -stateEstimatorEskf_DW.covP[i + 95] * rtb_Product2_c;
            errorStateHat[i] = rtb_XAxis2 * rtb_XAxis;
            d_K[i] = -rtb_XAxis2;
          }

          rtb_VectorConcatenate1_tmp = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            for (i = 0; i < 19; i++) {
              i_1 = i + rtb_VectorConcatenate1_tmp;
              b_covP[i_1] = stateEstimatorEskf_DW.covP[i_1] -
                stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 5] *
                d_K[i];
            }

            rtb_VectorConcatenate1_tmp += 19;
          }

          std::memcpy(&stateEstimatorEskf_DW.covP[0], &b_covP[0], 361U * sizeof
                      (real32_T));

          // Update the nominal state
          // 'errorStateEkf_function2:331' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
              errorStateHat[rtb_VectorConcatenate1_tmp + 3];
          }
        } else {
          // 'errorStateEkf_function2:332' else
          // 'errorStateEkf_function2:333' K = -covP(idxNoGpsEs, 6).*iS;
          // 'errorStateEkf_function2:334' errorStateHat(idxNoGpsEs) = K*nu;
          // 'errorStateEkf_function2:336' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - (-K*covP(6, idxNoGpsEs)); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            rtb_XAxis1 =
              -stateEstimatorEskf_DW.covP[c_0[rtb_VectorConcatenate1_tmp] + 95] *
              rtb_Product2_c;
            errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] = rtb_XAxis1 *
              rtb_XAxis;
            K_0[rtb_VectorConcatenate1_tmp] = -rtb_XAxis1;
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              i = 19 * c_0[rtb_VectorConcatenate1_tmp];
              tmp_5[i_2 + 15 * rtb_VectorConcatenate1_tmp] =
                stateEstimatorEskf_DW.covP[i + c_0[i_2]] -
                stateEstimatorEskf_DW.covP[i + 5] * K_0[i_2];
            }
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              stateEstimatorEskf_DW.covP[c_0[i_2] + 19 *
                c_0[rtb_VectorConcatenate1_tmp]] = tmp_5[15 *
                rtb_VectorConcatenate1_tmp + i_2];
            }
          }

          // Update the nominal state
          // 'errorStateEkf_function2:339' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states_0[rtb_VectorConcatenate1_tmp] =
              rtb_states[e[rtb_VectorConcatenate1_tmp]] +
              errorStateHat[f[rtb_VectorConcatenate1_tmp]];
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 12;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[e[rtb_VectorConcatenate1_tmp]] =
              rtb_states_0[rtb_VectorConcatenate1_tmp];
          }
        }

        // Construct quaternion from the rotation vector and reset covP
        // 'errorStateEkf_function2:343' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        qDelayed[0] = rtb_states[0];
        qDelayed[1] = rtb_states[1];
        qDelayed[2] = rtb_states[2];
        qDelayed[3] = rtb_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:344' states(1:4) = nomQuat;
        rtb_states[0] = qDelayed[0];
        rtb_states[1] = qDelayed[1];
        rtb_states[2] = qDelayed[2];
        rtb_states[3] = qDelayed[3];

        // 'errorStateEkf_function2:345' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (stateModeReady) {
          // 'errorStateEkf_function2:346' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
          rtb_VectorConcatenate1_tmp = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            i = 0;
            for (i_1 = 0; i_1 < 19; i_1++) {
              covP_tmp = i_1 + rtb_VectorConcatenate1_tmp;
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[i + i_2] +
                covP[covP_tmp]) * 0.5F;
              i += 19;
            }

            rtb_VectorConcatenate1_tmp += 19;
          }
        } else {
          // 'errorStateEkf_function2:347' else
          // 'errorStateEkf_function2:348' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:349'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
                c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:350' states(8:9) = 0;
          rtb_states[7] = 0.0F;
          rtb_states[8] = 0.0F;
        }
      }
    }

    // Fuse GPS data if it is valid
    // 'errorStateEkf_function2:356' if(isGpsValid && estSmMode == enumStateEstimateMode.RUN) 
    if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) &
         dhSensorIn_sensorValidity_isGps)) {
      // 'errorStateEkf_function2:357' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 4, idxEs, ... 
      // 'errorStateEkf_function2:358'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 4.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:359' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 5, idxEs, ... 
      // 'errorStateEkf_function2:360'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(2)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 5.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      //  if (~isBaroValid)
      // 'errorStateEkf_function2:362' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 6, idxEs, ... 
      // 'errorStateEkf_function2:363'                 idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(3)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 6.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        3.68F);

      //  end
      // 'errorStateEkf_function2:365' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 7, idxEs, ... 
      // 'errorStateEkf_function2:366'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(4)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 7.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:367' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:368'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(5)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 8.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:369' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 9, idxEs, ... 
      // 'errorStateEkf_function2:370'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(6)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
           rtb_VectorConcatenate1_tmp++) {
        tmp[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 1.0;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        tmp_0[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 4.0;
        tmp_1[rtb_VectorConcatenate1_tmp] = static_cast<real_T>
          (rtb_VectorConcatenate1_tmp) + 5.0;
      }

      applyGpsPosAndVelCorr_wtI6aFgH(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 9.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      //      K = covP(idxEs, 4:9)/ ...
      //          (covP(4:9, 4:9) + measNoiseR(4:9, 4:9));
      //      errorStateHat(idxEs) = K*(nedPosAndVel - states(5:10));
      //
      //      covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*covP(4:9, idxEs);
      //
      //      %Update the nominal state
      //      states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2);
      //
      //      %Construct quaternion from the rotation vector and reset covP
      //      [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      //      states(1:4) = nomQuat;
      //      if estSmMode == enumStateEstimateMode.RUN
      //          covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      //      else
      //          covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
      //              covP(idxNoGpsEs, idxNoGpsEs)').*0.5;
      //      end
    }

    // Compute Body To NED DCM
    //  dcmBodyToNed = quatToDcm_function(states(1:4));
    //  else
    //      states = prevStates;
    //  end
  }

  // End of MATLAB Function: '<S1>/EKF'

  // MATLAB Function: '<S12>/OutputPredictor' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   BusCreator: '<Root>/Bus Creator1'

  // MATLAB Function 'EKF/DelayedHorizonOutput/OutputPredictor': '<S16>:1'
  // '<S16>:1:4' statesOut = statesIn;
  std::memcpy(&rty_states[0], &rtb_states[0], 20U * sizeof(real32_T));

  // '<S16>:1:5' [quatOut, posOut, velOut, ~, ~, ~] = outputPredictor_function(statesIn, isImuValid, imuTimeIn_ms, ... 
  // '<S16>:1:6'                                                       imuIn, fusionTime_ms, gEarth_mps2, ... 
  // '<S16>:1:7'                                                       ekfParams.outputPredictorParams); 
  //
  //  Current-time output predictor for delayed-horizon ESKF.
  //
  //  imuTimeIn_ms:
  //    Current IMU timestamp.
  //
  //  fusionTime_ms:
  //    Delayed IMU timestamp returned by imuFifo and therefore the
  //    authoritative delayed ESKF fusion epoch.
  //
  //  states:
  //    Delayed ESKF state after prediction and aiding corrections at
  //    fusionTime_ms.
  //
  //  State layout:
  //    1:4   quaternion, body -> NED
  //    5:7   NED position [m]
  //    8:10  NED velocity [m/s]
  //    11:13 gyro bias [rad/s]
  //    14:16 accel bias [m/s^2]
  //
  //  outputPredictorParams:
  //    delaySamples
  //    posCorrectionGain
  //    velCorrectionGain
  //    attCorrectionGain
  // 'outputPredictor_function:33' MAX_SIZE = 64;
  // 'outputPredictor_function:35' STATUS_OK            = uint8(0);
  // 'outputPredictor_function:36' STATUS_BAD_CONFIG    = uint8(1);
  // 'outputPredictor_function:37' STATUS_SYNC_MISMATCH = uint8(2);
  // 'outputPredictor_function:45' if isempty(initialized)
  if (static_cast<boolean_T>(static_cast<int32_T>
       (stateEstimatorEskf_DW.initialized_not_empty) ^ 1)) {
    // 'outputPredictor_function:46' qCurrent = states(1:4);
    stateEstimatorEskf_DW.qCurrent[0] = rtb_states[0];
    stateEstimatorEskf_DW.qCurrent[1] = rtb_states[1];
    stateEstimatorEskf_DW.qCurrent[2] = rtb_states[2];
    stateEstimatorEskf_DW.qCurrent[3] = rtb_states[3];

    // 'outputPredictor_function:47' pCurrent = states(5:7);
    // 'outputPredictor_function:48' vCurrent = states(8:10);
    stateEstimatorEskf_DW.pCurrent[0] = rtb_states[4];
    stateEstimatorEskf_DW.vCurrent[0] = rtb_states[7];
    stateEstimatorEskf_DW.pCurrent[1] = rtb_states[5];
    stateEstimatorEskf_DW.vCurrent[1] = rtb_states[8];
    stateEstimatorEskf_DW.pCurrent[2] = rtb_states[6];
    stateEstimatorEskf_DW.vCurrent[2] = rtb_states[9];

    // 'outputPredictor_function:50' qNorm = norm(qCurrent);
    rtb_Product1_b = norm_NoMIKEmk(stateEstimatorEskf_DW.qCurrent);

    // 'outputPredictor_function:51' if qNorm > 1e-7
    if (rtb_Product1_b > 1.0E-7) {
      // 'outputPredictor_function:52' qCurrent = qCurrent/qNorm;
      stateEstimatorEskf_DW.qCurrent[0] /= rtb_Product1_b;
      stateEstimatorEskf_DW.qCurrent[1] /= rtb_Product1_b;
      stateEstimatorEskf_DW.qCurrent[2] /= rtb_Product1_b;
      stateEstimatorEskf_DW.qCurrent[3] /= rtb_Product1_b;
    } else {
      // 'outputPredictor_function:53' else
      // 'outputPredictor_function:54' qCurrent = single([1;0;0;0]);
      stateEstimatorEskf_DW.qCurrent[0] = 1.0F;
      stateEstimatorEskf_DW.qCurrent[1] = 0.0F;
      stateEstimatorEskf_DW.qCurrent[2] = 0.0F;
      stateEstimatorEskf_DW.qCurrent[3] = 0.0F;
    }

    // 'outputPredictor_function:57' currentTime_ms = uint64(0);
    // 'outputPredictor_function:59' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
    for (i = 0; i < 64; i++) {
      stateEstimatorEskf_DW.timeBuf_ms_d[i] = dhSensorIn_sensorTimestamp_ma_0;
    }

    // 'outputPredictor_function:60' quatBuf = zeros(4,MAX_SIZE,'single');
    std::memset(&stateEstimatorEskf_DW.quatBuf[0], 0, sizeof(real32_T) << 8U);

    // 'outputPredictor_function:61' posBuf = zeros(3,MAX_SIZE,'single');
    // 'outputPredictor_function:62' velBuf = zeros(3,MAX_SIZE,'single');
    std::memset(&stateEstimatorEskf_DW.posBuf[0], 0, 192U * sizeof(real32_T));
    std::memset(&stateEstimatorEskf_DW.velBuf[0], 0, 192U * sizeof(real32_T));

    // 'outputPredictor_function:64' head = uint16(1);
    stateEstimatorEskf_DW.head_l = 1U;

    // 'outputPredictor_function:65' tail = uint16(1);
    stateEstimatorEskf_DW.tail_j = 1U;

    // 'outputPredictor_function:66' count = uint16(0);
    stateEstimatorEskf_DW.count_a = 0U;

    // 'outputPredictor_function:68' initialized = true;
    stateEstimatorEskf_DW.initialized_not_empty = true;
  }

  // 'outputPredictor_function:71' quatOut = qCurrent;
  // 'outputPredictor_function:72' posOut = pCurrent;
  // 'outputPredictor_function:73' velOut = vCurrent;
  // 'outputPredictor_function:74' outputTime_ms = currentTime_ms;
  // 'outputPredictor_function:76' status = STATUS_OK;
  // 'outputPredictor_function:77' countOut = count;
  // 'outputPredictor_function:79' delaySamples = outputPredictorParams.delaySamples; 
  // 'outputPredictor_function:80' posGain = outputPredictorParams.posCorrectionGain; 
  // 'outputPredictor_function:81' velGain = outputPredictorParams.velCorrectionGain; 
  // 'outputPredictor_function:82' attGain = outputPredictorParams.attCorrectionGain; 
  // 'outputPredictor_function:84' if delaySamples < 1 || delaySamples > MAX_SIZE 
  if (rtu_imuData->isImuDataValid) {
    //  No new current-time IMU sample: hold previous controller-facing output.
    // 'outputPredictor_function:90' if ~isImuValid
    //  First current IMU epoch.
    //
    //  The initial ESKF state is associated with this epoch. No propagation is
    //  performed because there is no previous IMU interval available.
    // 'outputPredictor_function:98' if count == 0
    if (stateEstimatorEskf_DW.count_a == 0) {
      // 'outputPredictor_function:99' currentTime_ms = imuTimeIn_ms;
      // 'outputPredictor_function:101' timeBuf_ms(1) = currentTime_ms;
      stateEstimatorEskf_DW.timeBuf_ms_d[0] = rtu_imuData->timestamp_ms;

      // 'outputPredictor_function:102' quatBuf(:,1) = qCurrent;
      stateEstimatorEskf_DW.quatBuf[0] = stateEstimatorEskf_DW.qCurrent[0];
      stateEstimatorEskf_DW.quatBuf[1] = stateEstimatorEskf_DW.qCurrent[1];
      stateEstimatorEskf_DW.quatBuf[2] = stateEstimatorEskf_DW.qCurrent[2];
      stateEstimatorEskf_DW.quatBuf[3] = stateEstimatorEskf_DW.qCurrent[3];

      // 'outputPredictor_function:103' posBuf(:,1) = pCurrent;
      // 'outputPredictor_function:104' velBuf(:,1) = vCurrent;
      stateEstimatorEskf_DW.posBuf[0] = stateEstimatorEskf_DW.pCurrent[0];
      stateEstimatorEskf_DW.velBuf[0] = stateEstimatorEskf_DW.vCurrent[0];
      stateEstimatorEskf_DW.posBuf[1] = stateEstimatorEskf_DW.pCurrent[1];
      stateEstimatorEskf_DW.velBuf[1] = stateEstimatorEskf_DW.vCurrent[1];
      stateEstimatorEskf_DW.posBuf[2] = stateEstimatorEskf_DW.pCurrent[2];
      stateEstimatorEskf_DW.velBuf[2] = stateEstimatorEskf_DW.vCurrent[2];

      // 'outputPredictor_function:106' tail = uint16(1);
      stateEstimatorEskf_DW.tail_j = 1U;

      // 'outputPredictor_function:107' count = uint16(1);
      stateEstimatorEskf_DW.count_a = 1U;

      // 'outputPredictor_function:109' if delaySamples == 1
      // 'outputPredictor_function:111' else
      // 'outputPredictor_function:112' head = uint16(2);
      stateEstimatorEskf_DW.head_l = 2U;

      // 'outputPredictor_function:115' quatOut = qCurrent;
      // 'outputPredictor_function:116' posOut = pCurrent;
      // 'outputPredictor_function:117' velOut = vCurrent;
      // 'outputPredictor_function:118' outputTime_ms = currentTime_ms;
      // 'outputPredictor_function:119' countOut = count;
    } else {
      // 'outputPredictor_function:123' deltaAngleCorr = zeros(3,1,'single');
      rtb_CastToSingle[0] = 0.0F;
      rtb_CastToSingle[1] = 0.0F;
      rtb_CastToSingle[2] = 0.0F;

      //  ------------------------------------------------------------------------- 
      //  DELAYED ESKF -> OUTPUT PREDICTOR CORRECTION
      //  ------------------------------------------------------------------------- 
      //
      //  A completely filled output history means delaySamples IMU intervals
      //  have accumulated.
      //
      //  With synchronous 250 Hz IMU/output-predictor/ESKF execution, this is
      //  exactly when imuFifo has begun providing the delayed fusion horizon.
      // 'outputPredictor_function:134' if count == delaySamples
      if ((stateEstimatorEskf_DW.count_a == 50) && uMultiWordEq
          (&stateEstimatorEskf_DW.timeBuf_ms_d[stateEstimatorEskf_DW.tail_j - 1]
           .chunks[0U], &dhSensorIn_sensorTimestamp_imuT.chunks[0U], 2)) {
        //  Both timestamps originate from the same uint64 IMU timestamp stream. 
        // 'outputPredictor_function:137' if timeBuf_ms(tail) == fusionTime_ms
        //  Position/velocity error at delayed fusion horizon.
        // 'outputPredictor_function:140' posError = states(5:7) - posBuf(:,tail); 
        // 'outputPredictor_function:141' velError = states(8:10) - velBuf(:,tail); 
        // 'outputPredictor_function:143' posCorrection = posGain*posError;
        // 'outputPredictor_function:144' velCorrection = velGain*velError;
        rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.tail_j - 1) * 3;
        Divide[0] = (rtb_states[4] -
                     stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp]) *
          0.02F;
        rtb_UnitDelay_g = (rtb_states[7] -
                           stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp])
          * 0.02F;
        Divide[1] = (rtb_states[5] -
                     stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 1])
          * 0.02F;
        rtb_Sum2_idx_1 = (rtb_states[8] -
                          stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp
                          + 1]) * 0.02F;
        Divide[2] = (rtb_states[6] -
                     stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 2])
          * 0.02F;
        rtb_Sum2_idx_2 = (rtb_states[9] -
                          stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp
                          + 2]) * 0.02F;

        //  Apply the same p/v correction to all valid output-history states.
        // 'outputPredictor_function:147' idx = tail;
        nextIdx = stateEstimatorEskf_DW.tail_j;

        // 'outputPredictor_function:149' for k = 1:MAX_SIZE
        rtb_VectorConcatenate1_tmp = 0;
        while (static_cast<boolean_T>((rtb_VectorConcatenate1_tmp + 1 <= 50) &
                (rtb_VectorConcatenate1_tmp < 64))) {
          // 'outputPredictor_function:150' if k > count
          // 'outputPredictor_function:154' posBuf(:,idx) = posBuf(:,idx) + posCorrection; 
          i_2 = (nextIdx - 1) * 3;
          rtb_XAxis = stateEstimatorEskf_DW.posBuf[i_2 + 1] + Divide[1];
          rtb_Product1_b = stateEstimatorEskf_DW.posBuf[i_2 + 2] + Divide[2];
          stateEstimatorEskf_DW.posBuf[i_2] += Divide[0];
          stateEstimatorEskf_DW.posBuf[i_2 + 1] = rtb_XAxis;
          stateEstimatorEskf_DW.posBuf[i_2 + 2] = rtb_Product1_b;

          // 'outputPredictor_function:155' velBuf(:,idx) = velBuf(:,idx) + velCorrection; 
          rtb_XAxis = stateEstimatorEskf_DW.velBuf[i_2 + 1] + rtb_Sum2_idx_1;
          rtb_Product1_b = stateEstimatorEskf_DW.velBuf[i_2 + 2] +
            rtb_Sum2_idx_2;
          stateEstimatorEskf_DW.velBuf[i_2] += rtb_UnitDelay_g;
          stateEstimatorEskf_DW.velBuf[i_2 + 1] = rtb_XAxis;
          stateEstimatorEskf_DW.velBuf[i_2 + 2] = rtb_Product1_b;

          // 'outputPredictor_function:157' if idx == delaySamples
          if (nextIdx == 50) {
            // 'outputPredictor_function:158' idx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'outputPredictor_function:159' else
            // 'outputPredictor_function:160' idx = idx + 1;
            qY = nextIdx + 1U;
            if (nextIdx + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          rtb_VectorConcatenate1_tmp++;
        }

        //  Current controller-facing predictor state receives the same
        //  position/velocity correction.
        // 'outputPredictor_function:166' pCurrent = pCurrent + posCorrection;
        // 'outputPredictor_function:167' vCurrent = vCurrent + velCorrection;
        stateEstimatorEskf_DW.pCurrent[0] += Divide[0];
        stateEstimatorEskf_DW.vCurrent[0] += rtb_UnitDelay_g;
        stateEstimatorEskf_DW.pCurrent[1] += Divide[1];
        stateEstimatorEskf_DW.vCurrent[1] += rtb_Sum2_idx_1;
        stateEstimatorEskf_DW.pCurrent[2] += Divide[2];
        stateEstimatorEskf_DW.vCurrent[2] += rtb_Sum2_idx_2;

        //  -------------------------------------------------------------
        //  ATTITUDE ERROR AT DELAYED FUSION HORIZON
        //  -------------------------------------------------------------
        // 'outputPredictor_function:173' qDelayed = quatBuf(:,tail);
        // 'outputPredictor_function:174' qEskf = states(1:4);
        // 'outputPredictor_function:176' qNorm = norm(qDelayed);
        rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.tail_j - 1) << 2;
        rtb_Product1_b = norm_NoMIKEmk
          (&stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp]);

        // 'outputPredictor_function:177' if qNorm > 1e-7
        if (rtb_Product1_b > 1.0E-7) {
          // 'outputPredictor_function:178' qDelayed = qDelayed/qNorm;
          qDelayed[0] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp]
            / rtb_Product1_b;
          qDelayed[1] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 1] / rtb_Product1_b;
          qDelayed[2] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 2] / rtb_Product1_b;
          qDelayed[3] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 3] / rtb_Product1_b;
        } else {
          // 'outputPredictor_function:179' else
          // 'outputPredictor_function:180' qDelayed = single([1;0;0;0]);
          qDelayed[0] = 1.0F;
          qDelayed[1] = 0.0F;
          qDelayed[2] = 0.0F;
          qDelayed[3] = 0.0F;
        }

        // 'outputPredictor_function:183' qNorm = norm(qEskf);
        rtb_Product1_b = norm_NoMIKEmk(&rtb_states[0]);

        // 'outputPredictor_function:184' if qNorm > 1e-7
        if (rtb_Product1_b > 1.0E-7) {
          // 'outputPredictor_function:185' qEskf = qEskf/qNorm;
          rtb_XAxis = rtb_states[0] / rtb_Product1_b;
          rtb_XAxis1 = rtb_states[1] / rtb_Product1_b;
          rtb_XAxis2 = rtb_states[2] / rtb_Product1_b;
          rtb_Product1_b = rtb_states[3] / rtb_Product1_b;
        } else {
          // 'outputPredictor_function:186' else
          // 'outputPredictor_function:187' qEskf = single([1;0;0;0]);
          rtb_XAxis = 1.0F;
          rtb_XAxis1 = 0.0F;
          rtb_XAxis2 = 0.0F;
          rtb_Product1_b = 0.0F;
        }

        //  Right-multiplicative quaternion convention:
        //
        //  qEskf = qDelayed (*) qError
        //
        //  therefore:
        //
        //  qError = inverse(qDelayed) (*) qEskf
        // 'outputPredictor_function:197' qDelayedInv = ...
        // 'outputPredictor_function:198'             [ qDelayed(1);
        // 'outputPredictor_function:199'              -qDelayed(2);
        // 'outputPredictor_function:200'              -qDelayed(3);
        // 'outputPredictor_function:201'              -qDelayed(4)];
        // 'outputPredictor_function:203' qError = quatMultiplyLocal(qDelayedInv,qEskf); 
        // 'outputPredictor_function:331' q = zeros(4,1,'single');
        // 'outputPredictor_function:333' q(1) = q1(1)*q2(1) - ...
        // 'outputPredictor_function:334'        q1(2)*q2(2) - ...
        // 'outputPredictor_function:335'        q1(3)*q2(3) - ...
        // 'outputPredictor_function:336'        q1(4)*q2(4);
        qError[0] = ((qDelayed[0] * rtb_XAxis - -qDelayed[1] * rtb_XAxis1) -
                     -qDelayed[2] * rtb_XAxis2) - -qDelayed[3] * rtb_Product1_b;

        // 'outputPredictor_function:338' q(2) = q1(1)*q2(2) + ...
        // 'outputPredictor_function:339'        q1(2)*q2(1) + ...
        // 'outputPredictor_function:340'        q1(3)*q2(4) - ...
        // 'outputPredictor_function:341'        q1(4)*q2(3);
        qError[1] = ((qDelayed[0] * rtb_XAxis1 + rtb_XAxis * -qDelayed[1]) +
                     -qDelayed[2] * rtb_Product1_b) - rtb_XAxis2 * -qDelayed[3];

        // 'outputPredictor_function:343' q(3) = q1(1)*q2(3) - ...
        // 'outputPredictor_function:344'        q1(2)*q2(4) + ...
        // 'outputPredictor_function:345'        q1(3)*q2(1) + ...
        // 'outputPredictor_function:346'        q1(4)*q2(2);
        qError[2] = ((qDelayed[0] * rtb_XAxis2 - -qDelayed[1] * rtb_Product1_b)
                     + rtb_XAxis * -qDelayed[2]) + rtb_XAxis1 * -qDelayed[3];

        // 'outputPredictor_function:348' q(4) = q1(1)*q2(4) + ...
        // 'outputPredictor_function:349'        q1(2)*q2(3) - ...
        // 'outputPredictor_function:350'        q1(3)*q2(2) + ...
        // 'outputPredictor_function:351'        q1(4)*q2(1);
        qError[3] = ((qDelayed[0] * rtb_Product1_b + -qDelayed[1] * rtb_XAxis2)
                     - rtb_XAxis1 * -qDelayed[2]) + rtb_XAxis * -qDelayed[3];

        //  Select shortest equivalent quaternion rotation.
        // 'outputPredictor_function:206' if qError(1) < 0
        if (qError[0] < 0.0F) {
          // 'outputPredictor_function:207' qError = -qError;
          qError[0] = -qError[0];
          qError[1] = -qError[1];
          qError[2] = -qError[2];
          qError[3] = -qError[3];
        }

        // 'outputPredictor_function:210' qErrorVector = qError(2:4);
        // 'outputPredictor_function:211' qErrorVectorNorm = norm(qErrorVector); 
        rtb_Product1_b = norm_94qjDDKI(&qError[1]);

        // 'outputPredictor_function:213' if qErrorVectorNorm > 1e-7
        if (rtb_Product1_b > 1.0E-7) {
          // 'outputPredictor_function:214' attitudeErrorNorm = ...
          // 'outputPredictor_function:215'                 single(2)*atan2(qErrorVectorNorm,qError(1)); 
          // 'outputPredictor_function:217' attitudeError = ...
          // 'outputPredictor_function:218'                 (attitudeErrorNorm/qErrorVectorNorm)*qErrorVector; 
          rtb_Product1_b = 2.0F * std::atan2(rtb_Product1_b, qError[0]) /
            rtb_Product1_b;

          // 'outputPredictor_function:220' deltaAngleCorr = attGain*attitudeError; 
          rtb_CastToSingle[0] = rtb_Product1_b * qError[1] * 0.01F;
          rtb_CastToSingle[1] = rtb_Product1_b * qError[2] * 0.01F;
          rtb_CastToSingle[2] = rtb_Product1_b * qError[3] * 0.01F;
        }

        //  Delayed state has now been consumed.
        // 'outputPredictor_function:224' if tail == delaySamples
        if (stateEstimatorEskf_DW.tail_j == 50) {
          // 'outputPredictor_function:225' tail = uint16(1);
          stateEstimatorEskf_DW.tail_j = 1U;
        } else {
          // 'outputPredictor_function:226' else
          // 'outputPredictor_function:227' tail = tail + 1;
          qY = stateEstimatorEskf_DW.tail_j + 1U;
          if (stateEstimatorEskf_DW.tail_j + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.tail_j = static_cast<uint16_T>(qY);
        }

        // 'outputPredictor_function:230' count = count - 1;
        stateEstimatorEskf_DW.count_a = 49U;
      } else {
        // 'outputPredictor_function:232' else
        //  Exact timestamp equality is an estimator timing invariant.
        // 'outputPredictor_function:234' status = STATUS_SYNC_MISMATCH;
      }

      //  ------------------------------------------------------------------------- 
      //  CURRENT-TIME INERTIAL PROPAGATION
      //  ------------------------------------------------------------------------- 
      // 'outputPredictor_function:242' bodyAccels_mps2 = imuIn(1:3);
      // 'outputPredictor_function:243' bodyRates_radps = imuIn(4:6);
      //  Bias estimates remain authoritative delayed-ESKF states. The output
      //  predictor does not maintain separate bias states.
      // 'outputPredictor_function:247' gyroBias_radps = states(11:13);
      // 'outputPredictor_function:248' accelBias_mps2 = states(14:16);
      // 'outputPredictor_function:250' C_bodyToNed = quatToDcmBodyToNed(qCurrent); 
      // 'outputPredictor_function:358' q0 = q(1);
      // 'outputPredictor_function:359' q1 = q(2);
      // 'outputPredictor_function:360' q2 = q(3);
      // 'outputPredictor_function:361' q3 = q(4);
      // 'outputPredictor_function:363' q1q1 = q1*q1;
      rtb_Product1_b = stateEstimatorEskf_DW.qCurrent[1] *
        stateEstimatorEskf_DW.qCurrent[1];

      // 'outputPredictor_function:364' q2q2 = q2*q2;
      rtb_XAxis = stateEstimatorEskf_DW.qCurrent[2] *
        stateEstimatorEskf_DW.qCurrent[2];

      // 'outputPredictor_function:365' q3q3 = q3*q3;
      rtb_Product2_c = stateEstimatorEskf_DW.qCurrent[3] *
        stateEstimatorEskf_DW.qCurrent[3];

      // 'outputPredictor_function:367' q0q1 = q0*q1;
      rtb_XAxis1 = stateEstimatorEskf_DW.qCurrent[0] *
        stateEstimatorEskf_DW.qCurrent[1];

      // 'outputPredictor_function:368' q0q2 = q0*q2;
      rtb_XAxis2 = stateEstimatorEskf_DW.qCurrent[0] *
        stateEstimatorEskf_DW.qCurrent[2];

      // 'outputPredictor_function:369' q0q3 = q0*q3;
      rtb_UnitDelay_g = stateEstimatorEskf_DW.qCurrent[0] *
        stateEstimatorEskf_DW.qCurrent[3];

      // 'outputPredictor_function:371' q1q2 = q1*q2;
      rtb_Sum2_idx_1 = stateEstimatorEskf_DW.qCurrent[1] *
        stateEstimatorEskf_DW.qCurrent[2];

      // 'outputPredictor_function:372' q1q3 = q1*q3;
      rtb_Sum2_idx_2 = stateEstimatorEskf_DW.qCurrent[1] *
        stateEstimatorEskf_DW.qCurrent[3];

      // 'outputPredictor_function:373' q2q3 = q2*q3;
      q2q3 = stateEstimatorEskf_DW.qCurrent[2] * stateEstimatorEskf_DW.qCurrent
        [3];

      // 'outputPredictor_function:375' C_bodyToNed = zeros(3,3,'single');
      // 'outputPredictor_function:377' C_bodyToNed(1,1) = 1 - 2*(q2q2 + q3q3);
      // 'outputPredictor_function:378' C_bodyToNed(1,2) = 2*(q1q2 - q0q3);
      // 'outputPredictor_function:379' C_bodyToNed(1,3) = 2*(q1q3 + q0q2);
      // 'outputPredictor_function:381' C_bodyToNed(2,1) = 2*(q1q2 + q0q3);
      // 'outputPredictor_function:382' C_bodyToNed(2,2) = 1 - 2*(q1q1 + q3q3);
      // 'outputPredictor_function:383' C_bodyToNed(2,3) = 2*(q2q3 - q0q1);
      // 'outputPredictor_function:385' C_bodyToNed(3,1) = 2*(q1q3 - q0q2);
      // 'outputPredictor_function:386' C_bodyToNed(3,2) = 2*(q2q3 + q0q1);
      // 'outputPredictor_function:387' C_bodyToNed(3,3) = 1 - 2*(q1q1 + q2q2);
      // 'outputPredictor_function:252' accelNed_mps2 = ...
      // 'outputPredictor_function:253'     C_bodyToNed*(bodyAccels_mps2 - accelBias_mps2) + ... 
      // 'outputPredictor_function:254'     [single(0);single(0);gEarth_mps2];
      //  Same first-order integration convention as the nominal ESKF.
      // 'outputPredictor_function:257' sampleTime_s = imuIn(7);
      // 'outputPredictor_function:258' pCurrent = pCurrent + sampleTime_s*vCurrent; 
      // 'outputPredictor_function:259' vCurrent = vCurrent + sampleTime_s*accelNed_mps2; 
      stateEstimatorEskf_DW.pCurrent[0] += rtu_imuData->dtImuTime_s *
        stateEstimatorEskf_DW.vCurrent[0];
      rtb_dcmBodyToNed_idx_7 =
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] - rtb_states[13];
      stateEstimatorEskf_DW.pCurrent[1] += rtu_imuData->dtImuTime_s *
        stateEstimatorEskf_DW.vCurrent[1];
      rtb_dcmBodyToNed_idx_2 =
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] - rtb_states[14];
      stateEstimatorEskf_DW.pCurrent[2] += rtu_imuData->dtImuTime_s *
        stateEstimatorEskf_DW.vCurrent[2];
      rtb_dcmBodyToNed_idx_5 =
        stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] - rtb_states[15];

      //  Current gyro propagation plus delayed-horizon complementary attitude
      //  correction.
      // 'outputPredictor_function:263' dTheta = ...
      // 'outputPredictor_function:264'     (bodyRates_radps - gyroBias_radps)*sampleTime_s + ... 
      // 'outputPredictor_function:265'     deltaAngleCorr;
      stateEstimatorEskf_DW.vCurrent[0] += (((1.0F - (rtb_XAxis + rtb_Product2_c)
        * 2.0F) * rtb_dcmBodyToNed_idx_7 + (rtb_Sum2_idx_1 - rtb_UnitDelay_g) *
        2.0F * rtb_dcmBodyToNed_idx_2) + (rtb_Sum2_idx_2 + rtb_XAxis2) * 2.0F *
        rtb_dcmBodyToNed_idx_5) * rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[0] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] - rtb_states[10])
        * rtu_imuData->dtImuTime_s;
      stateEstimatorEskf_DW.vCurrent[1] += (((1.0F - (rtb_Product1_b +
        rtb_Product2_c) * 2.0F) * rtb_dcmBodyToNed_idx_2 + (rtb_Sum2_idx_1 +
        rtb_UnitDelay_g) * 2.0F * rtb_dcmBodyToNed_idx_7) + (q2q3 - rtb_XAxis1) *
        2.0F * rtb_dcmBodyToNed_idx_5) * rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[1] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] - rtb_states[11])
        * rtu_imuData->dtImuTime_s;
      stateEstimatorEskf_DW.vCurrent[2] += ((((rtb_Sum2_idx_2 - rtb_XAxis2) *
        2.0F * rtb_dcmBodyToNed_idx_7 + (q2q3 + rtb_XAxis1) * 2.0F *
        rtb_dcmBodyToNed_idx_2) + (1.0F - (rtb_Product1_b + rtb_XAxis) * 2.0F) *
        rtb_dcmBodyToNed_idx_5) + *rtu_gEarth_mps2) * rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[2] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] - rtb_states[12])
        * rtu_imuData->dtImuTime_s;

      // 'outputPredictor_function:267' dThetaNorm = norm(dTheta);
      rtb_Product2_c = norm_94qjDDKI(rtb_CastToSingle);

      // 'outputPredictor_function:269' if dThetaNorm > 1e-7
      if (rtb_Product2_c > 1.0E-7) {
        // 'outputPredictor_function:270' dThetaUnit = dTheta/dThetaNorm;
        // 'outputPredictor_function:272' dq = ...
        // 'outputPredictor_function:273'         [cos(single(0.5)*dThetaNorm);
        // 'outputPredictor_function:274'          sin(single(0.5)*dThetaNorm)*dThetaUnit]; 
        rtb_XAxis = 0.5F * rtb_Product2_c;
        rtb_Product1_b = std::sin(rtb_XAxis);
        rtb_XAxis = std::cos(rtb_XAxis);
        rtb_XAxis1 = rtb_CastToSingle[0] / rtb_Product2_c * rtb_Product1_b;
        rtb_XAxis2 = rtb_CastToSingle[1] / rtb_Product2_c * rtb_Product1_b;
        rtb_Product1_b *= rtb_CastToSingle[2] / rtb_Product2_c;

        // 'outputPredictor_function:276' qCurrent = quatMultiplyLocal(qCurrent,dq); 
        rtb_Product2_c = stateEstimatorEskf_DW.qCurrent[0];
        rtb_UnitDelay_g = stateEstimatorEskf_DW.qCurrent[1];
        rtb_Sum2_idx_1 = stateEstimatorEskf_DW.qCurrent[2];

        // 'outputPredictor_function:331' q = zeros(4,1,'single');
        // 'outputPredictor_function:333' q(1) = q1(1)*q2(1) - ...
        // 'outputPredictor_function:334'        q1(2)*q2(2) - ...
        // 'outputPredictor_function:335'        q1(3)*q2(3) - ...
        // 'outputPredictor_function:336'        q1(4)*q2(4);
        stateEstimatorEskf_DW.qCurrent[0] = ((stateEstimatorEskf_DW.qCurrent[0] *
          rtb_XAxis - stateEstimatorEskf_DW.qCurrent[1] * rtb_XAxis1) -
          stateEstimatorEskf_DW.qCurrent[2] * rtb_XAxis2) -
          stateEstimatorEskf_DW.qCurrent[3] * rtb_Product1_b;

        // 'outputPredictor_function:338' q(2) = q1(1)*q2(2) + ...
        // 'outputPredictor_function:339'        q1(2)*q2(1) + ...
        // 'outputPredictor_function:340'        q1(3)*q2(4) - ...
        // 'outputPredictor_function:341'        q1(4)*q2(3);
        stateEstimatorEskf_DW.qCurrent[1] = ((rtb_Product2_c * rtb_XAxis1 +
          rtb_XAxis * stateEstimatorEskf_DW.qCurrent[1]) +
          stateEstimatorEskf_DW.qCurrent[2] * rtb_Product1_b) - rtb_XAxis2 *
          stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:343' q(3) = q1(1)*q2(3) - ...
        // 'outputPredictor_function:344'        q1(2)*q2(4) + ...
        // 'outputPredictor_function:345'        q1(3)*q2(1) + ...
        // 'outputPredictor_function:346'        q1(4)*q2(2);
        stateEstimatorEskf_DW.qCurrent[2] = ((rtb_Product2_c * rtb_XAxis2 -
          rtb_UnitDelay_g * rtb_Product1_b) + rtb_XAxis *
          stateEstimatorEskf_DW.qCurrent[2]) + rtb_XAxis1 *
          stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:348' q(4) = q1(1)*q2(4) + ...
        // 'outputPredictor_function:349'        q1(2)*q2(3) - ...
        // 'outputPredictor_function:350'        q1(3)*q2(2) + ...
        // 'outputPredictor_function:351'        q1(4)*q2(1);
        stateEstimatorEskf_DW.qCurrent[3] = ((rtb_Product2_c * rtb_Product1_b +
          rtb_UnitDelay_g * rtb_XAxis2) - rtb_XAxis1 * rtb_Sum2_idx_1) +
          rtb_XAxis * stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:278' qNorm = norm(qCurrent);
        rtb_Product1_b = norm_NoMIKEmk(stateEstimatorEskf_DW.qCurrent);

        // 'outputPredictor_function:279' if qNorm > 1e-7
        if (rtb_Product1_b > 1.0E-7) {
          // 'outputPredictor_function:280' qCurrent = qCurrent/qNorm;
          stateEstimatorEskf_DW.qCurrent[0] /= rtb_Product1_b;
          stateEstimatorEskf_DW.qCurrent[1] /= rtb_Product1_b;
          stateEstimatorEskf_DW.qCurrent[2] /= rtb_Product1_b;
          stateEstimatorEskf_DW.qCurrent[3] /= rtb_Product1_b;
        } else {
          // 'outputPredictor_function:281' else
          // 'outputPredictor_function:282' qCurrent = single([1;0;0;0]);
          stateEstimatorEskf_DW.qCurrent[0] = 1.0F;
          stateEstimatorEskf_DW.qCurrent[1] = 0.0F;
          stateEstimatorEskf_DW.qCurrent[2] = 0.0F;
          stateEstimatorEskf_DW.qCurrent[3] = 0.0F;
        }
      }

      // 'outputPredictor_function:286' currentTime_ms = imuTimeIn_ms;
      //  ------------------------------------------------------------------------- 
      //  PUSH NEW CURRENT-TIME OUTPUT STATE
      //  ------------------------------------------------------------------------- 
      // 'outputPredictor_function:292' bufferWasFull = (count == delaySamples); 
      // 'outputPredictor_function:294' timeBuf_ms(head) = currentTime_ms;
      stateEstimatorEskf_DW.timeBuf_ms_d[stateEstimatorEskf_DW.head_l - 1] =
        rtu_imuData->timestamp_ms;

      // 'outputPredictor_function:295' quatBuf(:,head) = qCurrent;
      rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.head_l - 1) << 2;
      stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp] =
        stateEstimatorEskf_DW.qCurrent[0];
      stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp + 1] =
        stateEstimatorEskf_DW.qCurrent[1];
      stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp + 2] =
        stateEstimatorEskf_DW.qCurrent[2];
      stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp + 3] =
        stateEstimatorEskf_DW.qCurrent[3];

      // 'outputPredictor_function:296' posBuf(:,head) = pCurrent;
      // 'outputPredictor_function:297' velBuf(:,head) = vCurrent;
      rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.head_l - 1) * 3;
      stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp] =
        stateEstimatorEskf_DW.pCurrent[0];
      stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp] =
        stateEstimatorEskf_DW.vCurrent[0];
      stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 1] =
        stateEstimatorEskf_DW.pCurrent[1];
      stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp + 1] =
        stateEstimatorEskf_DW.vCurrent[1];
      stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 2] =
        stateEstimatorEskf_DW.pCurrent[2];
      stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp + 2] =
        stateEstimatorEskf_DW.vCurrent[2];

      // 'outputPredictor_function:299' if head == delaySamples
      if (stateEstimatorEskf_DW.head_l == 50) {
        // 'outputPredictor_function:300' head = uint16(1);
        stateEstimatorEskf_DW.head_l = 1U;
      } else {
        // 'outputPredictor_function:301' else
        // 'outputPredictor_function:302' head = head + 1;
        qY = stateEstimatorEskf_DW.head_l + 1U;
        if (stateEstimatorEskf_DW.head_l + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.head_l = static_cast<uint16_T>(qY);
      }

      // 'outputPredictor_function:305' if bufferWasFull
      if (stateEstimatorEskf_DW.count_a == 50) {
        //  This should only occur if the delayed-horizon correction was not
        //  consumed, for example following a timestamp synchronization fault.
        //  Retain the newest delaySamples states.
        // 'outputPredictor_function:309' if tail == delaySamples
        if (stateEstimatorEskf_DW.tail_j == 50) {
          // 'outputPredictor_function:310' tail = uint16(1);
          stateEstimatorEskf_DW.tail_j = 1U;
        } else {
          // 'outputPredictor_function:311' else
          // 'outputPredictor_function:312' tail = tail + 1;
          qY = stateEstimatorEskf_DW.tail_j + 1U;
          if (stateEstimatorEskf_DW.tail_j + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.tail_j = static_cast<uint16_T>(qY);
        }

        // 'outputPredictor_function:315' count = delaySamples;
      } else {
        // 'outputPredictor_function:316' else
        // 'outputPredictor_function:317' count = count + 1;
        qY = stateEstimatorEskf_DW.count_a + 1U;
        if (stateEstimatorEskf_DW.count_a + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.count_a = static_cast<uint16_T>(qY);
      }

      // 'outputPredictor_function:320' quatOut = qCurrent;
      // 'outputPredictor_function:321' posOut = pCurrent;
      // 'outputPredictor_function:322' velOut = vCurrent;
      // 'outputPredictor_function:323' outputTime_ms = currentTime_ms;
      // 'outputPredictor_function:324' countOut = count;
    }
  } else {
    // 'outputPredictor_function:85' status = STATUS_BAD_CONFIG;
  }

  // '<S16>:1:8' statesOut(1:4) = quatOut;
  rty_states[0] = stateEstimatorEskf_DW.qCurrent[0];
  rty_states[1] = stateEstimatorEskf_DW.qCurrent[1];
  rty_states[2] = stateEstimatorEskf_DW.qCurrent[2];
  rty_states[3] = stateEstimatorEskf_DW.qCurrent[3];

  // '<S16>:1:9' statesOut(5:7) = posOut;
  // '<S16>:1:10' statesOut(8:10) = velOut;
  rty_states[4] = stateEstimatorEskf_DW.pCurrent[0];
  rty_states[7] = stateEstimatorEskf_DW.vCurrent[0];
  rty_states[5] = stateEstimatorEskf_DW.pCurrent[1];
  rty_states[8] = stateEstimatorEskf_DW.vCurrent[1];
  rty_states[6] = stateEstimatorEskf_DW.pCurrent[2];
  rty_states[9] = stateEstimatorEskf_DW.vCurrent[2];

  // End of MATLAB Function: '<S12>/OutputPredictor'

  // MATLAB Function: '<S1>/QuatToDCM'
  // MATLAB Function 'EKF/QuatToDCM': '<S15>:1'
  // '<S15>:1:2' dcmBodyToNed = quatToDcm_function(quat);
  // Quaternions
  // 'quatToDcm_function:3' q0 = quat(1);
  // 'quatToDcm_function:4' q1 = quat(2);
  // 'quatToDcm_function:5' q2 = quat(3);
  // 'quatToDcm_function:6' q3 = quat(4);
  //  Direction Cosine Matrix (DCM) from body cooridinates to NED coordinates
  //  expressed using quaternions.
  // 'quatToDcm_function:10' dcmBodyToNed = [1-2*(q2^2+q3^2), 2*(q1*q2-q3*q0), 2*(q1*q3+q2*q0); 
  // 'quatToDcm_function:11'     2*(q1*q2+q3*q0), 1-2*(q1^2+q3^2), 2*(q2*q3-q1*q0); 
  // 'quatToDcm_function:12'     2*(q1*q3-q2*q0), 2*(q2*q3+q1*q0), 1-2*(q1^2+q2^2)]; 
  rtb_XAxis2 = 1.0F - (rty_states[2] * rty_states[2] + rty_states[3] *
                       rty_states[3]) * 2.0F;
  rtb_UnitDelay_g = (rty_states[1] * rty_states[2] - rty_states[0] * rty_states
                     [3]) * 2.0F;
  rtb_Sum2_idx_1 = (rty_states[1] * rty_states[3] + rty_states[0] * rty_states[2])
    * 2.0F;
  rtb_Sum2_idx_2 = (rty_states[1] * rty_states[2] + rty_states[0] * rty_states[3])
    * 2.0F;
  q2q3 = 1.0F - (rty_states[1] * rty_states[1] + rty_states[3] * rty_states[3]) *
    2.0F;
  rtb_dcmBodyToNed_idx_7 = (rty_states[2] * rty_states[3] - rty_states[0] *
    rty_states[1]) * 2.0F;
  rtb_dcmBodyToNed_idx_2 = (rty_states[1] * rty_states[3] - rty_states[0] *
    rty_states[2]) * 2.0F;
  rtb_dcmBodyToNed_idx_5 = (rty_states[2] * rty_states[3] + rty_states[0] *
    rty_states[1]) * 2.0F;
  rtb_dcmBodyToNed_idx_8 = 1.0F - (rty_states[1] * rty_states[1] + rty_states[2]
    * rty_states[2]) * 2.0F;

  // Sqrt: '<S58>/sqrt' incorporates:
  //   Product: '<S59>/Product'
  //   Product: '<S59>/Product1'
  //   Product: '<S59>/Product2'
  //   Product: '<S59>/Product3'
  //   Sum: '<S59>/Sum'

  rtb_XAxis = std::sqrt(((rty_states[0] * rty_states[0] + rty_states[1] *
    rty_states[1]) + rty_states[2] * rty_states[2]) + rty_states[3] *
                        rty_states[3]);

  // Product: '<S57>/Product'
  rtb_XAxis1 = rty_states[0] / rtb_XAxis;

  // Product: '<S57>/Product1'
  rtb_Product1_b = rty_states[1] / rtb_XAxis;

  // Product: '<S57>/Product2'
  rtb_Product2_c = rty_states[2] / rtb_XAxis;

  // Product: '<S57>/Product3'
  rtb_XAxis = rty_states[3] / rtb_XAxis;

  // Gain: '<S49>/Gain' incorporates:
  //   Product: '<S49>/Product1'
  //   Product: '<S49>/Product2'
  //   Sum: '<S49>/Sum'

  rtb_VectorConcatenate_m[6] = (rtb_Product1_b * rtb_XAxis - rtb_XAxis1 *
    rtb_Product2_c) * 2.0F;

  // If: '<S18>/If' incorporates:
  //   Concatenate: '<S56>/Vector Concatenate'
  //   Gain: '<S22>/Gain1'
  //   Selector: '<S22>/Selector1'

  if (static_cast<boolean_T>((-rtb_VectorConcatenate_m[6] >= 1.0F) |
       (-rtb_VectorConcatenate_m[6] <= -1.0F))) {
    // Outputs for IfAction SubSystem: '<S18>/AxisRotZeroR3' incorporates:
    //   ActionPort: '<S21>/Action Port'

    // Fcn: '<S21>/Fcn1' incorporates:
    //   Gain: '<S22>/Gain3'
    //   Gain: '<S50>/Gain'
    //   Product: '<S50>/Product2'
    //   Product: '<S50>/Product3'
    //   Product: '<S51>/Product'
    //   Product: '<S51>/Product1'
    //   Product: '<S51>/Product2'
    //   Product: '<S51>/Product3'
    //   Sum: '<S50>/Sum'
    //   Sum: '<S51>/Sum'

    rtb_Merge_idx_0 = std::atan2(-((rtb_Product1_b * rtb_Product2_c - rtb_XAxis *
      rtb_XAxis1) * 2.0F), ((rtb_XAxis1 * rtb_XAxis1 - rtb_Product1_b *
      rtb_Product1_b) + rtb_Product2_c * rtb_Product2_c) - rtb_XAxis * rtb_XAxis);

    // If: '<S28>/If' incorporates:
    //   Constant: '<S29>/Constant'
    //   Constant: '<S30>/Constant'
    //   Fcn: '<S21>/Fcn2'

    if (-rtb_VectorConcatenate_m[6] > 1.0F) {
      // Outputs for IfAction SubSystem: '<S28>/If Action Subsystem' incorporates:
      //   ActionPort: '<S29>/Action Port'

      rtb_XAxis1 = 1.0F;

      // End of Outputs for SubSystem: '<S28>/If Action Subsystem'
    } else if (-rtb_VectorConcatenate_m[6] < -1.0F) {
      // Outputs for IfAction SubSystem: '<S28>/If Action Subsystem1' incorporates:
      //   ActionPort: '<S30>/Action Port'

      rtb_XAxis1 = 1.0F;

      // End of Outputs for SubSystem: '<S28>/If Action Subsystem1'
    } else {
      rtb_XAxis1 = -rtb_VectorConcatenate_m[6];
    }

    rtb_Merge_idx_1 = std::asin(rtb_XAxis1);

    // End of If: '<S28>/If'

    // Fcn: '<S21>/Fcn3'
    rtb_Product1_b = 0.0F;

    // End of Outputs for SubSystem: '<S18>/AxisRotZeroR3'
  } else {
    // Product: '<S47>/Product3' incorporates:
    //   Product: '<S55>/Product3'

    rtb_Merge_idx_0_tmp = rtb_XAxis1 * rtb_XAxis1;

    // Product: '<S47>/Product2' incorporates:
    //   Product: '<S55>/Product2'

    rtb_Merge_idx_0_tmp_0 = rtb_Product1_b * rtb_Product1_b;

    // Product: '<S47>/Product1' incorporates:
    //   Product: '<S55>/Product1'

    rtb_Merge_idx_0_tmp_1 = rtb_Product2_c * rtb_Product2_c;

    // Product: '<S47>/Product' incorporates:
    //   Product: '<S55>/Product'

    rtb_Merge_idx_0_tmp_2 = rtb_XAxis * rtb_XAxis;

    // Outputs for IfAction SubSystem: '<S18>/AxisRotDefault' incorporates:
    //   ActionPort: '<S20>/Action Port'

    // Fcn: '<S20>/Fcn1' incorporates:
    //   Gain: '<S48>/Gain'
    //   Product: '<S47>/Product'
    //   Product: '<S47>/Product1'
    //   Product: '<S47>/Product2'
    //   Product: '<S47>/Product3'
    //   Product: '<S48>/Product2'
    //   Product: '<S48>/Product3'
    //   Sum: '<S47>/Sum'
    //   Sum: '<S48>/Sum'

    rtb_Merge_idx_0 = std::atan2((rtb_XAxis * rtb_XAxis1 + rtb_Product1_b *
      rtb_Product2_c) * 2.0F, ((rtb_Merge_idx_0_tmp + rtb_Merge_idx_0_tmp_0) -
      rtb_Merge_idx_0_tmp_1) - rtb_Merge_idx_0_tmp_2);

    // Fcn: '<S20>/Fcn2'
    rtb_Merge_idx_1 = std::asin(-rtb_VectorConcatenate_m[6]);

    // Fcn: '<S20>/Fcn3' incorporates:
    //   Gain: '<S52>/Gain'
    //   Product: '<S52>/Product1'
    //   Product: '<S52>/Product2'
    //   Sum: '<S52>/Sum'
    //   Sum: '<S55>/Sum'

    rtb_Product1_b = std::atan2((rtb_XAxis1 * rtb_Product1_b + rtb_Product2_c *
      rtb_XAxis) * 2.0F, ((rtb_Merge_idx_0_tmp - rtb_Merge_idx_0_tmp_0) -
                          rtb_Merge_idx_0_tmp_1) + rtb_Merge_idx_0_tmp_2);

    // End of Outputs for SubSystem: '<S18>/AxisRotDefault'
  }

  // End of If: '<S18>/If'

  // SignalConversion generated from: '<Root>/eulAng_rad'
  rty_eulAng_rad[0] = rtb_Product1_b;
  rty_eulAng_rad[1] = rtb_Merge_idx_1;
  rty_eulAng_rad[2] = rtb_Merge_idx_0;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[0] = Product[0] - rty_states[14];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[0] = rtb_XAxis2;
  rty_dcmNedToBody[1] = rtb_UnitDelay_g;
  rty_dcmNedToBody[2] = rtb_Sum2_idx_1;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[1] = Product[1] - rty_states[15];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[3] = rtb_Sum2_idx_2;
  rty_dcmNedToBody[4] = q2q3;
  rty_dcmNedToBody[5] = rtb_dcmBodyToNed_idx_7;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[2] = Product[2] - rty_states[16];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[6] = rtb_dcmBodyToNed_idx_2;
  rty_dcmNedToBody[7] = rtb_dcmBodyToNed_idx_5;
  rty_dcmNedToBody[8] = rtb_dcmBodyToNed_idx_8;

  // MATLAB Function: '<Root>/eulToDcm' incorporates:
  //   Gain: '<Root>/ZeroOutRollAndPitch'

  //  precalculate trignometric values
  // MATLAB Function 'eulToDcm': '<S6>:1'
  // '<S6>:1:4' s_phi = sin(eul_rad(1));
  // '<S6>:1:5' c_phi = cos(eul_rad(1));
  // '<S6>:1:7' s_theta = sin(eul_rad(2));
  // '<S6>:1:8' c_theta = cos(eul_rad(2));
  // '<S6>:1:10' s_psi = sin(eul_rad(3));
  rtb_XAxis = std::sin(rtb_Merge_idx_0);

  // '<S6>:1:11' c_psi = cos(eul_rad(3));
  rtb_Product2_c = std::cos(rtb_Merge_idx_0);

  // '<S6>:1:13' dcmFromNed = [c_psi*c_theta, c_theta*s_psi, -s_theta;
  // '<S6>:1:14'     c_psi*s_phi*s_theta - c_phi*s_psi, c_phi*c_psi + s_phi*s_psi*s_theta, c_theta*s_phi; 
  // '<S6>:1:15'     s_phi*s_psi + c_phi*c_psi*s_theta, c_phi*s_psi*s_theta - c_psi*s_phi, c_phi*c_theta]; 
  rty_dcmNedToFep[0] = rtb_Product2_c;
  rty_dcmNedToFep[3] = rtb_XAxis;
  rty_dcmNedToFep[6] = -0.0F;
  rty_dcmNedToFep[1] = 0.0F - rtb_XAxis;
  rty_dcmNedToFep[4] = rtb_Product2_c;
  rty_dcmNedToFep[7] = 0.0F;
  rty_dcmNedToFep[2] = 0.0F;
  rty_dcmNedToFep[5] = 0.0F;
  rty_dcmNedToFep[8] = 1.0F;

  // Update for DiscreteTransferFcn: '<S61>/X Axis'
  stateEstimatorEskf_DW.XAxis_states[1] = stateEstimatorEskf_DW.XAxis_states[0];
  stateEstimatorEskf_DW.XAxis_states[0] = stateEstimatorEskf_DW.XAxis_tmp;

  // Update for DiscreteTransferFcn: '<S61>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states[1] = stateEstimatorEskf_DW.XAxis1_states[0];
  stateEstimatorEskf_DW.XAxis1_states[0] = stateEstimatorEskf_DW.XAxis1_tmp;

  // Update for DiscreteTransferFcn: '<S61>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states[1] = stateEstimatorEskf_DW.XAxis2_states[0];
  stateEstimatorEskf_DW.XAxis2_states[0] = stateEstimatorEskf_DW.XAxis2_tmp;

  // Update for DiscreteTransferFcn: '<S62>/X Axis'
  stateEstimatorEskf_DW.XAxis_states_e[1] =
    stateEstimatorEskf_DW.XAxis_states_e[0];
  stateEstimatorEskf_DW.XAxis_states_e[0] = stateEstimatorEskf_DW.XAxis_tmp_o;

  // Update for DiscreteTransferFcn: '<S62>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states_a[1] =
    stateEstimatorEskf_DW.XAxis1_states_a[0];
  stateEstimatorEskf_DW.XAxis1_states_a[0] = stateEstimatorEskf_DW.XAxis1_tmp_l;

  // Update for DiscreteTransferFcn: '<S62>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states_j[1] =
    stateEstimatorEskf_DW.XAxis2_states_j[0];
  stateEstimatorEskf_DW.XAxis2_states_j[0] = stateEstimatorEskf_DW.XAxis2_tmp_o;

  // Update for UnitDelay: '<Root>/Unit Delay1'
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[0] = rtb_Product1_b;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[1] = rtb_Merge_idx_1;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[2] = rtb_Merge_idx_0;

  // Switch: '<S7>/Switch' incorporates:
  //   RelationalOperator: '<S64>/FixPt Relational Operator'
  //   UnitDelay: '<S64>/Delay Input1'
  //
  //  Block description for '<S64>/Delay Input1':
  //
  //   Store in Global RAM

  if (static_cast<int32_T>(rtb_Compare) > static_cast<int32_T>
      (stateEstimatorEskf_DW.DelayInput1_DSTATE)) {
    // Update for UnitDelay: '<S7>/Unit Delay' incorporates:
    //   UnitDelay: '<Root>/Unit Delay'

    stateEstimatorEskf_DW.UnitDelay_DSTATE =
      stateEstimatorEskf_DW.UnitDelay_DSTATE_e[6];
  }

  // End of Switch: '<S7>/Switch'

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] = rtb_XAxis2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] = rtb_Sum2_idx_2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] = rtb_UnitDelay_g;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] = q2q3;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[6] = rtb_Sum2_idx_1;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for DiscreteTransferFcn: '<S71>/Discrete Transfer Fcn'
  stateEstimatorEskf_DW.DiscreteTransferFcn_states =
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp;

  // Update for DiscreteTransferFcn: '<S70>/Discrete Transfer Fcn'
  stateEstimatorEskf_DW.DiscreteTransferFcn_states_n =
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b;

  // Update for Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = false;

  // Update for UnitDelay: '<Root>/Unit Delay'
  std::memcpy(&stateEstimatorEskf_DW.UnitDelay_DSTATE_e[0], &rty_states[0], 20U *
              sizeof(real32_T));

  // Update for Delay: '<S1>/Delay' incorporates:
  //   UnitDelay: '<Root>/Unit Delay'

  std::memcpy(&stateEstimatorEskf_DW.Delay_DSTATE[0], &rtb_states[0], 20U *
              sizeof(real32_T));

  // Update for Delay: '<S1>/Delay2'
  stateEstimatorEskf_DW.icLoad_g = false;
  stateEstimatorEskf_DW.Delay2_DSTATE[0] = rtb_XAxis2;
  stateEstimatorEskf_DW.Delay2_DSTATE[1] = rtb_Sum2_idx_2;
  stateEstimatorEskf_DW.Delay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;
  stateEstimatorEskf_DW.Delay2_DSTATE[3] = rtb_UnitDelay_g;
  stateEstimatorEskf_DW.Delay2_DSTATE[4] = q2q3;
  stateEstimatorEskf_DW.Delay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;
  stateEstimatorEskf_DW.Delay2_DSTATE[6] = rtb_Sum2_idx_1;
  stateEstimatorEskf_DW.Delay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;
  stateEstimatorEskf_DW.Delay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for UnitDelay: '<S64>/Delay Input1'
  //
  //  Block description for '<S64>/Delay Input1':
  //
  //   Store in Global RAM

  stateEstimatorEskf_DW.DelayInput1_DSTATE = rtb_Compare;
}

// Constructor
stateEstimatorEskf::stateEstimatorEskf():
  stateEstimatorEskf_DW()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
stateEstimatorEskf::~stateEstimatorEskf() = default;

//
// File trailer for generated code.
//
// [EOF]
//
