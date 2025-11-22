//
// File: stateEstimatorEskfAutocode_private.h
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
#ifndef RTW_HEADER_stateEstimatorEskfAutocode_private_h_
#define RTW_HEADER_stateEstimatorEskfAutocode_private_h_
#include "rtwtypes.h"

extern const real32_T rtCP_pooled_nEm8aofMDVqC[361];
extern const real32_T rtCP_pooled_W4igRJnX17mZ[361];
extern const real32_T rtCP_pooled_AAYZ0pAWiZJ2[225];
extern const real32_T rtCP_pooled_FQUpkqF8e3zU;

#define rtCP_processNoiseQ_Value       rtCP_pooled_nEm8aofMDVqC  // Expression: processNoiseQ
                                                                 //  Referenced by: '<Root>/processNoiseQ'

#define rtCP_initCovP_Value            rtCP_pooled_W4igRJnX17mZ  // Expression: initCovP
                                                                 //  Referenced by: '<Root>/initCovP'

#define rtCP_measNoiseR_Value          rtCP_pooled_AAYZ0pAWiZJ2  // Expression: measNoiseR
                                                                 //  Referenced by: '<Root>/measNoiseR'

#define rtCP_gEarth_mps2_Value         rtCP_pooled_FQUpkqF8e3zU  // Computed Parameter: rtCP_gEarth_mps2_Value
                                                                 //  Referenced by: '<Root>/gEarth_mps2'

#endif                      // RTW_HEADER_stateEstimatorEskfAutocode_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
