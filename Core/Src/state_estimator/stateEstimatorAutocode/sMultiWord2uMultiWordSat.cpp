//
// File: sMultiWord2uMultiWordSat.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.84
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:15 2026
//
#include "sMultiWord2uMultiWordSat.h"
#include "rtwtypes.h"

void sMultiWord2uMultiWordSat(const uint32_T u1[], int32_T n1, uint32_T y[],
  int32_T n)
{
  int32_T i;
  uint32_T ys;
  boolean_T doSaturation;
  ys = (u1[n1 - 1] & 2147483648U) != 0U ? MAX_uint32_T : 0U;
  doSaturation = (ys != 0U);
  i = n1 - 1;
  while ((!doSaturation) && (i >= n)) {
    doSaturation = (u1[i] != 0U);
    i--;
  }

  if (doSaturation) {
    ys = ~ys;
    for (i = 0; i < n; i++) {
      y[i] = ys;
    }
  } else {
    int32_T nc;
    nc = n1 < n ? n1 : n;
    for (i = 0; i < nc; i++) {
      y[i] = u1[i];
    }

    while (i < n) {
      y[i] = ys;
      i++;
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
