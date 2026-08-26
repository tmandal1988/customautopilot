//
// File: computEskfMagMeasJac_ecmRY7bq.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.88
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 23 09:38:07 2026
//
#include "rtwtypes.h"
#include "computEskfMagMeasJac_ecmRY7bq.h"
#include <cstring>

//
// Function for MATLAB Function: '<S1>/EKF'
// function  measJac = computEskfMagMeasJac(states, localNedUnitMag)
// COMPUTEESKFMAGMEASJAC Computes Mag Measurement Jacobian for ESKF
//
// Inputs:
// states:                         Nominal states
// localNedUnitMag:                Local NED Mag unit vector
//
// Outputs:
// measJac:                        Measurement Jacobian
//
void computEskfMagMeasJac_ecmRY7bq(const real32_T states[20], const real32_T
  localNedUnitMag[3], real32_T measJac[60])
{
  real32_T measJac_tmp;
  real32_T measJac_tmp_0;
  real32_T measJac_tmp_1;
  real32_T measJac_tmp_2;
  real32_T measJac_tmp_3;
  real32_T measJac_tmp_4;
  real32_T measJac_tmp_5;
  real32_T measJac_tmp_6;
  real32_T measJac_tmp_7;
  real32_T measJac_tmp_8;
  real32_T measJac_tmp_9;

  // Initialize the measJac to zero
  // 'computEskfMagMeasJac:12' measJac = zeros(3, 20, 'single');
  std::memset(&measJac[0], 0, 60U * sizeof(real32_T));

  // Temp Variables
  // 'computEskfMagMeasJac:15' q0 = states(1);
  // 'computEskfMagMeasJac:16' q1 = states(2);
  // 'computEskfMagMeasJac:17' q2 = states(3);
  // 'computEskfMagMeasJac:18' q3 = states(4);
  // 'computEskfMagMeasJac:20' magN = localNedUnitMag(1);
  // 'computEskfMagMeasJac:21' magE = localNedUnitMag(2);
  // 'computEskfMagMeasJac:22' magD = localNedUnitMag(3);
  // 'computEskfMagMeasJac:24' measJac(1, 1) = 2*magE*q3 - 2*magD*q2;
  measJac_tmp = 2.0F * localNedUnitMag[2];
  measJac_tmp_0 = 2.0F * localNedUnitMag[1];
  measJac_tmp_4 = measJac_tmp * states[2];
  measJac_tmp_8 = measJac_tmp_0 * states[3];
  measJac[0] = measJac_tmp_8 - measJac_tmp_4;

  // 'computEskfMagMeasJac:25' measJac(1, 2) = 2*magD*q3 + 2*magE*q2;
  measJac_tmp_3 = measJac_tmp * states[3];
  measJac_tmp_9 = measJac_tmp_0 * states[2];
  measJac[3] = measJac_tmp_3 + measJac_tmp_9;

  // 'computEskfMagMeasJac:26' measJac(1, 3) = 2*magE*q1 - 2*magD*q0 - 4*magN*q2; 
  measJac_tmp_1 = 4.0F * localNedUnitMag[0];
  measJac_tmp_2 = measJac_tmp * states[0];
  measJac_tmp_6 = measJac_tmp_0 * states[1];
  measJac[6] = (measJac_tmp_6 - measJac_tmp_2) - measJac_tmp_1 * states[2];

  // 'computEskfMagMeasJac:27' measJac(1, 4) = 2*magD*q1 + 2*magE*q0 - 4*magN*q3; 
  measJac_tmp *= states[1];
  measJac_tmp_0 *= states[0];
  measJac[9] = (measJac_tmp + measJac_tmp_0) - measJac_tmp_1 * states[3];

  // 'computEskfMagMeasJac:28' measJac(1, 17) = 1;
  measJac[48] = 1.0F;

  // 'computEskfMagMeasJac:29' measJac(2, 1) = 2*magD*q1 - 2*magN*q3;
  measJac_tmp_1 = 2.0F * localNedUnitMag[0];
  measJac_tmp_7 = measJac_tmp_1 * states[3];
  measJac[1] = measJac_tmp - measJac_tmp_7;

  // 'computEskfMagMeasJac:30' measJac(2, 2) = 2*magD*q0 - 4*magE*q1 + 2*magN*q2; 
  measJac_tmp = 4.0F * localNedUnitMag[1];
  measJac_tmp_5 = measJac_tmp_1 * states[2];
  measJac[4] = (measJac_tmp_2 - measJac_tmp * states[1]) + measJac_tmp_5;

  // 'computEskfMagMeasJac:31' measJac(2, 3) = 2*magD*q3 + 2*magN*q1;
  measJac_tmp_2 = measJac_tmp_1 * states[1];
  measJac[7] = measJac_tmp_3 + measJac_tmp_2;

  // 'computEskfMagMeasJac:32' measJac(2, 4) = 2*magD*q2 - 4*magE*q3 - 2*magN*q0; 
  measJac_tmp_3 = measJac_tmp_1 * states[0];
  measJac[10] = (measJac_tmp_4 - measJac_tmp * states[3]) - measJac_tmp_3;

  // 'computEskfMagMeasJac:33' measJac(2, 18) = 1;
  measJac[52] = 1.0F;

  // 'computEskfMagMeasJac:34' measJac(3, 1) = 2*magN*q2 - 2*magE*q1;
  measJac[2] = measJac_tmp_5 - measJac_tmp_6;

  // 'computEskfMagMeasJac:35' measJac(3, 2) = 2*magN*q3 - 2*magE*q0 - 4*magD*q1; 
  measJac_tmp = 4.0F * localNedUnitMag[2];
  measJac[5] = (measJac_tmp_7 - measJac_tmp_0) - measJac_tmp * states[1];

  // 'computEskfMagMeasJac:36' measJac(3, 3) = 2*magE*q3 - 4*magD*q2 + 2*magN*q0; 
  measJac[8] = (measJac_tmp_8 - measJac_tmp * states[2]) + measJac_tmp_3;

  // 'computEskfMagMeasJac:37' measJac(3, 4) = 2*magE*q2 + 2*magN*q1;
  measJac[11] = measJac_tmp_9 + measJac_tmp_2;

  // 'computEskfMagMeasJac:38' measJac(3, 19) = 1;
  measJac[56] = 1.0F;
}

//
// File trailer for generated code.
//
// [EOF]
//
