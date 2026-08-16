//
// File: computeEskfStateJac_tpF0ZWTA.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 7.81
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Sun Aug 16 10:34:06 2026
//
#include "rtwtypes.h"
#include "computeEskfStateJac_tpF0ZWTA.h"
#include <cstring>
#include <cmath>

//
// Function for MATLAB Function: '<S1>/EKF'
// function  stateJac = computeEskfStateJac(states, dcmBodyToNed, bodyAccels_mps2, ...
//     dAng, dAngUnitVec, noGps, isOfValid, sampleTime_s)
// COMPUTEESKFSTATEJAC Computes the state Jacobian for ESKF
//
// Inputs:
// states:               ESKF States
// dcmBodyToNed:         Body To NED DCM
// bodyAccels_mps2:      Body Accels
// dAng:                 Delta Ang Magnitude
// dAngUnitVec:          Delta Ang Unit Vector
// noGps:                True -> NoGps
// isOfValid:            False > No OF
// sampleTime_s:         Sample Time
//
// Outputs:
// stateJac:             State Jacobian
//
void computeEskfStateJac_tpF0ZWTA(const real32_T states[20], const real32_T
  dcmBodyToNed[9], const real32_T bodyAccels_mps2[3], real32_T dAng, const
  real32_T dAngUnitVec[3], boolean_T noGps, boolean_T isOfValid, real32_T
  b_sampleTime_s, real32_T stateJac[46])
{
  real32_T cAng;
  real32_T sAng;
  real32_T stateJac_tmp;
  real32_T tmp2;
  real32_T tmp3;
  real32_T tmp4;

  // 'computeEskfStateJac:18' stateJac = zeros(46, 1, 'single');
  std::memset(&stateJac[0], 0, 46U * sizeof(real32_T));

  // 'computeEskfStateJac:20' cAng = cos(dAng);
  cAng = std::cos(dAng);

  // 'computeEskfStateJac:21' sAng = sin(dAng);
  sAng = std::sin(dAng);

  // 'computeEskfStateJac:23' tmp1 = cAng - 1;
  // 'computeEskfStateJac:24' tmp2 = dAngUnitVec(1)*dAngUnitVec(2)*tmp1;
  tmp2 = dAngUnitVec[0] * dAngUnitVec[1] * (cAng - 1.0F);

  // 'computeEskfStateJac:25' tmp3 = dAngUnitVec(3)*sAng;
  tmp3 = dAngUnitVec[2] * sAng;

  // 'computeEskfStateJac:26' tmp4 = dAngUnitVec(2)*sAng;
  tmp4 = dAngUnitVec[1] * sAng;

  // 'computeEskfStateJac:27' tmp5 = dAngUnitVec(2)*dAngUnitVec(3);
  // 'computeEskfStateJac:28' tmp6 = dAngUnitVec(1)*sAng;
  sAng *= dAngUnitVec[0];

  // stateJac(1, 1) = 1 - dAngUnitVec(1)^2*tmp1;
  // 'computeEskfStateJac:31' stateJac(1) = 1 - dAngUnitVec(1)*dAngUnitVec(1)*tmp1; 
  stateJac[0] = 1.0F - dAngUnitVec[0] * dAngUnitVec[0] * (cAng - 1.0F);

  // stateJac(1, 2) = tmp3 - tmp2;
  // 'computeEskfStateJac:34' stateJac(2) = tmp3 - tmp2;
  stateJac[1] = tmp3 - tmp2;

  // stateJac(1, 3) = - tmp4 - tmp4*tmp1;
  // 'computeEskfStateJac:37' stateJac(3) = - tmp4 - tmp4*tmp1;
  stateJac_tmp = (cAng - 1.0F) * tmp4;
  stateJac[2] = -tmp4 - stateJac_tmp;

  // stateJac(2, 1) = - tmp3 - tmp2;
  // 'computeEskfStateJac:40' stateJac(4) = - tmp3 - tmp2;
  stateJac[3] = -tmp3 - tmp2;

  // stateJac(2, 2) = 1 - dAngUnitVec(2)^2*tmp1;
  // 'computeEskfStateJac:43' stateJac(5) = 1 - dAngUnitVec(2)*dAngUnitVec(2)*tmp1; 
  stateJac[4] = 1.0F - dAngUnitVec[1] * dAngUnitVec[1] * (cAng - 1.0F);

  // stateJac(2, 3) = tmp6 - tmp5*tmp1;
  // 'computeEskfStateJac:46' stateJac(6) = tmp6 - tmp5*tmp1;
  tmp2 = (cAng - 1.0F) * (dAngUnitVec[1] * dAngUnitVec[2]);
  stateJac[5] = sAng - tmp2;

  // stateJac(3, 1) = tmp4 - tmp4*tmp1;
  // 'computeEskfStateJac:49' stateJac(7) = tmp4 - tmp4*tmp1;
  stateJac[6] = tmp4 - stateJac_tmp;

  // stateJac(3, 2) = - tmp6 - tmp5*tmp1;
  // 'computeEskfStateJac:52' stateJac(8) = - tmp6 - tmp5*tmp1;
  stateJac[7] = -sAng - tmp2;

  // stateJac(3, 3) = 1 - dAngUnitVec(3)^2*tmp1;
  // 'computeEskfStateJac:55' stateJac(9) = 1 - dAngUnitVec(3)*dAngUnitVec(3)*tmp1; 
  stateJac[8] = 1.0F - dAngUnitVec[2] * dAngUnitVec[2] * (cAng - 1.0F);

  // 'computeEskfStateJac:57' if noGps && ~isOfValid
  if (static_cast<boolean_T>(static_cast<boolean_T>(static_cast<int32_T>
        (isOfValid) ^ 1) & noGps)) {
    // stateJac(4, 4) = 1;
    // 'computeEskfStateJac:59' stateJac(10) = 1;
    stateJac[9] = 1.0F;

    // stateJac(5, 5) = 1;
    // 'computeEskfStateJac:62' stateJac(12) = 1;
    stateJac[11] = 1.0F;

    // stateJac(6, 6) = 1;
    // 'computeEskfStateJac:65' stateJac(14) = 1;
    stateJac[13] = 1.0F;

    // stateJac(6, 9) = sampleTime_s;
    // 'computeEskfStateJac:68' stateJac(15) = sampleTime_s;
    stateJac[14] = b_sampleTime_s;

    // stateJac(7, 7) = 1;
    // 'computeEskfStateJac:71' stateJac(19) = 1;
    stateJac[18] = 1.0F;

    // stateJac(8, 8) = 1;
    // 'computeEskfStateJac:74' stateJac(26) = 1;
    stateJac[25] = 1.0F;

    // stateJac(9, 9) = 1;
    // 'computeEskfStateJac:77' stateJac(33) = 1;
    stateJac[32] = 1.0F;
  } else {
    // 'computeEskfStateJac:79' else
    // 'computeEskfStateJac:80' tmp7 = (bodyAccels_mps2(1) - states(14));
    cAng = bodyAccels_mps2[0] - states[13];

    // 'computeEskfStateJac:81' tmp8 = (bodyAccels_mps2(2) - states(15));
    tmp2 = bodyAccels_mps2[1] - states[14];

    // 'computeEskfStateJac:82' tmp9 = (bodyAccels_mps2(3) - states(16));
    tmp3 = bodyAccels_mps2[2] - states[15];

    // stateJac(4, 4) = 1;
    // 'computeEskfStateJac:85' stateJac(10) = 1;
    stateJac[9] = 1.0F;

    // stateJac(4, 7) = sampleTime_s;
    // 'computeEskfStateJac:88' stateJac(11) = sampleTime_s;
    stateJac[10] = b_sampleTime_s;

    // stateJac(5, 5) = 1;
    // 'computeEskfStateJac:91' stateJac(12) = 1;
    stateJac[11] = 1.0F;

    // stateJac(5, 8) = sampleTime_s;
    // 'computeEskfStateJac:94' stateJac(13) = sampleTime_s;
    stateJac[12] = b_sampleTime_s;

    // stateJac(6, 6) = 1;
    // 'computeEskfStateJac:97' stateJac(14) = 1;
    stateJac[13] = 1.0F;

    // stateJac(6, 9) = sampleTime_s;
    // 'computeEskfStateJac:100' stateJac(15) = sampleTime_s;
    stateJac[14] = b_sampleTime_s;

    // stateJac(7, 1) = sampleTime_s*(dcmBodyToNed(1, 3)*tmp8 - dcmBodyToNed(1, 2)*tmp9); 
    // 'computeEskfStateJac:104' stateJac(16) = sampleTime_s*(dcmBodyToNed(1, 3)*tmp8 - dcmBodyToNed(1, 2)*tmp9); 
    stateJac[15] = (dcmBodyToNed[6] * tmp2 - dcmBodyToNed[3] * tmp3) *
      b_sampleTime_s;

    // stateJac(7, 2) = -sampleTime_s*(dcmBodyToNed(1, 3)*tmp7 - dcmBodyToNed(1, 1)*tmp9); 
    // 'computeEskfStateJac:107' stateJac(17) = -sampleTime_s*(dcmBodyToNed(1, 3)*tmp7 - dcmBodyToNed(1, 1)*tmp9); 
    stateJac[16] = (dcmBodyToNed[6] * cAng - dcmBodyToNed[0] * tmp3) *
      -b_sampleTime_s;

    // stateJac(7, 3) = sampleTime_s*(dcmBodyToNed(1, 2)*tmp7 - dcmBodyToNed(1, 1)*tmp8); 
    // 'computeEskfStateJac:110' stateJac(18) = sampleTime_s*(dcmBodyToNed(1, 2)*tmp7 - dcmBodyToNed(1, 1)*tmp8); 
    stateJac[17] = (dcmBodyToNed[3] * cAng - dcmBodyToNed[0] * tmp2) *
      b_sampleTime_s;

    // stateJac(7, 7) = 1;
    // 'computeEskfStateJac:113' stateJac(19) = 1;
    stateJac[18] = 1.0F;

    // stateJac(7, 13) = -dcmBodyToNed(1, 1)*sampleTime_s;
    // 'computeEskfStateJac:116' stateJac(20) = -dcmBodyToNed(1, 1)*sampleTime_s; 
    stateJac[19] = -dcmBodyToNed[0] * b_sampleTime_s;

    // stateJac(7, 14) = -dcmBodyToNed(1, 2)*sampleTime_s;
    // 'computeEskfStateJac:119' stateJac(21) = -dcmBodyToNed(1, 2)*sampleTime_s; 
    stateJac[20] = -dcmBodyToNed[3] * b_sampleTime_s;

    // stateJac(7, 15) = -dcmBodyToNed(1, 3)*sampleTime_s;
    // 'computeEskfStateJac:122' stateJac(22) = -dcmBodyToNed(1, 3)*sampleTime_s; 
    stateJac[21] = -dcmBodyToNed[6] * b_sampleTime_s;

    // stateJac(8, 1) = sampleTime_s*(dcmBodyToNed(2, 3)*tmp8 - dcmBodyToNed(2, 2)*tmp9); 
    // 'computeEskfStateJac:125' stateJac(23) = sampleTime_s*(dcmBodyToNed(2, 3)*tmp8 - dcmBodyToNed(2, 2)*tmp9); 
    stateJac[22] = (dcmBodyToNed[7] * tmp2 - dcmBodyToNed[4] * tmp3) *
      b_sampleTime_s;

    // stateJac(8, 2) = -sampleTime_s*(dcmBodyToNed(2, 3)*tmp7 - dcmBodyToNed(2, 1)*tmp9); 
    // 'computeEskfStateJac:128' stateJac(24) = -sampleTime_s*(dcmBodyToNed(2, 3)*tmp7 - dcmBodyToNed(2, 1)*tmp9); 
    stateJac[23] = (dcmBodyToNed[7] * cAng - dcmBodyToNed[1] * tmp3) *
      -b_sampleTime_s;

    // stateJac(8, 3) = sampleTime_s*(dcmBodyToNed(2, 2)*tmp7 - dcmBodyToNed(2, 1)*tmp8); 
    // 'computeEskfStateJac:131' stateJac(25) = sampleTime_s*(dcmBodyToNed(2, 2)*tmp7 - dcmBodyToNed(2, 1)*tmp8); 
    stateJac[24] = (dcmBodyToNed[4] * cAng - dcmBodyToNed[1] * tmp2) *
      b_sampleTime_s;

    // stateJac(8, 8) = 1;
    // 'computeEskfStateJac:134' stateJac(26) = 1;
    stateJac[25] = 1.0F;

    // stateJac(8, 13) = -dcmBodyToNed(2, 1)*sampleTime_s;
    // 'computeEskfStateJac:137' stateJac(27) = -dcmBodyToNed(2, 1)*sampleTime_s; 
    stateJac[26] = -dcmBodyToNed[1] * b_sampleTime_s;

    // stateJac(8, 14) = -dcmBodyToNed(2, 2)*sampleTime_s;
    // 'computeEskfStateJac:140' stateJac(28) = -dcmBodyToNed(2, 2)*sampleTime_s; 
    stateJac[27] = -dcmBodyToNed[4] * b_sampleTime_s;

    // stateJac(8, 15) = -dcmBodyToNed(2, 3)*sampleTime_s;
    // 'computeEskfStateJac:143' stateJac(29) = -dcmBodyToNed(2, 3)*sampleTime_s; 
    stateJac[28] = -dcmBodyToNed[7] * b_sampleTime_s;

    // stateJac(9, 1) = sampleTime_s*(dcmBodyToNed(3, 3)*tmp8 - dcmBodyToNed(3, 2)*tmp9); 
    // 'computeEskfStateJac:146' stateJac(30) = sampleTime_s*(dcmBodyToNed(3, 3)*tmp8 - dcmBodyToNed(3, 2)*tmp9); 
    stateJac[29] = (dcmBodyToNed[8] * tmp2 - dcmBodyToNed[5] * tmp3) *
      b_sampleTime_s;

    // stateJac(9, 2) = -sampleTime_s*(dcmBodyToNed(3, 3)*tmp7 - dcmBodyToNed(3, 1)*tmp9); 
    // 'computeEskfStateJac:149' stateJac(31) = -sampleTime_s*(dcmBodyToNed(3, 3)*tmp7 - dcmBodyToNed(3, 1)*tmp9); 
    stateJac[30] = (dcmBodyToNed[8] * cAng - dcmBodyToNed[2] * tmp3) *
      -b_sampleTime_s;

    // stateJac(9, 3) = sampleTime_s*(dcmBodyToNed(3, 2)*tmp7 - dcmBodyToNed(3, 1)*tmp8); 
    // 'computeEskfStateJac:152' stateJac(32) = sampleTime_s*(dcmBodyToNed(3, 2)*tmp7 - dcmBodyToNed(3, 1)*tmp8); 
    stateJac[31] = (dcmBodyToNed[5] * cAng - dcmBodyToNed[2] * tmp2) *
      b_sampleTime_s;

    // stateJac(9, 9) = 1;
    // 'computeEskfStateJac:155' stateJac(33) = 1;
    stateJac[32] = 1.0F;

    // stateJac(9, 13) = -dcmBodyToNed(3, 1)*sampleTime_s;
    // 'computeEskfStateJac:158' stateJac(34) = -dcmBodyToNed(3, 1)*sampleTime_s; 
    stateJac[33] = -dcmBodyToNed[2] * b_sampleTime_s;

    // stateJac(9, 14) = -dcmBodyToNed(3, 2)*sampleTime_s;
    // 'computeEskfStateJac:161' stateJac(35) = -dcmBodyToNed(3, 2)*sampleTime_s; 
    stateJac[34] = -dcmBodyToNed[5] * b_sampleTime_s;

    // stateJac(9, 15) = -dcmBodyToNed(3, 3)*sampleTime_s;
    // 'computeEskfStateJac:164' stateJac(36) = -dcmBodyToNed(3, 3)*sampleTime_s; 
    stateJac[35] = -dcmBodyToNed[8] * b_sampleTime_s;
  }

  // stateJac(10, 10) = 1;
  // 'computeEskfStateJac:168' stateJac(37) = 1;
  stateJac[36] = 1.0F;

  // stateJac(11, 11) = 1;
  // 'computeEskfStateJac:171' stateJac(38) = 1;
  stateJac[37] = 1.0F;

  // stateJac(12, 12) = 1;
  // 'computeEskfStateJac:174' stateJac(39) = 1;
  stateJac[38] = 1.0F;

  // stateJac(13, 13) = 1;
  // 'computeEskfStateJac:177' stateJac(40) = 1;
  stateJac[39] = 1.0F;

  // stateJac(14, 14) = 1;
  // 'computeEskfStateJac:180' stateJac(41) = 1;
  stateJac[40] = 1.0F;

  // stateJac(15, 15) = 1;
  // 'computeEskfStateJac:183' stateJac(42) = 1;
  stateJac[41] = 1.0F;

  // stateJac(16, 16) = 1;
  // 'computeEskfStateJac:186' stateJac(43) = 1;
  stateJac[42] = 1.0F;

  // stateJac(17, 17) = 1;
  // 'computeEskfStateJac:189' stateJac(44) = 1;
  stateJac[43] = 1.0F;

  // stateJac(18, 18) = 1;
  // 'computeEskfStateJac:192' stateJac(45) = 1;
  stateJac[44] = 1.0F;

  // stateJac(19, 19) = 1;
  // 'computeEskfStateJac:195' stateJac(46) = 1;
  stateJac[45] = 1.0F;
}

//
// File trailer for generated code.
//
// [EOF]
//
