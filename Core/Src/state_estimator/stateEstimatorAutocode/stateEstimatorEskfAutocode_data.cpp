//
// File: stateEstimatorEskfAutocode_data.cpp
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 1.47
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:05:18 2025
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
    { 2.883422396918221F, -1.238530677462690F, 8.964449713442917F },

    { 1.032335682662910F, -0.000984174346905F, -0.014281844150252F, -0.000984174346905F, 0.993973726248168F,
      0.005887197117069F, -0.014281844150252F, 0.005887197117069F, 0.974783278360027F }
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
    0.05F,
    0.244977906F,
    true
  },

  // Expression: mtf01pParams
  //  Referenced by: '<Root>/mtf01pParams'

  {
    { 0.0F, 1.0F, -1.0F, 0.0F },

    { 0.08F, 5.0F },
    50U,
    1U,
    12.566371F
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
