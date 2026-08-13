//
// File: uMultiWord2MultiWord.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.53
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Aug 13 15:28:24 2026
//
#include "uMultiWord2MultiWord.h"
#include "rtwtypes.h"

void uMultiWord2MultiWord(const uint32_T u1[], int32_T n1, uint32_T y[], int32_T
  n)
{
  int32_T nm;
  nm = n1 < n ? n1 : n;
  for (int32_T i{0}; i < nm; i++) {
    y[i] = u1[i];
  }

  if (n > n1) {
    for (int32_T i{nm}; i < n; i++) {
      y[i] = 0U;
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
