//
// File: stateEstimatorEskf.h
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
#ifndef RTW_HEADER_stateEstimatorEskf_h_
#define RTW_HEADER_stateEstimatorEskf_h_
#include "rtwtypes.h"
#include "stateEstimatorEskf_types.h"
#include <cstring>

// Class declaration for model stateEstimatorEskf
class stateEstimatorEskf final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'stateEstimatorEskf'
  struct DW_stateEstimatorEskf_T {
    busSensorIn sensorDataOut;         // '<Root>/estimatorStateMachine'
    real_T refLatLonAlt[3];            // '<Root>/estimatorStateMachine'
    real_T gpsIdx;                     // '<Root>/estimatorStateMachine'
    real32_T TmpSignalConversionAtSFunctionI[3];// '<Root>/estimatorStateMachine' 
    real32_T initialStates[20];        // '<Root>/estimatorStateMachine'
    real32_T initialDcmBodyToNed[9];   // '<Root>/estimatorStateMachine'
    real32_T UnitDelay_DSTATE[20];     // '<Root>/Unit Delay'
    real32_T XAxis_states[2];          // '<S22>/X Axis'
    real32_T XAxis1_states[2];         // '<S22>/X Axis1'
    real32_T XAxis2_states[2];         // '<S22>/X Axis2'
    real32_T XAxis_states_e[2];        // '<S23>/X Axis'
    real32_T XAxis1_states_a[2];       // '<S23>/X Axis1'
    real32_T XAxis2_states_j[2];       // '<S23>/X Axis2'
    real32_T Delay_DSTATE[20];         // '<S1>/Delay'
    real32_T UnitDelay2_DSTATE[9];     // '<Root>/Unit Delay2'
    real32_T UnitDelay1_DSTATE[3];     // '<Root>/Unit Delay1'
    real32_T Delay2_DSTATE[9];         // '<S1>/Delay2'
    real32_T gyroBias_radps[3];        // '<Root>/estimatorStateMachine'
    real32_T initialQuat[4];           // '<Root>/estimatorStateMachine'
    real32_T imuM2[6];                 // '<Root>/estimatorStateMachine'
    real32_T imuMean[6];               // '<Root>/estimatorStateMachine'
    real32_T magM2[3];                 // '<Root>/estimatorStateMachine'
    real32_T magMean[3];               // '<Root>/estimatorStateMachine'
    real32_T accelBias_mps2[3];        // '<Root>/estimatorStateMachine'
    real32_T magBias_nd[3];            // '<Root>/estimatorStateMachine'
    real32_T covP[361];                // '<S1>/EKF'
    real32_T I3[9];                    // '<S1>/EKF'
    real32_T xErrorJac[380];           // '<S1>/EKF'
    real32_T stateEstInitPct;          // '<Root>/estimatorStateMachine'
    real32_T DiscreteTransferFcn_states;// '<S32>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_states_n;// '<S31>/Discrete Transfer Fcn'
    real32_T UnitDelay_DSTATE_j;       // '<S7>/Unit Delay'
    real32_T XAxis_tmp;                // '<S22>/X Axis'
    real32_T XAxis1_tmp;               // '<S22>/X Axis1'
    real32_T XAxis2_tmp;               // '<S22>/X Axis2'
    real32_T XAxis_tmp_o;              // '<S23>/X Axis'
    real32_T XAxis1_tmp_l;             // '<S23>/X Axis1'
    real32_T XAxis2_tmp_o;             // '<S23>/X Axis2'
    real32_T DiscreteTransferFcn_tmp;  // '<S32>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_tmp_b;// '<S31>/Discrete Transfer Fcn'
    real32_T imuIdx;                   // '<Root>/estimatorStateMachine'
    real32_T magIdx;                   // '<Root>/estimatorStateMachine'
    real32_T baroIdx;                  // '<Root>/estimatorStateMachine'
    real32_T baroBias_m;               // '<Root>/estimatorStateMachine'
    real32_T baroInitAltM2;            // '<Root>/estimatorStateMachine'
    real32_T baroInitAltMean;          // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1;         // '<S10>/Chart'
    int32_T durationCounter_1_j;       // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1_f;       // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1_fx;      // '<Root>/estimatorStateMachine'
    uint16_T gpsValidCount;            // '<Root>/estimatorStateMachine'
    uint8_T is_active_c5_stateEstimatorEskf;// '<S10>/Chart'
    uint8_T is_c5_stateEstimatorEskf;  // '<S10>/Chart'
    uint8_T is_active_c3_stateEstimatorEskf;// '<Root>/estimatorStateMachine'
    uint8_T is_c3_stateEstimatorEskf;  // '<Root>/estimatorStateMachine'
    boolean_T resetStates;             // '<Root>/estimatorStateMachine'
    boolean_T DelayInput1_DSTATE;      // '<S25>/Delay Input1'
    boolean_T icLoad;                  // '<S1>/Delay'
    boolean_T icLoad_g;                // '<S1>/Delay2'
    boolean_T isAttInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T isBaroInitialized;       // '<Root>/estimatorStateMachine'
    boolean_T isPosInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T covP_not_empty;          // '<S1>/EKF'
  };

  // Initial conditions function
  void init(busStateEstimatorDebug *rty_stateEstimatorDebug);

  // Copy Constructor
  stateEstimatorEskf(stateEstimatorEskf const&) = delete;

  // Assignment Operator
  stateEstimatorEskf& operator= (stateEstimatorEskf const&) & = delete;

  // Move Constructor
  stateEstimatorEskf(stateEstimatorEskf &&) = delete;

  // Move Assignment Operator
  stateEstimatorEskf& operator= (stateEstimatorEskf &&) = delete;

  // model step function
  void step(const busImuData *rtu_imuData, const busMagData *rtu_magData, const
            busGpsData *rtu_gpsData, const busBaroData *rtu_baroData, const
            busLidarData *rtu_lidarData, const busMtf01pData *rtu_mtf01pData,
            const busImuNtchFiltParams *rtu_imuNotchFiltParams, const
            busAccelParams *rtu_accelParams, const busMagParams *rtu_magParams,
            const busLidarParams *rtu_lidarParams, const busMtf01pParams
            *rtu_mtf01pParams, const busStateEstSmParams *rtu_stateEstSmParams,
            const real32_T rtu_processNoiseQ[361], const real32_T
            rtu_measNoiseR[225], const real32_T rtu_initCovP[361], const
            real32_T *rtu_gEarth_mps2, real32_T rty_states[20], real32_T
            rty_eulAng_rad[3], real32_T rty_dcmNedToBody[9], real32_T
            rty_dcmNedToFep[9], real32_T rty_bodyAccels_mps2[3],
            busStateEstimatorDebug *rty_stateEstimatorDebug);

  // Constructor
  stateEstimatorEskf();

  // Destructor
  ~stateEstimatorEskf();

  // private data and function members
 private:
  // Block states
  DW_stateEstimatorEskf_T stateEstimatorEskf_DW;

  // private member function(s) for subsystem '<S31>/Compute Filter Numerator And Denominator'
  static void ComputeFilterNumeratorAndDenomi(const real32_T
    *rtu_filterBandwidth_radps, real32_T rty_num[2], real32_T rty_den[2], real_T
    rtp_sampleTime_s);

  // private member function(s) for subsystem '<Root>/TmpModelReferenceSubsystem'
  void stateEstimatorEskf_INITIALIZE(enumStateEstimateMode *mode, real_T
    latLonAltOut[3], const real32_T *Divide1, const real32_T Product[3], const
    real32_T Divide[3], const busMagData *rtu_magData, const busGpsData
    *rtu_gpsData, const busBaroData *rtu_baroData, const busStateEstSmParams
    *rtu_stateEstSmParams);
  void state_enter_atomic_RUN_INIT_GPS(enumStateEstimateMode *mode, real_T
    latLonAltOut[3], const real32_T *Divide1, const real32_T Product[3], const
    real32_T Divide[3], const busMagData *rtu_magData, const busGpsData
    *rtu_gpsData, const busBaroData *rtu_baroData);
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Gain' : Unused code path elimination
//  Block '<S4>/Product1' : Unused code path elimination
//  Block '<S10>/Gain' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'stateEstimatorEskf'
//  '<S1>'   : 'stateEstimatorEskf/EKF'
//  '<S2>'   : 'stateEstimatorEskf/Quaternions to Rotation Angles'
//  '<S3>'   : 'stateEstimatorEskf/Subsystem Reference'
//  '<S4>'   : 'stateEstimatorEskf/accelCorrection'
//  '<S5>'   : 'stateEstimatorEskf/estimatorStateMachine'
//  '<S6>'   : 'stateEstimatorEskf/eulToDcm'
//  '<S7>'   : 'stateEstimatorEskf/latLonAltToNedPos'
//  '<S8>'   : 'stateEstimatorEskf/lidarRangeToAgl'
//  '<S9>'   : 'stateEstimatorEskf/magCorrection'
//  '<S10>'  : 'stateEstimatorEskf/opticalFlowToNeVel'
//  '<S11>'  : 'stateEstimatorEskf/pressureToAlt'
//  '<S12>'  : 'stateEstimatorEskf/EKF/EKF'
//  '<S13>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation'
//  '<S14>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize'
//  '<S15>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input'
//  '<S16>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem'
//  '<S17>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem1'
//  '<S18>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem2'
//  '<S19>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus'
//  '<S20>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S21>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters'
//  '<S22>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Accel Notch Filters'
//  '<S23>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Gyro Notch Filters'
//  '<S24>'  : 'stateEstimatorEskf/latLonAltToNedPos/Compare To Constant'
//  '<S25>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive'
//  '<S26>'  : 'stateEstimatorEskf/latLonAltToNedPos/convertLlhToNedPos'
//  '<S27>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive/Positive'
//  '<S28>'  : 'stateEstimatorEskf/opticalFlowToNeVel/Chart'
//  '<S29>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf'
//  '<S30>'  : 'stateEstimatorEskf/opticalFlowToNeVel/getMtf01pValidity'
//  '<S31>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter'
//  '<S32>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1'
//  '<S33>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter/Compute Filter Numerator And Denominator'
//  '<S34>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1/Compute Filter Numerator And Denominator'


//-
//  Requirements for '<Root>': stateEstimatorEskf

#endif                                 // RTW_HEADER_stateEstimatorEskf_h_

//
// File trailer for generated code.
//
// [EOF]
//
