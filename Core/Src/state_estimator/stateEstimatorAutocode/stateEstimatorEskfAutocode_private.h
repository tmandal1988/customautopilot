//
// File: stateEstimatorEskfAutocode_private.h
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
#ifndef RTW_HEADER_stateEstimatorEskfAutocode_private_h_
#define RTW_HEADER_stateEstimatorEskfAutocode_private_h_
#include "rtwtypes.h"

extern const real32_T rtCP_pooled_3yPn1d1EvTvF[361];
extern const real32_T rtCP_pooled_BRlAKMBTYMAX[361];
extern const real32_T rtCP_pooled_6CJQsolHd81h[196];
extern const real32_T rtCP_pooled_FQUpkqF8e3zU;

#define rtCP_processNoiseQ_Value       rtCP_pooled_3yPn1d1EvTvF  // Expression: processNoiseQ
                                                                 //  Referenced by: '<Root>/processNoiseQ'

#define rtCP_initCovP_Value            rtCP_pooled_BRlAKMBTYMAX  // Expression: initCovP
                                                                 //  Referenced by: '<Root>/initCovP'

#define rtCP_measNoiseR_Value          rtCP_pooled_6CJQsolHd81h  // Expression: measNoiseR
                                                                 //  Referenced by: '<Root>/measNoiseR'

#define rtCP_gEarth_mps2_Value         rtCP_pooled_FQUpkqF8e3zU  // Computed Parameter: rtCP_gEarth_mps2_Value
                                                                 //  Referenced by: '<Root>/gEarth_mps2'

#endif                      // RTW_HEADER_stateEstimatorEskfAutocode_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
