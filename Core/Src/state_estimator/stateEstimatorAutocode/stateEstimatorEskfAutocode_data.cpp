//
// File: stateEstimatorEskfAutocode_data.cpp
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 7.0
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 23 09:38:38 2026
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
    { -0.00345489639F, 0.0153643331F, 0.00632852921F },

    { 1.00597918F, 0.0F, 0.0F, 0.0F, 1.00528288F, 0.0F, 0.0F, 0.0F, 0.988832414F
    }
  },

  // Expression: magParams
  //  Referenced by: '<Root>/magParams'

  {
    { -1.96354842F, 0.0964627191F, -2.23013663F },

    { 0.999051034F, 0.00593270594F, 0.0306883957F, 0.00593270594F, 0.961621344F,
      3.2730135e-5F, 0.0306883957F, 3.2730135e-5F, 1.04187894F }
  },

  // Expression: mtf01pParams
  //  Referenced by: '<Root>/mtf01pParams'

  {
    { 0.0F, 1.0F, -1.0F, 0.0F },

    { 0.08F, 5.0F },
    50U,
    1U,
    12.566371F,

    { 0.103F, 0.0F, 0.01F }
  },

  // Computed Parameter: stateEstSmParams_Value
  //  Referenced by: '<Root>/stateEstSmParams'

  {
    10.0F,
    10.0F,
    5.0F,
    5.0F,
    3U,
    2.0F,
    2.0F,
    4.0F,
    0.05F,
    0.244977906F,
    true
  }
};

//
// File trailer for generated code.
//
// [EOF]
//
