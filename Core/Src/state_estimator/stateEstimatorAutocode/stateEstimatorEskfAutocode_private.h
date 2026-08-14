//
// File: stateEstimatorEskfAutocode_private.h
//
// Code generated for Simulink model 'stateEstimatorEskfAutocode'.
//
// Model version                  : 7.0
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Fri Aug 14 07:31:47 2026
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
#include "multiword_types.h"
#include "stateEstimatorEskfAutocode_types.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized ulong/long. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

extern const real32_T rtCP_pooled_TlazyOibR04I[361];
extern const real32_T rtCP_pooled_55ctUJ7jqF6j[361];
extern const real32_T rtCP_pooled_JLh1DCiSieNR[225];
extern const real32_T rtCP_pooled_FQUpkqF8e3zU;

#define rtCP_processNoiseQ_Value       rtCP_pooled_TlazyOibR04I  // Expression: processNoiseQ
                                                                 //  Referenced by: '<Root>/processNoiseQ'

#define rtCP_initCovP_Value            rtCP_pooled_55ctUJ7jqF6j  // Expression: initCovP
                                                                 //  Referenced by: '<Root>/initCovP'

#define rtCP_measNoiseR_Value          rtCP_pooled_JLh1DCiSieNR  // Expression: measNoiseR
                                                                 //  Referenced by: '<Root>/measNoiseR'

#define rtCP_gEarth_mps2_Value         rtCP_pooled_FQUpkqF8e3zU  // Computed Parameter: rtCP_gEarth_mps2_Value
                                                                 //  Referenced by: '<Root>/gEarth_mps2'

#endif                                 // stateEstimatorEskfAutocode_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
