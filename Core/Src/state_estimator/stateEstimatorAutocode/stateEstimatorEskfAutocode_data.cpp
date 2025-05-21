//
// File: stateEstimatorEskfAutocode_data.cpp
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 1.44
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu May  1 12:29:17 2025
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

// Constant parameters (default storage)
const stateEstimatorAutocode::ConstP_stateEstimatorEskfAuto_T
  stateEstimatorEskfAutoco_ConstP{
  // Expression: imuNtchFiltParams
  //  Referenced by: '<Root>/imuNtchFilterParams'

  {
    {
      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F },

      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F },

      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F }
    },

    {
      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F },

      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F },

      { 0.508761406F, 0.189643666F, 0.498865187F },

      { 1.0F, 0.189643666F, 0.00762659311F }
    }
  },

  // Expression: accelParams
  //  Referenced by: '<Root>/accelParams'

  {
    { -0.0147876265F, -0.00241447636F, 0.0508151F },

    { 0.998770714F, 0.0334529802F, 0.00165741274F, 0.0320384614F, 0.998117924F,
      -0.000863323919F, 0.00456682127F, -0.000593769946F, 0.995291293F }
  },

  // Expression: magParams
  //  Referenced by: '<Root>/magParams'

  {
    { 19.1927F, 42.3204F, -32.0349F },

    { 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F }
  },

  // Computed Parameter: stateEstSmParams_Value
  //  Referenced by: '<Root>/stateEstSmParams'

  {
    10.0F,
    10.0F,
    5.0F,
    5.0F,
    3U,
    10.0F,
    0.244977906F
  },

  // Expression: lidarParams
  //  Referenced by: '<Root>/lidarParams'

  {
    0.1F,
    0.02F,

    { 0.3F, 99.0F }
  }
};

//
// File trailer for generated code.
//
// [EOF]
//
