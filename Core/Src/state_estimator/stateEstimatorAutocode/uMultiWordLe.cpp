//
// File: uMultiWordLe.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.70
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sat Aug 15 22:52:20 2026
//
#include "uMultiWordLe.h"
#include "uMultiWordCmp.h"
#include "rtwtypes.h"

boolean_T uMultiWordLe(const uint32_T u1[], const uint32_T u2[], int32_T n)
{
  return uMultiWordCmp(u1, u2, n) <= 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
