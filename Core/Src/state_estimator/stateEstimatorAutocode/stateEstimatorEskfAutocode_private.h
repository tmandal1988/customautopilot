//
// File: stateEstimatorEskfAutocode_private.h
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 1.47
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Jul 26 11:57:41 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. ROM efficiency
// Validation result: Not run
//
#ifndef stateEstimatorEskfAutocode_private_h_
#define stateEstimatorEskfAutocode_private_h_
#include "rtwtypes.h"
#include "stateEstimatorEskfAutocode_types.h"

extern const real32_T rtCP_pooled_TlazyOibR04I[361];
extern const real32_T rtCP_pooled_W4igRJnX17mZ[361];
extern const real32_T rtCP_pooled_60ajHrPObCBy[225];
extern const real32_T rtCP_pooled_FQUpkqF8e3zU;

#define rtCP_processNoiseQ_Value       rtCP_pooled_TlazyOibR04I  // Expression: processNoiseQ
                                                                 //  Referenced by: '<Root>/processNoiseQ'

#define rtCP_initCovP_Value            rtCP_pooled_W4igRJnX17mZ  // Expression: initCovP
                                                                 //  Referenced by: '<Root>/initCovP'

#define rtCP_measNoiseR_Value          rtCP_pooled_60ajHrPObCBy  // Expression: measNoiseR
                                                                 //  Referenced by: '<Root>/measNoiseR'

#define rtCP_gEarth_mps2_Value         rtCP_pooled_FQUpkqF8e3zU  // Computed Parameter: rtCP_gEarth_mps2_Value
                                                                 //  Referenced by: '<Root>/gEarth_mps2'

#endif                                 // stateEstimatorEskfAutocode_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
