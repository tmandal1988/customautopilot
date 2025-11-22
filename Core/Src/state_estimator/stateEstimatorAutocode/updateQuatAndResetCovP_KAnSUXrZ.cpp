//
// File: updateQuatAndResetCovP_KAnSUXrZ.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "updateQuatAndResetCovP_KAnSUXrZ.h"
#include "norm_yrNKZSBO.h"
#include <cmath>
#include "quatMultiply_AJk9aby2.h"
#include "norm_7MzYkgry.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function [nomQuat, covP] = updateQuatAndResetCovP(nomQuat, angErr, covP)
// Construct quaternion from the rotation vector
//
void updateQuatAndResetCovP_KAnSUXrZ(real32_T nomQuat[4], const real32_T angErr
  [3], real32_T covP[361])
{
  real32_T angG[9];
  real32_T nomQuat_0[4];
  real32_T tmp[4];
  real32_T dAng;

  // 'errorStateEkf_function2:411' dAng = norm(angErr);
  dAng = norm_yrNKZSBO(angErr);

  // 'errorStateEkf_function2:412' if(dAng > 1e-7)
  if (dAng > 1.0E-7) {
    int32_T i_0;
    int32_T tmp_0;
    real32_T angG_tmp;
    real32_T b;
    real32_T v_idx_0;
    real32_T v_idx_1;

    // 'errorStateEkf_function2:413' du = angErr/dAng;
    // 'errorStateEkf_function2:414' qError = [cos(dAng*0.5); du*sin(dAng*0.5)]; 
    b = std::sin(dAng * 0.5F);

    // 'errorStateEkf_function2:416' nomQuat = quatMultiply(nomQuat, qError);
    tmp[0] = std::cos(dAng * 0.5F);

    // 'errorStateEkf_function2:417' angG = eye(3, 'single') - skew3(angErr*0.5); 
    tmp[1] = angErr[0] / dAng * b;
    v_idx_0 = angErr[0] * 0.5F;
    tmp[2] = angErr[1] / dAng * b;
    v_idx_1 = angErr[1] * 0.5F;
    tmp[3] = angErr[2] / dAng * b;
    dAng = angErr[2] * 0.5F;
    for (i_0 = 0; i_0 < 4; i_0++) {
      nomQuat_0[i_0] = nomQuat[i_0];
    }

    quatMultiply_AJk9aby2(nomQuat_0, tmp, nomQuat);

    //  Optimized skew-symmetric matrix from 3x1 vector
    //  Input v must be [3x1] single real vector
    // 'skew3:6' assert(isa(v, 'single') && isreal(v) && all(size(v) == [3 1])); 
    // 'skew3:8' S = single([  0,    -v(3),  v(2);
    // 'skew3:9'              v(3),   0,    -v(1);
    // 'skew3:10'             -v(2),  v(1),   0 ]);
    // 'errorStateEkf_function2:418' covP(1:3, 1:3) = angG*covP(1:3, 1:3)*angG'; 
    i_0 = 0;
    tmp_0 = 0;
    for (int32_T i{0}; i < 3; i++) {
      angG[i_0] = 0.0F;
      angG[i_0] += covP[tmp_0];
      b = covP[tmp_0 + 1];
      angG[i_0] += (0.0F - (-dAng)) * b;
      angG[i_0] += covP[tmp_0 + 2] * (0.0F - v_idx_1);
      angG[i_0 + 1] = 0.0F;
      angG[i_0 + 1] += (0.0F - dAng) * covP[tmp_0];
      angG[i_0 + 1] += b;
      angG_tmp = covP[tmp_0 + 2];
      angG[i_0 + 1] += (0.0F - (-v_idx_0)) * angG_tmp;
      angG[i_0 + 2] = 0.0F;
      angG[i_0 + 2] += (0.0F - (-v_idx_1)) * covP[tmp_0];
      angG[i_0 + 2] += (0.0F - v_idx_0) * b;
      angG[i_0 + 2] += angG_tmp;
      i_0 += 3;
      tmp_0 += 19;
    }

    for (i_0 = 0; i_0 < 3; i_0++) {
      covP[i_0] = 0.0F;
      covP[i_0] += angG[i_0];
      b = angG[i_0 + 3];
      covP[i_0] += (0.0F - (-dAng)) * b;
      angG_tmp = angG[i_0 + 6];
      covP[i_0] += (0.0F - v_idx_1) * angG_tmp;
      covP[i_0 + 19] = 0.0F;
      covP[i_0 + 19] += (0.0F - dAng) * angG[i_0];
      covP[i_0 + 19] += b;
      covP[i_0 + 19] += (0.0F - (-v_idx_0)) * angG_tmp;
      covP[i_0 + 38] = 0.0F;
      covP[i_0 + 38] += (0.0F - (-v_idx_1)) * angG[i_0];
      covP[i_0 + 38] += (0.0F - v_idx_0) * b;
      covP[i_0 + 38] += angG_tmp;
    }

    // 'errorStateEkf_function2:420' nQuat = norm(nomQuat);
    dAng = norm_7MzYkgry(nomQuat);

    // 'errorStateEkf_function2:421' if(nQuat > 1e-7)
    if (dAng > 1.0E-7) {
      // Normalize the quaternion
      // 'errorStateEkf_function2:423' nomQuat = nomQuat/nQuat;
      nomQuat[0] /= dAng;
      nomQuat[1] /= dAng;
      nomQuat[2] /= dAng;
      nomQuat[3] /= dAng;
    } else {
      // 'errorStateEkf_function2:424' else
      // 'errorStateEkf_function2:425' nomQuat = single([1; 0; 0; 0]);
      nomQuat[0] = 1.0F;
      nomQuat[1] = 0.0F;
      nomQuat[2] = 0.0F;
      nomQuat[3] = 0.0F;
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
