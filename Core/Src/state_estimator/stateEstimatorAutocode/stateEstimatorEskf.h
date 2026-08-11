//
// File: stateEstimatorEskf.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.7
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Mon Aug 10 17:43:41 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#ifndef stateEstimatorEskf_h_
#define stateEstimatorEskf_h_
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
    real32_T XAxis_states[2];          // '<S56>/X Axis'
    real32_T XAxis1_states[2];         // '<S56>/X Axis1'
    real32_T XAxis2_states[2];         // '<S56>/X Axis2'
    real32_T XAxis_states_e[2];        // '<S57>/X Axis'
    real32_T XAxis1_states_a[2];       // '<S57>/X Axis1'
    real32_T XAxis2_states_j[2];       // '<S57>/X Axis2'
    real32_T UnitDelay1_DSTATE[3];     // '<Root>/Unit Delay1'
    real32_T UnitDelay2_DSTATE[9];     // '<Root>/Unit Delay2'
    real32_T UnitDelay_DSTATE_e[20];   // '<Root>/Unit Delay'
    real32_T Delay_DSTATE[20];         // '<S1>/Delay'
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
    real32_T UnitDelay_DSTATE;         // '<S7>/Unit Delay'
    real32_T DiscreteTransferFcn_states;// '<S67>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_states_n;// '<S66>/Discrete Transfer Fcn'
    real32_T XAxis_tmp;                // '<S56>/X Axis'
    real32_T XAxis1_tmp;               // '<S56>/X Axis1'
    real32_T XAxis2_tmp;               // '<S56>/X Axis2'
    real32_T XAxis_tmp_o;              // '<S57>/X Axis'
    real32_T XAxis1_tmp_l;             // '<S57>/X Axis1'
    real32_T XAxis2_tmp_o;             // '<S57>/X Axis2'
    real32_T DiscreteTransferFcn_tmp;  // '<S67>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_tmp_b;// '<S66>/Discrete Transfer Fcn'
    real32_T imuIdx;                   // '<Root>/estimatorStateMachine'
    real32_T magIdx;                   // '<Root>/estimatorStateMachine'
    real32_T baroIdx;                  // '<Root>/estimatorStateMachine'
    real32_T baroBias_m;               // '<Root>/estimatorStateMachine'
    real32_T baroInitAltM2;            // '<Root>/estimatorStateMachine'
    real32_T baroInitAltMean;          // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1;        // '<S10>/Chart'
    uint32_T durationCounter_1_p;      // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_n;      // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_g;      // '<Root>/estimatorStateMachine'
    uint16_T gpsValidCount;            // '<Root>/estimatorStateMachine'
    uint8_T is_active_c5_stateEstimatorEskf;// '<S10>/Chart'
    uint8_T is_c5_stateEstimatorEskf;  // '<S10>/Chart'
    uint8_T is_active_c3_stateEstimatorEskf;// '<Root>/estimatorStateMachine'
    uint8_T is_c3_stateEstimatorEskf;  // '<Root>/estimatorStateMachine'
    boolean_T resetStates;             // '<Root>/estimatorStateMachine'
    boolean_T DelayInput1_DSTATE;      // '<S59>/Delay Input1'
    boolean_T icLoad;                  // '<S1>/Delay'
    boolean_T icLoad_g;                // '<S1>/Delay2'
    boolean_T isAttInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T isBaroInitialized;       // '<Root>/estimatorStateMachine'
    boolean_T isPosInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T covP_not_empty;          // '<S1>/EKF'
  };

  // Constant parameters (default storage)
  struct ConstP_stateEstimatorEskf_T {
    // Expression: ekfParams
    //  Referenced by: '<S1>/EKF'

    struct_lNBPdDZBS05BfQyyvsjhxE EKF_ekfParams;
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
  void stateEs_errorStateEkf_function2(const real32_T sensorIn_bodyAccels_mps2[3],
    const real32_T sensorIn_bodyRates_radps[3], real32_T sensorIn_dtImuTime_s,
    boolean_T sensorIn_isImuDataValid, const real32_T sensorIn_normMagVec_nd[3],
    const real32_T sensorIn_localNedUnitMag_nd[3], boolean_T sensorIn_isMagValid,
    const real32_T sensorIn_nedPosAndVel_pos_m[3], const real32_T
    sensorIn_nedPosAndVel_vel_mps[3], boolean_T sensorIn_isGpsValid, real32_T
    sensorIn_baroAlt_m, boolean_T sensorIn_isBaroValid, real32_T
    sensorIn_lidarAgl_m, boolean_T sensorIn_isLidarValid, const real32_T
    sensorIn_ofNeVel_mps[2], boolean_T sensorIn_isOfValid, const real32_T
    prevStates[20], real32_T covP[361], real32_T dcmBodyToNed[9],
    enumStateEstimateMode estSmMode, const real32_T processNoiseQ[361], const
    real32_T measNoiseR[225], real32_T gEarth_mps2, const
    struct_lNBPdDZBS05BfQyyvsjhxE b_ekfParams, real32_T states[20],
    busEkfDebugData *ekfDebug);
};

// Constant parameters (default storage)
extern const stateEstimatorEskf::ConstP_stateEstimatorEskf_T
  stateEstimatorEskf_ConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<Root>/Gain' : Unused code path elimination
//  Block '<S4>/Product1' : Unused code path elimination
//  Block '<S10>/Gain' : Unused code path elimination
//  Block '<S17>/Reshape' : Reshape block reduction
//  Block '<S17>/Reshape1' : Reshape block reduction
//  Block '<S17>/Reshape2' : Reshape block reduction
//  Block '<S31>/Reshape' : Reshape block reduction
//  Block '<S40>/Reshape' : Reshape block reduction
//  Block '<S51>/Reshape (9) to [3x3] column-major' : Reshape block reduction


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
//  '<S13>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang'
//  '<S14>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM'
//  '<S15>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault'
//  '<S16>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3'
//  '<S17>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Get DCM Values'
//  '<S18>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM'
//  '<S19>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input'
//  '<S20>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem'
//  '<S21>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem1'
//  '<S22>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem2'
//  '<S23>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input'
//  '<S24>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem'
//  '<S25>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem1'
//  '<S26>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem2'
//  '<S27>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error'
//  '<S28>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal'
//  '<S29>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else No Action'
//  '<S30>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper'
//  '<S31>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal'
//  '<S32>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper'
//  '<S33>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Error'
//  '<S34>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/None'
//  '<S35>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Warning'
//  '<S36>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Error'
//  '<S37>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/None'
//  '<S38>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Warning'
//  '<S39>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal/transpose*dcm ~= eye(3)'
//  '<S40>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/Determinant of 3x3 Matrix'
//  '<S41>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/determinant does not equal 1'
//  '<S42>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A11'
//  '<S43>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A12'
//  '<S44>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A13'
//  '<S45>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A21'
//  '<S46>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A22'
//  '<S47>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A23'
//  '<S48>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A31'
//  '<S49>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A32'
//  '<S50>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A33'
//  '<S51>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Create 3x3 Matrix'
//  '<S52>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize'
//  '<S53>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus'
//  '<S54>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S55>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters'
//  '<S56>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Accel Notch Filters'
//  '<S57>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Gyro Notch Filters'
//  '<S58>'  : 'stateEstimatorEskf/latLonAltToNedPos/Compare To Constant'
//  '<S59>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive'
//  '<S60>'  : 'stateEstimatorEskf/latLonAltToNedPos/convertLlhToNedPos'
//  '<S61>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive/Positive'
//  '<S62>'  : 'stateEstimatorEskf/opticalFlowToNeVel/Chart'
//  '<S63>'  : 'stateEstimatorEskf/opticalFlowToNeVel/Cross Product'
//  '<S64>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf'
//  '<S65>'  : 'stateEstimatorEskf/opticalFlowToNeVel/getMtf01pValidity'
//  '<S66>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter'
//  '<S67>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1'
//  '<S68>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter/Compute Filter Numerator And Denominator'
//  '<S69>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1/Compute Filter Numerator And Denominator'


//-
//  Requirements for '<Root>': stateEstimatorEskf


#endif                                 // stateEstimatorEskf_h_

//
// File trailer for generated code.
//
// [EOF]
//
