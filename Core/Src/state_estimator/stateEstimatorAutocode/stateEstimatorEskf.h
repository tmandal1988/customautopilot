//
// File: stateEstimatorEskf.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.48
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu May  1 12:28:28 2025
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
    real_T latLonAltOut[3];            // '<Root>/estimatorStateMachine'
    real_T refLatLonAlt[3];            // '<Root>/estimatorStateMachine'
    real_T gpsIdx;                     // '<Root>/estimatorStateMachine'
    real32_T Product[3];               // '<S4>/Product'
    real32_T Divide[3];                // '<S9>/Divide'
    real32_T TmpSignalConversionAtSFunctionI[3];// '<Root>/estimatorStateMachine' 
    real32_T initialStates[20];        // '<Root>/estimatorStateMachine'
    real32_T initialDcmBodyToNed[9];   // '<Root>/estimatorStateMachine'
    real32_T bodyRatesOut_radps[3];    // '<Root>/estimatorStateMachine'
    real32_T normMagVecOut_nd[3];      // '<Root>/estimatorStateMachine'
    real32_T UnitDelay_DSTATE[20];     // '<Root>/Unit Delay'
    real32_T XAxis_states[2];          // '<S21>/X Axis'
    real32_T XAxis1_states[2];         // '<S21>/X Axis1'
    real32_T XAxis2_states[2];         // '<S21>/X Axis2'
    real32_T XAxis_states_e[2];        // '<S22>/X Axis'
    real32_T XAxis1_states_a[2];       // '<S22>/X Axis1'
    real32_T XAxis2_states_j[2];       // '<S22>/X Axis2'
    real32_T Delay_DSTATE[20];         // '<S1>/Delay'
    real32_T Delay2_DSTATE[9];         // '<S1>/Delay2'
    real32_T UnitDelay1_DSTATE[3];     // '<Root>/Unit Delay1'
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
    real32_T Divide1;                  // '<S10>/Divide1'
    real32_T stateEstInitPct;          // '<Root>/estimatorStateMachine'
    real32_T UnitDelay_DSTATE_j;       // '<S7>/Unit Delay'
    real32_T XAxis_tmp;                // '<S21>/X Axis'
    real32_T XAxis1_tmp;               // '<S21>/X Axis1'
    real32_T XAxis2_tmp;               // '<S21>/X Axis2'
    real32_T XAxis_tmp_o;              // '<S22>/X Axis'
    real32_T XAxis1_tmp_l;             // '<S22>/X Axis1'
    real32_T XAxis2_tmp_o;             // '<S22>/X Axis2'
    real32_T imuIdx;                   // '<Root>/estimatorStateMachine'
    real32_T magIdx;                   // '<Root>/estimatorStateMachine'
    real32_T baroIdx;                  // '<Root>/estimatorStateMachine'
    real32_T baroBias_m;               // '<Root>/estimatorStateMachine'
    real32_T baroInitAltM2;            // '<Root>/estimatorStateMachine'
    real32_T baroInitAltMean;          // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1;         // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1_f;       // '<Root>/estimatorStateMachine'
    int32_T durationCounter_1_fx;      // '<Root>/estimatorStateMachine'
    uint16_T gpsValidCount;            // '<Root>/estimatorStateMachine'
    uint8_T is_active_c3_stateEstimatorEskf;// '<Root>/estimatorStateMachine'
    uint8_T is_c3_stateEstimatorEskf;  // '<Root>/estimatorStateMachine'
    boolean_T resetStates;             // '<Root>/estimatorStateMachine'
    boolean_T DelayInput1_DSTATE;      // '<S24>/Delay Input1'
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
            busLidarData *rtu_lidarData, const busImuNtchFiltParams
            *rtu_imuNotchFiltParams, const busAccelParams *rtu_accelParams,
            const busMagParams *rtu_magParams, const busLidarParams
            *rtu_lidarParams, const busStateEstSmParams *rtu_stateEstSmParams,
            const real32_T rtu_processNoiseQ[361], const real32_T
            rtu_measNoiseR[196], const real32_T rtu_initCovP[361], const
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

  // private member function(s) for subsystem '<Root>/TmpModelReferenceSubsystem'
  void stateEstimatorEskf_INITIALIZE(enumStateEstimateMode *mode, boolean_T
    *isMagValid, boolean_T *isGpsValid, real32_T *baroAltOut_m, boolean_T
    *isBaroValid, real32_T bodyAccelsOut_mps2[3], const busMagData *rtu_magData,
    const busGpsData *rtu_gpsData, const busBaroData *rtu_baroData, const
    busStateEstSmParams *rtu_stateEstSmParams);
  void state_enter_atomic_RUN_INIT_GPS(enumStateEstimateMode *mode, boolean_T
    *isMagValid, boolean_T *isGpsValid, real32_T *baroAltOut_m, boolean_T
    *isBaroValid, real32_T bodyAccelsOut_mps2[3], const busMagData *rtu_magData,
    const busGpsData *rtu_gpsData, const busBaroData *rtu_baroData);
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Gain' : Unused code path elimination
//  Block '<S4>/Product1' : Unused code path elimination


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
//  '<S10>'  : 'stateEstimatorEskf/pressureToAlt'
//  '<S11>'  : 'stateEstimatorEskf/EKF/EKF'
//  '<S12>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation'
//  '<S13>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize'
//  '<S14>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input'
//  '<S15>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem'
//  '<S16>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem1'
//  '<S17>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Angle Calculation/Protect asincos input/If Action Subsystem2'
//  '<S18>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus'
//  '<S19>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S20>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters'
//  '<S21>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Accel Notch Filters'
//  '<S22>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Gyro Notch Filters'
//  '<S23>'  : 'stateEstimatorEskf/latLonAltToNedPos/Compare To Constant'
//  '<S24>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive'
//  '<S25>'  : 'stateEstimatorEskf/latLonAltToNedPos/convertLlhToNedPos'
//  '<S26>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive/Positive'


//-
//  Requirements for '<Root>': stateEstimatorEskf

#endif                                 // RTW_HEADER_stateEstimatorEskf_h_

//
// File trailer for generated code.
//
// [EOF]
//
