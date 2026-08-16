//
// File: MultiWordSub.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.81
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 10:34:06 2026
//
#include "MultiWordSub.h"
#include "rtwtypes.h"

void MultiWordSub(const uint32_T u1[], const uint32_T u2[], uint32_T y[],
                  int32_T n)
{
  uint32_T borrow{ 0U };

  for (int32_T i{0}; i < n; i++) {
    uint32_T u1i;
    uint32_T yi;
    u1i = u1[i];
    yi = (u1i - u2[i]) - borrow;
    y[i] = yi;
    borrow = borrow != 0U ? static_cast<uint32_T>(yi >= u1i) : static_cast<
      uint32_T>(yi > u1i);
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
