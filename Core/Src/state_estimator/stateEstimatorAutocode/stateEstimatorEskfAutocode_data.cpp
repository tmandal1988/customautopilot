//
// File: stateEstimatorEskfAutocode_data.cpp
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 1.44
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Jun 13 14:24:07 2025
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
    { 10.4372272F, -0.414435655F, 0.0716963261F },

    { 1.06889939F, 0.00264356309F, -0.00201464887F, 0.00264356309F, 0.960914969F,
      -0.0233825091F, -0.00201464887F, -0.0233825091F, 0.974173903F }
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
