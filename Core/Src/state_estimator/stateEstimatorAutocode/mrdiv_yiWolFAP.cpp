//
// File: mrdiv_yiWolFAP.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "mrdiv_yiWolFAP.h"
#include <cmath>

// Function for MATLAB Function: '<S1>/EKF'
void mrdiv_yiWolFAP(const real32_T A[57], const real32_T B[9], real32_T Y[57])
{
  int32_T r1;
  int32_T r2;
  int32_T r3;
  int32_T rtemp;
  real32_T b_A[9];
  real32_T a21;
  real32_T maxval;
  b_A[0] = B[0];
  b_A[1] = B[1];
  b_A[2] = B[2];
  b_A[3] = B[3];
  b_A[4] = B[4];
  b_A[5] = B[5];
  b_A[6] = B[6];
  b_A[7] = B[7];
  b_A[8] = B[8];
  r1 = 0;
  r2 = 1;
  r3 = 2;
  maxval = std::abs(B[0]);
  a21 = std::abs(B[1]);
  if (a21 > maxval) {
    maxval = a21;
    r1 = 1;
    r2 = 0;
  }

  if (std::abs(B[2]) > maxval) {
    r1 = 2;
    r2 = 1;
    r3 = 0;
  }

  b_A[r2] = B[r2] / B[r1];
  b_A[r3] /= b_A[r1];
  b_A[r2 + 3] -= b_A[r1 + 3] * b_A[r2];
  b_A[r3 + 3] -= b_A[r1 + 3] * b_A[r3];
  b_A[r2 + 6] -= b_A[r1 + 6] * b_A[r2];
  b_A[r3 + 6] -= b_A[r1 + 6] * b_A[r3];
  if (std::abs(b_A[r3 + 3]) > std::abs(b_A[r2 + 3])) {
    rtemp = r2;
    r2 = r3;
    r3 = rtemp;
  }

  b_A[r3 + 3] /= b_A[r2 + 3];
  b_A[r3 + 6] -= b_A[r3 + 3] * b_A[r2 + 6];
  for (rtemp = 0; rtemp < 19; rtemp++) {
    int32_T Y_tmp;
    int32_T Y_tmp_0;
    int32_T Y_tmp_1;
    Y_tmp = 19 * r1 + rtemp;
    Y[Y_tmp] = A[rtemp] / b_A[r1];
    Y_tmp_0 = 19 * r2 + rtemp;
    Y[Y_tmp_0] = A[rtemp + 19] - b_A[r1 + 3] * Y[Y_tmp];
    Y_tmp_1 = 19 * r3 + rtemp;
    Y[Y_tmp_1] = A[rtemp + 38] - b_A[r1 + 6] * Y[Y_tmp];
    Y[Y_tmp_0] /= b_A[r2 + 3];
    Y[Y_tmp_1] -= b_A[r2 + 6] * Y[Y_tmp_0];
    Y[Y_tmp_1] /= b_A[r3 + 6];
    Y[Y_tmp_0] -= b_A[r3 + 3] * Y[Y_tmp_1];
    Y[Y_tmp] -= Y[Y_tmp_1] * b_A[r3];
    Y[Y_tmp] -= Y[Y_tmp_0] * b_A[r2];
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
