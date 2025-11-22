//
// File: applyGpsPosAndVelCorr_kaQP2N0T.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "applyGpsPosAndVelCorr_kaQP2N0T.h"
#include <cstring>
#include "updateQuatAndResetCovP_KAnSUXrZ.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function [states, covP] = applyGpsPosAndVelCorr(states, nedPosAndVel, covP, idx, idxEs, ...
//     idxEs2, idxNs2, measNoiseR, innovGate)
//
void applyGpsPosAndVelCorr_kaQP2N0T(real32_T states[20], const real32_T
  nedPosAndVel[6], real32_T covP[361], real_T idx, const real_T idxEs[19], const
  real_T idxEs2[16], const real_T idxNs2[16], const real32_T measNoiseR[225],
  real32_T innovGate)
{
  int32_T iS_tmp;
  real32_T covP_1[361];
  real32_T K[19];
  real32_T covP_0[19];
  real32_T errorStateHat[19];
  real32_T states_0[16];
  real32_T nomQuat[4];
  real32_T iS;
  real32_T nu;

  // 'errorStateEkf_function2:467' iS = 1/(covP(idx, idx) + measNoiseR(idx, idx)); 
  iS_tmp = (static_cast<int32_T>(idx) - 1) * 19;
  iS = 1.0F / (measNoiseR[((static_cast<int32_T>(idx) - 1) * 15 +
    static_cast<int32_T>(idx)) - 1] + covP[(iS_tmp + static_cast<int32_T>(idx))
               - 1]);

  // 'errorStateEkf_function2:468' nu = nedPosAndVel(idx - 3)  - states(idx + 1); 
  nu = nedPosAndVel[static_cast<int32_T>(idx - 3.0) - 1] - states
    [static_cast<int32_T>(idx + 1.0) - 1];

  // 'errorStateEkf_function2:469' NIS = nu*nu*iS;
  // ErrorStateHat
  // 'errorStateEkf_function2:472' errorStateHat = zeros(19, 1, 'single');
  std::memset(&errorStateHat[0], 0, 19U * sizeof(real32_T));

  // 'errorStateEkf_function2:474' if NIS < innovGate
  if (nu * nu * iS < innovGate) {
    // 'errorStateEkf_function2:475' K = covP(idxEs, idx).*iS;
    // 'errorStateEkf_function2:476' errorStateHat(idxEs) = K*nu;
    // 'errorStateEkf_function2:478' covP(idxEs, idxEs) = covP(idxEs, idxEs) - (K*covP(idx, idxEs)); 
    for (int32_T i{0}; i < 19; i++) {
      real_T idxEs_0;
      real32_T K_0;
      idxEs_0 = idxEs[i];
      K_0 = covP[(iS_tmp + static_cast<int32_T>(idxEs_0)) - 1] * iS;
      errorStateHat[static_cast<int32_T>(idxEs_0) - 1] = K_0 * nu;
      covP_0[i] = covP[((static_cast<int32_T>(idxEs_0) - 1) * 19 +
                        static_cast<int32_T>(idx)) - 1];
      K[i] = K_0;
    }

    for (int32_T i{0}; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP_1[iS_tmp + 19 * i] = covP[((static_cast<int32_T>(idxEs[i]) - 1) *
          19 + static_cast<int32_T>(idxEs[iS_tmp])) - 1] - K[iS_tmp] * covP_0[i];
      }
    }

    for (int32_T i{0}; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP[(static_cast<int32_T>(idxEs[iS_tmp]) + 19 * (static_cast<int32_T>
               (idxEs[i]) - 1)) - 1] = covP_1[19 * i + iS_tmp];
      }
    }

    // Update the nominal state
    // 'errorStateEkf_function2:481' states(idxNs2) = states(idxNs2) + errorStateHat(idxEs2); 
    for (int32_T i{0}; i < 16; i++) {
      states_0[i] = states[static_cast<int32_T>(idxNs2[i]) - 1] + errorStateHat[
        static_cast<int32_T>(idxEs2[i]) - 1];
    }

    for (int32_T i{0}; i < 16; i++) {
      states[static_cast<int32_T>(idxNs2[i]) - 1] = states_0[i];
    }

    // Construct quaternion from the rotation vector and reset covP
    // 'errorStateEkf_function2:484' [nomQuat, covP] = updateQuatAndResetCovP(states(1:4), errorStateHat(1:3), covP); 
    nomQuat[0] = states[0];
    nomQuat[1] = states[1];
    nomQuat[2] = states[2];
    nomQuat[3] = states[3];
    updateQuatAndResetCovP_KAnSUXrZ(nomQuat, &errorStateHat[0], covP);

    // 'errorStateEkf_function2:485' states(1:4) = nomQuat;
    states[0] = nomQuat[0];
    states[1] = nomQuat[1];
    states[2] = nomQuat[2];
    states[3] = nomQuat[3];

    // 'errorStateEkf_function2:487' covP(idxEs, idxEs) = (covP(idxEs, idxEs) + covP(idxEs, idxEs)').*0.5; 
    for (int32_T i{0}; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        int32_T covP_tmp;
        int32_T covP_tmp_0;
        covP_tmp = static_cast<int32_T>(idxEs[iS_tmp]);
        covP_tmp_0 = static_cast<int32_T>(idxEs[i]);
        covP_1[iS_tmp + 19 * i] = (covP[((covP_tmp_0 - 1) * 19 + covP_tmp) - 1]
          + covP[((covP_tmp - 1) * 19 + covP_tmp_0) - 1]) * 0.5F;
      }
    }

    for (int32_T i{0}; i < 19; i++) {
      for (iS_tmp = 0; iS_tmp < 19; iS_tmp++) {
        covP[(static_cast<int32_T>(idxEs[iS_tmp]) + 19 * (static_cast<int32_T>
               (idxEs[i]) - 1)) - 1] = covP_1[19 * i + iS_tmp];
      }
    }
  }
}

//
// File trailer for generated code.
//
// [EOF]
//
