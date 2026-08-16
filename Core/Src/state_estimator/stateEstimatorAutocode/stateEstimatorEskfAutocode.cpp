//
// File: stateEstimatorEskfAutocode.cpp
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 7.0
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:47 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#include "stateEstimatorEskfAutocode.h"
#include "stateEstimatorEskfAutocode_types.h"
#include "rtwtypes.h"
#include "stateEstimatorEskfAutocode_private.h"
#include "stateEstimatorEskf.h"

// Model step function
void stateEstimatorAutocode::step()
{
  // ModelReference: '<Root>/State Estimator' incorporates:
  //   Constant: '<Root>/accelParams'
  //   Constant: '<Root>/gEarth_mps2'
  //   Constant: '<Root>/imuNtchFilterParams'
  //   Constant: '<Root>/initCovP'
  //   Constant: '<Root>/magParams'
  //   Constant: '<Root>/measNoiseR'
  //   Constant: '<Root>/mtf01pParams'
  //   Constant: '<Root>/processNoiseQ'
  //   Constant: '<Root>/stateEstSmParams'
  //   Inport: '<Root>/baroData'
  //   Inport: '<Root>/gpsData'
  //   Inport: '<Root>/imuData'
  //   Inport: '<Root>/magData'
  //   Inport: '<Root>/mtf01pData'
  //   Outport: '<Root>/bodyAccels_mps2'
  //   Outport: '<Root>/dcmNedToBody'
  //   Outport: '<Root>/dcmNedToFep'
  //   Outport: '<Root>/eulAng_rad'
  //   Outport: '<Root>/stateEstimatorDebug'
  //   Outport: '<Root>/states'

  State_EstimatorMDLOBJ1.step(&stateEstimatorEskfAutocode_U.imuData,
    &stateEstimatorEskfAutocode_U.magData, &stateEstimatorEskfAutocode_U.gpsData,
    &stateEstimatorEskfAutocode_U.baroData,
    &stateEstimatorEskfAutocode_U.mtf01pData,
    &stateEstimatorEskfAutoco_ConstP.imuNtchFilterParams_Value,
    &stateEstimatorEskfAutoco_ConstP.accelParams_Value,
    &stateEstimatorEskfAutoco_ConstP.magParams_Value,
    &stateEstimatorEskfAutoco_ConstP.mtf01pParams_Value,
    &stateEstimatorEskfAutoco_ConstP.stateEstSmParams_Value,
    &rtCP_processNoiseQ_Value[0], &rtCP_measNoiseR_Value[0],
    &rtCP_initCovP_Value[0], &rtCP_gEarth_mps2_Value,
    &stateEstimatorEskfAutocode_Y.states[0],
    &stateEstimatorEskfAutocode_Y.eulAng_rad[0],
    &stateEstimatorEskfAutocode_Y.dcmNedToBody[0],
    &stateEstimatorEskfAutocode_Y.dcmNedToFep[0],
    &stateEstimatorEskfAutocode_Y.bodyAccels_mps2[0],
    &stateEstimatorEskfAutocode_Y.stateEstimatorDebug);
}

// Model initialize function
void stateEstimatorAutocode::initialize()
{
  // SystemInitialize for ModelReference: '<Root>/State Estimator' incorporates:
  //   Constant: '<Root>/accelParams'
  //   Constant: '<Root>/gEarth_mps2'
  //   Constant: '<Root>/imuNtchFilterParams'
  //   Constant: '<Root>/initCovP'
  //   Constant: '<Root>/lidarParams'
  //   Constant: '<Root>/magParams'
  //   Constant: '<Root>/measNoiseR'
  //   Constant: '<Root>/mtf01pParams'
  //   Constant: '<Root>/processNoiseQ'
  //   Constant: '<Root>/stateEstSmParams'
  //   Inport: '<Root>/baroData'
  //   Inport: '<Root>/gpsData'
  //   Inport: '<Root>/imuData'
  //   Inport: '<Root>/lidarData'
  //   Inport: '<Root>/magData'
  //   Inport: '<Root>/mtf01pData'
  //   Outport: '<Root>/bodyAccels_mps2'
  //   Outport: '<Root>/dcmNedToBody'
  //   Outport: '<Root>/dcmNedToFep'
  //   Outport: '<Root>/eulAng_rad'
  //   Outport: '<Root>/stateEstimatorDebug'
  //   Outport: '<Root>/states'

  State_EstimatorMDLOBJ1.init();
}

// Model terminate function
void stateEstimatorAutocode::terminate()
{
  // (no terminate code required)
}

// Constructor
stateEstimatorAutocode::stateEstimatorAutocode():
  stateEstimatorEskfAutocode_U(),
  stateEstimatorEskfAutocode_Y()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
stateEstimatorAutocode::~stateEstimatorAutocode() = default;

//
// File trailer for generated code.
//
// [EOF]
//
