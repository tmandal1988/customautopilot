//
// File: quatMultiply_AJk9aby2.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.48
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Thu May  1 12:28:28 2025
//
#include "rtwtypes.h"
#include "quatMultiply_AJk9aby2.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function r = quatMultiply(p, q)
//  Optimized Quaternion Multiplication - scalar first [w; x; y; z]
//
void quatMultiply_AJk9aby2(const real32_T p[4], const real32_T q[4], real32_T r
  [4])
{
  // 'quatMultiply:5' assert(isreal(p) && all(size(p) == [4,1]));
  // 'quatMultiply:6' assert(isreal(q) && all(size(q) == [4,1]));
  //  Unpack p and q
  // 'quatMultiply:9' pw = p(1);
  // 'quatMultiply:9' px = p(2);
  // 'quatMultiply:9' py = p(3);
  // 'quatMultiply:9' pz = p(4);
  // 'quatMultiply:10' qw = q(1);
  // 'quatMultiply:10' qx = q(2);
  // 'quatMultiply:10' qy = q(3);
  // 'quatMultiply:10' qz = q(4);
  //  Manual multiplication
  // 'quatMultiply:13' rw = pw*qw - px*qx - py*qy - pz*qz;
  // 'quatMultiply:14' rx = pw*qx + px*qw + py*qz - pz*qy;
  // 'quatMultiply:15' ry = pw*qy - px*qz + py*qw + pz*qx;
  // 'quatMultiply:16' rz = pw*qz + px*qy - py*qx + pz*qw;
  //  Pack result
  // 'quatMultiply:19' r = [rw; rx; ry; rz];
  r[0] = ((p[0] * q[0] - p[1] * q[1]) - p[2] * q[2]) - p[3] * q[3];
  r[1] = ((p[0] * q[1] + q[0] * p[1]) + p[2] * q[3]) - q[2] * p[3];
  r[2] = ((p[0] * q[2] - p[1] * q[3]) + q[0] * p[2]) + q[1] * p[3];
  r[3] = ((p[0] * q[3] + p[1] * q[2]) - q[1] * p[2]) + q[0] * p[3];
}

//
// File trailer for generated code.
//
// [EOF]
//
