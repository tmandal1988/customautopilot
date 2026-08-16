//
// File: computeEskfStateJac_tpF0ZWTA.h
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.84
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:15 2026
//
#ifndef computeEskfStateJac_tpF0ZWTA_h_
#define computeEskfStateJac_tpF0ZWTA_h_
#include "rtwtypes.h"
#include "multiword_types.h"

extern void computeEskfStateJac_tpF0ZWTA(const real32_T states[20], const
  real32_T dcmBodyToNed[9], const real32_T bodyAccels_mps2[3], real32_T dAng,
  const real32_T dAngUnitVec[3], boolean_T noGps, boolean_T isOfValid, real32_T
  b_sampleTime_s, real32_T stateJac[46]);

#endif                                 // computeEskfStateJac_tpF0ZWTA_h_

//
// File trailer for generated code.
//
// [EOF]
//
