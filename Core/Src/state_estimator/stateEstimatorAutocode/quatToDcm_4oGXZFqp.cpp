//
// File: quatToDcm_4oGXZFqp.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "quatToDcm_4oGXZFqp.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function C_ned2b = quatToDcm(q0, q1, q2, q3)
//  Inputs: q0, q1, q2, q3 (scalar floats)
//  Output: C_ned2b (3x3 direction cosine matrix)
//
void quatToDcm_4oGXZFqp(real32_T q0, real32_T q1, real32_T q2, real32_T q3,
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
  // 'errorStateEkf_function2:435' q1q1 = q1 * q1;
  q1q1 = q1 * q1;

  // 'errorStateEkf_function2:436' q2q2 = q2 * q2;
  q2q2 = q2 * q2;

  // 'errorStateEkf_function2:437' q3q3 = q3 * q3;
  q3q3 = q3 * q3;

  // 'errorStateEkf_function2:439' q0q1 = q0 * q1;
  q0q1 = q0 * q1;

  // 'errorStateEkf_function2:440' q0q2 = q0 * q2;
  q0q2 = q0 * q2;

  // 'errorStateEkf_function2:441' q0q3 = q0 * q3;
  q0q3 = q0 * q3;

  // 'errorStateEkf_function2:443' q1q2 = q1 * q2;
  q1q2 = q1 * q2;

  // 'errorStateEkf_function2:444' q1q3 = q1 * q3;
  q1q3 = q1 * q3;

  // 'errorStateEkf_function2:446' q2q3 = q2 * q3;
  q2q3 = q2 * q3;

  //  Initialize matrix
  // 'errorStateEkf_function2:449' C_ned2b = zeros(3,3);
  //  Fill in matrix elements
  // 'errorStateEkf_function2:452' C_ned2b(1,1) = 1 - 2 * (q2q2 + q3q3);
  C_ned2b[0] = 1.0F - (q2q2 + q3q3) * 2.0F;

  // 'errorStateEkf_function2:453' C_ned2b(1,2) = 2 * (q1q2 + q0q3);
  C_ned2b[3] = (q1q2 + q0q3) * 2.0F;

  // 'errorStateEkf_function2:454' C_ned2b(1,3) = 2 * (q1q3 - q0q2);
  C_ned2b[6] = (q1q3 - q0q2) * 2.0F;

  // 'errorStateEkf_function2:456' C_ned2b(2,1) = 2 * (q1q2 - q0q3);
  C_ned2b[1] = (q1q2 - q0q3) * 2.0F;

  // 'errorStateEkf_function2:457' C_ned2b(2,2) = 1 - 2 * (q1q1 + q3q3);
  C_ned2b[4] = 1.0F - (q1q1 + q3q3) * 2.0F;

  // 'errorStateEkf_function2:458' C_ned2b(2,3) = 2 * (q2q3 + q0q1);
  C_ned2b[7] = (q2q3 + q0q1) * 2.0F;

  // 'errorStateEkf_function2:460' C_ned2b(3,1) = 2 * (q1q3 + q0q2);
  C_ned2b[2] = (q1q3 + q0q2) * 2.0F;

  // 'errorStateEkf_function2:461' C_ned2b(3,2) = 2 * (q2q3 - q0q1);
  C_ned2b[5] = (q2q3 - q0q1) * 2.0F;

  // 'errorStateEkf_function2:462' C_ned2b(3,3) = 1 - 2 * (q1q1 + q2q2);
  C_ned2b[8] = 1.0F - (q1q1 + q2q2) * 2.0F;
}

//
// File trailer for generated code.
//
// [EOF]
//
