//
// File: applyGpsPosAndVelCorr_wtI6aFgH.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.53
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Thu Aug 13 15:28:24 2026
//
#include "rtwtypes.h"
#include "applyGpsPosAndVelCorr_wtI6aFgH.h"
#include <cstring>
#include "updateQuatAndResetCovP_o483nOwE.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, idx, idxEs, ...
//     idxEs2, idxNs2, measNoiseR, innovGate)
//
void applyGpsPosAndVelCorr_wtI6aFgH(real32_T states[20], const real32_T
  nedPosAndVel[6], real32_T covP[361], real_T idx, const real_T idxEs[19], const
  real_T idxEs2[16], const real_T idxNs2[16], const real32_T measNoiseR[225],
  real32_T innovGate)
{
  real_T idxEs_0;
  int32_T K_tmp[19];
  int32_T i;
  int32_T iS_tmp;
  real32_T covP_1[361];
  real32_T K[19];
  real32_T covP_0[19];
  real32_T errorStateHat[19];
  real32_T states_0[16];
  real32_T nomQuat[4];
  real32_T K_0;
  real32_T iS;
  real32_T nu;

  // 'errorStateEkf_function2:465' iS = 1/(covP(idx, idx) + measNoiseR(idx, idx)); 
  iS_tmp = (static_cast<int32_T>(idx) - 1) * 19;
  iS = 1.0F / (measNoiseR[((static_cast<int32_T>(idx) - 1) * 15 + static_cast<
    int32_T>(idx)) - 1] + covP[(iS_tmp + static_cast<int32_T>(idx)) - 1]);

  // 'errorStateEkf_function2:466' nu = nedPosAndVel(idx - 3)  - states(idx + 1); 
  nu = nedPosAndVel[static_cast<int32_T>(idx - 3.0) - 1] - states
    [static_cast<int32_T>(idx + 1.0) - 1];

  // 'errorStateEkf_function2:467' NIS = nu*nu*iS;
  // ErrorStateHat
  // 'errorStateEkf_function2:470' errorStateHat = zeros(19, 1, 'single');
  std::memset(&errorStateHat[0], 0, 19U * sizeof(real32_T));

  // 'errorStateEkf_function2:472' if NIS < innovGate
  if (nu * nu * iS < innovGate) {
    // 'errorStateEkf_function2:473' K = covP(idxEs, idx).*iS;
    // 'errorStateEkf_function2:474' errorStateHat(idxEs) = K*nu;
    // 'errorStateEkf_function2:476' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
    for (i = 0; i < 19; i++) {
      idxEs_0 = idxEs[i];
      K_tmp[i] = static_cast<int32_T>(idxEs_0);
      K_0 = covP[(iS_tmp + static_cast<int32_T>(idxEs_0)) - 1] * iS;
      K[i] = K_0;
      errorStateHat[static_cast<int32_T>(idxEs_0) - 1] = K_0 * nu;
      covP_0[i] = covP[((static_cast<int32_T>(idxEs_0) - 1) * 19 + static_cast<
                        int32_T>(idx)) - 1];
    }

    for (i = 0; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP_1[iS_tmp + 19 * i] = covP[((K_tmp[i] - 1) * 19 + K_tmp[iS_tmp]) - 1]
          - K[iS_tmp] * covP_0[i];
      }
    }

    for (i = 0; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP[(K_tmp[iS_tmp] + 19 * (K_tmp[i] - 1)) - 1] = covP_1[19 * i + iS_tmp];
      }
    }

    // Update the nominal state
    // 'errorStateEkf_function2:479' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
    for (i = 0; i < 16; i++) {
      states_0[i] = states[static_cast<int32_T>(idxNs2[i]) - 1] + errorStateHat[
        static_cast<int32_T>(idxEs2[i]) - 1];
    }

    for (i = 0; i < 16; i++) {
      states[static_cast<int32_T>(idxNs2[i]) - 1] = states_0[i];
    }

    // Construct quaternion from the rotation vector and reset covP
    // 'errorStateEkf_function2:482' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
    nomQuat[0] = states[0];
    nomQuat[1] = states[1];
    nomQuat[2] = states[2];
    nomQuat[3] = states[3];
    updateQuatAndResetCovP_o483nOwE(nomQuat, &errorStateHat[0], covP);

    // 'errorStateEkf_function2:483' states(1:4) = nomQuat;
    states[0] = nomQuat[0];
    states[1] = nomQuat[1];
    states[2] = nomQuat[2];
    states[3] = nomQuat[3];

    // 'errorStateEkf_function2:485' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
    for (i = 0; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP_1[iS_tmp + 19 * i] = (covP[((K_tmp[i] - 1) * 19 + K_tmp[iS_tmp]) -
          1] + covP[((K_tmp[iS_tmp] - 1) * 19 + K_tmp[i]) - 1]) * 0.5F;
      }
    }

    for (i = 0; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP[(K_tmp[iS_tmp] + 19 * (K_tmp[i] - 1)) - 1] = covP_1[19 * i + iS_tmp];
      }
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
