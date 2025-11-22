//
// File: stateEstimatorEskf.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
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
#include "rtwtypes.h"
#include "stateEstimatorEskf_types.h"
#include <cstring>
#include <cmath>
#include "applyGpsPosAndVelCorr_kaQP2N0T.h"
#include "computeEskfAccelMeasJa_g9O3pYCr.h"
#include "norm_yrNKZSBO.h"
#include "computEskfMagMeasJac_U07joj0p.h"
#include "computeEskfStateJac_4pNsZTDW.h"
#include "updateEskfCovP_yMTfUr7W.h"
#include "quatMultiply_AJk9aby2.h"
#include "updateQuatAndResetCovP_KAnSUXrZ.h"
#include "mrdiv_s6DFIKmD.h"
#include "quatToDcm_4oGXZFqp.h"
#include "norm_7MzYkgry.h"
#include "mrdiv_yiWolFAP.h"
#include "stateEstimatorEskf_private.h"

// Named constants for Chart: '<Root>/estimatorStateMachine'
const uint8_T stateEstima_IN_RUN_GPS_NOT_INIT{ 4U };

const uint8_T stateEstimatorE_IN_RUN_GPS_LOST{ 3U };

const uint8_T stateEstimatorE_IN_RUN_INIT_GPS{ 5U };

const uint8_T stateEstimatorEsk_IN_INITIALIZE{ 1U };

const uint8_T stateEstimatorEskf_IN_RUN{ 2U };

// Named constants for Chart: '<S10>/Chart'
const uint8_T stateEstimatorEsk_IN_OF_INVALID{ 1U };

const uint8_T stateEstimatorEskf_IN_OF_VALID{ 2U };

//
// Output and update for atomic system:
//    '<S31>/Compute Filter Numerator And Denominator'
//    '<S32>/Compute Filter Numerator And Denominator'
//
void stateEstimatorEskf::ComputeFilterNumeratorAndDenomi(const real32_T
  *rtu_filterBandwidth_radps, real32_T rty_num[2], real32_T rty_den[2], real_T
  rtp_sampleTime_s)
{
  real_T K;
  real32_T normalizer;
  real32_T tmp;

  //  Call the main function
  // MATLAB Function 'Discrete First Order Filter/Compute Filter Numerator And Denominator': '<S33>:1' 
  // '<S33>:1:4' [num, den] = computeFirstOrderFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
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
  K = 2.0 / rtp_sampleTime_s;

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
  normalizer = *rtu_filterBandwidth_radps + static_cast<real32_T>(K);

  // 'computeDiscreteTFNumAndDen_function:24' b0 = (B(1) + B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:25' b1 = (B(1) - B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:27' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:28' a1 = (A(1) - A(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:29' num = [b0, b1];
  tmp = *rtu_filterBandwidth_radps / normalizer;
  rty_num[0] = tmp;
  rty_num[1] = tmp;

  // 'computeDiscreteTFNumAndDen_function:30' den = [a0, a1];
  rty_den[0] = 1.0F;
  rty_den[1] = (*rtu_filterBandwidth_radps - static_cast<real32_T>(K)) /
    normalizer;
}

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
  // '<S5>:44:1' sf_internal_predicateOutput = isAttInitialized && isPosInitialized ... 
  // '<S5>:44:2'  && isBaroInitialized;
  if (static_cast<boolean_T>(static_cast<boolean_T>
       (stateEstimatorEskf_DW.isAttInitialized &
        stateEstimatorEskf_DW.isPosInitialized) &
       stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S5>:44'
    stateEstimatorEskf_DW.durationCounter_1_j = 0;
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
    // '<S5>:43:9' sensorDataOut.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isMagValid = rtu_magData->isMagDataValid;

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

    // '<S5>:43:11' sensorDataOut.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isGpsValid = rtu_gpsData->isGpsDataValid;

    // '<S5>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // Chart: '<Root>/estimatorStateMachine'
    // '<S5>:43:13' sensorDataOut.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
      rtu_baroData->isBaroDataValid;

    //

    // Chart: '<Root>/estimatorStateMachine' incorporates:
    //   Product: '<S4>/Product'
    //   Product: '<S9>/Divide'
    //   SignalConversion generated from: '<S5>/ SFunction '

    // '<S5>:63:1' sf_internal_predicateOutput = isAttInitialized && ~isGpsInitialized &&  isBaroInitialized; 
  } else if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<boolean_T>
               (rtu_gpsData->isGpsInitialized ^ 1) &
               stateEstimatorEskf_DW.isAttInitialized) &
              stateEstimatorEskf_DW.isBaroInitialized)) {
    // Transition: '<S5>:63'
    stateEstimatorEskf_DW.durationCounter_1_f = 0;
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
    // '<S5>:62:11' sensorDataOut.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isMagValid = rtu_magData->isMagDataValid;

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

    // '<S5>:62:13' sensorDataOut.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isGpsValid = rtu_gpsData->isGpsDataValid;

    // '<S5>:62:14' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
      stateEstimatorEskf_DW.baroInitAltMean;

    // '<S5>:62:15' sensorDataOut.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
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
        stateEstimatorEskf_DW.magMean[0] += imuDelta_idx_0 /
          stateEstimatorEskf_DW.magIdx;
        stateEstimatorEskf_DW.magM2[0] += (Divide[0] -
          stateEstimatorEskf_DW.magMean[0]) * imuDelta_idx_0;
        imuDelta_idx_0 = Divide[1] - stateEstimatorEskf_DW.magMean[1];
        stateEstimatorEskf_DW.magMean[1] += imuDelta_idx_0 /
          stateEstimatorEskf_DW.magIdx;
        stateEstimatorEskf_DW.magM2[1] += (Divide[1] -
          stateEstimatorEskf_DW.magMean[1]) * imuDelta_idx_0;
        imuDelta_idx_0 = Divide[2] - stateEstimatorEskf_DW.magMean[2];
        stateEstimatorEskf_DW.magMean[2] += imuDelta_idx_0 /
          stateEstimatorEskf_DW.magIdx;
        stateEstimatorEskf_DW.magM2[2] += (Divide[2] -
          stateEstimatorEskf_DW.magMean[2]) * imuDelta_idx_0;
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
        imuDelta_idx_0 = std::atan2(-stateEstimatorEskf_DW.imuMean[1],
          -stateEstimatorEskf_DW.imuMean[2]);

        // 'computeInitialAttitude:15' pitch_rad = atan2( -bodyAccels_mps2(1), norm(bodyAccels_mps2(2:3)) ); 
        imuDelta_idx_3 = 1.29246971E-26F;
        imuDelta_idx_1 = std::abs(stateEstimatorEskf_DW.imuMean[1]);
        if (imuDelta_idx_1 > 1.29246971E-26F) {
          imuDelta_idx_2 = 1.0F;
          imuDelta_idx_3 = imuDelta_idx_1;
        } else {
          imuDelta_idx_4 = imuDelta_idx_1 / 1.29246971E-26F;
          imuDelta_idx_2 = imuDelta_idx_4 * imuDelta_idx_4;
        }

        imuDelta_idx_1 = std::abs(stateEstimatorEskf_DW.imuMean[2]);
        if (imuDelta_idx_1 > imuDelta_idx_3) {
          imuDelta_idx_4 = imuDelta_idx_3 / imuDelta_idx_1;
          imuDelta_idx_2 = imuDelta_idx_2 * imuDelta_idx_4 * imuDelta_idx_4 +
            1.0F;
          imuDelta_idx_3 = imuDelta_idx_1;
        } else {
          imuDelta_idx_4 = imuDelta_idx_1 / imuDelta_idx_3;
          imuDelta_idx_2 += imuDelta_idx_4 * imuDelta_idx_4;
        }

        imuDelta_idx_2 = std::atan2(-stateEstimatorEskf_DW.imuMean[0],
          imuDelta_idx_3 * std::sqrt(imuDelta_idx_2));

        //  Compute corrected magnetometer readings
        // 'computeInitialAttitude:18' cPhi = cos(roll_rad);
        imuDelta_idx_3 = std::cos(imuDelta_idx_0);

        // 'computeInitialAttitude:19' sPhi = sin(roll_rad);
        imuDelta_idx_1 = std::sin(imuDelta_idx_0);

        // 'computeInitialAttitude:20' sTheta = sin(pitch_rad);
        imuDelta_idx_4 = std::sin(imuDelta_idx_2);

        // 'computeInitialAttitude:21' cTheta = cos(pitch_rad);
        // 'computeInitialAttitude:22' yaw_rad = atan2(-magVecNorm_nd(2) * cPhi + magVecNorm_nd(3) * sPhi, ... 
        // 'computeInitialAttitude:23'                     magVecNorm_nd(1) * cTheta + magVecNorm_nd(2) * sPhi * sTheta + ... 
        // 'computeInitialAttitude:24'                     magVecNorm_nd(3) * cPhi * sTheta) + magDec_rad; 
        imuDelta_idx_1 = std::atan2(-stateEstimatorEskf_DW.magMean[1] *
          imuDelta_idx_3 + stateEstimatorEskf_DW.magMean[2] * imuDelta_idx_1,
          (stateEstimatorEskf_DW.magMean[1] * imuDelta_idx_1 * imuDelta_idx_4 +
           stateEstimatorEskf_DW.magMean[0] * std::cos(imuDelta_idx_2)) +
          stateEstimatorEskf_DW.magMean[2] * imuDelta_idx_3 * imuDelta_idx_4) +
          rtu_stateEstSmParams->initMagDec_rad;

        //  Compute half-angles
        // 'computeInitialAttitude:27' cHalfPhi= cos(roll_rad/2);
        imuDelta_idx_3 = std::cos(imuDelta_idx_0 / 2.0F);

        // 'computeInitialAttitude:28' sHalfPhi = sin(roll_rad/2);
        imuDelta_idx_0 = std::sin(imuDelta_idx_0 / 2.0F);

        // 'computeInitialAttitude:32' cHalfTheta = cos(pitch_rad/2);
        imuDelta_idx_4 = std::cos(imuDelta_idx_2 / 2.0F);

        // 'computeInitialAttitude:33' sHalfTheta = sin(pitch_rad/2);
        imuDelta_idx_2 = std::sin(imuDelta_idx_2 / 2.0F);

        // 'computeInitialAttitude:35' cHalfPsi = cos(yaw_rad/2);
        imuDelta_idx_5 = std::cos(imuDelta_idx_1 / 2.0F);

        // 'computeInitialAttitude:36' sHalfPsi = sin(yaw_rad/2);
        imuDelta_idx_1 = std::sin(imuDelta_idx_1 / 2.0F);

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
        stateEstimatorEskf_DW.initialQuat[0] = initialQuat_tmp * imuDelta_idx_3
          + initialQuat_tmp_0 * imuDelta_idx_0;
        stateEstimatorEskf_DW.initialQuat[1] = initialQuat_tmp * imuDelta_idx_0
          - initialQuat_tmp_0 * imuDelta_idx_3;
        initialQuat_tmp = imuDelta_idx_1 * imuDelta_idx_4;
        initialQuat_tmp_0 = imuDelta_idx_5 * imuDelta_idx_2;
        stateEstimatorEskf_DW.initialQuat[2] = initialQuat_tmp_0 *
          imuDelta_idx_3 + initialQuat_tmp * imuDelta_idx_0;
        stateEstimatorEskf_DW.initialQuat[3] = initialQuat_tmp * imuDelta_idx_3
          - initialQuat_tmp_0 * imuDelta_idx_0;

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
        imuDelta_idx_0 = *Divide1 - stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:1:107' baroInitAltMean = baroInitAltMean + baroInitAltDelta / baroIdx; 
        stateEstimatorEskf_DW.baroInitAltMean += imuDelta_idx_0 /
          stateEstimatorEskf_DW.baroIdx;

        // '<S5>:1:108' baroInitAltM2 = baroInitAltM2 + baroInitAltDelta .* (baroPressAlt_m - baroInitAltMean); 
        stateEstimatorEskf_DW.baroInitAltM2 += (*Divide1 -
          stateEstimatorEskf_DW.baroInitAltMean) * imuDelta_idx_0;
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
    // '<S5>:1:136' sensorDataOut.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isMagValid = rtu_magData->isMagDataValid;

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

    // '<S5>:1:138' sensorDataOut.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isGpsValid = rtu_gpsData->isGpsDataValid;

    // '<S5>:1:139' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S5>:1:140' sensorDataOut.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
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
  // '<S5>:64:15' sensorDataOut.isMagValid = isMagDataValid;
  stateEstimatorEskf_DW.sensorDataOut.isMagValid = rtu_magData->isMagDataValid;

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

  // '<S5>:64:17' sensorDataOut.isGpsValid = isGpsDataValid;
  stateEstimatorEskf_DW.sensorDataOut.isGpsValid = rtu_gpsData->isGpsDataValid;

  // '<S5>:64:18' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean;
  stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = *Divide1 -
    stateEstimatorEskf_DW.baroInitAltMean;

  // Chart: '<Root>/estimatorStateMachine'
  // '<S5>:64:19' sensorDataOut.isBaroValid = isBaroDataValid;
  stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
    rtu_baroData->isBaroDataValid;

  //
}

// System initialize for referenced model: 'stateEstimatorEskf'
void stateEstimatorEskf::init(busStateEstimatorDebug *rty_stateEstimatorDebug)
{
  static const int8_T tmp_0[20]{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0 };

  int32_T i;
  int32_T k;
  int32_T tmp;
  int8_T b_I[256];

  // InitializeConditions for Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = true;

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

  // InitializeConditions for Delay: '<S1>/Delay2'
  stateEstimatorEskf_DW.icLoad_g = true;
  for (i = 0; i < 20; i++) {
    // InitializeConditions for UnitDelay: '<Root>/Unit Delay'
    stateEstimatorEskf_DW.UnitDelay_DSTATE[i] =
      rtCP_UnitDelay_InitialCondition[i];

    // SystemInitialize for Chart: '<Root>/estimatorStateMachine' incorporates:
    //   UnitDelay: '<Root>/Unit Delay'

    stateEstimatorEskf_DW.initialStates[i] = tmp_0[i];
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
  stateEstimatorEskf_DW.sensorDataOut.isImuDataValid = false;
  stateEstimatorEskf_DW.sensorDataOut.isMagValid = false;
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
  stateEstimatorEskf_DW.sensorDataOut.isGpsValid = false;
  stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.isBaroValid = false;
  stateEstimatorEskf_DW.sensorDataOut.lidarAgl_m = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.isLidarValid = false;
  stateEstimatorEskf_DW.sensorDataOut.ofNeVel_mps[0] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.ofNeVel_mps[1] = 0.0F;
  stateEstimatorEskf_DW.sensorDataOut.isOfValid = false;

  // SystemInitialize for MATLAB Function: '<S1>/EKF'
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

  // SystemInitialize for BusCreator: '<Root>/Bus Creator' incorporates:
  //   Chart: '<Root>/estimatorStateMachine'

  // Error state jacobian
  //      errorStateJac = eye(19, 'single');
  //      errorStateJac(1:3, 10:12) = -I3*sampleTime_s;
  rty_stateEstimatorDebug->stateEstInitPct = 0.0F;
  rty_stateEstimatorDebug->smMode = enumStateEstimateMode::NONE;
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
  static const int8_T c_0[15]{ 0, 1, 2, 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18 };

  static const int8_T e[12]{ 6, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };

  static const int8_T f[12]{ 5, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 };

  real_T tmp_0[19];
  real_T tmp_1[16];
  real_T tmp_2[16];
  real_T tmp_3[9];
  real_T latLonAltOut[3];
  real_T rtb_nedPos_m_idx_0;
  real_T rtb_nedPos_m_idx_1;
  real_T rtb_nedPos_m_idx_2;
  int32_T i;
  real32_T b_covP[361];
  real32_T covP[361];
  real32_T tmp_4[225];
  real32_T tmp[60];
  real32_T H[57];
  real32_T b_K[57];
  real32_T tmp1[57];
  real32_T tmp_8[46];
  real32_T K[45];
  real32_T tmp1_0[45];
  real32_T rtb_UnitDelay[20];
  real32_T rtb_prevStates[20];
  real32_T b_errorStateHat[19];
  real32_T d_K[19];
  real32_T errorStateHat[19];
  real32_T c_K[15];
  real32_T tmp_5[15];
  real32_T rty_states_0[12];
  real32_T rtb_Delay2[9];
  real32_T nedPosAndVel[6];
  real32_T nQuat_tmp[4];
  real32_T tmp_6[4];
  real32_T Divide[3];
  real32_T Product[3];
  real32_T dTheta[3];
  real32_T localNedUnitMag_nd[3];
  real32_T den[2];
  real32_T rtb_VectorConcatenate_k2[2];
  real32_T rtb_Product2;
  real32_T rtb_Product3;
  real32_T rtb_UnitDelay1_idx_0;
  real32_T rtb_UnitDelay1_idx_1;
  real32_T rtb_UnitDelay1_idx_2;
  real32_T rtb_UnitDelay_d;
  real32_T rtb_UnitDelay_f;
  real32_T rtb_UnitDelay_g;
  real32_T rtb_XAxis;
  real32_T rtb_XAxis1;
  real32_T rtb_dcmBodyToNed_idx_0;
  real32_T rtb_dcmBodyToNed_idx_1;
  real32_T rtb_dcmBodyToNed_idx_2;
  real32_T rtb_dcmBodyToNed_idx_3;
  real32_T rtb_dcmBodyToNed_idx_4;
  real32_T rtb_dcmBodyToNed_idx_5;
  real32_T rtb_dcmBodyToNed_idx_6;
  real32_T rtb_dcmBodyToNed_idx_7;
  real32_T rtb_dcmBodyToNed_idx_8;
  boolean_T rtb_AND;
  boolean_T rtb_Compare;
  boolean_T rtb_isOfDataValidOut;
  boolean_T rtb_sensorIn_isBaroValid;
  boolean_T rtb_sensorIn_isGpsValid;
  enumStateEstimateMode mode;

  // UnitDelay: '<Root>/Unit Delay'
  std::memcpy(&rtb_UnitDelay[0], &stateEstimatorEskf_DW.UnitDelay_DSTATE[0], 20U
              * sizeof(real32_T));

  // DiscreteTransferFcn: '<S22>/X Axis'
  stateEstimatorEskf_DW.XAxis_tmp = (rtu_imuData->bodyAccels_mps2[0] -
    stateEstimatorEskf_DW.XAxis_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[1]) -
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xDen[2];
  rtb_Product2 = (rtu_imuNotchFiltParams->accelNtchFilt.xNum[0] *
                  stateEstimatorEskf_DW.XAxis_tmp +
                  stateEstimatorEskf_DW.XAxis_states[0] *
                  rtu_imuNotchFiltParams->accelNtchFilt.xNum[1]) +
    stateEstimatorEskf_DW.XAxis_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.xNum[2];

  // DiscreteTransferFcn: '<S22>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_tmp = (rtu_imuData->bodyAccels_mps2[1] -
    stateEstimatorEskf_DW.XAxis1_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[1]) -
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yDen[2];
  rtb_UnitDelay_g = (rtu_imuNotchFiltParams->accelNtchFilt.yNum[0] *
                     stateEstimatorEskf_DW.XAxis1_tmp +
                     stateEstimatorEskf_DW.XAxis1_states[0] *
                     rtu_imuNotchFiltParams->accelNtchFilt.yNum[1]) +
    stateEstimatorEskf_DW.XAxis1_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.yNum[2];

  // DiscreteTransferFcn: '<S22>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_tmp = (rtu_imuData->bodyAccels_mps2[2] -
    stateEstimatorEskf_DW.XAxis2_states[0] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[1]) -
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zDen[2];
  rtb_Product3 = (rtu_imuNotchFiltParams->accelNtchFilt.zNum[0] *
                  stateEstimatorEskf_DW.XAxis2_tmp +
                  stateEstimatorEskf_DW.XAxis2_states[0] *
                  rtu_imuNotchFiltParams->accelNtchFilt.zNum[1]) +
    stateEstimatorEskf_DW.XAxis2_states[1] *
    rtu_imuNotchFiltParams->accelNtchFilt.zNum[2];

  // Product: '<S4>/Product' incorporates:
  //   Product: '<S4>/Matrix Multiply'
  //   SignalConversion generated from: '<S4>/Matrix Multiply'
  //   Sum: '<S4>/Sum'

  Product[0] = (((rtu_accelParams->scaleAlignMat_nd[0] * rtb_Product2 +
                  rtu_accelParams->scaleAlignMat_nd[3] * rtb_UnitDelay_g) +
                 rtu_accelParams->scaleAlignMat_nd[6] * rtb_Product3) +
                rtu_accelParams->offset_nd[0]) * *rtu_gEarth_mps2;
  Product[1] = (((rtu_accelParams->scaleAlignMat_nd[1] * rtb_Product2 +
                  rtu_accelParams->scaleAlignMat_nd[4] * rtb_UnitDelay_g) +
                 rtu_accelParams->scaleAlignMat_nd[7] * rtb_Product3) +
                rtu_accelParams->offset_nd[1]) * *rtu_gEarth_mps2;
  Product[2] = (((rtu_accelParams->scaleAlignMat_nd[2] * rtb_Product2 +
                  rtu_accelParams->scaleAlignMat_nd[5] * rtb_UnitDelay_g) +
                 rtu_accelParams->scaleAlignMat_nd[8] * rtb_Product3) +
                rtu_accelParams->offset_nd[2]) * *rtu_gEarth_mps2;

  // DiscreteTransferFcn: '<S23>/X Axis'
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

  // DiscreteTransferFcn: '<S23>/X Axis1'
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

  // DiscreteTransferFcn: '<S23>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_tmp_o = (rtu_imuData->bodyRates_radps[2] -
    stateEstimatorEskf_DW.XAxis2_states_j[0] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zDen[1]) -
    stateEstimatorEskf_DW.XAxis2_states_j[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zDen[2];

  // Sum: '<S9>/Sum'
  rtb_UnitDelay_g = rtu_magData->bodyMagVector_uT[0] - rtu_magParams->offset_uT
    [0];
  rtb_dcmBodyToNed_idx_0 = rtu_magData->bodyMagVector_uT[1] -
    rtu_magParams->offset_uT[1];
  rtb_dcmBodyToNed_idx_3 = rtu_magData->bodyMagVector_uT[2] -
    rtu_magParams->offset_uT[2];

  // Product: '<S9>/Matrix Multiply'
  rtb_Product3 = (rtu_magParams->scaleAlignMat_nd[0] * rtb_UnitDelay_g +
                  rtu_magParams->scaleAlignMat_nd[3] * rtb_dcmBodyToNed_idx_0) +
    rtu_magParams->scaleAlignMat_nd[6] * rtb_dcmBodyToNed_idx_3;
  rtb_Product2 = (rtu_magParams->scaleAlignMat_nd[1] * rtb_UnitDelay_g +
                  rtu_magParams->scaleAlignMat_nd[4] * rtb_dcmBodyToNed_idx_0) +
    rtu_magParams->scaleAlignMat_nd[7] * rtb_dcmBodyToNed_idx_3;
  rtb_dcmBodyToNed_idx_0 = (rtu_magParams->scaleAlignMat_nd[2] * rtb_UnitDelay_g
    + rtu_magParams->scaleAlignMat_nd[5] * rtb_dcmBodyToNed_idx_0) +
    rtu_magParams->scaleAlignMat_nd[8] * rtb_dcmBodyToNed_idx_3;

  // MinMax: '<S9>/Max' incorporates:
  //   Constant: '<S9>/Constant2'
  //   Product: '<S9>/Matrix Multiply1'
  //   Sqrt: '<S9>/Sqrt'

  rtb_UnitDelay_g = std::fmax(std::sqrt((rtb_Product3 * rtb_Product3 +
    rtb_Product2 * rtb_Product2) + rtb_dcmBodyToNed_idx_0 *
    rtb_dcmBodyToNed_idx_0), 1.0E-7F);

  // Product: '<S9>/Divide'
  Divide[0] = rtb_Product3 / rtb_UnitDelay_g;
  Divide[1] = rtb_Product2 / rtb_UnitDelay_g;
  Divide[2] = rtb_dcmBodyToNed_idx_0 / rtb_UnitDelay_g;

  // Product: '<S11>/Divide1' incorporates:
  //   Constant: '<S11>/Constant'
  //   Constant: '<S11>/Constant2'
  //   Constant: '<S11>/Constant3'
  //   Math: '<S11>/Power'
  //   Product: '<S11>/Divide'
  //   Sum: '<S11>/Sum'

  rtb_UnitDelay_g = (1.0F - std::pow(rtu_baroData->pressure_pa / 101325.0F,
    0.190294951F)) * 44330.0F;

  // SignalConversion generated from: '<S5>/ SFunction ' incorporates:
  //   Chart: '<Root>/estimatorStateMachine'
  //   DiscreteTransferFcn: '<S23>/X Axis2'

  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[0] = rtb_XAxis;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[1] = rtb_XAxis1;
  stateEstimatorEskf_DW.TmpSignalConversionAtSFunctionI[2] =
    (rtu_imuNotchFiltParams->gyroNtchFilt.zNum[0] *
     stateEstimatorEskf_DW.XAxis2_tmp_o + stateEstimatorEskf_DW.XAxis2_states_j
     [0] * rtu_imuNotchFiltParams->gyroNtchFilt.zNum[1]) +
    stateEstimatorEskf_DW.XAxis2_states_j[1] *
    rtu_imuNotchFiltParams->gyroNtchFilt.zNum[2];

  // Chart: '<Root>/estimatorStateMachine' incorporates:
  //   Product: '<S4>/Product'
  //   Product: '<S9>/Divide'
  //   SignalConversion generated from: '<S5>/ SFunction '

  // Gateway: estimatorStateMachine
  // During: estimatorStateMachine
  if (stateEstimatorEskf_DW.is_active_c3_stateEstimatorEskf == 0U) {
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
    // '<S5>:1:8' sensorDataOut.isMagValid = isMagDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isMagValid = rtu_magData->isMagDataValid;

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

    // '<S5>:1:10' sensorDataOut.isGpsValid = isGpsDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isGpsValid = rtu_gpsData->isGpsDataValid;

    // '<S5>:1:11' sensorDataOut.baroAlt_m = 0;
    stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = 0.0F;

    // '<S5>:1:12' sensorDataOut.isBaroValid = isBaroDataValid;
    stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
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
      stateEstimatorEskf_INITIALIZE(&mode, latLonAltOut, &rtb_UnitDelay_g,
        Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData,
        rtu_stateEstSmParams);
      break;

     case stateEstimatorEskf_IN_RUN:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN;

      // During 'RUN': '<S5>:43'
      // '<S5>:68:1' sf_internal_predicateOutput = duration(~isGpsDataValid) >=  ... 
      // '<S5>:68:2' stateEstSmParams.gpsLossCheckDuration_s;
      if (rtu_gpsData->isGpsDataValid) {
        stateEstimatorEskf_DW.durationCounter_1_j = 0;
      }

      if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_j) >=
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
        // '<S5>:67:9' sensorDataOut.isMagValid = isMagDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

        // '<S5>:67:11' sensorDataOut.isGpsValid = isGpsDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:67:13' sensorDataOut.isBaroValid = isBaroDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // FULL EKF WITH GPS IS RUNNING
        // '<S5>:43:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:43:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:43:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:43:19' sensorDataOut.isMagValid = isMagDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

        // '<S5>:43:21' sensorDataOut.isGpsValid = isGpsDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:43:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:43:23' sensorDataOut.isBaroValid = isBaroDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
          rtu_baroData->isBaroDataValid;
      }
      break;

     case stateEstimatorE_IN_RUN_GPS_LOST:
      mode = enumStateEstimateMode::RUN_GPS_LOST;

      // During 'RUN_GPS_LOST': '<S5>:67'
      // '<S5>:71:1' sf_internal_predicateOutput = gpsValidCount >= stateEstSmParams.desValidGpsCount; 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S5>:71'
        stateEstimatorEskf_DW.durationCounter_1_fx = 0;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_UnitDelay_g,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // GPS WAS LOST
        // '<S5>:67:16' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2;
        // '<S5>:67:17' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps;
        // '<S5>:67:18' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
        // '<S5>:67:19' sensorDataOut.isMagValid = isMagDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

        // '<S5>:67:21' sensorDataOut.isGpsValid = isGpsDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:67:22' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:67:23' sensorDataOut.isBaroValid = isBaroDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
        // '<S5>:67:25' if(isGpsDataValid)
        if (rtu_gpsData->isGpsDataValid) {
          // '<S5>:67:26' gpsValidCount = gpsValidCount + 1;
          stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
            (stateEstimatorEskf_DW.gpsValidCount + 1U);
        }
      }
      break;

     case stateEstima_IN_RUN_GPS_NOT_INIT:
      stateEstimatorEskf_DW.resetStates = false;
      mode = enumStateEstimateMode::RUN_GPS_NOT_INIT;

      // During 'RUN_GPS_NOT_INIT': '<S5>:62'
      // '<S5>:65:1' sf_internal_predicateOutput = gpsValidCount >= stateEstSmParams.desValidGpsCount; 
      if (stateEstimatorEskf_DW.gpsValidCount >=
          rtu_stateEstSmParams->desValidGpsCount) {
        // Transition: '<S5>:65'
        stateEstimatorEskf_DW.durationCounter_1_fx = 0;
        stateEstimatorEskf_DW.is_c3_stateEstimatorEskf =
          stateEstimatorE_IN_RUN_INIT_GPS;
        state_enter_atomic_RUN_INIT_GPS(&mode, latLonAltOut, &rtb_UnitDelay_g,
          Product, Divide, rtu_magData, rtu_gpsData, rtu_baroData);
      } else {
        // '<S5>:73:1' sf_internal_predicateOutput = duration(~isGpsDataValid) >=  ... 
        // '<S5>:73:2' stateEstSmParams.gpsLossCheckDuration_s;
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_f = 0;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_f) >=
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
          // '<S5>:67:9' sensorDataOut.isMagValid = isMagDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

          // '<S5>:67:11' sensorDataOut.isGpsValid = isGpsDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:67:13' sensorDataOut.isBaroValid = isBaroDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF STARTED RUNNING WITHOUT GPS
          // '<S5>:62:18' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S5>:62:19' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S5>:62:20' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:62:21' sensorDataOut.isMagValid = isMagDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

          // '<S5>:62:23' sensorDataOut.isGpsValid = isGpsDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:62:24' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:62:25' sensorDataOut.isBaroValid = isBaroDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
          // '<S5>:62:27' if(isGpsDataValid)
          if (rtu_gpsData->isGpsDataValid) {
            // '<S5>:62:28' gpsValidCount = gpsValidCount + 1;
            stateEstimatorEskf_DW.gpsValidCount = static_cast<uint16_T>
              (stateEstimatorEskf_DW.gpsValidCount + 1U);
          }
        }
      }
      break;

     default:
      mode = enumStateEstimateMode::RUN_INIT_GPS;

      // During 'RUN_INIT_GPS': '<S5>:64'
      // '<S5>:66:1' sf_internal_predicateOutput = isPosInitialized;
      if (stateEstimatorEskf_DW.isPosInitialized) {
        // Transition: '<S5>:66'
        stateEstimatorEskf_DW.durationCounter_1_j = 0;
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
        // '<S5>:43:9' sensorDataOut.isMagValid = isMagDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

        // '<S5>:43:11' sensorDataOut.isGpsValid = isGpsDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
          rtu_gpsData->isGpsDataValid;

        // '<S5>:43:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
        stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
          stateEstimatorEskf_DW.baroInitAltMean;

        // '<S5>:43:13' sensorDataOut.isBaroValid = isBaroDataValid;
        stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
          rtu_baroData->isBaroDataValid;

        //
      } else {
        // '<S5>:70:1' sf_internal_predicateOutput = duration(~isGpsDataValid) >=  ... 
        // '<S5>:70:2' stateEstSmParams.gpsLossCheckDuration_s;
        if (rtu_gpsData->isGpsDataValid) {
          stateEstimatorEskf_DW.durationCounter_1_fx = 0;
        }

        if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1_fx) >=
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
          // '<S5>:67:9' sensorDataOut.isMagValid = isMagDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

          // '<S5>:67:11' sensorDataOut.isGpsValid = isGpsDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:67:12' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:67:13' sensorDataOut.isBaroValid = isBaroDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
            rtu_baroData->isBaroDataValid;

          //
        } else {
          // EKF RUNNING WITHOUT GPS BUT WE HAVE HEALTHY GPS SIGNAL
          // START INITIALIZING GPS
          // '<S5>:64:22' sensorDataOut.bodyAccels_mps2 = filtBodyAccelsIn_mps2; 
          // '<S5>:64:23' sensorDataOut.bodyRates_radps = filtBodyRatesIn_radps; 
          // '<S5>:64:24' sensorDataOut.normMagVec_nd = normMagVecIn_nd;
          // '<S5>:64:25' sensorDataOut.isMagValid = isMagDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isMagValid =
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

          // '<S5>:64:27' sensorDataOut.isGpsValid = isGpsDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isGpsValid =
            rtu_gpsData->isGpsDataValid;

          // '<S5>:64:28' sensorDataOut.baroAlt_m = baroPressAlt_m - baroInitAltMean; 
          stateEstimatorEskf_DW.sensorDataOut.baroAlt_m = rtb_UnitDelay_g -
            stateEstimatorEskf_DW.baroInitAltMean;

          // '<S5>:64:29' sensorDataOut.isBaroValid = isBaroDataValid;
          stateEstimatorEskf_DW.sensorDataOut.isBaroValid =
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

  if (static_cast<boolean_T>(rtu_gpsData->isGpsDataValid ^ 1)) {
    stateEstimatorEskf_DW.durationCounter_1_j++;
    stateEstimatorEskf_DW.durationCounter_1_f++;
    stateEstimatorEskf_DW.durationCounter_1_fx++;
  } else {
    stateEstimatorEskf_DW.durationCounter_1_j = 0;
    stateEstimatorEskf_DW.durationCounter_1_f = 0;
    stateEstimatorEskf_DW.durationCounter_1_fx = 0;
  }

  // Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = stateEstimatorEskf_DW.resetStates |
    stateEstimatorEskf_DW.icLoad;
  for (i = 0; i < 20; i++) {
    if (stateEstimatorEskf_DW.icLoad) {
      stateEstimatorEskf_DW.Delay_DSTATE[i] =
        stateEstimatorEskf_DW.initialStates[i];
    }

    rtb_prevStates[i] = stateEstimatorEskf_DW.Delay_DSTATE[i];
  }

  // RelationalOperator: '<S24>/Compare' incorporates:
  //   Constant: '<S24>/Constant'

  rtb_Compare = (mode == enumStateEstimateMode::RUN);

  // MATLAB Function: '<S7>/convertLlhToNedPos'
  // MATLAB Function 'latLonAltToNedPos/convertLlhToNedPos': '<S26>:1'
  // '<S26>:1:3' nedPos_m = convertLlhToNedPos_function(latLonAlt, refLatLonAlt, isGpsValid); 
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
  if (static_cast<boolean_T>(rtb_Compare ^ 1)) {
    // 'convertLlhToNedPos_function:14' nedPos_m = double([0; 0; 0]);
    rtb_nedPos_m_idx_0 = 0.0;
    rtb_nedPos_m_idx_1 = 0.0;
    rtb_nedPos_m_idx_2 = 0.0;
  } else {
    real_T n;
    real_T nRef;
    real_T n_idx_0;
    real_T n_idx_0_tmp;
    real_T rtb_nedPos_m_tmp;
    real_T rtb_nedPos_m_tmp_0;

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
    nRef = std::sin(stateEstimatorEskf_DW.refLatLonAlt[0]);
    nRef = 6.378137E+6 / std::sqrt(1.0 - nRef * nRef * 0.0066943799901413165);

    //  Calculate ECEF coordinates of the reference point
    // 'convertLlhToNedPos_function:36' refX = (nRef + refHeight) * cos(refLatRad) * cos(refLonRad); 
    // 'convertLlhToNedPos_function:37' refY = (nRef + refHeight) * cos(refLatRad) * sin(refLonRad); 
    // 'convertLlhToNedPos_function:38' refZ = ((nRef * (1 - eccentricitySquared)) + refHeight) * sin(refLatRad); 
    //  Calculate the prime vertical radius of curvature at the current point
    // 'convertLlhToNedPos_function:41' n = semiMajorAxis / sqrt(1 - eccentricitySquared * sin(latRad)^2); 
    rtb_nedPos_m_idx_1 = std::sin(latLonAltOut[0]);
    n = 6.378137E+6 / std::sqrt(1.0 - rtb_nedPos_m_idx_1 * rtb_nedPos_m_idx_1 *
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
    rtb_nedPos_m_idx_2 = std::sin(stateEstimatorEskf_DW.refLatLonAlt[1]);
    rtb_nedPos_m_tmp = std::cos(stateEstimatorEskf_DW.refLatLonAlt[1]);
    rtb_nedPos_m_tmp_0 = std::cos(stateEstimatorEskf_DW.refLatLonAlt[0]);
    rtb_nedPos_m_idx_0 = std::sin(stateEstimatorEskf_DW.refLatLonAlt[0]);
    n_idx_0_tmp = (n + latLonAltOut[2]) * std::cos(latLonAltOut[0]);
    n_idx_0 = n_idx_0_tmp * std::cos(latLonAltOut[1]) - (nRef +
      stateEstimatorEskf_DW.refLatLonAlt[2]) * rtb_nedPos_m_tmp_0 *
      rtb_nedPos_m_tmp;
    n_idx_0_tmp = n_idx_0_tmp * std::sin(latLonAltOut[1]) - (nRef +
      stateEstimatorEskf_DW.refLatLonAlt[2]) * std::cos
      (stateEstimatorEskf_DW.refLatLonAlt[0]) * rtb_nedPos_m_idx_2;
    nRef = (n * 0.99330562000985867 + latLonAltOut[2]) * rtb_nedPos_m_idx_1 -
      (nRef * 0.99330562000985867 + stateEstimatorEskf_DW.refLatLonAlt[2]) *
      rtb_nedPos_m_idx_0;

    // 'convertLlhToNedPos_function:61' for idx = 1:3
    rtb_nedPos_m_idx_0 = (-rtb_nedPos_m_idx_0 * rtb_nedPos_m_tmp * n_idx_0 +
                          -std::sin(stateEstimatorEskf_DW.refLatLonAlt[0]) *
                          rtb_nedPos_m_idx_2 * n_idx_0_tmp) + rtb_nedPos_m_tmp_0
      * nRef;

    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(rtb_nedPos_m_idx_0) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      rtb_nedPos_m_idx_0 = 0.0;
    }

    rtb_nedPos_m_idx_1 = -rtb_nedPos_m_idx_2 * n_idx_0 + rtb_nedPos_m_tmp *
      n_idx_0_tmp;

    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(rtb_nedPos_m_idx_1) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      rtb_nedPos_m_idx_1 = 0.0;
    }

    rtb_nedPos_m_idx_2 = (-rtb_nedPos_m_tmp_0 * rtb_nedPos_m_tmp * n_idx_0 +
                          -std::cos(stateEstimatorEskf_DW.refLatLonAlt[0]) *
                          rtb_nedPos_m_idx_2 * n_idx_0_tmp) + -std::sin
      (stateEstimatorEskf_DW.refLatLonAlt[0]) * nRef;

    // 'convertLlhToNedPos_function:62' if( abs(nedPos_m(idx)) < 1e-7 )
    if (std::abs(rtb_nedPos_m_idx_2) < 1.0E-7) {
      // 'convertLlhToNedPos_function:63' nedPos_m(idx) = double(0);
      rtb_nedPos_m_idx_2 = 0.0;
    }
  }

  // End of MATLAB Function: '<S7>/convertLlhToNedPos'

  // SignalConversion generated from: '<S10>/Vector Concatenate'
  rtb_VectorConcatenate_k2[0] = rtu_mtf01pData->flowX_radps;

  // SignalConversion generated from: '<S10>/Vector Concatenate'
  rtb_VectorConcatenate_k2[1] = rtu_mtf01pData->flowY_radps;

  // Product: '<S8>/Product' incorporates:
  //   Trigonometry: '<S8>/Cos'
  //   Trigonometry: '<S8>/Cos2'
  //   UnitDelay: '<Root>/Unit Delay1'

  rtb_Product3 = std::cos(stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) * std::cos
    (stateEstimatorEskf_DW.UnitDelay1_DSTATE[1]);

  // Product: '<S10>/Product1'
  rtb_UnitDelay_g = rtu_mtf01pData->dist_m * rtb_Product3;

  // MATLAB Function: '<S32>/Compute Filter Numerator And Denominator'
  ComputeFilterNumeratorAndDenomi(&rtu_mtf01pParams->filterBw_radps,
    rtb_VectorConcatenate_k2, den, 0.004);

  // Product: '<S10>/Matrix Multiply1' incorporates:
  //   Product: '<S10>/Matrix Multiply'
  //   Product: '<S10>/Product'
  //   SignalConversion generated from: '<S10>/Vector Concatenate'
  //   Sum: '<S10>/Sum'
  //   Sum: '<S10>/Sum1'
  //   Sum: '<S10>/Sum2'
  //   UnitDelay: '<Root>/Unit Delay'
  //
  rtb_Product2 = ((rtu_mtf01pParams->sensorToBodyRot[0] *
                   rtu_mtf01pData->flowX_radps + rtu_mtf01pData->flowY_radps *
                   rtu_mtf01pParams->sensorToBodyRot[2]) - (rtb_XAxis1 -
    stateEstimatorEskf_DW.UnitDelay_DSTATE[11])) * rtb_UnitDelay_g;
  rtb_UnitDelay_g *= (rtu_mtf01pData->flowX_radps *
                      rtu_mtf01pParams->sensorToBodyRot[1] +
                      rtu_mtf01pData->flowY_radps *
                      rtu_mtf01pParams->sensorToBodyRot[3]) + (rtb_XAxis -
    stateEstimatorEskf_DW.UnitDelay_DSTATE[10]);

  // DiscreteTransferFcn: '<S32>/Discrete Transfer Fcn' incorporates:
  //   Product: '<S10>/Matrix Multiply1'
  //   Product: '<S10>/Product'
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.DiscreteTransferFcn_tmp = (rtb_Product2 *
    stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] + rtb_UnitDelay_g *
    stateEstimatorEskf_DW.UnitDelay2_DSTATE[3]) - den[1] *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;
  rtb_UnitDelay_f = rtb_VectorConcatenate_k2[0] *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp + rtb_VectorConcatenate_k2[1] *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states;

  // MATLAB Function: '<S31>/Compute Filter Numerator And Denominator'
  ComputeFilterNumeratorAndDenomi(&rtu_mtf01pParams->filterBw_radps,
    rtb_VectorConcatenate_k2, den, 0.004);

  // DiscreteTransferFcn: '<S31>/Discrete Transfer Fcn' incorporates:
  //   Product: '<S10>/Matrix Multiply1'
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b = (rtb_Product2 *
    stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] + rtb_UnitDelay_g *
    stateEstimatorEskf_DW.UnitDelay2_DSTATE[4]) - den[1] *
    stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;
  rtb_UnitDelay_d = rtb_VectorConcatenate_k2[0] *
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b + rtb_VectorConcatenate_k2[1]
    * stateEstimatorEskf_DW.DiscreteTransferFcn_states_n;

  // Sum: '<S8>/Sum1' incorporates:
  //   Gain: '<S8>/Gain'
  //   Product: '<S8>/Product1'
  //   Product: '<S8>/Product2'
  //   Product: '<S8>/Product3'
  //   Sum: '<S8>/Sum'
  //   Trigonometry: '<S8>/Cos1'
  //   UnitDelay: '<Root>/Unit Delay1'

  rtb_Product3 = rtb_Product3 * rtu_lidarData->range_m -
    (rtu_lidarParams->yMntOff_m * -std::sin
     (stateEstimatorEskf_DW.UnitDelay1_DSTATE[0]) + rtu_lidarParams->zMntOff_m *
     rtb_Product3);

  // UnitDelay: '<S7>/Unit Delay'
  rtb_UnitDelay_g = stateEstimatorEskf_DW.UnitDelay_DSTATE_j;

  // Logic: '<S10>/AND' incorporates:
  //   MATLAB Function: '<S10>/getMtf01pValidity'

  // This function generates a validity flag based to inform EKF when to not
  // use optical flow data
  // MATLAB Function 'opticalFlowToNeVel/getMtf01pValidity': '<S30>:1'
  // '<S30>:1:4' mtf01pValidityFlag = true;
  // Update flag based distance health
  // '<S30>:1:7' mtf01pValidityFlag = mtf01pValidityFlag & (mtf01pData.distPrecision <= mtf01pParams.distPrecisionThr) & ... 
  // '<S30>:1:8'     (mtf01pData.distStatus == 1) & (mtf01pData.dist_m > mtf01pParams.distLimit_m(1)) & (mtf01pData.dist_m < mtf01pParams.distLimit_m(2)); 
  // Update flag based on flow health
  // '<S30>:1:11' mtf01pValidityFlag = mtf01pValidityFlag & (mtf01pData.flowStatus == 1); 
  rtb_AND = static_cast<boolean_T>(static_cast<boolean_T>(static_cast<boolean_T>
    (static_cast<boolean_T>(static_cast<boolean_T>
    ((rtu_mtf01pData->distPrecision <= rtu_mtf01pParams->distPrecisionThr) &
     (rtu_mtf01pData->distStatus == 1)) & (rtu_mtf01pData->dist_m >
    rtu_mtf01pParams->distLimit_m[0])) & (rtu_mtf01pData->dist_m <
    rtu_mtf01pParams->distLimit_m[1])) & (rtu_mtf01pData->flowStatus == 1)) &
    rtu_mtf01pData->isMtf01pDataValid) & rtu_stateEstSmParams->useOpticalFlow;

  // Chart: '<S10>/Chart'
  // Gateway: opticalFlowToNeVel/Chart
  // During: opticalFlowToNeVel/Chart
  if (stateEstimatorEskf_DW.is_active_c5_stateEstimatorEskf == 0U) {
    // Entry: opticalFlowToNeVel/Chart
    stateEstimatorEskf_DW.is_active_c5_stateEstimatorEskf = 1U;

    // Entry Internal: opticalFlowToNeVel/Chart
    // Transition: '<S28>:2'
    stateEstimatorEskf_DW.is_c5_stateEstimatorEskf =
      stateEstimatorEsk_IN_OF_INVALID;

    // Entry 'OF_INVALID': '<S28>:1'
    // '<S28>:1:3' isOfDataValidOut = false;
    rtb_isOfDataValidOut = false;
  } else if (stateEstimatorEskf_DW.is_c5_stateEstimatorEskf ==
             stateEstimatorEsk_IN_OF_INVALID) {
    rtb_isOfDataValidOut = false;

    // During 'OF_INVALID': '<S28>:1'
    // '<S28>:5:1' sf_internal_predicateOutput = isOfDataValidIn;
    if (rtb_AND) {
      // Transition: '<S28>:5'
      stateEstimatorEskf_DW.durationCounter_1 = 0;
      stateEstimatorEskf_DW.is_c5_stateEstimatorEskf =
        stateEstimatorEskf_IN_OF_VALID;

      // Entry 'OF_VALID': '<S28>:4'
      // '<S28>:4:3' isOfDataValidOut = true;
      rtb_isOfDataValidOut = true;
    }
  } else {
    rtb_isOfDataValidOut = true;

    // During 'OF_VALID': '<S28>:4'
    // '<S28>:8:1' sf_internal_predicateOutput = duration(~isOfDataValidIn) > ... 
    // '<S28>:8:2' stateEstSmParams.ofLossCheckDuration_s;
    if (rtb_AND) {
      stateEstimatorEskf_DW.durationCounter_1 = 0;
    }

    if (static_cast<real_T>(stateEstimatorEskf_DW.durationCounter_1) >
        rtu_stateEstSmParams->ofLossCheckDuration_s * 250.0F) {
      // Transition: '<S28>:8'
      stateEstimatorEskf_DW.is_c5_stateEstimatorEskf =
        stateEstimatorEsk_IN_OF_INVALID;

      // Entry 'OF_INVALID': '<S28>:1'
      // '<S28>:1:3' isOfDataValidOut = false;
      rtb_isOfDataValidOut = false;
    }
  }

  if (static_cast<boolean_T>(rtb_AND ^ 1)) {
    stateEstimatorEskf_DW.durationCounter_1++;
  } else {
    stateEstimatorEskf_DW.durationCounter_1 = 0;
  }

  // End of Chart: '<S10>/Chart'

  // BusAssignment: '<Root>/Bus Assignment'
  Divide[0] = stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[0];
  dTheta[0] = stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[0];
  rtb_UnitDelay1_idx_0 = stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[0];
  Divide[1] = stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[1];
  dTheta[1] = stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[1];
  rtb_UnitDelay1_idx_1 = stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[1];
  Divide[2] = stateEstimatorEskf_DW.sensorDataOut.bodyAccels_mps2[2];
  dTheta[2] = stateEstimatorEskf_DW.sensorDataOut.bodyRates_radps[2];
  rtb_UnitDelay1_idx_2 = stateEstimatorEskf_DW.sensorDataOut.normMagVec_nd[2];
  rtb_AND = stateEstimatorEskf_DW.sensorDataOut.isMagValid;
  rtb_sensorIn_isGpsValid = stateEstimatorEskf_DW.sensorDataOut.isGpsValid;
  rtb_Product2 = stateEstimatorEskf_DW.sensorDataOut.baroAlt_m;
  rtb_sensorIn_isBaroValid = stateEstimatorEskf_DW.sensorDataOut.isBaroValid;

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

  rtb_Delay2[0] = stateEstimatorEskf_DW.Delay2_DSTATE[0];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_0 = stateEstimatorEskf_DW.Delay2_DSTATE[0];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[1] = stateEstimatorEskf_DW.Delay2_DSTATE[1];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_1 = stateEstimatorEskf_DW.Delay2_DSTATE[1];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[2] = stateEstimatorEskf_DW.Delay2_DSTATE[2];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_2 = stateEstimatorEskf_DW.Delay2_DSTATE[2];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[3] = stateEstimatorEskf_DW.Delay2_DSTATE[3];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_3 = stateEstimatorEskf_DW.Delay2_DSTATE[3];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[4] = stateEstimatorEskf_DW.Delay2_DSTATE[4];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_4 = stateEstimatorEskf_DW.Delay2_DSTATE[4];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[5] = stateEstimatorEskf_DW.Delay2_DSTATE[5];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_5 = stateEstimatorEskf_DW.Delay2_DSTATE[5];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[6] = stateEstimatorEskf_DW.Delay2_DSTATE[6];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_6 = stateEstimatorEskf_DW.Delay2_DSTATE[6];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[7] = stateEstimatorEskf_DW.Delay2_DSTATE[7];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   Delay: '<S1>/Delay2'

  rtb_dcmBodyToNed_idx_7 = stateEstimatorEskf_DW.Delay2_DSTATE[7];

  // Delay: '<S1>/Delay2'
  rtb_Delay2[8] = stateEstimatorEskf_DW.Delay2_DSTATE[8];

  // MATLAB Function: '<S1>/EKF' incorporates:
  //   BusAssignment: '<Root>/Bus Assignment'
  //   Constant: '<Root>/localNedMag_nd'
  //   DataTypeConversion: '<S7>/Cast To Single'
  //   Delay: '<S1>/Delay'
  //   Delay: '<S1>/Delay2'
  //   DiscreteTransferFcn: '<S31>/Discrete Transfer Fcn'
  //   DiscreteTransferFcn: '<S32>/Discrete Transfer Fcn'
  //   Logic: '<S8>/AND'
  //   Logic: '<S8>/AND1'
  //   Logic: '<S8>/NOT'
  //   Logic: '<S8>/OR'
  //   RelationalOperator: '<S8>/Less Than'
  //   RelationalOperator: '<S8>/Less Than1'
  //   Sum: '<S7>/Sum'
  //   UnitDelay: '<S7>/Unit Delay'

  rtb_dcmBodyToNed_idx_8 = stateEstimatorEskf_DW.Delay2_DSTATE[8];

  // MATLAB Function 'EKF/EKF': '<S12>:1'
  // '<S12>:1:5' if isempty(covP) || resetStates
  if (static_cast<boolean_T>(static_cast<boolean_T>
       (stateEstimatorEskf_DW.covP_not_empty ^ 1) |
       stateEstimatorEskf_DW.resetStates)) {
    // '<S12>:1:6' covP = initCovP;
    std::memcpy(&stateEstimatorEskf_DW.covP[0], &rtu_initCovP[0], 361U * sizeof
                (real32_T));
    stateEstimatorEskf_DW.covP_not_empty = true;
  }

  // '<S12>:1:9' [states, covP, dcmBodyToNed, ekfDebug] = errorStateEkf_function2(sensorIn, prevStates, covP, prevDcmBodyToNed, estSmMode, ... 
  // '<S12>:1:10'     processNoiseQ, measNoiseR, gEarth_mps2, ekfParams, sampleTime_s); 
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
    std::memcpy(&rty_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                sizeof(real32_T));
  } else {
    int32_T H_tmp;
    int32_T i_0;
    int32_T i_2;
    int32_T i_3;
    int8_T c;
    boolean_T gpsLossFlag;

    // ErrorStateHat
    // 'errorStateEkf_function2:83' errorStateHat = zeros(19, 1, 'single');
    std::memset(&errorStateHat[0], 0, 19U * sizeof(real32_T));

    // 'errorStateEkf_function2:85' isImuDataValid = sensorIn.isImuDataValid;
    //  if(isImuDataValid)
    // Get all the sensor data
    // 'errorStateEkf_function2:89' bodyAccels_mps2 = sensorIn.bodyAccels_mps2;
    // 'errorStateEkf_function2:90' bodyRates_radps = sensorIn.bodyRates_radps;
    // 'errorStateEkf_function2:91' dtImuTime_s = sensorIn.dtImuTime_s;
    // 'errorStateEkf_function2:93' normMagVec_nd = sensorIn.normMagVec_nd;
    // 'errorStateEkf_function2:94' localNedUnitMag_nd = sensorIn.localNedUnitMag_nd; 
    localNedUnitMag_nd[0] = 0.4752F;
    localNedUnitMag_nd[1] = 0.1096F;
    localNedUnitMag_nd[2] = 0.873F;

    // 'errorStateEkf_function2:95' isMagValid = sensorIn.isMagValid;
    // 'errorStateEkf_function2:96' nedPosAndVel =  [sensorIn.nedPosAndVel.pos_m; sensorIn.nedPosAndVel.vel_mps]; 
    nedPosAndVel[0] = static_cast<real32_T>(rtb_nedPos_m_idx_0);
    nedPosAndVel[1] = static_cast<real32_T>(rtb_nedPos_m_idx_1);
    nedPosAndVel[2] = static_cast<real32_T>(rtb_nedPos_m_idx_2) +
      stateEstimatorEskf_DW.UnitDelay_DSTATE_j;
    nedPosAndVel[3] = rtu_gpsData->nedVel_mps[0];
    nedPosAndVel[4] = rtu_gpsData->nedVel_mps[1];
    nedPosAndVel[5] = rtu_gpsData->nedVel_mps[2];

    // 'errorStateEkf_function2:97' isGpsValid = sensorIn.isGpsValid;
    // 'errorStateEkf_function2:98' baroAlt_m = sensorIn.baroAlt_m;
    // 'errorStateEkf_function2:99' isBaroValid = sensorIn.isBaroValid;
    // 'errorStateEkf_function2:100' lidarAgl_m = sensorIn.lidarAgl_m;
    // 'errorStateEkf_function2:101' isLidarValid = sensorIn.isLidarValid;
    // 'errorStateEkf_function2:102' ofNeVel_mps = sensorIn.ofNeVel_mps;
    // 'errorStateEkf_function2:103' isOfValid = sensorIn.isOfValid;
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
    // 'errorStateEkf_function2:148' if(isImuDataValid)
    if (rtu_imuData->isImuDataValid) {
      // 'errorStateEkf_function2:149' [states, dThetaNorm, dThetaUnit] = updateEskfStates(prevStates, bodyAccels_mps2, bodyRates_radps, ... 
      // 'errorStateEkf_function2:150'             dcmBodyToNed, estSmMode, isOfValid, dtImuTime_s, gEarth_mps2); 
      std::memcpy(&rty_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
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
           rtb_isOfDataValidOut)) {
        // 'updateEskfStates:28' stateDot = [ [states(8); states(9); states(10)]; ...                            % Derivative of [pN; pE; pD] 
        // 'updateEskfStates:29'     dcmBodyToNed * (bodyAccels_mps2 - [states(14);states(15);states(16)]) + ... 
        // 'updateEskfStates:30'     [0; 0; gEarth_mps2]];
        //                             % Derivative of [pN; pE; pD]
        //                                                        % Derivative of [vN; vE; vD] 
        // 'updateEskfStates:31' states(5:10) = states(5:10) + sampleTime_s * stateDot; 
        rtb_dcmBodyToNed_idx_0 = Divide[0] - rtb_prevStates[13];
        rtb_dcmBodyToNed_idx_3 = Divide[1] - rtb_prevStates[14];
        rtb_dcmBodyToNed_idx_6 = Divide[2] - rtb_prevStates[15];
        rty_states[4] = rtu_imuData->dtImuTime_s * rtb_prevStates[7] +
          rtb_prevStates[4];
        rty_states[7] = ((rtb_Delay2[0] * rtb_dcmBodyToNed_idx_0 + rtb_Delay2[3]
                          * rtb_dcmBodyToNed_idx_3) + rtb_Delay2[6] *
                         rtb_dcmBodyToNed_idx_6) * rtu_imuData->dtImuTime_s +
          rtb_prevStates[7];
        rty_states[5] = rtu_imuData->dtImuTime_s * rtb_prevStates[8] +
          rtb_prevStates[5];
        rty_states[8] = ((rtb_Delay2[1] * rtb_dcmBodyToNed_idx_0 + rtb_Delay2[4]
                          * rtb_dcmBodyToNed_idx_3) + rtb_Delay2[7] *
                         rtb_dcmBodyToNed_idx_6) * rtu_imuData->dtImuTime_s +
          rtb_prevStates[8];
        rty_states[6] = rtu_imuData->dtImuTime_s * rtb_prevStates[9] +
          rtb_prevStates[6];
        rty_states[9] = (((rtb_Delay2[2] * rtb_dcmBodyToNed_idx_0 + rtb_Delay2[5]
                           * rtb_dcmBodyToNed_idx_3) + rtb_Delay2[8] *
                          rtb_dcmBodyToNed_idx_6) + *rtu_gEarth_mps2) *
          rtu_imuData->dtImuTime_s + rtb_prevStates[9];
      } else {
        // 'updateEskfStates:32' else
        // 'updateEskfStates:33' stateDot = states(10);
        //  %Derivative of down position
        // 'updateEskfStates:35' states(7) = states(7) + sampleTime_s * stateDot; 
        rty_states[6] = rtu_imuData->dtImuTime_s * rtb_prevStates[9] +
          rtb_prevStates[6];
      }

      // 'updateEskfStates:38' dTheta = (bodyRates_radps - states(11:13))*sampleTime_s; 
      dTheta[0] = (dTheta[0] - rty_states[10]) * rtu_imuData->dtImuTime_s;
      dTheta[1] = (dTheta[1] - rty_states[11]) * rtu_imuData->dtImuTime_s;
      dTheta[2] = (dTheta[2] - rty_states[12]) * rtu_imuData->dtImuTime_s;

      // 'updateEskfStates:39' dThetaNorm = norm(dTheta);
      rtb_XAxis = norm_yrNKZSBO(dTheta);

      // 'updateEskfStates:41' if dThetaNorm > 1e-7
      if (rtb_XAxis > 1.0E-7) {
        // Propagate the quaternion part of the state
        // 'updateEskfStates:43' dThetaUnit = dTheta/dThetaNorm;
        // 'updateEskfStates:44' states(1:4) = quatMultiply(states(1:4), [cos(dThetaNorm*0.5); sin(dThetaNorm*0.5)*dThetaUnit]); 
        rtb_XAxis1 = std::sin(rtb_XAxis * 0.5F);
        tmp_6[0] = std::cos(rtb_XAxis * 0.5F);
        rtb_dcmBodyToNed_idx_0 = dTheta[0] / rtb_XAxis;
        tmp_6[1] = rtb_XAxis1 * rtb_dcmBodyToNed_idx_0;
        dTheta[0] = rtb_dcmBodyToNed_idx_0;
        rtb_dcmBodyToNed_idx_0 = dTheta[1] / rtb_XAxis;
        tmp_6[2] = rtb_XAxis1 * rtb_dcmBodyToNed_idx_0;
        dTheta[1] = rtb_dcmBodyToNed_idx_0;
        rtb_dcmBodyToNed_idx_0 = dTheta[2] / rtb_XAxis;
        tmp_6[3] = rtb_XAxis1 * rtb_dcmBodyToNed_idx_0;
        dTheta[2] = rtb_dcmBodyToNed_idx_0;
        quatMultiply_AJk9aby2(&rty_states[0], tmp_6, nQuat_tmp);
        rty_states[0] = nQuat_tmp[0];
        rty_states[1] = nQuat_tmp[1];
        rty_states[2] = nQuat_tmp[2];
        rty_states[3] = nQuat_tmp[3];

        // Normalize the quaternion
        // 'updateEskfStates:46' nQuat = norm(states(1:4));
        rtb_XAxis1 = norm_7MzYkgry(&rty_states[0]);

        // 'updateEskfStates:47' if nQuat > 1e-7
        if (rtb_XAxis1 > 1.0E-7) {
          // 'updateEskfStates:48' states(1:4) = states(1:4)/nQuat;
          rty_states[0] /= rtb_XAxis1;
          rty_states[1] /= rtb_XAxis1;
          rty_states[2] /= rtb_XAxis1;
          rty_states[3] /= rtb_XAxis1;
        }
      } else {
        // 'updateEskfStates:50' else
        // 'updateEskfStates:51' dThetaUnit = single([0; 0; 0]);
        dTheta[0] = 0.0F;
        dTheta[1] = 0.0F;
        dTheta[2] = 0.0F;
      }

      // 'errorStateEkf_function2:151' errorStateJac = computeEskfStateJac(prevStates, dcmBodyToNed, bodyAccels_mps2, ... 
      // 'errorStateEkf_function2:152'             dThetaNorm, dThetaUnit, gpsLossFlag, isOfValid, dtImuTime_s); 
      // 'errorStateEkf_function2:153' covP = updateEskfCovP(covP, errorStateJac, processNoiseQ, dtImuTime_s); 
      computeEskfStateJac_4pNsZTDW(rtb_prevStates, rtb_Delay2, Divide, rtb_XAxis,
        dTheta, gpsLossFlag, rtb_isOfDataValidOut, rtu_imuData->dtImuTime_s,
        tmp_8);
      updateEskfCovP_yMTfUr7W(stateEstimatorEskf_DW.covP, tmp_8,
        rtu_processNoiseQ, rtu_imuData->dtImuTime_s);
    } else {
      // 'errorStateEkf_function2:154' else
      // 'errorStateEkf_function2:155' states = prevStates;
      std::memcpy(&rty_states[0], &stateEstimatorEskf_DW.Delay_DSTATE[0], 20U *
                  sizeof(real32_T));
    }

    // Fuse Accel in Correction step if GPS is not available
    // 'errorStateEkf_function2:159' if estSmMode ~= enumStateEstimateMode.RUN && ~isOfValid && ... 
    // 'errorStateEkf_function2:160'             isImuDataValid
    if (static_cast<boolean_T>(static_cast<boolean_T>((mode !=
           enumStateEstimateMode::RUN) & static_cast<boolean_T>
          (rtb_isOfDataValidOut ^ 1)) & rtu_imuData->isImuDataValid)) {
      // 'errorStateEkf_function2:161' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:403' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:404'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:405'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:406'     -states(3), states(2), states(1)];
      rtb_dcmBodyToNed_idx_0 = 0.5F * -rty_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_dcmBodyToNed_idx_0;
      rtb_dcmBodyToNed_idx_3 = 0.5F * -rty_states[2];
      stateEstimatorEskf_DW.xErrorJac[20] = rtb_dcmBodyToNed_idx_3;
      rtb_dcmBodyToNed_idx_6 = 0.5F * -rty_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_dcmBodyToNed_idx_6;
      stateEstimatorEskf_DW.xErrorJac[1] = 0.5F * rty_states[0];
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_dcmBodyToNed_idx_6;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rty_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rty_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = 0.5F * rty_states[0];
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_dcmBodyToNed_idx_0;
      stateEstimatorEskf_DW.xErrorJac[3] = rtb_dcmBodyToNed_idx_3;
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rty_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = 0.5F * rty_states[0];

      // 'errorStateEkf_function2:163' measJac = computeEskfAccelMeasJac(states, gEarth_mps2); 
      // 'errorStateEkf_function2:164' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:165' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      computeEskfAccelMeasJa_g9O3pYCr(rty_states, *rtu_gEarth_mps2, tmp);
      i_0 = 0;
      i_3 = 0;
      for (i = 0; i < 3; i++) {
        for (i_2 = 0; i_2 < 3; i_2++) {
          H_tmp = i_2 + i_0;
          H[H_tmp] = 0.0F;
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3] * tmp[i_2];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 1] * tmp[i_2 + 3];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 2] * tmp[i_2 + 6];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 3] * tmp[i_2 + 9];
        }

        i_0 += 3;
        i_3 += 20;
      }

      // 'errorStateEkf_function2:166' H(1:3, 13:15) = I3;
      H[36] = stateEstimatorEskf_DW.I3[0];
      H[37] = stateEstimatorEskf_DW.I3[1];
      H[38] = stateEstimatorEskf_DW.I3[2];
      H[39] = stateEstimatorEskf_DW.I3[3];
      H[40] = stateEstimatorEskf_DW.I3[4];
      H[41] = stateEstimatorEskf_DW.I3[5];
      H[42] = stateEstimatorEskf_DW.I3[6];
      H[43] = stateEstimatorEskf_DW.I3[7];
      H[44] = stateEstimatorEskf_DW.I3[8];

      //      H = measJac(:, idxNs) * xErrorJac(idxNs, idxEs);
      // 'errorStateEkf_function2:170' C_ned2b  = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:171'             states(4));
      // NED gravity in body frame
      // 'errorStateEkf_function2:173' estGravityInBodyFrame = C_ned2b*[0; 0; -gEarth_mps2] + states(14:16); 
      // 'errorStateEkf_function2:175' tmp1 = covP(:, 13:15) + covP(:, 1:3) * H(:, 1:3).'; 
      for (i_0 = 0; i_0 < 19; i_0++) {
        rtb_dcmBodyToNed_idx_0 = stateEstimatorEskf_DW.covP[i_0 + 19];
        rtb_dcmBodyToNed_idx_3 = stateEstimatorEskf_DW.covP[i_0 + 38];
        tmp1[i_0] = ((rtb_dcmBodyToNed_idx_0 * H[3] +
                      stateEstimatorEskf_DW.covP[i_0] * H[0]) +
                     rtb_dcmBodyToNed_idx_3 * H[6]) +
          stateEstimatorEskf_DW.covP[i_0 + 228];
        tmp1[i_0 + 19] = ((rtb_dcmBodyToNed_idx_0 * H[4] +
                           stateEstimatorEskf_DW.covP[i_0] * H[1]) +
                          rtb_dcmBodyToNed_idx_3 * H[7]) +
          stateEstimatorEskf_DW.covP[i_0 + 247];
        tmp1[i_0 + 38] = ((rtb_dcmBodyToNed_idx_0 * H[5] +
                           stateEstimatorEskf_DW.covP[i_0] * H[2]) +
                          rtb_dcmBodyToNed_idx_3 * H[8]) +
          stateEstimatorEskf_DW.covP[i_0 + 266];
      }

      // covP(idxEs, idxEs)*H';
      // 'errorStateEkf_function2:176' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(12:14, 12:14)); 
      for (i_0 = 0; i_0 < 3; i_0++) {
        for (i_3 = 0; i_3 < 15; i_3++) {
          tmp1_0[i_3 + 15 * i_0] = tmp1[19 * i_0 + c_0[i_3]];
        }

        for (i_3 = 0; i_3 < 3; i_3++) {
          rtb_dcmBodyToNed_idx_0 = 0.0F;
          for (i = 0; i < 15; i++) {
            c = c_0[i];
            rtb_dcmBodyToNed_idx_0 += H[3 * c + i_0] * tmp1[19 * i_3 + c];
          }

          rtb_Delay2[i_0 + 3 * i_3] = rtu_measNoiseR[((i_3 + 11) * 15 + i_0) +
            11] + rtb_dcmBodyToNed_idx_0;
        }
      }

      mrdiv_s6DFIKmD(tmp1_0, rtb_Delay2, K);

      // 'errorStateEkf_function2:178' errorStateHat(idxNoGpsEs) = K*(bodyAccels_mps2 - estGravityInBodyFrame); 
      quatToDcm_4oGXZFqp(rty_states[0], rty_states[1], rty_states[2],
                         rty_states[3], tmp_3);
      rtb_dcmBodyToNed_idx_0 = Divide[0] - (static_cast<real32_T>(tmp_3[6]) *
        -*rtu_gEarth_mps2 + rty_states[13]);
      rtb_dcmBodyToNed_idx_3 = Divide[1] - (static_cast<real32_T>(tmp_3[7]) *
        -*rtu_gEarth_mps2 + rty_states[14]);
      rtb_dcmBodyToNed_idx_6 = Divide[2] - (static_cast<real32_T>(tmp_3[8]) *
        -*rtu_gEarth_mps2 + rty_states[15]);
      for (i_0 = 0; i_0 < 15; i_0++) {
        errorStateHat[c_0[i_0]] = (K[i_0 + 15] * rtb_dcmBodyToNed_idx_3 + K[i_0]
          * rtb_dcmBodyToNed_idx_0) + K[i_0 + 30] * rtb_dcmBodyToNed_idx_6;
      }

      // 'errorStateEkf_function2:180' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(13:15, idxNoGpsEs) + ... 
      // 'errorStateEkf_function2:181'             H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
      for (i_0 = 0; i_0 < 3; i_0++) {
        for (i_3 = 0; i_3 < 15; i_3++) {
          i = 19 * c_0[i_3];
          tmp1_0[i_0 + 3 * i_3] = ((stateEstimatorEskf_DW.covP[i + 1] * H[i_0 +
            3] + stateEstimatorEskf_DW.covP[i] * H[i_0]) +
            stateEstimatorEskf_DW.covP[i + 2] * H[i_0 + 6]) +
            stateEstimatorEskf_DW.covP[(i + i_0) + 12];
        }
      }

      for (i_0 = 0; i_0 < 15; i_0++) {
        for (i_3 = 0; i_3 < 15; i_3++) {
          tmp_4[i_0 + 15 * i_3] = stateEstimatorEskf_DW.covP[19 * c_0[i_3] +
            c_0[i_0]] - ((tmp1_0[3 * i_3 + 1] * K[i_0 + 15] + tmp1_0[3 * i_3] *
                          K[i_0]) + tmp1_0[3 * i_3 + 2] * K[i_0 + 30]);
        }
      }

      for (i_0 = 0; i_0 < 15; i_0++) {
        for (i_3 = 0; i_3 < 15; i_3++) {
          stateEstimatorEskf_DW.covP[c_0[i_3] + 19 * c_0[i_0]] = tmp_4[15 * i_0
            + i_3];
        }
      }

      // Update the nominal state
      // 'errorStateEkf_function2:184' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
      for (i_0 = 0; i_0 < 12; i_0++) {
        rty_states_0[i_0] = rty_states[e[i_0]] + errorStateHat[f[i_0]];
      }

      for (i_0 = 0; i_0 < 12; i_0++) {
        rty_states[e[i_0]] = rty_states_0[i_0];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:187' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      nQuat_tmp[0] = rty_states[0];
      nQuat_tmp[1] = rty_states[1];
      nQuat_tmp[2] = rty_states[2];
      nQuat_tmp[3] = rty_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:188' states(1:4) = nomQuat;
      rty_states[0] = nQuat_tmp[0];
      rty_states[1] = nQuat_tmp[1];
      rty_states[2] = nQuat_tmp[2];
      rty_states[3] = nQuat_tmp[3];

      // 'errorStateEkf_function2:190' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + covP(idxNoGpsEs, idxNoGpsEs)')/2; 
      for (i_0 = 0; i_0 < 15; i_0++) {
        for (i_3 = 0; i_3 < 15; i_3++) {
          H_tmp = 19 * c_0[i_0] + c_0[i_3];
          stateEstimatorEskf_DW.covP[H_tmp] = (covP[19 * c_0[i_3] + c_0[i_0]] +
            covP[H_tmp]) / 2.0F;
        }
      }
    }

    // Fuse Mag data if it is valid
    // 'errorStateEkf_function2:194' if(isMagValid)
    if (rtb_AND) {
      // 'errorStateEkf_function2:195' measJac = computEskfMagMeasJac(states, localNedUnitMag_nd); 
      // 'errorStateEkf_function2:197' C_ned2b = quatToDcm(states(1), states(2), states(3), ... 
      // 'errorStateEkf_function2:198'             states(4));
      // Rotate propogated mag states and add bias to estimate measurements
      // 'errorStateEkf_function2:201' estBodyMagUnitVec = C_ned2b*localNedUnitMag_nd + states(17:19); 
      // 'errorStateEkf_function2:203' xErrorJac = computeQuatJacWrtAngErr(states, xErrorJac); 
      // 'errorStateEkf_function2:403' xErrorJac(1:4, 1:3) = 0.5*[-states(2), -states(3), -states(4); 
      // 'errorStateEkf_function2:404'     states(1), -states(4), states(3);
      // 'errorStateEkf_function2:405'     states(4), states(1), -states(2);
      // 'errorStateEkf_function2:406'     -states(3), states(2), states(1)];
      rtb_dcmBodyToNed_idx_0 = 0.5F * -rty_states[1];
      stateEstimatorEskf_DW.xErrorJac[0] = rtb_dcmBodyToNed_idx_0;
      stateEstimatorEskf_DW.xErrorJac[20] = 0.5F * -rty_states[2];
      rtb_dcmBodyToNed_idx_3 = 0.5F * -rty_states[3];
      stateEstimatorEskf_DW.xErrorJac[40] = rtb_dcmBodyToNed_idx_3;
      stateEstimatorEskf_DW.xErrorJac[1] = 0.5F * rty_states[0];
      stateEstimatorEskf_DW.xErrorJac[21] = rtb_dcmBodyToNed_idx_3;
      stateEstimatorEskf_DW.xErrorJac[41] = 0.5F * rty_states[2];
      stateEstimatorEskf_DW.xErrorJac[2] = 0.5F * rty_states[3];
      stateEstimatorEskf_DW.xErrorJac[22] = 0.5F * rty_states[0];
      stateEstimatorEskf_DW.xErrorJac[42] = rtb_dcmBodyToNed_idx_0;
      stateEstimatorEskf_DW.xErrorJac[3] = 0.5F * -rty_states[2];
      stateEstimatorEskf_DW.xErrorJac[23] = 0.5F * rty_states[1];
      stateEstimatorEskf_DW.xErrorJac[43] = 0.5F * rty_states[0];

      // 'errorStateEkf_function2:205' H = zeros(3, 19, 'single');
      std::memset(&H[0], 0, 57U * sizeof(real32_T));

      // 'errorStateEkf_function2:206' H(1:3, 1:3) = measJac(:, 1:4) * xErrorJac(1:4, 1:3); 
      computEskfMagMeasJac_U07joj0p(rty_states, localNedUnitMag_nd, tmp);
      i_0 = 0;
      i_3 = 0;
      for (i = 0; i < 3; i++) {
        for (i_2 = 0; i_2 < 3; i_2++) {
          H_tmp = i_2 + i_0;
          H[H_tmp] = 0.0F;
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3] * tmp[i_2];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 1] * tmp[i_2 + 3];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 2] * tmp[i_2 + 6];
          H[H_tmp] += stateEstimatorEskf_DW.xErrorJac[i_3 + 3] * tmp[i_2 + 9];
        }

        i_0 += 3;
        i_3 += 20;
      }

      // 'errorStateEkf_function2:207' H(1:3, 16:18) = I3;
      H[45] = stateEstimatorEskf_DW.I3[0];
      H[46] = stateEstimatorEskf_DW.I3[1];
      H[47] = stateEstimatorEskf_DW.I3[2];
      H[48] = stateEstimatorEskf_DW.I3[3];
      H[49] = stateEstimatorEskf_DW.I3[4];
      H[50] = stateEstimatorEskf_DW.I3[5];
      H[51] = stateEstimatorEskf_DW.I3[6];
      H[52] = stateEstimatorEskf_DW.I3[7];
      H[53] = stateEstimatorEskf_DW.I3[8];

      // 'errorStateEkf_function2:208' tmp1 = covP(:, 16:18) + covP(:, 1:3) * H(:,1:3).'; 
      for (i_0 = 0; i_0 < 19; i_0++) {
        rtb_dcmBodyToNed_idx_0 = stateEstimatorEskf_DW.covP[i_0 + 19];
        rtb_dcmBodyToNed_idx_3 = stateEstimatorEskf_DW.covP[i_0 + 38];
        tmp1[i_0] = ((rtb_dcmBodyToNed_idx_0 * H[3] +
                      stateEstimatorEskf_DW.covP[i_0] * H[0]) +
                     rtb_dcmBodyToNed_idx_3 * H[6]) +
          stateEstimatorEskf_DW.covP[i_0 + 285];
        tmp1[i_0 + 19] = ((rtb_dcmBodyToNed_idx_0 * H[4] +
                           stateEstimatorEskf_DW.covP[i_0] * H[1]) +
                          rtb_dcmBodyToNed_idx_3 * H[7]) +
          stateEstimatorEskf_DW.covP[i_0 + 304];
        tmp1[i_0 + 38] = ((rtb_dcmBodyToNed_idx_0 * H[5] +
                           stateEstimatorEskf_DW.covP[i_0] * H[2]) +
                          rtb_dcmBodyToNed_idx_3 * H[8]) +
          stateEstimatorEskf_DW.covP[i_0 + 323];
      }

      // covP(idxEs, idxEs)*H(:, idxEs)';
      // 'errorStateEkf_function2:210' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
           rtb_isOfDataValidOut)) {
        // 'errorStateEkf_function2:211' K = tmp1(idxEs, :)/(H(:, idxEs) * tmp1(idxEs, :) + measNoiseR(1:3, 1:3)); 
        for (i_0 = 0; i_0 < 3; i_0++) {
          i_3 = 0;
          i = 0;
          i_2 = 0;
          for (H_tmp = 0; H_tmp < 3; H_tmp++) {
            int32_T tmp_7;
            rtb_dcmBodyToNed_idx_0 = 0.0F;
            tmp_7 = 0;
            for (int32_T i_1{0}; i_1 < 19; i_1++) {
              rtb_dcmBodyToNed_idx_0 += H[tmp_7 + i_0] * tmp1[i_1 + i_2];
              tmp_7 += 3;
            }

            rtb_Delay2[i_3 + i_0] = rtu_measNoiseR[i + i_0] +
              rtb_dcmBodyToNed_idx_0;
            i_3 += 3;
            i += 15;
            i_2 += 19;
          }
        }

        mrdiv_yiWolFAP(tmp1, rtb_Delay2, b_K);

        // 'errorStateEkf_function2:212' errorStateHat(idxEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_4oGXZFqp(rty_states[0], rty_states[1], rty_states[2],
                           rty_states[3], tmp_3);
        rtb_UnitDelay1_idx_0 -= ((static_cast<real32_T>(tmp_3[0]) * 0.4752F +
          static_cast<real32_T>(tmp_3[3]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[6]) * 0.873F) + rty_states[16];
        rtb_UnitDelay1_idx_1 -= ((static_cast<real32_T>(tmp_3[1]) * 0.4752F +
          static_cast<real32_T>(tmp_3[4]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[7]) * 0.873F) + rty_states[17];
        rtb_UnitDelay1_idx_2 -= ((static_cast<real32_T>(tmp_3[2]) * 0.4752F +
          static_cast<real32_T>(tmp_3[5]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[8]) * 0.873F) + rty_states[18];
        for (i_0 = 0; i_0 < 19; i_0++) {
          d_K[i_0] = 0.0F;
          d_K[i_0] += b_K[i_0] * rtb_UnitDelay1_idx_0;
          d_K[i_0] += b_K[i_0 + 19] * rtb_UnitDelay1_idx_1;
          d_K[i_0] += b_K[i_0 + 38] * rtb_UnitDelay1_idx_2;
          errorStateHat[i_0] = d_K[i_0];
        }

        // 'errorStateEkf_function2:214' covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*(covP(16:18, idxEs) + ... 
        // 'errorStateEkf_function2:215'                 H(:, 1:3) * covP(1:3, idxEs)); 
        for (i_0 = 0; i_0 < 3; i_0++) {
          i_3 = 0;
          i = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            tmp1[i_3 + i_0] = ((stateEstimatorEskf_DW.covP[i + 1] * H[i_0 + 3] +
                                stateEstimatorEskf_DW.covP[i] * H[i_0]) +
                               stateEstimatorEskf_DW.covP[i + 2] * H[i_0 + 6]) +
              stateEstimatorEskf_DW.covP[(i + i_0) + 15];
            i_3 += 3;
            i += 19;
          }
        }

        for (i_0 = 0; i_0 < 19; i_0++) {
          i_3 = 0;
          i = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            H_tmp = i_3 + i_0;
            stateEstimatorEskf_DW.covP[H_tmp] -= (tmp1[i + 1] * b_K[i_0 + 19] +
              tmp1[i] * b_K[i_0]) + tmp1[i + 2] * b_K[i_0 + 38];
            i_3 += 19;
            i += 3;
          }
        }

        // Update the nominal state
        // 'errorStateEkf_function2:218' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
        for (i_0 = 0; i_0 < 16; i_0++) {
          rty_states[i_0 + 4] += errorStateHat[i_0 + 3];
        }
      } else {
        // 'errorStateEkf_function2:219' else
        // 'errorStateEkf_function2:220' K = tmp1(idxNoGpsEs, :)/(H(:, idxNoGpsEs) * tmp1(idxNoGpsEs, :) + measNoiseR(1:3, 1:3)); 
        for (i_0 = 0; i_0 < 3; i_0++) {
          for (i_3 = 0; i_3 < 15; i_3++) {
            tmp1_0[i_3 + 15 * i_0] = tmp1[19 * i_0 + c_0[i_3]];
          }

          for (i_3 = 0; i_3 < 3; i_3++) {
            rtb_dcmBodyToNed_idx_0 = 0.0F;
            for (i = 0; i < 15; i++) {
              c = c_0[i];
              rtb_dcmBodyToNed_idx_0 += H[3 * c + i_0] * tmp1[19 * i_3 + c];
            }

            rtb_Delay2[i_0 + 3 * i_3] = rtu_measNoiseR[15 * i_3 + i_0] +
              rtb_dcmBodyToNed_idx_0;
          }
        }

        mrdiv_s6DFIKmD(tmp1_0, rtb_Delay2, K);

        // 'errorStateEkf_function2:221' errorStateHat(idxNoGpsEs) = K*(normMagVec_nd - estBodyMagUnitVec); 
        quatToDcm_4oGXZFqp(rty_states[0], rty_states[1], rty_states[2],
                           rty_states[3], tmp_3);
        rtb_UnitDelay1_idx_0 -= ((static_cast<real32_T>(tmp_3[0]) * 0.4752F +
          static_cast<real32_T>(tmp_3[3]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[6]) * 0.873F) + rty_states[16];
        rtb_UnitDelay1_idx_1 -= ((static_cast<real32_T>(tmp_3[1]) * 0.4752F +
          static_cast<real32_T>(tmp_3[4]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[7]) * 0.873F) + rty_states[17];
        rtb_UnitDelay1_idx_2 -= ((static_cast<real32_T>(tmp_3[2]) * 0.4752F +
          static_cast<real32_T>(tmp_3[5]) * 0.1096F) + static_cast<real32_T>
          (tmp_3[8]) * 0.873F) + rty_states[18];
        for (i_0 = 0; i_0 < 15; i_0++) {
          errorStateHat[c_0[i_0]] = (K[i_0 + 15] * rtb_UnitDelay1_idx_1 + K[i_0]
            * rtb_UnitDelay1_idx_0) + K[i_0 + 30] * rtb_UnitDelay1_idx_2;
        }

        // 'errorStateEkf_function2:223' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - K*(covP(16:18, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:224'                 H(:, 1:3) * covP(1:3, idxNoGpsEs)); 
        for (i_0 = 0; i_0 < 3; i_0++) {
          for (i_3 = 0; i_3 < 15; i_3++) {
            i = 19 * c_0[i_3];
            tmp1_0[i_0 + 3 * i_3] = ((stateEstimatorEskf_DW.covP[i + 1] * H[i_0
              + 3] + stateEstimatorEskf_DW.covP[i] * H[i_0]) +
              stateEstimatorEskf_DW.covP[i + 2] * H[i_0 + 6]) +
              stateEstimatorEskf_DW.covP[(i + i_0) + 15];
          }
        }

        for (i_0 = 0; i_0 < 15; i_0++) {
          for (i_3 = 0; i_3 < 15; i_3++) {
            tmp_4[i_0 + 15 * i_3] = stateEstimatorEskf_DW.covP[19 * c_0[i_3] +
              c_0[i_0]] - ((tmp1_0[3 * i_3 + 1] * K[i_0 + 15] + tmp1_0[3 * i_3] *
                            K[i_0]) + tmp1_0[3 * i_3 + 2] * K[i_0 + 30]);
          }
        }

        for (i_0 = 0; i_0 < 15; i_0++) {
          for (i_3 = 0; i_3 < 15; i_3++) {
            stateEstimatorEskf_DW.covP[c_0[i_3] + 19 * c_0[i_0]] = tmp_4[15 *
              i_0 + i_3];
          }
        }

        // Update the nominal state
        // 'errorStateEkf_function2:227' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
        for (i_0 = 0; i_0 < 12; i_0++) {
          rty_states_0[i_0] = rty_states[e[i_0]] + errorStateHat[f[i_0]];
        }

        for (i_0 = 0; i_0 < 12; i_0++) {
          rty_states[e[i_0]] = rty_states_0[i_0];
        }
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:231' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      nQuat_tmp[0] = rty_states[0];
      nQuat_tmp[1] = rty_states[1];
      nQuat_tmp[2] = rty_states[2];
      nQuat_tmp[3] = rty_states[3];
      std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                  (real32_T));
      updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &errorStateHat[0], covP);
      std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                  (real32_T));

      // 'errorStateEkf_function2:232' states(1:4) = nomQuat;
      rty_states[0] = nQuat_tmp[0];
      rty_states[1] = nQuat_tmp[1];
      rty_states[2] = nQuat_tmp[2];
      rty_states[3] = nQuat_tmp[3];

      // 'errorStateEkf_function2:233' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
      if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
           rtb_isOfDataValidOut)) {
        // 'errorStateEkf_function2:234' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
        i_0 = 0;
        for (i_3 = 0; i_3 < 19; i_3++) {
          i = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            H_tmp = i_2 + i_0;
            stateEstimatorEskf_DW.covP[H_tmp] = (covP[i + i_3] + covP[H_tmp]) *
              0.5F;
            i += 19;
          }

          i_0 += 19;
        }
      } else {
        // 'errorStateEkf_function2:235' else
        // 'errorStateEkf_function2:236' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
        // 'errorStateEkf_function2:237'                 covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
        for (i_0 = 0; i_0 < 15; i_0++) {
          for (i_3 = 0; i_3 < 15; i_3++) {
            H_tmp = 19 * c_0[i_0] + c_0[i_3];
            stateEstimatorEskf_DW.covP[H_tmp] = (covP[19 * c_0[i_3] + c_0[i_0]]
              + covP[H_tmp]) * 0.5F;
          }
        }

        // 'errorStateEkf_function2:238' states(8:9) = 0;
        rty_states[7] = 0.0F;
        rty_states[8] = 0.0F;
      }
    }

    // Integrate optical flow data
    // 'errorStateEkf_function2:243' if(isOfValid)
    if (rtb_isOfDataValidOut) {
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
      // 'errorStateEkf_function2:493' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      rtb_XAxis = 1.0F / (stateEstimatorEskf_DW.covP[120] + rtu_measNoiseR[224]);

      // 'errorStateEkf_function2:494' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_UnitDelay_f - rty_states[7];

      // 'errorStateEkf_function2:495' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:498' errorStateHat = zeros(19, 1, 'single');
      //  if NIS < innovGate
      // 'errorStateEkf_function2:501' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:502' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_dcmBodyToNed_idx_0 = stateEstimatorEskf_DW.covP[i + 114] * rtb_XAxis;
        b_errorStateHat[i] = rtb_dcmBodyToNed_idx_0 * rtb_XAxis1;
        d_K[i] = rtb_dcmBodyToNed_idx_0;
      }

      // 'errorStateEkf_function2:504' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
      i_0 = 0;
      for (i_3 = 0; i_3 < 19; i_3++) {
        for (i = 0; i < 19; i++) {
          H_tmp = i + i_0;
          covP[H_tmp] = stateEstimatorEskf_DW.covP[H_tmp] -
            stateEstimatorEskf_DW.covP[i_0 + 6] * d_K[i];
        }

        i_0 += 19;
      }

      // Update the nominal state
      // 'errorStateEkf_function2:507' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (i_0 = 0; i_0 < 16; i_0++) {
        rty_states[i_0 + 4] += b_errorStateHat[i_0 + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:510' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      nQuat_tmp[0] = rty_states[0];
      nQuat_tmp[1] = rty_states[1];
      nQuat_tmp[2] = rty_states[2];
      nQuat_tmp[3] = rty_states[3];
      updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &b_errorStateHat[0], covP);

      // 'errorStateEkf_function2:511' states(1:4) = nomQuat;
      rty_states[0] = nQuat_tmp[0];
      rty_states[1] = nQuat_tmp[1];
      rty_states[2] = nQuat_tmp[2];
      rty_states[3] = nQuat_tmp[3];

      // 'errorStateEkf_function2:513' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      //  end
      // 'errorStateEkf_function2:274' [states, covP] = applyOfVelCorr(states, ofNeVel_mps, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:275'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      // 'errorStateEkf_function2:493' iS = 1/(covP(idx, idx) + measNoiseR(15, 15)); 
      // 'errorStateEkf_function2:494' nu = ofNeVel_mps(idx - 6)  - states(idx + 1); 
      rtb_XAxis1 = rtb_UnitDelay_d - rty_states[8];

      // 'errorStateEkf_function2:495' NIS = nu*nu*iS;
      // ErrorStateHat
      // 'errorStateEkf_function2:498' errorStateHat = zeros(19, 1, 'single');
      i = 0;
      for (i_2 = 0; i_2 < 19; i_2++) {
        i_0 = 0;
        for (i_3 = 0; i_3 < 19; i_3++) {
          H_tmp = i_3 + i;
          b_covP[H_tmp] = (covP[i_0 + i_2] + covP[H_tmp]) * 0.5F;
          i_0 += 19;
        }

        i += 19;
      }

      rtb_XAxis = 1.0F / (b_covP[140] + rtu_measNoiseR[224]);

      //  if NIS < innovGate
      // 'errorStateEkf_function2:501' K = covP(idxEs, idx).*iS;
      // 'errorStateEkf_function2:502' errorStateHat(idxEs) = K*nu;
      for (i = 0; i < 19; i++) {
        rtb_dcmBodyToNed_idx_0 = b_covP[i + 133] * rtb_XAxis;
        b_errorStateHat[i] = rtb_dcmBodyToNed_idx_0 * rtb_XAxis1;
        d_K[i] = rtb_dcmBodyToNed_idx_0;
      }

      // 'errorStateEkf_function2:504' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
      i_0 = 0;
      for (i_3 = 0; i_3 < 19; i_3++) {
        for (i = 0; i < 19; i++) {
          H_tmp = i + i_0;
          covP[H_tmp] = b_covP[H_tmp] - b_covP[i_0 + 7] * d_K[i];
        }

        i_0 += 19;
      }

      std::memcpy(&b_covP[0], &covP[0], 361U * sizeof(real32_T));

      // Update the nominal state
      // 'errorStateEkf_function2:507' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
      for (i_0 = 0; i_0 < 16; i_0++) {
        rty_states[i_0 + 4] += b_errorStateHat[i_0 + 3];
      }

      // Construct quaternion from the rotation vector and reset covP
      // 'errorStateEkf_function2:510' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
      nQuat_tmp[0] = rty_states[0];
      nQuat_tmp[1] = rty_states[1];
      nQuat_tmp[2] = rty_states[2];
      nQuat_tmp[3] = rty_states[3];
      updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &b_errorStateHat[0], b_covP);

      // 'errorStateEkf_function2:511' states(1:4) = nomQuat;
      rty_states[0] = nQuat_tmp[0];
      rty_states[1] = nQuat_tmp[1];
      rty_states[2] = nQuat_tmp[2];
      rty_states[3] = nQuat_tmp[3];

      // 'errorStateEkf_function2:513' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
      i_0 = 0;
      for (i_3 = 0; i_3 < 19; i_3++) {
        i = 0;
        for (i_2 = 0; i_2 < 19; i_2++) {
          H_tmp = i_2 + i_0;
          stateEstimatorEskf_DW.covP[H_tmp] = (b_covP[i + i_3] + b_covP[H_tmp]) *
            0.5F;
          i += 19;
        }

        i_0 += 19;
      }

      //  end
    }

    // Fuse GPS data if it is valid
    // 'errorStateEkf_function2:280' if(isGpsValid && estSmMode == enumStateEstimateMode.RUN) 
    if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) &
         rtb_sensorIn_isGpsValid)) {
      // 'errorStateEkf_function2:281' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 4, idxEs, ... 
      // 'errorStateEkf_function2:282'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(1)); 
      for (i_0 = 0; i_0 < 19; i_0++) {
        tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
        tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
      }

      applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 4.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:283' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 5, idxEs, ... 
      // 'errorStateEkf_function2:284'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(2)); 
      for (i_0 = 0; i_0 < 19; i_0++) {
        tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
        tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
      }

      applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 5.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:285' if (~isBaroValid)
      if (static_cast<boolean_T>(rtb_sensorIn_isBaroValid ^ 1)) {
        // 'errorStateEkf_function2:286' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 6, idxEs, ... 
        // 'errorStateEkf_function2:287'                 idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(3)); 
        for (i_0 = 0; i_0 < 19; i_0++) {
          tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
        }

        for (i_0 = 0; i_0 < 16; i_0++) {
          tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
          tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
        }

        applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
          stateEstimatorEskf_DW.covP, 6.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
          3.68F);
      }

      // 'errorStateEkf_function2:289' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 7, idxEs, ... 
      // 'errorStateEkf_function2:290'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(4)); 
      for (i_0 = 0; i_0 < 19; i_0++) {
        tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
        tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
      }

      applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 7.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:291' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 8, idxEs, ... 
      // 'errorStateEkf_function2:292'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(5)); 
      for (i_0 = 0; i_0 < 19; i_0++) {
        tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
        tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
      }

      applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 8.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
        27.0F);

      // 'errorStateEkf_function2:293' [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, 9, idxEs, ... 
      // 'errorStateEkf_function2:294'             idxEs2, idxNs2, measNoiseR, ekfParams.nisParams.nisNedPosAndVel(6)); 
      for (i_0 = 0; i_0 < 19; i_0++) {
        tmp_0[i_0] = static_cast<real_T>(i_0) + 1.0;
      }

      for (i_0 = 0; i_0 < 16; i_0++) {
        tmp_1[i_0] = static_cast<real_T>(i_0) + 4.0;
        tmp_2[i_0] = static_cast<real_T>(i_0) + 5.0;
      }

      applyGpsPosAndVelCorr_kaQP2N0T(rty_states, nedPosAndVel,
        stateEstimatorEskf_DW.covP, 9.0, tmp_0, tmp_1, tmp_2, rtu_measNoiseR,
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

    // Fuse Baro data if it is valid
    // 'errorStateEkf_function2:318' if(isBaroValid)
    if (rtb_sensorIn_isBaroValid) {
      // 'errorStateEkf_function2:319' iS = 1/(covP(19, 19) - covP(19, 6) + covP(6, 6) - covP(6, 19) + measNoiseR(10, 10)); 
      rtb_XAxis = 1.0F / ((((stateEstimatorEskf_DW.covP[360] -
        stateEstimatorEskf_DW.covP[113]) + stateEstimatorEskf_DW.covP[100]) -
                           stateEstimatorEskf_DW.covP[347]) + rtu_measNoiseR[144]);

      // 'errorStateEkf_function2:320' nu = baroAlt_m  + states(7) - states(20); 
      rtb_Product2 = (rtb_Product2 + rty_states[6]) - rty_states[19];

      // 'errorStateEkf_function2:321' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:323' if NIS < 27
      if (rtb_Product2 * rtb_Product2 * rtb_XAxis < 27.0F) {
        // 'errorStateEkf_function2:324' ekfDebug.isBaroUsed = true;
        // 'errorStateEkf_function2:325' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
             rtb_isOfDataValidOut)) {
          // 'errorStateEkf_function2:326' K = (covP(idxEs, 19) - covP(idxEs, 6)).*iS; 
          // 'errorStateEkf_function2:327' errorStateHat(idxEs) = K*nu;
          //            covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs); 
          // 'errorStateEkf_function2:329' covP(idxEs, idxEs) = covP(idxEs, idxEs) - ... 
          // 'errorStateEkf_function2:330'                     (-K * (covP(6, idxEs) - covP(19, idxEs))); 
          i = 0;
          for (i_2 = 0; i_2 < 19; i_2++) {
            rtb_dcmBodyToNed_idx_0 = (stateEstimatorEskf_DW.covP[i_2 + 342] -
              stateEstimatorEskf_DW.covP[i_2 + 95]) * rtb_XAxis;
            errorStateHat[i_2] = rtb_dcmBodyToNed_idx_0 * rtb_Product2;
            d_K[i_2] = -rtb_dcmBodyToNed_idx_0;
            b_errorStateHat[i_2] = stateEstimatorEskf_DW.covP[i + 5] -
              stateEstimatorEskf_DW.covP[i + 18];
            i += 19;
          }

          i_0 = 0;
          for (i_3 = 0; i_3 < 19; i_3++) {
            for (i = 0; i < 19; i++) {
              H_tmp = i + i_0;
              stateEstimatorEskf_DW.covP[H_tmp] -= d_K[i] * b_errorStateHat[i_3];
            }

            i_0 += 19;
          }

          // Update the nominal state
          // 'errorStateEkf_function2:332' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (i_0 = 0; i_0 < 16; i_0++) {
            rty_states[i_0 + 4] += errorStateHat[i_0 + 3];
          }
        } else {
          // 'errorStateEkf_function2:333' else
          // 'errorStateEkf_function2:334' K = (covP(idxNoGpsEs, 19) - covP(idxNoGpsEs, 6)).*iS; 
          // 'errorStateEkf_function2:335' errorStateHat(idxNoGpsEs) = K*nu;
          //            covP(idxEs, idxEs) = covP(idxEs, idxEs) - K*H*covP(idxEs, idxEs); 
          // 'errorStateEkf_function2:337' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - ... 
          // 'errorStateEkf_function2:338'                     (-K * (covP(6, idxNoGpsEs) - covP(19, idxNoGpsEs))); 
          for (i_0 = 0; i_0 < 15; i_0++) {
            c = c_0[i_0];
            rtb_dcmBodyToNed_idx_0 = (stateEstimatorEskf_DW.covP[c + 342] -
              stateEstimatorEskf_DW.covP[c + 95]) * rtb_XAxis;
            errorStateHat[c_0[i_0]] = rtb_dcmBodyToNed_idx_0 * rtb_Product2;
            c_K[i_0] = -rtb_dcmBodyToNed_idx_0;
            i_3 = 19 * c;
            tmp_5[i_0] = stateEstimatorEskf_DW.covP[i_3 + 5] -
              stateEstimatorEskf_DW.covP[i_3 + 18];
          }

          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              tmp_4[i_3 + 15 * i_0] = stateEstimatorEskf_DW.covP[19 * c_0[i_0] +
                c_0[i_3]] - c_K[i_3] * tmp_5[i_0];
            }
          }

          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              stateEstimatorEskf_DW.covP[c_0[i_3] + 19 * c_0[i_0]] = tmp_4[15 *
                i_0 + i_3];
            }
          }

          // Update the nominal state
          // 'errorStateEkf_function2:340' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
          for (i_0 = 0; i_0 < 12; i_0++) {
            rty_states_0[i_0] = rty_states[e[i_0]] + errorStateHat[f[i_0]];
          }

          for (i_0 = 0; i_0 < 12; i_0++) {
            rty_states[e[i_0]] = rty_states_0[i_0];
          }
        }

        // Construct quaternion from the rotation vector and reset covP
        // 'errorStateEkf_function2:344' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        nQuat_tmp[0] = rty_states[0];
        nQuat_tmp[1] = rty_states[1];
        nQuat_tmp[2] = rty_states[2];
        nQuat_tmp[3] = rty_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:345' states(1:4) = nomQuat;
        rty_states[0] = nQuat_tmp[0];
        rty_states[1] = nQuat_tmp[1];
        rty_states[2] = nQuat_tmp[2];
        rty_states[3] = nQuat_tmp[3];

        // 'errorStateEkf_function2:346' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
             rtb_isOfDataValidOut)) {
          // 'errorStateEkf_function2:347' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
          i_0 = 0;
          for (i_3 = 0; i_3 < 19; i_3++) {
            i = 0;
            for (i_2 = 0; i_2 < 19; i_2++) {
              H_tmp = i_2 + i_0;
              stateEstimatorEskf_DW.covP[H_tmp] = (covP[i + i_3] + covP[H_tmp]) *
                0.5F;
              i += 19;
            }

            i_0 += 19;
          }
        } else {
          // 'errorStateEkf_function2:348' else
          // 'errorStateEkf_function2:349' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:350'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              H_tmp = 19 * c_0[i_0] + c_0[i_3];
              stateEstimatorEskf_DW.covP[H_tmp] = (covP[19 * c_0[i_3] + c_0[i_0]]
                + covP[H_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:351' states(8:9) = 0;
          rty_states[7] = 0.0F;
          rty_states[8] = 0.0F;
        }
      }
    }

    // Fuse Lidar data if it is valid
    // 'errorStateEkf_function2:358' if(isLidarValid)
    if (static_cast<boolean_T>(static_cast<boolean_T>((rtu_lidarData->range_m >=
           rtu_lidarParams->validRange_m[0]) & (rtu_lidarData->range_m <=
           rtu_lidarParams->validRange_m[1])) & static_cast<boolean_T>
         (rtu_lidarData->isLidarDataValid & rtu_lidarData->isLidarInitialized)))
    {
      // 'errorStateEkf_function2:359' iS = 1/(covP(6, 6) + measNoiseR(11, 11)); 
      rtb_XAxis = 1.0F / (stateEstimatorEskf_DW.covP[100] + rtu_measNoiseR[160]);

      // 'errorStateEkf_function2:360' nu = lidarAgl_m  + states(7);
      rtb_Product2 = rtb_Product3 + rty_states[6];

      // 'errorStateEkf_function2:361' NIS = nu*nu*iS;
      // 'errorStateEkf_function2:363' if NIS < 27
      if (rtb_Product2 * rtb_Product2 * rtb_XAxis < 27.0F) {
        // 'errorStateEkf_function2:364' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
             rtb_isOfDataValidOut)) {
          // 'errorStateEkf_function2:365' K = -covP(idxEs, 6).*iS;
          // 'errorStateEkf_function2:366' errorStateHat(idxEs) = K*nu;
          // 'errorStateEkf_function2:368' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (-K*covP(6, idxEs)); 
          for (i = 0; i < 19; i++) {
            rtb_dcmBodyToNed_idx_0 = -stateEstimatorEskf_DW.covP[i + 95] *
              rtb_XAxis;
            errorStateHat[i] = rtb_dcmBodyToNed_idx_0 * rtb_Product2;
            d_K[i] = -rtb_dcmBodyToNed_idx_0;
          }

          i_0 = 0;
          for (i_3 = 0; i_3 < 19; i_3++) {
            for (i = 0; i < 19; i++) {
              i_2 = i + i_0;
              b_covP[i_2] = stateEstimatorEskf_DW.covP[i_2] -
                stateEstimatorEskf_DW.covP[i_0 + 5] * d_K[i];
            }

            i_0 += 19;
          }

          std::memcpy(&stateEstimatorEskf_DW.covP[0], &b_covP[0], 361U * sizeof
                      (real32_T));

          // Update the nominal state
          // 'errorStateEkf_function2:371' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
          for (i_0 = 0; i_0 < 16; i_0++) {
            rty_states[i_0 + 4] += errorStateHat[i_0 + 3];
          }
        } else {
          // 'errorStateEkf_function2:372' else
          // 'errorStateEkf_function2:373' K = -covP(idxNoGpsEs, 6).*iS;
          // 'errorStateEkf_function2:374' errorStateHat(idxNoGpsEs) = K*nu;
          // 'errorStateEkf_function2:376' covP(idxNoGpsEs, idxNoGpsEs) = covP(idxNoGpsEs, idxNoGpsEs) - (-K*covP(6, idxNoGpsEs)); 
          for (i_0 = 0; i_0 < 15; i_0++) {
            rtb_dcmBodyToNed_idx_0 = -stateEstimatorEskf_DW.covP[c_0[i_0] + 95] *
              rtb_XAxis;
            errorStateHat[c_0[i_0]] = rtb_dcmBodyToNed_idx_0 * rtb_Product2;
            c_K[i_0] = -rtb_dcmBodyToNed_idx_0;
          }

          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              i = 19 * c_0[i_0];
              tmp_4[i_3 + 15 * i_0] = stateEstimatorEskf_DW.covP[i + c_0[i_3]] -
                stateEstimatorEskf_DW.covP[i + 5] * c_K[i_3];
            }
          }

          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              stateEstimatorEskf_DW.covP[c_0[i_3] + 19 * c_0[i_0]] = tmp_4[15 *
                i_0 + i_3];
            }
          }

          // Update the nominal state
          // 'errorStateEkf_function2:379' states(idxNoGpsNs2) = states(idxNoGpsNs2) + errorStateHat(idxNoGpsEs2); 
          for (i_0 = 0; i_0 < 12; i_0++) {
            rty_states_0[i_0] = rty_states[e[i_0]] + errorStateHat[f[i_0]];
          }

          for (i_0 = 0; i_0 < 12; i_0++) {
            rty_states[e[i_0]] = rty_states_0[i_0];
          }
        }

        // Construct quaternion from the rotation vector and reset covP
        // 'errorStateEkf_function2:383' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
        nQuat_tmp[0] = rty_states[0];
        nQuat_tmp[1] = rty_states[1];
        nQuat_tmp[2] = rty_states[2];
        nQuat_tmp[3] = rty_states[3];
        std::memcpy(&covP[0], &stateEstimatorEskf_DW.covP[0], 361U * sizeof
                    (real32_T));
        updateQuatAndResetCovP_KAnSUXrZ(nQuat_tmp, &errorStateHat[0], covP);
        std::memcpy(&stateEstimatorEskf_DW.covP[0], &covP[0], 361U * sizeof
                    (real32_T));

        // 'errorStateEkf_function2:384' states(1:4) = nomQuat;
        rty_states[0] = nQuat_tmp[0];
        rty_states[1] = nQuat_tmp[1];
        rty_states[2] = nQuat_tmp[2];
        rty_states[3] = nQuat_tmp[3];

        // 'errorStateEkf_function2:385' if estSmMode == enumStateEstimateMode.RUN || isOfValid 
        if (static_cast<boolean_T>((mode == enumStateEstimateMode::RUN) |
             rtb_isOfDataValidOut)) {
          // 'errorStateEkf_function2:386' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
          i_0 = 0;
          for (i_3 = 0; i_3 < 19; i_3++) {
            i = 0;
            for (i_2 = 0; i_2 < 19; i_2++) {
              H_tmp = i_2 + i_0;
              stateEstimatorEskf_DW.covP[H_tmp] = (covP[i + i_3] + covP[H_tmp]) *
                0.5F;
              i += 19;
            }

            i_0 += 19;
          }
        } else {
          // 'errorStateEkf_function2:387' else
          // 'errorStateEkf_function2:388' covP(idxNoGpsEs, idxNoGpsEs) = (covP(idxNoGpsEs, idxNoGpsEs) + ... 
          // 'errorStateEkf_function2:389'                     covP(idxNoGpsEs, idxNoGpsEs)').*0.5; 
          for (i_0 = 0; i_0 < 15; i_0++) {
            for (i_3 = 0; i_3 < 15; i_3++) {
              H_tmp = 19 * c_0[i_0] + c_0[i_3];
              stateEstimatorEskf_DW.covP[H_tmp] = (covP[19 * c_0[i_3] + c_0[i_0]]
                + covP[H_tmp]) * 0.5F;
            }
          }

          // 'errorStateEkf_function2:390' states(8:9) = 0;
          rty_states[7] = 0.0F;
          rty_states[8] = 0.0F;
        }
      }
    }

    // Compute Body To NED DCM
    // 'errorStateEkf_function2:396' dcmBodyToNed = quatToDcm_function(states(1:4)); 
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
    rtb_dcmBodyToNed_idx_4 = rty_states[3] * rty_states[3];
    rtb_Product3 = rty_states[2] * rty_states[2];
    rtb_dcmBodyToNed_idx_0 = 1.0F - (rtb_Product3 + rtb_dcmBodyToNed_idx_4) *
      2.0F;
    rtb_dcmBodyToNed_idx_1 = rty_states[1] * rty_states[2];
    rtb_dcmBodyToNed_idx_7 = rty_states[0] * rty_states[3];
    rtb_dcmBodyToNed_idx_3 = (rtb_dcmBodyToNed_idx_1 - rtb_dcmBodyToNed_idx_7) *
      2.0F;
    rtb_Product2 = rty_states[1] * rty_states[3];
    rtb_XAxis = rty_states[0] * rty_states[2];
    rtb_dcmBodyToNed_idx_6 = (rtb_Product2 + rtb_XAxis) * 2.0F;
    rtb_dcmBodyToNed_idx_1 = (rtb_dcmBodyToNed_idx_1 + rtb_dcmBodyToNed_idx_7) *
      2.0F;
    rtb_XAxis1 = rty_states[1] * rty_states[1];
    rtb_dcmBodyToNed_idx_4 = 1.0F - (rtb_XAxis1 + rtb_dcmBodyToNed_idx_4) * 2.0F;
    rtb_UnitDelay_f = rty_states[2] * rty_states[3];
    rtb_UnitDelay_d = rty_states[0] * rty_states[1];
    rtb_dcmBodyToNed_idx_7 = (rtb_UnitDelay_f - rtb_UnitDelay_d) * 2.0F;
    rtb_dcmBodyToNed_idx_2 = (rtb_Product2 - rtb_XAxis) * 2.0F;
    rtb_dcmBodyToNed_idx_5 = (rtb_UnitDelay_f + rtb_UnitDelay_d) * 2.0F;
    rtb_dcmBodyToNed_idx_8 = 1.0F - (rtb_XAxis1 + rtb_Product3) * 2.0F;

    //  else
    //      states = prevStates;
    //  end
  }

  // Sqrt: '<S19>/sqrt' incorporates:
  //   Product: '<S20>/Product'
  //   Product: '<S20>/Product1'
  //   Product: '<S20>/Product2'
  //   Product: '<S20>/Product3'
  //   Sum: '<S20>/Sum'

  rtb_Product3 = std::sqrt(((rty_states[0] * rty_states[0] + rty_states[1] *
    rty_states[1]) + rty_states[2] * rty_states[2]) + rty_states[3] *
    rty_states[3]);

  // Product: '<S14>/Product'
  rtb_XAxis = rty_states[0] / rtb_Product3;

  // Product: '<S14>/Product1'
  rtb_XAxis1 = rty_states[1] / rtb_Product3;

  // Product: '<S14>/Product2'
  rtb_Product2 = rty_states[2] / rtb_Product3;

  // Product: '<S14>/Product3'
  rtb_Product3 = rty_states[3] / rtb_Product3;

  // Fcn: '<S2>/fcn2' incorporates:
  //   Fcn: '<S2>/fcn5'

  rtb_UnitDelay_d = rtb_XAxis * rtb_XAxis;
  rtb_UnitDelay1_idx_0 = rtb_XAxis1 * rtb_XAxis1;
  rtb_UnitDelay1_idx_1 = rtb_Product2 * rtb_Product2;
  rtb_UnitDelay1_idx_2 = rtb_Product3 * rtb_Product3;

  // Trigonometry: '<S13>/Trigonometric Function1' incorporates:
  //   Fcn: '<S2>/fcn1'
  //   Fcn: '<S2>/fcn2'

  rtb_UnitDelay_f = std::atan2((rtb_XAxis1 * rtb_Product2 + rtb_XAxis *
    rtb_Product3) * 2.0F, ((rtb_UnitDelay_d + rtb_UnitDelay1_idx_0) -
    rtb_UnitDelay1_idx_1) - rtb_UnitDelay1_idx_2);

  // Trigonometry: '<S13>/Trigonometric Function3' incorporates:
  //   Fcn: '<S2>/fcn4'
  //   Fcn: '<S2>/fcn5'

  rtb_UnitDelay_d = std::atan2((rtb_Product2 * rtb_Product3 + rtb_XAxis *
    rtb_XAxis1) * 2.0F, ((rtb_UnitDelay_d - rtb_UnitDelay1_idx_0) -
    rtb_UnitDelay1_idx_1) + rtb_UnitDelay1_idx_2);

  // Fcn: '<S2>/fcn3'
  rtb_Product3 = (rtb_XAxis1 * rtb_Product3 - rtb_XAxis * rtb_Product2) * -2.0F;

  // If: '<S15>/If' incorporates:
  //   Constant: '<S16>/Constant'
  //   Constant: '<S17>/Constant'

  if (rtb_Product3 > 1.0F) {
    // Outputs for IfAction SubSystem: '<S15>/If Action Subsystem' incorporates:
    //   ActionPort: '<S16>/Action Port'

    rtb_Product3 = 1.0F;

    // End of Outputs for SubSystem: '<S15>/If Action Subsystem'
  } else if (rtb_Product3 < -1.0F) {
    // Outputs for IfAction SubSystem: '<S15>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S17>/Action Port'

    rtb_Product3 = 1.0F;

    // End of Outputs for SubSystem: '<S15>/If Action Subsystem1'
  }

  // End of If: '<S15>/If'

  // Trigonometry: '<S13>/trigFcn'
  rtb_Product3 = std::asin(rtb_Product3);

  // MATLAB Function: '<Root>/eulToDcm' incorporates:
  //   Gain: '<Root>/ZeroOutRollAndPitch'

  //  precalculate trignometric values
  // MATLAB Function 'eulToDcm': '<S6>:1'
  // '<S6>:1:4' s_phi = sin(eul_rad(1));
  // '<S6>:1:5' c_phi = cos(eul_rad(1));
  // '<S6>:1:7' s_theta = sin(eul_rad(2));
  // '<S6>:1:8' c_theta = cos(eul_rad(2));
  // '<S6>:1:10' s_psi = sin(eul_rad(3));
  rtb_Product2 = std::sin(rtb_UnitDelay_f);

  // '<S6>:1:11' c_psi = cos(eul_rad(3));
  rtb_XAxis = std::cos(rtb_UnitDelay_f);

  // '<S6>:1:13' dcmFromNed = [c_psi*c_theta, c_theta*s_psi, -s_theta;
  // '<S6>:1:14'     c_psi*s_phi*s_theta - c_phi*s_psi, c_phi*c_psi + s_phi*s_psi*s_theta, c_theta*s_phi; 
  // '<S6>:1:15'     s_phi*s_psi + c_phi*c_psi*s_theta, c_phi*s_psi*s_theta - c_psi*s_phi, c_phi*c_theta]; 
  rty_dcmNedToFep[0] = rtb_XAxis;
  rty_dcmNedToFep[3] = rtb_Product2;
  rty_dcmNedToFep[6] = -0.0F;
  rty_dcmNedToFep[1] = 0.0F - rtb_Product2;
  rty_dcmNedToFep[4] = rtb_XAxis;
  rty_dcmNedToFep[7] = 0.0F;
  rty_dcmNedToFep[2] = 0.0F;
  rty_dcmNedToFep[5] = 0.0F;
  rty_dcmNedToFep[8] = 1.0F;

  // SignalConversion generated from: '<Root>/eulAng_rad'
  rty_eulAng_rad[0] = rtb_UnitDelay_d;
  rty_eulAng_rad[1] = rtb_Product3;
  rty_eulAng_rad[2] = rtb_UnitDelay_f;

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[0] = rtb_dcmBodyToNed_idx_0;
  rty_dcmNedToBody[1] = rtb_dcmBodyToNed_idx_3;
  rty_dcmNedToBody[2] = rtb_dcmBodyToNed_idx_6;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[0] = Product[0] - rty_states[14];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[3] = rtb_dcmBodyToNed_idx_1;
  rty_dcmNedToBody[4] = rtb_dcmBodyToNed_idx_4;
  rty_dcmNedToBody[5] = rtb_dcmBodyToNed_idx_7;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[1] = Product[1] - rty_states[15];

  // Math: '<Root>/Transpose'
  rty_dcmNedToBody[6] = rtb_dcmBodyToNed_idx_2;
  rty_dcmNedToBody[7] = rtb_dcmBodyToNed_idx_5;
  rty_dcmNedToBody[8] = rtb_dcmBodyToNed_idx_8;

  // Sum: '<Root>/Sum' incorporates:
  //   Product: '<S4>/Product'

  rty_bodyAccels_mps2[2] = Product[2] - rty_states[16];

  // BusCreator: '<Root>/Bus Creator'
  rty_stateEstimatorDebug->stateEstInitPct =
    stateEstimatorEskf_DW.stateEstInitPct;
  rty_stateEstimatorDebug->smMode = mode;

  // Update for DiscreteTransferFcn: '<S22>/X Axis'
  stateEstimatorEskf_DW.XAxis_states[1] = stateEstimatorEskf_DW.XAxis_states[0];
  stateEstimatorEskf_DW.XAxis_states[0] = stateEstimatorEskf_DW.XAxis_tmp;

  // Update for DiscreteTransferFcn: '<S22>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states[1] = stateEstimatorEskf_DW.XAxis1_states[0];
  stateEstimatorEskf_DW.XAxis1_states[0] = stateEstimatorEskf_DW.XAxis1_tmp;

  // Update for DiscreteTransferFcn: '<S22>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states[1] = stateEstimatorEskf_DW.XAxis2_states[0];
  stateEstimatorEskf_DW.XAxis2_states[0] = stateEstimatorEskf_DW.XAxis2_tmp;

  // Update for DiscreteTransferFcn: '<S23>/X Axis'
  stateEstimatorEskf_DW.XAxis_states_e[1] =
    stateEstimatorEskf_DW.XAxis_states_e[0];
  stateEstimatorEskf_DW.XAxis_states_e[0] = stateEstimatorEskf_DW.XAxis_tmp_o;

  // Update for DiscreteTransferFcn: '<S23>/X Axis1'
  stateEstimatorEskf_DW.XAxis1_states_a[1] =
    stateEstimatorEskf_DW.XAxis1_states_a[0];
  stateEstimatorEskf_DW.XAxis1_states_a[0] = stateEstimatorEskf_DW.XAxis1_tmp_l;

  // Update for DiscreteTransferFcn: '<S23>/X Axis2'
  stateEstimatorEskf_DW.XAxis2_states_j[1] =
    stateEstimatorEskf_DW.XAxis2_states_j[0];
  stateEstimatorEskf_DW.XAxis2_states_j[0] = stateEstimatorEskf_DW.XAxis2_tmp_o;

  // Update for Delay: '<S1>/Delay'
  stateEstimatorEskf_DW.icLoad = false;

  // Update for UnitDelay: '<Root>/Unit Delay'
  std::memcpy(&stateEstimatorEskf_DW.UnitDelay_DSTATE[0], &rty_states[0], 20U *
              sizeof(real32_T));

  // Update for Delay: '<S1>/Delay' incorporates:
  //   UnitDelay: '<Root>/Unit Delay'

  std::memcpy(&stateEstimatorEskf_DW.Delay_DSTATE[0], &rty_states[0], 20U *
              sizeof(real32_T));

  // Update for UnitDelay: '<Root>/Unit Delay1'
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[0] = rtb_UnitDelay_d;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[1] = rtb_Product3;
  stateEstimatorEskf_DW.UnitDelay1_DSTATE[2] = rtb_UnitDelay_f;

  // Update for DiscreteTransferFcn: '<S32>/Discrete Transfer Fcn'
  stateEstimatorEskf_DW.DiscreteTransferFcn_states =
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp;

  // Update for DiscreteTransferFcn: '<S31>/Discrete Transfer Fcn'
  stateEstimatorEskf_DW.DiscreteTransferFcn_states_n =
    stateEstimatorEskf_DW.DiscreteTransferFcn_tmp_b;

  // Switch: '<S7>/Switch' incorporates:
  //   RelationalOperator: '<S25>/FixPt Relational Operator'
  //   UnitDelay: '<S25>/Delay Input1'
  //
  //  Block description for '<S25>/Delay Input1':
  //
  //   Store in Global RAM

  if (static_cast<int32_T>(rtb_Compare) > static_cast<int32_T>
      (stateEstimatorEskf_DW.DelayInput1_DSTATE)) {
    // Update for UnitDelay: '<S7>/Unit Delay'
    stateEstimatorEskf_DW.UnitDelay_DSTATE_j = rtb_UnitDelay[6];
  } else {
    // Update for UnitDelay: '<S7>/Unit Delay'
    stateEstimatorEskf_DW.UnitDelay_DSTATE_j = rtb_UnitDelay_g;
  }

  // End of Switch: '<S7>/Switch'

  // Update for Delay: '<S1>/Delay2'
  stateEstimatorEskf_DW.icLoad_g = false;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[0] = rtb_dcmBodyToNed_idx_0;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[0] = rtb_dcmBodyToNed_idx_0;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[1] = rtb_dcmBodyToNed_idx_1;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[1] = rtb_dcmBodyToNed_idx_1;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[2] = rtb_dcmBodyToNed_idx_2;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[3] = rtb_dcmBodyToNed_idx_3;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[3] = rtb_dcmBodyToNed_idx_3;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[4] = rtb_dcmBodyToNed_idx_4;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[4] = rtb_dcmBodyToNed_idx_4;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[5] = rtb_dcmBodyToNed_idx_5;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[6] = rtb_dcmBodyToNed_idx_6;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[6] = rtb_dcmBodyToNed_idx_6;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[7] = rtb_dcmBodyToNed_idx_7;

  // Update for UnitDelay: '<Root>/Unit Delay2'
  stateEstimatorEskf_DW.UnitDelay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for Delay: '<S1>/Delay2' incorporates:
  //   UnitDelay: '<Root>/Unit Delay2'

  stateEstimatorEskf_DW.Delay2_DSTATE[8] = rtb_dcmBodyToNed_idx_8;

  // Update for UnitDelay: '<S25>/Delay Input1'
  //
  //  Block description for '<S25>/Delay Input1':
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
stateEstimatorEskf::~stateEstimatorEskf()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
