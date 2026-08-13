//
// File: stateEstimatorEskf.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.53
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Aug 13 15:28:24 2026
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
#include "multiword_types.h"
#include <cstring>

// Class declaration for model stateEstimatorEskf
class stateEstimatorEskf final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for model 'stateEstimatorEskf'
  struct DW_stateEstimatorEskf_T {
    uint64m_T timeBuf_ms[64];          // '<S13>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_c[64];        // '<S13>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_d[64];        // '<S12>/OutputPredictor'
    uint64m_T timeBuf_ms_l[32];        // '<S13>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_o[24];        // '<S13>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_e[24];        // '<S13>/DelayedHorizonBufferManager'
    busSensorIn sensorDataOut;         // '<Root>/estimatorStateMachine'
    uint64m_T timeBuf_ms_k[16];        // '<S13>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms;         // '<S13>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_h;       // '<S13>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_g;       // '<S13>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_gh;      // '<S13>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_d;       // '<S13>/DelayedHorizonBufferManager'
    real_T refLatLonAlt[3];            // '<Root>/estimatorStateMachine'
    real_T gpsIdx;                     // '<Root>/estimatorStateMachine'
    real32_T TmpSignalConversionAtSFunctionI[3];// '<Root>/estimatorStateMachine' 
    real32_T initialStates[20];        // '<Root>/estimatorStateMachine'
    real32_T initialDcmBodyToNed[9];   // '<Root>/estimatorStateMachine'
    real32_T XAxis_states[2];          // '<S61>/X Axis'
    real32_T XAxis1_states[2];         // '<S61>/X Axis1'
    real32_T XAxis2_states[2];         // '<S61>/X Axis2'
    real32_T XAxis_states_e[2];        // '<S62>/X Axis'
    real32_T XAxis1_states_a[2];       // '<S62>/X Axis1'
    real32_T XAxis2_states_j[2];       // '<S62>/X Axis2'
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
    real32_T dataBuf[448];             // '<S13>/DelayedHorizonBufferManager'
    real32_T dataBuf_o[192];           // '<S13>/DelayedHorizonBufferManager'
    real32_T dataBuf_ot[96];           // '<S13>/DelayedHorizonBufferManager'
    real32_T altitudeBuf_m[24];        // '<S13>/DelayedHorizonBufferManager'
    real32_T aglBuf_m[32];             // '<S13>/DelayedHorizonBufferManager'
    real32_T dataBuf_i[48];            // '<S13>/DelayedHorizonBufferManager'
    real32_T qCurrent[4];              // '<S12>/OutputPredictor'
    real32_T pCurrent[3];              // '<S12>/OutputPredictor'
    real32_T vCurrent[3];              // '<S12>/OutputPredictor'
    real32_T quatBuf[256];             // '<S12>/OutputPredictor'
    real32_T posBuf[192];              // '<S12>/OutputPredictor'
    real32_T velBuf[192];              // '<S12>/OutputPredictor'
    real32_T stateEstInitPct;          // '<Root>/estimatorStateMachine'
    real32_T UnitDelay_DSTATE;         // '<S7>/Unit Delay'
    real32_T DiscreteTransferFcn_states;// '<S71>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_states_n;// '<S70>/Discrete Transfer Fcn'
    real32_T XAxis_tmp;                // '<S61>/X Axis'
    real32_T XAxis1_tmp;               // '<S61>/X Axis1'
    real32_T XAxis2_tmp;               // '<S61>/X Axis2'
    real32_T XAxis_tmp_o;              // '<S62>/X Axis'
    real32_T XAxis1_tmp_l;             // '<S62>/X Axis1'
    real32_T XAxis2_tmp_o;             // '<S62>/X Axis2'
    real32_T DiscreteTransferFcn_tmp;  // '<S71>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_tmp_b;// '<S70>/Discrete Transfer Fcn'
    real32_T imuIdx;                   // '<Root>/estimatorStateMachine'
    real32_T magIdx;                   // '<Root>/estimatorStateMachine'
    real32_T baroIdx;                  // '<Root>/estimatorStateMachine'
    real32_T baroBias_m;               // '<Root>/estimatorStateMachine'
    real32_T baroInitAltM2;            // '<Root>/estimatorStateMachine'
    real32_T baroInitAltMean;          // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1;        // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_n;      // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_g;      // '<Root>/estimatorStateMachine'
    enumStateEstimateMode modeBuf[64]; // '<S13>/DelayedHorizonBufferManager'
    uint16_T gpsValidCount;            // '<Root>/estimatorStateMachine'
    uint16_T head;                     // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail;                     // '<S13>/DelayedHorizonBufferManager'
    uint16_T count;                    // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_e;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_g;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_g;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_m;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_n;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_b;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_k;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_o;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_f;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_c;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_i;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_k;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_b;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_c;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_n;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_i;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T tail_l;                   // '<S13>/DelayedHorizonBufferManager'
    uint16_T count_i;                  // '<S13>/DelayedHorizonBufferManager'
    uint16_T head_l;                   // '<S12>/OutputPredictor'
    uint16_T tail_j;                   // '<S12>/OutputPredictor'
    uint16_T count_a;                  // '<S12>/OutputPredictor'
    uint8_T is_active_c3_stateEstimatorEskf;// '<Root>/estimatorStateMachine'
    uint8_T is_c3_stateEstimatorEskf;  // '<Root>/estimatorStateMachine'
    boolean_T resetStates;             // '<Root>/estimatorStateMachine'
    boolean_T DelayInput1_DSTATE;      // '<S64>/Delay Input1'
    boolean_T icLoad;                  // '<S1>/Delay'
    boolean_T icLoad_g;                // '<S1>/Delay2'
    boolean_T isAttInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T isBaroInitialized;       // '<Root>/estimatorStateMachine'
    boolean_T isPosInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T covP_not_empty;          // '<S1>/EKF'
    boolean_T haveLastPushTime;        // '<S13>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_h;      // '<S13>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_p;      // '<S13>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_l;      // '<S13>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_n;      // '<S13>/DelayedHorizonBufferManager'
    boolean_T initialized_not_empty;   // '<S12>/OutputPredictor'
  };

  // Constant parameters (default storage)
  struct ConstP_stateEstimatorEskf_T {
    // Expression: ekfParams
    //  Referenced by: '<S13>/DelayedHorizonBufferManager'

    struct_vLaQwIJ2Vqz5mLwc4gApSE DelayedHorizonBufferManager_ekf;
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
  void stateEstimatorEskf_magFifo(boolean_T isMagValid, const uint64m_T
    magTimeIn_ms, const real32_T magIn_uT[3], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T magFifoParams_capacity, const uint64m_T
    magFifoParams_minInterval_ms, const uint64m_T magFifoParams_maxAge_ms, const
    uint64m_T magFifoParams_resetThreshold_ms, boolean_T *magReady, uint64m_T
    *magTimeOut_ms, real32_T magOut_uT[3], uint8_T *status, uint16_T *countOut);
  void stateEstimatorEskf_gpsFifo(boolean_T isGpsValid, const uint64m_T
    gpsTimeIn_ms, const real32_T posVelIn[6], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T gpsFifoParams_capacity, const uint64m_T
    gpsFifoParams_minInterval_ms, const uint64m_T gpsFifoParams_maxAge_ms, const
    uint64m_T gpsFifoParams_resetThreshold_ms, boolean_T *gpsReady, uint64m_T
    *gpsTimeOut_ms, real32_T posVelOut[6], uint8_T *status, uint16_T *countOut);
  void stateEstimatorEskf_lidarFifo(boolean_T isLidarValid, const uint64m_T
    lidarTimeIn_ms, real32_T aglIn_m, const uint64m_T fusionTime_ms, boolean_T
    reset, uint16_T lidarFifoParams_capacity, const uint64m_T
    lidarFifoParams_minInterval_ms, const uint64m_T lidarFifoParams_maxAge_ms,
    const uint64m_T lidarFifoParams_resetThreshold_, boolean_T *lidarReady,
    uint64m_T *lidarTimeOut_ms, real32_T *aglOut_m, uint8_T *status, uint16_T
    *countOut);
  void stateEstimatorEskf_flowFifo(boolean_T isFlowValid, const uint64m_T
    flowTimeIn_ms, const real32_T velNEIn[2], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T flowFifoParams_capacity, const uint64m_T
    flowFifoParams_minInterval_ms, const uint64m_T flowFifoParams_maxAge_ms,
    const uint64m_T flowFifoParams_resetThreshold_m, boolean_T *flowReady,
    uint64m_T *flowTimeOut_ms, real32_T velNEOut[2], uint8_T *status, uint16_T
    *countOut);
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
//  Block '<S22>/Reshape' : Reshape block reduction
//  Block '<S22>/Reshape1' : Reshape block reduction
//  Block '<S22>/Reshape2' : Reshape block reduction
//  Block '<S36>/Reshape' : Reshape block reduction
//  Block '<S45>/Reshape' : Reshape block reduction
//  Block '<S56>/Reshape (9) to [3x3] column-major' : Reshape block reduction
//  Block '<Root>/Signal Conversion' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion1' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion2' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion3' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion4' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion5' : Eliminate redundant signal conversion block


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
//  '<S12>'  : 'stateEstimatorEskf/EKF/DelayedHorizonOutput'
//  '<S13>'  : 'stateEstimatorEskf/EKF/DelayedHorizonPassthrough'
//  '<S14>'  : 'stateEstimatorEskf/EKF/EKF'
//  '<S15>'  : 'stateEstimatorEskf/EKF/QuatToDCM'
//  '<S16>'  : 'stateEstimatorEskf/EKF/DelayedHorizonOutput/OutputPredictor'
//  '<S17>'  : 'stateEstimatorEskf/EKF/DelayedHorizonPassthrough/DelayedHorizonBufferManager'
//  '<S18>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang'
//  '<S19>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM'
//  '<S20>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault'
//  '<S21>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3'
//  '<S22>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Get DCM Values'
//  '<S23>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM'
//  '<S24>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input'
//  '<S25>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem'
//  '<S26>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem1'
//  '<S27>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem2'
//  '<S28>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input'
//  '<S29>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem'
//  '<S30>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem1'
//  '<S31>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem2'
//  '<S32>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error'
//  '<S33>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal'
//  '<S34>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else No Action'
//  '<S35>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper'
//  '<S36>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal'
//  '<S37>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper'
//  '<S38>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Error'
//  '<S39>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/None'
//  '<S40>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Warning'
//  '<S41>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Error'
//  '<S42>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/None'
//  '<S43>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Warning'
//  '<S44>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal/transpose*dcm ~= eye(3)'
//  '<S45>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/Determinant of 3x3 Matrix'
//  '<S46>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/determinant does not equal 1'
//  '<S47>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A11'
//  '<S48>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A12'
//  '<S49>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A13'
//  '<S50>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A21'
//  '<S51>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A22'
//  '<S52>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A23'
//  '<S53>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A31'
//  '<S54>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A32'
//  '<S55>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A33'
//  '<S56>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Create 3x3 Matrix'
//  '<S57>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize'
//  '<S58>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus'
//  '<S59>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S60>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters'
//  '<S61>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Accel Notch Filters'
//  '<S62>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Gyro Notch Filters'
//  '<S63>'  : 'stateEstimatorEskf/latLonAltToNedPos/Compare To Constant'
//  '<S64>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive'
//  '<S65>'  : 'stateEstimatorEskf/latLonAltToNedPos/convertLlhToNedPos'
//  '<S66>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive/Positive'
//  '<S67>'  : 'stateEstimatorEskf/opticalFlowToNeVel/Cross Product'
//  '<S68>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf'
//  '<S69>'  : 'stateEstimatorEskf/opticalFlowToNeVel/getMtf01pValidity'
//  '<S70>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter'
//  '<S71>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1'
//  '<S72>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter/Compute Filter Numerator And Denominator'
//  '<S73>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1/Compute Filter Numerator And Denominator'


//-
//  Requirements for '<Root>': stateEstimatorEskf


#endif                                 // stateEstimatorEskf_h_

//
// File trailer for generated code.
//
// [EOF]
//
