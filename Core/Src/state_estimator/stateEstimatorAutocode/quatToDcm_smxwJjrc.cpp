//
// File: quatToDcm_smxwJjrc.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.70
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 07:54:31 2026
//
#include "rtwtypes.h"
#include "quatToDcm_smxwJjrc.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function C_ned2b = quatToDcm(q0, q1, q2, q3)
//  Inputs: q0, q1, q2, q3 (scalar floats)
//  Output: C_ned2b (3x3 direction cosine matrix)
//
void quatToDcm_smxwJjrc(real32_T q0, real32_T q1, real32_T q2, real32_T q3,
  real_T C_ned2b[9])
{
  real32_T q0q1;
  real32_T q0q2;
  real32_T q0q3;
  real32_T q1q1;
  real32_T q1q2;
  real32_T q1q3;
  real32_T q2q2;
  real32_T q2q3;
  real32_T q3q3;

  //  Precompute reused terms
  // 'errorStateEkf_function2:494' q1q1 = q1 * q1;
  q1q1 = q1 * q1;

  // 'errorStateEkf_function2:495' q2q2 = q2 * q2;
  q2q2 = q2 * q2;

  // 'errorStateEkf_function2:496' q3q3 = q3 * q3;
  q3q3 = q3 * q3;

  // 'errorStateEkf_function2:498' q0q1 = q0 * q1;
  q0q1 = q0 * q1;

  // 'errorStateEkf_function2:499' q0q2 = q0 * q2;
  q0q2 = q0 * q2;

  // 'errorStateEkf_function2:500' q0q3 = q0 * q3;
  q0q3 = q0 * q3;

  // 'errorStateEkf_function2:502' q1q2 = q1 * q2;
  q1q2 = q1 * q2;

  // 'errorStateEkf_function2:503' q1q3 = q1 * q3;
  q1q3 = q1 * q3;

  // 'errorStateEkf_function2:505' q2q3 = q2 * q3;
  q2q3 = q2 * q3;

  //  Initialize matrix
  // 'errorStateEkf_function2:508' C_ned2b = zeros(3,3);
  //  Fill in matrix elements
  // 'errorStateEkf_function2:511' C_ned2b(1,1) = 1 - 2 * (q2q2 + q3q3);
  C_ned2b[0] = 1.0F - (q2q2 + q3q3) * 2.0F;

  // 'errorStateEkf_function2:512' C_ned2b(1,2) = 2 * (q1q2 + q0q3);
  C_ned2b[3] = (q1q2 + q0q3) * 2.0F;

  // 'errorStateEkf_function2:513' C_ned2b(1,3) = 2 * (q1q3 - q0q2);
  C_ned2b[6] = (q1q3 - q0q2) * 2.0F;

  // 'errorStateEkf_function2:515' C_ned2b(2,1) = 2 * (q1q2 - q0q3);
  C_ned2b[1] = (q1q2 - q0q3) * 2.0F;

  // 'errorStateEkf_function2:516' C_ned2b(2,2) = 1 - 2 * (q1q1 + q3q3);
  C_ned2b[4] = 1.0F - (q1q1 + q3q3) * 2.0F;

  // 'errorStateEkf_function2:517' C_ned2b(2,3) = 2 * (q2q3 + q0q1);
  C_ned2b[7] = (q2q3 + q0q1) * 2.0F;

  // 'errorStateEkf_function2:519' C_ned2b(3,1) = 2 * (q1q3 + q0q2);
  C_ned2b[2] = (q1q3 + q0q2) * 2.0F;

  // 'errorStateEkf_function2:520' C_ned2b(3,2) = 2 * (q2q3 - q0q1);
  C_ned2b[5] = (q2q3 - q0q1) * 2.0F;

  // 'errorStateEkf_function2:521' C_ned2b(3,3) = 1 - 2 * (q1q1 + q2q2);
  C_ned2b[8] = 1.0F - (q1q1 + q2q2) * 2.0F;
}

//
// File trailer for generated code.
//
// [EOF]
//
