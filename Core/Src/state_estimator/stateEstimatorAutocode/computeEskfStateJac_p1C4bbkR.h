//
// File: computeEskfStateJac_p1C4bbkR.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.56
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Jun 13 14:23:38 2025
//
#ifndef RTW_HEADER_computeEskfStateJac_p1C4bbkR_h_
#define RTW_HEADER_computeEskfStateJac_p1C4bbkR_h_
#include "rtwtypes.h"

extern void computeEskfStateJac_p1C4bbkR(const real32_T states[20], const
  real32_T dcmBodyToNed[9], const real32_T bodyAccels_mps2[3], real32_T dAng,
  const real32_T dAngUnitVec[3], boolean_T noGps, real32_T b_sampleTime_s,
  real32_T stateJac[46]);

#endif                            // RTW_HEADER_computeEskfStateJac_p1C4bbkR_h_

//
// File trailer for generated code.
//
// [EOF]
//
