//
// File: stateEstimatorEskf.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.88
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 23 09:38:07 2026
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
    uint64m_T timeBuf_ms[64];          // '<S15>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_k[64];        // '<S15>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_d[64];        // '<S14>/OutputPredictor'
    uint64m_T timeBuf_ms_m[32];        // '<S15>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_g[24];        // '<S15>/DelayedHorizonBufferManager'
    uint64m_T timeBuf_ms_o[24];        // '<S15>/DelayedHorizonBufferManager'
    busSensorIn sensorDataOut;         // '<Root>/estimatorStateMachine'
    uint64m_T timeBuf_ms_c[16];        // '<S15>/DelayedHorizonBufferManager'
    busDhBufferDebug dhBufferDebug;    // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastImuTimeOut_ms;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms;         // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastMagTimeOut_ms;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_b;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastGpsTimeOut_ms;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_j;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastBaroTimeOut_ms;      // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_c;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastLidarTimeOut_ms;     // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastPushTime_ms_f;       // '<S15>/DelayedHorizonBufferManager'
    uint64m_T lastFlowTimeOut_ms;      // '<S15>/DelayedHorizonBufferManager'
    real_T refLatLonAlt[3];            // '<Root>/estimatorStateMachine'
    real_T gpsIdx;                     // '<Root>/estimatorStateMachine'
    real32_T TmpSignalConversionAtSFunctionI[3];// '<Root>/estimatorStateMachine' 
    real32_T initialStates[20];        // '<Root>/estimatorStateMachine'
    real32_T initialDcmBodyToNed[9];   // '<Root>/estimatorStateMachine'
    real32_T XAxis_states[2];          // '<S66>/X Axis'
    real32_T XAxis1_states[2];         // '<S66>/X Axis1'
    real32_T XAxis2_states[2];         // '<S66>/X Axis2'
    real32_T XAxis_states_e[2];        // '<S67>/X Axis'
    real32_T XAxis1_states_a[2];       // '<S67>/X Axis1'
    real32_T XAxis2_states_j[2];       // '<S67>/X Axis2'
    real32_T UnitDelay2_DSTATE[9];     // '<Root>/Unit Delay2'
    real32_T UnitDelay_DSTATE_e[20];   // '<Root>/Unit Delay'
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
    real32_T dataBuf[448];             // '<S15>/DelayedHorizonBufferManager'
    real32_T lastImuOut[7];            // '<S15>/DelayedHorizonBufferManager'
    real32_T dataBuf_i[192];           // '<S15>/DelayedHorizonBufferManager'
    real32_T lastMagOut_uT[3];         // '<S15>/DelayedHorizonBufferManager'
    real32_T dataBuf_f[96];            // '<S15>/DelayedHorizonBufferManager'
    real32_T lastPosVelOut[6];         // '<S15>/DelayedHorizonBufferManager'
    real32_T altitudeBuf_m[24];        // '<S15>/DelayedHorizonBufferManager'
    real32_T aglBuf_m[32];             // '<S15>/DelayedHorizonBufferManager'
    real32_T dataBuf_l[48];            // '<S15>/DelayedHorizonBufferManager'
    real32_T lastVelNEOut[2];          // '<S15>/DelayedHorizonBufferManager'
    real32_T qCurrent[4];              // '<S14>/OutputPredictor'
    real32_T pCurrent[3];              // '<S14>/OutputPredictor'
    real32_T vCurrent[3];              // '<S14>/OutputPredictor'
    real32_T quatBuf[256];             // '<S14>/OutputPredictor'
    real32_T posBuf[192];              // '<S14>/OutputPredictor'
    real32_T velBuf[192];              // '<S14>/OutputPredictor'
    real32_T stateEstInitPct;          // '<Root>/estimatorStateMachine'
    real32_T UnitDelay_DSTATE;         // '<S10>/Unit Delay'
    real32_T DiscreteTransferFcn_states;// '<S77>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_states_n;// '<S76>/Discrete Transfer Fcn'
    real32_T XAxis_tmp;                // '<S66>/X Axis'
    real32_T XAxis1_tmp;               // '<S66>/X Axis1'
    real32_T XAxis2_tmp;               // '<S66>/X Axis2'
    real32_T XAxis_tmp_o;              // '<S67>/X Axis'
    real32_T XAxis1_tmp_l;             // '<S67>/X Axis1'
    real32_T XAxis2_tmp_o;             // '<S67>/X Axis2'
    real32_T DiscreteTransferFcn_tmp;  // '<S77>/Discrete Transfer Fcn'
    real32_T DiscreteTransferFcn_tmp_b;// '<S76>/Discrete Transfer Fcn'
    real32_T imuIdx;                   // '<Root>/estimatorStateMachine'
    real32_T magIdx;                   // '<Root>/estimatorStateMachine'
    real32_T baroIdx;                  // '<Root>/estimatorStateMachine'
    real32_T baroBias_m;               // '<Root>/estimatorStateMachine'
    real32_T baroInitAltM2;            // '<Root>/estimatorStateMachine'
    real32_T baroInitAltMean;          // '<Root>/estimatorStateMachine'
    real32_T lastAltitudeOut_m;        // '<S15>/DelayedHorizonBufferManager'
    real32_T lastAglOut_m;             // '<S15>/DelayedHorizonBufferManager'
    uint32_T durationCounter_1;        // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_n;      // '<Root>/estimatorStateMachine'
    uint32_T durationCounter_1_g;      // '<Root>/estimatorStateMachine'
    enumStateEstimateMode modeBuf[64]; // '<S15>/DelayedHorizonBufferManager'
    uint16_T gpsValidCount;            // '<Root>/estimatorStateMachine'
    uint16_T head;                     // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail;                     // '<S15>/DelayedHorizonBufferManager'
    uint16_T count;                    // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_e;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_g;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_g;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_b;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_k;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_d;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_a;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_c;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_k;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_f;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_p;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_d1;                 // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_f2;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_pr;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_kg;                 // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_c;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T tail_i;                   // '<S15>/DelayedHorizonBufferManager'
    uint16_T count_a;                  // '<S15>/DelayedHorizonBufferManager'
    uint16_T head_l;                   // '<S14>/OutputPredictor'
    uint16_T tail_j;                   // '<S14>/OutputPredictor'
    uint16_T count_aw;                 // '<S14>/OutputPredictor'
    uint8_T is_active_c3_stateEstimatorEskf;// '<Root>/estimatorStateMachine'
    uint8_T is_c3_stateEstimatorEskf;  // '<Root>/estimatorStateMachine'
    boolean_T resetStates;             // '<Root>/estimatorStateMachine'
    boolean_T DelayInput1_DSTATE;      // '<S69>/Delay Input1'
    boolean_T icLoad;                  // '<S1>/Delay'
    boolean_T icLoad_g;                // '<S1>/Delay2'
    boolean_T isAttInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T isBaroInitialized;       // '<Root>/estimatorStateMachine'
    boolean_T isPosInitialized;        // '<Root>/estimatorStateMachine'
    boolean_T covP_not_empty;          // '<S1>/EKF'
    boolean_T haveLastPushTime;        // '<S15>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_m;      // '<S15>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_j;      // '<S15>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_jj;     // '<S15>/DelayedHorizonBufferManager'
    boolean_T haveLastPushTime_p;      // '<S15>/DelayedHorizonBufferManager'
    boolean_T initialized_not_empty;   // '<S14>/OutputPredictor'
  };

  // Constant parameters (default storage)
  struct ConstP_stateEstimatorEskf_T {
    // Expression: ekfParams
    //  Referenced by: '<S15>/DelayedHorizonBufferManager'

    struct_yFxKZKIsAG5otzDoKHACEG DelayedHorizonBufferManager_ekf;
  };

  // Initial conditions function
  void init();

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
            busMtf01pData *rtu_mtf01pData, const busImuNtchFiltParams
            *rtu_imuNotchFiltParams, const busAccelParams *rtu_accelParams,
            const busMagParams *rtu_magParams, const busMtf01pParams
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
    latLonAltOut[3], const real32_T VectorConcatenate[2], const real32_T
    *Divide1, const real32_T Product[3], const real32_T Divide[3], const
    busMagData *rtu_magData, const busGpsData *rtu_gpsData, const busBaroData
    *rtu_baroData, const busStateEstSmParams *rtu_stateEstSmParams);
  void state_enter_atomic_RUN_INIT_GPS(enumStateEstimateMode *mode, real_T
    latLonAltOut[3], const real32_T *Divide1, const real32_T Product[3], const
    real32_T Divide[3], const busMagData *rtu_magData, const busGpsData
    *rtu_gpsData, const busBaroData *rtu_baroData);
  void stateEstimatorEskf_magFifo(boolean_T isMagValid, const uint64m_T
    magTimeIn_ms, const real32_T magIn_uT[3], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T magFifoParams_capacity, const uint64m_T
    magFifoParams_minInterval_ms, const uint64m_T magFifoParams_maxAge_ms, const
    uint64m_T magFifoParams_resetThreshold_ms, boolean_T *magReady, uint64m_T
    *magTimeOut_ms, real32_T magOut_uT[3], enumDhFifoStatus *status, uint16_T
    *countOut);
  void stateEstimatorEskf_gpsFifo(boolean_T isGpsValid, const uint64m_T
    gpsTimeIn_ms, const real32_T posVelIn[6], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T gpsFifoParams_capacity, const uint64m_T
    gpsFifoParams_minInterval_ms, const uint64m_T gpsFifoParams_maxAge_ms, const
    uint64m_T gpsFifoParams_resetThreshold_ms, boolean_T *gpsReady, uint64m_T
    *gpsTimeOut_ms, real32_T posVelOut[6], enumDhFifoStatus *status, uint16_T
    *countOut);
  void stateEstimatorEskf_lidarFifo(boolean_T isLidarValid, const uint64m_T
    lidarTimeIn_ms, real32_T aglIn_m, const uint64m_T fusionTime_ms, boolean_T
    reset, uint16_T lidarFifoParams_capacity, const uint64m_T
    lidarFifoParams_minInterval_ms, const uint64m_T lidarFifoParams_maxAge_ms,
    const uint64m_T lidarFifoParams_resetThreshold_, boolean_T *lidarReady,
    uint64m_T *lidarTimeOut_ms, real32_T *aglOut_m, enumDhFifoStatus *status,
    uint16_T *countOut);
  void stateEstimatorEskf_flowFifo(boolean_T isFlowValid, const uint64m_T
    flowTimeIn_ms, const real32_T velNEIn[2], const uint64m_T fusionTime_ms,
    boolean_T reset, uint16_T flowFifoParams_capacity, const uint64m_T
    flowFifoParams_minInterval_ms, const uint64m_T flowFifoParams_maxAge_ms,
    const uint64m_T flowFifoParams_resetThreshold_m, boolean_T *flowReady,
    uint64m_T *flowTimeOut_ms, real32_T velNEOut[2], enumDhFifoStatus *status,
    uint16_T *countOut);
};

// Constant parameters (default storage)
extern const stateEstimatorEskf::ConstP_stateEstimatorEskf_T
  stateEstimatorEskf_ConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S20>/Constant' : Unused code path elimination
//  Block '<S20>/Constant1' : Unused code path elimination
//  Block '<Root>/Gain' : Unused code path elimination
//  Block '<Root>/Gain1' : Unused code path elimination
//  Block '<Root>/Signal Conversion4' : Unused code path elimination
//  Block '<S7>/Product1' : Unused code path elimination
//  Block '<S27>/Reshape' : Reshape block reduction
//  Block '<S27>/Reshape1' : Reshape block reduction
//  Block '<S27>/Reshape2' : Reshape block reduction
//  Block '<S41>/Reshape' : Reshape block reduction
//  Block '<S50>/Reshape' : Reshape block reduction
//  Block '<S61>/Reshape (9) to [3x3] column-major' : Reshape block reduction
//  Block '<Root>/Signal Conversion' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion1' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion2' : Eliminate redundant signal conversion block
//  Block '<Root>/Signal Conversion3' : Eliminate redundant signal conversion block
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
//  '<S2>'   : 'stateEstimatorEskf/ExplicitLidarVariant'
//  '<S3>'   : 'stateEstimatorEskf/LidarTimestampVariant'
//  '<S4>'   : 'stateEstimatorEskf/LidarVariantSelector'
//  '<S5>'   : 'stateEstimatorEskf/Quaternions to Rotation Angles'
//  '<S6>'   : 'stateEstimatorEskf/Subsystem Reference'
//  '<S7>'   : 'stateEstimatorEskf/accelCorrection'
//  '<S8>'   : 'stateEstimatorEskf/estimatorStateMachine'
//  '<S9>'   : 'stateEstimatorEskf/eulToDcm'
//  '<S10>'  : 'stateEstimatorEskf/latLonAltToNedPos'
//  '<S11>'  : 'stateEstimatorEskf/magCorrection'
//  '<S12>'  : 'stateEstimatorEskf/opticalFlowToNeVel'
//  '<S13>'  : 'stateEstimatorEskf/pressureToAlt'
//  '<S14>'  : 'stateEstimatorEskf/EKF/DelayedHorizonOutput'
//  '<S15>'  : 'stateEstimatorEskf/EKF/DelayedHorizonPassthrough'
//  '<S16>'  : 'stateEstimatorEskf/EKF/EKF'
//  '<S17>'  : 'stateEstimatorEskf/EKF/QuatToDCM'
//  '<S18>'  : 'stateEstimatorEskf/EKF/DelayedHorizonOutput/OutputPredictor'
//  '<S19>'  : 'stateEstimatorEskf/EKF/DelayedHorizonPassthrough/DelayedHorizonBufferManager'
//  '<S20>'  : 'stateEstimatorEskf/ExplicitLidarVariant/Off'
//  '<S21>'  : 'stateEstimatorEskf/LidarTimestampVariant/Off'
//  '<S22>'  : 'stateEstimatorEskf/LidarVariantSelector/Off'
//  '<S23>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang'
//  '<S24>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM'
//  '<S25>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault'
//  '<S26>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3'
//  '<S27>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Get DCM Values'
//  '<S28>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM'
//  '<S29>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input'
//  '<S30>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem'
//  '<S31>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem1'
//  '<S32>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotDefault/Protect asincos input/If Action Subsystem2'
//  '<S33>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input'
//  '<S34>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem'
//  '<S35>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem1'
//  '<S36>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/AxisRotZeroR3/Protect asincos input/If Action Subsystem2'
//  '<S37>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error'
//  '<S38>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal'
//  '<S39>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else No Action'
//  '<S40>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper'
//  '<S41>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal'
//  '<S42>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper'
//  '<S43>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Error'
//  '<S44>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/None'
//  '<S45>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/Else If Not Orthogonal/Warning'
//  '<S46>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Error'
//  '<S47>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/None'
//  '<S48>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/If Not Proper/Warning'
//  '<S49>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotOrthogonal/transpose*dcm ~= eye(3)'
//  '<S50>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/Determinant of 3x3 Matrix'
//  '<S51>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/DCM2Ang/Validate DCM/If Warning//Error/isNotProper/determinant does not equal 1'
//  '<S52>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A11'
//  '<S53>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A12'
//  '<S54>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A13'
//  '<S55>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A21'
//  '<S56>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A22'
//  '<S57>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A23'
//  '<S58>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A31'
//  '<S59>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A32'
//  '<S60>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/A33'
//  '<S61>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Create 3x3 Matrix'
//  '<S62>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize'
//  '<S63>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus'
//  '<S64>'  : 'stateEstimatorEskf/Quaternions to Rotation Angles/Quat2DCM/Quaternion Normalize/Quaternion Modulus/Quaternion Norm'
//  '<S65>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters'
//  '<S66>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Accel Notch Filters'
//  '<S67>'  : 'stateEstimatorEskf/Subsystem Reference/IMU Filters/Gyro Notch Filters'
//  '<S68>'  : 'stateEstimatorEskf/latLonAltToNedPos/Compare To Constant'
//  '<S69>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive'
//  '<S70>'  : 'stateEstimatorEskf/latLonAltToNedPos/convertLlhToNedPos'
//  '<S71>'  : 'stateEstimatorEskf/latLonAltToNedPos/Detect Rise Positive/Positive'
//  '<S72>'  : 'stateEstimatorEskf/opticalFlowToNeVel/Cross Product'
//  '<S73>'  : 'stateEstimatorEskf/opticalFlowToNeVel/MATLAB Function'
//  '<S74>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf'
//  '<S75>'  : 'stateEstimatorEskf/opticalFlowToNeVel/getMtf01pValidity'
//  '<S76>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter'
//  '<S77>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1'
//  '<S78>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter/Compute Filter Numerator And Denominator'
//  '<S79>'  : 'stateEstimatorEskf/opticalFlowToNeVel/filterOf/Discrete First Order Filter1/Compute Filter Numerator And Denominator'


//-
//  Requirements for '<Root>': stateEstimatorEskf


#endif                                 // stateEstimatorEskf_h_

//
// File trailer for generated code.
//
// [EOF]
//
