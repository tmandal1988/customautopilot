//
// File: computeEskfAccelMeasJa_g9O3pYCr.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "computeEskfAccelMeasJa_g9O3pYCr.h"
#include <cstring>

//
// Function for MATLAB Function: '<S1>/EKF'
// function accelMeasJac = computeEskfAccelMeasJac(states, gEarth_mps2)
// COMPUTEESKFACCELMEASJAC Computes Meas Jacobian for accelerometer
// measurements
//
// Inputs:
// states:                EKF states
// gEarth_mps2:           Local accel due to gravity
//
// Outputs:
// accelaccelMeasJac:            3x16 Accel Meas Jacobian
//
void computeEskfAccelMeasJa_g9O3pYCr(const real32_T states[20], real32_T
  gEarth_mps2, real32_T accelMeasJac[60])
{
  real32_T tmp1;
  real32_T tmp2;
  real32_T tmp3;
  real32_T tmp4;
  real32_T tmp5;

  // Initialize the Meas jacobian to zero
  // 'computeEskfAccelMeasJac:13' accelMeasJac = zeros(3, 20, 'single');
  std::memset(&accelMeasJac[0], 0, 60U * sizeof(real32_T));

  // Extract quat states
  // 'computeEskfAccelMeasJac:16' states(1) = states(1);
  // 'computeEskfAccelMeasJac:17' states(2) = states(2);
  // 'computeEskfAccelMeasJac:18' states(3) = states(3);
  // 'computeEskfAccelMeasJac:19' states(4) = states(4);
  // 'computeEskfAccelMeasJac:21' tmp1 = gEarth_mps2*2*states(3);
  tmp1 = gEarth_mps2 * 2.0F * states[2];

  // 'computeEskfAccelMeasJac:22' tmp2 = -2*gEarth_mps2*states(4);
  tmp2 = -2.0F * gEarth_mps2 * states[3];

  // 'computeEskfAccelMeasJac:23' tmp3 = gEarth_mps2*2*states(1);
  tmp3 = gEarth_mps2 * 2.0F * states[0];

  // 'computeEskfAccelMeasJac:24' tmp4 = -gEarth_mps2*2*states(2);
  tmp4 = -gEarth_mps2 * 2.0F * states[1];

  // 'computeEskfAccelMeasJac:25' tmp5 = 4*gEarth_mps2;
  tmp5 = 4.0F * gEarth_mps2;

  // 'computeEskfAccelMeasJac:27' accelMeasJac(1, 1) = tmp1;
  accelMeasJac[0] = tmp1;

  // 'computeEskfAccelMeasJac:28' accelMeasJac(1, 2) = tmp2;
  accelMeasJac[3] = tmp2;

  // 'computeEskfAccelMeasJac:29' accelMeasJac(1, 3) = tmp3;
  accelMeasJac[6] = tmp3;

  // 'computeEskfAccelMeasJac:30' accelMeasJac(1, 4) = tmp4;
  accelMeasJac[9] = tmp4;

  // 'computeEskfAccelMeasJac:31' accelMeasJac(1, 14) = 1;
  accelMeasJac[39] = 1.0F;

  // 'computeEskfAccelMeasJac:33' accelMeasJac(2, 1) = tmp4;
  accelMeasJac[1] = tmp4;

  // 'computeEskfAccelMeasJac:34' accelMeasJac(2, 2) = -tmp3;
  accelMeasJac[4] = -tmp3;

  // 'computeEskfAccelMeasJac:35' accelMeasJac(2, 3) = tmp2;
  accelMeasJac[7] = tmp2;

  // 'computeEskfAccelMeasJac:36' accelMeasJac(2, 4) = -tmp1;
  accelMeasJac[10] = -tmp1;

  // 'computeEskfAccelMeasJac:37' accelMeasJac(2, 15) = 1;
  accelMeasJac[43] = 1.0F;

  // 'computeEskfAccelMeasJac:39' accelMeasJac(3, 2) = states(2)*tmp5;
  accelMeasJac[5] = states[1] * tmp5;

  // 'computeEskfAccelMeasJac:40' accelMeasJac(3, 3) = states(3)*tmp5;
  accelMeasJac[8] = states[2] * tmp5;

  // 'computeEskfAccelMeasJac:41' accelMeasJac(3, 16) = 1;
  accelMeasJac[47] = 1.0F;
}

//
// File trailer for generated code.
//
// [EOF]
//
