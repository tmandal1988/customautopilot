//
// File: stateEstimatorEskf.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.84
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:15 2026
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
#include "applyGpsPosAndVelCorr_KKxnC7ep.h"
#include "norm_94qjDDKI.h"
#include "computEskfMagMeasJac_ecmRY7bq.h"
#include "computeEskfStateJac_tpF0ZWTA.h"
#include "updateEskfCovP_Qn4XLGNE.h"
#include "quatMultiply_UkcBdhzN.h"
#include "updateQuatAndResetCovP_o483nOwE.h"
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
  *mode, real_T latLonAltOut[3], const real32_T VectorConcatenate[2], const
  real32_T *Divide1, const real32_T Product[3], const real32_T Divide[3], const
  busMagData *rtu_magData, const busGpsData *rtu_gpsData, const busBaroData
  *rtu_baroData, const busStateEstSmParams *rtu_stateEstSmParams)
{
  *mode = enumStateEstimateMode::INITIALIZE;
  stateEstimatorEskf_DW.resetStates = true;

  // During 'INITIALIZE': '<S7>:1'
  // '<S7>:44:1' sf_internal_predicateOutput = 0 | (isAttInitialized && isPosInitialized ... 
  // '<S7>:44:2'  && isBaroInitialized);
  if (static_cast<boolean_T>(static_cast<boolean_T>
       (stateEstimatorEskf_DW.isAttInitialized &
        stateEstimatorEskf_DW.isPosInitialized) &
       stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S7>:44'
    stateEstimatorEskf_DW.durationCounter_1 = 0U;
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf = stateEstimatorEskf_IN_RUN;

    // Entry 'RUN': '<S7>:43'
    // FULL EKF WITH GPS IS RUNNING
    // '<S7>:43:4' resetStates = false;
    stateEstimatorEskf_DW.resetStates = false;

    // '<S7>:43:5' mode = enumStateEstimateMode.RUN;
    *mode = enumStateEstimateMode::RUN;

    // Chart: '<Root>/estimatorStateMachine'
    // '<S7>:43:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S7>:43:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S7>:43:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S7>:43:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // Product: '<S6>/Product'
    // '<S7>:43:10' latLonAltOut = latLonAltIn;
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];

    // SignalConversion generated from: '<S7>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];

    // Product: '<S11>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[0] = rtu_gpsData->latLonAlt[0];

    // Product: '<S6>/Product'
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];

    // SignalConversion generated from: '<S7>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];

    // Product: '<S11>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[1] = rtu_gpsData->latLonAlt[1];

    // Product: '<S6>/Product'
    stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];

    // SignalConversion generated from: '<S7>/ SFunction '
    stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
      stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];

    // Product: '<S11>/Divide'
    stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];

    // Chart: '<Root>/estimatorStateMachine'
    latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

    // '<S7>:43:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S7>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // Chart: '<Root>/estimatorStateMachine'
    // '<S7>:43:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //

    // Chart: '<Root>/estimatorStateMachine' incorporates:
    //   Concatenate: '<Root>/Vector Concatenate'
    //   Product: '<S11>/Divide'
    //   Product: '<S6>/Product'
    //   SignalConversion generated from: '<S7>/ SFunction '

    // '<S7>:63:1' sf_internal_predicateOutput = 0 | (isAttInitialized && ~isGpsInitialized &&  isBaroInitialized); 
  } else if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<boolean_T>
               (static_cast<int32_T>(rtu_gpsData->isGpsInitialized) ^ 1) &
               stateEstimatorEskf_DW.isAttInitialized) &
              stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S7>:63'
    stateEstimatorEskf_DW.durationCounter_1_n = 0U;
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
      stateEstima_IN_RUN_GPS_NOT_INIT;

    // Entry 'RUN_GPS_NOT_INIT': '<S7>:62'
    // EKF STARTED RUNNING WITHOUT GPS
    // '<S7>:62:4' resetStates = false;
    stateEstimatorEskf_DW.resetStates = false;

    // '<S7>:62:5' isPosInitialized = false;
    stateEstimatorEskf_DW.isPosInitialized = false;

    // '<S7>:62:6' gpsValidCount = 0;
    stateEstimatorEskf_DW.gpsValidCount = 0U;

    // '<S7>:62:7' mode = enumStateEstimateMode.RUN_GPS_NOT_INIT;
    *mode = enumStateEstimateMode::RUN_GPS_NOT_INIT;

    // '<S7>:62:8' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S7>:62:9' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S7>:62:10' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S7>:62:11' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S7>:62:12' latLonAltOut = latLonAltIn;
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

    // '<S7>:62:13' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S7>:62:14' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // '<S7>:62:15' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
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
    // '<S7>:1:48' if ( imuIdx < max(stateEstSmParams.imuInitCount, 1) )
    if (stateEstimatorEskf_DW.imuIdx < std::fmax
        (rtu_stateEstSmParams->imuInitCount, 1.0F)) {
      // '<S7>:1:49' imuIdx = imuIdx + 1;
      stateEstimatorEskf_DW.imuIdx++;

      // '<S7>:1:50' imuDelta = [filtBodyAccelsIn_mps2; filtBodyRatesIn_radps] -  ... 
      // '<S7>:1:51'         imuMean;
      imuDelta_idx_0 = Product[0] - stateEstimatorEskf_DW.imuMean[0];
      imuDelta_idx_3 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0]
        - stateEstimatorEskf_DW.imuMean[3];
      imuDelta_idx_1 = Product[1] - stateEstimatorEskf_DW.imuMean[1];
      imuDelta_idx_4 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1]
        - stateEstimatorEskf_DW.imuMean[4];
      imuDelta_idx_2 = Product[2] - stateEstimatorEskf_DW.imuMean[2];
      imuDelta_idx_5 = stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2]
        - stateEstimatorEskf_DW.imuMean[5];

      // '<S7>:1:52' imuMean = imuMean + imuDelta / imuIdx;
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

      // '<S7>:1:53' imuM2 = imuM2 + imuDelta .* ( [filtBodyAccelsIn_mps2; filtBodyRatesIn_radps] - ... 
      // '<S7>:1:54'         imuMean);
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
    // '<S7>:1:69' if (isMagDataValid)
    if (rtu_magData->isMagDataValid) {
      // '<S7>:1:70' if( magIdx < max(stateEstSmParams.magInitCount, 1) )
      if (stateEstimatorEskf_DW.magIdx < std::fmax
          (rtu_stateEstSmParams->magInitCount, 1.0F)) {
        // '<S7>:1:71' magIdx = magIdx + 1;
        stateEstimatorEskf_DW.magIdx++;

        // '<S7>:1:72' magDelta = (normMagVecIn_nd - magMean);
        // '<S7>:1:73' magMean = magMean + magDelta / magIdx;
        // '<S7>:1:74' magM2 = magM2 + magDelta .* (normMagVecIn_nd - magMean);
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
      // '<S7>:1:77' if(magIdx >= stateEstSmParams.magInitCount)
      if (stateEstimatorEskf_DW.magIdx >= rtu_stateEstSmParams->magInitCount) {
        real32_T initialQuat_tmp;
        real32_T initialQuat_tmp_0;

        // if(stateEstSmParams.magInitCount > 1)
        // compute mag bias
        // magBias_nd = sqrt( magM2 / (stateEstSmParams.magInitCount - 1) );
        // else
        // magBias_nd = [0; 0; 0];
        //  end
        // '<S7>:1:84' [initialQuat, nedMagVecNorm_nd] = computeInitialAttitude(imuMean(1:3), magMean, stateEstSmParams.initMagDec_rad); 
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

        // '<S7>:1:85' isAttInitialized = true;
        stateEstimatorEskf_DW.isAttInitialized = true;
      }
    }

    //
    // Compute running mean of GPS data for NED origin Lat, Lon and Alt
    // '<S7>:1:90' if (isGpsDataValid && isGpsInitialized && gpsAcc_m(1) >= 0 && ... 
    // '<S7>:1:91'         gpsAcc_m(1) <= stateEstSmParams.gpsHorAccThres_m && ... 
    // '<S7>:1:92'         gpsAcc_m(2) >= 0 && ...
    // '<S7>:1:93'         gpsAcc_m(2) <= stateEstSmParams.gpsVerAccThres_m)
    if ((static_cast<boolean_T>(rtu_gpsData->isGpsDataValid &
          rtu_gpsData->isGpsInitialized)) && (VectorConcatenate[0] >= 0.0F) &&
        (VectorConcatenate[0] <= rtu_stateEstSmParams->gpsHorAccThres_m) &&
        (VectorConcatenate[1] >= 0.0F) && (VectorConcatenate[1] <=
         rtu_stateEstSmParams->gpsVerAccThres_m)) {
      // '<S7>:1:94' if( gpsIdx < max(stateEstSmParams.gpsInitCount, 1) )
      if (stateEstimatorEskf_DW.gpsIdx < std::fmax
          (rtu_stateEstSmParams->gpsInitCount, 1.0F)) {
        // '<S7>:1:95' gpsIdx = gpsIdx + 1;
        stateEstimatorEskf_DW.gpsIdx++;

        // '<S7>:1:96' llhDelta = (latLonAltIn - refLatLonAlt);
        // '<S7>:1:97' refLatLonAlt = refLatLonAlt + llhDelta/gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[0] += (rtu_gpsData->latLonAlt[0] -
          stateEstimatorEskf_DW.refLatLonAlt[0]) / stateEstimatorEskf_DW.gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[1] += (rtu_gpsData->latLonAlt[1] -
          stateEstimatorEskf_DW.refLatLonAlt[1]) / stateEstimatorEskf_DW.gpsIdx;
        stateEstimatorEskf_DW.refLatLonAlt[2] += (rtu_gpsData->latLonAlt[2] -
          stateEstimatorEskf_DW.refLatLonAlt[2]) / stateEstimatorEskf_DW.gpsIdx;
      }

      //
      // '<S7>:1:100' if(gpsIdx >= stateEstSmParams.gpsInitCount)
      if (stateEstimatorEskf_DW.gpsIdx >= rtu_stateEstSmParams->gpsInitCount) {
        // '<S7>:1:101' isPosInitialized = true;
        stateEstimatorEskf_DW.isPosInitialized = true;
      }
    }

    //
    // Compute running mean and bias of baro data
    // '<S7>:1:106' if (isBaroDataValid)
    if (rtu_baroData->isBaroDataValid) {
      // '<S7>:1:107' if( baroIdx < max(stateEstSmParams.baroInitCount, 1) )
      if (stateEstimatorEskf_DW.baroIdx < std::fmax
          (rtu_stateEstSmParams->baroInitCount, 1.0F)) {
        // '<S7>:1:108' baroIdx = baroIdx + 1;
        stateEstimatorEskf_DW.baroIdx++;

        // '<S7>:1:109' baroInitAltDelta = (baroPressAlt_m - baroInitAltMean);
        imuDelta_idx_3 = *Divide1 - stateEstimatorEskf_DW.baroInitAltMean;

        // '<S7>:1:110' baroInitAltMean = baroInitAltMean + baroInitAltDelta / baroIdx; 
        stateEstimatorEskf_DW.baroInitAltMean += imuDelta_idx_3 /
          stateEstimatorEskf_DW.baroIdx;

        // '<S7>:1:111' baroInitAltM2 = baroInitAltM2 + baroInitAltDelta .* (baroPressAlt_m - baroInitAltMean); 
        stateEstimatorEskf_DW.baroInitAltM2 += (*Divide1 -
          stateEstimatorEskf_DW.baroInitAltMean) * imuDelta_idx_3;
      }

      //
      // '<S7>:1:114' if(baroIdx >= stateEstSmParams.baroInitCount)
      if (stateEstimatorEskf_DW.baroIdx >= rtu_stateEstSmParams->baroInitCount)
      {
        // if(stateEstSmParams.baroInitCount > 1)
        // compute baro bias
        // baroBias_m = sqrt( baroInitAltM2 / (stateEstSmParams.baroInitCount - 1) ); 
        // else
        // baroBias_m = 0;
        //  end
        // '<S7>:1:121' isBaroInitialized = true;
        stateEstimatorEskf_DW.isBaroInitialized = true;
      }
    }

    // '<S7>:1:124' if(isGpsInitialized)
    if (rtu_gpsData->isGpsInitialized) {
      // Status of the initialization
      // '<S7>:1:126' stateEstInitPct = (imuIdx + magIdx + gpsIdx + baroIdx) / (stateEstSmParams.imuInitCount + ... 
      // '<S7>:1:127'         stateEstSmParams.magInitCount + stateEstSmParams.gpsInitCount +  ... 
      // '<S7>:1:128'         stateEstSmParams.baroInitCount) * 100;
      stateEstimatorEskf_DW.stateEstInitPct = (((stateEstimatorEskf_DW.imuIdx +
        stateEstimatorEskf_DW.magIdx) + static_cast<real32_T>
        (stateEstimatorEskf_DW.gpsIdx)) + stateEstimatorEskf_DW.baroIdx) /
        (((rtu_stateEstSmParams->imuInitCount +
           rtu_stateEstSmParams->magInitCount) +
          rtu_stateEstSmParams->gpsInitCount) +
         rtu_stateEstSmParams->baroInitCount) * 100.0F;
    } else {
      // '<S7>:1:129' else
      // Status of the initialization
      // '<S7>:1:131' stateEstInitPct = (imuIdx + magIdx + baroIdx) / (stateEstSmParams.imuInitCount + ... 
      // '<S7>:1:132'         stateEstSmParams.magInitCount + stateEstSmParams.baroInitCount) * 100; 
      stateEstimatorEskf_DW.stateEstInitPct = ((stateEstimatorEskf_DW.imuIdx +
        stateEstimatorEskf_DW.magIdx) + stateEstimatorEskf_DW.baroIdx) /
        ((rtu_stateEstSmParams->imuInitCount +
          rtu_stateEstSmParams->magInitCount) +
         rtu_stateEstSmParams->baroInitCount) * 100.0F;
    }

    //
    // '<S7>:1:136' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S7>:1:137' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S7>:1:138' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S7>:1:139' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S7>:1:140' latLonAltOut = latLonAltIn;
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

    // '<S7>:1:141' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S7>:1:142' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S7>:1:143' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //
    // '<S7>:1:146' initialStates = [initialQuat; 0; 0; 0; 0; 0; 0; gyroBias_radps; accelBias_mps2; magBias_nd; baroBias_m]; 
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
    // '<S7>:1:148' initialDcmBodyToNed = quatToDcm_function(initialStates(1:4)); 
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
  // Entry 'RUN_INIT_GPS': '<S7>:64'
  // EKF RUNNING WITHOUT GPS BUT WE HAVE HEALTHY GPS SIGNAL
  // START INITIALIZING GPS
  // '<S7>:64:5' gpsValidCount = 0;
  stateEstimatorEskf_DW.gpsValidCount = 0U;

  //  Index to keep track of how many gps readings we have summed
  //  so far
  // '<S7>:64:8' gpsIdx = 0;
  stateEstimatorEskf_DW.gpsIdx = 0.0;

  // '<S7>:64:9' refLatLonAlt = [0; 0; 0];
  // '<S7>:64:10' isPosInitialized = false;
  stateEstimatorEskf_DW.isPosInitialized = false;

  // '<S7>:64:11' mode = enumStateEstimateMode.RUN_INIT_GPS;
  *mode = enumStateEstimateMode::RUN_INIT_GPS;

  // Chart: '<Root>/estimatorStateMachine'
  // '<S7>:64:12' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
  // '<S7>:64:13' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
  // '<S7>:64:14' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
  // '<S7>:64:15' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
    rtu_magData->isMagDataValid;

  // '<S7>:64:16' latLonAltOut = latLonAltIn;
  stateEstimatorEskf_DW.refLatLonAlt[0] = 0.0;

  // Product: '<S6>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0] = Product[0];

  // SignalConversion generated from: '<S7>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0];

  // Product: '<S11>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0] = Divide[0];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[0] = rtu_gpsData->latLonAlt[0];
  stateEstimatorEskf_DW.refLatLonAlt[1] = 0.0;

  // Product: '<S6>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1] = Product[1];

  // SignalConversion generated from: '<S7>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1];

  // Product: '<S11>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1] = Divide[1];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[1] = rtu_gpsData->latLonAlt[1];
  stateEstimatorEskf_DW.refLatLonAlt[2] = 0.0;

  // Product: '<S6>/Product'
  stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2] = Product[2];

  // SignalConversion generated from: '<S7>/ SFunction '
  stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] =
    stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2];

  // Product: '<S11>/Divide'
  stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2] = Divide[2];

  // Chart: '<Root>/estimatorStateMachine'
  latLonAltOut[2] = rtu_gpsData->latLonAlt[2];

  // '<S7>:64:17' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
    rtu_gpsData->isGpsDataValid;

  // '<S7>:64:18' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
  stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
    stateEstimatorEskf_DW.baroInitAltMean;

  // Chart: '<Root>/estimatorStateMachine'
  // '<S7>:64:19' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
  stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
    rtu_baroData->isBaroDataValid;

  //
}

//
// Function for MATLAB Function: '<S15>/DelayedHorizonBufferManager'
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
  boolean_T *magReady, uint64m_T *magTimeOut_ms, real32_T magOut_uT[3],
  enumDhFifoStatus *status, uint16_T *countOut)
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
  //  Static FIFO storage/state initialization.
  // 'magFifo:56' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. magReady still indicates whether this call returned a new sample.
  // 'magFifo:72' magReady = false;
  *magReady = false;

  // 'magFifo:73' magTimeOut_ms = lastMagTimeOut_ms;
  *magTimeOut_ms = stateEstimatorEskf_DW.lastMagTimeOut_ms;

  // 'magFifo:74' magOut_uT = lastMagOut_uT;
  magOut_uT[0] = stateEstimatorEskf_DW.lastMagOut_uT[0];
  magOut_uT[1] = stateEstimatorEskf_DW.lastMagOut_uT[1];
  magOut_uT[2] = stateEstimatorEskf_DW.lastMagOut_uT[2];

  // 'magFifo:75' status = enumDhFifoStatus.OK;
  *status = enumDhFifoStatus::OK;

  // 'magFifo:76' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_d;

  //  Read-only configuration aliases.
  // 'magFifo:79' capacity = magFifoParams.capacity;
  // 'magFifo:80' maxAge_ms = magFifoParams.maxAge_ms;
  // 'magFifo:81' minObsInterval_ms = magFifoParams.minInterval_ms;
  // 'magFifo:82' timeResetThreshold_ms = magFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'magFifo:85' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((magFifoParams_capacity < 1) |
       (magFifoParams_capacity > 64))) {
    // 'magFifo:86' status = enumDhFifoStatus.BAD_CAPACITY;
    *status = enumDhFifoStatus::BAD_CAPACITY;

    //  Explicit estimator/FIFO reset.
    // 'magFifo:91' if reset
  } else if (reset) {
    // 'magFifo:92' head = uint16(1);
    stateEstimatorEskf_DW.head_b = 1U;

    // 'magFifo:93' tail = uint16(1);
    stateEstimatorEskf_DW.tail_k = 1U;

    // 'magFifo:94' count = uint16(0);
    stateEstimatorEskf_DW.count_d = 0U;

    // 'magFifo:95' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms = tmp_5;

    // 'magFifo:96' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime = false;

    // 'magFifo:98' lastMagTimeOut_ms = uint64(0);
    stateEstimatorEskf_DW.lastMagTimeOut_ms = tmp_5;

    // 'magFifo:99' lastMagOut_uT = zeros(3,1,'single');
    // 'magFifo:101' magReady = false;
    // 'magFifo:102' magTimeOut_ms = uint64(0);
    *magTimeOut_ms = tmp_5;

    // 'magFifo:103' magOut_uT = zeros(3,1,'single');
    stateEstimatorEskf_DW.lastMagOut_uT[0] = 0.0F;
    magOut_uT[0] = 0.0F;
    stateEstimatorEskf_DW.lastMagOut_uT[1] = 0.0F;
    magOut_uT[1] = 0.0F;
    stateEstimatorEskf_DW.lastMagOut_uT[2] = 0.0F;
    magOut_uT[2] = 0.0F;

    // 'magFifo:105' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_i_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW MAGNETOMETER MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'magFifo:112' if isMagValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isMagValid) {
      //  Magnetometer timestamps must normally increase monotonically.
      // 'magFifo:114' if haveLastPushTime && magTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime &
           uMultiWordLe(&magTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms.chunks[0U], 2)))
      {
        // 'magFifo:115' backwardsJump_ms = lastPushTime_ms - magTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'magFifo:118' if timeResetThreshold_ms > 0 && ...
        // 'magFifo:119'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&magTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&magFifoParams_resetThreshold_ms.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &magFifoParams_resetThreshold_ms.chunks[0U], 2))) {
          // 'magFifo:120' head = uint16(1);
          stateEstimatorEskf_DW.head_b = 1U;

          // 'magFifo:121' tail = uint16(1);
          stateEstimatorEskf_DW.tail_k = 1U;

          // 'magFifo:122' count = uint16(0);
          stateEstimatorEskf_DW.count_d = 0U;

          // 'magFifo:123' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms = tmp_5;

          // 'magFifo:124' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime = false;

          // 'magFifo:125' status = enumDhFifoStatus.TIMEBASE_RESET;
          *status = enumDhFifoStatus::TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'magFifo:126' else
          //  Duplicate timestamp or small backward jump.
          // 'magFifo:128' status = enumDhFifoStatus.BAD_TIMESTAMP;
          *status = enumDhFifoStatus::BAD_TIMESTAMP;
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
      // 'magFifo:135' if haveLastPushTime && ...
      // 'magFifo:136'             (magTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
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
        // 'magFifo:141' bufferWasFull = (count == capacity);
        //  Store complete 3-axis magnetic-field observation.
        //  Each column contains [Bx; By; Bz] in uT.
        // 'magFifo:145' timeBuf_ms(head) = magTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.head_b - 1] =
          magTimeIn_ms;

        // 'magFifo:146' dataBuf(:,head) = magIn_uT;
        dataBuf_i_tmp = (stateEstimatorEskf_DW.head_b - 1) * 3;
        stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp] = magIn_uT[0];
        stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 1] = magIn_uT[1];
        stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 2] = magIn_uT[2];

        //  Advance write location.
        // 'magFifo:149' if head == capacity
        if (stateEstimatorEskf_DW.head_b == magFifoParams_capacity) {
          // 'magFifo:150' head = uint16(1);
          stateEstimatorEskf_DW.head_b = 1U;
        } else {
          // 'magFifo:151' else
          // 'magFifo:152' head = head + 1;
          qY = stateEstimatorEskf_DW.head_b + 1U;
          if (stateEstimatorEskf_DW.head_b + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_b = static_cast<uint16_T>(qY);
        }

        // 'magFifo:155' if bufferWasFull
        if (stateEstimatorEskf_DW.count_d == magFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'magFifo:158' if tail == capacity
          if (stateEstimatorEskf_DW.tail_k == magFifoParams_capacity) {
            // 'magFifo:159' tail = uint16(1);
            stateEstimatorEskf_DW.tail_k = 1U;
          } else {
            // 'magFifo:160' else
            // 'magFifo:161' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_k + 1U;
            if (stateEstimatorEskf_DW.tail_k + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_k = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'magFifo:165' count = capacity;
          stateEstimatorEskf_DW.count_d = magFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'magFifo:168' if status == enumDhFifoStatus.OK
          if (*status == enumDhFifoStatus::OK) {
            // 'magFifo:169' status = enumDhFifoStatus.OVERWRITE;
            *status = enumDhFifoStatus::OVERWRITE;
          }
        } else {
          // 'magFifo:171' else
          //  One new measurement was added without replacing anything.
          // 'magFifo:173' count = count + 1;
          qY = stateEstimatorEskf_DW.count_d + 1U;
          if (stateEstimatorEskf_DW.count_d + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_d = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'magFifo:177' lastPushTime_ms = magTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms = magTimeIn_ms;

        // 'magFifo:178' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime = true;

        // 'magFifo:179' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_d;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST MAGNETOMETER MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'magFifo:187' if count == 0
      if ((stateEstimatorEskf_DW.count_d == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_k - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'magFifo:193' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ magTime | magTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'magFifo:204' for k = 1:(MAX_SIZE-1)
        dataBuf_i_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_i_tmp < 63) &
                 (stateEstimatorEskf_DW.count_d != 1)))) {
          uint16_T nextIdx;

          // 'magFifo:205' if count == 1
          // 'magFifo:209' if tail == capacity
          if (stateEstimatorEskf_DW.tail_k == magFifoParams_capacity) {
            // 'magFifo:210' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'magFifo:211' else
            // 'magFifo:212' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_k + 1U;
            if (stateEstimatorEskf_DW.tail_k + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'magFifo:215' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_k[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'magFifo:220' tail = nextIdx;
            stateEstimatorEskf_DW.tail_k = nextIdx;

            // 'magFifo:221' count = count - 1;
            qY = stateEstimatorEskf_DW.count_d -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_d - 1U >
                stateEstimatorEskf_DW.count_d) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_d = static_cast<uint16_T>(qY);
            dataBuf_i_tmp++;
          }
        }

        //  Tail now points to the newest eligible magnetometer measurement.
        // 'magFifo:225' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'magFifo:229' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&magFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_k -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &magFifoParams_maxAge_ms.chunks[0U],
                           2)) {
            // 'magFifo:230' if tail == capacity
            if (stateEstimatorEskf_DW.tail_k == magFifoParams_capacity) {
              // 'magFifo:231' tail = uint16(1);
              stateEstimatorEskf_DW.tail_k = 1U;
            } else {
              // 'magFifo:232' else
              // 'magFifo:233' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_k + 1U;
              if (stateEstimatorEskf_DW.tail_k + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_k = static_cast<uint16_T>(qY);
            }

            // 'magFifo:236' count = count - 1;
            qY = stateEstimatorEskf_DW.count_d -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_d - 1U >
                stateEstimatorEskf_DW.count_d) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_d = static_cast<uint16_T>(qY);

            // 'magFifo:237' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_d;

            // 'magFifo:238' status = enumDhFifoStatus.STALE_DISCARD;
            *status = enumDhFifoStatus::STALE_DISCARD;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      real32_T magOut_uT_tmp;

      //  Return selected magnetic-field measurement.
      // 'magFifo:243' magTimeOut_ms = timeBuf_ms(tail);
      *magTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_k[stateEstimatorEskf_DW.tail_k - 1];

      // 'magFifo:244' magOut_uT = dataBuf(:,tail);
      // 'magFifo:245' magReady = true;
      *magReady = true;

      //  Hold last returned measurement for future calls where magReady is false. 
      // 'magFifo:248' lastMagTimeOut_ms = magTimeOut_ms;
      stateEstimatorEskf_DW.lastMagTimeOut_ms = *magTimeOut_ms;

      // 'magFifo:249' lastMagOut_uT = magOut_uT;
      dataBuf_i_tmp = (stateEstimatorEskf_DW.tail_k - 1) * 3;
      magOut_uT_tmp = stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp];
      magOut_uT[0] = magOut_uT_tmp;
      stateEstimatorEskf_DW.lastMagOut_uT[0] = magOut_uT_tmp;
      magOut_uT_tmp = stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 1];
      magOut_uT[1] = magOut_uT_tmp;
      stateEstimatorEskf_DW.lastMagOut_uT[1] = magOut_uT_tmp;
      magOut_uT_tmp = stateEstimatorEskf_DW.dataBuf_i[dataBuf_i_tmp + 2];
      magOut_uT[2] = magOut_uT_tmp;
      stateEstimatorEskf_DW.lastMagOut_uT[2] = magOut_uT_tmp;

      //  Consume returned measurement.
      // 'magFifo:252' if tail == capacity
      if (stateEstimatorEskf_DW.tail_k == magFifoParams_capacity) {
        // 'magFifo:253' tail = uint16(1);
        stateEstimatorEskf_DW.tail_k = 1U;
      } else {
        // 'magFifo:254' else
        // 'magFifo:255' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_k + 1U;
        if (stateEstimatorEskf_DW.tail_k + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_k = static_cast<uint16_T>(qY);
      }

      // 'magFifo:258' count = count - 1;
      qY = stateEstimatorEskf_DW.count_d -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_d - 1U > stateEstimatorEskf_DW.count_d) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_d = static_cast<uint16_T>(qY);

      // 'magFifo:259' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_d;
    }
  }
}

//
// Function for MATLAB Function: '<S15>/DelayedHorizonBufferManager'
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
  boolean_T *gpsReady, uint64m_T *gpsTimeOut_ms, real32_T posVelOut[6],
  enumDhFifoStatus *status, uint16_T *countOut)
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
  //  Static FIFO storage/state initialization.
  // 'gpsFifo:56' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. gpsReady still indicates whether this call returned a new sample.
  // 'gpsFifo:72' gpsReady = false;
  *gpsReady = false;

  // 'gpsFifo:73' gpsTimeOut_ms = lastGpsTimeOut_ms;
  *gpsTimeOut_ms = stateEstimatorEskf_DW.lastGpsTimeOut_ms;

  // 'gpsFifo:74' posVelOut = lastPosVelOut;
  posVelOut[0] = stateEstimatorEskf_DW.lastPosVelOut[0];
  posVelOut[1] = stateEstimatorEskf_DW.lastPosVelOut[1];
  posVelOut[2] = stateEstimatorEskf_DW.lastPosVelOut[2];
  posVelOut[3] = stateEstimatorEskf_DW.lastPosVelOut[3];
  posVelOut[4] = stateEstimatorEskf_DW.lastPosVelOut[4];
  posVelOut[5] = stateEstimatorEskf_DW.lastPosVelOut[5];

  // 'gpsFifo:75' status = enumDhFifoStatus.OK;
  *status = enumDhFifoStatus::OK;

  // 'gpsFifo:76' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_k;

  //  Read-only configuration aliases.
  // 'gpsFifo:79' capacity = gpsFifoParams.capacity;
  // 'gpsFifo:80' maxAge_ms = gpsFifoParams.maxAge_ms;
  // 'gpsFifo:81' minObsInterval_ms = gpsFifoParams.minInterval_ms;
  // 'gpsFifo:82' timeResetThreshold_ms = gpsFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'gpsFifo:85' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((gpsFifoParams_capacity < 1) |
       (gpsFifoParams_capacity > 16))) {
    // 'gpsFifo:86' status = enumDhFifoStatus.BAD_CAPACITY;
    *status = enumDhFifoStatus::BAD_CAPACITY;

    //  Explicit estimator/FIFO reset.
    // 'gpsFifo:91' if reset
  } else if (reset) {
    // 'gpsFifo:92' head = uint16(1);
    stateEstimatorEskf_DW.head_a = 1U;

    // 'gpsFifo:93' tail = uint16(1);
    stateEstimatorEskf_DW.tail_c = 1U;

    // 'gpsFifo:94' count = uint16(0);
    stateEstimatorEskf_DW.count_k = 0U;

    // 'gpsFifo:95' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_b = tmp_5;

    // 'gpsFifo:96' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_m = false;

    // 'gpsFifo:98' lastGpsTimeOut_ms = uint64(0);
    stateEstimatorEskf_DW.lastGpsTimeOut_ms = tmp_5;

    // 'gpsFifo:99' lastPosVelOut = zeros(6,1,'single');
    // 'gpsFifo:101' gpsReady = false;
    // 'gpsFifo:102' gpsTimeOut_ms = uint64(0);
    *gpsTimeOut_ms = tmp_5;

    // 'gpsFifo:103' posVelOut = zeros(6,1,'single');
    stateEstimatorEskf_DW.lastPosVelOut[0] = 0.0F;
    posVelOut[0] = 0.0F;
    stateEstimatorEskf_DW.lastPosVelOut[1] = 0.0F;
    posVelOut[1] = 0.0F;
    stateEstimatorEskf_DW.lastPosVelOut[2] = 0.0F;
    posVelOut[2] = 0.0F;
    stateEstimatorEskf_DW.lastPosVelOut[3] = 0.0F;
    posVelOut[3] = 0.0F;
    stateEstimatorEskf_DW.lastPosVelOut[4] = 0.0F;
    posVelOut[4] = 0.0F;
    stateEstimatorEskf_DW.lastPosVelOut[5] = 0.0F;
    posVelOut[5] = 0.0F;

    // 'gpsFifo:105' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_f_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW GPS MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'gpsFifo:112' if isGpsValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isGpsValid) {
      //  GPS timestamps must normally increase monotonically.
      // 'gpsFifo:114' if haveLastPushTime && gpsTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_m &
           uMultiWordLe(&gpsTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_b.chunks[0U], 2)))
      {
        // 'gpsFifo:115' backwardsJump_ms = lastPushTime_ms - gpsTimeIn_ms;
        //  Large backward jump indicates a GPS time-base discontinuity.
        // 'gpsFifo:118' if timeResetThreshold_ms > 0 && ...
        // 'gpsFifo:119'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_b.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&gpsTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&gpsFifoParams_resetThreshold_ms.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &gpsFifoParams_resetThreshold_ms.chunks[0U], 2))) {
          // 'gpsFifo:120' head = uint16(1);
          stateEstimatorEskf_DW.head_a = 1U;

          // 'gpsFifo:121' tail = uint16(1);
          stateEstimatorEskf_DW.tail_c = 1U;

          // 'gpsFifo:122' count = uint16(0);
          stateEstimatorEskf_DW.count_k = 0U;

          // 'gpsFifo:123' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_b = tmp_5;

          // 'gpsFifo:124' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_m = false;

          // 'gpsFifo:125' status = enumDhFifoStatus.TIMEBASE_RESET;
          *status = enumDhFifoStatus::TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'gpsFifo:126' else
          //  Duplicate timestamp or small backward jump.
          // 'gpsFifo:128' status = enumDhFifoStatus.BAD_TIMESTAMP;
          *status = enumDhFifoStatus::BAD_TIMESTAMP;
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
      // 'gpsFifo:135' if haveLastPushTime && ...
      // 'gpsFifo:136'             (gpsTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&gpsTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_b.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_m &
           uMultiWordLt(&tmp_3.chunks[0U], &gpsFifoParams_minInterval_ms.chunks
                        [0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'gpsFifo:141' bufferWasFull = (count == capacity);
        //  Store complete GPS observation at current write location.
        // 'gpsFifo:144' timeBuf_ms(head) = gpsTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.head_a - 1] =
          gpsTimeIn_ms;

        // 'gpsFifo:145' dataBuf(:,head) = posVelIn;
        dataBuf_f_tmp = (stateEstimatorEskf_DW.head_a - 1) * 6;
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp] = posVelIn[0];
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 1] = posVelIn[1];
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 2] = posVelIn[2];
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 3] = posVelIn[3];
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 4] = posVelIn[4];
        stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 5] = posVelIn[5];

        //  Advance write location.
        // 'gpsFifo:148' if head == capacity
        if (stateEstimatorEskf_DW.head_a == gpsFifoParams_capacity) {
          // 'gpsFifo:149' head = uint16(1);
          stateEstimatorEskf_DW.head_a = 1U;
        } else {
          // 'gpsFifo:150' else
          // 'gpsFifo:151' head = head + 1;
          qY = stateEstimatorEskf_DW.head_a + 1U;
          if (stateEstimatorEskf_DW.head_a + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_a = static_cast<uint16_T>(qY);
        }

        // 'gpsFifo:154' if bufferWasFull
        if (stateEstimatorEskf_DW.count_k == gpsFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'gpsFifo:157' if tail == capacity
          if (stateEstimatorEskf_DW.tail_c == gpsFifoParams_capacity) {
            // 'gpsFifo:158' tail = uint16(1);
            stateEstimatorEskf_DW.tail_c = 1U;
          } else {
            // 'gpsFifo:159' else
            // 'gpsFifo:160' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_c + 1U;
            if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'gpsFifo:164' count = capacity;
          stateEstimatorEskf_DW.count_k = gpsFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'gpsFifo:167' if status == enumDhFifoStatus.OK
          if (*status == enumDhFifoStatus::OK) {
            // 'gpsFifo:168' status = enumDhFifoStatus.OVERWRITE;
            *status = enumDhFifoStatus::OVERWRITE;
          }
        } else {
          // 'gpsFifo:170' else
          //  One new measurement was added without replacing anything.
          // 'gpsFifo:172' count = count + 1;
          qY = stateEstimatorEskf_DW.count_k + 1U;
          if (stateEstimatorEskf_DW.count_k + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'gpsFifo:176' lastPushTime_ms = gpsTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_b = gpsTimeIn_ms;

        // 'gpsFifo:177' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_m = true;

        // 'gpsFifo:178' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_k;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST GPS MEASUREMENT ELIGIBLE AT THE FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'gpsFifo:186' if count == 0
      if ((stateEstimatorEskf_DW.count_k == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_c - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Because measurements are chronologically ordered, if the oldest
        //  measurement is newer than the fusion horizon, nothing is ready.
        // 'gpsFifo:192' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ gpsTime | gpsTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'gpsFifo:203' for k = 1:(MAX_SIZE-1)
        dataBuf_f_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_f_tmp < 15) &
                 (stateEstimatorEskf_DW.count_k != 1)))) {
          uint16_T nextIdx;

          // 'gpsFifo:204' if count == 1
          // 'gpsFifo:208' if tail == capacity
          if (stateEstimatorEskf_DW.tail_c == gpsFifoParams_capacity) {
            // 'gpsFifo:209' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'gpsFifo:210' else
            // 'gpsFifo:211' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_c + 1U;
            if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'gpsFifo:214' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_c[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible sample exists; discard current tail.
            // 'gpsFifo:219' tail = nextIdx;
            stateEstimatorEskf_DW.tail_c = nextIdx;

            // 'gpsFifo:220' count = count - 1;
            qY = stateEstimatorEskf_DW.count_k -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_k - 1U >
                stateEstimatorEskf_DW.count_k) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);
            dataBuf_f_tmp++;
          }
        }

        //  Tail now points to the newest eligible GPS measurement.
        // 'gpsFifo:224' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'gpsFifo:228' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&gpsFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_c -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &gpsFifoParams_maxAge_ms.chunks[0U],
                           2)) {
            // 'gpsFifo:229' if tail == capacity
            if (stateEstimatorEskf_DW.tail_c == gpsFifoParams_capacity) {
              // 'gpsFifo:230' tail = uint16(1);
              stateEstimatorEskf_DW.tail_c = 1U;
            } else {
              // 'gpsFifo:231' else
              // 'gpsFifo:232' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_c + 1U;
              if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
            }

            // 'gpsFifo:235' count = count - 1;
            qY = stateEstimatorEskf_DW.count_k -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_k - 1U >
                stateEstimatorEskf_DW.count_k) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);

            // 'gpsFifo:236' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_k;

            // 'gpsFifo:237' status = enumDhFifoStatus.STALE_DISCARD;
            *status = enumDhFifoStatus::STALE_DISCARD;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      real32_T posVelOut_tmp;

      //  Return selected GPS measurement.
      // 'gpsFifo:242' gpsTimeOut_ms = timeBuf_ms(tail);
      *gpsTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_c[stateEstimatorEskf_DW.tail_c - 1];

      // 'gpsFifo:243' posVelOut = dataBuf(:,tail);
      // 'gpsFifo:244' gpsReady = true;
      *gpsReady = true;

      //  Hold last returned measurement for future calls where gpsReady is false. 
      // 'gpsFifo:247' lastGpsTimeOut_ms = gpsTimeOut_ms;
      stateEstimatorEskf_DW.lastGpsTimeOut_ms = *gpsTimeOut_ms;

      // 'gpsFifo:248' lastPosVelOut = posVelOut;
      dataBuf_f_tmp = (stateEstimatorEskf_DW.tail_c - 1) * 6;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp];
      posVelOut[0] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[0] = posVelOut_tmp;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 1];
      posVelOut[1] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[1] = posVelOut_tmp;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 2];
      posVelOut[2] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[2] = posVelOut_tmp;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 3];
      posVelOut[3] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[3] = posVelOut_tmp;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 4];
      posVelOut[4] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[4] = posVelOut_tmp;
      posVelOut_tmp = stateEstimatorEskf_DW.dataBuf_f[dataBuf_f_tmp + 5];
      posVelOut[5] = posVelOut_tmp;
      stateEstimatorEskf_DW.lastPosVelOut[5] = posVelOut_tmp;

      //  Consume returned measurement.
      // 'gpsFifo:251' if tail == capacity
      if (stateEstimatorEskf_DW.tail_c == gpsFifoParams_capacity) {
        // 'gpsFifo:252' tail = uint16(1);
        stateEstimatorEskf_DW.tail_c = 1U;
      } else {
        // 'gpsFifo:253' else
        // 'gpsFifo:254' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_c + 1U;
        if (stateEstimatorEskf_DW.tail_c + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_c = static_cast<uint16_T>(qY);
      }

      // 'gpsFifo:257' count = count - 1;
      qY = stateEstimatorEskf_DW.count_k -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_k - 1U > stateEstimatorEskf_DW.count_k) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_k = static_cast<uint16_T>(qY);

      // 'gpsFifo:258' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_k;
    }
  }
}

//
// Function for MATLAB Function: '<S15>/DelayedHorizonBufferManager'
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
  boolean_T *lidarReady, uint64m_T *lidarTimeOut_ms, real32_T *aglOut_m,
  enumDhFifoStatus *status, uint16_T *countOut)
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
  //  Static FIFO storage/state initialization.
  // 'lidarFifo:55' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. lidarReady still indicates whether this call returned a new sample. 
  // 'lidarFifo:71' lidarReady = false;
  *lidarReady = false;

  // 'lidarFifo:72' lidarTimeOut_ms = lastLidarTimeOut_ms;
  *lidarTimeOut_ms = stateEstimatorEskf_DW.lastLidarTimeOut_ms;

  // 'lidarFifo:73' aglOut_m = lastAglOut_m;
  *aglOut_m = stateEstimatorEskf_DW.lastAglOut_m;

  // 'lidarFifo:74' status = enumDhFifoStatus.OK;
  *status = enumDhFifoStatus::OK;

  // 'lidarFifo:75' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_kg;

  //  Read-only configuration aliases.
  // 'lidarFifo:78' capacity = lidarFifoParams.capacity;
  // 'lidarFifo:79' maxAge_ms = lidarFifoParams.maxAge_ms;
  // 'lidarFifo:80' minObsInterval_ms = lidarFifoParams.minInterval_ms;
  // 'lidarFifo:81' timeResetThreshold_ms = lidarFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'lidarFifo:84' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((lidarFifoParams_capacity < 1) |
       (lidarFifoParams_capacity > 32))) {
    // 'lidarFifo:85' status = enumDhFifoStatus.BAD_CAPACITY;
    *status = enumDhFifoStatus::BAD_CAPACITY;

    //  Explicit estimator/FIFO reset.
    // 'lidarFifo:90' if reset
  } else if (reset) {
    // 'lidarFifo:91' head = uint16(1);
    stateEstimatorEskf_DW.head_f2 = 1U;

    // 'lidarFifo:92' tail = uint16(1);
    stateEstimatorEskf_DW.tail_pr = 1U;

    // 'lidarFifo:93' count = uint16(0);
    stateEstimatorEskf_DW.count_kg = 0U;

    // 'lidarFifo:94' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_c = tmp_5;

    // 'lidarFifo:95' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_jj = false;

    // 'lidarFifo:97' lastLidarTimeOut_ms = uint64(0);
    stateEstimatorEskf_DW.lastLidarTimeOut_ms = tmp_5;

    // 'lidarFifo:98' lastAglOut_m = single(0);
    stateEstimatorEskf_DW.lastAglOut_m = 0.0F;

    // 'lidarFifo:100' lidarReady = false;
    // 'lidarFifo:101' lidarTimeOut_ms = uint64(0);
    *lidarTimeOut_ms = tmp_5;

    // 'lidarFifo:102' aglOut_m = single(0);
    *aglOut_m = 0.0F;

    // 'lidarFifo:104' countOut = count;
    *countOut = 0U;
  } else {
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW LIDAR MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'lidarFifo:111' if isLidarValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isLidarValid) {
      //  LiDAR timestamps must normally increase monotonically.
      // 'lidarFifo:113' if haveLastPushTime && lidarTimeIn_ms <= lastPushTime_ms 
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_jj &
           uMultiWordLe(&lidarTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_c.chunks[0U], 2)))
      {
        // 'lidarFifo:114' backwardsJump_ms = lastPushTime_ms - lidarTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'lidarFifo:117' if timeResetThreshold_ms > 0 && ...
        // 'lidarFifo:118'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_c.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&lidarTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&lidarFifoParams_resetThreshold_.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &lidarFifoParams_resetThreshold_.chunks[0U], 2))) {
          // 'lidarFifo:119' head = uint16(1);
          stateEstimatorEskf_DW.head_f2 = 1U;

          // 'lidarFifo:120' tail = uint16(1);
          stateEstimatorEskf_DW.tail_pr = 1U;

          // 'lidarFifo:121' count = uint16(0);
          stateEstimatorEskf_DW.count_kg = 0U;

          // 'lidarFifo:122' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_c = tmp_5;

          // 'lidarFifo:123' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_jj = false;

          // 'lidarFifo:124' status = enumDhFifoStatus.TIMEBASE_RESET;
          *status = enumDhFifoStatus::TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'lidarFifo:125' else
          //  Duplicate timestamp or small backward jump.
          // 'lidarFifo:127' status = enumDhFifoStatus.BAD_TIMESTAMP;
          *status = enumDhFifoStatus::BAD_TIMESTAMP;
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
      // 'lidarFifo:134' if haveLastPushTime && ...
      // 'lidarFifo:135'             (lidarTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&lidarTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_c.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_jj &
           uMultiWordLt(&tmp_3.chunks[0U],
                        &lidarFifoParams_minInterval_ms.chunks[0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'lidarFifo:140' bufferWasFull = (count == capacity);
        //  Store AGL measurement and timestamp.
        // 'lidarFifo:143' timeBuf_ms(head) = lidarTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_m[stateEstimatorEskf_DW.head_f2 - 1] =
          lidarTimeIn_ms;

        // 'lidarFifo:144' aglBuf_m(head) = aglIn_m;
        stateEstimatorEskf_DW.aglBuf_m[stateEstimatorEskf_DW.head_f2 - 1] =
          aglIn_m;

        //  Advance write location.
        // 'lidarFifo:147' if head == capacity
        if (stateEstimatorEskf_DW.head_f2 == lidarFifoParams_capacity) {
          // 'lidarFifo:148' head = uint16(1);
          stateEstimatorEskf_DW.head_f2 = 1U;
        } else {
          // 'lidarFifo:149' else
          // 'lidarFifo:150' head = head + 1;
          qY = stateEstimatorEskf_DW.head_f2 + 1U;
          if (stateEstimatorEskf_DW.head_f2 + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_f2 = static_cast<uint16_T>(qY);
        }

        // 'lidarFifo:153' if bufferWasFull
        if (stateEstimatorEskf_DW.count_kg == lidarFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'lidarFifo:156' if tail == capacity
          if (stateEstimatorEskf_DW.tail_pr == lidarFifoParams_capacity) {
            // 'lidarFifo:157' tail = uint16(1);
            stateEstimatorEskf_DW.tail_pr = 1U;
          } else {
            // 'lidarFifo:158' else
            // 'lidarFifo:159' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_pr + 1U;
            if (stateEstimatorEskf_DW.tail_pr + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_pr = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'lidarFifo:163' count = capacity;
          stateEstimatorEskf_DW.count_kg = lidarFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'lidarFifo:166' if status == enumDhFifoStatus.OK
          if (*status == enumDhFifoStatus::OK) {
            // 'lidarFifo:167' status = enumDhFifoStatus.OVERWRITE;
            *status = enumDhFifoStatus::OVERWRITE;
          }
        } else {
          // 'lidarFifo:169' else
          //  One new measurement was added without replacing anything.
          // 'lidarFifo:171' count = count + 1;
          qY = stateEstimatorEskf_DW.count_kg + 1U;
          if (stateEstimatorEskf_DW.count_kg + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_kg = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'lidarFifo:175' lastPushTime_ms = lidarTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_c = lidarTimeIn_ms;

        // 'lidarFifo:176' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_jj = true;

        // 'lidarFifo:177' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_kg;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST LIDAR MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'lidarFifo:185' if count == 0
      if ((stateEstimatorEskf_DW.count_kg == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_m[stateEstimatorEskf_DW.tail_pr - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        int32_T k;
        boolean_T exitg1;

        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'lidarFifo:191' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ lidarTime | lidarTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'lidarFifo:202' for k = 1:(MAX_SIZE-1)
        k = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((k < 31) &
                 (stateEstimatorEskf_DW.count_kg != 1)))) {
          uint16_T nextIdx;

          // 'lidarFifo:203' if count == 1
          // 'lidarFifo:207' if tail == capacity
          if (stateEstimatorEskf_DW.tail_pr == lidarFifoParams_capacity) {
            // 'lidarFifo:208' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'lidarFifo:209' else
            // 'lidarFifo:210' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_pr + 1U;
            if (stateEstimatorEskf_DW.tail_pr + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'lidarFifo:213' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_m[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'lidarFifo:218' tail = nextIdx;
            stateEstimatorEskf_DW.tail_pr = nextIdx;

            // 'lidarFifo:219' count = count - 1;
            qY = stateEstimatorEskf_DW.count_kg -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_kg - 1U >
                stateEstimatorEskf_DW.count_kg) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_kg = static_cast<uint16_T>(qY);
            k++;
          }
        }

        //  Tail now points to the newest eligible LiDAR measurement.
        // 'lidarFifo:223' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail); 
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'lidarFifo:227' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&lidarFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_m[stateEstimatorEskf_DW.tail_pr -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &lidarFifoParams_maxAge_ms.chunks
                           [0U], 2)) {
            // 'lidarFifo:228' if tail == capacity
            if (stateEstimatorEskf_DW.tail_pr == lidarFifoParams_capacity) {
              // 'lidarFifo:229' tail = uint16(1);
              stateEstimatorEskf_DW.tail_pr = 1U;
            } else {
              // 'lidarFifo:230' else
              // 'lidarFifo:231' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_pr + 1U;
              if (stateEstimatorEskf_DW.tail_pr + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_pr = static_cast<uint16_T>(qY);
            }

            // 'lidarFifo:234' count = count - 1;
            qY = stateEstimatorEskf_DW.count_kg -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_kg - 1U >
                stateEstimatorEskf_DW.count_kg) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_kg = static_cast<uint16_T>(qY);

            // 'lidarFifo:235' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_kg;

            // 'lidarFifo:236' status = enumDhFifoStatus.STALE_DISCARD;
            *status = enumDhFifoStatus::STALE_DISCARD;
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
      // 'lidarFifo:241' lidarTimeOut_ms = timeBuf_ms(tail);
      *lidarTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_m[stateEstimatorEskf_DW.tail_pr - 1];

      // 'lidarFifo:242' aglOut_m = aglBuf_m(tail);
      *aglOut_m = stateEstimatorEskf_DW.aglBuf_m[stateEstimatorEskf_DW.tail_pr -
        1];

      // 'lidarFifo:243' lidarReady = true;
      *lidarReady = true;

      //  Hold last returned measurement for future calls where lidarReady is false. 
      // 'lidarFifo:246' lastLidarTimeOut_ms = lidarTimeOut_ms;
      stateEstimatorEskf_DW.lastLidarTimeOut_ms = *lidarTimeOut_ms;

      // 'lidarFifo:247' lastAglOut_m = aglOut_m;
      stateEstimatorEskf_DW.lastAglOut_m = *aglOut_m;

      //  Consume returned measurement.
      // 'lidarFifo:250' if tail == capacity
      if (stateEstimatorEskf_DW.tail_pr == lidarFifoParams_capacity) {
        // 'lidarFifo:251' tail = uint16(1);
        stateEstimatorEskf_DW.tail_pr = 1U;
      } else {
        // 'lidarFifo:252' else
        // 'lidarFifo:253' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_pr + 1U;
        if (stateEstimatorEskf_DW.tail_pr + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_pr = static_cast<uint16_T>(qY);
      }

      // 'lidarFifo:256' count = count - 1;
      qY = stateEstimatorEskf_DW.count_kg -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_kg - 1U > stateEstimatorEskf_DW.count_kg)
      {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_kg = static_cast<uint16_T>(qY);

      // 'lidarFifo:257' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_kg;
    }
  }
}

//
// Function for MATLAB Function: '<S15>/DelayedHorizonBufferManager'
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
  boolean_T *flowReady, uint64m_T *flowTimeOut_ms, real32_T velNEOut[2],
  enumDhFifoStatus *status, uint16_T *countOut)
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
  //  Static FIFO storage/state initialization.
  // 'flowFifo:56' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. flowReady still indicates whether this call returned a new sample.
  // 'flowFifo:72' flowReady = false;
  *flowReady = false;

  // 'flowFifo:73' flowTimeOut_ms = lastFlowTimeOut_ms;
  *flowTimeOut_ms = stateEstimatorEskf_DW.lastFlowTimeOut_ms;

  // 'flowFifo:74' velNEOut = lastVelNEOut;
  velNEOut[0] = stateEstimatorEskf_DW.lastVelNEOut[0];
  velNEOut[1] = stateEstimatorEskf_DW.lastVelNEOut[1];

  // 'flowFifo:75' status = enumDhFifoStatus.OK;
  *status = enumDhFifoStatus::OK;

  // 'flowFifo:76' countOut = count;
  *countOut = stateEstimatorEskf_DW.count_a;

  //  Read-only configuration aliases.
  // 'flowFifo:79' capacity = flowFifoParams.capacity;
  // 'flowFifo:80' maxAge_ms = flowFifoParams.maxAge_ms;
  // 'flowFifo:81' minObsInterval_ms = flowFifoParams.minInterval_ms;
  // 'flowFifo:82' timeResetThreshold_ms = flowFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'flowFifo:85' if capacity < 1 || capacity > MAX_SIZE
  if (static_cast<boolean_T>((flowFifoParams_capacity < 1) |
       (flowFifoParams_capacity > 24))) {
    // 'flowFifo:86' status = enumDhFifoStatus.BAD_CAPACITY;
    *status = enumDhFifoStatus::BAD_CAPACITY;

    //  Explicit estimator/FIFO reset.
    // 'flowFifo:91' if reset
  } else if (reset) {
    // 'flowFifo:92' head = uint16(1);
    stateEstimatorEskf_DW.head_c = 1U;

    // 'flowFifo:93' tail = uint16(1);
    stateEstimatorEskf_DW.tail_i = 1U;

    // 'flowFifo:94' count = uint16(0);
    stateEstimatorEskf_DW.count_a = 0U;

    // 'flowFifo:95' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_f = tmp_5;

    // 'flowFifo:96' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_p = false;

    // 'flowFifo:98' lastFlowTimeOut_ms = uint64(0);
    stateEstimatorEskf_DW.lastFlowTimeOut_ms = tmp_5;

    // 'flowFifo:99' lastVelNEOut = zeros(2,1,'single');
    // 'flowFifo:101' flowReady = false;
    // 'flowFifo:102' flowTimeOut_ms = uint64(0);
    *flowTimeOut_ms = tmp_5;

    // 'flowFifo:103' velNEOut = zeros(2,1,'single');
    stateEstimatorEskf_DW.lastVelNEOut[0] = 0.0F;
    velNEOut[0] = 0.0F;
    stateEstimatorEskf_DW.lastVelNEOut[1] = 0.0F;
    velNEOut[1] = 0.0F;

    // 'flowFifo:105' countOut = count;
    *countOut = 0U;
  } else {
    int32_T dataBuf_l_tmp;
    uint32_T qY;
    boolean_T guard1;
    boolean_T guard2;
    boolean_T guard3;

    //  ------------------------------------------------------------------------- 
    //  PUSH NEW OPTICAL-FLOW MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'flowFifo:112' if isFlowValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (isFlowValid) {
      //  Optical-flow timestamps must normally increase monotonically.
      // 'flowFifo:114' if haveLastPushTime && flowTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_p &
           uMultiWordLe(&flowTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_f.chunks[0U], 2)))
      {
        // 'flowFifo:115' backwardsJump_ms = lastPushTime_ms - flowTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'flowFifo:118' if timeResetThreshold_ms > 0 && ...
        // 'flowFifo:119'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_f.chunks[0U],
                             2, &tmp_1.chunks[0U], 3);
        uMultiWord2MultiWord(&flowTimeIn_ms.chunks[0U], 2, &tmp_2.chunks[0U], 3);
        MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
        sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp.chunks[0U], 2);
        if (static_cast<boolean_T>(uMultiWordGt
             (&flowFifoParams_resetThreshold_m.chunks[0U], &tmp_5.chunks[0U], 2)
             & uMultiWordGe(&tmp.chunks[0U],
                            &flowFifoParams_resetThreshold_m.chunks[0U], 2))) {
          // 'flowFifo:120' head = uint16(1);
          stateEstimatorEskf_DW.head_c = 1U;

          // 'flowFifo:121' tail = uint16(1);
          stateEstimatorEskf_DW.tail_i = 1U;

          // 'flowFifo:122' count = uint16(0);
          stateEstimatorEskf_DW.count_a = 0U;

          // 'flowFifo:123' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_f = tmp_5;

          // 'flowFifo:124' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_p = false;

          // 'flowFifo:125' status = enumDhFifoStatus.TIMEBASE_RESET;
          *status = enumDhFifoStatus::TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'flowFifo:126' else
          //  Duplicate timestamp or small backward jump.
          // 'flowFifo:128' status = enumDhFifoStatus.BAD_TIMESTAMP;
          *status = enumDhFifoStatus::BAD_TIMESTAMP;
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
      // 'flowFifo:135' if haveLastPushTime && ...
      // 'flowFifo:136'             (flowTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&flowTimeIn_ms.chunks[0U], 2, &tmp_1.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_f.chunks[0U],
                           2, &tmp_2.chunks[0U], 3);
      MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_3.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_p &
           uMultiWordLt(&tmp_3.chunks[0U],
                        &flowFifoParams_minInterval_ms.chunks[0U], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'flowFifo:141' bufferWasFull = (count == capacity);
        //  Store complete optical-flow observation at current write location.
        //  Each column contains [vN; vE].
        // 'flowFifo:145' timeBuf_ms(head) = flowTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.head_c - 1] =
          flowTimeIn_ms;

        // 'flowFifo:146' dataBuf(:,head) = velNEIn;
        dataBuf_l_tmp = (stateEstimatorEskf_DW.head_c - 1) << 1;
        stateEstimatorEskf_DW.dataBuf_l[dataBuf_l_tmp] = velNEIn[0];
        stateEstimatorEskf_DW.dataBuf_l[dataBuf_l_tmp + 1] = velNEIn[1];

        //  Advance write location.
        // 'flowFifo:149' if head == capacity
        if (stateEstimatorEskf_DW.head_c == flowFifoParams_capacity) {
          // 'flowFifo:150' head = uint16(1);
          stateEstimatorEskf_DW.head_c = 1U;
        } else {
          // 'flowFifo:151' else
          // 'flowFifo:152' head = head + 1;
          qY = stateEstimatorEskf_DW.head_c + 1U;
          if (stateEstimatorEskf_DW.head_c + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_c = static_cast<uint16_T>(qY);
        }

        // 'flowFifo:155' if bufferWasFull
        if (stateEstimatorEskf_DW.count_a == flowFifoParams_capacity) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'flowFifo:158' if tail == capacity
          if (stateEstimatorEskf_DW.tail_i == flowFifoParams_capacity) {
            // 'flowFifo:159' tail = uint16(1);
            stateEstimatorEskf_DW.tail_i = 1U;
          } else {
            // 'flowFifo:160' else
            // 'flowFifo:161' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_i + 1U;
            if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'flowFifo:165' count = capacity;
          stateEstimatorEskf_DW.count_a = flowFifoParams_capacity;

          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'flowFifo:168' if status == enumDhFifoStatus.OK
          if (*status == enumDhFifoStatus::OK) {
            // 'flowFifo:169' status = enumDhFifoStatus.OVERWRITE;
            *status = enumDhFifoStatus::OVERWRITE;
          }
        } else {
          // 'flowFifo:171' else
          //  One new measurement was added without replacing anything.
          // 'flowFifo:173' count = count + 1;
          qY = stateEstimatorEskf_DW.count_a + 1U;
          if (stateEstimatorEskf_DW.count_a + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_a = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'flowFifo:177' lastPushTime_ms = flowTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_f = flowTimeIn_ms;

        // 'flowFifo:178' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_p = true;

        // 'flowFifo:179' countOut = count;
        *countOut = stateEstimatorEskf_DW.count_a;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST OPTICAL-FLOW MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'flowFifo:187' if count == 0
      if ((stateEstimatorEskf_DW.count_a == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.tail_i - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        boolean_T exitg1;

        //  Because measurements are chronologically ordered, if the oldest
        //  measurement is newer than the fusion horizon, nothing is ready.
        // 'flowFifo:193' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ flowTime | flowTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'flowFifo:204' for k = 1:(MAX_SIZE-1)
        dataBuf_l_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((dataBuf_l_tmp < 23) &
                 (stateEstimatorEskf_DW.count_a != 1)))) {
          uint16_T nextIdx;

          // 'flowFifo:205' if count == 1
          // 'flowFifo:209' if tail == capacity
          if (stateEstimatorEskf_DW.tail_i == flowFifoParams_capacity) {
            // 'flowFifo:210' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'flowFifo:211' else
            // 'flowFifo:212' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_i + 1U;
            if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'flowFifo:215' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_o[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible sample exists; discard current tail.
            // 'flowFifo:220' tail = nextIdx;
            stateEstimatorEskf_DW.tail_i = nextIdx;

            // 'flowFifo:221' count = count - 1;
            qY = stateEstimatorEskf_DW.count_a -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_a - 1U >
                stateEstimatorEskf_DW.count_a) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_a = static_cast<uint16_T>(qY);
            dataBuf_l_tmp++;
          }
        }

        //  Tail now points to the newest eligible optical-flow measurement.
        // 'flowFifo:225' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'flowFifo:229' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt(&flowFifoParams_maxAge_ms.chunks[0U], &tmp_5.chunks[0U],
                         2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_1.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.tail_i -
             1].chunks[0U], 2, &tmp_2.chunks[0U], 3);
          MultiWordSub(&tmp_1.chunks[0U], &tmp_2.chunks[0U], &tmp_0.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_0.chunks[0U], 3, &tmp_4.chunks[0U], 2);
          if (uMultiWordGe(&tmp_4.chunks[0U], &flowFifoParams_maxAge_ms.chunks
                           [0U], 2)) {
            // 'flowFifo:230' if tail == capacity
            if (stateEstimatorEskf_DW.tail_i == flowFifoParams_capacity) {
              // 'flowFifo:231' tail = uint16(1);
              stateEstimatorEskf_DW.tail_i = 1U;
            } else {
              // 'flowFifo:232' else
              // 'flowFifo:233' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_i + 1U;
              if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
            }

            // 'flowFifo:236' count = count - 1;
            qY = stateEstimatorEskf_DW.count_a -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_a - 1U >
                stateEstimatorEskf_DW.count_a) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_a = static_cast<uint16_T>(qY);

            // 'flowFifo:237' countOut = count;
            *countOut = stateEstimatorEskf_DW.count_a;

            // 'flowFifo:238' status = enumDhFifoStatus.STALE_DISCARD;
            *status = enumDhFifoStatus::STALE_DISCARD;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }
    }

    if (guard1) {
      real32_T velNEOut_tmp;

      //  Return selected optical-flow measurement.
      // 'flowFifo:243' flowTimeOut_ms = timeBuf_ms(tail);
      *flowTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_o[stateEstimatorEskf_DW.tail_i - 1];

      // 'flowFifo:244' velNEOut = dataBuf(:,tail);
      // 'flowFifo:245' flowReady = true;
      *flowReady = true;

      //  Hold last returned measurement for future calls where flowReady is false. 
      // 'flowFifo:248' lastFlowTimeOut_ms = flowTimeOut_ms;
      stateEstimatorEskf_DW.lastFlowTimeOut_ms = *flowTimeOut_ms;

      // 'flowFifo:249' lastVelNEOut = velNEOut;
      dataBuf_l_tmp = (stateEstimatorEskf_DW.tail_i - 1) << 1;
      velNEOut_tmp = stateEstimatorEskf_DW.dataBuf_l[dataBuf_l_tmp];
      velNEOut[0] = velNEOut_tmp;
      stateEstimatorEskf_DW.lastVelNEOut[0] = velNEOut_tmp;
      velNEOut_tmp = stateEstimatorEskf_DW.dataBuf_l[dataBuf_l_tmp + 1];
      velNEOut[1] = velNEOut_tmp;
      stateEstimatorEskf_DW.lastVelNEOut[1] = velNEOut_tmp;

      //  Consume returned measurement.
      // 'flowFifo:252' if tail == capacity
      if (stateEstimatorEskf_DW.tail_i == flowFifoParams_capacity) {
        // 'flowFifo:253' tail = uint16(1);
        stateEstimatorEskf_DW.tail_i = 1U;
      } else {
        // 'flowFifo:254' else
        // 'flowFifo:255' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_i + 1U;
        if (stateEstimatorEskf_DW.tail_i + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_i = static_cast<uint16_T>(qY);
      }

      // 'flowFifo:258' count = count - 1;
      qY = stateEstimatorEskf_DW.count_a -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_a - 1U > stateEstimatorEskf_DW.count_a) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_a = static_cast<uint16_T>(qY);

      // 'flowFifo:259' countOut = count;
      *countOut = stateEstimatorEskf_DW.count_a;
    }
  }
}

// System initialize for referenced model: 'stateEstimatorEskf'
void stateEstimatorEskf::init(void)
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

  // SystemInitialize for MATLAB Function: '<S15>/DelayedHorizonBufferManager'
  // 'stateFifo:109' modeBuf = repmat(enumStateEstimateMode.NONE,1,MAX_SIZE);
  // 'stateFifo:111' head = uint16(1);
  stateEstimatorEskf_DW.head = 1U;

  //  Next write location
  // 'stateFifo:112' tail = uint16(1);
  stateEstimatorEskf_DW.tail = 1U;

  //  Oldest valid mode
  // 'stateFifo:113' count = uint16(0);
  //  Number of valid stored modes
  // 'imuFifo:85' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'imuFifo:86' dataBuf = zeros(7,MAX_SIZE,'single');
  // 'imuFifo:88' head = uint16(1);
  stateEstimatorEskf_DW.head_e = 1U;

  //  Next write location
  // 'imuFifo:89' tail = uint16(1);
  stateEstimatorEskf_DW.tail_g = 1U;

  //  Oldest valid IMU sample
  // 'imuFifo:90' count = uint16(0);
  //  Number of valid samples
  // 'imuFifo:92' lastImuTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastImuTimeOut_ms = tmp_0;

  // 'imuFifo:93' lastImuOut = zeros(7,1,'single');
  // 'magFifo:57' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 64; i++) {
    stateEstimatorEskf_DW.timeBuf_ms[i] = tmp_0;
    stateEstimatorEskf_DW.timeBuf_ms_k[i] = tmp_0;
  }

  // 'magFifo:58' dataBuf = zeros(3,MAX_SIZE,'single');
  // 'magFifo:59' head = uint16(1);
  stateEstimatorEskf_DW.head_b = 1U;

  //  Next write location
  // 'magFifo:60' tail = uint16(1);
  stateEstimatorEskf_DW.tail_k = 1U;

  //  Oldest valid measurement
  // 'magFifo:61' count = uint16(0);
  //  Number of valid measurements
  // 'magFifo:62' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms = tmp_0;

  //  Last accepted magnetometer timestamp
  // 'magFifo:63' haveLastPushTime = false;
  // 'magFifo:65' lastMagTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastMagTimeOut_ms = tmp_0;

  // 'magFifo:66' lastMagOut_uT = zeros(3,1,'single');
  // 'gpsFifo:57' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 16; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_c[i] = tmp_0;
  }

  // 'gpsFifo:58' dataBuf = zeros(6,MAX_SIZE,'single');
  // 'gpsFifo:59' head = uint16(1);
  stateEstimatorEskf_DW.head_a = 1U;

  //  Next write location
  // 'gpsFifo:60' tail = uint16(1);
  stateEstimatorEskf_DW.tail_c = 1U;

  //  Oldest valid measurement
  // 'gpsFifo:61' count = uint16(0);
  //  Number of valid measurements
  // 'gpsFifo:62' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_b = tmp_0;

  //  Last accepted GPS timestamp
  // 'gpsFifo:63' haveLastPushTime = false;
  // 'gpsFifo:65' lastGpsTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastGpsTimeOut_ms = tmp_0;

  // 'gpsFifo:66' lastPosVelOut = zeros(6,1,'single');
  // 'baroFifo:56' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'baroFifo:57' altitudeBuf_m = zeros(1,MAX_SIZE,'single');
  for (i = 0; i < 24; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_g[i] = tmp_0;
  }

  // 'baroFifo:58' head = uint16(1);
  stateEstimatorEskf_DW.head_f = 1U;

  //  Next write location
  // 'baroFifo:59' tail = uint16(1);
  stateEstimatorEskf_DW.tail_p = 1U;

  //  Oldest valid measurement
  // 'baroFifo:60' count = uint16(0);
  //  Number of valid measurements
  // 'baroFifo:61' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_j = tmp_0;

  //  Last accepted barometer timestamp
  // 'baroFifo:62' haveLastPushTime = false;
  // 'baroFifo:64' lastBaroTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastBaroTimeOut_ms = tmp_0;

  // 'baroFifo:65' lastAltitudeOut_m = single(0);
  // 'lidarFifo:56' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  // 'lidarFifo:57' aglBuf_m = zeros(1,MAX_SIZE,'single');
  for (i = 0; i < 32; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_m[i] = tmp_0;
  }

  // 'lidarFifo:58' head = uint16(1);
  stateEstimatorEskf_DW.head_f2 = 1U;

  //  Next write location
  // 'lidarFifo:59' tail = uint16(1);
  stateEstimatorEskf_DW.tail_pr = 1U;

  //  Oldest valid measurement
  // 'lidarFifo:60' count = uint16(0);
  //  Number of valid measurements
  // 'lidarFifo:61' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_c = tmp_0;

  //  Last accepted LiDAR timestamp
  // 'lidarFifo:62' haveLastPushTime = false;
  // 'lidarFifo:64' lastLidarTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastLidarTimeOut_ms = tmp_0;

  // 'lidarFifo:65' lastAglOut_m = single(0);
  // 'flowFifo:57' timeBuf_ms = zeros(1,MAX_SIZE,'uint64');
  for (i = 0; i < 24; i++) {
    stateEstimatorEskf_DW.timeBuf_ms_o[i] = tmp_0;
  }

  // 'flowFifo:58' dataBuf = zeros(2,MAX_SIZE,'single');
  // 'flowFifo:59' head = uint16(1);
  stateEstimatorEskf_DW.head_c = 1U;

  //  Next write location
  // 'flowFifo:60' tail = uint16(1);
  stateEstimatorEskf_DW.tail_i = 1U;

  //  Oldest valid measurement
  // 'flowFifo:61' count = uint16(0);
  //  Number of valid measurements
  // 'flowFifo:62' lastPushTime_ms = uint64(0);
  stateEstimatorEskf_DW.lastPushTime_ms_f = tmp_0;

  //  Last accepted flow timestamp
  // 'flowFifo:63' haveLastPushTime = false;
  // 'flowFifo:65' lastFlowTimeOut_ms = uint64(0);
  stateEstimatorEskf_DW.lastFlowTimeOut_ms = tmp_0;

  // End of SystemInitialize for MATLAB Function: '<S15>/DelayedHorizonBufferManager' 

  // SystemInitialize for MATLAB Function: '<S1>/EKF'
  // 'flowFifo:66' lastVelNEOut = zeros(2,1,'single');
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
  const busMtf01pData *rtu_mtf01pData, const busImuNtchFiltParams
  *rtu_imuNotchFiltParams, const busAccelParams *rtu_accelParams, const
  busMagParams *rtu_magParams, const busMtf01pParams *rtu_mtf01pParams, const
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
  real32_T rtb_ekfDebugOut_dhStates[23];
  real32_T rtb_states[20];
  real32_T b_errorStateHat[19];
  real32_T d_K[19];
  real32_T errorStateHat[19];
  real32_T K_0[15];
  real32_T tmp_3[15];
  real32_T rtb_states_0[12];
  real32_T rtb_VectorConcatenate_p[9];
  real32_T nedPosAndVel[6];
  real32_T rtb_CastToSingle_0[6];
  real32_T rtb_ElementProduct[6];
  real32_T qDelayed[4];
  real32_T qError[4];
  real32_T tmp_2[4];
  real32_T Divide[3];
  real32_T Product[3];
  real32_T rtb_CastToSingle[3];
  real32_T rtb_VectorConcatenate1_0[3];
  real32_T VectorConcatenate[2];
  real32_T rtb_VectorConcatenate_k2[2];
  real32_T q2q3;
  real32_T rtb_Merge_idx_0;
  real32_T rtb_Merge_idx_0_tmp;
  real32_T rtb_Merge_idx_0_tmp_0;
  real32_T rtb_Merge_idx_0_tmp_1;
  real32_T rtb_Merge_idx_0_tmp_2;
  real32_T rtb_Merge_idx_1;
  real32_T rtb_Product2_c;
  real32_T rtb_Product3_l;
  real32_T rtb_Sum_m1_idx_1;
  real32_T rtb_Sum_m1_idx_2;
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
  uint8_T rtb_ekfDebugOut_isAidingUsed_is;
  boolean_T dhSensorIn_sensorValidity_isGps;
  boolean_T dhSensorIn_sensorValidity_isLid;
  boolean_T dhSensorIn_sensorValidity_isMag;
  boolean_T dhSensorIn_sensorValidity_isOfV;
  boolean_T gpsLossFlag;
  boolean_T imuReady;
  boolean_T mtf01pDistValidityFlag;
  boolean_T rtb_Compare;
  boolean_T rtb_ekfDebugOut_isAidingUsed__0;
  boolean_T rtb_ekfDebugOut_isAidingUsed__1;
  boolean_T stateModeReady;
  enumDhFifoStatus status;
  enumStateEstimateMode estSmModeOut;
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

  // DiscreteTransferFcn: '<S65>/X Axis'
  stateEstimatorEskf_DW.XAxis_tmp = (rtu_imuData->bodyAccels_mps2[0] -
    stateEstimatorEskf_DW.XAxis_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[1]) -
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[2];
  rtb_Product2_c = (rtu_imuNotchFiltParams->accelNtchFilt.xNum[0] *
                    stateEstimatorEskf_DW.XAxis_tmp +
                    stateEstimatorEskf_DW.XAxis_states[0] *
                    rtu_imuNotchFiltParams->accelNtchFilt.xNum[1]) +
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xNum[2];

  // DiscreteTransferFcn: '<S65>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_tmp = (rtu_imuData->bodyAccels_mps2[1] -
    stateEstimatorEskf_DW.XAxis1_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[1]) -
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[2];
  rtb_Product3_l = (rtu_imuNotchFiltParams->accelNtchFilt.yNum[0] *
                    stateEstimatorEskf_DW.XAxis1_tmp +
                    stateEstimatorEskf_DW.XAxis1_states[0] *
                    rtu_imuNotchFiltParams->accelNtchFilt.yNum[1]) +
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yNum[2];

  // DiscreteTransferFcn: '<S65>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_tmp = (rtu_imuData->bodyAccels_mps2[2] -
    stateEstimatorEskf_DW.XAxis2_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[1]) -
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[2];
  rtb_UnitDelay_g = (rtu_imuNotchFiltParams->accelNtchFilt.zNum[0] *
                     stateEstimatorEskf_DW.XAxis2_tmp +
                     stateEstimatorEskf_DW.XAxis2_states[0] *
                     rtu_imuNotchFiltParams->accelNtchFilt.zNum[1]) +
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zNum[2];

  // Product: '<S6>/Product' incorporates:
  //   Product: '<S6>/Matrix Multiply'
  //   SignalConversion generated from: '<S6>/Matrix Multiply'
  //   Sum: '<S6>/Sum'

  Product[0] = (((rtu_accelParams->scaleAlignMat_nd[0] * rtb_Product2_c +
                  rtu_accelParams->scaleAlignMat_nd[3] * rtb_Product3_l) +
                 rtu_accelParams->scaleAlignMat_nd[6] * rtb_UnitDelay_g) +
                rtu_accelParams->offset_nd[0]) * *rtu_gEarth_mps2;
  Product[1] = (((rtu_accelParams->scaleAlignMat_nd[1] * rtb_Product2_c +
                  rtu_accelParams->scaleAlignMat_nd[4] * rtb_Product3_l) +
                 rtu_accelParams->scaleAlignMat_nd[7] * rtb_UnitDelay_g) +
                rtu_accelParams->offset_nd[1]) * *rtu_gEarth_mps2;
  Product[2] = (((rtu_accelParams->scaleAlignMat_nd[2] * rtb_Product2_c +
                  rtu_accelParams->scaleAlignMat_nd[5] * rtb_Product3_l) +
                 rtu_accelParams->scaleAlignMat_nd[8] * rtb_UnitDelay_g) +
                rtu_accelParams->offset_nd[2]) * *rtu_gEarth_mps2;

  // DiscreteTransferFcn: '<S66>/X Axis'
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

  // DiscreteTransferFcn: '<S66>/X Axis1'
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

  // DiscreteTransferFcn: '<S66>/X Axis2'
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

  // Sum: '<S11>/Sum'
  rtb_Product2_c = rtu_magData->bodyMagVector_uT[0] - rtu_magParams->offset_uT[0];
  rtb_UnitDelay_g = rtu_magData->bodyMagVector_uT[1] - rtu_magParams->offset_uT
    [1];
  rtb_Sum_m1_idx_2 = rtu_magData->bodyMagVector_uT[2] - rtu_magParams->
    offset_uT[2];

  // Product: '<S11>/Matrix Multiply' incorporates:
  //   Sum: '<S72>/Sum'

  rtb_Product3_l = (rtu_magParams->scaleAlignMat_nd[0] * rtb_Product2_c +
                    rtu_magParams->scaleAlignMat_nd[3] * rtb_UnitDelay_g) +
    rtu_magParams->scaleAlignMat_nd[6] * rtb_Sum_m1_idx_2;
  rtb_Sum_m1_idx_1 = (rtu_magParams->scaleAlignMat_nd[1] * rtb_Product2_c +
                      rtu_magParams->scaleAlignMat_nd[4] * rtb_UnitDelay_g) +
    rtu_magParams->scaleAlignMat_nd[7] * rtb_Sum_m1_idx_2;
  rtb_Sum_m1_idx_2 = (rtu_magParams->scaleAlignMat_nd[2] * rtb_Product2_c +
                      rtu_magParams->scaleAlignMat_nd[5] * rtb_UnitDelay_g) +
    rtu_magParams->scaleAlignMat_nd[8] * rtb_Sum_m1_idx_2;

  // MinMax: '<S11>/Max' incorporates:
  //   Constant: '<S11>/Constant2'
  //   Math: '<S11>/Transpose'
  //   Product: '<S11>/Matrix Multiply1'
  //   Sqrt: '<S11>/Sqrt'
  //   Sum: '<S72>/Sum'

  rtb_Product2_c = std::fmax(std::sqrt((rtb_Product3_l * rtb_Product3_l +
    rtb_Sum_m1_idx_1 * rtb_Sum_m1_idx_1) + rtb_Sum_m1_idx_2 * rtb_Sum_m1_idx_2),
    1.0E-7F);

  // Product: '<S11>/Divide' incorporates:
  //   Sum: '<S72>/Sum'

  Divide[0] = rtb_Product3_l / rtb_Product2_c;
  Divide[1] = rtb_Sum_m1_idx_1 / rtb_Product2_c;
  Divide[2] = rtb_Sum_m1_idx_2 / rtb_Product2_c;

  // SignalConversion generated from: '<Root>/Vector Concatenate' incorporates:
  //   Concatenate: '<Root>/Vector Concatenate'

  VectorConcatenate[0] = rtu_gpsData->hacc_m;

  // SignalConversion generated from: '<Root>/Vector Concatenate' incorporates:
  //   Concatenate: '<Root>/Vector Concatenate'

  VectorConcatenate[1] = rtu_gpsData->vacc_m;

  // Product: '<S13>/Divide1' incorporates:
  //   Constant: '<S13>/Constant'
  //   Constant: '<S13>/Constant2'
  //   Constant: '<S13>/Constant3'
  //   Math: '<S13>/Power'
  //   Product: '<S13>/Divide'
  //   Sum: '<S13>/Sum'

  rtb_Product2_c = (1.0F - std::pow(rtu_baroData->pressure_pa / 101325.0F,
    0.190294951F)) * 44330.0F;

  // SignalConversion generated from: '<S7>/ SFunction ' incorporates:
  //   Chart: '<Root>/estimatorStateMachine'

  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0] = rtb_XAxis;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1] = rtb_XAxis1;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2] = rtb_XAxis2;

  // Chart: '<Root>/estimatorStateMachine' incorporates:
  //   Product: '<S11>/Divide'
  //   Product: '<S6>/Product'
  //   SignalConversion generated from: '<Root>/Vector Concatenate'
  //   SignalConversion generated from: '<S7>/ SFunction '
  //
  // Gateway: estimatorStateMachine
  // During: estimatorStateMachine
  if (stateEstimatorEskf_DW.is_active_c3_stateEstimatorEskf == 0) {
    // Entry: estimatorStateMachine
    stateEstimatorEskf_DW.is_active_c3_stateEstimatorEskf = 1U;

    // Entry Internal: estimatorStateMachine
    // Transition: '<S7>:2'
    stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
      stateEstimatorEsk_IN_INITIALIZE;

    // Entry 'INITIALIZE': '<S7>:1'
    // Variables to set on entry
    // '<S7>:1:4' mode = enumStateEstimateMode.INITIALIZE;
    mode = enumStateEstimateMode::INITIALIZE;

    // '<S7>:1:5' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
    // '<S7>:1:6' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
    // '<S7>:1:7' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
    // '<S7>:1:8' sensorDataOut.sensorValidity.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
      rtu_magData->isMagDataValid;

    // '<S7>:1:9' latLonAltOut = latLonAltIn;
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

    // '<S7>:1:10' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
      rtu_gpsData->isGpsDataValid;

    // '<S7>:1:11' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S7>:1:12' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
      rtu_baroData->isBaroDataValid;

    // '<S7>:1:13' resetStates = true;
    stateEstimatorEskf_DW.resetStates = true;

    //  Index to keep track of how many imu readings we have summed
    //  so far
    // '<S7>:1:16' imuIdx = 0;
    stateEstimatorEskf_DW.imuIdx = 0.0F;

    // '<S7>:1:17' imuMean = [0; 0; 0; 0; 0; 0];
    stateEstimatorEskf_DW.imuMean[0] = 0.0F;
    stateEstimatorEskf_DW.imuMean[1] = 0.0F;
    stateEstimatorEskf_DW.imuMean[2] = 0.0F;
    stateEstimatorEskf_DW.imuMean[3] = 0.0F;
    stateEstimatorEskf_DW.imuMean[4] = 0.0F;
    stateEstimatorEskf_DW.imuMean[5] = 0.0F;

    // '<S7>:1:18' accelBias_mps2 = [0; 0; 0];
    // '<S7>:1:19' gyroBias_radps = [0; 0; 0];
    stateEstimatorEskf_DW.accelBias_mps2[0] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[0] = 0.0F;
    stateEstimatorEskf_DW.accelBias_mps2[1] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[1] = 0.0F;
    stateEstimatorEskf_DW.accelBias_mps2[2] = 0.0F;
    stateEstimatorEskf_DW.gyroBias_radps[2] = 0.0F;

    // '<S7>:1:20' imuM2 = [0; 0; 0; 0; 0; 0];
    stateEstimatorEskf_DW.imuM2[0] = 0.0F;
    stateEstimatorEskf_DW.imuM2[1] = 0.0F;
    stateEstimatorEskf_DW.imuM2[2] = 0.0F;
    stateEstimatorEskf_DW.imuM2[3] = 0.0F;
    stateEstimatorEskf_DW.imuM2[4] = 0.0F;
    stateEstimatorEskf_DW.imuM2[5] = 0.0F;

    // '<S7>:1:21' initialQuat = [1; 0; 0; 0];
    stateEstimatorEskf_DW.initialQuat[0] = 1.0F;
    stateEstimatorEskf_DW.initialQuat[1] = 0.0F;
    stateEstimatorEskf_DW.initialQuat[2] = 0.0F;
    stateEstimatorEskf_DW.initialQuat[3] = 0.0F;

    // '<S7>:1:22' isAttInitialized = false;
    stateEstimatorEskf_DW.isAttInitialized = false;

    //  Index to keep track of how many mag readings we have summed
    //  so far
    // '<S7>:1:25' magIdx = 0;
    stateEstimatorEskf_DW.magIdx = 0.0F;

    // '<S7>:1:26' magMean = [0; 0; 0];
    // '<S7>:1:27' magM2 = [0; 0; 0];
    // '<S7>:1:28' magBias_nd = [0; 0; 0];
    // '<S7>:1:29' nedMagVecNorm_nd = [0; 0; 0];
    //  Index to keep track of how many gps readings we have summed
    //  so far
    // '<S7>:1:32' gpsIdx = 0;
    stateEstimatorEskf_DW.gpsIdx = 0.0;

    // '<S7>:1:33' refLatLonAlt = [0; 0; 0];
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

    // '<S7>:1:34' isPosInitialized = false;
    stateEstimatorEskf_DW.isPosInitialized = false;

    //  Index to keep track of how many baro readings we have summed
    //  so far
    // '<S7>:1:37' baroIdx = 0;
    stateEstimatorEskf_DW.baroIdx = 0.0F;

    // '<S7>:1:38' baroInitAltMean = 0;
    stateEstimatorEskf_DW.baroInitAltMean = 0.0F;

    // '<S7>:1:39' baroInitAltM2 = 0;
    stateEstimatorEskf_DW.baroInitAltM2 = 0.0F;

    // '<S7>:1:40' baroBias_m = 0;
    stateEstimatorEskf_DW.baroBias_m = 0.0F;

    // '<S7>:1:41' isBaroInitialized = false;
    stateEstimatorEskf_DW.isBaroInitialized = false;

    // End of entry stage
  } else {
    switch (stateEstimatorEskf_DW.is_c3_stateEstimatorEskf) {
     case stateEstimatorEsk_IN_INITIALIZE:
      stateEstimatorEskf_INITIALIZE(&mode, latLonAltOut, VectorConcatenate,
        &rtb_Product2_c, Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData,
        rtu_stateEstSmParams);
      break;

     case stateEstimatorEskf_IN_RUN:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN;

      // During 'RUN': '<S7>:43'
      // '<S7>:68:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
      // '<S7>:68:2' stateEstSmParams.gpsLossCheckDuration_s);
      if (rtu_gpsData->isGpsDataValid) {
        stateEstimatorEskf_DW.durationCounter_1 = 0U;
      }

      if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1) >=
          rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
        // Transition: '<S7>:68'
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_GPS_LOST;

        // Entry 'RUN_GPS_LOST': '<S7>:67'
        // GPS WAS LOST
        // '<S7>:67:4' gpsValidCount = 0;
        stateEstimatorEskf_DW.gpsValidCount = 0U;

        // '<S7>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
        mode = enumStateEstimateMode::RUN_GPS_LOST;

        // '<S7>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S7>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S7>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S7>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S7>:67:10' latLonAltOut = latLonAltIn;
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

        // '<S7>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S7>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S7>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // FULL EKF WITH GPS IS RUNNING
        // '<S7>:43:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S7>:43:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S7>:43:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S7>:43:19' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S7>:43:20' latLonAltOut = latLonAltIn;
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

        // '<S7>:43:21' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S7>:43:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S7>:43:23' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;
      }
      break;

     case stateEstimatorE_IN_RUN_GPS_LOST:
      mode = enumStateEstimateMode::RUN_GPS_LOST;

      // During 'RUN_GPS_LOST': '<S7>:67'
      // '<S7>:71:1' sf_internal_predicateOutput = 0 | (gpsValidCount >= stateEstSmParams.desValidGpsCount); 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S7>:71'
        stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_Product2_c,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // GPS WAS LOST
        // '<S7>:67:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S7>:67:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S7>:67:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S7>:67:19' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S7>:67:20' latLonAltOut = latLonAltIn;
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

        // '<S7>:67:21' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S7>:67:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S7>:67:23' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
        // '<S7>:67:25' if(isGpsDataValid)
        if (rtu_gpsData->isGpsDataValid) {
          // '<S7>:67:26' gpsValidCount = gpsValidCount + 1;
          stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
            (stateEstimatorEskf_DW.gpsValidCount + 1);
        }
      }
      break;

     case stateEstima_IN_RUN_GPS_NOT_INIT:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN_GPS_NOT_INIT;

      // During 'RUN_GPS_NOT_INIT': '<S7>:62'
      // '<S7>:65:1' sf_internal_predicateOutput = 0 | (gpsValidCount >= stateEstSmParams.desValidGpsCount); 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S7>:65'
        stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_Product2_c,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // '<S7>:73:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
        // '<S7>:73:2' stateEstSmParams.gpsLossCheckDuration_s);
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_n = 0U;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_n) >=
            rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
          // Transition: '<S7>:73'
          stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
            stateEstimatorE_IN_RUN_GPS_LOST;

          // Entry 'RUN_GPS_LOST': '<S7>:67'
          // GPS WAS LOST
          // '<S7>:67:4' gpsValidCount = 0;
          stateEstimatorEskf_DW.gpsValidCount = 0U;

          // '<S7>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
          mode = enumStateEstimateMode::RUN_GPS_LOST;

          // '<S7>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
          // '<S7>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
          // '<S7>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S7>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S7>:67:10' latLonAltOut = latLonAltIn;
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

          // '<S7>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S7>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S7>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF STARTED RUNNING WITHOUT GPS
          // '<S7>:62:18' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S7>:62:19' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S7>:62:20' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S7>:62:21' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S7>:62:22' latLonAltOut = latLonAltIn;
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

          // '<S7>:62:23' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S7>:62:24' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S7>:62:25' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
          // '<S7>:62:27' if(isGpsDataValid)
          if (rtu_gpsData->isGpsDataValid) {
            // '<S7>:62:28' gpsValidCount = gpsValidCount + 1;
            stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
              (stateEstimatorEskf_DW.gpsValidCount + 1);
          }
        }
      }
      break;

     default:
      mode = enumStateEstimateMode::RUN_INIT_GPS;

      // During 'RUN_INIT_GPS': '<S7>:64'
      // '<S7>:66:1' sf_internal_predicateOutput = 0 | (isPosInitialized);
      if (stateEstimatorEskf_DW.isPosInitialized) {
        // Transition: '<S7>:66'
        stateEstimatorEskf_DW.durationCounter_1 = 0U;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorEskf_IN_RUN;

        // Entry 'RUN': '<S7>:43'
        // FULL EKF WITH GPS IS RUNNING
        // '<S7>:43:4' resetStates = false;
        stateEstimatorEskf_DW.resetStates = false;

        // '<S7>:43:5' mode = enumStateEstimateMode.RUN;
        mode = enumStateEstimateMode::RUN;

        // '<S7>:43:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S7>:43:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S7>:43:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S7>:43:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
          rtu_magData->isMagDataValid;

        // '<S7>:43:10' latLonAltOut = latLonAltIn;
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

        // '<S7>:43:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S7>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S7>:43:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
        stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // '<S7>:70:1' sf_internal_predicateOutput = 0 | (duration(~isGpsDataValid) >=  ... 
        // '<S7>:70:2' stateEstSmParams.gpsLossCheckDuration_s);
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_g = 0U;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_g) >=
            rtu_stateEstSmParams->gpsLossCheckDuration_s * 250.0F) {
          // Transition: '<S7>:70'
          stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
            stateEstimatorE_IN_RUN_GPS_LOST;

          // Entry 'RUN_GPS_LOST': '<S7>:67'
          // GPS WAS LOST
          // '<S7>:67:4' gpsValidCount = 0;
          stateEstimatorEskf_DW.gpsValidCount = 0U;

          // '<S7>:67:5' mode = enumStateEstimateMode.RUN_GPS_LOST;
          mode = enumStateEstimateMode::RUN_GPS_LOST;

          // '<S7>:67:6' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
          // '<S7>:67:7' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
          // '<S7>:67:8' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S7>:67:9' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S7>:67:10' latLonAltOut = latLonAltIn;
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

          // '<S7>:67:11' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S7>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S7>:67:13' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF RUNNING WITHOUT GPS BUT WE HAVE HEALTHY GPS SIGNAL
          // START INITIALIZING GPS
          // '<S7>:64:22' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S7>:64:23' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S7>:64:24' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S7>:64:25' sensorDataOut.sensorValidity.isMagValid = isMagDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid =
            rtu_magData->isMagDataValid;

          // '<S7>:64:26' latLonAltOut = latLonAltIn;
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

          // '<S7>:64:27' sensorDataOut.sensorValidity.isGpsValid = isGpsDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S7>:64:28' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_Product2_c -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S7>:64:29' sensorDataOut.sensorValidity.isBaroValid = isBaroDataValid; 
          stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
          // Compute running mean of GPS data for NED origin Lat, Lon and Alt
          // '<S7>:64:32' if (isGpsDataValid && gpsAcc_m(1) >= 0 && ...
          // '<S7>:64:33'         gpsAcc_m(1) <= stateEstSmParams.gpsHorAccThres_m && ... 
          // '<S7>:64:34'         gpsAcc_m(2) >= 0 && ...
          // '<S7>:64:35'         gpsAcc_m(2) <= stateEstSmParams.gpsVerAccThres_m) 
          if (rtu_gpsData->isGpsDataValid && (rtu_gpsData->hacc_m >= 0.0F) &&
              (rtu_gpsData->hacc_m <= rtu_stateEstSmParams->gpsHorAccThres_m) &&
              (rtu_gpsData->vacc_m >= 0.0F) && (rtu_gpsData->vacc_m <=
               rtu_stateEstSmParams->gpsVerAccThres_m)) {
            // '<S7>:64:36' if( gpsIdx < max(stateEstSmParams.gpsInitCount, 1) ) 
            if (stateEstimatorEskf_DW.gpsIdx < std::fmax
                (rtu_stateEstSmParams->gpsInitCount, 1.0F)) {
              // '<S7>:64:37' gpsIdx = gpsIdx + 1;
              stateEstimatorEskf_DW.gpsIdx++;

              // '<S7>:64:38' llhDelta = (latLonAltIn - refLatLonAlt);
              // '<S7>:64:39' refLatLonAlt = refLatLonAlt + llhDelta/gpsIdx;
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
            // '<S7>:64:42' if(gpsIdx >= stateEstSmParams.gpsInitCount)
            if (stateEstimatorEskf_DW.gpsIdx >=
                rtu_stateEstSmParams->gpsInitCount) {
              // '<S7>:64:43' isPosInitialized = true;
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

  // MATLAB Function: '<S12>/MATLAB Function' incorporates:
  //   UnitDelay: '<Root>/Unit Delay1'

  // MATLAB Function 'opticalFlowToNeVel/MATLAB Function': '<S73>:1'
  // '<S73>:1:2' agl_m = lidarRangeToAgl_function( ...
  // '<S73>:1:3'     lidarRange_m, roll_rad, pitch_rad, xL_m, yL_m, zL_m);
  // CORRECTLIDARRANGETOBODYAGLFAST_FUNCTION
  //  Fast scalar version for fixed body-frame lidar lever arm.
  //
  //  Body frame:
  //    x forward
  //    y right
  //    z down
  //
  //  Lidar:
  //    beam aligned with body +Z/down
  // 'lidarRangeToAgl_function:14' sinRoll_rad = sin(roll_rad);
  // 'lidarRangeToAgl_function:15' cosRoll_rad = cos(roll_rad);
  // 'lidarRangeToAgl_function:16' sinPitch_rad = sin(pitch_rad);
  // 'lidarRangeToAgl_function:17' cosPitch_rad = cos(pitch_rad);
  rtb_UnitDelay_g = std::cos(stateEstimatorEskf_DW.UnitDelay1_DSTATE[1]);

  // MATLAB Function: '<S12>/getMtf01pValidity'
  // 'lidarRangeToAgl_function:19' agl_m = ...
  // 'lidarRangeToAgl_function:20'     -sinPitch_rad*xL_m + ...
  // 'lidarRangeToAgl_function:21'      sinRoll_rad*cosPitch_rad*yL_m + ...
  // 'lidarRangeToAgl_function:22'      cosRoll_rad*cosPitch_rad*(zL_m + lidarRange_m); 
  // This function generates a validity flag based to inform EKF when to not
  // use optical flow data
  // MATLAB Function 'opticalFlowToNeVel/getMtf01pValidity': '<S75>:1'
  // Update MTF01P distance validity
  // '<S75>:1:6' mtf01pDistValidityFlag = (mtf01pData.distPrecision <= mtf01pParams.distPrecisionThr) & ... 
  // '<S75>:1:7'     (mtf01pData.distStatus == 1) & (mtf01pData.dist_m > mtf01pParams.distLimit_m(1)) & (mtf01pData.dist_m < mtf01pParams.distLimit_m(2)); 
  mtf01pDistValidityFlag = static_cast<boolean_T>(static_cast<boolean_T>
    ((rtu_mtf01pData->distPrecision <= rtu_mtf01pParams->distPrecisionThr) &
     (rtu_mtf01pData->distStatus == 1)) & (rtu_mtf01pData->dist_m >
    rtu_mtf01pParams->distLimit_m[0])) & (rtu_mtf01pData->dist_m <
    rtu_mtf01pParams->distLimit_m[1]);

  // RelationalOperator: '<S67>/Compare' incorporates:
  //   Constant: '<S67>/Constant'

  // Update flag based on flow health
  // '<S75>:1:10' mtf01pValidityFlag = mtf01pDistValidityFlag & (mtf01pData.flowStatus == 1); 
  rtb_Compare = (mode == enumStateEstimateMode::RUN);

  // MATLAB Function: '<S9>/convertLlhToNedPos'
  // MATLAB Function 'latLonAltToNedPos/convertLlhToNedPos': '<S69>:1'
  // '<S69>:1:3' nedPos_m = convertLlhToNedPos_function(latLonAlt, refLatLonAlt, isGpsValid); 
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

  // End of MATLAB Function: '<S9>/convertLlhToNedPos'

  // Sum: '<S12>/Sum2' incorporates:
  //   Sum: '<S72>/Sum'
  //   UnitDelay: '<Root>/Unit Delay'

  rtb_Product3_l = rtb_XAxis - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[10];
  rtb_Sum_m1_idx_1 = rtb_XAxis1 - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[11];
  rtb_Sum_m1_idx_2 = rtb_XAxis2 - stateEstimatorEskf_DW.UnitDelay_DSTATE_e[12];

  // Sum: '<S12>/Sum4' incorporates:
  //   Product: '<S12>/Matrix Multiply'
  //   Product: '<S12>/Product'
  //   Product: '<S72>/Element Product'
  //   SignalConversion generated from: '<S12>/Vector Concatenate'
  //   Sum: '<S12>/Sum'
  //   Sum: '<S12>/Sum1'
  //   Sum: '<S72>/Sum'
  //
  rtb_Product2_c = ((rtu_mtf01pParams->sensorToBodyRot[0] *
                     rtu_mtf01pData->flowX_radps +
                     rtu_mtf01pParams->sensorToBodyRot[2] *
                     rtu_mtf01pData->flowY_radps) - rtb_Sum_m1_idx_1) *
    rtu_mtf01pData->dist_m + (rtb_Sum_m1_idx_1 * rtu_mtf01pParams->posVector_m[2]
    - rtu_mtf01pParams->posVector_m[1] * rtb_Sum_m1_idx_2);
  rtb_XAxis = ((rtu_mtf01pParams->sensorToBodyRot[1] *
                rtu_mtf01pData->flowX_radps + rtu_mtf01pParams->sensorToBodyRot
                [3] * rtu_mtf01pData->flowY_radps) + rtb_Product3_l) *
    rtu_mtf01pData->dist_m + (rtu_mtf01pParams->posVector_m[0] *
    rtb_Sum_m1_idx_2 - rtb_Product3_l * rtu_mtf01pParams->posVector_m[2]);
  rtb_Product3_l = rtb_Product3_l * rtu_mtf01pParams->posVector_m[1] -
    rtu_mtf01pParams->posVector_m[0] * rtb_Sum_m1_idx_1;

  // MATLAB Function: '<S77>/Compute Filter Numerator And Denominator' incorporates:
  //   MATLAB Function: '<S76>/Compute Filter Numerator And Denominator'

  //  Call the main function
  // MATLAB Function 'Discrete First Order Filter/Compute Filter Numerator And Denominator': '<S79>:1' 
  // '<S79>:1:4' [num, den] = computeFirstOrderFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
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
  rtb_XAxis1 = (rtu_mtf01pParams->filterBw_radps - 500.0F) /
    (rtu_mtf01pParams->filterBw_radps + 500.0F);

  // DiscreteTransferFcn: '<S77>/Discrete Transfer Fcn' incorporates:
  //   MATLAB Function: '<S77>/Compute Filter Numerator And Denominator'
  //   Product: '<S12>/Matrix Multiply1'
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.DiscreteTransferFcn_tmp =
    ((stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] * rtb_Product2_c +
      stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] * rtb_XAxis) +
     stateEstimatorEskf_DW.UnitDelay2_DSTATE[6] * rtb_Product3_l) - rtb_XAxis1 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;

  // MATLAB Function: '<S77>/Compute Filter Numerator And Denominator' incorporates:
  //   MATLAB Function: '<S76>/Compute Filter Numerator And Denominator'

  rtb_XAxis2 = rtu_mtf01pParams->filterBw_radps /
    (rtu_mtf01pParams->filterBw_radps + 500.0F);

  // DiscreteTransferFcn: '<S77>/Discrete Transfer Fcn' incorporates:
  //   MATLAB Function: '<S77>/Compute Filter Numerator And Denominator'

  VectorConcatenate[0] = rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp + rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;

  // DiscreteTransferFcn: '<S76>/Discrete Transfer Fcn' incorporates:
  //   Product: '<S12>/Matrix Multiply1'
  //   UnitDelay: '<Root>/Unit Delay2'

  //  Call the main function
  // MATLAB Function 'Discrete First Order Filter/Compute Filter Numerator And Denominator': '<S78>:1' 
  // '<S78>:1:4' [num, den] = computeFirstOrderFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
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
    ((stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] * rtb_Product2_c +
      stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] * rtb_XAxis) +
     stateEstimatorEskf_DW.UnitDelay2_DSTATE[7] * rtb_Product3_l) - rtb_XAxis1 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;
  VectorConcatenate[1] = rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b + rtb_XAxis2 *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;

  // MATLAB Function: '<S15>/DelayedHorizonBufferManager' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   BusCreator: '<Root>/Bus Creator1'
  //   Concatenate: '<S74>/Vector Concatenate'
  //   DataTypeConversion: '<S9>/Cast To Single'
  //   Logic: '<S12>/AND'
  //   MATLAB Function: '<S12>/MATLAB Function'
  //   MATLAB Function: '<S12>/getMtf01pValidity'
  //   Sum: '<S9>/Sum'
  //   UnitDelay: '<Root>/Unit Delay1'
  //   UnitDelay: '<S9>/Unit Delay'

  // MATLAB Function 'EKF/DelayedHorizonPassthrough/DelayedHorizonBufferManager': '<S19>:1' 
  // '<S19>:1:2' [dhSensorIn, estSmModeOut, dhBufferDebug] = delayedHorizonBufferManager_function(sensorIn, estSmMode, reset, ekfParams); 
  // DELAYEDHORIZONBUFFERMANAGER manages sensor buffers so the ESKF operates
  // at the delayed IMU horizon.
  // 'delayedHorizonBufferManager_function:7' dhSensorIn = sensorIn;
  // 'delayedHorizonBufferManager_function:8' dhBufferDebug = struct;
  // 'delayedHorizonBufferManager_function:10' [stateModeReady, delayedEstSmMode, count] = ... 
  // 'delayedHorizonBufferManager_function:11'     stateFifo(estSmMode, ekfParams.stateFifoParams); 
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
  estSmModeOut = mode;

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
  if (stateEstimatorEskf_DW.count <= 50) {
    // 'stateFifo:179' countOut = count;
    stateEstimatorEskf_DW.dhBufferDebug.statusFifoDebugData.count =
      stateEstimatorEskf_DW.count;
  } else {
    //  ------------------------------------------------------------------------- 
    //  POP GPS-DELAYED ESTIMATOR MODE
    //  ------------------------------------------------------------------------- 
    //  Tail is the estimator mode corresponding to the GPS-delayed epoch.
    // 'stateFifo:188' delayedMode = modeBuf(tail);
    estSmModeOut = stateEstimatorEskf_DW.modeBuf[stateEstimatorEskf_DW.tail - 1];

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
    stateEstimatorEskf_DW.dhBufferDebug.statusFifoDebugData.count =
      stateEstimatorEskf_DW.count;
  }

  // 'delayedHorizonBufferManager_function:13' dhBufferDebug.statusFifoDebugData.status = enumDhFifoStatus.OK; 
  stateEstimatorEskf_DW.dhBufferDebug.statusFifoDebugData.status =
    enumDhFifoStatus::OK;

  // 'delayedHorizonBufferManager_function:14' dhBufferDebug.statusFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:16' if stateModeReady
  if (static_cast<boolean_T>(static_cast<int32_T>(stateModeReady) ^ 1)) {
    // 'delayedHorizonBufferManager_function:18' else
    // 'delayedHorizonBufferManager_function:19' estSmModeOut = estSmMode;
    estSmModeOut = mode;
  } else {
    // 'delayedHorizonBufferManager_function:17' estSmModeOut = delayedEstSmMode; 
  }

  // 'delayedHorizonBufferManager_function:22' [imuReady, imuTimeOut_ms, imuOut, status, count] = ... 
  // 'delayedHorizonBufferManager_function:23'     imuFifo(sensorIn.sensorValidity.isImuValid, ... 
  // 'delayedHorizonBufferManager_function:24'             sensorIn.sensorTimestamp.imuTimestamp_ms, ... 
  // 'delayedHorizonBufferManager_function:25'             [sensorIn.bodyAccels_mps2; ... 
  // 'delayedHorizonBufferManager_function:26'              sensorIn.bodyRates_radps; ... 
  // 'delayedHorizonBufferManager_function:27'              sensorIn.dtImuTime_s], ... 
  // 'delayedHorizonBufferManager_function:28'             ekfParams.imuFifoParams); 
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
  //  Static FIFO allocation/state initialization.
  // 'imuFifo:84' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. imuReady still indicates whether this call returned a new sample.
  // 'imuFifo:99' imuReady = false;
  imuReady = false;

  // 'imuFifo:100' imuTimeOut_ms = lastImuTimeOut_ms;
  fusionTime_ms = stateEstimatorEskf_DW.lastImuTimeOut_ms;

  // 'imuFifo:101' imuOut = lastImuOut;
  // 'imuFifo:102' status = enumDhFifoStatus.OK;
  stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.status = enumDhFifoStatus::
    OK;

  // 'imuFifo:103' countOut = count;
  stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.count =
    stateEstimatorEskf_DW.count_g;

  //  Read-only configuration aliases.
  // 'imuFifo:106' imuPeriod_ms = imuFifoParams.imuPeriod_ms;
  // 'imuFifo:107' delaySamples = imuFifoParams.delaySamples;
  //  imuPeriod_ms is supplied as a system configuration parameter. It is not
  //  required by the FIFO indexing itself, but retaining it here keeps the
  //  configured temporal horizon explicit:
  //
  //    delayHorizon_ms = delaySamples * imuPeriod_ms
  //
  //  A delay of N intervals requires storage for N+1 timestamped samples
  //  before the oldest sample can first be released.
  // 'imuFifo:117' if imuPeriod_ms == 0 || delaySamples >= MAX_SIZE
  if (uMultiWordEq
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.imuFifoParams.imuPeriod_ms.chunks
       [0], &dhSensorIn_sensorTimestamp_ma_0.chunks[0U], 2)) {
    // 'imuFifo:118' status = enumDhFifoStatus.BAD_CONFIG;
    stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.status =
      enumDhFifoStatus::BAD_CONFIG;

    //  No new IMU measurement this estimator step.
    // 'imuFifo:123' if ~isImuValid
  } else if (rtu_imuData->isImuDataValid) {
    //  ------------------------------------------------------------------------- 
    //  PUSH CURRENT-TIME IMU MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'imuFifo:131' bufferWasFull = (count == MAX_SIZE);
    //  Store complete IMU observation at current write location.
    // 'imuFifo:134' timeBuf_ms(head) = imuTimeIn_ms;
    stateEstimatorEskf_DW.timeBuf_ms[stateEstimatorEskf_DW.head_e - 1] =
      rtu_imuData->timestamp_ms;

    // 'imuFifo:135' dataBuf(:,head) = imuIn;
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
    // 'imuFifo:138' if head == MAX_SIZE
    if (stateEstimatorEskf_DW.head_e == 64) {
      // 'imuFifo:139' head = uint16(1);
      stateEstimatorEskf_DW.head_e = 1U;
    } else {
      // 'imuFifo:140' else
      // 'imuFifo:141' head = head + 1;
      qY = stateEstimatorEskf_DW.head_e + 1U;
      if (stateEstimatorEskf_DW.head_e + 1U > 65535U) {
        qY = 65535U;
      }

      stateEstimatorEskf_DW.head_e = static_cast<uint16_T>(qY);
    }

    // 'imuFifo:144' if bufferWasFull
    if (stateEstimatorEskf_DW.count_g == 64) {
      //  New sample replaced the oldest stored IMU sample.
      //
      //  Under correctly sized steady-state operation this should not occur
      //  before the oldest sample has already been consumed.
      // 'imuFifo:149' if tail == MAX_SIZE
      if (stateEstimatorEskf_DW.tail_g == 64) {
        // 'imuFifo:150' tail = uint16(1);
        stateEstimatorEskf_DW.tail_g = 1U;
      } else {
        // 'imuFifo:151' else
        // 'imuFifo:152' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_g + 1U;
        if (stateEstimatorEskf_DW.tail_g + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_g = static_cast<uint16_T>(qY);
      }

      // 'imuFifo:155' count = uint16(MAX_SIZE);
      // 'imuFifo:156' status = enumDhFifoStatus.OVERWRITE;
      stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.status =
        enumDhFifoStatus::OVERWRITE;
    } else {
      // 'imuFifo:157' else
      // 'imuFifo:158' count = count + 1;
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
    // 'imuFifo:179' if count <= delaySamples
    if (stateEstimatorEskf_DW.count_g <= 50) {
      // 'imuFifo:180' countOut = count;
      stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.count =
        stateEstimatorEskf_DW.count_g;
    } else {
      //  ------------------------------------------------------------------------- 
      //  POP OLDEST DELAYED IMU MEASUREMENT
      //  ------------------------------------------------------------------------- 
      //  Tail points to the oldest valid IMU measurement.
      // 'imuFifo:189' imuTimeOut_ms = timeBuf_ms(tail);
      fusionTime_ms =
        stateEstimatorEskf_DW.timeBuf_ms[stateEstimatorEskf_DW.tail_g - 1];

      // 'imuFifo:190' imuOut = dataBuf(:,tail);
      rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.tail_g - 1) * 7;
      stateEstimatorEskf_DW.lastImuOut[0] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp];
      stateEstimatorEskf_DW.lastImuOut[1] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 1];
      stateEstimatorEskf_DW.lastImuOut[2] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 2];
      stateEstimatorEskf_DW.lastImuOut[3] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 3];
      stateEstimatorEskf_DW.lastImuOut[4] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 4];
      stateEstimatorEskf_DW.lastImuOut[5] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 5];
      stateEstimatorEskf_DW.lastImuOut[6] =
        stateEstimatorEskf_DW.dataBuf[rtb_VectorConcatenate1_tmp + 6];

      // 'imuFifo:191' imuReady = true;
      imuReady = true;

      //  Hold last returned measurement for future calls where imuReady is false. 
      // 'imuFifo:194' lastImuTimeOut_ms = imuTimeOut_ms;
      stateEstimatorEskf_DW.lastImuTimeOut_ms = fusionTime_ms;

      // 'imuFifo:195' lastImuOut = imuOut;
      //  Consume returned measurement.
      // 'imuFifo:198' if tail == MAX_SIZE
      if (stateEstimatorEskf_DW.tail_g == 64) {
        // 'imuFifo:199' tail = uint16(1);
        stateEstimatorEskf_DW.tail_g = 1U;
      } else {
        // 'imuFifo:200' else
        // 'imuFifo:201' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_g + 1U;
        if (stateEstimatorEskf_DW.tail_g + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_g = static_cast<uint16_T>(qY);
      }

      // 'imuFifo:204' count = count - 1;
      qY = stateEstimatorEskf_DW.count_g -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_g - 1U > stateEstimatorEskf_DW.count_g) {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_g = static_cast<uint16_T>(qY);

      // 'imuFifo:205' countOut = count;
      stateEstimatorEskf_DW.dhBufferDebug.imuFifoDebugData.count =
        stateEstimatorEskf_DW.count_g;
    }
  }

  dhSensorIn_sensorTimestamp_imuT = fusionTime_ms;

  // 'delayedHorizonBufferManager_function:30' dhBufferDebug.imuFifoDebugData.status = status; 
  // 'delayedHorizonBufferManager_function:31' dhBufferDebug.imuFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:33' dhSensorIn.sensorValidity.isImuValid = imuReady; 
  // 'delayedHorizonBufferManager_function:34' dhSensorIn.sensorTimestamp.imuTimestamp_ms = imuTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:35' dhSensorIn.bodyAccels_mps2 = imuOut(1:3); 
  // 'delayedHorizonBufferManager_function:36' dhSensorIn.bodyRates_radps = imuOut(4:6); 
  // 'delayedHorizonBufferManager_function:37' dhSensorIn.dtImuTime_s = imuOut(7); 
  // 'delayedHorizonBufferManager_function:39' if imuReady
  if (static_cast<boolean_T>(static_cast<int32_T>(imuReady) ^ 1)) {
    // 'delayedHorizonBufferManager_function:41' else
    // 'delayedHorizonBufferManager_function:42' fusionTime_ms = uint64(0);
    fusionTime_ms = dhSensorIn_sensorTimestamp_ma_0;
  } else {
    // 'delayedHorizonBufferManager_function:40' fusionTime_ms = imuTimeOut_ms;
  }

  // 'delayedHorizonBufferManager_function:45' magDelay_ms   = ekfParams.magDelay_ms; 
  // 'delayedHorizonBufferManager_function:46' gpsDelay_ms   = ekfParams.gpsDelay_ms; 
  // 'delayedHorizonBufferManager_function:47' baroDelay_ms  = ekfParams.baroDelay_ms; 
  // 'delayedHorizonBufferManager_function:48' lidarDelay_ms = ekfParams.lidarDelay_ms; 
  // 'delayedHorizonBufferManager_function:49' ofDelay_ms    = ekfParams.ofDelay_ms; 
  // 'delayedHorizonBufferManager_function:51' if sensorIn.sensorTimestamp.magTimestamp_ms >= magDelay_ms 
  if (uMultiWordGe(&rtu_magData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:52' magTimeIn_ms = sensorIn.sensorTimestamp.magTimestamp_ms - magDelay_ms; 
    uMultiWord2MultiWord(&rtu_magData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &magTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:53' else
    // 'delayedHorizonBufferManager_function:54' magTimeIn_ms = uint64(0);
    magTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:57' if sensorIn.sensorTimestamp.gpsTimestamp_ms >= gpsDelay_ms 
  if (uMultiWordGe(&rtu_gpsData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:58' gpsTimeIn_ms = sensorIn.sensorTimestamp.gpsTimestamp_ms - gpsDelay_ms; 
    uMultiWord2MultiWord(&rtu_gpsData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.gpsDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &gpsTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:59' else
    // 'delayedHorizonBufferManager_function:60' gpsTimeIn_ms = uint64(0);
    gpsTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:63' if sensorIn.sensorTimestamp.baroTimestamp_ms >= baroDelay_ms 
  if (uMultiWordGe(&rtu_baroData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:64' baroTimeIn_ms = sensorIn.sensorTimestamp.baroTimestamp_ms - baroDelay_ms; 
    uMultiWord2MultiWord(&rtu_baroData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &baroTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:65' else
    // 'delayedHorizonBufferManager_function:66' baroTimeIn_ms = uint64(0);
    baroTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:69' if sensorIn.sensorTimestamp.lidarTimestamp_ms >= lidarDelay_ms 
  if (uMultiWordGe(&rtu_mtf01pData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:70' lidarTimeIn_ms = sensorIn.sensorTimestamp.lidarTimestamp_ms - lidarDelay_ms; 
    uMultiWord2MultiWord(&rtu_mtf01pData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &lidarTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:71' else
    // 'delayedHorizonBufferManager_function:72' lidarTimeIn_ms = uint64(0);
    lidarTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:75' if sensorIn.sensorTimestamp.ofTimestamp_ms >= ofDelay_ms 
  if (uMultiWordGe(&rtu_mtf01pData->timestamp_ms.chunks[0U],
                   &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.ofDelay_ms.chunks
                   [0], 2)) {
    // 'delayedHorizonBufferManager_function:76' ofTimeIn_ms = sensorIn.sensorTimestamp.ofTimestamp_ms - ofDelay_ms; 
    uMultiWord2MultiWord(&rtu_mtf01pData->timestamp_ms.chunks[0U], 2,
                         &tmp_7.chunks[0U], 3);
    uMultiWord2MultiWord
      (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.ofDelay_ms.chunks
       [0], 2, &tmp_8.chunks[0U], 3);
    MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
    sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &ofTimeIn_ms.chunks[0U], 2);
  } else {
    // 'delayedHorizonBufferManager_function:77' else
    // 'delayedHorizonBufferManager_function:78' ofTimeIn_ms = uint64(0);
    ofTimeIn_ms = dhSensorIn_sensorTimestamp_ma_0;
  }

  // 'delayedHorizonBufferManager_function:81' [magReady, magTimeOut_ms, magOut_uT, status, count] = ... 
  // 'delayedHorizonBufferManager_function:82'     magFifo(sensorIn.sensorValidity.isMagValid, ... 
  // 'delayedHorizonBufferManager_function:83'             magTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:84'             sensorIn.normMagVec_nd, ... 
  // 'delayedHorizonBufferManager_function:85'             fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:86'             ekfParams.magFifoParams); 
  stateEstimatorEskf_magFifo
    (stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isMagValid, magTimeIn_ms,
     stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd, fusionTime_ms,
     stateEstimatorEskf_DW.resetStates,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.capacity,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.minInterval_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.maxAge_ms,
     stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.magFifoParams.resetThreshold_ms,
     &dhSensorIn_sensorValidity_isMag, &dhSensorIn_sensorTimestamp_magT, Divide,
     &stateEstimatorEskf_DW.dhBufferDebug.magFifoDebugData.status,
     &stateEstimatorEskf_DW.dhBufferDebug.magFifoDebugData.count);

  // 'delayedHorizonBufferManager_function:88' dhBufferDebug.magFifoDebugData.status = status; 
  // 'delayedHorizonBufferManager_function:89' dhBufferDebug.magFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:91' dhSensorIn.sensorValidity.isMagValid = magReady; 
  // 'delayedHorizonBufferManager_function:92' dhSensorIn.sensorTimestamp.magTimestamp_ms = magTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:93' dhSensorIn.normMagVec_nd = magOut_uT; 
  // 'delayedHorizonBufferManager_function:95' [gpsReady, gpsTimeOut_ms, posVelOut, status, count] = ... 
  // 'delayedHorizonBufferManager_function:96'     gpsFifo(sensorIn.sensorValidity.isGpsValid, ... 
  // 'delayedHorizonBufferManager_function:97'             gpsTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:98'             [sensorIn.nedPosAndVel.pos_m; ... 
  // 'delayedHorizonBufferManager_function:99'              sensorIn.nedPosAndVel.vel_mps], ... 
  // 'delayedHorizonBufferManager_function:100'             fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:101'             ekfParams.gpsFifoParams); 
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
     &dhSensorIn_sensorValidity_isGps, &magTimeIn_ms, rtb_ElementProduct,
     &stateEstimatorEskf_DW.dhBufferDebug.gpsFifoDebugData.status,
     &stateEstimatorEskf_DW.dhBufferDebug.gpsFifoDebugData.count);

  // 'delayedHorizonBufferManager_function:103' dhBufferDebug.gpsFifoDebugData.status = status; 
  // 'delayedHorizonBufferManager_function:104' dhBufferDebug.gpsFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:106' dhSensorIn.sensorValidity.isGpsValid = gpsReady; 
  // 'delayedHorizonBufferManager_function:107' dhSensorIn.sensorTimestamp.gpsTimestamp_ms = gpsTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:108' dhSensorIn.nedPosAndVel.pos_m = posVelOut(1:3); 
  // 'delayedHorizonBufferManager_function:109' dhSensorIn.nedPosAndVel.vel_mps = posVelOut(4:6); 
  // 'delayedHorizonBufferManager_function:111' [baroReady, baroTimeOut_ms, baroAltOut_m, status, count] = ... 
  // 'delayedHorizonBufferManager_function:112'     baroFifo(sensorIn.sensorValidity.isBaroValid, ... 
  // 'delayedHorizonBufferManager_function:113'              baroTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:114'              sensorIn.baroAlt_m, ... 
  // 'delayedHorizonBufferManager_function:115'              fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:116'              ekfParams.baroFifoParams); 
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
  //  Static FIFO storage/state initialization.
  // 'baroFifo:55' if isempty(head)
  //  Default outputs.
  //  Hold the most recently returned measurement when no new measurement is
  //  ready. baroReady still indicates whether this call returned a new sample.
  // 'baroFifo:71' baroReady = false;
  stateModeReady = false;

  // 'baroFifo:72' baroTimeOut_ms = lastBaroTimeOut_ms;
  // 'baroFifo:73' altitudeOut_m = lastAltitudeOut_m;
  rtb_Product3_l = stateEstimatorEskf_DW.lastAltitudeOut_m;

  // 'baroFifo:74' status = enumDhFifoStatus.OK;
  status = enumDhFifoStatus::OK;

  // 'baroFifo:75' countOut = count;
  stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.count =
    stateEstimatorEskf_DW.count_d1;

  //  Read-only configuration aliases.
  // 'baroFifo:78' capacity = baroFifoParams.capacity;
  // 'baroFifo:79' maxAge_ms = baroFifoParams.maxAge_ms;
  // 'baroFifo:80' minObsInterval_ms = baroFifoParams.minInterval_ms;
  // 'baroFifo:81' timeResetThreshold_ms = baroFifoParams.resetThreshold_ms;
  //  Prevent invalid circular-buffer indexing if configuration is bad.
  // 'baroFifo:84' if capacity < 1 || capacity > MAX_SIZE
  //  Explicit estimator/FIFO reset.
  // 'baroFifo:90' if reset
  if (stateEstimatorEskf_DW.resetStates) {
    // 'baroFifo:91' head = uint16(1);
    stateEstimatorEskf_DW.head_f = 1U;

    // 'baroFifo:92' tail = uint16(1);
    stateEstimatorEskf_DW.tail_p = 1U;

    // 'baroFifo:93' count = uint16(0);
    stateEstimatorEskf_DW.count_d1 = 0U;

    // 'baroFifo:94' lastPushTime_ms = uint64(0);
    stateEstimatorEskf_DW.lastPushTime_ms_j = dhSensorIn_sensorTimestamp_ma_0;

    // 'baroFifo:95' haveLastPushTime = false;
    stateEstimatorEskf_DW.haveLastPushTime_j = false;

    // 'baroFifo:97' lastBaroTimeOut_ms = uint64(0);
    stateEstimatorEskf_DW.lastBaroTimeOut_ms = dhSensorIn_sensorTimestamp_ma_0;

    // 'baroFifo:98' lastAltitudeOut_m = single(0);
    stateEstimatorEskf_DW.lastAltitudeOut_m = 0.0F;

    // 'baroFifo:100' baroReady = false;
    // 'baroFifo:101' baroTimeOut_ms = uint64(0);
    // 'baroFifo:102' altitudeOut_m = single(0);
    rtb_Product3_l = 0.0F;

    // 'baroFifo:104' countOut = count;
    stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.count = 0U;
  } else {
    //  ------------------------------------------------------------------------- 
    //  PUSH NEW BAROMETER MEASUREMENT
    //  ------------------------------------------------------------------------- 
    // 'baroFifo:111' if isBaroValid
    guard1 = false;
    guard2 = false;
    guard3 = false;
    if (stateEstimatorEskf_DW.sensorDataOut.sensorValidity.isBaroValid) {
      //  Barometer timestamps must normally increase monotonically.
      // 'baroFifo:113' if haveLastPushTime && baroTimeIn_ms <= lastPushTime_ms
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_j &
           uMultiWordLe(&baroTimeIn_ms.chunks[0U],
                        &stateEstimatorEskf_DW.lastPushTime_ms_j.chunks[0U], 2)))
      {
        // 'baroFifo:114' backwardsJump_ms = lastPushTime_ms - baroTimeIn_ms;
        //  Large backward jump indicates a sensor time-base discontinuity.
        // 'baroFifo:117' if timeResetThreshold_ms > 0 && ...
        // 'baroFifo:118'                 backwardsJump_ms >= timeResetThreshold_ms 
        uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_j.chunks[0U],
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
          // 'baroFifo:119' head = uint16(1);
          stateEstimatorEskf_DW.head_f = 1U;

          // 'baroFifo:120' tail = uint16(1);
          stateEstimatorEskf_DW.tail_p = 1U;

          // 'baroFifo:121' count = uint16(0);
          stateEstimatorEskf_DW.count_d1 = 0U;

          // 'baroFifo:122' lastPushTime_ms = uint64(0);
          stateEstimatorEskf_DW.lastPushTime_ms_j =
            dhSensorIn_sensorTimestamp_ma_0;

          // 'baroFifo:123' haveLastPushTime = false;
          stateEstimatorEskf_DW.haveLastPushTime_j = false;

          // 'baroFifo:124' status = enumDhFifoStatus.TIMEBASE_RESET;
          status = enumDhFifoStatus::TIMEBASE_RESET;
          guard3 = true;
        } else {
          // 'baroFifo:125' else
          //  Duplicate timestamp or small backward jump.
          // 'baroFifo:127' status = enumDhFifoStatus.BAD_TIMESTAMP;
          status = enumDhFifoStatus::BAD_TIMESTAMP;
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
      // 'baroFifo:134' if haveLastPushTime && ...
      // 'baroFifo:135'             (baroTimeIn_ms - lastPushTime_ms) < minObsInterval_ms 
      uMultiWord2MultiWord(&baroTimeIn_ms.chunks[0U], 2, &tmp_7.chunks[0U], 3);
      uMultiWord2MultiWord(&stateEstimatorEskf_DW.lastPushTime_ms_j.chunks[0U],
                           2, &tmp_8.chunks[0U], 3);
      MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U], 3);
      sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &tmp_a.chunks[0U], 2);
      if (static_cast<boolean_T>(stateEstimatorEskf_DW.haveLastPushTime_j &
           uMultiWordLt(&tmp_a.chunks[0U],
                        &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.minInterval_ms.chunks
                        [0], 2))) {
      } else {
        //  count is constrained to 0 <= count <= capacity.
        // 'baroFifo:140' bufferWasFull = (count == capacity);
        //  Store barometric altitude and its measurement timestamp.
        // 'baroFifo:143' timeBuf_ms(head) = baroTimeIn_ms;
        stateEstimatorEskf_DW.timeBuf_ms_g[stateEstimatorEskf_DW.head_f - 1] =
          baroTimeIn_ms;

        // 'baroFifo:144' altitudeBuf_m(head) = altitudeIn_m;
        stateEstimatorEskf_DW.altitudeBuf_m[stateEstimatorEskf_DW.head_f - 1] =
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m;

        //  Advance write location.
        // 'baroFifo:147' if head == capacity
        if (stateEstimatorEskf_DW.head_f == 16) {
          // 'baroFifo:148' head = uint16(1);
          stateEstimatorEskf_DW.head_f = 1U;
        } else {
          // 'baroFifo:149' else
          // 'baroFifo:150' head = head + 1;
          qY = stateEstimatorEskf_DW.head_f + 1U;
          if (stateEstimatorEskf_DW.head_f + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.head_f = static_cast<uint16_T>(qY);
        }

        // 'baroFifo:153' if bufferWasFull
        if (stateEstimatorEskf_DW.count_d1 == 16) {
          //  New write replaced the oldest measurement.
          //  Advance tail to the new oldest measurement.
          // 'baroFifo:156' if tail == capacity
          if (stateEstimatorEskf_DW.tail_p == 16) {
            // 'baroFifo:157' tail = uint16(1);
            stateEstimatorEskf_DW.tail_p = 1U;
          } else {
            // 'baroFifo:158' else
            // 'baroFifo:159' tail = tail + 1;
            qY = stateEstimatorEskf_DW.tail_p + 1U;
            if (stateEstimatorEskf_DW.tail_p + 1U > 65535U) {
              qY = 65535U;
            }

            stateEstimatorEskf_DW.tail_p = static_cast<uint16_T>(qY);
          }

          //  Occupancy remains full.
          // 'baroFifo:163' count = capacity;
          //  Preserve TIMEBASE_RESET if that occurred during this push.
          // 'baroFifo:166' if status == enumDhFifoStatus.OK
          if (status == enumDhFifoStatus::OK) {
            // 'baroFifo:167' status = enumDhFifoStatus.OVERWRITE;
            status = enumDhFifoStatus::OVERWRITE;
          }
        } else {
          // 'baroFifo:169' else
          //  One new measurement was added without replacing anything.
          // 'baroFifo:171' count = count + 1;
          qY = stateEstimatorEskf_DW.count_d1 + 1U;
          if (stateEstimatorEskf_DW.count_d1 + 1U > 65535U) {
            qY = 65535U;
          }

          stateEstimatorEskf_DW.count_d1 = static_cast<uint16_T>(qY);
        }

        //  Update timestamp history only for measurements actually accepted.
        // 'baroFifo:175' lastPushTime_ms = baroTimeIn_ms;
        stateEstimatorEskf_DW.lastPushTime_ms_j = baroTimeIn_ms;

        // 'baroFifo:176' haveLastPushTime = true;
        stateEstimatorEskf_DW.haveLastPushTime_j = true;

        // 'baroFifo:177' countOut = count;
        stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.count =
          stateEstimatorEskf_DW.count_d1;
        guard2 = true;
      }
    }

    if (guard2) {
      //  ------------------------------------------------------------------------- 
      //  POP NEWEST BAROMETER MEASUREMENT ELIGIBLE AT FUSION HORIZON
      //  ------------------------------------------------------------------------- 
      //  Empty FIFO.
      // 'baroFifo:185' if count == 0
      if ((stateEstimatorEskf_DW.count_d1 == 0) || uMultiWordGt
          (&stateEstimatorEskf_DW.timeBuf_ms_g[stateEstimatorEskf_DW.tail_p - 1]
           .chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
      } else {
        //  Measurements are chronologically ordered. If the oldest measurement is 
        //  newer than the fusion horizon, no buffered measurement is ready.
        // 'baroFifo:191' if timeBuf_ms(tail) > fusionTime_ms
        //  At least one measurement is eligible.
        //
        //  Discard older eligible measurements until tail points at:
        //
        //    max{ baroTime | baroTime <= fusionTime_ms }
        //
        //  Loop execution is statically bounded by MAX_SIZE-1.
        // 'baroFifo:202' for k = 1:(MAX_SIZE-1)
        rtb_VectorConcatenate1_tmp = 0;
        exitg1 = false;
        while ((!exitg1) && (static_cast<boolean_T>((rtb_VectorConcatenate1_tmp <
                  23) & (stateEstimatorEskf_DW.count_d1 != 1)))) {
          // 'baroFifo:203' if count == 1
          // 'baroFifo:207' if tail == capacity
          if (stateEstimatorEskf_DW.tail_p == 16) {
            // 'baroFifo:208' nextIdx = uint16(1);
            nextIdx = 1U;
          } else {
            // 'baroFifo:209' else
            // 'baroFifo:210' nextIdx = tail + 1;
            qY = stateEstimatorEskf_DW.tail_p + 1U;
            if (stateEstimatorEskf_DW.tail_p + 1U > 65535U) {
              qY = 65535U;
            }

            nextIdx = static_cast<uint16_T>(qY);
          }

          // 'baroFifo:213' if timeBuf_ms(nextIdx) > fusionTime_ms
          if (uMultiWordGt(&stateEstimatorEskf_DW.timeBuf_ms_g[nextIdx - 1].
                           chunks[0U], &fusionTime_ms.chunks[0U], 2)) {
            exitg1 = true;
          } else {
            //  A newer eligible measurement exists; discard current tail.
            // 'baroFifo:218' tail = nextIdx;
            stateEstimatorEskf_DW.tail_p = nextIdx;

            // 'baroFifo:219' count = count - 1;
            qY = stateEstimatorEskf_DW.count_d1 -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_d1 - 1U >
                stateEstimatorEskf_DW.count_d1) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_d1 = static_cast<uint16_T>(qY);
            rtb_VectorConcatenate1_tmp++;
          }
        }

        //  Tail now points to the newest eligible barometer measurement.
        // 'baroFifo:223' measurementAge_ms = fusionTime_ms - timeBuf_ms(tail);
        //  Optional maximum-age gate.
        //  maxAge_ms == 0 disables this check.
        // 'baroFifo:227' if maxAge_ms > 0 && measurementAge_ms >= maxAge_ms
        if (uMultiWordGt
            (&stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.maxAge_ms.chunks
             [0], &dhSensorIn_sensorTimestamp_ma_0.chunks[0U], 2)) {
          uMultiWord2MultiWord(&fusionTime_ms.chunks[0U], 2, &tmp_7.chunks[0U],
                               3);
          uMultiWord2MultiWord
            (&stateEstimatorEskf_DW.timeBuf_ms_g[stateEstimatorEskf_DW.tail_p -
             1].chunks[0U], 2, &tmp_8.chunks[0U], 3);
          MultiWordSub(&tmp_7.chunks[0U], &tmp_8.chunks[0U], &tmp_6.chunks[0U],
                       3);
          sMultiWord2uMultiWordSat(&tmp_6.chunks[0U], 3, &tmp_b.chunks[0U], 2);
          if (uMultiWordGe(&tmp_b.chunks[0U],
                           &stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.baroFifoParams.maxAge_ms.chunks
                           [0], 2)) {
            // 'baroFifo:228' if tail == capacity
            if (stateEstimatorEskf_DW.tail_p == 16) {
              // 'baroFifo:229' tail = uint16(1);
              stateEstimatorEskf_DW.tail_p = 1U;
            } else {
              // 'baroFifo:230' else
              // 'baroFifo:231' tail = tail + 1;
              qY = stateEstimatorEskf_DW.tail_p + 1U;
              if (stateEstimatorEskf_DW.tail_p + 1U > 65535U) {
                qY = 65535U;
              }

              stateEstimatorEskf_DW.tail_p = static_cast<uint16_T>(qY);
            }

            // 'baroFifo:234' count = count - 1;
            qY = stateEstimatorEskf_DW.count_d1 -
              /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
              /*MW:OvSatOk*/ 1U;
            if (stateEstimatorEskf_DW.count_d1 - 1U >
                stateEstimatorEskf_DW.count_d1) {
              qY = 0U;
            }

            stateEstimatorEskf_DW.count_d1 = static_cast<uint16_T>(qY);

            // 'baroFifo:235' countOut = count;
            stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.count =
              stateEstimatorEskf_DW.count_d1;

            // 'baroFifo:236' status = enumDhFifoStatus.STALE_DISCARD;
            status = enumDhFifoStatus::STALE_DISCARD;
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
      // 'baroFifo:241' baroTimeOut_ms = timeBuf_ms(tail);
      // 'baroFifo:242' altitudeOut_m = altitudeBuf_m(tail);
      rtb_Product3_l =
        stateEstimatorEskf_DW.altitudeBuf_m[stateEstimatorEskf_DW.tail_p - 1];

      // 'baroFifo:243' baroReady = true;
      stateModeReady = true;

      //  Hold last returned measurement for future calls where baroReady is false. 
      // 'baroFifo:246' lastBaroTimeOut_ms = baroTimeOut_ms;
      stateEstimatorEskf_DW.lastBaroTimeOut_ms =
        stateEstimatorEskf_DW.timeBuf_ms_g[stateEstimatorEskf_DW.tail_p - 1];

      // 'baroFifo:247' lastAltitudeOut_m = altitudeOut_m;
      stateEstimatorEskf_DW.lastAltitudeOut_m = rtb_Product3_l;

      //  Consume returned measurement.
      // 'baroFifo:250' if tail == capacity
      if (stateEstimatorEskf_DW.tail_p == 16) {
        // 'baroFifo:251' tail = uint16(1);
        stateEstimatorEskf_DW.tail_p = 1U;
      } else {
        // 'baroFifo:252' else
        // 'baroFifo:253' tail = tail + 1;
        qY = stateEstimatorEskf_DW.tail_p + 1U;
        if (stateEstimatorEskf_DW.tail_p + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.tail_p = static_cast<uint16_T>(qY);
      }

      // 'baroFifo:256' count = count - 1;
      qY = stateEstimatorEskf_DW.count_d1 -
        /*MW:operator MISRA2012:D4.1 CERT-C:INT30-C 'Justifying MISRA C rule violation'*/
        /*MW:OvSatOk*/ 1U;
      if (stateEstimatorEskf_DW.count_d1 - 1U > stateEstimatorEskf_DW.count_d1)
      {
        qY = 0U;
      }

      stateEstimatorEskf_DW.count_d1 = static_cast<uint16_T>(qY);

      // 'baroFifo:257' countOut = count;
      stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.count =
        stateEstimatorEskf_DW.count_d1;
    }
  }

  // 'delayedHorizonBufferManager_function:118' dhBufferDebug.baroFifoDebugData.status= status; 
  stateEstimatorEskf_DW.dhBufferDebug.baroFifoDebugData.status = status;

  // 'delayedHorizonBufferManager_function:119' dhBufferDebug.baroFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:121' dhSensorIn.sensorValidity.isBaroValid = baroReady; 
  // 'delayedHorizonBufferManager_function:122' dhSensorIn.sensorTimestamp.baroTimestamp_ms = baroTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:123' dhSensorIn.baroAlt_m = baroAltOut_m; 
  // 'delayedHorizonBufferManager_function:125' [lidarReady, lidarTimeOut_ms, lidarAglOut_m, status, count] = ... 
  // 'delayedHorizonBufferManager_function:126'     lidarFifo(sensorIn.sensorValidity.isLidarValid, ... 
  // 'delayedHorizonBufferManager_function:127'               lidarTimeIn_ms, ... 
  // 'delayedHorizonBufferManager_function:128'               sensorIn.lidarAgl_m, ... 
  // 'delayedHorizonBufferManager_function:129'               fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:130'               ekfParams.lidarFifoParams); 
  stateEstimatorEskf_lidarFifo(mtf01pDistValidityFlag, lidarTimeIn_ms, (-std::
    sin(stateEstimatorEskf_DW.UnitDelay1_DSTATE[1]) *
    rtu_mtf01pParams->posVector_m[0] + std::sin
    (stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) * rtb_UnitDelay_g *
    rtu_mtf01pParams->posVector_m[1]) + std::cos
    (stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) * rtb_UnitDelay_g *
    (rtu_mtf01pParams->posVector_m[2] + rtu_mtf01pData->dist_m), fusionTime_ms,
    stateEstimatorEskf_DW.resetStates,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.capacity,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.minInterval_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.maxAge_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.lidarFifoParams.resetThreshold_ms,
    &dhSensorIn_sensorValidity_isLid, &gpsTimeIn_ms, &rtb_Product2_c,
    &stateEstimatorEskf_DW.dhBufferDebug.lidarFifoDebugData.status,
    &stateEstimatorEskf_DW.dhBufferDebug.lidarFifoDebugData.count);

  // 'delayedHorizonBufferManager_function:132' dhBufferDebug.lidarFifoDebugData.status = status; 
  // 'delayedHorizonBufferManager_function:133' dhBufferDebug.lidarFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:135' dhSensorIn.sensorValidity.isLidarValid = lidarReady; 
  // 'delayedHorizonBufferManager_function:136' dhSensorIn.sensorTimestamp.lidarTimestamp_ms = lidarTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:137' dhSensorIn.lidarAgl_m = lidarAglOut_m; 
  // 'delayedHorizonBufferManager_function:139' [ofReady, ofTimeOut_ms, ofNeVelOut_mps, status, count] = ... 
  // 'delayedHorizonBufferManager_function:140'     flowFifo(sensorIn.sensorValidity.isOfValid, ... 
  // 'delayedHorizonBufferManager_function:141'              ofTimeIn_ms, ...
  // 'delayedHorizonBufferManager_function:142'              sensorIn.ofNeVel_mps, ... 
  // 'delayedHorizonBufferManager_function:143'              fusionTime_ms, reset, ... 
  // 'delayedHorizonBufferManager_function:144'              ekfParams.flowFifoParams); 
  stateEstimatorEskf_flowFifo(static_cast<boolean_T>(static_cast<boolean_T>(
    static_cast<boolean_T>(mtf01pDistValidityFlag & (rtu_mtf01pData->flowStatus ==
    1)) & rtu_mtf01pData->isMtf01pDataValid) &
    rtu_stateEstSmParams->useOpticalFlow), ofTimeIn_ms, VectorConcatenate,
    fusionTime_ms, stateEstimatorEskf_DW.resetStates,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.capacity,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.minInterval_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.maxAge_ms,
    stateEstimatorEskf_ConstP.DelayedHorizonBufferManager_ekf.flowFifoParams.resetThreshold_ms,
    &dhSensorIn_sensorValidity_isOfV, &lidarTimeIn_ms, rtb_VectorConcatenate_k2,
    &stateEstimatorEskf_DW.dhBufferDebug.flowFifoDebugData.status,
    &stateEstimatorEskf_DW.dhBufferDebug.flowFifoDebugData.count);

  // Delay: '<S1>/Delay'
  // 'delayedHorizonBufferManager_function:146' dhBufferDebug.flowFifoDebugData.status= status; 
  // 'delayedHorizonBufferManager_function:147' dhBufferDebug.flowFifoDebugData.count = count; 
  // 'delayedHorizonBufferManager_function:149' dhSensorIn.sensorValidity.isOfValid = ofReady; 
  // 'delayedHorizonBufferManager_function:150' dhSensorIn.sensorTimestamp.ofTimestamp_ms = ofTimeOut_ms; 
  // 'delayedHorizonBufferManager_function:151' dhSensorIn.ofNeVel_mps = ofNeVelOut_mps; 
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

  // BusAssignment: '<Root>/Bus Assignment1' incorporates:
  //   Constant: '<S1>/Constant'
  //   MATLAB Function: '<S1>/EKF'

  rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isMagUsed = false;

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   Constant: '<Root>/localNedMag_nd'
  //   Constant: '<S1>/Constant'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay2'
  //   MATLAB Function: '<S15>/DelayedHorizonBufferManager'

  rtb_ekfDebugOut_isAidingUsed_is = 0U;
  mtf01pDistValidityFlag = false;
  rtb_ekfDebugOut_isAidingUsed__0 = false;
  rtb_ekfDebugOut_isAidingUsed__1 = false;
  std::memset(&rtb_ekfDebugOut_dhStates[0], 0, 23U * sizeof(real32_T));

  // MATLAB Function 'EKF/EKF': '<S16>:1'
  // '<S16>:1:5' if isempty(covP) || resetStates
  if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<int32_T>
        (stateEstimatorEskf_DW.covP_not_empty) ^ 1) |
       stateEstimatorEskf_DW.resetStates)) {
    // '<S16>:1:6' covP = initCovP;
    std::memcpy(&stateEstimatorEskf_DW.covP[0], &rtu_initCovP[0], 361U * sizeof
                (real32_T));
    stateEstimatorEskf_DW.covP_not_empty = true;
  }

  // '<S16>:1:9' [states, covP, ekfDebugOut] = errorStateEkf_function2(sensorIn, prevStates, covP, prevDcmBodyToNed, estSmMode, ... 
  // '<S16>:1:10'     processNoiseQ, measNoiseR, gEarth_mps2, ekfDebugIn, sampleTime_s, ekfParams); 
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
  // 'errorStateEkf_function2:75' ekfDebugOut = ekfDebugIn;
  //  ekfDebugOut.isAidingUsed.mag = false;
  //  ekfDebugOut.isAidingUsed.gps = false;
  //  ekfDebugOut.isAidingUsed.baro = false;
  //  ekfDebugOut.isAidingUsed.lidar = false;
  //  ekfDebugOut.isAidingUsed.flow = false;
  // Propagate state
  // 'errorStateEkf_function2:84' if (estSmMode == enumStateEstimateMode.INITIALIZE) 
  if (estSmModeOut == enumStateEstimateMode::INITIALIZE) {
    // 'errorStateEkf_function2:85' states = prevStates;
    std::memcpy(&rtb_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                sizeof(real32_T));
  } else {
    // ErrorStateHat
    // 'errorStateEkf_function2:90' errorStateHat = zeros(19, 1, 'single');
    std::memset(&errorStateHat[0], 0, 19U * sizeof(real32_T));

    // 'errorStateEkf_function2:92' isImuValid = sensorIn.sensorValidity.isImuValid; 
    //  if(isImuValid)
    // Get all the sensor data
    // 'errorStateEkf_function2:96' bodyAccels_mps2 = sensorIn.bodyAccels_mps2;
    // 'errorStateEkf_function2:97' bodyRates_radps = sensorIn.bodyRates_radps;
    // 'errorStateEkf_function2:98' dtImuTime_s = sensorIn.dtImuTime_s;
    // 'errorStateEkf_function2:100' normMagVec_nd = sensorIn.normMagVec_nd;
    // 'errorStateEkf_function2:101' localNedUnitMag_nd = sensorIn.localNedUnitMag_nd; 
    // 'errorStateEkf_function2:102' isMagValid = sensorIn.sensorValidity.isMagValid; 
    // 'errorStateEkf_function2:103' nedPosAndVel =  [sensorIn.nedPosAndVel.pos_m; sensorIn.nedPosAndVel.vel_mps]; 
    nedPosAndVel[0] = rtb_ElementProduct[0];
    nedPosAndVel[3] = rtb_ElementProduct[3];
    nedPosAndVel[1] = rtb_ElementProduct[1];
    nedPosAndVel[4] = rtb_ElementProduct[4];
    nedPosAndVel[2] = rtb_ElementProduct[2];
    nedPosAndVel[5] = rtb_ElementProduct[5];

    // 'errorStateEkf_function2:104' isGpsValid = sensorIn.sensorValidity.isGpsValid; 
    // 'errorStateEkf_function2:105' baroAlt_m = sensorIn.baroAlt_m;
    // 'errorStateEkf_function2:106' isBaroValid = sensorIn.sensorValidity.isBaroValid; 
    // 'errorStateEkf_function2:107' lidarAgl_m = sensorIn.lidarAgl_m;
    // 'errorStateEkf_function2:108' isLidarValid = sensorIn.sensorValidity.isLidarValid; 
    // 'errorStateEkf_function2:109' ofNeVel_mps = sensorIn.ofNeVel_mps;
    // 'errorStateEkf_function2:110' isOfValid = sensorIn.sensorValidity.isOfValid; 
    // We don't update error states because initial value of error states are all 
    // zero
    // errorStates = errorStateJac*errorStates + inputPerturbJac*processNoiseCov 
    // Valid nominal state indices when GPS is avalable
    //      idxNs = 1:20;
    // 'errorStateEkf_function2:118' idxNs2 = 5:20;
    // Without quaternion
    // Valid error state indices when GPS is avalable
    // 'errorStateEkf_function2:120' idxEs = 1:19;
    // 'errorStateEkf_function2:121' idxEs2 = 4:19;
    // Without angle error
    // Valid nominal state indices when there is no GPS avalable
    //      idxNs = [1:4, 7, 10:20];
    // 'errorStateEkf_function2:125' idxNoGpsNs2 = [7, 10:20];
    // Without quaternion
    // Valid error state indices when there is no GPS avalable
    // 'errorStateEkf_function2:127' idxNoGpsEs = [1:3, 6, 9:19];
    // 'errorStateEkf_function2:128' idxNoGpsEs2 = [6, 9:19];
    // Without angle error
    //  %Apply Rodrigues formula to the angleVector to get the rotation vector
    //  dR = I3 + sin(dThetaNorm)*skew3(dThetaUnit) + dThetaUnit*dThetaUnit'*(1 - cos(dThetaNorm)); 
    //  %
    //  errorStateJac(1:3, 1:3) = dR';
    // 'errorStateEkf_function2:135' if estSmMode == enumStateEstimateMode.RUN
    if (estSmModeOut == enumStateEstimateMode::RUN) {
      //      Compute the skew matrix for accel - accelbias
      //      accelSkew = skew3(bodyAccels_mps2 - prevStates(14:16));
      //      errorStateJac(4:6, 7:9) = I3*sampleTime_s;
      //      errorStateJac(7:9, 1:3) = -dcmBodyToNed * ...
      //          accelSkew*sampleTime_s;
      //      errorStateJac(7:9, 13:15) = -dcmBodyToNed*sampleTime_s;
      //  covP = errorStateJac*covP*errorStateJac' + processNoiseQ;
      // 'errorStateEkf_function2:143' gpsLossFlag = false;
      gpsLossFlag = false;
    } else {
      // 'errorStateEkf_function2:144' else
      //      errorStateJac(6, 9) = sampleTime_s;
      //      errorStateJac(7:9, 1:3) = single(0);
      //      errorStateJac(7:9, 13:15) = single(0);
      //      %Propogate covariances
      //      covP(idxEs, idxEs) = errorStateJac(idxEs, idxEs) * covP(idxEs, idxEs) * ... 
      //          errorStateJac(idxEs, idxEs)' + processNoiseQ(idxEs, idxEs);
      // 'errorStateEkf_function2:151' gpsLossFlag = true;
      gpsLossFlag = true;
    }

    // Propagate nominal states and Compute Jacobian and propagate covariance only if IMU data is valid 
    // 'errorStateEkf_function2:155' if(isImuValid)
    if (imuReady) {
      // 'errorStateEkf_function2:156' [states, dThetaNorm, dThetaUnit] = updateEskfStates(prevStates, bodyAccels_mps2, bodyRates_radps, ... 
      // 'errorStateEkf_function2:157'             dcmBodyToNed, estSmMode, isOfValid, dtImuTime_s, gEarth_mps2); 
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
      if (static_cast<boolean_T>((estSmModeOut == enumStateEstimateMode::RUN) |
           dhSensorIn_sensorValidity_isOfV)) {
        // 'updateEskfStates:28' stateDot = [ [states(8); states(9); states(10)]; ...                            % Derivative of [pN; pE; pD] 
        // 'updateEskfStates:29'     dcmBodyToNed * (bodyAccels_mps2 - [states(14);states(15);states(16)]) + ... 
        // 'updateEskfStates:30'     [0; 0; gEarth_mps2]];
        //                             % Derivative of [pN; pE; pD]
        //                                                        % Derivative of [vN; vE; vD] 
        // 'updateEskfStates:31' states(5:10) = states(5:10) + sampleTime_s * stateDot; 
        rtb_UnitDelay_g = stateEstimatorEskf_DW.lastImuOut[0] -
          stateEstimatorEskf_DW.Delay_DSTATE[13];
        rtb_XAxis = stateEstimatorEskf_DW.lastImuOut[1] -
          stateEstimatorEskf_DW.Delay_DSTATE[14];
        rtb_XAxis1 = stateEstimatorEskf_DW.lastImuOut[2] -
          stateEstimatorEskf_DW.Delay_DSTATE[15];
        rtb_states[4] = stateEstimatorEskf_DW.lastImuOut[6] *
          stateEstimatorEskf_DW.Delay_DSTATE[7] +
          stateEstimatorEskf_DW.Delay_DSTATE[4];
        rtb_states[5] = stateEstimatorEskf_DW.lastImuOut[6] *
          stateEstimatorEskf_DW.Delay_DSTATE[8] +
          stateEstimatorEskf_DW.Delay_DSTATE[5];
        rtb_states[6] = stateEstimatorEskf_DW.lastImuOut[6] *
          stateEstimatorEskf_DW.Delay_DSTATE[9] +
          stateEstimatorEskf_DW.Delay_DSTATE[6];
        rtb_states[7] = ((stateEstimatorEskf_DW.Delay2_DSTATE[0] *
                          rtb_UnitDelay_g + stateEstimatorEskf_DW.Delay2_DSTATE
                          [3] * rtb_XAxis) +
                         stateEstimatorEskf_DW.Delay2_DSTATE[6] * rtb_XAxis1) *
          stateEstimatorEskf_DW.lastImuOut[6] +
          stateEstimatorEskf_DW.Delay_DSTATE[7];
        rtb_states[8] = ((stateEstimatorEskf_DW.Delay2_DSTATE[1] *
                          rtb_UnitDelay_g + stateEstimatorEskf_DW.Delay2_DSTATE
                          [4] * rtb_XAxis) +
                         stateEstimatorEskf_DW.Delay2_DSTATE[7] * rtb_XAxis1) *
          stateEstimatorEskf_DW.lastImuOut[6] +
          stateEstimatorEskf_DW.Delay_DSTATE[8];
        rtb_states[9] = (((stateEstimatorEskf_DW.Delay2_DSTATE[2] *
                           rtb_UnitDelay_g +
                           stateEstimatorEskf_DW.Delay2_DSTATE[5] * rtb_XAxis) +
                          stateEstimatorEskf_DW.Delay2_DSTATE[8] * rtb_XAxis1) +
                         *rtu_gEarth_mps2) * stateEstimatorEskf_DW.lastImuOut[6]
          + stateEstimatorEskf_DW.Delay_DSTATE[9];
      } else {
        // 'updateEskfStates:32' else
        // 'updateEskfStates:33' stateDot = states(10);
        //  %Derivative of down position
        // 'updateEskfStates:35' states(7) = states(7) + sampleTime_s * stateDot; 
        rtb_states[6] = stateEstimatorEskf_DW.lastImuOut[6] *
          stateEstimatorEskf_DW.Delay_DSTATE[9] +
          stateEstimatorEskf_DW.Delay_DSTATE[6];
      }

      // 'updateEskfStates:38' dTheta = (bodyRates_radps - states(11:13))*sampleTime_s; 
      rtb_CastToSingle[0] = (stateEstimatorEskf_DW.lastImuOut[3] - rtb_states[10])
        * stateEstimatorEskf_DW.lastImuOut[6];
      rtb_CastToSingle[1] = (stateEstimatorEskf_DW.lastImuOut[4] - rtb_states[11])
        * stateEstimatorEskf_DW.lastImuOut[6];
      rtb_CastToSingle[2] = (stateEstimatorEskf_DW.lastImuOut[5] - rtb_states[12])
        * stateEstimatorEskf_DW.lastImuOut[6];

      // 'updateEskfStates:39' dThetaNorm = norm(dTheta);
      rtb_XAxis = norm_94qjDDKI(rtb_CastToSingle);

      // 'updateEskfStates:41' if dThetaNorm > 1e-7
      if (rtb_XAxis > 1.0E-7) {
        // Propagate the quaternion part of the state
        // 'updateEskfStates:43' dThetaUnit = dTheta/dThetaNorm;
        // 'updateEskfStates:44' states(1:4) = quatMultiply(states(1:4), [cos(dThetaNorm*0.5); sin(dThetaNorm*0.5)*dThetaUnit]); 
        rtb_UnitDelay_g = rtb_XAxis * 0.5F;
        rtb_XAxis1 = std::sin(rtb_UnitDelay_g);
        tmp_2[0] = std::cos(rtb_UnitDelay_g);
        rtb_UnitDelay_g = rtb_CastToSingle[0] / rtb_XAxis;
        rtb_CastToSingle[0] = rtb_UnitDelay_g;
        tmp_2[1] = rtb_XAxis1 * rtb_UnitDelay_g;
        rtb_UnitDelay_g = rtb_CastToSingle[1] / rtb_XAxis;
        rtb_CastToSingle[1] = rtb_UnitDelay_g;
        tmp_2[2] = rtb_XAxis1 * rtb_UnitDelay_g;
        rtb_UnitDelay_g = rtb_CastToSingle[2] / rtb_XAxis;
        rtb_CastToSingle[2] = rtb_UnitDelay_g;
        tmp_2[3] = rtb_XAxis1 * rtb_UnitDelay_g;
        quatMultiply_UkcBdhzN(&rtb_states[0], tmp_2, qDelayed);
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

      // 'errorStateEkf_function2:158' errorStateJac = computeEskfStateJac(prevStates, dcmBodyToNed, bodyAccels_mps2, ... 
      // 'errorStateEkf_function2:159'             dThetaNorm, dThetaUnit, gpsLossFlag, isOfValid, dtImuTime_s); 
      // 'errorStateEkf_function2:160' covP = updateEskfCovP(covP, errorStateJac, processNoiseQ, dtImuTime_s); 
      computeEskfStateJac_tpF0ZWTA(stateEstimatorEskf_DW.Delay_DSTATE,
        stateEstimatorEskf_DW.Delay2_DSTATE, &stateEstimatorEskf_DW.lastImuOut[0],
        rtb_XAxis, rtb_CastToSingle, gpsLossFlag,
        dhSensorIn_sensorValidity_isOfV, stateEstimatorEskf_DW.lastImuOut[6],
        tmp_d);
      updateEskfCovP_Qn4XLGNE(stateEstimatorEskf_DW.covP, tmp_d,
        rtu_processNoiseQ, stateEstimatorEskf_DW.lastImuOut[6]);
    } else {
      // 'errorStateEkf_function2:161' else
      // 'errorStateEkf_function2:162' states = prevStates;
      std::memcpy(&rtb_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                  sizeof(real32_T));
    }

    // Fuse Accel in Correction step if GPS is not available
    // 'errorStateEkf_function2:166' if estSmMode ~= enumStateEstimateMode.RUN && ~isOfValid && ... 
    // 'errorStateEkf_function2:167'             isImuValid
    if (static_cast<boolean_T>(static_cast<boolean_T>((estSmModeOut !=
           enumStateEstimateMode::RUN) & static_cast<boolean_T>
          (static_cast<int32_T>(dhSensorIn_sensorValidity_isOfV) ^ 1)) &
         imuReady)) {
      // 'errorStateEkf_function2:168' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:462' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:463'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:464'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:465'     -states(3), states(2), states(1)];
      rtb_XAxis = 0.5F * -rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_XAxis;
      rtb_XAxis1 = 0.5F * -rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[20] = rtb_XAxis1;
      rtb_UnitDelay_g = 0.5F * -rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_UnitDelay_g;
      rtb_XAxis2 = 0.5F * rtb_states[0];
      stateEstimatorEskf_DW.xErrorJac[1] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_XAxis;
      stateEstimatorEskf_DW.xErrorJac[3] = rtb_XAxis1;
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = rtb_XAxis2;

      // 'errorStateEkf_function2:170' measJac = computeEskfAccelMeasJac(states, gEarth_mps2); 
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
      rtb_UnitDelay_g = *rtu_gEarth_mps2 * 2.0F;
      rtb_XAxis = rtb_UnitDelay_g * rtb_states[2];

      // 'computeEskfAccelMeasJac:22' tmp2 = -2*gEarth_mps2*states(4);
      rtb_XAxis1 = -2.0F * *rtu_gEarth_mps2 * rtb_states[3];

      // 'computeEskfAccelMeasJac:23' tmp3 = gEarth_mps2*2*states(1);
      rtb_XAxis2 = rtb_UnitDelay_g * rtb_states[0];

      // 'computeEskfAccelMeasJac:24' tmp4 = -gEarth_mps2*2*states(2);
      rtb_UnitDelay_g = -*rtu_gEarth_mps2 * 2.0F * rtb_states[1];

      // 'computeEskfAccelMeasJac:25' tmp5 = 4*gEarth_mps2;
      rtb_Sum_m1_idx_1 = 4.0F * *rtu_gEarth_mps2;

      // 'computeEskfAccelMeasJac:27' accelMeasJac(1, 1) = tmp1;
      measJac[0] = rtb_XAxis;

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
      measJac[10] = -rtb_XAxis;

      // 'computeEskfAccelMeasJac:37' accelMeasJac(2, 15) = 1;
      measJac[43] = 1.0F;

      // 'computeEskfAccelMeasJac:39' accelMeasJac(3, 2) = states(2)*tmp5;
      measJac[5] = rtb_states[1] * rtb_Sum_m1_idx_1;

      // 'computeEskfAccelMeasJac:40' accelMeasJac(3, 3) = states(3)*tmp5;
      measJac[8] = rtb_states[2] * rtb_Sum_m1_idx_1;

      // 'computeEskfAccelMeasJac:41' accelMeasJac(3, 16) = 1;
      measJac[47] = 1.0F;

      // 'errorStateEkf_function2:171' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:172' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      // 'errorStateEkf_function2:173' H(1:3, 13:15) = I3;
      rtb_VectorConcatenate1_tmp = 0;
      i_2 = 0;
      for (i = 0; i < 3; i++) {
        rtb_XAxis1 = 0.0F;
        rtb_UnitDelay_g = 0.0F;
        rtb_XAxis2 = 0.0F;
        i_1 = 0;
        for (covP_tmp = 0; covP_tmp < 4; covP_tmp++) {
          rtb_XAxis = stateEstimatorEskf_DW.xErrorJac[covP_tmp +
            rtb_VectorConcatenate1_tmp];
          rtb_XAxis1 += measJac[i_1] * rtb_XAxis;
          rtb_UnitDelay_g += measJac[i_1 + 1] * rtb_XAxis;
          rtb_XAxis2 += measJac[i_1 + 2] * rtb_XAxis;
          i_1 += 3;
        }

        H[i_2 + 2] = rtb_XAxis2;
        H[i_2 + 1] = rtb_UnitDelay_g;
        H[i_2] = rtb_XAxis1;
        H[i_2 + 36] = stateEstimatorEskf_DW.I3[i_2];
        H[i_2 + 37] = stateEstimatorEskf_DW.I3[i_2 + 1];
        H[i_2 + 38] = stateEstimatorEskf_DW.I3[i_2 + 2];
        rtb_VectorConcatenate1_tmp += 20;
        i_2 += 3;
      }

      //      H = measJac(:, idxNs) * xErrorJac(idxNs, idxEs);
      // 'errorStateEkf_function2:177' C_ned2b  = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:178'             states(4));
      // NED gravity in body frame
      // 'errorStateEkf_function2:180' estGravityInBodyFrame = C_ned2b*[0; 0; -gEarth_mps2] + states(14:16); 
      // 'errorStateEkf_function2:182' tmp1 = covP(:, 13:15) + covP(:, 1:3) * H(:, 1:3).'; 
      rtb_XAxis1 = H[3];
      rtb_UnitDelay_g = H[0];
      rtb_XAxis2 = H[6];
      rtb_XAxis = H[4];
      rtb_Sum_m1_idx_1 = H[1];
      rtb_Sum_m1_idx_2 = H[7];
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
        tmp1[rtb_VectorConcatenate1_tmp] = ((rtb_dcmBodyToNed_idx_5 * rtb_XAxis1
          + rtb_dcmBodyToNed_idx_8 * rtb_UnitDelay_g) + rtb_Merge_idx_0 *
          rtb_XAxis2) + stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp +
          228];
        tmp1[rtb_VectorConcatenate1_tmp + 19] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_XAxis + rtb_dcmBodyToNed_idx_8 * rtb_Sum_m1_idx_1) +
          rtb_Merge_idx_0 * rtb_Sum_m1_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 247];
        tmp1[rtb_VectorConcatenate1_tmp + 38] = ((rtb_dcmBodyToNed_idx_5 * q2q3
          + rtb_dcmBodyToNed_idx_8 * rtb_dcmBodyToNed_idx_7) + rtb_Merge_idx_0 *
          rtb_dcmBodyToNed_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 266];
      }

      // covP(idxEs, idxEs)*H';
      // 'errorStateEkf_function2:183' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(12:14, 12:14)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
           rtb_VectorConcatenate1_tmp++) {
        for (i_2 = 0; i_2 < 15; i_2++) {
          tmp1_0[i_2 + 15 * rtb_VectorConcatenate1_tmp] = tmp1[19 *
            rtb_VectorConcatenate1_tmp + c_0[i_2]];
        }

        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis = 0.0F;
          for (i = 0; i < 15; i++) {
            c = c_0[i];
            rtb_XAxis += H[3 * c + rtb_VectorConcatenate1_tmp] * tmp1[19 * i_2 +
              c];
          }

          rtb_VectorConcatenate_p[rtb_VectorConcatenate1_tmp + 3 * i_2] =
            rtu_measNoiseR[((i_2 + 11) * 15 + rtb_VectorConcatenate1_tmp) + 11]
            + rtb_XAxis;
        }
      }

      mrdiv_9ppBIzmt(tmp1_0, rtb_VectorConcatenate_p, K);

      // 'errorStateEkf_function2:185' errorStateHat(idxNoGpsEs) = K*(bodyAccels_mps2 - estGravityInBodyFrame); 
      quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                         rtb_states[3], tmp_4);
      rtb_VectorConcatenate1_0[0] = stateEstimatorEskf_DW.lastImuOut[0] - (
        static_cast<real32_T>(tmp_4[6]) * -*rtu_gEarth_mps2 + rtb_states[13]);
      rtb_VectorConcatenate1_0[1] = stateEstimatorEskf_DW.lastImuOut[1] - (
        static_cast<real32_T>(tmp_4[7]) * -*rtu_gEarth_mps2 + rtb_states[14]);
      rtb_VectorConcatenate1_0[2] = stateEstimatorEskf_DW.lastImuOut[2] - (
        static_cast<real32_T>(tmp_4[8]) * -*rtu_gEarth_mps2 + rtb_states[15]);
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        K_0[rtb_VectorConcatenate1_tmp] = 0.0F;
      }

      rtb_VectorConcatenate1_tmp = 0;
      for (i_2 = 0; i_2 < 3; i_2++) {
        rtb_XAxis = rtb_VectorConcatenate1_0[i_2];
        for (i = 0; i < 15; i++) {
          K_0[i] += K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis;
        }

        rtb_VectorConcatenate1_tmp += 15;
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] =
          K_0[rtb_VectorConcatenate1_tmp];
      }

      // 'errorStateEkf_function2:187' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(13:15, idxNoGpsEs) + ... 
      // 'errorStateEkf_function2:188'             H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
           rtb_VectorConcatenate1_tmp++) {
        rtb_XAxis1 = H[rtb_VectorConcatenate1_tmp + 3];
        rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp];
        rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp + 6];
        for (i_2 = 0; i_2 < 15; i_2++) {
          i = 19 * c_0[i_2];
          tmp1_0[rtb_VectorConcatenate1_tmp + 3 * i_2] =
            ((stateEstimatorEskf_DW.covP[i + 1] * rtb_XAxis1 +
              stateEstimatorEskf_DW.covP[i] * rtb_UnitDelay_g) +
             stateEstimatorEskf_DW.covP[i + 2] * rtb_XAxis2) +
            stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 12];
        }
      }

      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
           rtb_VectorConcatenate1_tmp++) {
        rtb_XAxis = K[rtb_VectorConcatenate1_tmp + 15];
        rtb_XAxis1 = K[rtb_VectorConcatenate1_tmp];
        rtb_UnitDelay_g = K[rtb_VectorConcatenate1_tmp + 30];
        for (i_2 = 0; i_2 < 15; i_2++) {
          tmp_5[rtb_VectorConcatenate1_tmp + 15 * i_2] =
            stateEstimatorEskf_DW.covP[19 * c_0[i_2] +
            c_0[rtb_VectorConcatenate1_tmp]] - ((tmp1_0[3 * i_2 + 1] * rtb_XAxis
            + tmp1_0[3 * i_2] * rtb_XAxis1) + tmp1_0[3 * i_2 + 2] *
            rtb_UnitDelay_g);
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
      // 'errorStateEkf_function2:191' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
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
      // 'errorStateEkf_function2:194' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:195' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:197' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + covP(idxNoGpsEs, idxNoGpsEs)')/2; 
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
    // 'errorStateEkf_function2:201' if(isMagValid)
    if (dhSensorIn_sensorValidity_isMag) {
      // 'errorStateEkf_function2:202' measJac = computEskfMagMeasJac(states, localNedUnitMag_nd); 
      // 'errorStateEkf_function2:204' C_ned2b = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:205'             states(4));
      // Rotate propogated mag states and add bias to estimate measurements
      // 'errorStateEkf_function2:208' estBodyMagUnitVec = C_ned2b*localNedUnitMag_nd + states(17:19); 
      // 'errorStateEkf_function2:210' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:462' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:463'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:464'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:465'     -states(3), states(2), states(1)];
      rtb_XAxis = 0.5F * -rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_XAxis;
      rtb_XAxis1 = 0.5F * -rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[20] = rtb_XAxis1;
      rtb_UnitDelay_g = 0.5F * -rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_UnitDelay_g;
      rtb_XAxis2 = 0.5F * rtb_states[0];
      stateEstimatorEskf_DW.xErrorJac[1] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_UnitDelay_g;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rtb_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rtb_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = rtb_XAxis2;
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_XAxis;
      stateEstimatorEskf_DW.xErrorJac[3] = rtb_XAxis1;
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rtb_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = rtb_XAxis2;

      // 'errorStateEkf_function2:212' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:213' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      computEskfMagMeasJac_ecmRY7bq(rtb_states, rtCP_localNedMag_nd_Value,
        measJac);

      // 'errorStateEkf_function2:214' H(1:3, 16:18) = I3;
      rtb_VectorConcatenate1_tmp = 0;
      i_2 = 0;
      for (i = 0; i < 3; i++) {
        rtb_XAxis1 = 0.0F;
        rtb_UnitDelay_g = 0.0F;
        rtb_XAxis2 = 0.0F;
        i_1 = 0;
        for (covP_tmp = 0; covP_tmp < 4; covP_tmp++) {
          rtb_XAxis = stateEstimatorEskf_DW.xErrorJac[covP_tmp +
            rtb_VectorConcatenate1_tmp];
          rtb_XAxis1 += measJac[i_1] * rtb_XAxis;
          rtb_UnitDelay_g += measJac[i_1 + 1] * rtb_XAxis;
          rtb_XAxis2 += measJac[i_1 + 2] * rtb_XAxis;
          i_1 += 3;
        }

        H[i_2 + 2] = rtb_XAxis2;
        H[i_2 + 1] = rtb_UnitDelay_g;
        H[i_2] = rtb_XAxis1;
        H[i_2 + 45] = stateEstimatorEskf_DW.I3[i_2];
        H[i_2 + 46] = stateEstimatorEskf_DW.I3[i_2 + 1];
        H[i_2 + 47] = stateEstimatorEskf_DW.I3[i_2 + 2];
        rtb_VectorConcatenate1_tmp += 20;
        i_2 += 3;
      }

      // 'errorStateEkf_function2:215' tmp1 = covP(:, 16:18) + covP(:, 1:3) * H(:,1:3).'; 
      rtb_XAxis1 = H[3];
      rtb_UnitDelay_g = H[0];
      rtb_XAxis2 = H[6];
      rtb_XAxis = H[4];
      rtb_Sum_m1_idx_1 = H[1];
      rtb_Sum_m1_idx_2 = H[7];
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
        tmp1[rtb_VectorConcatenate1_tmp] = ((rtb_dcmBodyToNed_idx_5 * rtb_XAxis1
          + rtb_dcmBodyToNed_idx_8 * rtb_UnitDelay_g) + rtb_Merge_idx_0 *
          rtb_XAxis2) + stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp +
          285];
        tmp1[rtb_VectorConcatenate1_tmp + 19] = ((rtb_dcmBodyToNed_idx_5 *
          rtb_XAxis + rtb_dcmBodyToNed_idx_8 * rtb_Sum_m1_idx_1) +
          rtb_Merge_idx_0 * rtb_Sum_m1_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 304];
        tmp1[rtb_VectorConcatenate1_tmp + 38] = ((rtb_dcmBodyToNed_idx_5 * q2q3
          + rtb_dcmBodyToNed_idx_8 * rtb_dcmBodyToNed_idx_7) + rtb_Merge_idx_0 *
          rtb_dcmBodyToNed_idx_2) +
          stateEstimatorEskf_DW.covP[rtb_VectorConcatenate1_tmp + 323];
      }

      // covP(idxEs, idxEs)*H(:, idxEs)';
      // 'errorStateEkf_function2:217' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      imuReady = (estSmModeOut == enumStateEstimateMode::RUN) |
        dhSensorIn_sensorValidity_isOfV;
      if (imuReady) {
        // 'errorStateEkf_function2:218' K = tmp1(idxEs, :)/(H(:, idxEs) * tmp1(idxEs, :) + measNoiseR(1:3, 1:3)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          i_2 = 0;
          i = 0;
          i_1 = 0;
          for (covP_tmp = 0; covP_tmp < 3; covP_tmp++) {
            rtb_XAxis = 0.0F;
            tmp_c = 0;
            for (i_0 = 0; i_0 < 19; i_0++) {
              rtb_XAxis += H[tmp_c + rtb_VectorConcatenate1_tmp] * tmp1[i_0 +
                i_1];
              tmp_c += 3;
            }

            rtb_VectorConcatenate_p[i_2 + rtb_VectorConcatenate1_tmp] =
              rtu_measNoiseR[i + rtb_VectorConcatenate1_tmp] + rtb_XAxis;
            i_2 += 3;
            i += 15;
            i_1 += 19;
          }
        }

        mrdiv_7fpDxZtR(tmp1, rtb_VectorConcatenate_p, b_K);

        // 'errorStateEkf_function2:219' errorStateHat(idxEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                           rtb_states[3], tmp_4);
        rtb_VectorConcatenate1_0[0] = Divide[0] - (((static_cast<real32_T>
          (tmp_4[0]) * 0.4752F + static_cast<real32_T>(tmp_4[3]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[6]) * 0.873F) + rtb_states[16]);
        rtb_VectorConcatenate1_0[1] = Divide[1] - (((static_cast<real32_T>
          (tmp_4[1]) * 0.4752F + static_cast<real32_T>(tmp_4[4]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[7]) * 0.873F) + rtb_states[17]);
        rtb_VectorConcatenate1_0[2] = Divide[2] - (((static_cast<real32_T>
          (tmp_4[2]) * 0.4752F + static_cast<real32_T>(tmp_4[5]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[8]) * 0.873F) + rtb_states[18]);
        std::memset(&d_K[0], 0, 19U * sizeof(real32_T));
        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis = rtb_VectorConcatenate1_0[i_2];
          for (i = 0; i < 19; i++) {
            d_K[i] += b_K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis;
          }

          rtb_VectorConcatenate1_tmp += 19;
        }

        std::memcpy(&errorStateHat[0], &d_K[0], 19U * sizeof(real32_T));

        // 'errorStateEkf_function2:221' covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*(covP(16:18, idxEs) + ... 
        // 'errorStateEkf_function2:222'                 H(:, 1:3) * covP(1:3, idxEs)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          rtb_XAxis1 = H[rtb_VectorConcatenate1_tmp + 3];
          rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp];
          rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp + 6];
          i_2 = 0;
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            tmp1[i_2 + rtb_VectorConcatenate1_tmp] =
              ((stateEstimatorEskf_DW.covP[i + 1] * rtb_XAxis1 +
                stateEstimatorEskf_DW.covP[i] * rtb_UnitDelay_g) +
               stateEstimatorEskf_DW.covP[i + 2] * rtb_XAxis2) +
              stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 15];
            i_2 += 3;
            i += 19;
          }
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 19;
             rtb_VectorConcatenate1_tmp++) {
          rtb_XAxis = b_K[rtb_VectorConcatenate1_tmp + 19];
          rtb_XAxis1 = b_K[rtb_VectorConcatenate1_tmp];
          rtb_UnitDelay_g = b_K[rtb_VectorConcatenate1_tmp + 38];
          i_2 = 0;
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            covP_tmp = i_2 + rtb_VectorConcatenate1_tmp;
            stateEstimatorEskf_DW.covP[covP_tmp] -= (tmp1[i + 1] * rtb_XAxis +
              tmp1[i] * rtb_XAxis1) + tmp1[i + 2] * rtb_UnitDelay_g;
            i_2 += 19;
            i += 3;
          }
        }

        // Update the nominal state
        // 'errorStateEkf_function2:225' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
             rtb_VectorConcatenate1_tmp++) {
          rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
            errorStateHat[rtb_VectorConcatenate1_tmp + 3];
        }
      } else {
        // 'errorStateEkf_function2:226' else
        // 'errorStateEkf_function2:227' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(1:3, 1:3)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          for (i_2 = 0; i_2 < 15; i_2++) {
            tmp1_0[i_2 + 15 * rtb_VectorConcatenate1_tmp] = tmp1[19 *
              rtb_VectorConcatenate1_tmp + c_0[i_2]];
          }

          for (i_2 = 0; i_2 < 3; i_2++) {
            rtb_XAxis = 0.0F;
            for (i = 0; i < 15; i++) {
              c = c_0[i];
              rtb_XAxis += H[3 * c + rtb_VectorConcatenate1_tmp] * tmp1[19 * i_2
                + c];
            }

            rtb_VectorConcatenate_p[rtb_VectorConcatenate1_tmp + 3 * i_2] =
              rtu_measNoiseR[15 * i_2 + rtb_VectorConcatenate1_tmp] + rtb_XAxis;
          }
        }

        mrdiv_9ppBIzmt(tmp1_0, rtb_VectorConcatenate_p, K);

        // 'errorStateEkf_function2:228' errorStateHat(idxNoGpsEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_smxwJjrc(rtb_states[0], rtb_states[1], rtb_states[2],
                           rtb_states[3], tmp_4);
        rtb_VectorConcatenate1_0[0] = Divide[0] - (((static_cast<real32_T>
          (tmp_4[0]) * 0.4752F + static_cast<real32_T>(tmp_4[3]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[6]) * 0.873F) + rtb_states[16]);
        rtb_VectorConcatenate1_0[1] = Divide[1] - (((static_cast<real32_T>
          (tmp_4[1]) * 0.4752F + static_cast<real32_T>(tmp_4[4]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[7]) * 0.873F) + rtb_states[17]);
        rtb_VectorConcatenate1_0[2] = Divide[2] - (((static_cast<real32_T>
          (tmp_4[2]) * 0.4752F + static_cast<real32_T>(tmp_4[5]) * 0.1096F) +
          static_cast<real32_T>(tmp_4[8]) * 0.873F) + rtb_states[18]);
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          K_0[rtb_VectorConcatenate1_tmp] = 0.0F;
        }

        rtb_VectorConcatenate1_tmp = 0;
        for (i_2 = 0; i_2 < 3; i_2++) {
          rtb_XAxis = rtb_VectorConcatenate1_0[i_2];
          for (i = 0; i < 15; i++) {
            K_0[i] += K[i + rtb_VectorConcatenate1_tmp] * rtb_XAxis;
          }

          rtb_VectorConcatenate1_tmp += 15;
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] =
            K_0[rtb_VectorConcatenate1_tmp];
        }

        // 'errorStateEkf_function2:230' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(16:18, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:231'                 H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 3;
             rtb_VectorConcatenate1_tmp++) {
          rtb_XAxis1 = H[rtb_VectorConcatenate1_tmp + 3];
          rtb_UnitDelay_g = H[rtb_VectorConcatenate1_tmp];
          rtb_XAxis2 = H[rtb_VectorConcatenate1_tmp + 6];
          for (i_2 = 0; i_2 < 15; i_2++) {
            i = 19 * c_0[i_2];
            tmp1_0[rtb_VectorConcatenate1_tmp + 3 * i_2] =
              ((stateEstimatorEskf_DW.covP[i + 1] * rtb_XAxis1 +
                stateEstimatorEskf_DW.covP[i] * rtb_UnitDelay_g) +
               stateEstimatorEskf_DW.covP[i + 2] * rtb_XAxis2) +
              stateEstimatorEskf_DW.covP[(i + rtb_VectorConcatenate1_tmp) + 15];
          }
        }

        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          rtb_XAxis = K[rtb_VectorConcatenate1_tmp + 15];
          rtb_XAxis1 = K[rtb_VectorConcatenate1_tmp];
          rtb_UnitDelay_g = K[rtb_VectorConcatenate1_tmp + 30];
          for (i_2 = 0; i_2 < 15; i_2++) {
            tmp_5[rtb_VectorConcatenate1_tmp + 15 * i_2] =
              stateEstimatorEskf_DW.covP[19 * c_0[i_2] +
              c_0[rtb_VectorConcatenate1_tmp]] - ((tmp1_0[3 * i_2 + 1] *
              rtb_XAxis + tmp1_0[3 * i_2] * rtb_XAxis1) + tmp1_0[3 * i_2 + 2] *
              rtb_UnitDelay_g);
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
        // 'errorStateEkf_function2:234' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
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
      // 'errorStateEkf_function2:238' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:239' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:240' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      if (imuReady) {
        // 'errorStateEkf_function2:241' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
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
        // 'errorStateEkf_function2:242' else
        // 'errorStateEkf_function2:243' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:244'                 covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
        for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
             rtb_VectorConcatenate1_tmp++) {
          for (i_2 = 0; i_2 < 15; i_2++) {
            covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
            stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
              c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
          }
        }

        // 'errorStateEkf_function2:245' states(8:9) = 0;
        rtb_states[7] = 0.0F;
        rtb_states[8] = 0.0F;
      }

      // BusAssignment: '<Root>/Bus Assignment1' incorporates:
      //   BusAssignment: '<Root>/Bus Assignment'
      //   Constant: '<Root>/localNedMag_nd'

      // 'errorStateEkf_function2:248' ekfDebugOut.isAidingUsed.isMagUsed = true; 
      rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isMagUsed = true;
    }

    // Integrate optical flow data
    // 'errorStateEkf_function2:252' if(isOfValid)
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
      // 'errorStateEkf_function2:281' [states, covP] = applyOfVelCorr(states, ofNeVel_mps, covP, 7, idxEs, ... 
      // 'errorStateEkf_function2:282'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      // 'errorStateEkf_function2:554' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      rtb_XAxis = 1.0F / (stateEstimatorEskf_DW.covP[120] + rtu_measNoiseR[224]);

      // 'errorStateEkf_function2:555' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_VectorConcatenate_k2[0] - rtb_states[7];

      // 'errorStateEkf_function2:556' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:559' errorStateHat = zeros(19, 1, 'single');
      //  if NIS < innovGate
      // 'errorStateEkf_function2:562' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:563' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_UnitDelay_g = stateEstimatorEskf_DW.covP[i + 114] * rtb_XAxis;
        d_K[i] = rtb_UnitDelay_g;
        b_errorStateHat[i] = rtb_UnitDelay_g * rtb_XAxis1;
      }

      // 'errorStateEkf_function2:565' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
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
      // 'errorStateEkf_function2:568' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
          b_errorStateHat[rtb_VectorConcatenate1_tmp + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:571' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      updateQuatAndResetCovP_o483nOwE(qDelayed, &b_errorStateHat[0], covP);

      // 'errorStateEkf_function2:572' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:574' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      //  end
      // 'errorStateEkf_function2:283' [states, covP] = applyOfVelCorr(states, ofNeVel_mps, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:284'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      // 'errorStateEkf_function2:554' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      // 'errorStateEkf_function2:555' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_VectorConcatenate_k2[1] - rtb_states[8];

      // 'errorStateEkf_function2:556' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:559' errorStateHat = zeros(19, 1, 'single');
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

      rtb_XAxis = 1.0F / (b_covP[140] + rtu_measNoiseR[224]);

      //  if NIS < innovGate
      // 'errorStateEkf_function2:562' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:563' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_UnitDelay_g = b_covP[i + 133] * rtb_XAxis;
        d_K[i] = rtb_UnitDelay_g;
        b_errorStateHat[i] = rtb_UnitDelay_g * rtb_XAxis1;
      }

      // 'errorStateEkf_function2:565' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
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
      // 'errorStateEkf_function2:568' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
           rtb_VectorConcatenate1_tmp++) {
        rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
          b_errorStateHat[rtb_VectorConcatenate1_tmp + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:571' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      qDelayed[0] = rtb_states[0];
      qDelayed[1] = rtb_states[1];
      qDelayed[2] = rtb_states[2];
      qDelayed[3] = rtb_states[3];
      updateQuatAndResetCovP_o483nOwE(qDelayed, &b_errorStateHat[0], b_covP);

      // 'errorStateEkf_function2:572' states(1:4) = nomQuat;
      rtb_states[0] = qDelayed[0];
      rtb_states[1] = qDelayed[1];
      rtb_states[2] = qDelayed[2];
      rtb_states[3] = qDelayed[3];

      // 'errorStateEkf_function2:574' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
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
      // 'errorStateEkf_function2:286' ekfDebugOut.isAidingUsed.isFlowUsed = true; 
      rtb_ekfDebugOut_isAidingUsed__1 = true;
    }

    // Fuse GPS data if it is valid
    // 'errorStateEkf_function2:290' if(isGpsValid && estSmMode == enumStateEstimateMode.RUN) 
    if (static_cast<boolean_T>((estSmModeOut == enumStateEstimateMode::RUN) &
         dhSensorIn_sensorValidity_isGps)) {
      // 'errorStateEkf_function2:291' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:292'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 4, idxEs, ... 
      // 'errorStateEkf_function2:293'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:294'             ekfParams.nisParams.nisNedPosAndVel(1)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 4.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:296' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:297'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 1, usedFlag); 
      rtb_ekfDebugOut_isAidingUsed_is = imuReady;

      // 'errorStateEkf_function2:299' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:300'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 5, idxEs, ... 
      // 'errorStateEkf_function2:301'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:302'             ekfParams.nisParams.nisNedPosAndVel(2)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 5.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:304' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:305'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 2, usedFlag); 
      if (imuReady) {
        rtb_ekfDebugOut_isAidingUsed_is = static_cast<uint8_T>
          (rtb_ekfDebugOut_isAidingUsed_is | 2);
      }

      // 'errorStateEkf_function2:307' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:308'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 6, idxEs, ... 
      // 'errorStateEkf_function2:309'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:310'             ekfParams.nisParams.nisNedPosAndVel(3)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 6.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        3.68F);

      //  if isBaroValid
      //      [states, covP, usedFlag] = applyBaroBiasCorr(states, nedPosAndVel(3), baroAlt_m, covP, idxEs, ... 
      //          idxEs2, idxNs2, measNoiseR(6, 6) + measNoiseR(10, 10) );
      //  end
      // 'errorStateEkf_function2:316' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:317'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 3, usedFlag); 
      if (imuReady) {
        rtb_ekfDebugOut_isAidingUsed_is = static_cast<uint8_T>
          (rtb_ekfDebugOut_isAidingUsed_is | 4);
      }

      // 'errorStateEkf_function2:319' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:320'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 7, idxEs, ... 
      // 'errorStateEkf_function2:321'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:322'             ekfParams.nisParams.nisNedPosAndVel(4)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 7.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:324' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:325'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 4, usedFlag); 
      if (imuReady) {
        rtb_ekfDebugOut_isAidingUsed_is = static_cast<uint8_T>
          (rtb_ekfDebugOut_isAidingUsed_is | 8);
      }

      // 'errorStateEkf_function2:327' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:328'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:329'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:330'             ekfParams.nisParams.nisNedPosAndVel(5)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 8.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:332' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:333'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 5, usedFlag); 
      if (imuReady) {
        rtb_ekfDebugOut_isAidingUsed_is = static_cast<uint8_T>
          (rtb_ekfDebugOut_isAidingUsed_is | 16);
      }

      // 'errorStateEkf_function2:335' [states, covP, usedFlag] = ...
      // 'errorStateEkf_function2:336'             applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 9, idxEs, ... 
      // 'errorStateEkf_function2:337'             idxEs2, idxNs2, measNoiseR, ... 
      // 'errorStateEkf_function2:338'             ekfParams.nisParams.nisNedPosAndVel(6)); 
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

      imuReady = applyGpsPosAndVelCorr_KKxnC7ep(rtb_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 9.0, tmp, tmp_0, tmp_1, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:340' ekfDebugOut.isAidingUsed.isGpsUsed = ...
      // 'errorStateEkf_function2:341'             bitset(ekfDebugOut.isAidingUsed.isGpsUsed, 6, usedFlag); 
      if (imuReady) {
        rtb_ekfDebugOut_isAidingUsed_is = static_cast<uint8_T>
          (rtb_ekfDebugOut_isAidingUsed_is | 32);
      }

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

    // Fuse Baro data if it is valid
    // 'errorStateEkf_function2:364' if(isBaroValid)
    if (stateModeReady) {
      // 'errorStateEkf_function2:365' iS = 1/(covP(19, 19) - covP(19, 6) + covP(6, 6) - covP(6, 19) + measNoiseR(10, 10)); 
      rtb_XAxis = 1.0F / ((((stateEstimatorEskf_DW.covP[360] -
        stateEstimatorEskf_DW.covP[113]) + stateEstimatorEskf_DW.covP[100]) -
                           stateEstimatorEskf_DW.covP[347]) + rtu_measNoiseR[144]);

      // 'errorStateEkf_function2:366' nu = baroAlt_m  + states(7) - states(20); 
      rtb_Product3_l = (rtb_Product3_l + rtb_states[6]) - rtb_states[19];

      //  iS = 1/(covP(6, 6) + measNoiseR(10, 10));
      //  baroAltCorrected_m = baroAlt_m - states(20);
      //  baroAltEst_m = -states(7);
      //  nu = baroAlt_m + states(7);
      //  nu = baroAltCorrected_m - baroAltEst_m;
      // 'errorStateEkf_function2:372' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:374' if NIS <= 25
      if (rtb_Product3_l * rtb_Product3_l * rtb_XAxis <= 25.0F) {
        // 'errorStateEkf_function2:375' ekfDebugOut.isAidingUsed.isBaroUsed = true; 
        mtf01pDistValidityFlag = true;

        // 'errorStateEkf_function2:377' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        imuReady = (estSmModeOut == enumStateEstimateMode::RUN) |
          dhSensorIn_sensorValidity_isOfV;
        if (imuReady) {
          // 'errorStateEkf_function2:378' K = (covP(idxEs, 19) - covP(idxEs, 6)).*iS; 
          //  K = (-covP(idxEs, 6)).*iS;
          // 'errorStateEkf_function2:380' errorStateHat(idxEs) = K*nu;
          //  covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs);
          // 'errorStateEkf_function2:382' covP(idxEs, idxEs) = covP(idxEs, idxEs) - ... 
          // 'errorStateEkf_function2:383'                     (-K * (covP(6, idxEs) - covP(19, idxEs))); 
          i = 0;
          for (i_1 = 0; i_1 < 19; i_1++) {
            rtb_UnitDelay_g = (stateEstimatorEskf_DW.covP[i_1 + 342] -
                               stateEstimatorEskf_DW.covP[i_1 + 95]) * rtb_XAxis;
            errorStateHat[i_1] = rtb_UnitDelay_g * rtb_Product3_l;
            d_K[i_1] = -rtb_UnitDelay_g;
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

          //  covP(idxEs,idxEs) = covP(idxEs,idxEs) + ...
          //                      K*covP(6,idxEs);
          // Update the nominal state
          // 'errorStateEkf_function2:387' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
              errorStateHat[rtb_VectorConcatenate1_tmp + 3];
          }
        } else {
          // 'errorStateEkf_function2:388' else
          // 'errorStateEkf_function2:389' K = (covP(idxNoGpsEs, 19) - covP(idxNoGpsEs, 6)).*iS; 
          //  K = (-covP(idxNoGpsEs, 6)).*iS;
          // 'errorStateEkf_function2:391' errorStateHat(idxNoGpsEs) = K*nu;
          //            covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs); 
          // 'errorStateEkf_function2:393' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - ... 
          // 'errorStateEkf_function2:394'                     (-K * (covP(6, idxNoGpsEs) - covP(19, idxNoGpsEs))); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            c = c_0[rtb_VectorConcatenate1_tmp];
            rtb_XAxis1 = (stateEstimatorEskf_DW.covP[c + 342] -
                          stateEstimatorEskf_DW.covP[c + 95]) * rtb_XAxis;
            errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] = rtb_XAxis1 *
              rtb_Product3_l;
            K_0[rtb_VectorConcatenate1_tmp] = -rtb_XAxis1;
            i_2 = 19 * c;
            tmp_3[rtb_VectorConcatenate1_tmp] = stateEstimatorEskf_DW.covP[i_2 +
              5] - stateEstimatorEskf_DW.covP[i_2 + 18];
          }

          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              tmp_5[i_2 + 15 * rtb_VectorConcatenate1_tmp] =
                stateEstimatorEskf_DW.covP[19 * c_0[rtb_VectorConcatenate1_tmp]
                + c_0[i_2]] - K_0[i_2] * tmp_3[rtb_VectorConcatenate1_tmp];
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

          //  covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) + ...
          //      K * (covP(6, idxNoGpsEs));
          // Update the nominal state
          // 'errorStateEkf_function2:398' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
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
        // 'errorStateEkf_function2:402' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        qDelayed[0] = rtb_states[0];
        qDelayed[1] = rtb_states[1];
        qDelayed[2] = rtb_states[2];
        qDelayed[3] = rtb_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:403' states(1:4) = nomQuat;
        rtb_states[0] = qDelayed[0];
        rtb_states[1] = qDelayed[1];
        rtb_states[2] = qDelayed[2];
        rtb_states[3] = qDelayed[3];

        // 'errorStateEkf_function2:404' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (imuReady) {
          // 'errorStateEkf_function2:405' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
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
          // 'errorStateEkf_function2:406' else
          // 'errorStateEkf_function2:407' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:408'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
                c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:409' states(8:9) = 0;
          rtb_states[7] = 0.0F;
          rtb_states[8] = 0.0F;
        }
      }
    }

    // Fuse Lidar data if it is valid
    // 'errorStateEkf_function2:415' if(isLidarValid)
    if (dhSensorIn_sensorValidity_isLid) {
      // 'errorStateEkf_function2:416' iS = 1/(covP(6, 6) + measNoiseR(11, 11)); 
      rtb_XAxis = 1.0F / (stateEstimatorEskf_DW.covP[100] + rtu_measNoiseR[160]);

      // 'errorStateEkf_function2:417' nu = lidarAgl_m  + states(7);
      rtb_Product3_l = rtb_Product2_c + rtb_states[6];

      // 'errorStateEkf_function2:418' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:420' if NIS < 27
      if (rtb_Product3_l * rtb_Product3_l * rtb_XAxis < 27.0F) {
        // 'errorStateEkf_function2:421' ekfDebugOut.isAidingUsed.isLidarUsed = true; 
        rtb_ekfDebugOut_isAidingUsed__0 = true;

        // 'errorStateEkf_function2:422' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        imuReady = (estSmModeOut == enumStateEstimateMode::RUN) |
          dhSensorIn_sensorValidity_isOfV;
        if (imuReady) {
          // 'errorStateEkf_function2:423' K = -covP(idxEs, 6).*iS;
          // 'errorStateEkf_function2:424' errorStateHat(idxEs) = K*nu;
          // 'errorStateEkf_function2:426' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (-K*covP(6, idxEs)); 
          for (i = 0; i < 19; i++) {
            rtb_UnitDelay_g = -stateEstimatorEskf_DW.covP[i + 95] * rtb_XAxis;
            errorStateHat[i] = rtb_UnitDelay_g * rtb_Product3_l;
            d_K[i] = -rtb_UnitDelay_g;
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
          // 'errorStateEkf_function2:429' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 16;
               rtb_VectorConcatenate1_tmp++) {
            rtb_states[rtb_VectorConcatenate1_tmp + 4] +=
              errorStateHat[rtb_VectorConcatenate1_tmp + 3];
          }
        } else {
          // 'errorStateEkf_function2:430' else
          // 'errorStateEkf_function2:431' K = -covP(idxNoGpsEs, 6).*iS;
          // 'errorStateEkf_function2:432' errorStateHat(idxNoGpsEs) = K*nu;
          // 'errorStateEkf_function2:434' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - (-K*covP(6, idxNoGpsEs)); 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            rtb_XAxis1 =
              -stateEstimatorEskf_DW.covP[c_0[rtb_VectorConcatenate1_tmp] + 95] *
              rtb_XAxis;
            errorStateHat[c_0[rtb_VectorConcatenate1_tmp]] = rtb_XAxis1 *
              rtb_Product3_l;
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
          // 'errorStateEkf_function2:437' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
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
        // 'errorStateEkf_function2:441' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        qDelayed[0] = rtb_states[0];
        qDelayed[1] = rtb_states[1];
        qDelayed[2] = rtb_states[2];
        qDelayed[3] = rtb_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_o483nOwE(qDelayed, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:442' states(1:4) = nomQuat;
        rtb_states[0] = qDelayed[0];
        rtb_states[1] = qDelayed[1];
        rtb_states[2] = qDelayed[2];
        rtb_states[3] = qDelayed[3];

        // 'errorStateEkf_function2:443' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (imuReady) {
          // 'errorStateEkf_function2:444' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
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
          // 'errorStateEkf_function2:445' else
          // 'errorStateEkf_function2:446' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:447'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (rtb_VectorConcatenate1_tmp = 0; rtb_VectorConcatenate1_tmp < 15;
               rtb_VectorConcatenate1_tmp++) {
            for (i_2 = 0; i_2 < 15; i_2++) {
              covP_tmp = 19 * c_0[rtb_VectorConcatenate1_tmp] + c_0[i_2];
              stateEstimatorEskf_DW.covP[covP_tmp] = (covP[19 * c_0[i_2] +
                c_0[rtb_VectorConcatenate1_tmp]] + covP[covP_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:448' states(8:9) = 0;
          rtb_states[7] = 0.0F;
          rtb_states[8] = 0.0F;
        }
      }
    }

    // 'errorStateEkf_function2:453' ekfDebugOut.dhStates(1:20) = states;
    std::memcpy(&rtb_ekfDebugOut_dhStates[0], &rtb_states[0], 20U * sizeof
                (real32_T));

    // Compute Body To NED DCM
    //  dcmBodyToNed = quatToDcm_function(states(1:4));
    //  else
    //      states = prevStates;
    //  end
  }

  // BusAssignment: '<Root>/Bus Assignment1' incorporates:
  //   BusAssignment: '<S1>/Bus Assignment'

  rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isGpsUsed =
    rtb_ekfDebugOut_isAidingUsed_is;
  rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isBaroUsed =
    mtf01pDistValidityFlag;
  rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isLidarUsed =
    rtb_ekfDebugOut_isAidingUsed__0;
  rty_stateEstimatorDebug->ekfDebugData.isAidingUsed.isFlowUsed =
    rtb_ekfDebugOut_isAidingUsed__1;
  std::memcpy(&rty_stateEstimatorDebug->ekfDebugData.dhStates[0],
              &rtb_ekfDebugOut_dhStates[0], 23U * sizeof(real32_T));
  rty_stateEstimatorDebug->ekfDebugData.dhBufferDebugData =
    stateEstimatorEskf_DW.dhBufferDebug;
  rty_stateEstimatorDebug->stateEstInitPct =
    stateEstimatorEskf_DW.stateEstInitPct;
  rty_stateEstimatorDebug->smMode = mode;

  // MATLAB Function: '<S14>/OutputPredictor' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   BusCreator: '<Root>/Bus Creator1'

  // MATLAB Function 'EKF/DelayedHorizonOutput/OutputPredictor': '<S18>:1'
  // '<S18>:1:4' statesOut = statesIn;
  std::memcpy(&rty_states[0], &rtb_states[0], 20U * sizeof(real32_T));

  // '<S18>:1:5' [quatOut, posOut, velOut, ~, ~, ~] = outputPredictor_function(statesIn, isImuValid, imuTimeIn_ms, ... 
  // '<S18>:1:6'                                                       imuIn, fusionTime_ms, gEarth_mps2, ... 
  // '<S18>:1:7'                                                       ekfParams.outputPredictorParams); 
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
    rtb_Product2_c = norm_NoMIKEmk(stateEstimatorEskf_DW.qCurrent);

    // 'outputPredictor_function:51' if qNorm > 1e-7
    if (rtb_Product2_c > 1.0E-7) {
      // 'outputPredictor_function:52' qCurrent = qCurrent/qNorm;
      stateEstimatorEskf_DW.qCurrent[0] /= rtb_Product2_c;
      stateEstimatorEskf_DW.qCurrent[1] /= rtb_Product2_c;
      stateEstimatorEskf_DW.qCurrent[2] /= rtb_Product2_c;
      stateEstimatorEskf_DW.qCurrent[3] /= rtb_Product2_c;
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
    stateEstimatorEskf_DW.count_aw = 0U;

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
    if (stateEstimatorEskf_DW.count_aw == 0) {
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
      stateEstimatorEskf_DW.count_aw = 1U;

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
      if ((stateEstimatorEskf_DW.count_aw == 50) && uMultiWordEq
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
        rtb_Product3_l = (rtb_states[7] -
                          stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp])
          * 0.02F;
        Divide[1] = (rtb_states[5] -
                     stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 1])
          * 0.02F;
        rtb_Sum_m1_idx_1 = (rtb_states[8] -
                            stateEstimatorEskf_DW.velBuf[rtb_VectorConcatenate1_tmp
                            + 1]) * 0.02F;
        Divide[2] = (rtb_states[6] -
                     stateEstimatorEskf_DW.posBuf[rtb_VectorConcatenate1_tmp + 2])
          * 0.02F;
        rtb_Sum_m1_idx_2 = (rtb_states[9] -
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
          rtb_XAxis1 = stateEstimatorEskf_DW.posBuf[i_2 + 2] + Divide[2];
          stateEstimatorEskf_DW.posBuf[i_2] += Divide[0];
          stateEstimatorEskf_DW.posBuf[i_2 + 1] = rtb_XAxis;
          stateEstimatorEskf_DW.posBuf[i_2 + 2] = rtb_XAxis1;

          // 'outputPredictor_function:155' velBuf(:,idx) = velBuf(:,idx) + velCorrection; 
          rtb_XAxis = stateEstimatorEskf_DW.velBuf[i_2 + 1] + rtb_Sum_m1_idx_1;
          rtb_XAxis1 = stateEstimatorEskf_DW.velBuf[i_2 + 2] + rtb_Sum_m1_idx_2;
          stateEstimatorEskf_DW.velBuf[i_2] += rtb_Product3_l;
          stateEstimatorEskf_DW.velBuf[i_2 + 1] = rtb_XAxis;
          stateEstimatorEskf_DW.velBuf[i_2 + 2] = rtb_XAxis1;

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
        stateEstimatorEskf_DW.vCurrent[0] += rtb_Product3_l;
        stateEstimatorEskf_DW.pCurrent[1] += Divide[1];
        stateEstimatorEskf_DW.vCurrent[1] += rtb_Sum_m1_idx_1;
        stateEstimatorEskf_DW.pCurrent[2] += Divide[2];
        stateEstimatorEskf_DW.vCurrent[2] += rtb_Sum_m1_idx_2;

        //  -------------------------------------------------------------
        //  ATTITUDE ERROR AT DELAYED FUSION HORIZON
        //  -------------------------------------------------------------
        // 'outputPredictor_function:173' qDelayed = quatBuf(:,tail);
        // 'outputPredictor_function:174' qEskf = states(1:4);
        // 'outputPredictor_function:176' qNorm = norm(qDelayed);
        rtb_VectorConcatenate1_tmp = (stateEstimatorEskf_DW.tail_j - 1) << 2;
        rtb_Product2_c = norm_NoMIKEmk
          (&stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp]);

        // 'outputPredictor_function:177' if qNorm > 1e-7
        if (rtb_Product2_c > 1.0E-7) {
          // 'outputPredictor_function:178' qDelayed = qDelayed/qNorm;
          qDelayed[0] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp]
            / rtb_Product2_c;
          qDelayed[1] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 1] / rtb_Product2_c;
          qDelayed[2] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 2] / rtb_Product2_c;
          qDelayed[3] = stateEstimatorEskf_DW.quatBuf[rtb_VectorConcatenate1_tmp
            + 3] / rtb_Product2_c;
        } else {
          // 'outputPredictor_function:179' else
          // 'outputPredictor_function:180' qDelayed = single([1;0;0;0]);
          qDelayed[0] = 1.0F;
          qDelayed[1] = 0.0F;
          qDelayed[2] = 0.0F;
          qDelayed[3] = 0.0F;
        }

        // 'outputPredictor_function:183' qNorm = norm(qEskf);
        rtb_Product2_c = norm_NoMIKEmk(&rtb_states[0]);

        // 'outputPredictor_function:184' if qNorm > 1e-7
        if (rtb_Product2_c > 1.0E-7) {
          // 'outputPredictor_function:185' qEskf = qEskf/qNorm;
          rtb_Product3_l = rtb_states[0] / rtb_Product2_c;
          rtb_XAxis1 = rtb_states[1] / rtb_Product2_c;
          rtb_UnitDelay_g = rtb_states[2] / rtb_Product2_c;
          rtb_Product2_c = rtb_states[3] / rtb_Product2_c;
        } else {
          // 'outputPredictor_function:186' else
          // 'outputPredictor_function:187' qEskf = single([1;0;0;0]);
          rtb_Product3_l = 1.0F;
          rtb_XAxis1 = 0.0F;
          rtb_UnitDelay_g = 0.0F;
          rtb_Product2_c = 0.0F;
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
        qError[0] = ((qDelayed[0] * rtb_Product3_l - -qDelayed[1] * rtb_XAxis1)
                     - -qDelayed[2] * rtb_UnitDelay_g) - -qDelayed[3] *
          rtb_Product2_c;

        // 'outputPredictor_function:338' q(2) = q1(1)*q2(2) + ...
        // 'outputPredictor_function:339'        q1(2)*q2(1) + ...
        // 'outputPredictor_function:340'        q1(3)*q2(4) - ...
        // 'outputPredictor_function:341'        q1(4)*q2(3);
        qError[1] = ((qDelayed[0] * rtb_XAxis1 + rtb_Product3_l * -qDelayed[1])
                     + -qDelayed[2] * rtb_Product2_c) - rtb_UnitDelay_g *
          -qDelayed[3];

        // 'outputPredictor_function:343' q(3) = q1(1)*q2(3) - ...
        // 'outputPredictor_function:344'        q1(2)*q2(4) + ...
        // 'outputPredictor_function:345'        q1(3)*q2(1) + ...
        // 'outputPredictor_function:346'        q1(4)*q2(2);
        qError[2] = ((qDelayed[0] * rtb_UnitDelay_g - -qDelayed[1] *
                      rtb_Product2_c) + rtb_Product3_l * -qDelayed[2]) +
          rtb_XAxis1 * -qDelayed[3];

        // 'outputPredictor_function:348' q(4) = q1(1)*q2(4) + ...
        // 'outputPredictor_function:349'        q1(2)*q2(3) - ...
        // 'outputPredictor_function:350'        q1(3)*q2(2) + ...
        // 'outputPredictor_function:351'        q1(4)*q2(1);
        qError[3] = ((qDelayed[0] * rtb_Product2_c + -qDelayed[1] *
                      rtb_UnitDelay_g) - rtb_XAxis1 * -qDelayed[2]) +
          rtb_Product3_l * -qDelayed[3];

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
        rtb_Product2_c = norm_94qjDDKI(&qError[1]);

        // 'outputPredictor_function:213' if qErrorVectorNorm > 1e-7
        if (rtb_Product2_c > 1.0E-7) {
          // 'outputPredictor_function:214' attitudeErrorNorm = ...
          // 'outputPredictor_function:215'                 single(2)*atan2(qErrorVectorNorm,qError(1)); 
          // 'outputPredictor_function:217' attitudeError = ...
          // 'outputPredictor_function:218'                 (attitudeErrorNorm/qErrorVectorNorm)*qErrorVector; 
          rtb_Product2_c = 2.0F * std::atan2(rtb_Product2_c, qError[0]) /
            rtb_Product2_c;

          // 'outputPredictor_function:220' deltaAngleCorr = attGain*attitudeError; 
          rtb_CastToSingle[0] = rtb_Product2_c * qError[1] * 0.01F;
          rtb_CastToSingle[1] = rtb_Product2_c * qError[2] * 0.01F;
          rtb_CastToSingle[2] = rtb_Product2_c * qError[3] * 0.01F;
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
        stateEstimatorEskf_DW.count_aw = 49U;
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
      rtb_Product2_c = stateEstimatorEskf_DW.qCurrent[1] *
        stateEstimatorEskf_DW.qCurrent[1];

      // 'outputPredictor_function:364' q2q2 = q2*q2;
      rtb_Product3_l = stateEstimatorEskf_DW.qCurrent[2] *
        stateEstimatorEskf_DW.qCurrent[2];

      // 'outputPredictor_function:365' q3q3 = q3*q3;
      rtb_XAxis = stateEstimatorEskf_DW.qCurrent[3] *
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
      rtb_Sum_m1_idx_1 = stateEstimatorEskf_DW.qCurrent[1] *
        stateEstimatorEskf_DW.qCurrent[2];

      // 'outputPredictor_function:372' q1q3 = q1*q3;
      rtb_Sum_m1_idx_2 = stateEstimatorEskf_DW.qCurrent[1] *
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
      stateEstimatorEskf_DW.vCurrent[0] += (((1.0F - (rtb_Product3_l + rtb_XAxis)
        * 2.0F) * rtb_dcmBodyToNed_idx_7 + (rtb_Sum_m1_idx_1 - rtb_UnitDelay_g) *
        2.0F * rtb_dcmBodyToNed_idx_2) + (rtb_Sum_m1_idx_2 + rtb_XAxis2) * 2.0F *
        rtb_dcmBodyToNed_idx_5) * rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[0] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0] - rtb_states[10])
        * rtu_imuData->dtImuTime_s;
      stateEstimatorEskf_DW.vCurrent[1] += (((1.0F - (rtb_Product2_c + rtb_XAxis)
        * 2.0F) * rtb_dcmBodyToNed_idx_2 + (rtb_Sum_m1_idx_1 + rtb_UnitDelay_g) *
        2.0F * rtb_dcmBodyToNed_idx_7) + (q2q3 - rtb_XAxis1) * 2.0F *
        rtb_dcmBodyToNed_idx_5) * rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[1] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1] - rtb_states[11])
        * rtu_imuData->dtImuTime_s;
      stateEstimatorEskf_DW.vCurrent[2] += ((((rtb_Sum_m1_idx_2 - rtb_XAxis2) *
        2.0F * rtb_dcmBodyToNed_idx_7 + (q2q3 + rtb_XAxis1) * 2.0F *
        rtb_dcmBodyToNed_idx_2) + (1.0F - (rtb_Product2_c + rtb_Product3_l) *
        2.0F) * rtb_dcmBodyToNed_idx_5) + *rtu_gEarth_mps2) *
        rtu_imuData->dtImuTime_s;
      rtb_CastToSingle[2] +=
        (stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2] - rtb_states[12])
        * rtu_imuData->dtImuTime_s;

      // 'outputPredictor_function:267' dThetaNorm = norm(dTheta);
      rtb_XAxis = norm_94qjDDKI(rtb_CastToSingle);

      // 'outputPredictor_function:269' if dThetaNorm > 1e-7
      if (rtb_XAxis > 1.0E-7) {
        // 'outputPredictor_function:270' dThetaUnit = dTheta/dThetaNorm;
        // 'outputPredictor_function:272' dq = ...
        // 'outputPredictor_function:273'         [cos(single(0.5)*dThetaNorm);
        // 'outputPredictor_function:274'          sin(single(0.5)*dThetaNorm)*dThetaUnit]; 
        rtb_Product3_l = 0.5F * rtb_XAxis;
        rtb_Product2_c = std::sin(rtb_Product3_l);
        rtb_Product3_l = std::cos(rtb_Product3_l);
        rtb_XAxis1 = rtb_CastToSingle[0] / rtb_XAxis * rtb_Product2_c;
        rtb_UnitDelay_g = rtb_CastToSingle[1] / rtb_XAxis * rtb_Product2_c;
        rtb_Product2_c *= rtb_CastToSingle[2] / rtb_XAxis;

        // 'outputPredictor_function:276' qCurrent = quatMultiplyLocal(qCurrent,dq); 
        rtb_XAxis = stateEstimatorEskf_DW.qCurrent[0];
        rtb_XAxis2 = stateEstimatorEskf_DW.qCurrent[1];
        rtb_Sum_m1_idx_1 = stateEstimatorEskf_DW.qCurrent[2];

        // 'outputPredictor_function:331' q = zeros(4,1,'single');
        // 'outputPredictor_function:333' q(1) = q1(1)*q2(1) - ...
        // 'outputPredictor_function:334'        q1(2)*q2(2) - ...
        // 'outputPredictor_function:335'        q1(3)*q2(3) - ...
        // 'outputPredictor_function:336'        q1(4)*q2(4);
        stateEstimatorEskf_DW.qCurrent[0] = ((stateEstimatorEskf_DW.qCurrent[0] *
          rtb_Product3_l - stateEstimatorEskf_DW.qCurrent[1] * rtb_XAxis1) -
          stateEstimatorEskf_DW.qCurrent[2] * rtb_UnitDelay_g) -
          stateEstimatorEskf_DW.qCurrent[3] * rtb_Product2_c;

        // 'outputPredictor_function:338' q(2) = q1(1)*q2(2) + ...
        // 'outputPredictor_function:339'        q1(2)*q2(1) + ...
        // 'outputPredictor_function:340'        q1(3)*q2(4) - ...
        // 'outputPredictor_function:341'        q1(4)*q2(3);
        stateEstimatorEskf_DW.qCurrent[1] = ((rtb_XAxis * rtb_XAxis1 +
          rtb_Product3_l * stateEstimatorEskf_DW.qCurrent[1]) +
          stateEstimatorEskf_DW.qCurrent[2] * rtb_Product2_c) - rtb_UnitDelay_g *
          stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:343' q(3) = q1(1)*q2(3) - ...
        // 'outputPredictor_function:344'        q1(2)*q2(4) + ...
        // 'outputPredictor_function:345'        q1(3)*q2(1) + ...
        // 'outputPredictor_function:346'        q1(4)*q2(2);
        stateEstimatorEskf_DW.qCurrent[2] = ((rtb_XAxis * rtb_UnitDelay_g -
          rtb_XAxis2 * rtb_Product2_c) + rtb_Product3_l *
          stateEstimatorEskf_DW.qCurrent[2]) + rtb_XAxis1 *
          stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:348' q(4) = q1(1)*q2(4) + ...
        // 'outputPredictor_function:349'        q1(2)*q2(3) - ...
        // 'outputPredictor_function:350'        q1(3)*q2(2) + ...
        // 'outputPredictor_function:351'        q1(4)*q2(1);
        stateEstimatorEskf_DW.qCurrent[3] = ((rtb_XAxis * rtb_Product2_c +
          rtb_XAxis2 * rtb_UnitDelay_g) - rtb_XAxis1 * rtb_Sum_m1_idx_1) +
          rtb_Product3_l * stateEstimatorEskf_DW.qCurrent[3];

        // 'outputPredictor_function:278' qNorm = norm(qCurrent);
        rtb_Product2_c = norm_NoMIKEmk(stateEstimatorEskf_DW.qCurrent);

        // 'outputPredictor_function:279' if qNorm > 1e-7
        if (rtb_Product2_c > 1.0E-7) {
          // 'outputPredictor_function:280' qCurrent = qCurrent/qNorm;
          stateEstimatorEskf_DW.qCurrent[0] /= rtb_Product2_c;
          stateEstimatorEskf_DW.qCurrent[1] /= rtb_Product2_c;
          stateEstimatorEskf_DW.qCurrent[2] /= rtb_Product2_c;
          stateEstimatorEskf_DW.qCurrent[3] /= rtb_Product2_c;
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
      if (stateEstimatorEskf_DW.count_aw == 50) {
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
        qY = stateEstimatorEskf_DW.count_aw + 1U;
        if (stateEstimatorEskf_DW.count_aw + 1U > 65535U) {
          qY = 65535U;
        }

        stateEstimatorEskf_DW.count_aw = static_cast<uint16_T>(qY);
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

  // '<S18>:1:8' statesOut(1:4) = quatOut;
  rty_states[0] = stateEstimatorEskf_DW.qCurrent[0];
  rty_states[1] = stateEstimatorEskf_DW.qCurrent[1];
  rty_states[2] = stateEstimatorEskf_DW.qCurrent[2];
  rty_states[3] = stateEstimatorEskf_DW.qCurrent[3];

  // '<S18>:1:9' statesOut(5:7) = posOut;
  // '<S18>:1:10' statesOut(8:10) = velOut;
  rty_states[4] = stateEstimatorEskf_DW.pCurrent[0];
  rty_states[7] = stateEstimatorEskf_DW.vCurrent[0];
  rty_states[5] = stateEstimatorEskf_DW.pCurrent[1];
  rty_states[8] = stateEstimatorEskf_DW.vCurrent[1];
  rty_states[6] = stateEstimatorEskf_DW.pCurrent[2];
  rty_states[9] = stateEstimatorEskf_DW.vCurrent[2];

  // End of MATLAB Function: '<S14>/OutputPredictor'

  // MATLAB Function: '<S1>/QuatToDCM'
  // MATLAB Function 'EKF/QuatToDCM': '<S17>:1'
  // '<S17>:1:2' dcmBodyToNed = quatToDcm_function(quat);
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
  rtb_UnitDelay_g = 1.0F - (rty_states[2] * rty_states[2] + rty_states[3] *
    rty_states[3]) * 2.0F;
  rtb_XAxis2 = (rty_states[1] * rty_states[2] - rty_states[0] * rty_states[3]) *
    2.0F;
  rtb_Sum_m1_idx_1 = (rty_states[1] * rty_states[3] + rty_states[0] *
                      rty_states[2]) * 2.0F;
  rtb_Sum_m1_idx_2 = (rty_states[1] * rty_states[2] + rty_states[0] *
                      rty_states[3]) * 2.0F;
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

  // Sqrt: '<S62>/sqrt' incorporates:
  //   Product: '<S63>/Product'
  //   Product: '<S63>/Product1'
  //   Product: '<S63>/Product2'
  //   Product: '<S63>/Product3'
  //   Sum: '<S63>/Sum'

  rtb_Product3_l = std::sqrt(((rty_states[0] * rty_states[0] + rty_states[1] *
    rty_states[1]) + rty_states[2] * rty_states[2]) + rty_states[3] *
    rty_states[3]);

  // Product: '<S61>/Product'
  rtb_XAxis = rty_states[0] / rtb_Product3_l;

  // Product: '<S61>/Product1'
  rtb_XAxis1 = rty_states[1] / rtb_Product3_l;

  // Product: '<S61>/Product2'
  rtb_Product2_c = rty_states[2] / rtb_Product3_l;

  // Product: '<S61>/Product3'
  rtb_Product3_l = rty_states[3] / rtb_Product3_l;

  // Gain: '<S53>/Gain' incorporates:
  //   Product: '<S53>/Product1'
  //   Product: '<S53>/Product2'
  //   Sum: '<S53>/Sum'

  rtb_VectorConcatenate_p[6] = (rtb_XAxis1 * rtb_Product3_l - rtb_XAxis *
    rtb_Product2_c) * 2.0F;

  // If: '<S22>/If' incorporates:
  //   Concatenate: '<S60>/Vector Concatenate'
  //   Gain: '<S26>/Gain1'
  //   Selector: '<S26>/Selector1'

  if (static_cast<boolean_T>((-rtb_VectorConcatenate_p[6] >= 1.0F) |
       (-rtb_VectorConcatenate_p[6] <= -1.0F))) {
    // Outputs for IfAction SubSystem: '<S22>/AxisRotZeroR3' incorporates:
    //   ActionPort: '<S25>/Action Port'

    // Fcn: '<S25>/Fcn1' incorporates:
    //   Gain: '<S26>/Gain3'
    //   Gain: '<S54>/Gain'
    //   Product: '<S54>/Product2'
    //   Product: '<S54>/Product3'
    //   Product: '<S55>/Product'
    //   Product: '<S55>/Product1'
    //   Product: '<S55>/Product2'
    //   Product: '<S55>/Product3'
    //   Sum: '<S54>/Sum'
    //   Sum: '<S55>/Sum'

    rtb_Merge_idx_0 = std::atan2(-((rtb_XAxis1 * rtb_Product2_c - rtb_Product3_l
      * rtb_XAxis) * 2.0F), ((rtb_XAxis * rtb_XAxis - rtb_XAxis1 * rtb_XAxis1) +
      rtb_Product2_c * rtb_Product2_c) - rtb_Product3_l * rtb_Product3_l);

    // If: '<S32>/If' incorporates:
    //   Constant: '<S33>/Constant'
    //   Constant: '<S34>/Constant'
    //   Fcn: '<S25>/Fcn2'

    if (-rtb_VectorConcatenate_p[6] > 1.0F) {
      // Outputs for IfAction SubSystem: '<S32>/If Action Subsystem' incorporates:
      //   ActionPort: '<S33>/Action Port'

      rtb_XAxis = 1.0F;

      // End of Outputs for SubSystem: '<S32>/If Action Subsystem'
    } else if (-rtb_VectorConcatenate_p[6] < -1.0F) {
      // Outputs for IfAction SubSystem: '<S32>/If Action Subsystem1' incorporates:
      //   ActionPort: '<S34>/Action Port'

      rtb_XAxis = 1.0F;

      // End of Outputs for SubSystem: '<S32>/If Action Subsystem1'
    } else {
      rtb_XAxis = -rtb_VectorConcatenate_p[6];
    }

    rtb_Merge_idx_1 = std::asin(rtb_XAxis);

    // End of If: '<S32>/If'

    // Fcn: '<S25>/Fcn3'
    rtb_Product2_c = 0.0F;

    // End of Outputs for SubSystem: '<S22>/AxisRotZeroR3'
  } else {
    // Product: '<S51>/Product3' incorporates:
    //   Product: '<S59>/Product3'

    rtb_Merge_idx_0_tmp = rtb_XAxis * rtb_XAxis;

    // Product: '<S51>/Product2' incorporates:
    //   Product: '<S59>/Product2'

    rtb_Merge_idx_0_tmp_0 = rtb_XAxis1 * rtb_XAxis1;

    // Product: '<S51>/Product1' incorporates:
    //   Product: '<S59>/Product1'

    rtb_Merge_idx_0_tmp_1 = rtb_Product2_c * rtb_Product2_c;

    // Product: '<S51>/Product' incorporates:
    //   Product: '<S59>/Product'

    rtb_Merge_idx_0_tmp_2 = rtb_Product3_l * rtb_Product3_l;

    // Outputs for IfAction SubSystem: '<S22>/AxisRotDefault' incorporates:
    //   ActionPort: '<S24>/Action Port'

    // Fcn: '<S24>/Fcn1' incorporates:
    //   Gain: '<S52>/Gain'
    //   Product: '<S51>/Product'
    //   Product: '<S51>/Product1'
    //   Product: '<S51>/Product2'
    //   Product: '<S51>/Product3'
    //   Product: '<S52>/Product2'
    //   Product: '<S52>/Product3'
    //   Sum: '<S51>/Sum'
    //   Sum: '<S52>/Sum'

    rtb_Merge_idx_0 = std::atan2((rtb_Product3_l * rtb_XAxis + rtb_XAxis1 *
      rtb_Product2_c) * 2.0F, ((rtb_Merge_idx_0_tmp + rtb_Merge_idx_0_tmp_0) -
      rtb_Merge_idx_0_tmp_1) - rtb_Merge_idx_0_tmp_2);

    // Fcn: '<S24>/Fcn2'
    rtb_Merge_idx_1 = std::asin(-rtb_VectorConcatenate_p[6]);

    // Fcn: '<S24>/Fcn3' incorporates:
    //   Gain: '<S56>/Gain'
    //   Product: '<S56>/Product1'
    //   Product: '<S56>/Product2'
    //   Sum: '<S56>/Sum'
    //   Sum: '<S59>/Sum'

    rtb_Product2_c = std::atan2((rtb_XAxis * rtb_XAxis1 + rtb_Product2_c *
      rtb_Product3_l) * 2.0F, ((rtb_Merge_idx_0_tmp - rtb_Merge_idx_0_tmp_0) -
      rtb_Merge_idx_0_tmp_1) + rtb_Merge_idx_0_tmp_2);

    // End of Outputs for SubSystem: '<S22>/AxisRotDefault'
  }

  // End of If: '<S22>/If'

  // SignalConversion generated from: '<Root>/eulAng_rad'
  rty_eulAng_rad[0] = rtb_Product2_c;
  rty_eulAng_rad[1] = rtb_Merge_idx_1;
  rty_eulAng_rad[2] = rtb_Merge_idx_0;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S6>/Product'

  rty_bodyAccels_mps2[0] = Product[0] - rty_states[14];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[0] = rtb_UnitDelay_g;
  rty_dcmNedToBody[1] = rtb_XAxis2;
  rty_dcmNedToBody[2] = rtb_Sum_m1_idx_1;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S6>/Product'

  rty_bodyAccels_mps2[1] = Product[1] - rty_states[15];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[3] = rtb_Sum_m1_idx_2;
  rty_dcmNedToBody[4] = q2q3;
  rty_dcmNedToBody[5] = rtb_dcmBodyToNed_idx_7;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S6>/Product'

  rty_bodyAccels_mps2[2] = Product[2] - rty_states[16];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[6] = rtb_dcmBodyToNed_idx_2;
  rty_dcmNedToBody[7] = rtb_dcmBodyToNed_idx_5;
  rty_dcmNedToBody[8] = rtb_dcmBodyToNed_idx_8;

  // MATLAB Function: '<Root>/eulToDcm' incorporates:
  //   Gain: '<Root>/ZeroOutRollAndPitch'

  //  precalculate trignometric values
  // MATLAB Function 'eulToDcm': '<S8>:1'
  // '<S8>:1:4' s_phi = sin(eul_rad(1));
  // '<S8>:1:5' c_phi = cos(eul_rad(1));
  // '<S8>:1:7' s_theta = sin(eul_rad(2));
  // '<S8>:1:8' c_theta = cos(eul_rad(2));
  // '<S8>:1:10' s_psi = sin(eul_rad(3));
  rtb_Product3_l = std::sin(rtb_Merge_idx_0);

  // '<S8>:1:11' c_psi = cos(eul_rad(3));
  rtb_XAxis = std::cos(rtb_Merge_idx_0);

  // '<S8>:1:13' dcmFromNed = [c_psi*c_theta, c_theta*s_psi, -s_theta;
  // '<S8>:1:14'     c_psi*s_phi*s_theta - c_phi*s_psi, c_phi*c_psi + s_phi*s_psi*s_theta, c_theta*s_phi; 
  // '<S8>:1:15'     s_phi*s_psi + c_phi*c_psi*s_theta, c_phi*s_psi*s_theta - c_psi*s_phi, c_phi*c_theta]; 
  rty_dcmNedToFep[0] = rtb_XAxis;
  rty_dcmNedToFep[3] = rtb_Product3_l;
  rty_dcmNedToFep[6] = -0.0F;
  rty_dcmNedToFep[1] = 0.0F - rtb_Product3_l;
  rty_dcmNedToFep[4] = rtb_XAxis;
  rty_dcmNedToFep[7] = 0.0F;
  rty_dcmNedToFep[2] = 0.0F;
  rty_dcmNedToFep[5] = 0.0F;
  rty_dcmNedToFep[8] = 1.0F;

  // Update for DiscreteTransferFcn: '<S65>/X Axis'
  // MATLAB Function 'lidarRangeToAgl/MATLAB Function': '<S71>:1'
  // '<S71>:1:2' agl_m = lidarRangeToAgl_function( ...
  // '<S71>:1:3'     lidarRange_m, roll_rad, pitch_rad, xL_m, yL_m, zL_m);
  // CORRECTLIDARRANGETOBODYAGLFAST_FUNCTION
  //  Fast scalar version for fixed body-frame lidar lever arm.
  //
  //  Body frame:
  //    x forward
  //    y right
  //    z down
  //
  //  Lidar:
  //    beam aligned with body +Z/down
  // 'lidarRangeToAgl_function:14' sinRoll_rad = sin(roll_rad);
  // 'lidarRangeToAgl_function:15' cosRoll_rad = cos(roll_rad);
  // 'lidarRangeToAgl_function:16' sinPitch_rad = sin(pitch_rad);
  // 'lidarRangeToAgl_function:17' cosPitch_rad = cos(pitch_rad);
  // 'lidarRangeToAgl_function:19' agl_m = ...
  // 'lidarRangeToAgl_function:20'     -sinPitch_rad*xL_m + ...
  // 'lidarRangeToAgl_function:21'      sinRoll_rad*cosPitch_rad*yL_m + ...
  // 'lidarRangeToAgl_function:22'      cosRoll_rad*cosPitch_rad*(zL_m + lidarRange_m); 
  stateEstimatorEskf_DW.XAxis_states[1] = stateEstimatorEskf_DW.XAxis_states[0];
  stateEstimatorEskf_DW.XAxis_states[0] = stateEstimatorEskf_DW.XAxis_tmp;

  // Update for DiscreteTransferFcn: '<S65>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states[1] = stateEstimatorEskf_DW.XAxis1_states[0];
  stateEstimatorEskf_DW.XAxis1_states[0] = stateEstimatorEskf_DW.XAxis1_tmp;

  // Update for DiscreteTransferFcn: '<S65>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states[1] = stateEstimatorEskf_DW.XAxis2_states[0];
  stateEstimatorEskf_DW.XAxis2_states[0] = stateEstimatorEskf_DW.XAxis2_tmp;

  // Update for DiscreteTransferFcn: '<S66>/X Axis'
  stateEstimatorEskf_DW.XAxis_states_e[1] =
    stateEstimatorEskf_DW.XAxis_states_e[0];
  stateEstimatorEskf_DW.XAxis_states_e[0] = stateEstimatorEskf_DW.XAxis_tmp_o;

  // Update for DiscreteTransferFcn: '<S66>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states_a[1] =
    stateEstimatorEskf_DW.XAxis1_states_a[0];
  stateEstimatorEskf_DW.XAxis1_states_a[0] = stateEstimatorEskf_DW.XAxis1_tmp_l;

  // Update for DiscreteTransferFcn: '<S66>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states_j[1] =
    stateEstimatorEskf_DW.XAxis2_states_j[0];
  stateEstimatorEskf_DW.XAxis2_states_j[0] = stateEstimatorEskf_DW.XAxis2_tmp_o;

  // Update for UnitDelay: '<Root>/Unit Delay1'
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[0] = rtb_Product2_c;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[1] = rtb_Merge_idx_1;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[2] = rtb_Merge_idx_0;

  // Switch: '<S9>/Switch' incorporates:
  //   RelationalOperator: '<S68>/FixPt Relational Operator'
  //   UnitDelay: '<S68>/Delay Input1'
  //
  //  Block description for '<S68>/Delay Input1':
  //
  //   Store in Global RAM

  if (static_cast<int32_T>(rtb_Compare) > static_cast<int32_T>
      (stateEstimatorEskf_DW.DelayInput1_DSTATE)) {
    // Update for UnitDelay: '<S9>/Unit Delay' incorporates:
    //   UnitDelay: '<Root>/Unit Delay'

    stateEstimatorEskf_DW.UnitDelay_DSTATE =
      stateEstimatorEskf_DW.UnitDelay_DSTATE_e[6];
  }

  // End of Switch: '<S9>/Switch'

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] = rtb_UnitDelay_g;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] = rtb_Sum_m1_idx_2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] = rtb_XAxis2;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] = q2q3;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[6] = rtb_Sum_m1_idx_1;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for DiscreteTransferFcn: '<S77>/Discrete Transfer Fcn'
  stateEstimatorEskf_DW.DiscreteTransferFcn_states =
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp;

  // Update for DiscreteTransferFcn: '<S76>/Discrete Transfer Fcn'
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
  stateEstimatorEskf_DW.Delay2_DSTATE[0] = rtb_UnitDelay_g;
  stateEstimatorEskf_DW.Delay2_DSTATE[1] = rtb_Sum_m1_idx_2;
  stateEstimatorEskf_DW.Delay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;
  stateEstimatorEskf_DW.Delay2_DSTATE[3] = rtb_XAxis2;
  stateEstimatorEskf_DW.Delay2_DSTATE[4] = q2q3;
  stateEstimatorEskf_DW.Delay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;
  stateEstimatorEskf_DW.Delay2_DSTATE[6] = rtb_Sum_m1_idx_1;
  stateEstimatorEskf_DW.Delay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;
  stateEstimatorEskf_DW.Delay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for UnitDelay: '<S68>/Delay Input1'
  //
  //  Block description for '<S68>/Delay Input1':
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
