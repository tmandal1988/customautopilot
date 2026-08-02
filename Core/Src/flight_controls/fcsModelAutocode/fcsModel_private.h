//
// File: fcsModel_private.h
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 7.9
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Fri Jul 31 11:50:47 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#ifndef fcsModel_private_h_
#define fcsModel_private_h_
#include "rtwtypes.h"
#include "fcsModel_types.h"
#include "fcsModel.h"

extern real_T rt_urand_Upu32_Yd_f_pw(uint32_T *u);
extern real_T rt_nrand_Upu32_Yd_f_pw(uint32_T *u);
extern uint32_T plook_bincpag(real_T u, const real_T bp[], uint32_T maxIndex,
  real_T *fraction, uint32_T *prevIndex);
extern real_T intrp1d_la(uint32_T bpIndex, real_T frac, const real_T table[],
  uint32_T maxIndex);
extern uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T
  startIndex, uint32_T maxIndex);

#endif                                 // fcsModel_private_h_

//
// File trailer for generated code.
//
// [EOF]
//
