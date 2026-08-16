//
// File: updateQuatAndResetCovP_o483nOwE.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.84
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 16:27:15 2026
//
#include "rtwtypes.h"
#include "updateQuatAndResetCovP_o483nOwE.h"
#include "norm_94qjDDKI.h"
#include <cmath>
#include "quatMultiply_UkcBdhzN.h"
#include "norm_NoMIKEmk.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function [nomQuat, covP] = updateQuatAndResetCovP(nomQuat, angErr, covP)
// Construct quaternion from the rotation vector
//
void updateQuatAndResetCovP_o483nOwE(real32_T nomQuat[4], const real32_T angErr
  [3], real32_T covP[361])
{
  int32_T i;
  int32_T i_0;
  int32_T i_1;
  real32_T angG[9];
  real32_T angG_0[9];
  real32_T nomQuat_0[4];
  real32_T tmp[4];
  real32_T angG_1;
  real32_T angG_2;
  real32_T angG_3;
  real32_T angG_4;
  real32_T b;
  real32_T b_tmp;
  real32_T covP_0;
  real32_T covP_1;
  real32_T covP_2;
  real32_T dAng;
  real32_T v_idx_0;
  real32_T v_idx_1;
  real32_T v_idx_2;

  // 'errorStateEkf_function2:470' dAng = norm(angErr);
  dAng = norm_94qjDDKI(angErr);

  // 'errorStateEkf_function2:471' if(dAng > 1e-7)
  if (dAng > 1.0E-7) {
    // 'errorStateEkf_function2:472' du = angErr/dAng;
    // 'errorStateEkf_function2:473' qError = [cos(dAng*0.5); du*sin(dAng*0.5)]; 
    b_tmp = dAng * 0.5F;
    b = std::sin(b_tmp);

    // 'errorStateEkf_function2:475' nomQuat = quatMultiply(nomQuat, qError);
    tmp[0] = std::cos(b_tmp);

    // 'errorStateEkf_function2:476' angG = eye(3, 'single') - skew3(angErr*0.5); 
    tmp[1] = angErr[0] / dAng * b;
    v_idx_0 = angErr[0] * 0.5F;
    tmp[2] = angErr[1] / dAng * b;
    v_idx_1 = angErr[1] * 0.5F;
    tmp[3] = angErr[2] / dAng * b;
    v_idx_2 = angErr[2] * 0.5F;
    for (i_1 = 0; i_1 < 4; i_1++) {
      nomQuat_0[i_1] = nomQuat[i_1];
    }

    quatMultiply_UkcBdhzN(nomQuat_0, tmp, nomQuat);

    //  Optimized skew-symmetric matrix from 3x1 vector
    //  Input v must be [3x1] single real vector
    // 'skew3:6' assert(isa(v, 'single') && isreal(v) && all(size(v) == [3 1])); 
    // 'skew3:8' S = single([  0,    -v(3),  v(2);
    // 'skew3:9'              v(3),   0,    -v(1);
    // 'skew3:10'             -v(2),  v(1),   0 ]);
    // 'errorStateEkf_function2:477' covP(1:3, 1:3) = angG*covP(1:3, 1:3)*angG'; 
    angG[0] = 1.0F;
    angG[1] = 0.0F - v_idx_2;
    angG[2] = 0.0F - (-v_idx_1);
    angG_0[0] = 0.0F;
    angG_0[1] = 0.0F;
    angG_0[2] = 0.0F;
    angG[3] = 0.0F - (-v_idx_2);
    angG[4] = 1.0F;
    angG[5] = 0.0F - v_idx_0;
    angG_0[3] = 0.0F;
    angG_0[4] = 0.0F;
    angG_0[5] = 0.0F;
    angG[6] = 0.0F - v_idx_1;
    angG[7] = 0.0F - (-v_idx_0);
    angG[8] = 1.0F;
    angG_0[6] = 0.0F;
    angG_0[7] = 0.0F;
    angG_0[8] = 0.0F;
    i_1 = 0;
    i_0 = 0;
    for (i = 0; i < 3; i++) {
      dAng = covP[i_1];
      b = angG_0[i_0] + dAng;
      b_tmp = (0.0F - v_idx_2) * dAng + angG_0[i_0 + 1];
      angG_4 = (0.0F - (-v_idx_1)) * dAng + angG_0[i_0 + 2];
      covP[i_1] = 0.0F;
      dAng = covP[i_1 + 1];
      b += (0.0F - (-v_idx_2)) * dAng;
      b_tmp += dAng;
      angG_4 += (0.0F - v_idx_0) * dAng;
      covP[i_1 + 1] = 0.0F;
      dAng = covP[i_1 + 2];
      angG_0[i_0] = (0.0F - v_idx_1) * dAng + b;
      angG_0[i_0 + 1] = (0.0F - (-v_idx_0)) * dAng + b_tmp;
      angG_0[i_0 + 2] = angG_4 + dAng;
      covP[i_1 + 2] = 0.0F;
      i_1 += 19;
      i_0 += 3;
    }

    b = angG_0[0];
    b_tmp = angG_0[1];
    angG_4 = angG_0[2];
    v_idx_0 = angG_0[3];
    v_idx_1 = angG_0[4];
    v_idx_2 = angG_0[5];
    angG_1 = angG_0[6];
    angG_2 = angG_0[7];
    angG_3 = angG_0[8];
    i_1 = 0;
    for (i_0 = 0; i_0 < 3; i_0++) {
      dAng = angG[i_0];
      covP_0 = b * dAng + covP[i_1];
      covP_1 = covP[i_1 + 1] + b_tmp * dAng;
      covP_2 = covP[i_1 + 2] + angG_4 * dAng;
      dAng = angG[i_0 + 3];
      covP_0 += v_idx_0 * dAng;
      covP_1 += v_idx_1 * dAng;
      covP_2 += v_idx_2 * dAng;
      dAng = angG[i_0 + 6];
      covP[i_1] = angG_1 * dAng + covP_0;
      covP[i_1 + 1] = angG_2 * dAng + covP_1;
      covP[i_1 + 2] = angG_3 * dAng + covP_2;
      i_1 += 19;
    }

    // 'errorStateEkf_function2:479' nQuat = norm(nomQuat);
    dAng = norm_NoMIKEmk(nomQuat);

    // 'errorStateEkf_function2:480' if(nQuat > 1e-7)
    if (dAng > 1.0E-7) {
      // Normalize the quaternion
      // 'errorStateEkf_function2:482' nomQuat = nomQuat/nQuat;
      nomQuat[0] /= dAng;
      nomQuat[1] /= dAng;
      nomQuat[2] /= dAng;
      nomQuat[3] /= dAng;
    } else {
      // 'errorStateEkf_function2:483' else
      // 'errorStateEkf_function2:484' nomQuat = single([1; 0; 0; 0]);
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
