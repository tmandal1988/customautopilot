//
// File: updateEskfCovP_yMTfUr7W.cpp
//
// Code generated for Simulink model 'stateEstimatorEskf'.
//
// Model version                  : 1.145
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Fri Nov 21 11:04:20 2025
//
#include "rtwtypes.h"
#include "updateEskfCovP_yMTfUr7W.h"

//
// Function for MATLAB Function: '<S1>/EKF'
// function  covP = updateEskfCovP2(covP, stateJac, processNoiseQ, sampleTime_s)
// UPDATEESKFCOVTEST Computes the covariance for the prediction stage of the EKF
//
// Inputs:
// covP:               Covariance from previous time step
// stateJac:           State Jacobian using previous time estimate
// processNoiseQ:      Process noise Q
// sampleTime_s:       Sample Time
//
// Outputs:
// covP:               Updated covariance
//
void updateEskfCovP_yMTfUr7W(real32_T covP[361], const real32_T stateJac[46],
  const real32_T processNoiseQ[361], real32_T b_sampleTime_s)
{
  real32_T tmp1;
  real32_T tmp10;
  real32_T tmp100;
  real32_T tmp101;
  real32_T tmp102;
  real32_T tmp103;
  real32_T tmp104;
  real32_T tmp105;
  real32_T tmp106;
  real32_T tmp107;
  real32_T tmp108;
  real32_T tmp109;
  real32_T tmp11;
  real32_T tmp110;
  real32_T tmp111;
  real32_T tmp112;
  real32_T tmp113;
  real32_T tmp114;
  real32_T tmp115;
  real32_T tmp116;
  real32_T tmp117;
  real32_T tmp118;
  real32_T tmp119;
  real32_T tmp12;
  real32_T tmp120;
  real32_T tmp121;
  real32_T tmp122;
  real32_T tmp123;
  real32_T tmp124;
  real32_T tmp125;
  real32_T tmp126;
  real32_T tmp127;
  real32_T tmp128;
  real32_T tmp129;
  real32_T tmp13;
  real32_T tmp130;
  real32_T tmp131;
  real32_T tmp132;
  real32_T tmp133;
  real32_T tmp134;
  real32_T tmp135;
  real32_T tmp14;
  real32_T tmp15;
  real32_T tmp16;
  real32_T tmp17;
  real32_T tmp18;
  real32_T tmp19;
  real32_T tmp2;
  real32_T tmp20;
  real32_T tmp21;
  real32_T tmp22;
  real32_T tmp23;
  real32_T tmp24;
  real32_T tmp25;
  real32_T tmp26;
  real32_T tmp27;
  real32_T tmp28;
  real32_T tmp29;
  real32_T tmp3;
  real32_T tmp30;
  real32_T tmp31;
  real32_T tmp32;
  real32_T tmp33;
  real32_T tmp34;
  real32_T tmp35;
  real32_T tmp36;
  real32_T tmp37;
  real32_T tmp38;
  real32_T tmp39;
  real32_T tmp4;
  real32_T tmp40;
  real32_T tmp41;
  real32_T tmp42;
  real32_T tmp43;
  real32_T tmp44;
  real32_T tmp45;
  real32_T tmp46;
  real32_T tmp47;
  real32_T tmp48;
  real32_T tmp49;
  real32_T tmp5;
  real32_T tmp50;
  real32_T tmp51;
  real32_T tmp52;
  real32_T tmp53;
  real32_T tmp54;
  real32_T tmp55;
  real32_T tmp56;
  real32_T tmp57;
  real32_T tmp58;
  real32_T tmp59;
  real32_T tmp6;
  real32_T tmp60;
  real32_T tmp61;
  real32_T tmp62;
  real32_T tmp63;
  real32_T tmp64;
  real32_T tmp65;
  real32_T tmp66;
  real32_T tmp67;
  real32_T tmp68;
  real32_T tmp69;
  real32_T tmp7;
  real32_T tmp70;
  real32_T tmp71;
  real32_T tmp72;
  real32_T tmp73;
  real32_T tmp74;
  real32_T tmp75;
  real32_T tmp76;
  real32_T tmp77;
  real32_T tmp78;
  real32_T tmp79;
  real32_T tmp8;
  real32_T tmp80;
  real32_T tmp81;
  real32_T tmp82;
  real32_T tmp83;
  real32_T tmp84;
  real32_T tmp85;
  real32_T tmp86;
  real32_T tmp87;
  real32_T tmp88;
  real32_T tmp89;
  real32_T tmp9;
  real32_T tmp90;
  real32_T tmp91;
  real32_T tmp92;
  real32_T tmp93;
  real32_T tmp94;
  real32_T tmp95;
  real32_T tmp96;
  real32_T tmp97;
  real32_T tmp98;
  real32_T tmp99;

  // 'updateEskfCovP:13' tmp1 = -covP(10, 1)*sampleTime_s + covP(1, 1)*stateJac(1) + covP(2, 1)*stateJac(2) + covP(3, 1)*stateJac(3); 
  tmp1 = ((-covP[9] * b_sampleTime_s + covP[0] * stateJac[0]) + covP[1] *
          stateJac[1]) + covP[2] * stateJac[2];

  // 'updateEskfCovP:14' tmp2 = -covP(10, 2)*sampleTime_s + covP(1, 2)*stateJac(1) + covP(2, 2)*stateJac(2) + covP(3, 2)*stateJac(3); 
  tmp2 = ((-covP[28] * b_sampleTime_s + stateJac[0] * covP[19]) + stateJac[1] *
          covP[20]) + stateJac[2] * covP[21];

  // 'updateEskfCovP:15' tmp3 = -covP(10, 3)*sampleTime_s + covP(1, 3)*stateJac(1) + covP(2, 3)*stateJac(2) + covP(3, 3)*stateJac(3); 
  tmp3 = ((-covP[47] * b_sampleTime_s + stateJac[0] * covP[38]) + stateJac[1] *
          covP[39]) + stateJac[2] * covP[40];

  // 'updateEskfCovP:16' tmp4 = -covP(10, 10)*sampleTime_s;
  tmp4 = -covP[180] * b_sampleTime_s;

  // 'updateEskfCovP:17' tmp5 = covP(1, 10)*stateJac(1) + covP(2, 10)*stateJac(2) + covP(3, 10)*stateJac(3) + tmp4; 
  tmp5 = ((stateJac[0] * covP[171] + stateJac[1] * covP[172]) + stateJac[2] *
          covP[173]) + tmp4;

  // 'updateEskfCovP:18' tmp6 = -covP(10, 11)*sampleTime_s;
  tmp6 = -covP[199] * b_sampleTime_s;

  // 'updateEskfCovP:19' tmp7 = covP(1, 11)*stateJac(1) + covP(2, 11)*stateJac(2) + covP(3, 11)*stateJac(3) + tmp6; 
  tmp7 = ((stateJac[0] * covP[190] + stateJac[1] * covP[191]) + stateJac[2] *
          covP[192]) + tmp6;

  // 'updateEskfCovP:20' tmp8 = -covP(10, 12)*sampleTime_s;
  tmp8 = -covP[218] * b_sampleTime_s;

  // 'updateEskfCovP:21' tmp9 = covP(1, 12)*stateJac(1) + covP(2, 12)*stateJac(2) + covP(3, 12)*stateJac(3) + tmp8; 
  tmp9 = ((stateJac[0] * covP[209] + stateJac[1] * covP[210]) + stateJac[2] *
          covP[211]) + tmp8;

  // 'updateEskfCovP:22' tmp10 = covP(10, 7)*sampleTime_s;
  tmp10 = covP[123] * b_sampleTime_s;

  // 'updateEskfCovP:23' tmp11 = covP(1, 7)*stateJac(1) + covP(2, 7)*stateJac(2) + covP(3, 7)*stateJac(3) - tmp10; 
  tmp11 = ((stateJac[0] * covP[114] + stateJac[1] * covP[115]) + stateJac[2] *
           covP[116]) - tmp10;

  // 'updateEskfCovP:24' tmp12 = covP(10, 8)*sampleTime_s;
  tmp12 = covP[142] * b_sampleTime_s;

  // 'updateEskfCovP:25' tmp13 = covP(1, 8)*stateJac(1) + covP(2, 8)*stateJac(2) + covP(3, 8)*stateJac(3) - tmp12; 
  tmp13 = ((stateJac[0] * covP[133] + stateJac[1] * covP[134]) + stateJac[2] *
           covP[135]) - tmp12;

  // 'updateEskfCovP:26' tmp14 = covP(10, 9)*sampleTime_s;
  tmp14 = covP[161] * b_sampleTime_s;

  // 'updateEskfCovP:27' tmp15 = covP(1, 9)*stateJac(1) + covP(2, 9)*stateJac(2) + covP(3, 9)*stateJac(3) - tmp14; 
  tmp15 = ((stateJac[0] * covP[152] + stateJac[1] * covP[153]) + stateJac[2] *
           covP[154]) - tmp14;

  // 'updateEskfCovP:28' tmp16 = -covP(10, 13)*sampleTime_s + covP(1, 13)*stateJac(1) + covP(2, 13)*stateJac(2) + covP(3, 13)*stateJac(3); 
  tmp16 = ((-covP[237] * b_sampleTime_s + stateJac[0] * covP[228]) + stateJac[1]
           * covP[229]) + stateJac[2] * covP[230];

  // 'updateEskfCovP:29' tmp17 = -covP(10, 14)*sampleTime_s + covP(1, 14)*stateJac(1) + covP(2, 14)*stateJac(2) + covP(3, 14)*stateJac(3); 
  tmp17 = ((-covP[256] * b_sampleTime_s + stateJac[0] * covP[247]) + stateJac[1]
           * covP[248]) + stateJac[2] * covP[249];

  // 'updateEskfCovP:30' tmp18 = -covP(10, 15)*sampleTime_s + covP(1, 15)*stateJac(1) + covP(2, 15)*stateJac(2) + covP(3, 15)*stateJac(3); 
  tmp18 = ((-covP[275] * b_sampleTime_s + stateJac[0] * covP[266]) + stateJac[1]
           * covP[267]) + stateJac[2] * covP[268];

  // 'updateEskfCovP:31' tmp19 = -covP(11, 1)*sampleTime_s + covP(1, 1)*stateJac(4) + covP(2, 1)*stateJac(5) + covP(3, 1)*stateJac(6); 
  tmp19 = ((-covP[10] * b_sampleTime_s + covP[0] * stateJac[3]) + covP[1] *
           stateJac[4]) + covP[2] * stateJac[5];

  // 'updateEskfCovP:32' tmp20 = -covP(11, 2)*sampleTime_s + covP(1, 2)*stateJac(4) + covP(2, 2)*stateJac(5) + covP(3, 2)*stateJac(6); 
  tmp20 = ((-covP[29] * b_sampleTime_s + stateJac[3] * covP[19]) + stateJac[4] *
           covP[20]) + stateJac[5] * covP[21];

  // 'updateEskfCovP:33' tmp21 = -covP(11, 3)*sampleTime_s + covP(1, 3)*stateJac(4) + covP(2, 3)*stateJac(5) + covP(3, 3)*stateJac(6); 
  tmp21 = ((-covP[48] * b_sampleTime_s + stateJac[3] * covP[38]) + stateJac[4] *
           covP[39]) + stateJac[5] * covP[40];

  // 'updateEskfCovP:34' tmp22 = -covP(11, 10)*sampleTime_s;
  tmp22 = -covP[181] * b_sampleTime_s;

  // 'updateEskfCovP:35' tmp23 = covP(1, 10)*stateJac(4) + covP(2, 10)*stateJac(5) + covP(3, 10)*stateJac(6) + tmp22; 
  tmp23 = ((stateJac[3] * covP[171] + stateJac[4] * covP[172]) + stateJac[5] *
           covP[173]) + tmp22;

  // 'updateEskfCovP:36' tmp24 = -covP(11, 11)*sampleTime_s;
  tmp24 = -covP[200] * b_sampleTime_s;

  // 'updateEskfCovP:37' tmp25 = covP(1, 11)*stateJac(4) + covP(2, 11)*stateJac(5) + covP(3, 11)*stateJac(6) + tmp24; 
  tmp25 = ((stateJac[3] * covP[190] + stateJac[4] * covP[191]) + stateJac[5] *
           covP[192]) + tmp24;

  // 'updateEskfCovP:38' tmp26 = -covP(11, 12)*sampleTime_s;
  tmp26 = -covP[219] * b_sampleTime_s;

  // 'updateEskfCovP:39' tmp27 = covP(1, 12)*stateJac(4) + covP(2, 12)*stateJac(5) + covP(3, 12)*stateJac(6) + tmp26; 
  tmp27 = ((stateJac[3] * covP[209] + stateJac[4] * covP[210]) + stateJac[5] *
           covP[211]) + tmp26;

  // 'updateEskfCovP:40' tmp28 = covP(11, 7)*sampleTime_s;
  tmp28 = covP[124] * b_sampleTime_s;

  // 'updateEskfCovP:41' tmp29 = covP(1, 7)*stateJac(4) + covP(2, 7)*stateJac(5) + covP(3, 7)*stateJac(6) - tmp28; 
  tmp29 = ((stateJac[3] * covP[114] + stateJac[4] * covP[115]) + stateJac[5] *
           covP[116]) - tmp28;

  // 'updateEskfCovP:42' tmp30 = covP(11, 8)*sampleTime_s;
  tmp30 = covP[143] * b_sampleTime_s;

  // 'updateEskfCovP:43' tmp31 = covP(1, 8)*stateJac(4) + covP(2, 8)*stateJac(5) + covP(3, 8)*stateJac(6) - tmp30; 
  tmp31 = ((stateJac[3] * covP[133] + stateJac[4] * covP[134]) + stateJac[5] *
           covP[135]) - tmp30;

  // 'updateEskfCovP:44' tmp32 = covP(11, 9)*sampleTime_s;
  tmp32 = covP[162] * b_sampleTime_s;

  // 'updateEskfCovP:45' tmp33 = covP(1, 9)*stateJac(4) + covP(2, 9)*stateJac(5) + covP(3, 9)*stateJac(6) - tmp32; 
  tmp33 = ((stateJac[3] * covP[152] + stateJac[4] * covP[153]) + stateJac[5] *
           covP[154]) - tmp32;

  // 'updateEskfCovP:46' tmp34 = -covP(11, 13)*sampleTime_s + covP(1, 13)*stateJac(4) + covP(2, 13)*stateJac(5) + covP(3, 13)*stateJac(6); 
  tmp34 = ((-covP[238] * b_sampleTime_s + stateJac[3] * covP[228]) + stateJac[4]
           * covP[229]) + stateJac[5] * covP[230];

  // 'updateEskfCovP:47' tmp35 = -covP(11, 14)*sampleTime_s + covP(1, 14)*stateJac(4) + covP(2, 14)*stateJac(5) + covP(3, 14)*stateJac(6); 
  tmp35 = ((-covP[257] * b_sampleTime_s + stateJac[3] * covP[247]) + stateJac[4]
           * covP[248]) + stateJac[5] * covP[249];

  // 'updateEskfCovP:48' tmp36 = -covP(11, 15)*sampleTime_s + covP(1, 15)*stateJac(4) + covP(2, 15)*stateJac(5) + covP(3, 15)*stateJac(6); 
  tmp36 = ((-covP[276] * b_sampleTime_s + stateJac[3] * covP[266]) + stateJac[4]
           * covP[267]) + stateJac[5] * covP[268];

  // 'updateEskfCovP:49' tmp37 = -covP(12, 1)*sampleTime_s + covP(1, 1)*stateJac(7) + covP(2, 1)*stateJac(8) + covP(3, 1)*stateJac(9); 
  tmp37 = ((-covP[11] * b_sampleTime_s + covP[0] * stateJac[6]) + covP[1] *
           stateJac[7]) + covP[2] * stateJac[8];

  // 'updateEskfCovP:50' tmp38 = -covP(12, 2)*sampleTime_s + covP(1, 2)*stateJac(7) + covP(2, 2)*stateJac(8) + covP(3, 2)*stateJac(9); 
  tmp38 = ((-covP[30] * b_sampleTime_s + stateJac[6] * covP[19]) + stateJac[7] *
           covP[20]) + stateJac[8] * covP[21];

  // 'updateEskfCovP:51' tmp39 = -covP(12, 3)*sampleTime_s + covP(1, 3)*stateJac(7) + covP(2, 3)*stateJac(8) + covP(3, 3)*stateJac(9); 
  tmp39 = ((-covP[49] * b_sampleTime_s + stateJac[6] * covP[38]) + stateJac[7] *
           covP[39]) + stateJac[8] * covP[40];

  // 'updateEskfCovP:52' tmp40 = -covP(12, 10)*sampleTime_s;
  tmp40 = -covP[182] * b_sampleTime_s;

  // 'updateEskfCovP:53' tmp41 = covP(1, 10)*stateJac(7) + covP(2, 10)*stateJac(8) + covP(3, 10)*stateJac(9) + tmp40; 
  tmp41 = ((stateJac[6] * covP[171] + stateJac[7] * covP[172]) + stateJac[8] *
           covP[173]) + tmp40;

  // 'updateEskfCovP:54' tmp42 = -covP(12, 11)*sampleTime_s;
  tmp42 = -covP[201] * b_sampleTime_s;

  // 'updateEskfCovP:55' tmp43 = covP(1, 11)*stateJac(7) + covP(2, 11)*stateJac(8) + covP(3, 11)*stateJac(9) + tmp42; 
  tmp43 = ((stateJac[6] * covP[190] + stateJac[7] * covP[191]) + stateJac[8] *
           covP[192]) + tmp42;

  // 'updateEskfCovP:56' tmp44 = -covP(12, 12)*sampleTime_s;
  tmp44 = -covP[220] * b_sampleTime_s;

  // 'updateEskfCovP:57' tmp45 = covP(1, 12)*stateJac(7) + covP(2, 12)*stateJac(8) + covP(3, 12)*stateJac(9) + tmp44; 
  tmp45 = ((stateJac[6] * covP[209] + stateJac[7] * covP[210]) + stateJac[8] *
           covP[211]) + tmp44;

  // 'updateEskfCovP:58' tmp46 = covP(12, 7)*sampleTime_s;
  tmp46 = covP[125] * b_sampleTime_s;

  // 'updateEskfCovP:59' tmp47 = covP(1, 7)*stateJac(7) + covP(2, 7)*stateJac(8) + covP(3, 7)*stateJac(9) - tmp46; 
  tmp47 = ((stateJac[6] * covP[114] + stateJac[7] * covP[115]) + stateJac[8] *
           covP[116]) - tmp46;

  // 'updateEskfCovP:60' tmp48 = covP(12, 8)*sampleTime_s;
  tmp48 = covP[144] * b_sampleTime_s;

  // 'updateEskfCovP:61' tmp49 = covP(1, 8)*stateJac(7) + covP(2, 8)*stateJac(8) + covP(3, 8)*stateJac(9) - tmp48; 
  tmp49 = ((stateJac[6] * covP[133] + stateJac[7] * covP[134]) + stateJac[8] *
           covP[135]) - tmp48;

  // 'updateEskfCovP:62' tmp50 = covP(12, 9)*sampleTime_s;
  tmp50 = covP[163] * b_sampleTime_s;

  // 'updateEskfCovP:63' tmp51 = covP(1, 9)*stateJac(7) + covP(2, 9)*stateJac(8) + covP(3, 9)*stateJac(9) - tmp50; 
  tmp51 = ((stateJac[6] * covP[152] + stateJac[7] * covP[153]) + stateJac[8] *
           covP[154]) - tmp50;

  // 'updateEskfCovP:64' tmp52 = -covP(12, 13)*sampleTime_s + covP(1, 13)*stateJac(7) + covP(2, 13)*stateJac(8) + covP(3, 13)*stateJac(9); 
  tmp52 = ((-covP[239] * b_sampleTime_s + stateJac[6] * covP[228]) + stateJac[7]
           * covP[229]) + stateJac[8] * covP[230];

  // 'updateEskfCovP:65' tmp53 = -covP(12, 14)*sampleTime_s + covP(1, 14)*stateJac(7) + covP(2, 14)*stateJac(8) + covP(3, 14)*stateJac(9); 
  tmp53 = ((-covP[258] * b_sampleTime_s + stateJac[6] * covP[247]) + stateJac[7]
           * covP[248]) + stateJac[8] * covP[249];

  // 'updateEskfCovP:66' tmp54 = -covP(12, 15)*sampleTime_s + covP(1, 15)*stateJac(7) + covP(2, 15)*stateJac(8) + covP(3, 15)*stateJac(9); 
  tmp54 = ((-covP[277] * b_sampleTime_s + stateJac[6] * covP[266]) + stateJac[7]
           * covP[267]) + stateJac[8] * covP[268];

  // 'updateEskfCovP:67' tmp55 = covP(4, 1) + covP(7, 1)*sampleTime_s;
  tmp55 = covP[6] * b_sampleTime_s + covP[3];

  // 'updateEskfCovP:68' tmp56 = covP(4, 2) + covP(7, 2)*sampleTime_s;
  tmp56 = covP[25] * b_sampleTime_s + covP[22];

  // 'updateEskfCovP:69' tmp57 = covP(4, 3) + covP(7, 3)*sampleTime_s;
  tmp57 = covP[44] * b_sampleTime_s + covP[41];

  // 'updateEskfCovP:70' tmp58 = covP(4, 10) + covP(7, 10)*sampleTime_s;
  tmp58 = covP[177] * b_sampleTime_s + covP[174];

  // 'updateEskfCovP:71' tmp59 = covP(4, 11) + covP(7, 11)*sampleTime_s;
  tmp59 = covP[196] * b_sampleTime_s + covP[193];

  // 'updateEskfCovP:72' tmp60 = covP(4, 12) + covP(7, 12)*sampleTime_s;
  tmp60 = covP[215] * b_sampleTime_s + covP[212];

  // 'updateEskfCovP:73' tmp61 = covP(4, 7) + covP(7, 7)*sampleTime_s;
  tmp61 = covP[120] * b_sampleTime_s + covP[117];

  // 'updateEskfCovP:74' tmp62 = covP(4, 8) + covP(7, 8)*sampleTime_s;
  tmp62 = covP[139] * b_sampleTime_s + covP[136];

  // 'updateEskfCovP:75' tmp63 = covP(4, 9) + covP(7, 9)*sampleTime_s;
  tmp63 = covP[158] * b_sampleTime_s + covP[155];

  // 'updateEskfCovP:76' tmp64 = covP(4, 13) + covP(7, 13)*sampleTime_s;
  tmp64 = covP[234] * b_sampleTime_s + covP[231];

  // 'updateEskfCovP:77' tmp65 = covP(4, 14) + covP(7, 14)*sampleTime_s;
  tmp65 = covP[253] * b_sampleTime_s + covP[250];

  // 'updateEskfCovP:78' tmp66 = covP(4, 15) + covP(7, 15)*sampleTime_s;
  tmp66 = covP[272] * b_sampleTime_s + covP[269];

  // 'updateEskfCovP:79' tmp67 = covP(5, 1) + covP(8, 1)*sampleTime_s;
  tmp67 = covP[7] * b_sampleTime_s + covP[4];

  // 'updateEskfCovP:80' tmp68 = covP(5, 2) + covP(8, 2)*sampleTime_s;
  tmp68 = covP[26] * b_sampleTime_s + covP[23];

  // 'updateEskfCovP:81' tmp69 = covP(5, 3) + covP(8, 3)*sampleTime_s;
  tmp69 = covP[45] * b_sampleTime_s + covP[42];

  // 'updateEskfCovP:82' tmp70 = covP(5, 10) + covP(8, 10)*sampleTime_s;
  tmp70 = covP[178] * b_sampleTime_s + covP[175];

  // 'updateEskfCovP:83' tmp71 = covP(5, 11) + covP(8, 11)*sampleTime_s;
  tmp71 = covP[197] * b_sampleTime_s + covP[194];

  // 'updateEskfCovP:84' tmp72 = covP(5, 12) + covP(8, 12)*sampleTime_s;
  tmp72 = covP[216] * b_sampleTime_s + covP[213];

  // 'updateEskfCovP:85' tmp73 = covP(5, 7) + covP(8, 7)*sampleTime_s;
  tmp73 = covP[121] * b_sampleTime_s + covP[118];

  // 'updateEskfCovP:86' tmp74 = covP(5, 8) + covP(8, 8)*sampleTime_s;
  tmp74 = covP[140] * b_sampleTime_s + covP[137];

  // 'updateEskfCovP:87' tmp75 = covP(5, 9) + covP(8, 9)*sampleTime_s;
  tmp75 = covP[159] * b_sampleTime_s + covP[156];

  // 'updateEskfCovP:88' tmp76 = covP(5, 13) + covP(8, 13)*sampleTime_s;
  tmp76 = covP[235] * b_sampleTime_s + covP[232];

  // 'updateEskfCovP:89' tmp77 = covP(5, 14) + covP(8, 14)*sampleTime_s;
  tmp77 = covP[254] * b_sampleTime_s + covP[251];

  // 'updateEskfCovP:90' tmp78 = covP(5, 15) + covP(8, 15)*sampleTime_s;
  tmp78 = covP[273] * b_sampleTime_s + covP[270];

  // 'updateEskfCovP:91' tmp79 = covP(6, 1) + covP(9, 1)*sampleTime_s;
  tmp79 = covP[8] * b_sampleTime_s + covP[5];

  // 'updateEskfCovP:92' tmp80 = covP(6, 2) + covP(9, 2)*sampleTime_s;
  tmp80 = covP[27] * b_sampleTime_s + covP[24];

  // 'updateEskfCovP:93' tmp81 = covP(6, 3) + covP(9, 3)*sampleTime_s;
  tmp81 = covP[46] * b_sampleTime_s + covP[43];

  // 'updateEskfCovP:94' tmp82 = covP(6, 10) + covP(9, 10)*sampleTime_s;
  tmp82 = covP[179] * b_sampleTime_s + covP[176];

  // 'updateEskfCovP:95' tmp83 = covP(6, 11) + covP(9, 11)*sampleTime_s;
  tmp83 = covP[198] * b_sampleTime_s + covP[195];

  // 'updateEskfCovP:96' tmp84 = covP(6, 12) + covP(9, 12)*sampleTime_s;
  tmp84 = covP[217] * b_sampleTime_s + covP[214];

  // 'updateEskfCovP:97' tmp85 = covP(6, 7) + covP(9, 7)*sampleTime_s;
  tmp85 = covP[122] * b_sampleTime_s + covP[119];

  // 'updateEskfCovP:98' tmp86 = covP(6, 8) + covP(9, 8)*sampleTime_s;
  tmp86 = covP[141] * b_sampleTime_s + covP[138];

  // 'updateEskfCovP:99' tmp87 = covP(6, 9) + covP(9, 9)*sampleTime_s;
  tmp87 = covP[160] * b_sampleTime_s + covP[157];

  // 'updateEskfCovP:100' tmp88 = covP(6, 13) + covP(9, 13)*sampleTime_s;
  tmp88 = covP[236] * b_sampleTime_s + covP[233];

  // 'updateEskfCovP:101' tmp89 = covP(6, 14) + covP(9, 14)*sampleTime_s;
  tmp89 = covP[255] * b_sampleTime_s + covP[252];

  // 'updateEskfCovP:102' tmp90 = covP(6, 15) + covP(9, 15)*sampleTime_s;
  tmp90 = covP[274] * b_sampleTime_s + covP[271];

  // 'updateEskfCovP:103' tmp91 = covP(13, 1)*stateJac(20) + covP(14, 1)*stateJac(21) + covP(15, 1)*stateJac(22) + covP(1, 1)*stateJac(16) + covP(2, 1)*stateJac(17) + covP(3, 1)*stateJac(18) + covP(7, 1); 
  tmp91 = (((((covP[12] * stateJac[19] + covP[13] * stateJac[20]) + covP[14] *
              stateJac[21]) + covP[0] * stateJac[15]) + covP[1] * stateJac[16])
           + covP[2] * stateJac[17]) + covP[6];

  // 'updateEskfCovP:104' tmp92 = covP(13, 2)*stateJac(20) + covP(14, 2)*stateJac(21) + covP(15, 2)*stateJac(22) + covP(1, 2)*stateJac(16) + covP(2, 2)*stateJac(17) + covP(3, 2)*stateJac(18) + covP(7, 2); 
  tmp92 = (((((stateJac[19] * covP[31] + stateJac[20] * covP[32]) + stateJac[21]
              * covP[33]) + stateJac[15] * covP[19]) + stateJac[16] * covP[20])
           + stateJac[17] * covP[21]) + covP[25];

  // 'updateEskfCovP:105' tmp93 = covP(13, 3)*stateJac(20) + covP(14, 3)*stateJac(21) + covP(15, 3)*stateJac(22) + covP(1, 3)*stateJac(16) + covP(2, 3)*stateJac(17) + covP(3, 3)*stateJac(18) + covP(7, 3); 
  tmp93 = (((((stateJac[19] * covP[50] + stateJac[20] * covP[51]) + stateJac[21]
              * covP[52]) + stateJac[15] * covP[38]) + stateJac[16] * covP[39])
           + stateJac[17] * covP[40]) + covP[44];

  // 'updateEskfCovP:106' tmp94 = covP(13, 10)*stateJac(20) + covP(14, 10)*stateJac(21) + covP(15, 10)*stateJac(22) + covP(1, 10)*stateJac(16) + covP(2, 10)*stateJac(17) + covP(3, 10)*stateJac(18) + covP(7, 10); 
  tmp94 = (((((stateJac[19] * covP[183] + stateJac[20] * covP[184]) + stateJac
              [21] * covP[185]) + stateJac[15] * covP[171]) + stateJac[16] *
            covP[172]) + stateJac[17] * covP[173]) + covP[177];

  // 'updateEskfCovP:107' tmp95 = covP(13, 11)*stateJac(20) + covP(14, 11)*stateJac(21) + covP(15, 11)*stateJac(22) + covP(1, 11)*stateJac(16) + covP(2, 11)*stateJac(17) + covP(3, 11)*stateJac(18) + covP(7, 11); 
  tmp95 = (((((stateJac[19] * covP[202] + stateJac[20] * covP[203]) + stateJac
              [21] * covP[204]) + stateJac[15] * covP[190]) + stateJac[16] *
            covP[191]) + stateJac[17] * covP[192]) + covP[196];

  // 'updateEskfCovP:108' tmp96 = covP(13, 12)*stateJac(20) + covP(14, 12)*stateJac(21) + covP(15, 12)*stateJac(22) + covP(1, 12)*stateJac(16) + covP(2, 12)*stateJac(17) + covP(3, 12)*stateJac(18) + covP(7, 12); 
  tmp96 = (((((stateJac[19] * covP[221] + stateJac[20] * covP[222]) + stateJac
              [21] * covP[223]) + stateJac[15] * covP[209]) + stateJac[16] *
            covP[210]) + stateJac[17] * covP[211]) + covP[215];

  // 'updateEskfCovP:109' tmp97 = covP(13, 7)*stateJac(20) + covP(14, 7)*stateJac(21) + covP(15, 7)*stateJac(22) + covP(1, 7)*stateJac(16) + covP(2, 7)*stateJac(17) + covP(3, 7)*stateJac(18) + covP(7, 7); 
  tmp97 = (((((stateJac[19] * covP[126] + stateJac[20] * covP[127]) + stateJac
              [21] * covP[128]) + stateJac[15] * covP[114]) + stateJac[16] *
            covP[115]) + stateJac[17] * covP[116]) + covP[120];

  // 'updateEskfCovP:110' tmp98 = covP(13, 8)*stateJac(20) + covP(14, 8)*stateJac(21) + covP(15, 8)*stateJac(22) + covP(1, 8)*stateJac(16) + covP(2, 8)*stateJac(17) + covP(3, 8)*stateJac(18) + covP(7, 8); 
  tmp98 = (((((stateJac[19] * covP[145] + stateJac[20] * covP[146]) + stateJac
              [21] * covP[147]) + stateJac[15] * covP[133]) + stateJac[16] *
            covP[134]) + stateJac[17] * covP[135]) + covP[139];

  // 'updateEskfCovP:111' tmp99 = covP(13, 9)*stateJac(20) + covP(14, 9)*stateJac(21) + covP(15, 9)*stateJac(22) + covP(1, 9)*stateJac(16) + covP(2, 9)*stateJac(17) + covP(3, 9)*stateJac(18) + covP(7, 9); 
  tmp99 = (((((stateJac[19] * covP[164] + stateJac[20] * covP[165]) + stateJac
              [21] * covP[166]) + stateJac[15] * covP[152]) + stateJac[16] *
            covP[153]) + stateJac[17] * covP[154]) + covP[158];

  // 'updateEskfCovP:112' tmp100 = covP(13, 13)*stateJac(20);
  tmp100 = stateJac[19] * covP[240];

  // 'updateEskfCovP:113' tmp101 = covP(14, 13)*stateJac(21) + covP(15, 13)*stateJac(22) + covP(1, 13)*stateJac(16) + covP(2, 13)*stateJac(17) + covP(3, 13)*stateJac(18) + covP(7, 13) + tmp100; 
  tmp101 = (((((stateJac[20] * covP[241] + stateJac[21] * covP[242]) + stateJac
               [15] * covP[228]) + stateJac[16] * covP[229]) + stateJac[17] *
             covP[230]) + covP[234]) + tmp100;

  // 'updateEskfCovP:114' tmp102 = covP(14, 14)*stateJac(21);
  tmp102 = stateJac[20] * covP[260];

  // 'updateEskfCovP:115' tmp103 = covP(13, 14)*stateJac(20) + covP(15, 14)*stateJac(22) + covP(1, 14)*stateJac(16) + covP(2, 14)*stateJac(17) + covP(3, 14)*stateJac(18) + covP(7, 14) + tmp102; 
  tmp103 = (((((stateJac[19] * covP[259] + stateJac[21] * covP[261]) + stateJac
               [15] * covP[247]) + stateJac[16] * covP[248]) + stateJac[17] *
             covP[249]) + covP[253]) + tmp102;

  // 'updateEskfCovP:116' tmp104 = covP(15, 15)*stateJac(22);
  tmp104 = stateJac[21] * covP[280];

  // 'updateEskfCovP:117' tmp105 = covP(13, 15)*stateJac(20) + covP(14, 15)*stateJac(21) + covP(1, 15)*stateJac(16) + covP(2, 15)*stateJac(17) + covP(3, 15)*stateJac(18) + covP(7, 15) + tmp104; 
  tmp105 = (((((stateJac[19] * covP[278] + stateJac[20] * covP[279]) + stateJac
               [15] * covP[266]) + stateJac[16] * covP[267]) + stateJac[17] *
             covP[268]) + covP[272]) + tmp104;

  // 'updateEskfCovP:118' tmp106 = covP(13, 1)*stateJac(27) + covP(14, 1)*stateJac(28) + covP(15, 1)*stateJac(29) + covP(1, 1)*stateJac(23) + covP(2, 1)*stateJac(24) + covP(3, 1)*stateJac(25) + covP(8, 1); 
  tmp106 = (((((covP[12] * stateJac[26] + covP[13] * stateJac[27]) + covP[14] *
               stateJac[28]) + covP[0] * stateJac[22]) + covP[1] * stateJac[23])
            + covP[2] * stateJac[24]) + covP[7];

  // 'updateEskfCovP:119' tmp107 = covP(13, 2)*stateJac(27) + covP(14, 2)*stateJac(28) + covP(15, 2)*stateJac(29) + covP(1, 2)*stateJac(23) + covP(2, 2)*stateJac(24) + covP(3, 2)*stateJac(25) + covP(8, 2); 
  tmp107 = (((((stateJac[26] * covP[31] + stateJac[27] * covP[32]) + stateJac[28]
               * covP[33]) + covP[19] * stateJac[22]) + covP[20] * stateJac[23])
            + covP[21] * stateJac[24]) + covP[26];

  // 'updateEskfCovP:120' tmp108 = covP(13, 3)*stateJac(27) + covP(14, 3)*stateJac(28) + covP(15, 3)*stateJac(29) + covP(1, 3)*stateJac(23) + covP(2, 3)*stateJac(24) + covP(3, 3)*stateJac(25) + covP(8, 3); 
  tmp108 = (((((stateJac[26] * covP[50] + stateJac[27] * covP[51]) + stateJac[28]
               * covP[52]) + stateJac[22] * covP[38]) + stateJac[23] * covP[39])
            + stateJac[24] * covP[40]) + covP[45];

  // 'updateEskfCovP:121' tmp109 = covP(13, 10)*stateJac(27) + covP(14, 10)*stateJac(28) + covP(15, 10)*stateJac(29) + covP(1, 10)*stateJac(23) + covP(2, 10)*stateJac(24) + covP(3, 10)*stateJac(25) + covP(8, 10); 
  tmp109 = (((((stateJac[26] * covP[183] + stateJac[27] * covP[184]) + stateJac
               [28] * covP[185]) + stateJac[22] * covP[171]) + stateJac[23] *
             covP[172]) + stateJac[24] * covP[173]) + covP[178];

  // 'updateEskfCovP:122' tmp110 = covP(13, 11)*stateJac(27) + covP(14, 11)*stateJac(28) + covP(15, 11)*stateJac(29) + covP(1, 11)*stateJac(23) + covP(2, 11)*stateJac(24) + covP(3, 11)*stateJac(25) + covP(8, 11); 
  tmp110 = (((((stateJac[26] * covP[202] + stateJac[27] * covP[203]) + stateJac
               [28] * covP[204]) + stateJac[22] * covP[190]) + stateJac[23] *
             covP[191]) + stateJac[24] * covP[192]) + covP[197];

  // 'updateEskfCovP:123' tmp111 = covP(13, 12)*stateJac(27) + covP(14, 12)*stateJac(28) + covP(15, 12)*stateJac(29) + covP(1, 12)*stateJac(23) + covP(2, 12)*stateJac(24) + covP(3, 12)*stateJac(25) + covP(8, 12); 
  tmp111 = (((((stateJac[26] * covP[221] + stateJac[27] * covP[222]) + stateJac
               [28] * covP[223]) + stateJac[22] * covP[209]) + stateJac[23] *
             covP[210]) + stateJac[24] * covP[211]) + covP[216];

  // 'updateEskfCovP:124' tmp112 = covP(13, 7)*stateJac(27) + covP(14, 7)*stateJac(28) + covP(15, 7)*stateJac(29) + covP(1, 7)*stateJac(23) + covP(2, 7)*stateJac(24) + covP(3, 7)*stateJac(25) + covP(8, 7); 
  tmp112 = (((((stateJac[26] * covP[126] + stateJac[27] * covP[127]) + stateJac
               [28] * covP[128]) + stateJac[22] * covP[114]) + stateJac[23] *
             covP[115]) + stateJac[24] * covP[116]) + covP[121];

  // 'updateEskfCovP:125' tmp113 = covP(13, 8)*stateJac(27) + covP(14, 8)*stateJac(28) + covP(15, 8)*stateJac(29) + covP(1, 8)*stateJac(23) + covP(2, 8)*stateJac(24) + covP(3, 8)*stateJac(25) + covP(8, 8); 
  tmp113 = (((((stateJac[26] * covP[145] + stateJac[27] * covP[146]) + stateJac
               [28] * covP[147]) + stateJac[22] * covP[133]) + stateJac[23] *
             covP[134]) + stateJac[24] * covP[135]) + covP[140];

  // 'updateEskfCovP:126' tmp114 = covP(13, 9)*stateJac(27) + covP(14, 9)*stateJac(28) + covP(15, 9)*stateJac(29) + covP(1, 9)*stateJac(23) + covP(2, 9)*stateJac(24) + covP(3, 9)*stateJac(25) + covP(8, 9); 
  tmp114 = (((((stateJac[26] * covP[164] + stateJac[27] * covP[165]) + stateJac
               [28] * covP[166]) + stateJac[22] * covP[152]) + stateJac[23] *
             covP[153]) + stateJac[24] * covP[154]) + covP[159];

  // 'updateEskfCovP:127' tmp115 = covP(13, 13)*stateJac(27);
  tmp115 = stateJac[26] * covP[240];

  // 'updateEskfCovP:128' tmp116 = covP(14, 13)*stateJac(28) + covP(15, 13)*stateJac(29) + covP(1, 13)*stateJac(23) + covP(2, 13)*stateJac(24) + covP(3, 13)*stateJac(25) + covP(8, 13) + tmp115; 
  tmp116 = (((((stateJac[27] * covP[241] + stateJac[28] * covP[242]) + stateJac
               [22] * covP[228]) + stateJac[23] * covP[229]) + stateJac[24] *
             covP[230]) + covP[235]) + tmp115;

  // 'updateEskfCovP:129' tmp117 = covP(14, 14)*stateJac(28);
  tmp117 = stateJac[27] * covP[260];

  // 'updateEskfCovP:130' tmp118 = covP(13, 14)*stateJac(27) + covP(15, 14)*stateJac(29) + covP(1, 14)*stateJac(23) + covP(2, 14)*stateJac(24) + covP(3, 14)*stateJac(25) + covP(8, 14) + tmp117; 
  tmp118 = (((((stateJac[26] * covP[259] + stateJac[28] * covP[261]) + stateJac
               [22] * covP[247]) + stateJac[23] * covP[248]) + stateJac[24] *
             covP[249]) + covP[254]) + tmp117;

  // 'updateEskfCovP:131' tmp119 = covP(15, 15)*stateJac(29);
  tmp119 = stateJac[28] * covP[280];

  // 'updateEskfCovP:132' tmp120 = covP(13, 15)*stateJac(27) + covP(14, 15)*stateJac(28) + covP(1, 15)*stateJac(23) + covP(2, 15)*stateJac(24) + covP(3, 15)*stateJac(25) + covP(8, 15) + tmp119; 
  tmp120 = (((((stateJac[26] * covP[278] + stateJac[27] * covP[279]) + stateJac
               [22] * covP[266]) + stateJac[23] * covP[267]) + stateJac[24] *
             covP[268]) + covP[273]) + tmp119;

  // 'updateEskfCovP:133' tmp121 = covP(13, 1)*stateJac(34) + covP(14, 1)*stateJac(35) + covP(15, 1)*stateJac(36) + covP(1, 1)*stateJac(30) + covP(2, 1)*stateJac(31) + covP(3, 1)*stateJac(32) + covP(9, 1); 
  tmp121 = (((((covP[12] * stateJac[33] + covP[13] * stateJac[34]) + covP[14] *
               stateJac[35]) + covP[0] * stateJac[29]) + covP[1] * stateJac[30])
            + covP[2] * stateJac[31]) + covP[8];

  // 'updateEskfCovP:134' tmp122 = covP(13, 2)*stateJac(34) + covP(14, 2)*stateJac(35) + covP(15, 2)*stateJac(36) + covP(1, 2)*stateJac(30) + covP(2, 2)*stateJac(31) + covP(3, 2)*stateJac(32) + covP(9, 2); 
  tmp122 = (((((covP[31] * stateJac[33] + covP[32] * stateJac[34]) + covP[33] *
               stateJac[35]) + covP[19] * stateJac[29]) + covP[20] * stateJac[30])
            + covP[21] * stateJac[31]) + covP[27];

  // 'updateEskfCovP:135' tmp123 = covP(13, 3)*stateJac(34) + covP(14, 3)*stateJac(35) + covP(15, 3)*stateJac(36) + covP(1, 3)*stateJac(30) + covP(2, 3)*stateJac(31) + covP(3, 3)*stateJac(32) + covP(9, 3); 
  tmp123 = (((((stateJac[33] * covP[50] + stateJac[34] * covP[51]) + stateJac[35]
               * covP[52]) + stateJac[29] * covP[38]) + stateJac[30] * covP[39])
            + stateJac[31] * covP[40]) + covP[46];

  // 'updateEskfCovP:136' tmp124 = covP(13, 10)*stateJac(34) + covP(14, 10)*stateJac(35) + covP(15, 10)*stateJac(36) + covP(1, 10)*stateJac(30) + covP(2, 10)*stateJac(31) + covP(3, 10)*stateJac(32) + covP(9, 10); 
  tmp124 = (((((stateJac[33] * covP[183] + stateJac[34] * covP[184]) + stateJac
               [35] * covP[185]) + stateJac[29] * covP[171]) + stateJac[30] *
             covP[172]) + stateJac[31] * covP[173]) + covP[179];

  // 'updateEskfCovP:137' tmp125 = covP(13, 11)*stateJac(34) + covP(14, 11)*stateJac(35) + covP(15, 11)*stateJac(36) + covP(1, 11)*stateJac(30) + covP(2, 11)*stateJac(31) + covP(3, 11)*stateJac(32) + covP(9, 11); 
  tmp125 = (((((stateJac[33] * covP[202] + stateJac[34] * covP[203]) + stateJac
               [35] * covP[204]) + stateJac[29] * covP[190]) + stateJac[30] *
             covP[191]) + stateJac[31] * covP[192]) + covP[198];

  // 'updateEskfCovP:138' tmp126 = covP(13, 12)*stateJac(34) + covP(14, 12)*stateJac(35) + covP(15, 12)*stateJac(36) + covP(1, 12)*stateJac(30) + covP(2, 12)*stateJac(31) + covP(3, 12)*stateJac(32) + covP(9, 12); 
  tmp126 = (((((stateJac[33] * covP[221] + stateJac[34] * covP[222]) + stateJac
               [35] * covP[223]) + stateJac[29] * covP[209]) + stateJac[30] *
             covP[210]) + stateJac[31] * covP[211]) + covP[217];

  // 'updateEskfCovP:139' tmp127 = covP(13, 7)*stateJac(34) + covP(14, 7)*stateJac(35) + covP(15, 7)*stateJac(36) + covP(1, 7)*stateJac(30) + covP(2, 7)*stateJac(31) + covP(3, 7)*stateJac(32) + covP(9, 7); 
  tmp127 = (((((stateJac[33] * covP[126] + stateJac[34] * covP[127]) + stateJac
               [35] * covP[128]) + stateJac[29] * covP[114]) + stateJac[30] *
             covP[115]) + stateJac[31] * covP[116]) + covP[122];

  // 'updateEskfCovP:140' tmp128 = covP(13, 8)*stateJac(34) + covP(14, 8)*stateJac(35) + covP(15, 8)*stateJac(36) + covP(1, 8)*stateJac(30) + covP(2, 8)*stateJac(31) + covP(3, 8)*stateJac(32) + covP(9, 8); 
  tmp128 = (((((stateJac[33] * covP[145] + stateJac[34] * covP[146]) + stateJac
               [35] * covP[147]) + stateJac[29] * covP[133]) + stateJac[30] *
             covP[134]) + stateJac[31] * covP[135]) + covP[141];

  // 'updateEskfCovP:141' tmp129 = covP(13, 9)*stateJac(34) + covP(14, 9)*stateJac(35) + covP(15, 9)*stateJac(36) + covP(1, 9)*stateJac(30) + covP(2, 9)*stateJac(31) + covP(3, 9)*stateJac(32) + covP(9, 9); 
  tmp129 = (((((stateJac[33] * covP[164] + stateJac[34] * covP[165]) + stateJac
               [35] * covP[166]) + stateJac[29] * covP[152]) + stateJac[30] *
             covP[153]) + stateJac[31] * covP[154]) + covP[160];

  // 'updateEskfCovP:142' tmp130 = covP(13, 13)*stateJac(34);
  tmp130 = stateJac[33] * covP[240];

  // 'updateEskfCovP:143' tmp131 = covP(14, 13)*stateJac(35) + covP(15, 13)*stateJac(36) + covP(1, 13)*stateJac(30) + covP(2, 13)*stateJac(31) + covP(3, 13)*stateJac(32) + covP(9, 13) + tmp130; 
  tmp131 = (((((stateJac[34] * covP[241] + stateJac[35] * covP[242]) + stateJac
               [29] * covP[228]) + stateJac[30] * covP[229]) + stateJac[31] *
             covP[230]) + covP[236]) + tmp130;

  // 'updateEskfCovP:144' tmp132 = covP(14, 14)*stateJac(35);
  tmp132 = stateJac[34] * covP[260];

  // 'updateEskfCovP:145' tmp133 = covP(13, 14)*stateJac(34) + covP(15, 14)*stateJac(36) + covP(1, 14)*stateJac(30) + covP(2, 14)*stateJac(31) + covP(3, 14)*stateJac(32) + covP(9, 14) + tmp132; 
  tmp133 = (((((stateJac[33] * covP[259] + stateJac[35] * covP[261]) + stateJac
               [29] * covP[247]) + stateJac[30] * covP[248]) + stateJac[31] *
             covP[249]) + covP[255]) + tmp132;

  // 'updateEskfCovP:146' tmp134 = covP(15, 15)*stateJac(36);
  tmp134 = stateJac[35] * covP[280];

  // 'updateEskfCovP:147' tmp135 = covP(13, 15)*stateJac(34) + covP(14, 15)*stateJac(35) + covP(1, 15)*stateJac(30) + covP(2, 15)*stateJac(31) + covP(3, 15)*stateJac(32) + covP(9, 15) + tmp134; 
  tmp135 = (((((stateJac[33] * covP[278] + stateJac[34] * covP[279]) + stateJac
               [29] * covP[266]) + stateJac[30] * covP[267]) + stateJac[31] *
             covP[268]) + covP[274]) + tmp134;

  // 'updateEskfCovP:148' covP(1, 1) = processNoiseQ(1, 1) - sampleTime_s*tmp5 + stateJac(1)*tmp1 + stateJac(2)*tmp2 + stateJac(3)*tmp3; 
  covP[0] = (((processNoiseQ[0] - b_sampleTime_s * tmp5) + stateJac[0] * tmp1) +
             stateJac[1] * tmp2) + stateJac[2] * tmp3;

  // 'updateEskfCovP:149' covP(1, 2) = -sampleTime_s*tmp7 + stateJac(4)*tmp1 + stateJac(5)*tmp2 + stateJac(6)*tmp3; 
  covP[19] = ((-b_sampleTime_s * tmp7 + stateJac[3] * tmp1) + stateJac[4] * tmp2)
    + stateJac[5] * tmp3;

  // 'updateEskfCovP:150' covP(1, 3) = -sampleTime_s*tmp9 + stateJac(7)*tmp1 + stateJac(8)*tmp2 + stateJac(9)*tmp3; 
  covP[38] = ((-b_sampleTime_s * tmp9 + stateJac[6] * tmp1) + stateJac[7] * tmp2)
    + stateJac[8] * tmp3;

  // 'updateEskfCovP:151' covP(1, 4) = -covP(10, 4)*sampleTime_s + covP(1, 4)*stateJac(1) + covP(2, 4)*stateJac(2) + covP(3, 4)*stateJac(3) + sampleTime_s*tmp11; 
  covP[57] = (((-covP[66] * b_sampleTime_s + stateJac[0] * covP[57]) + stateJac
               [1] * covP[58]) + stateJac[2] * covP[59]) + b_sampleTime_s *
    tmp11;

  // 'updateEskfCovP:152' covP(1, 5) = -covP(10, 5)*sampleTime_s + covP(1, 5)*stateJac(1) + covP(2, 5)*stateJac(2) + covP(3, 5)*stateJac(3) + sampleTime_s*tmp13; 
  covP[76] = (((-covP[85] * b_sampleTime_s + stateJac[0] * covP[76]) + stateJac
               [1] * covP[77]) + stateJac[2] * covP[78]) + b_sampleTime_s *
    tmp13;

  // 'updateEskfCovP:153' covP(1, 6) = -covP(10, 6)*sampleTime_s + covP(1, 6)*stateJac(1) + covP(2, 6)*stateJac(2) + covP(3, 6)*stateJac(3) + sampleTime_s*tmp15; 
  covP[95] = (((-covP[104] * b_sampleTime_s + stateJac[0] * covP[95]) +
               stateJac[1] * covP[96]) + stateJac[2] * covP[97]) +
    b_sampleTime_s * tmp15;

  // 'updateEskfCovP:154' covP(1, 7) = stateJac(16)*tmp1 + stateJac(20)*tmp16 + stateJac(21)*tmp17 + stateJac(22)*tmp18 + stateJac(17)*tmp2 + stateJac(18)*tmp3 + tmp11; 
  covP[114] = (((((stateJac[15] * tmp1 + stateJac[19] * tmp16) + stateJac[20] *
                  tmp17) + stateJac[21] * tmp18) + stateJac[16] * tmp2) +
               stateJac[17] * tmp3) + tmp11;

  // 'updateEskfCovP:155' covP(1, 8) = stateJac(23)*tmp1 + stateJac(27)*tmp16 + stateJac(28)*tmp17 + stateJac(29)*tmp18 + stateJac(24)*tmp2 + stateJac(25)*tmp3 + tmp13; 
  covP[133] = (((((stateJac[22] * tmp1 + stateJac[26] * tmp16) + stateJac[27] *
                  tmp17) + stateJac[28] * tmp18) + stateJac[23] * tmp2) +
               stateJac[24] * tmp3) + tmp13;

  // 'updateEskfCovP:156' covP(1, 9) = stateJac(30)*tmp1 + stateJac(34)*tmp16 + stateJac(35)*tmp17 + stateJac(36)*tmp18 + stateJac(31)*tmp2 + stateJac(32)*tmp3 + tmp15; 
  covP[152] = (((((stateJac[29] * tmp1 + stateJac[33] * tmp16) + stateJac[34] *
                  tmp17) + stateJac[35] * tmp18) + stateJac[30] * tmp2) +
               stateJac[31] * tmp3) + tmp15;

  // 'updateEskfCovP:157' covP(1, 10) = tmp5;
  covP[171] = tmp5;

  // 'updateEskfCovP:158' covP(1, 11) = tmp7;
  covP[190] = tmp7;

  // 'updateEskfCovP:159' covP(1, 12) = tmp9;
  covP[209] = tmp9;

  // 'updateEskfCovP:160' covP(1, 13) = tmp16;
  covP[228] = tmp16;

  // 'updateEskfCovP:161' covP(1, 14) = tmp17;
  covP[247] = tmp17;

  // 'updateEskfCovP:162' covP(1, 15) = tmp18;
  covP[266] = tmp18;

  // 'updateEskfCovP:163' covP(1, 16) = -covP(10, 16)*sampleTime_s + covP(1, 16)*stateJac(1) + covP(2, 16)*stateJac(2) + covP(3, 16)*stateJac(3); 
  covP[285] = ((-covP[294] * b_sampleTime_s + stateJac[0] * covP[285]) +
               stateJac[1] * covP[286]) + stateJac[2] * covP[287];

  // 'updateEskfCovP:164' covP(1, 17) = -covP(10, 17)*sampleTime_s + covP(1, 17)*stateJac(1) + covP(2, 17)*stateJac(2) + covP(3, 17)*stateJac(3); 
  covP[304] = ((-covP[313] * b_sampleTime_s + stateJac[0] * covP[304]) +
               stateJac[1] * covP[305]) + stateJac[2] * covP[306];

  // 'updateEskfCovP:165' covP(1, 18) = -covP(10, 18)*sampleTime_s + covP(1, 18)*stateJac(1) + covP(2, 18)*stateJac(2) + covP(3, 18)*stateJac(3); 
  covP[323] = ((-covP[332] * b_sampleTime_s + stateJac[0] * covP[323]) +
               stateJac[1] * covP[324]) + stateJac[2] * covP[325];

  // 'updateEskfCovP:166' covP(1, 19) = -covP(10, 19)*sampleTime_s + covP(1, 19)*stateJac(1) + covP(2, 19)*stateJac(2) + covP(3, 19)*stateJac(3); 
  covP[342] = ((-covP[351] * b_sampleTime_s + stateJac[0] * covP[342]) +
               stateJac[1] * covP[343]) + stateJac[2] * covP[344];

  // 'updateEskfCovP:167' covP(2, 1) = -sampleTime_s*tmp23 + stateJac(1)*tmp19 + stateJac(2)*tmp20 + stateJac(3)*tmp21; 
  covP[1] = ((-b_sampleTime_s * tmp23 + stateJac[0] * tmp19) + stateJac[1] *
             tmp20) + stateJac[2] * tmp21;

  // 'updateEskfCovP:168' covP(2, 2) = processNoiseQ(2, 2) - sampleTime_s*tmp25 + stateJac(4)*tmp19 + stateJac(5)*tmp20 + stateJac(6)*tmp21; 
  covP[20] = (((processNoiseQ[20] - b_sampleTime_s * tmp25) + stateJac[3] *
               tmp19) + stateJac[4] * tmp20) + stateJac[5] * tmp21;

  // 'updateEskfCovP:169' covP(2, 3) = -sampleTime_s*tmp27 + stateJac(7)*tmp19 + stateJac(8)*tmp20 + stateJac(9)*tmp21; 
  covP[39] = ((-b_sampleTime_s * tmp27 + stateJac[6] * tmp19) + stateJac[7] *
              tmp20) + stateJac[8] * tmp21;

  // 'updateEskfCovP:170' covP(2, 4) = -covP(11, 4)*sampleTime_s + covP(1, 4)*stateJac(4) + covP(2, 4)*stateJac(5) + covP(3, 4)*stateJac(6) + sampleTime_s*tmp29; 
  covP[58] = (((-covP[67] * b_sampleTime_s + stateJac[3] * covP[57]) + stateJac
               [4] * covP[58]) + stateJac[5] * covP[59]) + b_sampleTime_s *
    tmp29;

  // 'updateEskfCovP:171' covP(2, 5) = -covP(11, 5)*sampleTime_s + covP(1, 5)*stateJac(4) + covP(2, 5)*stateJac(5) + covP(3, 5)*stateJac(6) + sampleTime_s*tmp31; 
  covP[77] = (((-covP[86] * b_sampleTime_s + stateJac[3] * covP[76]) + stateJac
               [4] * covP[77]) + stateJac[5] * covP[78]) + b_sampleTime_s *
    tmp31;

  // 'updateEskfCovP:172' covP(2, 6) = -covP(11, 6)*sampleTime_s + covP(1, 6)*stateJac(4) + covP(2, 6)*stateJac(5) + covP(3, 6)*stateJac(6) + sampleTime_s*tmp33; 
  covP[96] = (((-covP[105] * b_sampleTime_s + stateJac[3] * covP[95]) +
               stateJac[4] * covP[96]) + stateJac[5] * covP[97]) +
    b_sampleTime_s * tmp33;

  // 'updateEskfCovP:173' covP(2, 7) = stateJac(16)*tmp19 + stateJac(20)*tmp34 + stateJac(21)*tmp35 + stateJac(22)*tmp36 + stateJac(17)*tmp20 + stateJac(18)*tmp21 + tmp29; 
  covP[115] = (((((stateJac[15] * tmp19 + stateJac[19] * tmp34) + stateJac[20] *
                  tmp35) + stateJac[21] * tmp36) + stateJac[16] * tmp20) +
               stateJac[17] * tmp21) + tmp29;

  // 'updateEskfCovP:174' covP(2, 8) = stateJac(23)*tmp19 + stateJac(27)*tmp34 + stateJac(28)*tmp35 + stateJac(29)*tmp36 + stateJac(24)*tmp20 + stateJac(25)*tmp21 + tmp31; 
  covP[134] = (((((stateJac[22] * tmp19 + stateJac[26] * tmp34) + stateJac[27] *
                  tmp35) + stateJac[28] * tmp36) + stateJac[23] * tmp20) +
               stateJac[24] * tmp21) + tmp31;

  // 'updateEskfCovP:175' covP(2, 9) = stateJac(30)*tmp19 + stateJac(34)*tmp34 + stateJac(35)*tmp35 + stateJac(36)*tmp36 + stateJac(31)*tmp20 + stateJac(32)*tmp21 + tmp33; 
  covP[153] = (((((stateJac[29] * tmp19 + stateJac[33] * tmp34) + stateJac[34] *
                  tmp35) + stateJac[35] * tmp36) + stateJac[30] * tmp20) +
               stateJac[31] * tmp21) + tmp33;

  // 'updateEskfCovP:176' covP(2, 10) = tmp23;
  covP[172] = tmp23;

  // 'updateEskfCovP:177' covP(2, 11) = tmp25;
  covP[191] = tmp25;

  // 'updateEskfCovP:178' covP(2, 12) = tmp27;
  covP[210] = tmp27;

  // 'updateEskfCovP:179' covP(2, 13) = tmp34;
  covP[229] = tmp34;

  // 'updateEskfCovP:180' covP(2, 14) = tmp35;
  covP[248] = tmp35;

  // 'updateEskfCovP:181' covP(2, 15) = tmp36;
  covP[267] = tmp36;

  // 'updateEskfCovP:182' covP(2, 16) = -covP(11, 16)*sampleTime_s + covP(1, 16)*stateJac(4) + covP(2, 16)*stateJac(5) + covP(3, 16)*stateJac(6); 
  covP[286] = ((-covP[295] * b_sampleTime_s + stateJac[3] * covP[285]) +
               stateJac[4] * covP[286]) + stateJac[5] * covP[287];

  // 'updateEskfCovP:183' covP(2, 17) = -covP(11, 17)*sampleTime_s + covP(1, 17)*stateJac(4) + covP(2, 17)*stateJac(5) + covP(3, 17)*stateJac(6); 
  covP[305] = ((-covP[314] * b_sampleTime_s + stateJac[3] * covP[304]) +
               stateJac[4] * covP[305]) + stateJac[5] * covP[306];

  // 'updateEskfCovP:184' covP(2, 18) = -covP(11, 18)*sampleTime_s + covP(1, 18)*stateJac(4) + covP(2, 18)*stateJac(5) + covP(3, 18)*stateJac(6); 
  covP[324] = ((-covP[333] * b_sampleTime_s + stateJac[3] * covP[323]) +
               stateJac[4] * covP[324]) + stateJac[5] * covP[325];

  // 'updateEskfCovP:185' covP(2, 19) = -covP(11, 19)*sampleTime_s + covP(1, 19)*stateJac(4) + covP(2, 19)*stateJac(5) + covP(3, 19)*stateJac(6); 
  covP[343] = ((-covP[352] * b_sampleTime_s + stateJac[3] * covP[342]) +
               stateJac[4] * covP[343]) + stateJac[5] * covP[344];

  // 'updateEskfCovP:186' covP(3, 1) = -sampleTime_s*tmp41 + stateJac(1)*tmp37 + stateJac(2)*tmp38 + stateJac(3)*tmp39; 
  covP[2] = ((-b_sampleTime_s * tmp41 + stateJac[0] * tmp37) + stateJac[1] *
             tmp38) + stateJac[2] * tmp39;

  // 'updateEskfCovP:187' covP(3, 2) = -sampleTime_s*tmp43 + stateJac(4)*tmp37 + stateJac(5)*tmp38 + stateJac(6)*tmp39; 
  covP[21] = ((-b_sampleTime_s * tmp43 + stateJac[3] * tmp37) + stateJac[4] *
              tmp38) + stateJac[5] * tmp39;

  // 'updateEskfCovP:188' covP(3, 3) = processNoiseQ(3, 3) - sampleTime_s*tmp45 + stateJac(7)*tmp37 + stateJac(8)*tmp38 + stateJac(9)*tmp39; 
  covP[40] = (((processNoiseQ[40] - b_sampleTime_s * tmp45) + stateJac[6] *
               tmp37) + stateJac[7] * tmp38) + stateJac[8] * tmp39;

  // 'updateEskfCovP:189' covP(3, 4) = -covP(12, 4)*sampleTime_s + covP(1, 4)*stateJac(7) + covP(2, 4)*stateJac(8) + covP(3, 4)*stateJac(9) + sampleTime_s*tmp47; 
  covP[59] = (((-covP[68] * b_sampleTime_s + stateJac[6] * covP[57]) + stateJac
               [7] * covP[58]) + stateJac[8] * covP[59]) + b_sampleTime_s *
    tmp47;

  // 'updateEskfCovP:190' covP(3, 5) = -covP(12, 5)*sampleTime_s + covP(1, 5)*stateJac(7) + covP(2, 5)*stateJac(8) + covP(3, 5)*stateJac(9) + sampleTime_s*tmp49; 
  covP[78] = (((-covP[87] * b_sampleTime_s + stateJac[6] * covP[76]) + stateJac
               [7] * covP[77]) + stateJac[8] * covP[78]) + b_sampleTime_s *
    tmp49;

  // 'updateEskfCovP:191' covP(3, 6) = -covP(12, 6)*sampleTime_s + covP(1, 6)*stateJac(7) + covP(2, 6)*stateJac(8) + covP(3, 6)*stateJac(9) + sampleTime_s*tmp51; 
  covP[97] = (((-covP[106] * b_sampleTime_s + stateJac[6] * covP[95]) +
               stateJac[7] * covP[96]) + stateJac[8] * covP[97]) +
    b_sampleTime_s * tmp51;

  // 'updateEskfCovP:192' covP(3, 7) = stateJac(16)*tmp37 + stateJac(20)*tmp52 + stateJac(21)*tmp53 + stateJac(22)*tmp54 + stateJac(17)*tmp38 + stateJac(18)*tmp39 + tmp47; 
  covP[116] = (((((stateJac[15] * tmp37 + stateJac[19] * tmp52) + stateJac[20] *
                  tmp53) + stateJac[21] * tmp54) + stateJac[16] * tmp38) +
               stateJac[17] * tmp39) + tmp47;

  // 'updateEskfCovP:193' covP(3, 8) = stateJac(23)*tmp37 + stateJac(27)*tmp52 + stateJac(28)*tmp53 + stateJac(29)*tmp54 + stateJac(24)*tmp38 + stateJac(25)*tmp39 + tmp49; 
  covP[135] = (((((stateJac[22] * tmp37 + stateJac[26] * tmp52) + stateJac[27] *
                  tmp53) + stateJac[28] * tmp54) + stateJac[23] * tmp38) +
               stateJac[24] * tmp39) + tmp49;

  // 'updateEskfCovP:194' covP(3, 9) = stateJac(30)*tmp37 + stateJac(34)*tmp52 + stateJac(35)*tmp53 + stateJac(36)*tmp54 + stateJac(31)*tmp38 + stateJac(32)*tmp39 + tmp51; 
  covP[154] = (((((stateJac[29] * tmp37 + stateJac[33] * tmp52) + stateJac[34] *
                  tmp53) + stateJac[35] * tmp54) + stateJac[30] * tmp38) +
               stateJac[31] * tmp39) + tmp51;

  // 'updateEskfCovP:195' covP(3, 10) = tmp41;
  covP[173] = tmp41;

  // 'updateEskfCovP:196' covP(3, 11) = tmp43;
  covP[192] = tmp43;

  // 'updateEskfCovP:197' covP(3, 12) = tmp45;
  covP[211] = tmp45;

  // 'updateEskfCovP:198' covP(3, 13) = tmp52;
  covP[230] = tmp52;

  // 'updateEskfCovP:199' covP(3, 14) = tmp53;
  covP[249] = tmp53;

  // 'updateEskfCovP:200' covP(3, 15) = tmp54;
  covP[268] = tmp54;

  // 'updateEskfCovP:201' covP(3, 16) = -covP(12, 16)*sampleTime_s + covP(1, 16)*stateJac(7) + covP(2, 16)*stateJac(8) + covP(3, 16)*stateJac(9); 
  covP[287] = ((-covP[296] * b_sampleTime_s + stateJac[6] * covP[285]) +
               stateJac[7] * covP[286]) + stateJac[8] * covP[287];

  // 'updateEskfCovP:202' covP(3, 17) = -covP(12, 17)*sampleTime_s + covP(1, 17)*stateJac(7) + covP(2, 17)*stateJac(8) + covP(3, 17)*stateJac(9); 
  covP[306] = ((-covP[315] * b_sampleTime_s + stateJac[6] * covP[304]) +
               stateJac[7] * covP[305]) + stateJac[8] * covP[306];

  // 'updateEskfCovP:203' covP(3, 18) = -covP(12, 18)*sampleTime_s + covP(1, 18)*stateJac(7) + covP(2, 18)*stateJac(8) + covP(3, 18)*stateJac(9); 
  covP[325] = ((-covP[334] * b_sampleTime_s + stateJac[6] * covP[323]) +
               stateJac[7] * covP[324]) + stateJac[8] * covP[325];

  // 'updateEskfCovP:204' covP(3, 19) = -covP(12, 19)*sampleTime_s + covP(1, 19)*stateJac(7) + covP(2, 19)*stateJac(8) + covP(3, 19)*stateJac(9); 
  covP[344] = ((-covP[353] * b_sampleTime_s + stateJac[6] * covP[342]) +
               stateJac[7] * covP[343]) + stateJac[8] * covP[344];

  // 'updateEskfCovP:205' covP(4, 1) = -sampleTime_s*tmp58 + stateJac(1)*tmp55 + stateJac(2)*tmp56 + stateJac(3)*tmp57; 
  covP[3] = ((-b_sampleTime_s * tmp58 + stateJac[0] * tmp55) + stateJac[1] *
             tmp56) + stateJac[2] * tmp57;

  // 'updateEskfCovP:206' covP(4, 2) = -sampleTime_s*tmp59 + stateJac(4)*tmp55 + stateJac(5)*tmp56 + stateJac(6)*tmp57; 
  covP[22] = ((-b_sampleTime_s * tmp59 + stateJac[3] * tmp55) + stateJac[4] *
              tmp56) + stateJac[5] * tmp57;

  // 'updateEskfCovP:207' covP(4, 3) = -sampleTime_s*tmp60 + stateJac(7)*tmp55 + stateJac(8)*tmp56 + stateJac(9)*tmp57; 
  covP[41] = ((-b_sampleTime_s * tmp60 + stateJac[6] * tmp55) + stateJac[7] *
              tmp56) + stateJac[8] * tmp57;

  // 'updateEskfCovP:208' covP(4, 4) = covP(4, 4) + covP(7, 4)*sampleTime_s + processNoiseQ(4, 4) + sampleTime_s*tmp61; 
  covP[60] = ((covP[63] * b_sampleTime_s + covP[60]) + processNoiseQ[60]) +
    b_sampleTime_s * tmp61;

  // 'updateEskfCovP:209' covP(4, 5) = covP(4, 5) + covP(7, 5)*sampleTime_s + sampleTime_s*tmp62; 
  covP[79] = (covP[82] * b_sampleTime_s + covP[79]) + b_sampleTime_s * tmp62;

  // 'updateEskfCovP:210' covP(4, 6) = covP(4, 6) + covP(7, 6)*sampleTime_s + sampleTime_s*tmp63; 
  covP[98] = (covP[101] * b_sampleTime_s + covP[98]) + b_sampleTime_s * tmp63;

  // 'updateEskfCovP:211' covP(4, 7) = stateJac(16)*tmp55 + stateJac(20)*tmp64 + stateJac(21)*tmp65 + stateJac(22)*tmp66 + stateJac(17)*tmp56 + stateJac(18)*tmp57 + tmp61; 
  covP[117] = (((((stateJac[15] * tmp55 + stateJac[19] * tmp64) + stateJac[20] *
                  tmp65) + stateJac[21] * tmp66) + stateJac[16] * tmp56) +
               stateJac[17] * tmp57) + tmp61;

  // 'updateEskfCovP:212' covP(4, 8) = stateJac(23)*tmp55 + stateJac(27)*tmp64 + stateJac(28)*tmp65 + stateJac(29)*tmp66 + stateJac(24)*tmp56 + stateJac(25)*tmp57 + tmp62; 
  covP[136] = (((((stateJac[22] * tmp55 + stateJac[26] * tmp64) + stateJac[27] *
                  tmp65) + stateJac[28] * tmp66) + stateJac[23] * tmp56) +
               stateJac[24] * tmp57) + tmp62;

  // 'updateEskfCovP:213' covP(4, 9) = stateJac(30)*tmp55 + stateJac(34)*tmp64 + stateJac(35)*tmp65 + stateJac(36)*tmp66 + stateJac(31)*tmp56 + stateJac(32)*tmp57 + tmp63; 
  covP[155] = (((((stateJac[29] * tmp55 + stateJac[33] * tmp64) + stateJac[34] *
                  tmp65) + stateJac[35] * tmp66) + stateJac[30] * tmp56) +
               stateJac[31] * tmp57) + tmp63;

  // 'updateEskfCovP:214' covP(4, 10) = tmp58;
  covP[174] = tmp58;

  // 'updateEskfCovP:215' covP(4, 11) = tmp59;
  covP[193] = tmp59;

  // 'updateEskfCovP:216' covP(4, 12) = tmp60;
  covP[212] = tmp60;

  // 'updateEskfCovP:217' covP(4, 13) = tmp64;
  covP[231] = tmp64;

  // 'updateEskfCovP:218' covP(4, 14) = tmp65;
  covP[250] = tmp65;

  // 'updateEskfCovP:219' covP(4, 15) = tmp66;
  covP[269] = tmp66;

  // 'updateEskfCovP:220' covP(4, 16) = covP(4, 16) + covP(7, 16)*sampleTime_s;
  covP[288] += covP[291] * b_sampleTime_s;

  // 'updateEskfCovP:221' covP(4, 17) = covP(4, 17) + covP(7, 17)*sampleTime_s;
  covP[307] += covP[310] * b_sampleTime_s;

  // 'updateEskfCovP:222' covP(4, 18) = covP(4, 18) + covP(7, 18)*sampleTime_s;
  covP[326] += covP[329] * b_sampleTime_s;

  // 'updateEskfCovP:223' covP(4, 19) = covP(4, 19) + covP(7, 19)*sampleTime_s;
  covP[345] += covP[348] * b_sampleTime_s;

  // 'updateEskfCovP:224' covP(5, 1) = -sampleTime_s*tmp70 + stateJac(1)*tmp67 + stateJac(2)*tmp68 + stateJac(3)*tmp69; 
  covP[4] = ((-b_sampleTime_s * tmp70 + stateJac[0] * tmp67) + stateJac[1] *
             tmp68) + stateJac[2] * tmp69;

  // 'updateEskfCovP:225' covP(5, 2) = -sampleTime_s*tmp71 + stateJac(4)*tmp67 + stateJac(5)*tmp68 + stateJac(6)*tmp69; 
  covP[23] = ((-b_sampleTime_s * tmp71 + stateJac[3] * tmp67) + stateJac[4] *
              tmp68) + stateJac[5] * tmp69;

  // 'updateEskfCovP:226' covP(5, 3) = -sampleTime_s*tmp72 + stateJac(7)*tmp67 + stateJac(8)*tmp68 + stateJac(9)*tmp69; 
  covP[42] = ((-b_sampleTime_s * tmp72 + stateJac[6] * tmp67) + stateJac[7] *
              tmp68) + stateJac[8] * tmp69;

  // 'updateEskfCovP:227' covP(5, 4) = covP(5, 4) + covP(8, 4)*sampleTime_s + sampleTime_s*tmp73; 
  covP[61] = (covP[64] * b_sampleTime_s + covP[61]) + b_sampleTime_s * tmp73;

  // 'updateEskfCovP:228' covP(5, 5) = covP(5, 5) + covP(8, 5)*sampleTime_s + processNoiseQ(5, 5) + sampleTime_s*tmp74; 
  covP[80] = ((covP[83] * b_sampleTime_s + covP[80]) + processNoiseQ[80]) +
    b_sampleTime_s * tmp74;

  // 'updateEskfCovP:229' covP(5, 6) = covP(5, 6) + covP(8, 6)*sampleTime_s + sampleTime_s*tmp75; 
  covP[99] = (covP[102] * b_sampleTime_s + covP[99]) + b_sampleTime_s * tmp75;

  // 'updateEskfCovP:230' covP(5, 7) = stateJac(16)*tmp67 + stateJac(20)*tmp76 + stateJac(21)*tmp77 + stateJac(22)*tmp78 + stateJac(17)*tmp68 + stateJac(18)*tmp69 + tmp73; 
  covP[118] = (((((stateJac[15] * tmp67 + stateJac[19] * tmp76) + stateJac[20] *
                  tmp77) + stateJac[21] * tmp78) + stateJac[16] * tmp68) +
               stateJac[17] * tmp69) + tmp73;

  // 'updateEskfCovP:231' covP(5, 8) = stateJac(23)*tmp67 + stateJac(27)*tmp76 + stateJac(28)*tmp77 + stateJac(29)*tmp78 + stateJac(24)*tmp68 + stateJac(25)*tmp69 + tmp74; 
  covP[137] = (((((stateJac[22] * tmp67 + stateJac[26] * tmp76) + stateJac[27] *
                  tmp77) + stateJac[28] * tmp78) + stateJac[23] * tmp68) +
               stateJac[24] * tmp69) + tmp74;

  // 'updateEskfCovP:232' covP(5, 9) = stateJac(30)*tmp67 + stateJac(34)*tmp76 + stateJac(35)*tmp77 + stateJac(36)*tmp78 + stateJac(31)*tmp68 + stateJac(32)*tmp69 + tmp75; 
  covP[156] = (((((stateJac[29] * tmp67 + stateJac[33] * tmp76) + stateJac[34] *
                  tmp77) + stateJac[35] * tmp78) + stateJac[30] * tmp68) +
               stateJac[31] * tmp69) + tmp75;

  // 'updateEskfCovP:233' covP(5, 10) = tmp70;
  covP[175] = tmp70;

  // 'updateEskfCovP:234' covP(5, 11) = tmp71;
  covP[194] = tmp71;

  // 'updateEskfCovP:235' covP(5, 12) = tmp72;
  covP[213] = tmp72;

  // 'updateEskfCovP:236' covP(5, 13) = tmp76;
  covP[232] = tmp76;

  // 'updateEskfCovP:237' covP(5, 14) = tmp77;
  covP[251] = tmp77;

  // 'updateEskfCovP:238' covP(5, 15) = tmp78;
  covP[270] = tmp78;

  // 'updateEskfCovP:239' covP(5, 16) = covP(5, 16) + covP(8, 16)*sampleTime_s;
  covP[289] += covP[292] * b_sampleTime_s;

  // 'updateEskfCovP:240' covP(5, 17) = covP(5, 17) + covP(8, 17)*sampleTime_s;
  covP[308] += covP[311] * b_sampleTime_s;

  // 'updateEskfCovP:241' covP(5, 18) = covP(5, 18) + covP(8, 18)*sampleTime_s;
  covP[327] += covP[330] * b_sampleTime_s;

  // 'updateEskfCovP:242' covP(5, 19) = covP(5, 19) + covP(8, 19)*sampleTime_s;
  covP[346] += covP[349] * b_sampleTime_s;

  // 'updateEskfCovP:243' covP(6, 1) = -sampleTime_s*tmp82 + stateJac(1)*tmp79 + stateJac(2)*tmp80 + stateJac(3)*tmp81; 
  covP[5] = ((-b_sampleTime_s * tmp82 + stateJac[0] * tmp79) + stateJac[1] *
             tmp80) + stateJac[2] * tmp81;

  // 'updateEskfCovP:244' covP(6, 2) = -sampleTime_s*tmp83 + stateJac(4)*tmp79 + stateJac(5)*tmp80 + stateJac(6)*tmp81; 
  covP[24] = ((-b_sampleTime_s * tmp83 + stateJac[3] * tmp79) + stateJac[4] *
              tmp80) + stateJac[5] * tmp81;

  // 'updateEskfCovP:245' covP(6, 3) = -sampleTime_s*tmp84 + stateJac(7)*tmp79 + stateJac(8)*tmp80 + stateJac(9)*tmp81; 
  covP[43] = ((-b_sampleTime_s * tmp84 + stateJac[6] * tmp79) + stateJac[7] *
              tmp80) + stateJac[8] * tmp81;

  // 'updateEskfCovP:246' covP(6, 4) = covP(6, 4) + covP(9, 4)*sampleTime_s + sampleTime_s*tmp85; 
  covP[62] = (covP[65] * b_sampleTime_s + covP[62]) + b_sampleTime_s * tmp85;

  // 'updateEskfCovP:247' covP(6, 5) = covP(6, 5) + covP(9, 5)*sampleTime_s + sampleTime_s*tmp86; 
  covP[81] = (covP[84] * b_sampleTime_s + covP[81]) + b_sampleTime_s * tmp86;

  // 'updateEskfCovP:248' covP(6, 6) = covP(6, 6) + covP(9, 6)*sampleTime_s + processNoiseQ(6, 6) + sampleTime_s*tmp87; 
  covP[100] = ((covP[103] * b_sampleTime_s + covP[100]) + processNoiseQ[100]) +
    b_sampleTime_s * tmp87;

  // 'updateEskfCovP:249' covP(6, 7) = stateJac(16)*tmp79 + stateJac(20)*tmp88 + stateJac(21)*tmp89 + stateJac(22)*tmp90 + stateJac(17)*tmp80 + stateJac(18)*tmp81 + tmp85; 
  covP[119] = (((((stateJac[15] * tmp79 + stateJac[19] * tmp88) + stateJac[20] *
                  tmp89) + stateJac[21] * tmp90) + stateJac[16] * tmp80) +
               stateJac[17] * tmp81) + tmp85;

  // 'updateEskfCovP:250' covP(6, 8) = stateJac(23)*tmp79 + stateJac(27)*tmp88 + stateJac(28)*tmp89 + stateJac(29)*tmp90 + stateJac(24)*tmp80 + stateJac(25)*tmp81 + tmp86; 
  covP[138] = (((((stateJac[22] * tmp79 + stateJac[26] * tmp88) + stateJac[27] *
                  tmp89) + stateJac[28] * tmp90) + stateJac[23] * tmp80) +
               stateJac[24] * tmp81) + tmp86;

  // 'updateEskfCovP:251' covP(6, 9) = stateJac(30)*tmp79 + stateJac(34)*tmp88 + stateJac(35)*tmp89 + stateJac(36)*tmp90 + stateJac(31)*tmp80 + stateJac(32)*tmp81 + tmp87; 
  covP[157] = (((((stateJac[29] * tmp79 + stateJac[33] * tmp88) + stateJac[34] *
                  tmp89) + stateJac[35] * tmp90) + stateJac[30] * tmp80) +
               stateJac[31] * tmp81) + tmp87;

  // 'updateEskfCovP:252' covP(6, 10) = tmp82;
  covP[176] = tmp82;

  // 'updateEskfCovP:253' covP(6, 11) = tmp83;
  covP[195] = tmp83;

  // 'updateEskfCovP:254' covP(6, 12) = tmp84;
  covP[214] = tmp84;

  // 'updateEskfCovP:255' covP(6, 13) = tmp88;
  covP[233] = tmp88;

  // 'updateEskfCovP:256' covP(6, 14) = tmp89;
  covP[252] = tmp89;

  // 'updateEskfCovP:257' covP(6, 15) = tmp90;
  covP[271] = tmp90;

  // 'updateEskfCovP:258' covP(6, 16) = covP(6, 16) + covP(9, 16)*sampleTime_s;
  covP[290] += covP[293] * b_sampleTime_s;

  // 'updateEskfCovP:259' covP(6, 17) = covP(6, 17) + covP(9, 17)*sampleTime_s;
  covP[309] += covP[312] * b_sampleTime_s;

  // 'updateEskfCovP:260' covP(6, 18) = covP(6, 18) + covP(9, 18)*sampleTime_s;
  covP[328] += covP[331] * b_sampleTime_s;

  // 'updateEskfCovP:261' covP(6, 19) = covP(6, 19) + covP(9, 19)*sampleTime_s;
  covP[347] += covP[350] * b_sampleTime_s;

  // 'updateEskfCovP:262' covP(7, 1) = -sampleTime_s*tmp94 + stateJac(1)*tmp91 + stateJac(2)*tmp92 + stateJac(3)*tmp93; 
  covP[6] = ((-b_sampleTime_s * tmp94 + stateJac[0] * tmp91) + stateJac[1] *
             tmp92) + stateJac[2] * tmp93;

  // 'updateEskfCovP:263' covP(7, 2) = -sampleTime_s*tmp95 + stateJac(4)*tmp91 + stateJac(5)*tmp92 + stateJac(6)*tmp93; 
  covP[25] = ((-b_sampleTime_s * tmp95 + stateJac[3] * tmp91) + stateJac[4] *
              tmp92) + stateJac[5] * tmp93;

  // 'updateEskfCovP:264' covP(7, 3) = -sampleTime_s*tmp96 + stateJac(7)*tmp91 + stateJac(8)*tmp92 + stateJac(9)*tmp93; 
  covP[44] = ((-b_sampleTime_s * tmp96 + stateJac[6] * tmp91) + stateJac[7] *
              tmp92) + stateJac[8] * tmp93;

  // 'updateEskfCovP:265' covP(7, 4) = covP(13, 4)*stateJac(20) + covP(14, 4)*stateJac(21) + covP(15, 4)*stateJac(22) + covP(1, 4)*stateJac(16) + covP(2, 4)*stateJac(17) + covP(3, 4)*stateJac(18) + covP(7, 4) + sampleTime_s*tmp97; 
  covP[63] = ((((((stateJac[19] * covP[69] + stateJac[20] * covP[70]) +
                  stateJac[21] * covP[71]) + stateJac[15] * covP[57]) +
                stateJac[16] * covP[58]) + stateJac[17] * covP[59]) + covP[63])
    + b_sampleTime_s * tmp97;

  // 'updateEskfCovP:266' covP(7, 5) = covP(13, 5)*stateJac(20) + covP(14, 5)*stateJac(21) + covP(15, 5)*stateJac(22) + covP(1, 5)*stateJac(16) + covP(2, 5)*stateJac(17) + covP(3, 5)*stateJac(18) + covP(7, 5) + sampleTime_s*tmp98; 
  covP[82] = ((((((stateJac[19] * covP[88] + stateJac[20] * covP[89]) +
                  stateJac[21] * covP[90]) + stateJac[15] * covP[76]) +
                stateJac[16] * covP[77]) + stateJac[17] * covP[78]) + covP[82])
    + b_sampleTime_s * tmp98;

  // 'updateEskfCovP:267' covP(7, 6) = covP(13, 6)*stateJac(20) + covP(14, 6)*stateJac(21) + covP(15, 6)*stateJac(22) + covP(1, 6)*stateJac(16) + covP(2, 6)*stateJac(17) + covP(3, 6)*stateJac(18) + covP(7, 6) + sampleTime_s*tmp99; 
  covP[101] = ((((((stateJac[19] * covP[107] + stateJac[20] * covP[108]) +
                   stateJac[21] * covP[109]) + stateJac[15] * covP[95]) +
                 stateJac[16] * covP[96]) + stateJac[17] * covP[97]) + covP[101])
    + b_sampleTime_s * tmp99;

  // 'updateEskfCovP:268' covP(7, 7) = processNoiseQ(7, 7) + stateJac(16)*tmp91 + stateJac(20)*tmp101 + stateJac(21)*tmp103 + stateJac(22)*tmp105 + stateJac(17)*tmp92 + stateJac(18)*tmp93 + tmp97; 
  covP[120] = ((((((stateJac[15] * tmp91 + processNoiseQ[120]) + stateJac[19] *
                   tmp101) + stateJac[20] * tmp103) + stateJac[21] * tmp105) +
                stateJac[16] * tmp92) + stateJac[17] * tmp93) + tmp97;

  // 'updateEskfCovP:269' covP(7, 8) = stateJac(23)*tmp91 + stateJac(27)*tmp101 + stateJac(28)*tmp103 + stateJac(29)*tmp105 + stateJac(24)*tmp92 + stateJac(25)*tmp93 + tmp98; 
  covP[139] = (((((stateJac[22] * tmp91 + stateJac[26] * tmp101) + stateJac[27] *
                  tmp103) + stateJac[28] * tmp105) + stateJac[23] * tmp92) +
               stateJac[24] * tmp93) + tmp98;

  // 'updateEskfCovP:270' covP(7, 9) = stateJac(30)*tmp91 + stateJac(34)*tmp101 + stateJac(35)*tmp103 + stateJac(36)*tmp105 + stateJac(31)*tmp92 + stateJac(32)*tmp93 + tmp99; 
  covP[158] = (((((stateJac[29] * tmp91 + stateJac[33] * tmp101) + stateJac[34] *
                  tmp103) + stateJac[35] * tmp105) + stateJac[30] * tmp92) +
               stateJac[31] * tmp93) + tmp99;

  // 'updateEskfCovP:271' covP(7, 10) = tmp94;
  covP[177] = tmp94;

  // 'updateEskfCovP:272' covP(7, 11) = tmp95;
  covP[196] = tmp95;

  // 'updateEskfCovP:273' covP(7, 12) = tmp96;
  covP[215] = tmp96;

  // 'updateEskfCovP:274' covP(7, 13) = tmp101;
  covP[234] = tmp101;

  // 'updateEskfCovP:275' covP(7, 14) = tmp103;
  covP[253] = tmp103;

  // 'updateEskfCovP:276' covP(7, 15) = tmp105;
  covP[272] = tmp105;

  // 'updateEskfCovP:277' covP(7, 16) = covP(13, 16)*stateJac(20) + covP(14, 16)*stateJac(21) + covP(15, 16)*stateJac(22) + covP(1, 16)*stateJac(16) + covP(2, 16)*stateJac(17) + covP(3, 16)*stateJac(18) + covP(7, 16); 
  covP[291] += ((((stateJac[19] * covP[297] + stateJac[20] * covP[298]) +
                  stateJac[21] * covP[299]) + stateJac[15] * covP[285]) +
                stateJac[16] * covP[286]) + stateJac[17] * covP[287];

  // 'updateEskfCovP:278' covP(7, 17) = covP(13, 17)*stateJac(20) + covP(14, 17)*stateJac(21) + covP(15, 17)*stateJac(22) + covP(1, 17)*stateJac(16) + covP(2, 17)*stateJac(17) + covP(3, 17)*stateJac(18) + covP(7, 17); 
  covP[310] += ((((stateJac[19] * covP[316] + stateJac[20] * covP[317]) +
                  stateJac[21] * covP[318]) + stateJac[15] * covP[304]) +
                stateJac[16] * covP[305]) + stateJac[17] * covP[306];

  // 'updateEskfCovP:279' covP(7, 18) = covP(13, 18)*stateJac(20) + covP(14, 18)*stateJac(21) + covP(15, 18)*stateJac(22) + covP(1, 18)*stateJac(16) + covP(2, 18)*stateJac(17) + covP(3, 18)*stateJac(18) + covP(7, 18); 
  covP[329] += ((((stateJac[19] * covP[335] + stateJac[20] * covP[336]) +
                  stateJac[21] * covP[337]) + stateJac[15] * covP[323]) +
                stateJac[16] * covP[324]) + stateJac[17] * covP[325];

  // 'updateEskfCovP:280' covP(7, 19) = covP(13, 19)*stateJac(20) + covP(14, 19)*stateJac(21) + covP(15, 19)*stateJac(22) + covP(1, 19)*stateJac(16) + covP(2, 19)*stateJac(17) + covP(3, 19)*stateJac(18) + covP(7, 19); 
  covP[348] += ((((stateJac[19] * covP[354] + stateJac[20] * covP[355]) +
                  stateJac[21] * covP[356]) + stateJac[15] * covP[342]) +
                stateJac[16] * covP[343]) + stateJac[17] * covP[344];

  // 'updateEskfCovP:281' covP(8, 1) = -sampleTime_s*tmp109 + stateJac(1)*tmp106 + stateJac(2)*tmp107 + stateJac(3)*tmp108; 
  covP[7] = ((-b_sampleTime_s * tmp109 + stateJac[0] * tmp106) + stateJac[1] *
             tmp107) + stateJac[2] * tmp108;

  // 'updateEskfCovP:282' covP(8, 2) = -sampleTime_s*tmp110 + stateJac(4)*tmp106 + stateJac(5)*tmp107 + stateJac(6)*tmp108; 
  covP[26] = ((-b_sampleTime_s * tmp110 + stateJac[3] * tmp106) + stateJac[4] *
              tmp107) + stateJac[5] * tmp108;

  // 'updateEskfCovP:283' covP(8, 3) = -sampleTime_s*tmp111 + stateJac(7)*tmp106 + stateJac(8)*tmp107 + stateJac(9)*tmp108; 
  covP[45] = ((-b_sampleTime_s * tmp111 + stateJac[6] * tmp106) + stateJac[7] *
              tmp107) + stateJac[8] * tmp108;

  // 'updateEskfCovP:284' covP(8, 4) = covP(13, 4)*stateJac(27) + covP(14, 4)*stateJac(28) + covP(15, 4)*stateJac(29) + covP(1, 4)*stateJac(23) + covP(2, 4)*stateJac(24) + covP(3, 4)*stateJac(25) + covP(8, 4) + sampleTime_s*tmp112; 
  covP[64] = ((((((stateJac[26] * covP[69] + stateJac[27] * covP[70]) +
                  stateJac[28] * covP[71]) + stateJac[22] * covP[57]) +
                stateJac[23] * covP[58]) + stateJac[24] * covP[59]) + covP[64])
    + b_sampleTime_s * tmp112;

  // 'updateEskfCovP:285' covP(8, 5) = covP(13, 5)*stateJac(27) + covP(14, 5)*stateJac(28) + covP(15, 5)*stateJac(29) + covP(1, 5)*stateJac(23) + covP(2, 5)*stateJac(24) + covP(3, 5)*stateJac(25) + covP(8, 5) + sampleTime_s*tmp113; 
  covP[83] = ((((((stateJac[26] * covP[88] + stateJac[27] * covP[89]) +
                  stateJac[28] * covP[90]) + stateJac[22] * covP[76]) +
                stateJac[23] * covP[77]) + stateJac[24] * covP[78]) + covP[83])
    + b_sampleTime_s * tmp113;

  // 'updateEskfCovP:286' covP(8, 6) = covP(13, 6)*stateJac(27) + covP(14, 6)*stateJac(28) + covP(15, 6)*stateJac(29) + covP(1, 6)*stateJac(23) + covP(2, 6)*stateJac(24) + covP(3, 6)*stateJac(25) + covP(8, 6) + sampleTime_s*tmp114; 
  covP[102] = ((((((stateJac[26] * covP[107] + stateJac[27] * covP[108]) +
                   stateJac[28] * covP[109]) + stateJac[22] * covP[95]) +
                 stateJac[23] * covP[96]) + stateJac[24] * covP[97]) + covP[102])
    + b_sampleTime_s * tmp114;

  // 'updateEskfCovP:287' covP(8, 7) = stateJac(16)*tmp106 + stateJac(20)*tmp116 + stateJac(21)*tmp118 + stateJac(22)*tmp120 + stateJac(17)*tmp107 + stateJac(18)*tmp108 + tmp112; 
  covP[121] = (((((stateJac[15] * tmp106 + stateJac[19] * tmp116) + stateJac[20]
                  * tmp118) + stateJac[21] * tmp120) + stateJac[16] * tmp107) +
               stateJac[17] * tmp108) + tmp112;

  // 'updateEskfCovP:288' covP(8, 8) = processNoiseQ(8, 8) + stateJac(23)*tmp106 + stateJac(27)*tmp116 + stateJac(28)*tmp118 + stateJac(29)*tmp120 + stateJac(24)*tmp107 + stateJac(25)*tmp108 + tmp113; 
  covP[140] = ((((((stateJac[22] * tmp106 + processNoiseQ[140]) + stateJac[26] *
                   tmp116) + stateJac[27] * tmp118) + stateJac[28] * tmp120) +
                stateJac[23] * tmp107) + stateJac[24] * tmp108) + tmp113;

  // 'updateEskfCovP:289' covP(8, 9) = stateJac(30)*tmp106 + stateJac(34)*tmp116 + stateJac(35)*tmp118 + stateJac(36)*tmp120 + stateJac(31)*tmp107 + stateJac(32)*tmp108 + tmp114; 
  covP[159] = (((((stateJac[29] * tmp106 + stateJac[33] * tmp116) + stateJac[34]
                  * tmp118) + stateJac[35] * tmp120) + stateJac[30] * tmp107) +
               stateJac[31] * tmp108) + tmp114;

  // 'updateEskfCovP:290' covP(8, 10) = tmp109;
  covP[178] = tmp109;

  // 'updateEskfCovP:291' covP(8, 11) = tmp110;
  covP[197] = tmp110;

  // 'updateEskfCovP:292' covP(8, 12) = tmp111;
  covP[216] = tmp111;

  // 'updateEskfCovP:293' covP(8, 13) = tmp116;
  covP[235] = tmp116;

  // 'updateEskfCovP:294' covP(8, 14) = tmp118;
  covP[254] = tmp118;

  // 'updateEskfCovP:295' covP(8, 15) = tmp120;
  covP[273] = tmp120;

  // 'updateEskfCovP:296' covP(8, 16) = covP(13, 16)*stateJac(27) + covP(14, 16)*stateJac(28) + covP(15, 16)*stateJac(29) + covP(1, 16)*stateJac(23) + covP(2, 16)*stateJac(24) + covP(3, 16)*stateJac(25) + covP(8, 16); 
  covP[292] += ((((stateJac[26] * covP[297] + stateJac[27] * covP[298]) +
                  stateJac[28] * covP[299]) + stateJac[22] * covP[285]) +
                stateJac[23] * covP[286]) + stateJac[24] * covP[287];

  // 'updateEskfCovP:297' covP(8, 17) = covP(13, 17)*stateJac(27) + covP(14, 17)*stateJac(28) + covP(15, 17)*stateJac(29) + covP(1, 17)*stateJac(23) + covP(2, 17)*stateJac(24) + covP(3, 17)*stateJac(25) + covP(8, 17); 
  covP[311] += ((((stateJac[26] * covP[316] + stateJac[27] * covP[317]) +
                  stateJac[28] * covP[318]) + stateJac[22] * covP[304]) +
                stateJac[23] * covP[305]) + stateJac[24] * covP[306];

  // 'updateEskfCovP:298' covP(8, 18) = covP(13, 18)*stateJac(27) + covP(14, 18)*stateJac(28) + covP(15, 18)*stateJac(29) + covP(1, 18)*stateJac(23) + covP(2, 18)*stateJac(24) + covP(3, 18)*stateJac(25) + covP(8, 18); 
  covP[330] += ((((stateJac[26] * covP[335] + stateJac[27] * covP[336]) +
                  stateJac[28] * covP[337]) + stateJac[22] * covP[323]) +
                stateJac[23] * covP[324]) + stateJac[24] * covP[325];

  // 'updateEskfCovP:299' covP(8, 19) = covP(13, 19)*stateJac(27) + covP(14, 19)*stateJac(28) + covP(15, 19)*stateJac(29) + covP(1, 19)*stateJac(23) + covP(2, 19)*stateJac(24) + covP(3, 19)*stateJac(25) + covP(8, 19); 
  covP[349] += ((((stateJac[26] * covP[354] + stateJac[27] * covP[355]) +
                  stateJac[28] * covP[356]) + stateJac[22] * covP[342]) +
                stateJac[23] * covP[343]) + stateJac[24] * covP[344];

  // 'updateEskfCovP:300' covP(9, 1) = -sampleTime_s*tmp124 + stateJac(1)*tmp121 + stateJac(2)*tmp122 + stateJac(3)*tmp123; 
  covP[8] = ((-b_sampleTime_s * tmp124 + stateJac[0] * tmp121) + stateJac[1] *
             tmp122) + stateJac[2] * tmp123;

  // 'updateEskfCovP:301' covP(9, 2) = -sampleTime_s*tmp125 + stateJac(4)*tmp121 + stateJac(5)*tmp122 + stateJac(6)*tmp123; 
  covP[27] = ((-b_sampleTime_s * tmp125 + stateJac[3] * tmp121) + stateJac[4] *
              tmp122) + stateJac[5] * tmp123;

  // 'updateEskfCovP:302' covP(9, 3) = -sampleTime_s*tmp126 + stateJac(7)*tmp121 + stateJac(8)*tmp122 + stateJac(9)*tmp123; 
  covP[46] = ((-b_sampleTime_s * tmp126 + stateJac[6] * tmp121) + stateJac[7] *
              tmp122) + stateJac[8] * tmp123;

  // 'updateEskfCovP:303' covP(9, 4) = covP(13, 4)*stateJac(34) + covP(14, 4)*stateJac(35) + covP(15, 4)*stateJac(36) + covP(1, 4)*stateJac(30) + covP(2, 4)*stateJac(31) + covP(3, 4)*stateJac(32) + covP(9, 4) + sampleTime_s*tmp127; 
  covP[65] = ((((((stateJac[33] * covP[69] + stateJac[34] * covP[70]) +
                  stateJac[35] * covP[71]) + stateJac[29] * covP[57]) +
                stateJac[30] * covP[58]) + stateJac[31] * covP[59]) + covP[65])
    + b_sampleTime_s * tmp127;

  // 'updateEskfCovP:304' covP(9, 5) = covP(13, 5)*stateJac(34) + covP(14, 5)*stateJac(35) + covP(15, 5)*stateJac(36) + covP(1, 5)*stateJac(30) + covP(2, 5)*stateJac(31) + covP(3, 5)*stateJac(32) + covP(9, 5) + sampleTime_s*tmp128; 
  covP[84] = ((((((stateJac[33] * covP[88] + stateJac[34] * covP[89]) +
                  stateJac[35] * covP[90]) + stateJac[29] * covP[76]) +
                stateJac[30] * covP[77]) + stateJac[31] * covP[78]) + covP[84])
    + b_sampleTime_s * tmp128;

  // 'updateEskfCovP:305' covP(9, 6) = covP(13, 6)*stateJac(34) + covP(14, 6)*stateJac(35) + covP(15, 6)*stateJac(36) + covP(1, 6)*stateJac(30) + covP(2, 6)*stateJac(31) + covP(3, 6)*stateJac(32) + covP(9, 6) + sampleTime_s*tmp129; 
  covP[103] = ((((((stateJac[33] * covP[107] + stateJac[34] * covP[108]) +
                   stateJac[35] * covP[109]) + stateJac[29] * covP[95]) +
                 stateJac[30] * covP[96]) + stateJac[31] * covP[97]) + covP[103])
    + b_sampleTime_s * tmp129;

  // 'updateEskfCovP:306' covP(9, 7) = stateJac(16)*tmp121 + stateJac(20)*tmp131 + stateJac(21)*tmp133 + stateJac(22)*tmp135 + stateJac(17)*tmp122 + stateJac(18)*tmp123 + tmp127; 
  covP[122] = (((((stateJac[15] * tmp121 + stateJac[19] * tmp131) + stateJac[20]
                  * tmp133) + stateJac[21] * tmp135) + stateJac[16] * tmp122) +
               stateJac[17] * tmp123) + tmp127;

  // 'updateEskfCovP:307' covP(9, 8) = stateJac(23)*tmp121 + stateJac(27)*tmp131 + stateJac(28)*tmp133 + stateJac(29)*tmp135 + stateJac(24)*tmp122 + stateJac(25)*tmp123 + tmp128; 
  covP[141] = (((((stateJac[22] * tmp121 + stateJac[26] * tmp131) + stateJac[27]
                  * tmp133) + stateJac[28] * tmp135) + stateJac[23] * tmp122) +
               stateJac[24] * tmp123) + tmp128;

  // 'updateEskfCovP:308' covP(9, 9) = processNoiseQ(9, 9) + stateJac(30)*tmp121 + stateJac(34)*tmp131 + stateJac(35)*tmp133 + stateJac(36)*tmp135 + stateJac(31)*tmp122 + stateJac(32)*tmp123 + tmp129; 
  covP[160] = ((((((stateJac[29] * tmp121 + processNoiseQ[160]) + stateJac[33] *
                   tmp131) + stateJac[34] * tmp133) + stateJac[35] * tmp135) +
                stateJac[30] * tmp122) + stateJac[31] * tmp123) + tmp129;

  // 'updateEskfCovP:309' covP(9, 10) = tmp124;
  covP[179] = tmp124;

  // 'updateEskfCovP:310' covP(9, 11) = tmp125;
  covP[198] = tmp125;

  // 'updateEskfCovP:311' covP(9, 12) = tmp126;
  covP[217] = tmp126;

  // 'updateEskfCovP:312' covP(9, 13) = tmp131;
  covP[236] = tmp131;

  // 'updateEskfCovP:313' covP(9, 14) = tmp133;
  covP[255] = tmp133;

  // 'updateEskfCovP:314' covP(9, 15) = tmp135;
  covP[274] = tmp135;

  // 'updateEskfCovP:315' covP(9, 16) = covP(13, 16)*stateJac(34) + covP(14, 16)*stateJac(35) + covP(15, 16)*stateJac(36) + covP(1, 16)*stateJac(30) + covP(2, 16)*stateJac(31) + covP(3, 16)*stateJac(32) + covP(9, 16); 
  covP[293] += ((((stateJac[33] * covP[297] + stateJac[34] * covP[298]) +
                  stateJac[35] * covP[299]) + stateJac[29] * covP[285]) +
                stateJac[30] * covP[286]) + stateJac[31] * covP[287];

  // 'updateEskfCovP:316' covP(9, 17) = covP(13, 17)*stateJac(34) + covP(14, 17)*stateJac(35) + covP(15, 17)*stateJac(36) + covP(1, 17)*stateJac(30) + covP(2, 17)*stateJac(31) + covP(3, 17)*stateJac(32) + covP(9, 17); 
  covP[312] += ((((stateJac[33] * covP[316] + stateJac[34] * covP[317]) +
                  stateJac[35] * covP[318]) + stateJac[29] * covP[304]) +
                stateJac[30] * covP[305]) + stateJac[31] * covP[306];

  // 'updateEskfCovP:317' covP(9, 18) = covP(13, 18)*stateJac(34) + covP(14, 18)*stateJac(35) + covP(15, 18)*stateJac(36) + covP(1, 18)*stateJac(30) + covP(2, 18)*stateJac(31) + covP(3, 18)*stateJac(32) + covP(9, 18); 
  covP[331] += ((((stateJac[33] * covP[335] + stateJac[34] * covP[336]) +
                  stateJac[35] * covP[337]) + stateJac[29] * covP[323]) +
                stateJac[30] * covP[324]) + stateJac[31] * covP[325];

  // 'updateEskfCovP:318' covP(9, 19) = covP(13, 19)*stateJac(34) + covP(14, 19)*stateJac(35) + covP(15, 19)*stateJac(36) + covP(1, 19)*stateJac(30) + covP(2, 19)*stateJac(31) + covP(3, 19)*stateJac(32) + covP(9, 19); 
  covP[350] += ((((stateJac[33] * covP[354] + stateJac[34] * covP[355]) +
                  stateJac[35] * covP[356]) + stateJac[29] * covP[342]) +
                stateJac[30] * covP[343]) + stateJac[31] * covP[344];

  // 'updateEskfCovP:319' covP(10, 1) = covP(10, 1)*stateJac(1) + covP(10, 2)*stateJac(2) + covP(10, 3)*stateJac(3) + tmp4; 
  covP[9] = ((stateJac[0] * covP[9] + stateJac[1] * covP[28]) + stateJac[2] *
             covP[47]) + tmp4;

  // 'updateEskfCovP:320' covP(10, 2) = covP(10, 1)*stateJac(4) + covP(10, 2)*stateJac(5) + covP(10, 3)*stateJac(6) + tmp6; 
  covP[28] = ((stateJac[3] * covP[9] + stateJac[4] * covP[28]) + stateJac[5] *
              covP[47]) + tmp6;

  // 'updateEskfCovP:321' covP(10, 3) = covP(10, 1)*stateJac(7) + covP(10, 2)*stateJac(8) + covP(10, 3)*stateJac(9) + tmp8; 
  covP[47] = ((stateJac[6] * covP[9] + stateJac[7] * covP[28]) + stateJac[8] *
              covP[47]) + tmp8;

  // 'updateEskfCovP:322' covP(10, 4) = covP(10, 4) + tmp10;
  covP[66] += tmp10;

  // 'updateEskfCovP:323' covP(10, 5) = covP(10, 5) + tmp12;
  covP[85] += tmp12;

  // 'updateEskfCovP:324' covP(10, 6) = covP(10, 6) + tmp14;
  covP[104] += tmp14;

  // 'updateEskfCovP:325' covP(10, 7) = covP(10, 1)*stateJac(16) + covP(10, 13)*stateJac(20) + covP(10, 14)*stateJac(21) + covP(10, 15)*stateJac(22) + covP(10, 2)*stateJac(17) + covP(10, 3)*stateJac(18) + covP(10, 7); 
  covP[123] += ((((covP[9] * stateJac[15] + stateJac[19] * covP[237]) +
                  stateJac[20] * covP[256]) + stateJac[21] * covP[275]) +
                stateJac[16] * covP[28]) + stateJac[17] * covP[47];

  // 'updateEskfCovP:326' covP(10, 8) = covP(10, 1)*stateJac(23) + covP(10, 13)*stateJac(27) + covP(10, 14)*stateJac(28) + covP(10, 15)*stateJac(29) + covP(10, 2)*stateJac(24) + covP(10, 3)*stateJac(25) + covP(10, 8); 
  covP[142] += ((((covP[9] * stateJac[22] + stateJac[26] * covP[237]) +
                  stateJac[27] * covP[256]) + stateJac[28] * covP[275]) +
                stateJac[23] * covP[28]) + stateJac[24] * covP[47];

  // 'updateEskfCovP:327' covP(10, 9) = covP(10, 1)*stateJac(30) + covP(10, 13)*stateJac(34) + covP(10, 14)*stateJac(35) + covP(10, 15)*stateJac(36) + covP(10, 2)*stateJac(31) + covP(10, 3)*stateJac(32) + covP(10, 9); 
  covP[161] += ((((covP[9] * stateJac[29] + stateJac[33] * covP[237]) +
                  stateJac[34] * covP[256]) + stateJac[35] * covP[275]) + covP
                [28] * stateJac[30]) + stateJac[31] * covP[47];

  // 'updateEskfCovP:328' covP(10, 10) = covP(10, 10) + processNoiseQ(10, 10);
  covP[180] += processNoiseQ[180];

  // 'updateEskfCovP:329' covP(11, 1) = covP(11, 1)*stateJac(1) + covP(11, 2)*stateJac(2) + covP(11, 3)*stateJac(3) + tmp22; 
  covP[10] = ((stateJac[0] * covP[10] + stateJac[1] * covP[29]) + stateJac[2] *
              covP[48]) + tmp22;

  // 'updateEskfCovP:330' covP(11, 2) = covP(11, 1)*stateJac(4) + covP(11, 2)*stateJac(5) + covP(11, 3)*stateJac(6) + tmp24; 
  covP[29] = ((stateJac[3] * covP[10] + stateJac[4] * covP[29]) + stateJac[5] *
              covP[48]) + tmp24;

  // 'updateEskfCovP:331' covP(11, 3) = covP(11, 1)*stateJac(7) + covP(11, 2)*stateJac(8) + covP(11, 3)*stateJac(9) + tmp26; 
  covP[48] = ((stateJac[6] * covP[10] + stateJac[7] * covP[29]) + stateJac[8] *
              covP[48]) + tmp26;

  // 'updateEskfCovP:332' covP(11, 4) = covP(11, 4) + tmp28;
  covP[67] += tmp28;

  // 'updateEskfCovP:333' covP(11, 5) = covP(11, 5) + tmp30;
  covP[86] += tmp30;

  // 'updateEskfCovP:334' covP(11, 6) = covP(11, 6) + tmp32;
  covP[105] += tmp32;

  // 'updateEskfCovP:335' covP(11, 7) = covP(11, 1)*stateJac(16) + covP(11, 13)*stateJac(20) + covP(11, 14)*stateJac(21) + covP(11, 15)*stateJac(22) + covP(11, 2)*stateJac(17) + covP(11, 3)*stateJac(18) + covP(11, 7); 
  covP[124] += ((((covP[10] * stateJac[15] + stateJac[19] * covP[238]) +
                  stateJac[20] * covP[257]) + stateJac[21] * covP[276]) +
                stateJac[16] * covP[29]) + stateJac[17] * covP[48];

  // 'updateEskfCovP:336' covP(11, 8) = covP(11, 1)*stateJac(23) + covP(11, 13)*stateJac(27) + covP(11, 14)*stateJac(28) + covP(11, 15)*stateJac(29) + covP(11, 2)*stateJac(24) + covP(11, 3)*stateJac(25) + covP(11, 8); 
  covP[143] += ((((covP[10] * stateJac[22] + stateJac[26] * covP[238]) +
                  stateJac[27] * covP[257]) + stateJac[28] * covP[276]) +
                stateJac[23] * covP[29]) + stateJac[24] * covP[48];

  // 'updateEskfCovP:337' covP(11, 9) = covP(11, 1)*stateJac(30) + covP(11, 13)*stateJac(34) + covP(11, 14)*stateJac(35) + covP(11, 15)*stateJac(36) + covP(11, 2)*stateJac(31) + covP(11, 3)*stateJac(32) + covP(11, 9); 
  covP[162] += ((((covP[10] * stateJac[29] + stateJac[33] * covP[238]) +
                  stateJac[34] * covP[257]) + stateJac[35] * covP[276]) + covP
                [29] * stateJac[30]) + stateJac[31] * covP[48];

  // 'updateEskfCovP:338' covP(11, 11) = covP(11, 11) + processNoiseQ(11, 11);
  covP[200] += processNoiseQ[200];

  // 'updateEskfCovP:339' covP(12, 1) = covP(12, 1)*stateJac(1) + covP(12, 2)*stateJac(2) + covP(12, 3)*stateJac(3) + tmp40; 
  covP[11] = ((stateJac[0] * covP[11] + stateJac[1] * covP[30]) + stateJac[2] *
              covP[49]) + tmp40;

  // 'updateEskfCovP:340' covP(12, 2) = covP(12, 1)*stateJac(4) + covP(12, 2)*stateJac(5) + covP(12, 3)*stateJac(6) + tmp42; 
  covP[30] = ((stateJac[3] * covP[11] + stateJac[4] * covP[30]) + stateJac[5] *
              covP[49]) + tmp42;

  // 'updateEskfCovP:341' covP(12, 3) = covP(12, 1)*stateJac(7) + covP(12, 2)*stateJac(8) + covP(12, 3)*stateJac(9) + tmp44; 
  covP[49] = ((stateJac[6] * covP[11] + stateJac[7] * covP[30]) + stateJac[8] *
              covP[49]) + tmp44;

  // 'updateEskfCovP:342' covP(12, 4) = covP(12, 4) + tmp46;
  covP[68] += tmp46;

  // 'updateEskfCovP:343' covP(12, 5) = covP(12, 5) + tmp48;
  covP[87] += tmp48;

  // 'updateEskfCovP:344' covP(12, 6) = covP(12, 6) + tmp50;
  covP[106] += tmp50;

  // 'updateEskfCovP:345' covP(12, 7) = covP(12, 1)*stateJac(16) + covP(12, 13)*stateJac(20) + covP(12, 14)*stateJac(21) + covP(12, 15)*stateJac(22) + covP(12, 2)*stateJac(17) + covP(12, 3)*stateJac(18) + covP(12, 7); 
  covP[125] += ((((covP[11] * stateJac[15] + stateJac[19] * covP[239]) +
                  stateJac[20] * covP[258]) + stateJac[21] * covP[277]) +
                stateJac[16] * covP[30]) + stateJac[17] * covP[49];

  // 'updateEskfCovP:346' covP(12, 8) = covP(12, 1)*stateJac(23) + covP(12, 13)*stateJac(27) + covP(12, 14)*stateJac(28) + covP(12, 15)*stateJac(29) + covP(12, 2)*stateJac(24) + covP(12, 3)*stateJac(25) + covP(12, 8); 
  covP[144] += ((((covP[11] * stateJac[22] + stateJac[26] * covP[239]) +
                  stateJac[27] * covP[258]) + stateJac[28] * covP[277]) +
                stateJac[23] * covP[30]) + stateJac[24] * covP[49];

  // 'updateEskfCovP:347' covP(12, 9) = covP(12, 1)*stateJac(30) + covP(12, 13)*stateJac(34) + covP(12, 14)*stateJac(35) + covP(12, 15)*stateJac(36) + covP(12, 2)*stateJac(31) + covP(12, 3)*stateJac(32) + covP(12, 9); 
  covP[163] += ((((covP[11] * stateJac[29] + stateJac[33] * covP[239]) +
                  stateJac[34] * covP[258]) + stateJac[35] * covP[277]) + covP
                [30] * stateJac[30]) + stateJac[31] * covP[49];

  // 'updateEskfCovP:348' covP(12, 12) = covP(12, 12) + processNoiseQ(12, 12);
  covP[220] += processNoiseQ[220];

  // 'updateEskfCovP:349' covP(13, 1) = covP(13, 1)*stateJac(1) - covP(13, 10)*sampleTime_s + covP(13, 2)*stateJac(2) + covP(13, 3)*stateJac(3); 
  covP[12] = ((stateJac[0] * covP[12] - covP[183] * b_sampleTime_s) + stateJac[1]
              * covP[31]) + stateJac[2] * covP[50];

  // 'updateEskfCovP:350' covP(13, 2) = covP(13, 1)*stateJac(4) - covP(13, 11)*sampleTime_s + covP(13, 2)*stateJac(5) + covP(13, 3)*stateJac(6); 
  covP[31] = ((stateJac[3] * covP[12] - covP[202] * b_sampleTime_s) + stateJac[4]
              * covP[31]) + stateJac[5] * covP[50];

  // 'updateEskfCovP:351' covP(13, 3) = covP(13, 1)*stateJac(7) - covP(13, 12)*sampleTime_s + covP(13, 2)*stateJac(8) + covP(13, 3)*stateJac(9); 
  covP[50] = ((stateJac[6] * covP[12] - covP[221] * b_sampleTime_s) + stateJac[7]
              * covP[31]) + stateJac[8] * covP[50];

  // 'updateEskfCovP:352' covP(13, 4) = covP(13, 4) + covP(13, 7)*sampleTime_s;
  covP[69] += covP[126] * b_sampleTime_s;

  // 'updateEskfCovP:353' covP(13, 5) = covP(13, 5) + covP(13, 8)*sampleTime_s;
  covP[88] += covP[145] * b_sampleTime_s;

  // 'updateEskfCovP:354' covP(13, 6) = covP(13, 6) + covP(13, 9)*sampleTime_s;
  covP[107] += covP[164] * b_sampleTime_s;

  // 'updateEskfCovP:355' covP(13, 7) = covP(13, 1)*stateJac(16) + covP(13, 14)*stateJac(21) + covP(13, 15)*stateJac(22) + covP(13, 2)*stateJac(17) + covP(13, 3)*stateJac(18) + covP(13, 7) + tmp100; 
  covP[126] = (((((covP[12] * stateJac[15] + stateJac[20] * covP[259]) +
                  stateJac[21] * covP[278]) + stateJac[16] * covP[31]) +
                stateJac[17] * covP[50]) + covP[126]) + tmp100;

  // 'updateEskfCovP:356' covP(13, 8) = covP(13, 1)*stateJac(23) + covP(13, 14)*stateJac(28) + covP(13, 15)*stateJac(29) + covP(13, 2)*stateJac(24) + covP(13, 3)*stateJac(25) + covP(13, 8) + tmp115; 
  covP[145] = (((((covP[12] * stateJac[22] + stateJac[27] * covP[259]) +
                  stateJac[28] * covP[278]) + stateJac[23] * covP[31]) +
                stateJac[24] * covP[50]) + covP[145]) + tmp115;

  // 'updateEskfCovP:357' covP(13, 9) = covP(13, 1)*stateJac(30) + covP(13, 14)*stateJac(35) + covP(13, 15)*stateJac(36) + covP(13, 2)*stateJac(31) + covP(13, 3)*stateJac(32) + covP(13, 9) + tmp130; 
  covP[164] = (((((covP[12] * stateJac[29] + stateJac[34] * covP[259]) +
                  stateJac[35] * covP[278]) + stateJac[30] * covP[31]) +
                stateJac[31] * covP[50]) + covP[164]) + tmp130;

  // 'updateEskfCovP:358' covP(13, 13) = covP(13, 13) + processNoiseQ(13, 13);
  covP[240] += processNoiseQ[240];

  // 'updateEskfCovP:359' covP(14, 1) = covP(14, 1)*stateJac(1) - covP(14, 10)*sampleTime_s + covP(14, 2)*stateJac(2) + covP(14, 3)*stateJac(3); 
  covP[13] = ((stateJac[0] * covP[13] - covP[184] * b_sampleTime_s) + stateJac[1]
              * covP[32]) + stateJac[2] * covP[51];

  // 'updateEskfCovP:360' covP(14, 2) = covP(14, 1)*stateJac(4) - covP(14, 11)*sampleTime_s + covP(14, 2)*stateJac(5) + covP(14, 3)*stateJac(6); 
  covP[32] = ((stateJac[3] * covP[13] - covP[203] * b_sampleTime_s) + stateJac[4]
              * covP[32]) + stateJac[5] * covP[51];

  // 'updateEskfCovP:361' covP(14, 3) = covP(14, 1)*stateJac(7) - covP(14, 12)*sampleTime_s + covP(14, 2)*stateJac(8) + covP(14, 3)*stateJac(9); 
  covP[51] = ((stateJac[6] * covP[13] - covP[222] * b_sampleTime_s) + stateJac[7]
              * covP[32]) + stateJac[8] * covP[51];

  // 'updateEskfCovP:362' covP(14, 4) = covP(14, 4) + covP(14, 7)*sampleTime_s;
  covP[70] += covP[127] * b_sampleTime_s;

  // 'updateEskfCovP:363' covP(14, 5) = covP(14, 5) + covP(14, 8)*sampleTime_s;
  covP[89] += covP[146] * b_sampleTime_s;

  // 'updateEskfCovP:364' covP(14, 6) = covP(14, 6) + covP(14, 9)*sampleTime_s;
  covP[108] += covP[165] * b_sampleTime_s;

  // 'updateEskfCovP:365' covP(14, 7) = covP(14, 1)*stateJac(16) + covP(14, 13)*stateJac(20) + covP(14, 15)*stateJac(22) + covP(14, 2)*stateJac(17) + covP(14, 3)*stateJac(18) + covP(14, 7) + tmp102; 
  covP[127] = (((((covP[13] * stateJac[15] + stateJac[19] * covP[241]) +
                  stateJac[21] * covP[279]) + stateJac[16] * covP[32]) +
                stateJac[17] * covP[51]) + covP[127]) + tmp102;

  // 'updateEskfCovP:366' covP(14, 8) = covP(14, 1)*stateJac(23) + covP(14, 13)*stateJac(27) + covP(14, 15)*stateJac(29) + covP(14, 2)*stateJac(24) + covP(14, 3)*stateJac(25) + covP(14, 8) + tmp117; 
  covP[146] = (((((covP[13] * stateJac[22] + stateJac[26] * covP[241]) +
                  stateJac[28] * covP[279]) + stateJac[23] * covP[32]) +
                stateJac[24] * covP[51]) + covP[146]) + tmp117;

  // 'updateEskfCovP:367' covP(14, 9) = covP(14, 1)*stateJac(30) + covP(14, 13)*stateJac(34) + covP(14, 15)*stateJac(36) + covP(14, 2)*stateJac(31) + covP(14, 3)*stateJac(32) + covP(14, 9) + tmp132; 
  covP[165] = (((((covP[13] * stateJac[29] + stateJac[33] * covP[241]) +
                  stateJac[35] * covP[279]) + stateJac[30] * covP[32]) +
                stateJac[31] * covP[51]) + covP[165]) + tmp132;

  // 'updateEskfCovP:368' covP(14, 14) = covP(14, 14) + processNoiseQ(14, 14);
  covP[260] += processNoiseQ[260];

  // 'updateEskfCovP:369' covP(15, 1) = covP(15, 1)*stateJac(1) - covP(15, 10)*sampleTime_s + covP(15, 2)*stateJac(2) + covP(15, 3)*stateJac(3); 
  covP[14] = ((stateJac[0] * covP[14] - covP[185] * b_sampleTime_s) + stateJac[1]
              * covP[33]) + stateJac[2] * covP[52];

  // 'updateEskfCovP:370' covP(15, 2) = covP(15, 1)*stateJac(4) - covP(15, 11)*sampleTime_s + covP(15, 2)*stateJac(5) + covP(15, 3)*stateJac(6); 
  covP[33] = ((stateJac[3] * covP[14] - covP[204] * b_sampleTime_s) + stateJac[4]
              * covP[33]) + stateJac[5] * covP[52];

  // 'updateEskfCovP:371' covP(15, 3) = covP(15, 1)*stateJac(7) - covP(15, 12)*sampleTime_s + covP(15, 2)*stateJac(8) + covP(15, 3)*stateJac(9); 
  covP[52] = ((stateJac[6] * covP[14] - covP[223] * b_sampleTime_s) + stateJac[7]
              * covP[33]) + stateJac[8] * covP[52];

  // 'updateEskfCovP:372' covP(15, 4) = covP(15, 4) + covP(15, 7)*sampleTime_s;
  covP[71] += covP[128] * b_sampleTime_s;

  // 'updateEskfCovP:373' covP(15, 5) = covP(15, 5) + covP(15, 8)*sampleTime_s;
  covP[90] += covP[147] * b_sampleTime_s;

  // 'updateEskfCovP:374' covP(15, 6) = covP(15, 6) + covP(15, 9)*sampleTime_s;
  covP[109] += covP[166] * b_sampleTime_s;

  // 'updateEskfCovP:375' covP(15, 7) = covP(15, 1)*stateJac(16) + covP(15, 13)*stateJac(20) + covP(15, 14)*stateJac(21) + covP(15, 2)*stateJac(17) + covP(15, 3)*stateJac(18) + covP(15, 7) + tmp104; 
  covP[128] = (((((covP[14] * stateJac[15] + stateJac[19] * covP[242]) +
                  stateJac[20] * covP[261]) + stateJac[16] * covP[33]) +
                stateJac[17] * covP[52]) + covP[128]) + tmp104;

  // 'updateEskfCovP:376' covP(15, 8) = covP(15, 1)*stateJac(23) + covP(15, 13)*stateJac(27) + covP(15, 14)*stateJac(28) + covP(15, 2)*stateJac(24) + covP(15, 3)*stateJac(25) + covP(15, 8) + tmp119; 
  covP[147] = (((((covP[14] * stateJac[22] + stateJac[26] * covP[242]) +
                  stateJac[27] * covP[261]) + stateJac[23] * covP[33]) +
                stateJac[24] * covP[52]) + covP[147]) + tmp119;

  // 'updateEskfCovP:377' covP(15, 9) = covP(15, 1)*stateJac(30) + covP(15, 13)*stateJac(34) + covP(15, 14)*stateJac(35) + covP(15, 2)*stateJac(31) + covP(15, 3)*stateJac(32) + covP(15, 9) + tmp134; 
  covP[166] = (((((covP[14] * stateJac[29] + stateJac[33] * covP[242]) +
                  stateJac[34] * covP[261]) + stateJac[30] * covP[33]) +
                stateJac[31] * covP[52]) + covP[166]) + tmp134;

  // 'updateEskfCovP:378' covP(15, 15) = covP(15, 15) + processNoiseQ(15, 15);
  covP[280] += processNoiseQ[280];

  // 'updateEskfCovP:379' covP(16, 1) = covP(16, 1)*stateJac(1) - covP(16, 10)*sampleTime_s + covP(16, 2)*stateJac(2) + covP(16, 3)*stateJac(3); 
  covP[15] = ((stateJac[0] * covP[15] - covP[186] * b_sampleTime_s) + stateJac[1]
              * covP[34]) + stateJac[2] * covP[53];

  // 'updateEskfCovP:380' covP(16, 2) = covP(16, 1)*stateJac(4) - covP(16, 11)*sampleTime_s + covP(16, 2)*stateJac(5) + covP(16, 3)*stateJac(6); 
  covP[34] = ((stateJac[3] * covP[15] - covP[205] * b_sampleTime_s) + stateJac[4]
              * covP[34]) + stateJac[5] * covP[53];

  // 'updateEskfCovP:381' covP(16, 3) = covP(16, 1)*stateJac(7) - covP(16, 12)*sampleTime_s + covP(16, 2)*stateJac(8) + covP(16, 3)*stateJac(9); 
  covP[53] = ((stateJac[6] * covP[15] - covP[224] * b_sampleTime_s) + stateJac[7]
              * covP[34]) + stateJac[8] * covP[53];

  // 'updateEskfCovP:382' covP(16, 4) = covP(16, 4) + covP(16, 7)*sampleTime_s;
  covP[72] += covP[129] * b_sampleTime_s;

  // 'updateEskfCovP:383' covP(16, 5) = covP(16, 5) + covP(16, 8)*sampleTime_s;
  covP[91] += covP[148] * b_sampleTime_s;

  // 'updateEskfCovP:384' covP(16, 6) = covP(16, 6) + covP(16, 9)*sampleTime_s;
  covP[110] += covP[167] * b_sampleTime_s;

  // 'updateEskfCovP:385' covP(16, 7) = covP(16, 1)*stateJac(16) + covP(16, 13)*stateJac(20) + covP(16, 14)*stateJac(21) + covP(16, 15)*stateJac(22) + covP(16, 2)*stateJac(17) + covP(16, 3)*stateJac(18) + covP(16, 7); 
  covP[129] += ((((covP[15] * stateJac[15] + stateJac[19] * covP[243]) +
                  stateJac[20] * covP[262]) + stateJac[21] * covP[281]) +
                stateJac[16] * covP[34]) + stateJac[17] * covP[53];

  // 'updateEskfCovP:386' covP(16, 8) = covP(16, 1)*stateJac(23) + covP(16, 13)*stateJac(27) + covP(16, 14)*stateJac(28) + covP(16, 15)*stateJac(29) + covP(16, 2)*stateJac(24) + covP(16, 3)*stateJac(25) + covP(16, 8); 
  covP[148] += ((((covP[15] * stateJac[22] + stateJac[26] * covP[243]) +
                  stateJac[27] * covP[262]) + stateJac[28] * covP[281]) +
                stateJac[23] * covP[34]) + stateJac[24] * covP[53];

  // 'updateEskfCovP:387' covP(16, 9) = covP(16, 1)*stateJac(30) + covP(16, 13)*stateJac(34) + covP(16, 14)*stateJac(35) + covP(16, 15)*stateJac(36) + covP(16, 2)*stateJac(31) + covP(16, 3)*stateJac(32) + covP(16, 9); 
  covP[167] += ((((covP[15] * stateJac[29] + stateJac[33] * covP[243]) +
                  stateJac[34] * covP[262]) + stateJac[35] * covP[281]) +
                stateJac[30] * covP[34]) + stateJac[31] * covP[53];

  // 'updateEskfCovP:388' covP(16, 16) = covP(16, 16) + processNoiseQ(16, 16);
  covP[300] += processNoiseQ[300];

  // 'updateEskfCovP:389' covP(17, 1) = covP(17, 1)*stateJac(1) - covP(17, 10)*sampleTime_s + covP(17, 2)*stateJac(2) + covP(17, 3)*stateJac(3); 
  covP[16] = ((stateJac[0] * covP[16] - covP[187] * b_sampleTime_s) + stateJac[1]
              * covP[35]) + stateJac[2] * covP[54];

  // 'updateEskfCovP:390' covP(17, 2) = covP(17, 1)*stateJac(4) - covP(17, 11)*sampleTime_s + covP(17, 2)*stateJac(5) + covP(17, 3)*stateJac(6); 
  covP[35] = ((stateJac[3] * covP[16] - covP[206] * b_sampleTime_s) + stateJac[4]
              * covP[35]) + stateJac[5] * covP[54];

  // 'updateEskfCovP:391' covP(17, 3) = covP(17, 1)*stateJac(7) - covP(17, 12)*sampleTime_s + covP(17, 2)*stateJac(8) + covP(17, 3)*stateJac(9); 
  covP[54] = ((stateJac[6] * covP[16] - covP[225] * b_sampleTime_s) + stateJac[7]
              * covP[35]) + stateJac[8] * covP[54];

  // 'updateEskfCovP:392' covP(17, 4) = covP(17, 4) + covP(17, 7)*sampleTime_s;
  covP[73] += covP[130] * b_sampleTime_s;

  // 'updateEskfCovP:393' covP(17, 5) = covP(17, 5) + covP(17, 8)*sampleTime_s;
  covP[92] += covP[149] * b_sampleTime_s;

  // 'updateEskfCovP:394' covP(17, 6) = covP(17, 6) + covP(17, 9)*sampleTime_s;
  covP[111] += covP[168] * b_sampleTime_s;

  // 'updateEskfCovP:395' covP(17, 7) = covP(17, 1)*stateJac(16) + covP(17, 13)*stateJac(20) + covP(17, 14)*stateJac(21) + covP(17, 15)*stateJac(22) + covP(17, 2)*stateJac(17) + covP(17, 3)*stateJac(18) + covP(17, 7); 
  covP[130] += ((((stateJac[15] * covP[16] + stateJac[19] * covP[244]) +
                  stateJac[20] * covP[263]) + stateJac[21] * covP[282]) +
                stateJac[16] * covP[35]) + stateJac[17] * covP[54];

  // 'updateEskfCovP:396' covP(17, 8) = covP(17, 1)*stateJac(23) + covP(17, 13)*stateJac(27) + covP(17, 14)*stateJac(28) + covP(17, 15)*stateJac(29) + covP(17, 2)*stateJac(24) + covP(17, 3)*stateJac(25) + covP(17, 8); 
  covP[149] += ((((covP[16] * stateJac[22] + stateJac[26] * covP[244]) +
                  stateJac[27] * covP[263]) + stateJac[28] * covP[282]) +
                stateJac[23] * covP[35]) + stateJac[24] * covP[54];

  // 'updateEskfCovP:397' covP(17, 9) = covP(17, 1)*stateJac(30) + covP(17, 13)*stateJac(34) + covP(17, 14)*stateJac(35) + covP(17, 15)*stateJac(36) + covP(17, 2)*stateJac(31) + covP(17, 3)*stateJac(32) + covP(17, 9); 
  covP[168] += ((((covP[16] * stateJac[29] + stateJac[33] * covP[244]) +
                  stateJac[34] * covP[263]) + stateJac[35] * covP[282]) +
                stateJac[30] * covP[35]) + stateJac[31] * covP[54];

  // 'updateEskfCovP:398' covP(17, 17) = covP(17, 17) + processNoiseQ(17, 17);
  covP[320] += processNoiseQ[320];

  // 'updateEskfCovP:399' covP(18, 1) = covP(18, 1)*stateJac(1) - covP(18, 10)*sampleTime_s + covP(18, 2)*stateJac(2) + covP(18, 3)*stateJac(3); 
  covP[17] = ((stateJac[0] * covP[17] - covP[188] * b_sampleTime_s) + stateJac[1]
              * covP[36]) + stateJac[2] * covP[55];

  // 'updateEskfCovP:400' covP(18, 2) = covP(18, 1)*stateJac(4) - covP(18, 11)*sampleTime_s + covP(18, 2)*stateJac(5) + covP(18, 3)*stateJac(6); 
  covP[36] = ((stateJac[3] * covP[17] - covP[207] * b_sampleTime_s) + stateJac[4]
              * covP[36]) + stateJac[5] * covP[55];

  // 'updateEskfCovP:401' covP(18, 3) = covP(18, 1)*stateJac(7) - covP(18, 12)*sampleTime_s + covP(18, 2)*stateJac(8) + covP(18, 3)*stateJac(9); 
  covP[55] = ((stateJac[6] * covP[17] - covP[226] * b_sampleTime_s) + stateJac[7]
              * covP[36]) + stateJac[8] * covP[55];

  // 'updateEskfCovP:402' covP(18, 4) = covP(18, 4) + covP(18, 7)*sampleTime_s;
  covP[74] += covP[131] * b_sampleTime_s;

  // 'updateEskfCovP:403' covP(18, 5) = covP(18, 5) + covP(18, 8)*sampleTime_s;
  covP[93] += covP[150] * b_sampleTime_s;

  // 'updateEskfCovP:404' covP(18, 6) = covP(18, 6) + covP(18, 9)*sampleTime_s;
  covP[112] += covP[169] * b_sampleTime_s;

  // 'updateEskfCovP:405' covP(18, 7) = covP(18, 1)*stateJac(16) + covP(18, 13)*stateJac(20) + covP(18, 14)*stateJac(21) + covP(18, 15)*stateJac(22) + covP(18, 2)*stateJac(17) + covP(18, 3)*stateJac(18) + covP(18, 7); 
  covP[131] += ((((stateJac[15] * covP[17] + stateJac[19] * covP[245]) +
                  stateJac[20] * covP[264]) + stateJac[21] * covP[283]) +
                stateJac[16] * covP[36]) + stateJac[17] * covP[55];

  // 'updateEskfCovP:406' covP(18, 8) = covP(18, 1)*stateJac(23) + covP(18, 13)*stateJac(27) + covP(18, 14)*stateJac(28) + covP(18, 15)*stateJac(29) + covP(18, 2)*stateJac(24) + covP(18, 3)*stateJac(25) + covP(18, 8); 
  covP[150] += ((((covP[17] * stateJac[22] + stateJac[26] * covP[245]) +
                  stateJac[27] * covP[264]) + stateJac[28] * covP[283]) +
                stateJac[23] * covP[36]) + stateJac[24] * covP[55];

  // 'updateEskfCovP:407' covP(18, 9) = covP(18, 1)*stateJac(30) + covP(18, 13)*stateJac(34) + covP(18, 14)*stateJac(35) + covP(18, 15)*stateJac(36) + covP(18, 2)*stateJac(31) + covP(18, 3)*stateJac(32) + covP(18, 9); 
  covP[169] += ((((covP[17] * stateJac[29] + stateJac[33] * covP[245]) +
                  stateJac[34] * covP[264]) + stateJac[35] * covP[283]) +
                stateJac[30] * covP[36]) + stateJac[31] * covP[55];

  // 'updateEskfCovP:408' covP(18, 18) = covP(18, 18) + processNoiseQ(18, 18);
  covP[340] += processNoiseQ[340];

  // 'updateEskfCovP:409' covP(19, 1) = covP(19, 1)*stateJac(1) - covP(19, 10)*sampleTime_s + covP(19, 2)*stateJac(2) + covP(19, 3)*stateJac(3); 
  covP[18] = ((stateJac[0] * covP[18] - covP[189] * b_sampleTime_s) + stateJac[1]
              * covP[37]) + stateJac[2] * covP[56];

  // 'updateEskfCovP:410' covP(19, 2) = covP(19, 1)*stateJac(4) - covP(19, 11)*sampleTime_s + covP(19, 2)*stateJac(5) + covP(19, 3)*stateJac(6); 
  covP[37] = ((stateJac[3] * covP[18] - covP[208] * b_sampleTime_s) + stateJac[4]
              * covP[37]) + stateJac[5] * covP[56];

  // 'updateEskfCovP:411' covP(19, 3) = covP(19, 1)*stateJac(7) - covP(19, 12)*sampleTime_s + covP(19, 2)*stateJac(8) + covP(19, 3)*stateJac(9); 
  covP[56] = ((stateJac[6] * covP[18] - covP[227] * b_sampleTime_s) + stateJac[7]
              * covP[37]) + stateJac[8] * covP[56];

  // 'updateEskfCovP:412' covP(19, 4) = covP(19, 4) + covP(19, 7)*sampleTime_s;
  covP[75] += covP[132] * b_sampleTime_s;

  // 'updateEskfCovP:413' covP(19, 5) = covP(19, 5) + covP(19, 8)*sampleTime_s;
  covP[94] += covP[151] * b_sampleTime_s;

  // 'updateEskfCovP:414' covP(19, 6) = covP(19, 6) + covP(19, 9)*sampleTime_s;
  covP[113] += covP[170] * b_sampleTime_s;

  // 'updateEskfCovP:415' covP(19, 7) = covP(19, 1)*stateJac(16) + covP(19, 13)*stateJac(20) + covP(19, 14)*stateJac(21) + covP(19, 15)*stateJac(22) + covP(19, 2)*stateJac(17) + covP(19, 3)*stateJac(18) + covP(19, 7); 
  covP[132] += ((((stateJac[15] * covP[18] + stateJac[19] * covP[246]) +
                  stateJac[20] * covP[265]) + stateJac[21] * covP[284]) +
                stateJac[16] * covP[37]) + stateJac[17] * covP[56];

  // 'updateEskfCovP:416' covP(19, 8) = covP(19, 1)*stateJac(23) + covP(19, 13)*stateJac(27) + covP(19, 14)*stateJac(28) + covP(19, 15)*stateJac(29) + covP(19, 2)*stateJac(24) + covP(19, 3)*stateJac(25) + covP(19, 8); 
  covP[151] += ((((covP[18] * stateJac[22] + stateJac[26] * covP[246]) +
                  stateJac[27] * covP[265]) + stateJac[28] * covP[284]) +
                stateJac[23] * covP[37]) + stateJac[24] * covP[56];

  // 'updateEskfCovP:417' covP(19, 9) = covP(19, 1)*stateJac(30) + covP(19, 13)*stateJac(34) + covP(19, 14)*stateJac(35) + covP(19, 15)*stateJac(36) + covP(19, 2)*stateJac(31) + covP(19, 3)*stateJac(32) + covP(19, 9); 
  covP[170] += ((((covP[18] * stateJac[29] + stateJac[33] * covP[246]) +
                  stateJac[34] * covP[265]) + stateJac[35] * covP[284]) +
                stateJac[30] * covP[37]) + stateJac[31] * covP[56];

  // 'updateEskfCovP:418' covP(19, 19) = covP(19, 19) + processNoiseQ(19, 19);
  covP[360] += processNoiseQ[360];
}

//
// File trailer for generated code.
//
// [EOF]
//
