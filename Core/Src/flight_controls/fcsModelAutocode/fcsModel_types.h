//
// File: fcsModel_types.h
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 1.117
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Sat Jun 14 09:55:15 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#ifndef RTW_HEADER_fcsModel_types_h_
#define RTW_HEADER_fcsModel_types_h_
#include "rtwtypes.h"
#include <array>

// Model Code Variants
#ifndef DEFINED_TYPEDEF_FOR_enumStateMachine_
#define DEFINED_TYPEDEF_FOR_enumStateMachine_

// Defines the current state of the state machine
enum class enumStateMachine
  : int32_T {
  INACTIVE = 0,                        // Default value
  MTR_ARMED,
  INFLIGHT
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumChirpTrigger_
#define DEFINED_TYPEDEF_FOR_enumChirpTrigger_

// Defines if chirp is on or off
enum class enumChirpTrigger
  : int32_T {
  OFF = 0,                             // Default value
  ON
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumChirpType_
#define DEFINED_TYPEDEF_FOR_enumChirpType_

// Defines where chirp is inejected
enum class enumChirpType
  : int32_T {
  NONE = 0,                            // Default value
  MX,
  MY,
  MZ,
  FZ
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_enumFlightMode_
#define DEFINED_TYPEDEF_FOR_enumFlightMode_

// Defines the flight mode in use
enum class enumFlightMode
  : int32_T {
  ACRO = 0,
  STABILIZE,                           // Default value
  ALT_CONTROL,
  POS_CONTROL
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busRcInCmds_
#define DEFINED_TYPEDEF_FOR_busRcInCmds_

// Radio controller commands
struct busRcInCmds
{
  // Non dimensional throttle command from RC transmitter
  uint16_T throttleCmd_nd;

  // Non dimensional joystick Y command from RC transmitter
  uint16_T joystickYCmd_nd;

  // Non dimensional joystick X command from RC transmitter
  uint16_T joystickXCmd_nd;

  // Non dimensional joystick Z command from RC transmitter
  uint16_T joystickZCmd_nd;

  // Switch 1 from RC transmitter
  uint16_T rcSwitch1_nd;

  // Switch 2 from RC transmitter
  uint16_T rcSwitch2_nd;

  // Switch 3 from RC transmitter
  uint16_T rcSwitch3_nd;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busGeodeticPos_
#define DEFINED_TYPEDEF_FOR_busGeodeticPos_

// Bus containing position in lat, lon and altitude
struct busGeodeticPos
{
  // Lattitude in radian
  real_T lat_rad;

  // Longitude in radian
  real_T lon_rad;

  // Height above the ellipsoid in meter
  real_T alt_m;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busStateEstimate_
#define DEFINED_TYPEDEF_FOR_busStateEstimate_

// Bus containing process measurement data to be used by the controllers
struct busStateEstimate
{
  std::array<real_T, 3> attitude_rad;
  std::array<real_T, 3> bodyAngRates_radps;
  std::array<real_T, 3> bodyAccels_mps2;
  busGeodeticPos geodeticPos;
  std::array<real_T, 3> nedPos_m;
  std::array<real_T, 3> nedVel_mps;
  real_T pressure_mbar;
  real_T temp_c;
  real_T aglEst_m;
  real_T climbRateEst_mps;
  std::array<real_T, 9> ned2BodyDcm_nd;
  std::array<real_T, 9> ned2FepDcm_nd;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busPidParams_
#define DEFINED_TYPEDEF_FOR_busPidParams_

// Bus with all the params needed by the PID controller
struct busPidParams
{
  real_T Kp;
  real_T Ki;
  real_T Kd;
  real_T Kb;
  real_T Kt;
  real_T filterBandwidth_radps;
  std::array<real_T, 2> outputLimits;
  std::array<real_T, 2> outputRateLimits;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busSecondOrderFilterParam_
#define DEFINED_TYPEDEF_FOR_busSecondOrderFilterParam_

struct busSecondOrderFilterParam
{
  // Bandwidth of the second order filter
  real_T filterBandwidth_radps;

  // Damping ration of the filter
  real_T dampingRatio_nd;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busSignalConditioningParams_
#define DEFINED_TYPEDEF_FOR_busSignalConditioningParams_

// Bus to contain all the params required by the signal conditioning block
struct busSignalConditioningParams
{
  std::array<real_T, 2> filteredInputLimits;
  std::array<real_T, 2> filteredInputRateLimits;
  std::array<real_T, 2> filteredInputAccelLimits;
  std::array<real_T, 2> filteredInputJerkLimits;
  busSecondOrderFilterParam filterParams;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAttCtrlParams_
#define DEFINED_TYPEDEF_FOR_busAttCtrlParams_

// Bus containing all the parameters necessary for attitude controller
struct busAttCtrlParams
{
  std::array<busPidParams, 3> ctrlParamsArray;
  std::array<busSignalConditioningParams, 3> cmdSignalConditioningParamsArray;
  std::array<busSignalConditioningParams, 3> measSignalConditioningParamsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAngRateCtrlParams_
#define DEFINED_TYPEDEF_FOR_busAngRateCtrlParams_

// Bus containing all the parameters necessary for angular rate controller
struct busAngRateCtrlParams
{
  std::array<busPidParams, 3> ctrlParamsArray;
  std::array<busSignalConditioningParams, 3> cmdSignalConditioningParamsArray;
  std::array<busSignalConditioningParams, 3> measSignalConditioningParamsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busInnerLoopCtrlParams_
#define DEFINED_TYPEDEF_FOR_busInnerLoopCtrlParams_

// Bus containing parameters for inner loop controllers
struct busInnerLoopCtrlParams
{
  busAttCtrlParams attCtrlParams;
  busAngRateCtrlParams angRateCtrlParams;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busPosCtrlParams_
#define DEFINED_TYPEDEF_FOR_busPosCtrlParams_

// Position controller parameters
struct busPosCtrlParams
{
  std::array<busPidParams, 3> ctrlParamsArray;
  std::array<busSignalConditioningParams, 3> cmdSignalConditioningParamsArray;
  std::array<busSignalConditioningParams, 3> measSignalConditioningParamsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busVelCtrlParams_
#define DEFINED_TYPEDEF_FOR_busVelCtrlParams_

// Velocity controller parameters
struct busVelCtrlParams
{
  std::array<busPidParams, 3> ctrlParamsArray;
  std::array<busSignalConditioningParams, 3> cmdSignalConditioningParamsArray;
  std::array<busSignalConditioningParams, 3> measSignalConditioningParamsArray;
  std::array<busSignalConditioningParams, 3> accelSignalConditioningParamsArray;
  std::array<real_T, 3> accelFbGainsArray;
  std::array<real_T, 3> ffGainsArray;
  real_T baseMass_kg;
  std::array<real_T, 2> baseMassMinMax_kg;
  std::array<real_T, 2> firstOrderHeadingFilterNum;
  std::array<real_T, 2> firstOrderHeadingFilterDen;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busXyBodyAccelCtrlParams_
#define DEFINED_TYPEDEF_FOR_busXyBodyAccelCtrlParams_

// Control parameters for xy body accel feedback
struct busXyBodyAccelCtrlParams
{
  std::array<busPidParams, 2> ctrlParamsArray;
  std::array<real_T, 2> ffGainsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busZaccelCtrlParams_
#define DEFINED_TYPEDEF_FOR_busZaccelCtrlParams_

// Bus Containing Data For Z (NED without gravity) acceleration controller
struct busZaccelCtrlParams
{
  busPidParams ctrlParams;
  busSignalConditioningParams cmdSignalConditioningParams;
  busSignalConditioningParams measSignalConditioningParams;
  real_T ffGain;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busOuterLoopCtrlParams_
#define DEFINED_TYPEDEF_FOR_busOuterLoopCtrlParams_

// Bus containing parameters for outer loop controllers
struct busOuterLoopCtrlParams
{
  busPosCtrlParams posCtrlParams;
  busVelCtrlParams velCtrlParams;
  busXyBodyAccelCtrlParams xyBodyAccelCtrlParams;
  busZaccelCtrlParams zAccelCtrlParams;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busSysIdInjectionParams_
#define DEFINED_TYPEDEF_FOR_busSysIdInjectionParams_

// Bus containing necessary parameters to generate sysId inputs
struct busSysIdInjectionParams
{
  real_T mxAmp;
  real_T myAmp;
  real_T mzAmp;
  real_T fzAmp;
  real_T fStart_Hz;
  real_T fEnd_hz;
  real_T tRec_s;
  real_T fadeInTime_s;
  real_T fadeOutTime_s;
  real_T c1;
  real_T c2;
  std::array<real_T, 2> filterNum;
  std::array<real_T, 2> filterDen;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFcsParams_
#define DEFINED_TYPEDEF_FOR_busFcsParams_

// All the FCS params as input
struct busFcsParams
{
  busInnerLoopCtrlParams innerLoopCtrlParams;
  busOuterLoopCtrlParams outerLoopCtrlParams;
  busSysIdInjectionParams sysIdInjectionParams;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busInnerLoopToAlloc_
#define DEFINED_TYPEDEF_FOR_busInnerLoopToAlloc_

// Bus containing data from inner loop to allocation block
struct busInnerLoopToAlloc
{
  // Commanded thrust
  real_T thrustCmd_N;

  // X moment command
  real_T xMomCmd_Nm;

  // Y moment command
  real_T yMomCmd_Nm;

  // Z moment command
  real_T zMomCmd_Nm;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busPidDebug_
#define DEFINED_TYPEDEF_FOR_busPidDebug_

// Bus with PID controller debug signals for post run analysis
struct busPidDebug
{
  // Final output of the PID controller
  real_T output;

  // Output portion due to propertional gain
  real_T proportionalOutput;

  // Output portion due to integral gain
  real_T integralOutput;

  // Output portion due to derivative gain
  real_T derivativeOutput;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAngRateCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busAngRateCtrlDebug_

// Bus containing angular rate controller debug data
struct busAngRateCtrlDebug
{
  std::array<real_T, 3> cmd;
  std::array<real_T, 3> meas;
  std::array<busPidDebug, 3> pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAttCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busAttCtrlDebug_

// Contains debug information from attitude control system
struct busAttCtrlDebug
{
  std::array<real_T, 3> cmd;
  std::array<real_T, 3> meas;
  std::array<busPidDebug, 3> pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busInnerLoopCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busInnerLoopCtrlDebug_

// Bus containing inner loop controller debug data
struct busInnerLoopCtrlDebug
{
  busAngRateCtrlDebug angRateCtrlDebug;
  busAttCtrlDebug attCtrlDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busVelCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busVelCtrlDebug_

// Debug data from celocity controllers
struct busVelCtrlDebug
{
  std::array<real_T, 3> cmd;
  std::array<real_T, 3> meas;
  std::array<real_T, 3> velCtrlFf;
  std::array<busPidDebug, 3> pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busPosCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busPosCtrlDebug_

// Debug data from position controllers
struct busPosCtrlDebug
{
  std::array<real_T, 3> cmd;
  std::array<real_T, 3> meas;
  std::array<busPidDebug, 3> pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busZaccelCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busZaccelCtrlDebug_

// Debug data from Z accel (NED without gravity) controller
struct busZaccelCtrlDebug
{
  // Az (NED without gravity) cmd
  real_T cmd;

  // Az (NED without gravity) measured
  real_T meas;

  // PID debug data from Az (NED without gravity) controller
  busPidDebug pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busXyBodyAccelCtrIDebug_
#define DEFINED_TYPEDEF_FOR_busXyBodyAccelCtrIDebug_

// Debug data from xy body accel controller
struct busXyBodyAccelCtrIDebug
{
  std::array<real_T, 2> cmd;
  std::array<real_T, 2> meas;
  std::array<busPidDebug, 2> pidDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busOuterLoopCtrlDebug_
#define DEFINED_TYPEDEF_FOR_busOuterLoopCtrlDebug_

// Bus containing debug data from the outer loop controller
struct busOuterLoopCtrlDebug
{
  real_T frcCmd_N;
  busVelCtrlDebug velCtrlDebug;
  busPosCtrlDebug posCtrlDebug;
  busZaccelCtrlDebug zAccelCtrlDebug;
  busXyBodyAccelCtrIDebug xyBodyAccelCtrlDebug;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busSysIdDebug_
#define DEFINED_TYPEDEF_FOR_busSysIdDebug_

// Bus containing chirp debug data
struct busSysIdDebug
{
  // Chirp trigger signal
  enumChirpTrigger chirpTrigger;

  // Chirp type
  enumChirpType chirpType;

  // Chirp signal that was injected
  real_T chirpSignal;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busFcsDebug_
#define DEFINED_TYPEDEF_FOR_busFcsDebug_

// Debug data from FCS
struct busFcsDebug
{
  busInnerLoopToAlloc allocDebug;
  busInnerLoopCtrlDebug innerLoopCtrlDebug;
  busOuterLoopCtrlDebug outerLoopCtrlDebug;
  busSysIdDebug sysIdDebug;
  enumStateMachine state;
  enumFlightMode flightMode;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busOuterLoopCmds_
#define DEFINED_TYPEDEF_FOR_busOuterLoopCmds_

// Bus containing outer loop controller outputs
struct busOuterLoopCmds
{
  // Thrust command generated by the outer loop controller
  real_T thrustCmd_N;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busCtrlInputs_
#define DEFINED_TYPEDEF_FOR_busCtrlInputs_

// A generic bus to contains inputs to a controller
struct busCtrlInputs
{
  // Feed forward command to be fed into the controller
  real_T feedForwardCmd;

  // Controller commanded set point
  real_T cmd;

  // measurement data
  real_T meas;

  // Flag to reset integrators
  boolean_T integratorReset;

  // Command that the controller should track in tracking mode
  real_T trackingCtrlCmd;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAttCtrlInputs_
#define DEFINED_TYPEDEF_FOR_busAttCtrlInputs_

// Bus containing all the inputs necessary for attitude controllers
struct busAttCtrlInputs
{
  std::array<busCtrlInputs, 3> ctrlInputsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busOuterLoopToInnerLoop_
#define DEFINED_TYPEDEF_FOR_busOuterLoopToInnerLoop_

// Bus containing all the data from outer loop to inner loop
struct busOuterLoopToInnerLoop
{
  busOuterLoopCmds outerLoopCmds;
  busAttCtrlInputs attCtrlInputs;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busAngRateCtrlInputs_
#define DEFINED_TYPEDEF_FOR_busAngRateCtrlInputs_

// Bus containing all the inputs necessary for angular rate controllers
struct busAngRateCtrlInputs
{
  std::array<busCtrlInputs, 3> ctrlInputsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busRcOutCmds_
#define DEFINED_TYPEDEF_FOR_busRcOutCmds_

// Bus containing data from RC Interpreter Library
struct busRcOutCmds
{
  // Dimensionalized command computed using throttle stick input.
  // Interpreted unit depends on the flight mode.
  real_T throttleStick;

  // Dimensionalized command computed using roll stick input.
  // Interpreted unit depends on the flight mode.
  real_T rollStick;

  // Dimensionalized command computed using pitch stick input.
  // Interpreted unit depends on the flight mode.
  real_T pitchStick;

  // Dimensionalized command computed using yaw stick input.
  // Interpreted unit depends on the flight mode.
  real_T yawStick;

  // Velocity Command in given X axis generated from RC interpreter in flight modes that
  // require altitude hold
  real_T vxStick_mps;

  // Velocity Command in given Y axis generated from RC interpreter in flight modes that
  // require altitude hold
  real_T vyStick_mps;

  // Velocity Command in given Z axis generated from RC interpreter in flight modes that
  // require altitude hold
  real_T vzStick_mps;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busVelCtrlInputs_
#define DEFINED_TYPEDEF_FOR_busVelCtrlInputs_

// Bus containing all the inputs necessary for velocity controllers
struct busVelCtrlInputs
{
  std::array<busCtrlInputs, 3> ctrlInputsArray;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_busPosCtrlInputs_
#define DEFINED_TYPEDEF_FOR_busPosCtrlInputs_

// Bus containing all the inputs necessary for position controllers
struct busPosCtrlInputs
{
  std::array<busCtrlInputs, 3> ctrlInputsArray;
  busGeodeticPos originGeodeticPos;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_YSjXhEpMC1LYZqGeNUk2hH_
#define DEFINED_TYPEDEF_FOR_struct_YSjXhEpMC1LYZqGeNUk2hH_

struct struct_YSjXhEpMC1LYZqGeNUk2hH
{
  real_T center;
  real_T posDeltaFromCenter;
  real_T negDeltaFromCenter;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_mmIgzrMBPVhvIbVLm0lB6F_
#define DEFINED_TYPEDEF_FOR_struct_mmIgzrMBPVhvIbVLm0lB6F_

struct struct_mmIgzrMBPVhvIbVLm0lB6F
{
  std::array<real_T, 2> pitchRate_radps;
  std::array<real_T, 2> rollRate_radps;
  std::array<real_T, 2> yawRate_radps;
  std::array<real_T, 2> pitch_rad;
  std::array<real_T, 2> roll_rad;
  std::array<real_T, 2> zForce_N;
  std::array<real_T, 2> vx_mps;
  std::array<real_T, 2> vy_mps;
  std::array<real_T, 2> vz_mps;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_bSGttGVktiVsoVvrNEqEKH_
#define DEFINED_TYPEDEF_FOR_struct_bSGttGVktiVsoVvrNEqEKH_

struct struct_bSGttGVktiVsoVvrNEqEKH
{
  std::array<real_T, 2> roll_nd;
  std::array<real_T, 2> pitch_nd;
  std::array<real_T, 2> yaw_nd;
  std::array<real_T, 2> throttle_nd;
  std::array<real_T, 2> vx_nd;
  std::array<real_T, 2> vy_nd;
  std::array<real_T, 2> vz_nd;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_7H7ujBUuoTktv2vn3DMdcE_
#define DEFINED_TYPEDEF_FOR_struct_7H7ujBUuoTktv2vn3DMdcE_

struct struct_7H7ujBUuoTktv2vn3DMdcE
{
  real_T a;
  real_T b;
  real_T c;
};

#endif

#ifndef DEFINED_TYPEDEF_FOR_struct_eOoIfVNgmaQXa53hgj2ZoG_
#define DEFINED_TYPEDEF_FOR_struct_eOoIfVNgmaQXa53hgj2ZoG_

struct struct_eOoIfVNgmaQXa53hgj2ZoG
{
  struct_mmIgzrMBPVhvIbVLm0lB6F cmdLimits;
  std::array<real_T, 2> pwmLimits;
  std::array<real_T, 2> pwmLimitsThrottle;
  real_T pwmThrottleMidHigh;
  real_T pwmThrottleMidLow;
  real_T pwmRollStickMidHigh;
  real_T pwmRollStickMidLow;
  real_T pwmPitchStickMidHigh;
  real_T pwmPitchStickMidLow;
  real_T pwmYawStickMidHigh;
  real_T pwmYawStickMidLow;
  real_T pwmToCmdThrottleSlopeLow;
  real_T pwmToCmdThrottleIncptLow;
  real_T pwmToCmdThrottleSlopeHigh;
  real_T pwmToCmdThrottleIncptHigh;
  real_T pwmMtrArm;
  struct_bSGttGVktiVsoVvrNEqEKH coeffs;
  struct_7H7ujBUuoTktv2vn3DMdcE throttleUnitRangeMapCoeff;
  struct_7H7ujBUuoTktv2vn3DMdcE vzLowRangeMapCoeff;
  struct_7H7ujBUuoTktv2vn3DMdcE vzHighRangeMapCoeff;
  struct_7H7ujBUuoTktv2vn3DMdcE fCmdRangeMapCoeff;
};

#endif
#endif                                 // RTW_HEADER_fcsModel_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
