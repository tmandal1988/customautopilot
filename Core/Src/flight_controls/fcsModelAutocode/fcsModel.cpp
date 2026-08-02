//
// File: fcsModel.cpp
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 7.9
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Fri Jul 31 11:50:47 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#include "fcsModel.h"
#include "rtwtypes.h"
#include "fcsModel_types.h"
#include <cmath>
#include <array>
#include <cstring>
#include "fcsModel_private.h"

// Named constants for Chart: '<S4>/Chart'
const uint8_T fcsModel_IN_ARM_MTRS{ 1U };

const uint8_T fcsModel_IN_INACTIVE{ 2U };

const uint8_T fcsModel_IN_INFLIGHT{ 3U };

const busXyBodyAccelCtrIDebug fcsModel_rtZbusXyBodyAccelCtrIDebug{
  busXyBodyAccelCtrIDebug{
    { {
        0.0, 0.0 } }
    ,                                  // cmd

    { {
        0.0, 0.0 } }
    ,                                  // meas

    { {
        busPidDebug{
          0.0,                         // output
          0.0,                         // proportionalOutput
          0.0,                         // integralOutput
          0.0                          // derivativeOutput
        }, busPidDebug{
          0.0,                         // output
          0.0,                         // proportionalOutput
          0.0,                         // integralOutput
          0.0                          // derivativeOutput
        } } }
    // pidDebug
  } };                                 // busXyBodyAccelCtrIDebug ground

static void rate_scheduler(fcsModel::RT_MODEL_fcsModel_T *const fcsModel_M);
const busFcsDebug fcsModel_rtZbusFcsDebug{ { 0.0,// thrustCmd_N
    0.0,                               // xMomCmd_Nm
    0.0,                               // yMomCmd_Nm
    0.0                                // zMomCmd_Nm
  },                                   // allocDebug

  { { { { 0.0, 0.0, 0.0 } },           // cmd

      { { 0.0, 0.0, 0.0 } },           // meas

      { { { 0.0,                       // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          } } }                        // pidDebug
    },                                 // angRateCtrlDebug

    { { { 0.0, 0.0, 0.0 } },           // cmd

      { { 0.0, 0.0, 0.0 } },           // meas

      { { { 0.0,                       // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          } } }                        // pidDebug
    }                                  // attCtrlDebug
  },                                   // innerLoopCtrlDebug

  { 0.0,                               // frcCmd_N
    { { { 0.0, 0.0, 0.0 } },           // cmd

      { { 0.0, 0.0, 0.0 } },           // meas

      { { 0.0, 0.0, 0.0 } },           // velCtrlFf

      { { { 0.0,                       // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          } } }                        // pidDebug
    },                                 // velCtrlDebug

    { { { 0.0, 0.0, 0.0 } },           // cmd

      { { 0.0, 0.0, 0.0 } },           // meas

      { { { 0.0,                       // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          } } }                        // pidDebug
    },                                 // posCtrlDebug

    { 0.0,                             // cmd
      0.0,                             // meas

      { 0.0,                           // output
        0.0,                           // proportionalOutput
        0.0,                           // integralOutput
        0.0                            // derivativeOutput
      }                                // pidDebug
    },                                 // zAccelCtrlDebug

    { { { 0.0, 0.0 } },                // cmd

      { { 0.0, 0.0 } },                // meas

      { { { 0.0,                       // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          }, { 0.0,                    // output
            0.0,                       // proportionalOutput
            0.0,                       // integralOutput
            0.0                        // derivativeOutput
          } } }                        // pidDebug
    }                                  // xyBodyAccelCtrlDebug
  },                                   // outerLoopCtrlDebug

  { enumChirpTrigger::OFF,             // chirpTrigger
    enumChirpType::NONE,               // chirpType
    0.0                                // chirpSignal
  },                                   // sysIdDebug
  enumStateMachine::INACTIVE,          // state
  enumFlightMode::STABILIZE            // flightMode
};

uint32_T plook_bincpag(real_T u, const real_T bp[], uint32_T maxIndex, real_T
  *fraction, uint32_T *prevIndex)
{
  uint32_T bpIndex;

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Use previous index: 'on'
  // Use last breakpoint for index at or above upper limit: 'on'
  // Remove protection against out-of-range input in generated code: 'on'

  if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d_prevIdx(u, bp, *prevIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex;
    *fraction = 0.0;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

real_T intrp1d_la(uint32_T bpIndex, real_T frac, const real_T table[], uint32_T
                  maxIndex)
{
  real_T y;

  // Column-major Interpolation 1-D
  // Interpolation method: 'Linear point-slope'
  // Use last breakpoint for index at or above upper limit: 'on'
  // Overflow mode: 'wrapping'

  if (bpIndex == maxIndex) {
    y = table[bpIndex];
  } else {
    real_T yL_0d0;
    yL_0d0 = table[bpIndex];
    y = (table[bpIndex + 1U] - yL_0d0) * frac + yL_0d0;
  }

  return y;
}

uint32_T binsearch_u32d_prevIdx(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIndex;
  uint32_T found;
  uint32_T iLeft;
  uint32_T iRght;

  // Binary Search using Previous Index
  bpIndex = startIndex;
  iLeft = 0U;
  iRght = maxIndex;
  found = 0U;
  while (found == 0U) {
    if (u < bp[bpIndex]) {
      iRght = bpIndex - 1U;
      bpIndex = ((bpIndex + iLeft) - 1U) >> 1U;
    } else if (u < bp[bpIndex + 1U]) {
      found = 1U;
    } else {
      iLeft = bpIndex + 1U;
      bpIndex = ((bpIndex + iRght) + 1U) >> 1U;
    }
  }

  return bpIndex;
}

//
//         This function updates active task flag for each subrate.
//         The function is called at model base rate, hence the
//         generated code self-manages all its subrates.
//
static void rate_scheduler(fcsModel::RT_MODEL_fcsModel_T *const fcsModel_M)
{
  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (fcsModel_M->Timing.TaskCounters.TID[1])++;
  if ((fcsModel_M->Timing.TaskCounters.TID[1]) > 1) {// Sample time: [0.008s, 0.0s] 
    fcsModel_M->Timing.TaskCounters.TID[1] = 0;
  }
}

//
// Output and update for atomic system:
//    '<S24>/Discrete First Order Deriv Filter'
//    '<S68>/Discrete First Order Deriv Filter'
//    '<S132>/Discrete First Order Deriv Filter'
//    '<S192>/Discrete First Order Deriv Filter'
//
void fcsModel::f_DiscreteFirstOrderDerivFilter(real_T rtu_input, real_T
  rtu_filterBandwidth_radps, real_T *rty_filteredInputRate, real_T
  rtp_sampleTime_s, DW_DiscreteFirstOrderDerivFil_T *localDW)
{
  real_T K;
  real_T normalizer;
  real_T num_tmp;

  // MATLAB Function: '<S55>/Compute Deriv Filter Numerator And Denominator'
  //  Call the main function
  // MATLAB Function 'Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator': '<S58>:1' 
  // '<S58>:1:4' [num, den] = computeFirstOrderDerivFilterNumAndDen_function(filterBandwidth_radps, sampleTime_s); 
  //  This function computes the numerator and denominator of the discrete
  //  first order derivative filter
  //
  // Inputs:
  // filterBandwidth_radps: Bandwidth of the filter
  // sampleTime_s: sampling time
  //
  // Outputs:
  // num: Numerator array for the discrete transfer function
  // den: Denominator array for the discrete transfer function
  // 'computeFirstOrderDerivFilterNumAndDen_function:13' B1 = filterBandwidth_radps; 
  // 'computeFirstOrderDerivFilterNumAndDen_function:14' B0 = 0;
  // 'computeFirstOrderDerivFilterNumAndDen_function:16' A0 = B1;
  // 'computeFirstOrderDerivFilterNumAndDen_function:17' A1 = 1;
  // 'computeFirstOrderDerivFilterNumAndDen_function:18' K = 2/sampleTime_s;
  K = 2.0 / rtp_sampleTime_s;

  // 'computeFirstOrderDerivFilterNumAndDen_function:20' [num, den] = computeDiscreteTFNumAndDen_function([B0, B1], [A0, A1], K); 
  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  //  For 1st order system
  // 'computeDiscreteTFNumAndDen_function:23' normalizer = A(1) + A(2)*K;
  normalizer = rtu_filterBandwidth_radps + K;

  // 'computeDiscreteTFNumAndDen_function:24' b0 = (B(1) + B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:25' b1 = (B(1) - B(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:27' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:28' a1 = (A(1) - A(2)*K)/normalizer;
  // 'computeDiscreteTFNumAndDen_function:29' num = [b0, b1];
  num_tmp = rtu_filterBandwidth_radps * K;
  localDW->num[0] = num_tmp / normalizer;
  localDW->num[1] = (0.0 - num_tmp) / normalizer;

  // 'computeDiscreteTFNumAndDen_function:30' den = [a0, a1];
  localDW->den[0] = 1.0;
  localDW->den[1] = (rtu_filterBandwidth_radps - K) / normalizer;

  // DiscreteTransferFcn: '<S55>/Discrete Transfer Fcn'
  K = rtu_input - localDW->den[1] * localDW->DiscreteTransferFcn_states;
  *rty_filteredInputRate = localDW->num[0] * K + localDW->num[1] *
    localDW->DiscreteTransferFcn_states;

  // Update for DiscreteTransferFcn: '<S55>/Discrete Transfer Fcn'
  localDW->DiscreteTransferFcn_states = K;
}

//
// System initialize for atomic system:
//    '<S21>/pidWithDebug'
//    '<S62>/pidWithDebug'
//
void fcsModel::fcsModel_pidWithDebug_Init(DW_pidWithDebug_fcsModel_T *localDW)
{
  // InitializeConditions for DiscreteIntegrator: '<S24>/Discrete-Time Integrator' 
  localDW->DiscreteTimeIntegrator_IC_LOADI = 1U;

  // InitializeConditions for Delay: '<S56>/Delay'
  localDW->icLoad = true;
}

//
// Output and update for atomic system:
//    '<S21>/pidWithDebug'
//    '<S62>/pidWithDebug'
//
void fcsModel::fcsModel_pidWithDebug(real_T rtu_feedForward, real_T rtu_cmd,
  real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc, const
  busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T *rty_ctrlCmd,
  busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s, DW_pidWithDebug_fcsModel_T
  *localDW)
{
  real_T rtb_Product5_o;
  real_T rtb_Sum1_b;
  real_T rtb_Sum_k;
  real_T rtb_Switch2_oz;
  real_T rtb_Switch2_p;
  real_T rtb_UkYk1_k;
  real_T rtb_UnitDelay_i;

  // Product: '<S56>/delta rise limit' incorporates:
  //   SampleTimeMath: '<S56>/sample time'
  //
  //  About '<S56>/sample time':
  //   y = K where K = ( w * Ts )
  //
  rtb_Switch2_p = rtu_pidParamBus->outputRateLimits[1] * 0.004;

  // Sum: '<S24>/Sum'
  rtb_Sum_k = rtu_cmd - rtu_meas;

  // Outputs for Atomic SubSystem: '<S24>/Discrete First Order Deriv Filter'
  f_DiscreteFirstOrderDerivFilter(rtb_Sum_k,
    rtu_pidParamBus->filterBandwidth_radps, &rtb_Product5_o, rtp_sampleTime_s,
    &localDW->DiscreteFirstOrderDerivFilter);

  // End of Outputs for SubSystem: '<S24>/Discrete First Order Deriv Filter'

  // Product: '<S24>/Product'
  rtb_Product5_o *= rtu_pidParamBus->Kd;

  // Product: '<S24>/Product1'
  rtb_UnitDelay_i = rtb_Sum_k * rtu_pidParamBus->Kp;

  // DiscreteIntegrator: '<S24>/Discrete-Time Integrator'
  if (localDW->DiscreteTimeIntegrator_IC_LOADI != 0) {
    localDW->DiscreteTimeIntegrator_DSTATE = rtu_integratorIc;
  }

  if (rtu_integratorReset || (localDW->DiscreteTimeIntegrator_PrevRese != 0)) {
    localDW->DiscreteTimeIntegrator_DSTATE = rtu_integratorIc;
  }

  // Sum: '<S24>/Sum1' incorporates:
  //   DiscreteIntegrator: '<S24>/Discrete-Time Integrator'

  rtb_Sum1_b = ((rtu_feedForward + rtb_Product5_o) + rtb_UnitDelay_i) +
    localDW->DiscreteTimeIntegrator_DSTATE;

  // Switch: '<S57>/Switch2' incorporates:
  //   RelationalOperator: '<S57>/LowerRelop1'
  //   RelationalOperator: '<S57>/UpperRelop'
  //   Switch: '<S57>/Switch'

  if (rtb_Sum1_b > rtu_pidParamBus->outputLimits[1]) {
    rtb_Switch2_oz = rtu_pidParamBus->outputLimits[1];
  } else if (rtb_Sum1_b < rtu_pidParamBus->outputLimits[0]) {
    // Switch: '<S57>/Switch'
    rtb_Switch2_oz = rtu_pidParamBus->outputLimits[0];
  } else {
    rtb_Switch2_oz = rtb_Sum1_b;
  }

  // End of Switch: '<S57>/Switch2'

  // Delay: '<S56>/Delay'
  if (localDW->icLoad) {
    localDW->Delay_DSTATE = rtb_Switch2_oz;
  }

  // Sum: '<S56>/Difference Inputs1' incorporates:
  //   Delay: '<S56>/Delay'
  //
  //  Block description for '<S56>/Difference Inputs1':
  //
  //   Add in CPU

  rtb_UkYk1_k = rtb_Switch2_oz - localDW->Delay_DSTATE;

  // Switch: '<S59>/Switch2' incorporates:
  //   RelationalOperator: '<S59>/LowerRelop1'

  if (rtb_UkYk1_k <= rtb_Switch2_p) {
    // Product: '<S56>/delta fall limit' incorporates:
    //   SampleTimeMath: '<S56>/sample time'
    //
    //  About '<S56>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rtb_Switch2_p = 0.004 * rtu_pidParamBus->outputRateLimits[0];

    // Switch: '<S59>/Switch' incorporates:
    //   RelationalOperator: '<S59>/UpperRelop'

    if (rtb_UkYk1_k >= rtb_Switch2_p) {
      rtb_Switch2_p = rtb_UkYk1_k;
    }

    // End of Switch: '<S59>/Switch'
  }

  // End of Switch: '<S59>/Switch2'

  // Sum: '<S56>/Difference Inputs2' incorporates:
  //   Delay: '<S56>/Delay'
  //
  //  Block description for '<S56>/Difference Inputs2':
  //
  //   Add in CPU

  *rty_ctrlCmd = rtb_Switch2_p + localDW->Delay_DSTATE;

  // BusCreator: '<S24>/Bus Creator' incorporates:
  //   DiscreteIntegrator: '<S24>/Discrete-Time Integrator'

  rty_pidDebug->output = *rty_ctrlCmd;
  rty_pidDebug->proportionalOutput = rtb_UnitDelay_i;
  rty_pidDebug->integralOutput = localDW->DiscreteTimeIntegrator_DSTATE;
  rty_pidDebug->derivativeOutput = rtb_Product5_o;

  // Update for DiscreteIntegrator: '<S24>/Discrete-Time Integrator' incorporates:
  //   Product: '<S24>/Product2'
  //   Product: '<S24>/Product3'
  //   Product: '<S24>/Product5'
  //   Sum: '<S24>/Sum2'
  //   Sum: '<S24>/Sum3'
  //   Sum: '<S24>/Sum4'
  //   Sum: '<S24>/Sum5'
  //   UnitDelay: '<S24>/Unit Delay'
  //   UnitDelay: '<S24>/Unit Delay1'

  localDW->DiscreteTimeIntegrator_IC_LOADI = 0U;
  localDW->DiscreteTimeIntegrator_DSTATE += (((rtu_trackingCtrlCmd -
    localDW->UnitDelay_DSTATE) * rtu_pidParamBus->Kt +
    (localDW->UnitDelay_DSTATE - localDW->UnitDelay1_DSTATE) *
    rtu_pidParamBus->Kb) + rtb_Sum_k * rtu_pidParamBus->Ki) * 0.004;
  localDW->DiscreteTimeIntegrator_PrevRese = static_cast<int8_T>
    (rtu_integratorReset);

  // Update for Delay: '<S56>/Delay'
  localDW->icLoad = false;
  localDW->Delay_DSTATE = *rty_ctrlCmd;

  // Update for UnitDelay: '<S24>/Unit Delay'
  localDW->UnitDelay_DSTATE = rtb_Switch2_oz;

  // Update for UnitDelay: '<S24>/Unit Delay1'
  localDW->UnitDelay1_DSTATE = rtb_Sum1_b;
}

//
// Output and update for atomic system:
//    '<S40>/Compute Natural Frequency'
//    '<S41>/Compute Natural Frequency'
//    '<S25>/Compute Natural Frequency'
//    '<S26>/Compute Natural Frequency'
//    '<S84>/Compute Natural Frequency'
//    '<S85>/Compute Natural Frequency'
//    '<S69>/Compute Natural Frequency'
//    '<S70>/Compute Natural Frequency'
//    '<S148>/Compute Natural Frequency'
//    '<S149>/Compute Natural Frequency'
//    ...
//
void fcsModel::fcsMode_ComputeNaturalFrequency(real_T rtu_bandwidth_radps,
  real_T rtu_dampingRatio_nd, real_T *rty_naturalFrequency_radps)
{
  real_T tmp;

  //  call the main function
  // MATLAB Function 'Discrete Second Order Filter/Compute Natural Frequency': '<S48>:1' 
  // '<S48>:1:4' naturalFrequency_radps = computeSecondOrderSystemNaturalFrequency_function(bandwidth_radps, dampingRatio_nd); 
  // COMPUTESECONDORDERSYSTEMNATURALFREQUENCY_FUNCTION computes the natural
  // frequency of a second order system when user provides damping ratio and
  // required bandwith.
  //
  // Input:
  // bandwidth_radps: Desired Bandwidth in rad/s
  // dampingRatio_nd: Damping ration of the system
  // 'computeSecondOrderSystemNaturalFrequency_function:9' naturalFrequency_radps = bandwidth_radps/(sqrt(1 - 2*dampingRatio_nd^2 + sqrt(2 - 4*dampingRatio_nd^2 + 4*dampingRatio_nd^4))); 
  tmp = rtu_dampingRatio_nd * rtu_dampingRatio_nd;
  *rty_naturalFrequency_radps = rtu_bandwidth_radps / std::sqrt(std::sqrt((2.0 -
    tmp * 4.0) + 4.0 * std::pow(rtu_dampingRatio_nd, 4.0)) + (1.0 - tmp * 2.0));
}

//
// Output and update for atomic system:
//    '<S40>/Compute Numerator And Denominator'
//    '<S25>/Compute Numerator And Denominator'
//    '<S84>/Compute Numerator And Denominator'
//    '<S69>/Compute Numerator And Denominator'
//    '<S148>/Compute Numerator And Denominator'
//    '<S133>/Compute Numerator And Denominator'
//    '<S223>/Compute Numerator And Denominator'
//    '<S208>/Compute Numerator And Denominator'
//    '<S193>/Compute Numerator And Denominator'
//
void fcsModel::ComputeNumeratorAndDenominator(real_T rtu_naturalFrequency_radps,
  real_T rtu_dampingRatio_nd, real_T rty_rateNum[3], real_T rty_accelNum[3],
  real_T rty_den[3], real_T rtp_sampleTime_s)
{
  real_T B_idx_1;
  real_T K;
  real_T normalizer;
  real_T normalizer_tmp;
  real_T normalizer_tmp_0;

  //  call the main function
  // MATLAB Function 'Discrete Second Order Deriv Filter/Compute Numerator And Denominator': '<S49>:1' 
  // '<S49>:1:4' [rateNum, accelNum, den] = computeSecondOrderDerivFilterNumAndDen_function(naturalFrequency_radps, dampingRatio_nd, sampleTime_s); 
  // COMPUTESECONDORDERDERIVFILTERNUMANDDEN_FUNCTION % This function computes the numerator and denominator of the dicrete 
  //  second order derivative and double derivative filters
  //
  // Inputs:
  // naturalFrequency_radps: Natural frequency of the filter
  // dampingRation_nd: Damping Ration of the filter
  // sampleTime_s: sampling time
  //
  // Outputs:
  // rateNum: Numerator array for the discrete derivative transfer function
  // accelNum: Numerator array for the discrete double derivative transfer function 
  // den: Denominator array for the discrete derivative transfer functions
  // 'computeSecondOrderDerivFilterNumAndDen_function:15' K = 2/sampleTime_s;
  K = 2.0 / rtp_sampleTime_s;

  // 'computeSecondOrderDerivFilterNumAndDen_function:16' A0 = naturalFrequency_radps^2; 
  // 'computeSecondOrderDerivFilterNumAndDen_function:17' A1 = 2*dampingRatio_nd*naturalFrequency_radps; 
  // 'computeSecondOrderDerivFilterNumAndDen_function:18' A2 = 1;
  // 'computeSecondOrderDerivFilterNumAndDen_function:20' B0 = 0;
  // 'computeSecondOrderDerivFilterNumAndDen_function:22' B1 = naturalFrequency_radps^2; 
  // 'computeSecondOrderDerivFilterNumAndDen_function:23' B2 = 0;
  //  compute the rate transfer function numerator and the denominator
  // 'computeSecondOrderDerivFilterNumAndDen_function:26' [rateNum, den] = computeDiscreteTFNumAndDen_function([B0, B1, B2], [A0, A1, A2], K); 
  B_idx_1 = rtu_naturalFrequency_radps * rtu_naturalFrequency_radps;

  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  // 'computeDiscreteTFNumAndDen_function:32' elseif (nArray == 3)
  //  For 2nd order system
  // 'computeDiscreteTFNumAndDen_function:34' normalizer = A(1) + A(2)*K + A(3)*K^2; 
  normalizer_tmp = K * K;
  normalizer_tmp_0 = 2.0 * rtu_dampingRatio_nd * rtu_naturalFrequency_radps * K;
  normalizer = (normalizer_tmp_0 + B_idx_1) + normalizer_tmp;

  // 'computeDiscreteTFNumAndDen_function:35' b0 = (B(1) + B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:36' b1 = (2*B(1) - 2*B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:37' b2 =  (B(1) - B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:39' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:40' a1 = (2*A(1) - 2*A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:41' a2 = (A(1) - A(2)*K + A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:42' num = [b0, b1, b2];
  K *= B_idx_1;
  rty_rateNum[0] = K / normalizer;
  rty_rateNum[1] = 0.0 / normalizer;
  rty_rateNum[2] = (0.0 - K) / normalizer;

  // 'computeDiscreteTFNumAndDen_function:43' den = [a0, a1, a2];
  rty_den[0] = 1.0;
  K = 2.0 * B_idx_1;
  rty_den[1] = (K - normalizer_tmp * 2.0) / normalizer;
  rty_den[2] = ((B_idx_1 - normalizer_tmp_0) + normalizer_tmp) / normalizer;

  // 'computeSecondOrderDerivFilterNumAndDen_function:28' B1 = 0;
  // 'computeSecondOrderDerivFilterNumAndDen_function:29' B2 = naturalFrequency_radps^2; 
  //  compute the accel transfer function numerator
  // 'computeSecondOrderDerivFilterNumAndDen_function:31' [accelNum, ~] = computeDiscreteTFNumAndDen_function([B0, B1, B2], [A0, A1, A2], K); 
  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  // 'computeDiscreteTFNumAndDen_function:32' elseif (nArray == 3)
  //  For 2nd order system
  // 'computeDiscreteTFNumAndDen_function:34' normalizer = A(1) + A(2)*K + A(3)*K^2; 
  // 'computeDiscreteTFNumAndDen_function:35' b0 = (B(1) + B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:36' b1 = (2*B(1) - 2*B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:37' b2 =  (B(1) - B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:39' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:40' a1 = (2*A(1) - 2*A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:41' a2 = (A(1) - A(2)*K + A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:42' num = [b0, b1, b2];
  B_idx_1 = normalizer_tmp * B_idx_1 / normalizer;
  rty_accelNum[0] = B_idx_1;
  rty_accelNum[1] = (0.0 - K * normalizer_tmp) / normalizer;
  rty_accelNum[2] = B_idx_1;

  // 'computeDiscreteTFNumAndDen_function:43' den = [a0, a1, a2];
}

//
// System initialize for atomic system:
//    '<S41>/Compute Filter Numerator And Denominator'
//    '<S26>/Compute Filter Numerator And Denominator'
//    '<S85>/Compute Filter Numerator And Denominator'
//    '<S70>/Compute Filter Numerator And Denominator'
//    '<S149>/Compute Filter Numerator And Denominator'
//    '<S134>/Compute Filter Numerator And Denominator'
//    '<S224>/Compute Filter Numerator And Denominator'
//    '<S209>/Compute Filter Numerator And Denominator'
//    '<S194>/Compute Filter Numerator And Denominator'
//
void fcsModel::ComputeFilterNumeratorAndD_Init(real_T rty_num[3], real_T
  rty_den[3])
{
  rty_num[0] = 0.0;
  rty_den[0] = 0.0;
  rty_num[1] = 0.0;
  rty_den[1] = 0.0;
  rty_num[2] = 0.0;
  rty_den[2] = 0.0;
}

//
// Output and update for atomic system:
//    '<S41>/Compute Filter Numerator And Denominator'
//    '<S26>/Compute Filter Numerator And Denominator'
//    '<S85>/Compute Filter Numerator And Denominator'
//    '<S70>/Compute Filter Numerator And Denominator'
//    '<S149>/Compute Filter Numerator And Denominator'
//    '<S134>/Compute Filter Numerator And Denominator'
//    '<S224>/Compute Filter Numerator And Denominator'
//    '<S209>/Compute Filter Numerator And Denominator'
//    '<S194>/Compute Filter Numerator And Denominator'
//
void fcsModel::ComputeFilterNumeratorAndDenomi(real_T rtu_naturalFrequency_radps,
  real_T rtu_dampingRatio_nd, real_T rty_num[3], real_T rty_den[3], real_T
  rtp_sampleTime_s)
{
  real_T B0;
  real_T B0_tmp;
  real_T B_idx_0;
  real_T K;
  real_T tmp;
  real_T tmp_0;

  //  Call the main function
  // MATLAB Function 'Discrete Second Order Filter/Compute Filter Numerator And Denominator': '<S50>:1' 
  // '<S50>:1:4' [num, den] = computeSecondOrderFilterNumAndDen_function(naturalFrequency_radps, dampingRatio_nd, sampleTime_s); 
  //  This function computes the numerator and denominator of the dicrete
  //  second order filter
  //
  // Inputs:
  // naturalFrequency_radps: Natural frequency of the filter
  // dampingRation_nd: Damping Ration of the filter
  // sampleTime_s: sampling time
  //
  // Outputs:
  // num: Numerator array for the discrete transfer function
  // den: Denominator array for the discrete transfer function
  // 'computeSecondOrderFilterNumAndDen_function:14' B0 = naturalFrequency_radps^2; 
  B0 = rtu_naturalFrequency_radps * rtu_naturalFrequency_radps;

  // 'computeSecondOrderFilterNumAndDen_function:15' B1 = 0;
  // 'computeSecondOrderFilterNumAndDen_function:16' B2 = 0;
  // 'computeSecondOrderFilterNumAndDen_function:18' A0 = B0;
  // 'computeSecondOrderFilterNumAndDen_function:19' A1 = 2*dampingRatio_nd*naturalFrequency_radps; 
  // 'computeSecondOrderFilterNumAndDen_function:20' A2 = 1;
  // 'computeSecondOrderFilterNumAndDen_function:22' K = 2/sampleTime_s;
  K = 2.0 / rtp_sampleTime_s;

  // 'computeSecondOrderFilterNumAndDen_function:24' [num, den] = computeDiscreteTFNumAndDen_function([B0, B1, B2], [A0, A1, A2], K); 
  B_idx_0 = B0;

  // COMPUTEDISCRETETFNUMANDDEN_FUNCTION computes the numerator and denominator
  //  for a first and second order discrete transfer function from it's
  //  continuous counterpart
  //
  //  Inputs:
  //  B: Array of coefficients of continuous transfer function numerator arranged 
  //  in ascending power of s
  //  A: Array of coefficients of continuous transfer function denominator arranged 
  //  in ascending power of s
  //  K: 2/sampling time in sec
  //
  //  Outputs:
  // num: numerator of the equivalent discrete transfer function in descending power of z 
  // den: denominator of the equivalent discrete transfer function in descending power of z 
  //  get the length of coefficient array to determine the order of transfer
  //  function
  // 'computeDiscreteTFNumAndDen_function:19' nArray = length(B);
  // 'computeDiscreteTFNumAndDen_function:21' if (nArray == 2)
  // 'computeDiscreteTFNumAndDen_function:32' elseif (nArray == 3)
  //  For 2nd order system
  // 'computeDiscreteTFNumAndDen_function:34' normalizer = A(1) + A(2)*K + A(3)*K^2; 
  B0_tmp = K * K;
  K *= 2.0 * rtu_dampingRatio_nd * rtu_naturalFrequency_radps;
  B0 = (K + B0) + B0_tmp;

  // 'computeDiscreteTFNumAndDen_function:35' b0 = (B(1) + B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:36' b1 = (2*B(1) - 2*B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:37' b2 =  (B(1) - B(2)*K + B(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:39' a0 = 1;
  // 'computeDiscreteTFNumAndDen_function:40' a1 = (2*A(1) - 2*A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:41' a2 = (A(1) - A(2)*K + A(3)*K^2)/normalizer; 
  // 'computeDiscreteTFNumAndDen_function:42' num = [b0, b1, b2];
  tmp = B_idx_0 / B0;
  rty_num[0] = tmp;
  tmp_0 = 2.0 * B_idx_0;
  rty_num[1] = tmp_0 / B0;
  rty_num[2] = tmp;

  // 'computeDiscreteTFNumAndDen_function:43' den = [a0, a1, a2];
  rty_den[0] = 1.0;
  rty_den[1] = (tmp_0 - B0_tmp * 2.0) / B0;
  rty_den[2] = ((B_idx_0 - K) + B0_tmp) / B0;
}

//
// System initialize for atomic system:
//    '<S21>/Signal Conditioning Block1'
//    '<S21>/Signal Conditioning Block'
//    '<S62>/Signal Conditioning Block1'
//    '<S62>/Signal Conditioning Block'
//
void fcsModel::f_SignalConditioningBlock1_Init(DW_SignalConditioningBlock1_f_T
  *localDW)
{
  // InitializeConditions for Delay: '<S42>/Delay'
  localDW->icLoad = true;

  // SystemInitialize for MATLAB Function: '<S41>/Compute Filter Numerator And Denominator' 
  ComputeFilterNumeratorAndD_Init(&localDW->num[0], &localDW->den[0]);
}

//
// Output and update for atomic system:
//    '<S21>/Signal Conditioning Block1'
//    '<S21>/Signal Conditioning Block'
//    '<S62>/Signal Conditioning Block1'
//    '<S62>/Signal Conditioning Block'
//
void fcsModel::fcsMod_SignalConditioningBlock1(real_T rtu_input, const
  busSignalConditioningParams *rtu_params, real_T *rty_filteredInput, real_T
  rtp_sampleTime_s, DW_SignalConditioningBlock1_f_T *localDW)
{
  std::array<real_T, 3> rtb_accelNum_i;
  std::array<real_T, 3> rtb_den;
  std::array<real_T, 3> rtb_rateNum;
  real_T rtb_DiscreteTransferFcn_j;
  real_T rtb_Switch2_h;

  // MATLAB Function: '<S40>/Compute Natural Frequency'
  fcsMode_ComputeNaturalFrequency(rtu_params->filterParams.filterBandwidth_radps,
    rtu_params->filterParams.dampingRatio_nd, &rtb_Switch2_h);

  // MATLAB Function: '<S40>/Compute Numerator And Denominator'
  ComputeNumeratorAndDenominator(rtb_Switch2_h,
    rtu_params->filterParams.dampingRatio_nd, &rtb_rateNum[0], &rtb_accelNum_i[0],
    &rtb_den[0], rtp_sampleTime_s);

  // MATLAB Function: '<S41>/Compute Natural Frequency'
  fcsMode_ComputeNaturalFrequency(rtu_params->filterParams.filterBandwidth_radps,
    rtu_params->filterParams.dampingRatio_nd, &rtb_Switch2_h);

  // MATLAB Function: '<S41>/Compute Filter Numerator And Denominator'
  ComputeFilterNumeratorAndDenomi(rtb_Switch2_h,
    rtu_params->filterParams.dampingRatio_nd, &localDW->num[0], &localDW->den[0],
    rtp_sampleTime_s);

  // DiscreteTransferFcn: '<S41>/Discrete Transfer Fcn'
  localDW->DiscreteTransferFcn_tmp = (rtu_input -
    localDW->DiscreteTransferFcn_states[0] * localDW->den[1]) -
    localDW->DiscreteTransferFcn_states[1] * localDW->den[2];
  rtb_DiscreteTransferFcn_j = (localDW->num[0] *
    localDW->DiscreteTransferFcn_tmp + localDW->DiscreteTransferFcn_states[0] *
    localDW->num[1]) + localDW->DiscreteTransferFcn_states[1] * localDW->num[2];

  // Switch: '<S45>/Switch2' incorporates:
  //   RelationalOperator: '<S45>/LowerRelop1'
  //   RelationalOperator: '<S45>/UpperRelop'
  //   Switch: '<S45>/Switch'

  if (rtb_DiscreteTransferFcn_j > rtu_params->filteredInputLimits[1]) {
    rtb_Switch2_h = rtu_params->filteredInputLimits[1];
  } else if (rtb_DiscreteTransferFcn_j < rtu_params->filteredInputLimits[0]) {
    // Switch: '<S45>/Switch'
    rtb_Switch2_h = rtu_params->filteredInputLimits[0];
  } else {
    rtb_Switch2_h = rtb_DiscreteTransferFcn_j;
  }

  // End of Switch: '<S45>/Switch2'

  // Delay: '<S42>/Delay'
  if (localDW->icLoad) {
    localDW->Delay_DSTATE = rtb_Switch2_h;
  }

  // Sum: '<S42>/Difference Inputs1' incorporates:
  //   Delay: '<S42>/Delay'
  //
  //  Block description for '<S42>/Difference Inputs1':
  //
  //   Add in CPU

  rtb_DiscreteTransferFcn_j = rtb_Switch2_h - localDW->Delay_DSTATE;

  // Switch: '<S52>/Switch2' incorporates:
  //   Product: '<S42>/delta rise limit'
  //   SampleTimeMath: '<S42>/sample time'
  //
  //  About '<S42>/sample time':
  //   y = K where K = ( w * Ts )
  //
  rtb_Switch2_h = rtu_params->filteredInputRateLimits[1] * 0.004;

  // Switch: '<S52>/Switch2' incorporates:
  //   RelationalOperator: '<S52>/LowerRelop1'

  if (rtb_DiscreteTransferFcn_j <= rtb_Switch2_h) {
    // Product: '<S42>/delta fall limit' incorporates:
    //   SampleTimeMath: '<S42>/sample time'
    //
    //  About '<S42>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rtb_Switch2_h = 0.004 * rtu_params->filteredInputRateLimits[0];

    // Switch: '<S52>/Switch' incorporates:
    //   RelationalOperator: '<S52>/UpperRelop'

    if (rtb_DiscreteTransferFcn_j >= rtb_Switch2_h) {
      // Switch: '<S52>/Switch2'
      rtb_Switch2_h = rtb_DiscreteTransferFcn_j;
    }

    // End of Switch: '<S52>/Switch'
  }

  // End of Switch: '<S52>/Switch2'

  // Sum: '<S42>/Difference Inputs2' incorporates:
  //   Delay: '<S42>/Delay'
  //
  //  Block description for '<S42>/Difference Inputs2':
  //
  //   Add in CPU

  *rty_filteredInput = rtb_Switch2_h + localDW->Delay_DSTATE;

  // Update for DiscreteTransferFcn: '<S41>/Discrete Transfer Fcn'
  localDW->DiscreteTransferFcn_states[1] = localDW->DiscreteTransferFcn_states[0];
  localDW->DiscreteTransferFcn_states[0] = localDW->DiscreteTransferFcn_tmp;

  // Update for Delay: '<S42>/Delay'
  localDW->icLoad = false;
  localDW->Delay_DSTATE = *rty_filteredInput;
}

//
// Output and update for atomic system:
//    '<S114>/holdOutputAtCenter1'
//    '<S114>/holdOutputAtCenter2'
//
void fcsModel::fcsModel_holdOutputAtCenter1(real_T rtu_input, real_T rtu_trigger,
  boolean_T rtu_auxTrigger, real_T *rty_output, boolean_T *rty_atCenter,
  DW_holdOutputAtCenter1_fcsMod_T *localDW)
{
  // MATLAB Function: '<S125>/holdOutputAtCenter'
  // MATLAB Function 'holdOutputAtCenter/holdOutputAtCenter': '<S128>:1'
  // '<S128>:1:2' [output, atCenter] = holdOutputAtCenter_function(input, trigger, auxTrigger, params); 
  // HOLDOUTPUTATCENTER_FUNCTION holds the output constant at last input if the
  // trigger value is within user defined delta from the center
  //
  // auxTrigger is an additional trigger that is used to indicate when to
  // actually start holding the output
  // 'holdOutputAtCenter_function:8' if isempty(last_input)
  // 'holdOutputAtCenter_function:12' if(trigger <= (params.center + params.posDeltaFromCenter) && ... 
  // 'holdOutputAtCenter_function:13'         trigger >=(params.center - params.negDeltaFromCenter)) && auxTrigger 
  if ((rtu_trigger <= 0.05) && (rtu_trigger >= -0.05) && rtu_auxTrigger) {
    // 'holdOutputAtCenter_function:14' atCenter = true;
    *rty_atCenter = true;
  } else {
    // 'holdOutputAtCenter_function:15' else
    // 'holdOutputAtCenter_function:16' atCenter = false;
    *rty_atCenter = false;

    // 'holdOutputAtCenter_function:17' last_input = input;
    localDW->last_input = rtu_input;
  }

  // 'holdOutputAtCenter_function:20' output = last_input;
  *rty_output = localDW->last_input;

  // End of MATLAB Function: '<S125>/holdOutputAtCenter'
}

//
// System initialize for atomic system:
//    '<S115>/pidWithDebug'
//    '<S169>/pidWithDebug'
//
void fcsModel::fcsModel_pidWithDebug_m_Init(DW_pidWithDebug_fcsModel_i_T
  *localDW)
{
  // InitializeConditions for DiscreteIntegrator: '<S132>/Discrete-Time Integrator' 
  localDW->DiscreteTimeIntegrator_IC_LOADI = 1U;

  // InitializeConditions for Delay: '<S164>/Delay'
  localDW->icLoad = true;
}

//
// Output and update for atomic system:
//    '<S115>/pidWithDebug'
//    '<S169>/pidWithDebug'
//
void fcsModel::fcsModel_pidWithDebug_j(real_T rtu_feedForward, real_T rtu_cmd,
  real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc, const
  busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T *rty_ctrlCmd,
  busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s,
  DW_pidWithDebug_fcsModel_i_T *localDW)
{
  real_T rtb_Product5_e;
  real_T rtb_Sum1_o;
  real_T rtb_Sum_b;
  real_T rtb_Switch2_b;
  real_T rtb_Switch2_d3;
  real_T rtb_UkYk1_h;
  real_T rtb_UnitDelay_a;

  // Product: '<S164>/delta rise limit' incorporates:
  //   SampleTimeMath: '<S164>/sample time'
  //
  //  About '<S164>/sample time':
  //   y = K where K = ( w * Ts )
  //
  rtb_Switch2_d3 = rtu_pidParamBus->outputRateLimits[1] * 0.008;

  // Sum: '<S132>/Sum'
  rtb_Sum_b = rtu_cmd - rtu_meas;

  // Outputs for Atomic SubSystem: '<S132>/Discrete First Order Deriv Filter'
  f_DiscreteFirstOrderDerivFilter(rtb_Sum_b,
    rtu_pidParamBus->filterBandwidth_radps, &rtb_Product5_e, rtp_sampleTime_s,
    &localDW->DiscreteFirstOrderDerivFilter);

  // End of Outputs for SubSystem: '<S132>/Discrete First Order Deriv Filter'

  // Product: '<S132>/Product'
  rtb_Product5_e *= rtu_pidParamBus->Kd;

  // Product: '<S132>/Product1'
  rtb_UnitDelay_a = rtb_Sum_b * rtu_pidParamBus->Kp;

  // DiscreteIntegrator: '<S132>/Discrete-Time Integrator'
  if (localDW->DiscreteTimeIntegrator_IC_LOADI != 0) {
    localDW->DiscreteTimeIntegrator_DSTATE = rtu_integratorIc;
  }

  if (rtu_integratorReset || (localDW->DiscreteTimeIntegrator_PrevRese != 0)) {
    localDW->DiscreteTimeIntegrator_DSTATE = rtu_integratorIc;
  }

  // Sum: '<S132>/Sum1' incorporates:
  //   DiscreteIntegrator: '<S132>/Discrete-Time Integrator'

  rtb_Sum1_o = ((rtu_feedForward + rtb_Product5_e) + rtb_UnitDelay_a) +
    localDW->DiscreteTimeIntegrator_DSTATE;

  // Switch: '<S165>/Switch2' incorporates:
  //   RelationalOperator: '<S165>/LowerRelop1'
  //   RelationalOperator: '<S165>/UpperRelop'
  //   Switch: '<S165>/Switch'

  if (rtb_Sum1_o > rtu_pidParamBus->outputLimits[1]) {
    rtb_Switch2_b = rtu_pidParamBus->outputLimits[1];
  } else if (rtb_Sum1_o < rtu_pidParamBus->outputLimits[0]) {
    // Switch: '<S165>/Switch'
    rtb_Switch2_b = rtu_pidParamBus->outputLimits[0];
  } else {
    rtb_Switch2_b = rtb_Sum1_o;
  }

  // End of Switch: '<S165>/Switch2'

  // Delay: '<S164>/Delay'
  if (localDW->icLoad) {
    localDW->Delay_DSTATE = rtb_Switch2_b;
  }

  // Sum: '<S164>/Difference Inputs1' incorporates:
  //   Delay: '<S164>/Delay'
  //
  //  Block description for '<S164>/Difference Inputs1':
  //
  //   Add in CPU

  rtb_UkYk1_h = rtb_Switch2_b - localDW->Delay_DSTATE;

  // Switch: '<S167>/Switch2' incorporates:
  //   RelationalOperator: '<S167>/LowerRelop1'

  if (rtb_UkYk1_h <= rtb_Switch2_d3) {
    // Product: '<S164>/delta fall limit' incorporates:
    //   SampleTimeMath: '<S164>/sample time'
    //
    //  About '<S164>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rtb_Switch2_d3 = 0.008 * rtu_pidParamBus->outputRateLimits[0];

    // Switch: '<S167>/Switch' incorporates:
    //   RelationalOperator: '<S167>/UpperRelop'

    if (rtb_UkYk1_h >= rtb_Switch2_d3) {
      rtb_Switch2_d3 = rtb_UkYk1_h;
    }

    // End of Switch: '<S167>/Switch'
  }

  // End of Switch: '<S167>/Switch2'

  // Sum: '<S164>/Difference Inputs2' incorporates:
  //   Delay: '<S164>/Delay'
  //
  //  Block description for '<S164>/Difference Inputs2':
  //
  //   Add in CPU

  *rty_ctrlCmd = rtb_Switch2_d3 + localDW->Delay_DSTATE;

  // BusCreator: '<S132>/Bus Creator' incorporates:
  //   DiscreteIntegrator: '<S132>/Discrete-Time Integrator'

  rty_pidDebug->output = *rty_ctrlCmd;
  rty_pidDebug->proportionalOutput = rtb_UnitDelay_a;
  rty_pidDebug->integralOutput = localDW->DiscreteTimeIntegrator_DSTATE;
  rty_pidDebug->derivativeOutput = rtb_Product5_e;

  // Update for DiscreteIntegrator: '<S132>/Discrete-Time Integrator' incorporates:
  //   Product: '<S132>/Product2'
  //   Product: '<S132>/Product3'
  //   Product: '<S132>/Product5'
  //   Sum: '<S132>/Sum2'
  //   Sum: '<S132>/Sum3'
  //   Sum: '<S132>/Sum4'
  //   Sum: '<S132>/Sum5'
  //   UnitDelay: '<S132>/Unit Delay'
  //   UnitDelay: '<S132>/Unit Delay1'

  localDW->DiscreteTimeIntegrator_IC_LOADI = 0U;
  localDW->DiscreteTimeIntegrator_DSTATE += (((rtu_trackingCtrlCmd -
    localDW->UnitDelay_DSTATE) * rtu_pidParamBus->Kt +
    (localDW->UnitDelay_DSTATE - localDW->UnitDelay1_DSTATE) *
    rtu_pidParamBus->Kb) + rtb_Sum_b * rtu_pidParamBus->Ki) * 0.008;
  localDW->DiscreteTimeIntegrator_PrevRese = static_cast<int8_T>
    (rtu_integratorReset);

  // Update for Delay: '<S164>/Delay'
  localDW->icLoad = false;
  localDW->Delay_DSTATE = *rty_ctrlCmd;

  // Update for UnitDelay: '<S132>/Unit Delay'
  localDW->UnitDelay_DSTATE = rtb_Switch2_b;

  // Update for UnitDelay: '<S132>/Unit Delay1'
  localDW->UnitDelay1_DSTATE = rtb_Sum1_o;
}

//
// System initialize for atomic system:
//    '<S115>/Signal Conditioning Block1'
//    '<S115>/Signal Conditioning Block'
//    '<S169>/Signal Conditioning Block2'
//    '<S169>/Signal Conditioning Block1'
//
void fcsModel::SignalConditioningBlock1_c_Init(DW_SignalConditioningBlock1_g_T
  *localDW)
{
  // InitializeConditions for Delay: '<S150>/Delay'
  localDW->icLoad = true;

  // SystemInitialize for MATLAB Function: '<S149>/Compute Filter Numerator And Denominator' 
  ComputeFilterNumeratorAndD_Init(&localDW->num[0], &localDW->den[0]);
}

//
// Output and update for atomic system:
//    '<S115>/Signal Conditioning Block1'
//    '<S115>/Signal Conditioning Block'
//    '<S169>/Signal Conditioning Block2'
//    '<S169>/Signal Conditioning Block1'
//
void fcsModel::fcsM_SignalConditioningBlock1_f(real_T rtu_input, const
  busSignalConditioningParams *rtu_params, real_T *rty_filteredInput, real_T
  rtp_sampleTime_s, DW_SignalConditioningBlock1_g_T *localDW)
{
  std::array<real_T, 3> rtb_accelNum_i;
  std::array<real_T, 3> rtb_den;
  std::array<real_T, 3> rtb_rateNum;
  real_T rtb_DiscreteTransferFcn_d;
  real_T rtb_Switch2_n;

  // MATLAB Function: '<S148>/Compute Natural Frequency'
  fcsMode_ComputeNaturalFrequency(rtu_params->filterParams.filterBandwidth_radps,
    rtu_params->filterParams.dampingRatio_nd, &rtb_Switch2_n);

  // MATLAB Function: '<S148>/Compute Numerator And Denominator'
  ComputeNumeratorAndDenominator(rtb_Switch2_n,
    rtu_params->filterParams.dampingRatio_nd, &rtb_rateNum[0], &rtb_accelNum_i[0],
    &rtb_den[0], rtp_sampleTime_s);

  // MATLAB Function: '<S149>/Compute Natural Frequency'
  fcsMode_ComputeNaturalFrequency(rtu_params->filterParams.filterBandwidth_radps,
    rtu_params->filterParams.dampingRatio_nd, &rtb_Switch2_n);

  // MATLAB Function: '<S149>/Compute Filter Numerator And Denominator'
  ComputeFilterNumeratorAndDenomi(rtb_Switch2_n,
    rtu_params->filterParams.dampingRatio_nd, &localDW->num[0], &localDW->den[0],
    rtp_sampleTime_s);

  // DiscreteTransferFcn: '<S149>/Discrete Transfer Fcn'
  localDW->DiscreteTransferFcn_tmp = (rtu_input -
    localDW->DiscreteTransferFcn_states[0] * localDW->den[1]) -
    localDW->DiscreteTransferFcn_states[1] * localDW->den[2];
  rtb_DiscreteTransferFcn_d = (localDW->num[0] *
    localDW->DiscreteTransferFcn_tmp + localDW->DiscreteTransferFcn_states[0] *
    localDW->num[1]) + localDW->DiscreteTransferFcn_states[1] * localDW->num[2];

  // Switch: '<S153>/Switch2' incorporates:
  //   RelationalOperator: '<S153>/LowerRelop1'
  //   RelationalOperator: '<S153>/UpperRelop'
  //   Switch: '<S153>/Switch'

  if (rtb_DiscreteTransferFcn_d > rtu_params->filteredInputLimits[1]) {
    rtb_Switch2_n = rtu_params->filteredInputLimits[1];
  } else if (rtb_DiscreteTransferFcn_d < rtu_params->filteredInputLimits[0]) {
    // Switch: '<S153>/Switch'
    rtb_Switch2_n = rtu_params->filteredInputLimits[0];
  } else {
    rtb_Switch2_n = rtb_DiscreteTransferFcn_d;
  }

  // End of Switch: '<S153>/Switch2'

  // Delay: '<S150>/Delay'
  if (localDW->icLoad) {
    localDW->Delay_DSTATE = rtb_Switch2_n;
  }

  // Sum: '<S150>/Difference Inputs1' incorporates:
  //   Delay: '<S150>/Delay'
  //
  //  Block description for '<S150>/Difference Inputs1':
  //
  //   Add in CPU

  rtb_DiscreteTransferFcn_d = rtb_Switch2_n - localDW->Delay_DSTATE;

  // Switch: '<S160>/Switch2' incorporates:
  //   Product: '<S150>/delta rise limit'
  //   SampleTimeMath: '<S150>/sample time'
  //
  //  About '<S150>/sample time':
  //   y = K where K = ( w * Ts )
  //
  rtb_Switch2_n = rtu_params->filteredInputRateLimits[1] * 0.008;

  // Switch: '<S160>/Switch2' incorporates:
  //   RelationalOperator: '<S160>/LowerRelop1'

  if (rtb_DiscreteTransferFcn_d <= rtb_Switch2_n) {
    // Product: '<S150>/delta fall limit' incorporates:
    //   SampleTimeMath: '<S150>/sample time'
    //
    //  About '<S150>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rtb_Switch2_n = 0.008 * rtu_params->filteredInputRateLimits[0];

    // Switch: '<S160>/Switch' incorporates:
    //   RelationalOperator: '<S160>/UpperRelop'

    if (rtb_DiscreteTransferFcn_d >= rtb_Switch2_n) {
      // Switch: '<S160>/Switch2'
      rtb_Switch2_n = rtb_DiscreteTransferFcn_d;
    }

    // End of Switch: '<S160>/Switch'
  }

  // End of Switch: '<S160>/Switch2'

  // Sum: '<S150>/Difference Inputs2' incorporates:
  //   Delay: '<S150>/Delay'
  //
  //  Block description for '<S150>/Difference Inputs2':
  //
  //   Add in CPU

  *rty_filteredInput = rtb_Switch2_n + localDW->Delay_DSTATE;

  // Update for DiscreteTransferFcn: '<S149>/Discrete Transfer Fcn'
  localDW->DiscreteTransferFcn_states[1] = localDW->DiscreteTransferFcn_states[0];
  localDW->DiscreteTransferFcn_states[0] = localDW->DiscreteTransferFcn_tmp;

  // Update for Delay: '<S150>/Delay'
  localDW->icLoad = false;
  localDW->Delay_DSTATE = *rty_filteredInput;
}

//
// Function for Chart: '<S4>/Chart'
// function isTrue = checkRcCmds
//
boolean_T fcsModel::fcsModel_checkRcCmds(const busRcInCmds
  *BusConversion_InsertedFor_Chart)
{
  boolean_T isTrue;

  // MATLAB Function 'checkRcCmds': '<S243>:7'
  // '<S243>:7:2' pwmLowVal = paramsStruct.pwmLimits(1);
  // '<S243>:7:3' if(rcCmds.throttleCmd_nd <= paramsStruct.pwmLimitsThrottle(1) && ... 
  // '<S243>:7:4'        rcCmds.joystickYCmd_nd <= pwmLowVal && ...
  // '<S243>:7:5'        rcCmds.joystickXCmd_nd <= pwmLowVal && ...
  // '<S243>:7:6'        rcCmds.joystickZCmd_nd <= pwmLowVal)
  if (BusConversion_InsertedFor_Chart->throttleCmd_nd <= 1000) {
    if (BusConversion_InsertedFor_Chart->joystickYCmd_nd <= 1000) {
      if (BusConversion_InsertedFor_Chart->joystickXCmd_nd <= 1000) {
        if (BusConversion_InsertedFor_Chart->joystickZCmd_nd <= 1000) {
          // '<S243>:7:7' isTrue = true;
          isTrue = true;
        } else {
          // '<S243>:7:8' else
          // '<S243>:7:9' isTrue = false;
          isTrue = false;
        }
      } else {
        // '<S243>:7:8' else
        // '<S243>:7:9' isTrue = false;
        isTrue = false;
      }
    } else {
      // '<S243>:7:8' else
      // '<S243>:7:9' isTrue = false;
      isTrue = false;
    }
  } else {
    // '<S243>:7:8' else
    // '<S243>:7:9' isTrue = false;
    isTrue = false;
  }

  return isTrue;
}

real_T rt_urand_Upu32_Yd_f_pw(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  // Uniform random number generator (random number between 0 and 1)

  // #define IA      16807                      magic multiplier = 7^5
  // #define IM      2147483647                 modulus = 2^31-1
  // #define IQ      127773                     IM div IA
  // #define IR      2836                       IM modulo IA
  // #define S       4.656612875245797e-10      reciprocal of 2^31-1
  // test = IA * (seed % IQ) - IR * (seed/IQ)
  // seed = test < 0 ? (test + IM) : test
  // return (seed*S)

  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return static_cast<real_T>(*u) * 4.6566128752457969E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  // Normal (Gaussian) random number generator
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = std::sqrt(-2.0 * std::log(si) / si) * sr;
  return y;
}

// Model step function
void fcsModel::step()
{
  std::array<real_T, 4> rtb_DiscreteTransferFcn_e;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_cmd_at_i;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_filtCmd_;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_filtMeas;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_meas_at_;
  std::array<real_T, 4> rtb_ImpAsg_InsertedFor_mtrPwmCm;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_neVelCmd;
  std::array<busPidDebug, 3> rtb_ImpAsg_InsertedFor_pidDeb_m;
  std::array<busPidDebug, 3> rtb_ImpAsg_InsertedFor_pidDebug;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_velCtrlF;
  std::array<real_T, 3> rtb_ImpAsg_InsertedFor_velCtrlO;
  std::array<real_T, 3> rtb_MatrixMultiply;
  std::array<real_T, 9> rtb_Transpose;
  std::array<real_T, 9> rtb_Transpose_h;
  std::array<busCtrlInputs, 3> rtb_VectorConcatenate;
  std::array<real_T, 3> rtb_VectorConcatenate1;
  busCtrlInputs rtb_BusAssignment2;
  busCtrlInputs rtb_BusAssignment3_g;
  busCtrlInputs rtb_BusAssignment4;
  busPidDebug rtb_BusCreator_og;
  real_T DiscreteTransferFcn;
  real_T az;
  real_T fE;
  real_T plim;
  real_T rlim;
  real_T rlim_0;
  real_T rlim_1;
  real_T rlim_2;
  real_T rtb_ImpAsg_InsertedFor_angRat_0;
  real_T rtb_ImpAsg_InsertedFor_angRat_1;
  real_T rtb_ImpAsg_InsertedFor_angRat_2;
  real_T rtb_Product_i;
  real_T vxCmd_unitRange;
  real_T vylim;
  real_T yCmd;
  real_T ylim;
  int32_T pCmd;
  int32_T rCmd;
  int32_T tCmd;
  uint32_T rtb_Prelookup_o1;
  boolean_T resetIntegrator;
  boolean_T rtb_AND_p;
  boolean_T rtb_Compare;
  boolean_T rtb_Compare_od;
  boolean_T tmp;
  boolean_T tmp_0;
  enumChirpTrigger rtb_chirpTrigger;
  enumChirpType rtb_chirpType;
  enumFlightMode flightMode;
  enumStateMachine state;

  // RateTransition: '<Root>/Rate Transition'
  tmp = ((&fcsModel_M)->Timing.TaskCounters.TID[1] == 0);
  if (tmp) {
    // Math: '<S114>/Transpose' incorporates:
    //   Inport: '<Root>/stateEstimate'
    //   Math: '<S111>/Transpose'

    tCmd = 0;
    for (rCmd = 0; rCmd < 3; rCmd++) {
      rtb_Transpose[tCmd] = ExtUPointer_ref_U->stateEstimate.ned2FepDcm_nd[rCmd];
      rtb_Transpose_h[tCmd] = ExtUPointer_ref_U->
        stateEstimate.ned2BodyDcm_nd[rCmd];
      rtb_Transpose[tCmd + 1] = ExtUPointer_ref_U->
        stateEstimate.ned2FepDcm_nd[rCmd + 3];
      rtb_Transpose_h[tCmd + 1] =
        ExtUPointer_ref_U->stateEstimate.ned2BodyDcm_nd[rCmd + 3];
      rtb_Transpose[tCmd + 2] = ExtUPointer_ref_U->
        stateEstimate.ned2FepDcm_nd[rCmd + 6];
      rtb_Transpose_h[tCmd + 2] =
        ExtUPointer_ref_U->stateEstimate.ned2BodyDcm_nd[rCmd + 6];
      tCmd += 3;
    }

    // End of Math: '<S114>/Transpose'
  }

  // Chart: '<S4>/Chart' incorporates:
  //   BusCreator generated from: '<S4>/Chart'
  //   Inport: '<Root>/externalCmds'
  //   Inport: '<Root>/rcCmdsIn'

  // Gateway: rcInterpreter/Chart
  if (fcsModel_DW.temporalCounter_i1 < 16383) {
    fcsModel_DW.temporalCounter_i1 = static_cast<uint16_T>
      (fcsModel_DW.temporalCounter_i1 + 1);
  }

  // During: rcInterpreter/Chart
  if (fcsModel_DW.is_active_c1_rcInterpreter == 0) {
    // Entry: rcInterpreter/Chart
    fcsModel_DW.is_active_c1_rcInterpreter = 1U;

    // Entry Internal: rcInterpreter/Chart
    // Transition: '<S243>:2'
    fcsModel_DW.durationCounter_1 = 0U;
    fcsModel_DW.is_c1_rcInterpreter = fcsModel_IN_INACTIVE;

    // Entry 'INACTIVE': '<S243>:1'
    // '<S243>:1:2' state = enumStateMachine.INACTIVE;
    state = enumStateMachine::INACTIVE;

    // '<S243>:1:3' rcCheckFlag = checkRcCmds;
    fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds(&ExtUPointer_ref_U->rcCmdsIn);
    if (!fcsModel_DW.rcCheckFlag) {
      fcsModel_DW.durationCounter_1_d = 0U;
    }

    // '<S243>:1:4' resetIntegrator = true;
    resetIntegrator = true;
  } else {
    switch (fcsModel_DW.is_c1_rcInterpreter) {
     case fcsModel_IN_ARM_MTRS:
      // During 'ARM_MTRS': '<S243>:3'
      // '<S243>:10:1' sf_internal_predicateOutput = 0 | ((after(60, sec) || duration(rcCheckFlag == true, sec) >= 5) || ... 
      // '<S243>:10:2' (externalCmds.armCmdIssued == 2));
      if (fcsModel_DW.temporalCounter_i1 >= 15000) {
        resetIntegrator = true;
      } else {
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        resetIntegrator = ((fcsModel_DW.durationCounter_1_d >= 1250U) ||
                           (ExtUPointer_ref_U->externalCmds.armCmdIssued == 2));
      }

      if (resetIntegrator) {
        // Transition: '<S243>:10'
        fcsModel_DW.durationCounter_1 = 0U;
        fcsModel_DW.is_c1_rcInterpreter = fcsModel_IN_INACTIVE;

        // Entry 'INACTIVE': '<S243>:1'
        // '<S243>:1:2' state = enumStateMachine.INACTIVE;
        state = enumStateMachine::INACTIVE;

        // '<S243>:1:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        // '<S243>:1:4' resetIntegrator = true;

        // '<S243>:12:1' sf_internal_predicateOutput = 0 | (rcCmds.throttleCmd_nd > paramsStruct.pwmLimitsThrottle(1)); 
      } else if (ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd > 1000) {
        // Transition: '<S243>:12'
        fcsModel_DW.is_c1_rcInterpreter = fcsModel_IN_INFLIGHT;

        // Entry 'INFLIGHT': '<S243>:11'
        // '<S243>:11:2' state = enumStateMachine.INFLIGHT;
        state = enumStateMachine::INFLIGHT;

        // '<S243>:11:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        // '<S243>:11:4' resetIntegrator = false;
      } else {
        // '<S243>:3:2' state = enumStateMachine.MTR_ARMED;
        state = enumStateMachine::MTR_ARMED;

        // '<S243>:3:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        // '<S243>:3:4' resetIntegrator = true;
        resetIntegrator = true;
      }
      break;

     case fcsModel_IN_INACTIVE:
      // During 'INACTIVE': '<S243>:1'
      // '<S243>:5:1' sf_internal_predicateOutput = 0 | ((duration(rcCheckFlag, sec) >= 1 && rcCmds.throttleCmd_nd >= 900) || ... 
      // '<S243>:5:2' (externalCmds.armCmdIssued == 1));
      if (!fcsModel_DW.rcCheckFlag) {
        fcsModel_DW.durationCounter_1 = 0U;
      }

      if (((fcsModel_DW.durationCounter_1 >= 250U) &&
           (ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd >= 900)) ||
          (ExtUPointer_ref_U->externalCmds.armCmdIssued == 1)) {
        // Transition: '<S243>:5'
        fcsModel_DW.temporalCounter_i1 = 0U;
        fcsModel_DW.durationCounter_1_d = 0U;
        fcsModel_DW.is_c1_rcInterpreter = fcsModel_IN_ARM_MTRS;

        // Entry 'ARM_MTRS': '<S243>:3'
        // '<S243>:3:2' state = enumStateMachine.MTR_ARMED;
        state = enumStateMachine::MTR_ARMED;

        // '<S243>:3:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
        }

        // '<S243>:3:4' resetIntegrator = true;
        resetIntegrator = true;
      } else {
        // '<S243>:1:2' state = enumStateMachine.INACTIVE;
        state = enumStateMachine::INACTIVE;

        // '<S243>:1:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        // '<S243>:1:4' resetIntegrator = true;
        resetIntegrator = true;
      }
      break;

     default:
      // During 'INFLIGHT': '<S243>:11'
      // '<S243>:20:1' sf_internal_predicateOutput = 0 | (rcCmds.throttleCmd_nd <= paramsStruct.pwmLimitsThrottle (1)); 
      if (ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd <= 1000) {
        // Transition: '<S243>:20'
        fcsModel_DW.temporalCounter_i1 = 0U;
        fcsModel_DW.durationCounter_1_d = 0U;
        fcsModel_DW.is_c1_rcInterpreter = fcsModel_IN_ARM_MTRS;

        // Entry 'ARM_MTRS': '<S243>:3'
        // '<S243>:3:2' state = enumStateMachine.MTR_ARMED;
        state = enumStateMachine::MTR_ARMED;

        // '<S243>:3:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
        }

        // '<S243>:3:4' resetIntegrator = true;
        resetIntegrator = true;
      } else {
        // '<S243>:11:2' state = enumStateMachine.INFLIGHT;
        state = enumStateMachine::INFLIGHT;

        // '<S243>:11:3' rcCheckFlag = checkRcCmds;
        fcsModel_DW.rcCheckFlag = fcsModel_checkRcCmds
          (&ExtUPointer_ref_U->rcCmdsIn);
        if (!fcsModel_DW.rcCheckFlag) {
          fcsModel_DW.durationCounter_1 = 0U;
          fcsModel_DW.durationCounter_1_d = 0U;
        }

        // '<S243>:11:4' resetIntegrator = false;
        resetIntegrator = false;
      }
      break;
    }
  }

  if (fcsModel_DW.rcCheckFlag) {
    fcsModel_DW.durationCounter_1++;
    fcsModel_DW.durationCounter_1_d++;
  } else {
    fcsModel_DW.durationCounter_1 = 0U;
    fcsModel_DW.durationCounter_1_d = 0U;
  }

  // End of Chart: '<S4>/Chart'

  // MATLAB Function: '<S4>/Interpret RC In Cmds' incorporates:
  //   BusCreator generated from: '<S4>/Interpret RC In Cmds'
  //   Inport: '<Root>/rcCmdsIn'
  //   UnitDelay: '<S4>/Unit Delay'

  // Computes command and flight mode from the rc inputs
  // MATLAB Function 'rcInterpreter/Interpret RC In Cmds': '<S244>:1'
  // '<S244>:1:3' [flightMode, rcOutCmds, chirpTrigger, chirpType] = interpretRcInputs_function(rcCmds, prevChirpTrigger, rcParamsStruct); 
  // INTERPRETRCINPUTS_FUNCTION
  // Computes command and flight mode from the rc inputs
  //  In Alt hold mode or Pos hold mode throttle bottom position is
  //  mapped to max descent rate which then decreases to zero when throttle is
  //  mid-stick on the RC transmitter and from there as throttle is increased
  //  throttle top position is mapped to max asent rate. This works well when
  //  vehicle is already in the air. For special case when we start the Alt
  //  hold mode or Pos hold from takeoff having bottom half of throttle mapped
  //  to descent rate windsup the climbrate controller integrator as the
  //  vehicle is on the ground with zero vertical velocity and vehicle is being
  //  commanded to descent. Also having the bottom half of the throttle mapped
  //  to descent rate means pilot has to move the stick to half throttle before
  //  vehicle is commanded to ascent during takeoff. A simple solution to fix this special 
  //  case is to have a flag that is not set when throttle is at bottom and set
  //  when the throttle reaches the mid stick. This way the flag can be used to
  //  set entire throttle stick range to be climb rate when we are taking off
  //  and once we reach mid throttle the throttle range will be split into
  //  equal parts descent and ascent rate
  // 'interpretRcInputs_function:24' if isempty(throttle_is_up)
  // 'interpretRcInputs_function:29' if(rcInCmds.rcSwitch3_nd >= 1500)
  if (ExtUPointer_ref_U->rcCmdsIn.rcSwitch3_nd >= 1500) {
    // 'interpretRcInputs_function:30' if(prevChirpTrigger == enumChirpTrigger.OFF) 
    if (fcsModel_DW.UnitDelay_DSTATE_g == enumChirpTrigger::OFF) {
      // 'interpretRcInputs_function:31' chirpCount_ = chirpCount_ + uint8(1);
      rtb_Prelookup_o1 = fcsModel_DW.chirpCount_ + 1U;
      if (fcsModel_DW.chirpCount_ + 1U > 255U) {
        rtb_Prelookup_o1 = 255U;
      }

      fcsModel_DW.chirpCount_ = static_cast<uint8_T>(rtb_Prelookup_o1);
    }

    // 'interpretRcInputs_function:33' chirpTrigger = enumChirpTrigger.ON;
    rtb_chirpTrigger = enumChirpTrigger::ON;
  } else {
    // 'interpretRcInputs_function:34' else
    // 'interpretRcInputs_function:35' if(chirpCount_ == enumChirpType.FZ)
    if (fcsModel_DW.chirpCount_ == static_cast<int32_T>(enumChirpType::FZ)) {
      // 'interpretRcInputs_function:36' chirpCount_ = uint8(0);
      fcsModel_DW.chirpCount_ = 0U;
    }

    // 'interpretRcInputs_function:38' chirpTrigger = enumChirpTrigger.OFF;
    rtb_chirpTrigger = enumChirpTrigger::OFF;
  }

  // 'interpretRcInputs_function:41' switch chirpCount_
  switch (fcsModel_DW.chirpCount_) {
   case 0U:
    // 'interpretRcInputs_function:42' case 0
    // 'interpretRcInputs_function:43' chirpType = enumChirpType.NONE;
    rtb_chirpType = enumChirpType::NONE;
    break;

   case 1U:
    // 'interpretRcInputs_function:44' case 1
    // 'interpretRcInputs_function:45' chirpType = enumChirpType.MX;
    rtb_chirpType = enumChirpType::MX;
    break;

   case 2U:
    // 'interpretRcInputs_function:46' case 2
    // 'interpretRcInputs_function:47' chirpType = enumChirpType.MY;
    rtb_chirpType = enumChirpType::MY;
    break;

   case 3U:
    // 'interpretRcInputs_function:48' case 3
    // 'interpretRcInputs_function:49' chirpType = enumChirpType.MZ;
    rtb_chirpType = enumChirpType::MZ;
    break;

   case 4U:
    // 'interpretRcInputs_function:50' case 4
    // 'interpretRcInputs_function:51' chirpType = enumChirpType.FZ;
    rtb_chirpType = enumChirpType::FZ;
    break;

   default:
    // 'interpretRcInputs_function:52' otherwise
    // 'interpretRcInputs_function:53' chirpType = enumChirpType.NONE;
    rtb_chirpType = enumChirpType::NONE;
    break;
  }

  //  Used to directly set force commands in N that is fed into allocation in
  //  STABILIZE and ACRO flight modes
  // 'interpretRcInputs_function:59' rcOutCmds.throttleStick = 0;
  //  Used to command roll and pitch angles in STABILIZE flight mode
  // 'interpretRcInputs_function:62' rcOutCmds.rollStick = 0;
  // 'interpretRcInputs_function:63' rcOutCmds.pitchStick = 0;
  //  Used to control yaw rate in all flight modes
  // 'interpretRcInputs_function:66' rcOutCmds.yawStick = 0;
  //  Not used currently
  // 'interpretRcInputs_function:69' rcOutCmds.vxStick_mps = 0;
  // 'interpretRcInputs_function:70' rcOutCmds.vyStick_mps = 0;
  //  Used to command vertical velocity in ALT_CONTROL flight mode. In this
  //  mode rcOutCmds.throttleStick is ignored.
  // 'interpretRcInputs_function:74' rcOutCmds.vzStick_mps = 0;
  // Select mode and set the command limits
  // 'interpretRcInputs_function:77' if(rcInCmds.rcSwitch1_nd < 1100)
  if (ExtUPointer_ref_U->rcCmdsIn.rcSwitch1_nd < 1100) {
    // 'interpretRcInputs_function:78' flightMode = enumFlightMode.STABILIZE;
    flightMode = enumFlightMode::STABILIZE;

    // 'interpretRcInputs_function:79' rlim = rcParamsStruct.cmdLimits.roll_rad(2); 
    rlim = 0.78539816339744828;

    // 'interpretRcInputs_function:80' plim = rcParamsStruct.cmdLimits.pitch_rad(2); 
    plim = 0.78539816339744828;

    // 'interpretRcInputs_function:81' ylim = rcParamsStruct.cmdLimits.yawRate_radps(2); 
    // 'interpretRcInputs_function:82' vxlim = rcParamsStruct.cmdLimits.vx_mps(2); 
    // 'interpretRcInputs_function:83' vylim = rcParamsStruct.cmdLimits.vy_mps(2); 
  } else if ((ExtUPointer_ref_U->rcCmdsIn.rcSwitch1_nd >= 1100) &&
             (ExtUPointer_ref_U->rcCmdsIn.rcSwitch1_nd < 1700)) {
    // 'interpretRcInputs_function:85' elseif (rcInCmds.rcSwitch1_nd >= 1100 && rcInCmds.rcSwitch1_nd < 1700) 
    // 'interpretRcInputs_function:86' flightMode = enumFlightMode.ALT_CONTROL;
    flightMode = enumFlightMode::ALT_CONTROL;

    // 'interpretRcInputs_function:87' vxlim = rcParamsStruct.cmdLimits.vx_mps(2); 
    // 'interpretRcInputs_function:88' vylim = rcParamsStruct.cmdLimits.vy_mps(2); 
    // 'interpretRcInputs_function:89' rlim = rcParamsStruct.cmdLimits.roll_rad(2); 
    rlim = 0.78539816339744828;

    // 'interpretRcInputs_function:90' plim = rcParamsStruct.cmdLimits.pitch_rad(2); 
    plim = 0.78539816339744828;

    // 'interpretRcInputs_function:91' ylim = rcParamsStruct.cmdLimits.yawRate_radps(2); 
  } else if (ExtUPointer_ref_U->rcCmdsIn.rcSwitch1_nd >= 1700) {
    // 'interpretRcInputs_function:92' elseif (rcInCmds.rcSwitch1_nd >= 1700)
    // 'interpretRcInputs_function:93' flightMode = enumFlightMode.POS_CONTROL;
    flightMode = enumFlightMode::POS_CONTROL;

    // 'interpretRcInputs_function:94' vxlim = rcParamsStruct.cmdLimits.vx_mps(2); 
    // 'interpretRcInputs_function:95' vylim = rcParamsStruct.cmdLimits.vy_mps(2); 
    // 'interpretRcInputs_function:96' rlim = 0;
    rlim = 0.0;

    // 'interpretRcInputs_function:97' plim = 0;
    plim = 0.0;

    // 'interpretRcInputs_function:98' ylim = rcParamsStruct.cmdLimits.yawRate_radps(2); 
  } else {
    // 'interpretRcInputs_function:99' else
    // 'interpretRcInputs_function:100' flightMode = enumFlightMode.STABILIZE;
    flightMode = enumFlightMode::STABILIZE;

    // 'interpretRcInputs_function:101' rlim = rcParamsStruct.cmdLimits.roll_rad(2); 
    rlim = 0.78539816339744828;

    // 'interpretRcInputs_function:102' plim = rcParamsStruct.cmdLimits.pitch_rad(2); 
    plim = 0.78539816339744828;

    // 'interpretRcInputs_function:103' ylim = rcParamsStruct.cmdLimits.yawRate_radps(2); 
    // 'interpretRcInputs_function:104' vxlim = rcParamsStruct.cmdLimits.vx_mps(2); 
    // 'interpretRcInputs_function:105' vylim = rcParamsStruct.cmdLimits.vy_mps(2); 
  }

  // 'interpretRcInputs_function:108' if double(rcInCmds.throttleCmd_nd) < rcParamsStruct.pwmLimits(1) 
  if (ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd < 1000) {
    // This means either we haven't take off yet or we landed after a flight
    // and might take of again so set the throttle_is_up to false
    // 'interpretRcInputs_function:111' throttle_is_up = false;
    fcsModel_DW.throttle_is_up = false;
  } else if ((ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd <= 1450) &&
             (ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd >= 1300)) {
    // 'interpretRcInputs_function:112' elseif ((double(rcInCmds.throttleCmd_nd) <= rcParamsStruct.pwmThrottleMidHigh) && ... 
    // 'interpretRcInputs_function:113'         double(rcInCmds.throttleCmd_nd) >= rcParamsStruct.pwmThrottleMidLow) 
    // Since throttle stick on RC transmitter is half way up this probably
    // means the vehicle is flying so set throttle_is_up flag to true
    // 'interpretRcInputs_function:116' throttle_is_up = true;
    fcsModel_DW.throttle_is_up = true;
  } else {
    // 'interpretRcInputs_function:117' else
    // Otherwise preserve the previous flag
  }

  // 'interpretRcInputs_function:121' tCmd = min( rcParamsStruct.pwmLimitsThrottle(2), ... 
  // 'interpretRcInputs_function:122'     max( rcParamsStruct.pwmLimitsThrottle(1), double(rcInCmds.throttleCmd_nd) ) ); 
  tCmd = static_cast<int32_T>(std::fmin(1900.0, std::fmax(1000.0,
    static_cast<real_T>(ExtUPointer_ref_U->rcCmdsIn.throttleCmd_nd))));

  //  In stabilize mode throttle stick starts at 0
  // 'interpretRcInputs_function:125' tCmd_unitRange = (rcParamsStruct.throttleUnitRangeMapCoeff.a*tCmd + ... 
  // 'interpretRcInputs_function:126'     rcParamsStruct.throttleUnitRangeMapCoeff.c)/rcParamsStruct.throttleUnitRangeMapCoeff.b; 
  //  Use throttle stick to set Vz commands. Always set but only used
  //  in ALT_CONTROL flight mode. Has different slopes about the center point
  //  as the center point is not always at 1500 which is PWM center
  // 'interpretRcInputs_function:131' if ((tCmd <= rcParamsStruct.pwmThrottleMidHigh) && ... 
  // 'interpretRcInputs_function:132'         tCmd >= rcParamsStruct.pwmThrottleMidLow) 
  if ((tCmd <= 1450) && (tCmd >= 1300)) {
    // 'interpretRcInputs_function:133' rcOutCmds.vzStick_mps = 0;
    fcsModel_DW.rcOutCmds.vzStick_mps = 0.0;
  } else if (tCmd < 1300) {
    // 'interpretRcInputs_function:134' elseif (tCmd < rcParamsStruct.pwmThrottleMidLow) 
    // 'interpretRcInputs_function:135' if (throttle_is_up)
    if (fcsModel_DW.throttle_is_up) {
      //  This means we are already flying, make lower half of throttle
      //  stick map to descent rates
      // 'interpretRcInputs_function:138' vzCmd_unitRange = rcParamsStruct.pwmToCmdThrottleSlopeLow*tCmd + ...; 
      // 'interpretRcInputs_function:139'             rcParamsStruct.pwmToCmdThrottleIncptLow; 
      // ;
      // 'interpretRcInputs_function:140' rcOutCmds.vzStick_mps = (rcParamsStruct.vzLowRangeMapCoeff.a*vzCmd_unitRange + ... 
      // 'interpretRcInputs_function:141'             rcParamsStruct.vzLowRangeMapCoeff.c)/ ... 
      // 'interpretRcInputs_function:142'             rcParamsStruct.vzLowRangeMapCoeff.b; 
      fcsModel_DW.rcOutCmds.vzStick_mps = -(0.0033333333333333335 * static_cast<
        real_T>(tCmd) - 4.333333333333333);
    } else {
      // 'interpretRcInputs_function:143' else
      //  This means we haven't taken off yet or we landed and might take
      //  off again. Map the lower half of the throttle stick also to
      //  ascent rate as we want to climb up from the ground and only go
      //  upto 50% of the max ascent rate at mid point. Lowest position is
      //  throttle maps to -1 so add +1 make lowest throttle point 0 and
      //  increase from there on
      // 'interpretRcInputs_function:150' vzCmd_unitRange = ((rcParamsStruct.pwmToCmdThrottleSlopeLow*tCmd + ...; 
      // 'interpretRcInputs_function:151'             rcParamsStruct.pwmToCmdThrottleIncptLow) + 1)*0.50; 
      // ;
      // 'interpretRcInputs_function:152' rcOutCmds.vzStick_mps = (rcParamsStruct.vzHighRangeMapCoeff.a*vzCmd_unitRange + ... 
      // 'interpretRcInputs_function:153'             rcParamsStruct.vzHighRangeMapCoeff.c)/ ... 
      // 'interpretRcInputs_function:154'             rcParamsStruct.vzHighRangeMapCoeff.b; 
      fcsModel_DW.rcOutCmds.vzStick_mps = ((0.0033333333333333335 *
        static_cast<real_T>(tCmd) - 4.333333333333333) + 1.0) * 0.5 * -1.5;
    }
  } else {
    // 'interpretRcInputs_function:156' else
    // 'interpretRcInputs_function:157' vzCmd_unitRange = rcParamsStruct.pwmToCmdThrottleSlopeHigh*tCmd + ...; 
    // 'interpretRcInputs_function:158'         rcParamsStruct.pwmToCmdThrottleIncptHigh; 
    // ;
    // 'interpretRcInputs_function:159' rcOutCmds.vzStick_mps = (rcParamsStruct.vzHighRangeMapCoeff.a*vzCmd_unitRange + ... 
    // 'interpretRcInputs_function:160'         rcParamsStruct.vzHighRangeMapCoeff.c)/ ... 
    // 'interpretRcInputs_function:161'         rcParamsStruct.vzHighRangeMapCoeff.b; 
    fcsModel_DW.rcOutCmds.vzStick_mps = (0.0022222222222222222 *
      static_cast<real_T>(tCmd) - 3.2222222222222223) * -1.5;
  }

  //  Set roll, pitch and yaw stick
  // 'interpretRcInputs_function:165' rCmd = min( rcParamsStruct.pwmLimits(2), ... 
  // 'interpretRcInputs_function:166'     max( rcParamsStruct.pwmLimits(1), double(rcInCmds.joystickXCmd_nd) ) ); 
  rCmd = static_cast<int32_T>(std::fmin(2000.0, std::fmax(1000.0,
    static_cast<real_T>(ExtUPointer_ref_U->rcCmdsIn.joystickXCmd_nd))));

  //  Use roll stick to set FEP Vy to be used for POS control mode
  // 'interpretRcInputs_function:169' if ((rCmd <= rcParamsStruct.pwmRollStickMidHigh) && ... 
  // 'interpretRcInputs_function:170'         rCmd >= rcParamsStruct.pwmRollStickMidLow) 
  if ((rCmd <= 1650) && (rCmd >= 1350)) {
    // 'interpretRcInputs_function:171' vyCmd_unitRange  = 0;
    ylim = 0.0;
  } else {
    // 'interpretRcInputs_function:172' else
    // 'interpretRcInputs_function:173' vyCmd_unitRange = -3 + rCmd/500;
    ylim = static_cast<real_T>(rCmd) / 500.0 - 3.0;
  }

  // 'interpretRcInputs_function:175' rCmd_unitRange = -3 + rCmd/500;
  // 'interpretRcInputs_function:178' pCmd = min( rcParamsStruct.pwmLimits(2), ... 
  // 'interpretRcInputs_function:179'     max( rcParamsStruct.pwmLimits(1),  double(rcInCmds.joystickYCmd_nd) ) ); 
  pCmd = static_cast<int32_T>(std::fmin(2000.0, std::fmax(1000.0,
    static_cast<real_T>(ExtUPointer_ref_U->rcCmdsIn.joystickYCmd_nd))));

  //  Use pitch stick to set FEP Vx to be used for POS control mode
  // 'interpretRcInputs_function:182' if ((pCmd <= rcParamsStruct.pwmPitchStickMidHigh) && ... 
  // 'interpretRcInputs_function:183'         pCmd >= rcParamsStruct.pwmPitchStickMidLow) 
  if ((pCmd <= 1650) && (pCmd >= 1350)) {
    // 'interpretRcInputs_function:184' vxCmd_unitRange  = 0;
    vxCmd_unitRange = 0.0;
  } else {
    // 'interpretRcInputs_function:185' else
    // 'interpretRcInputs_function:186' vxCmd_unitRange = -3 + pCmd/500;
    vxCmd_unitRange = static_cast<real_T>(pCmd) / 500.0 - 3.0;
  }

  //  Reverse the pitch cmd
  // 'interpretRcInputs_function:190' pCmd_unitRange = -(-3 + pCmd/500);
  // 'interpretRcInputs_function:193' yCmd = min( rcParamsStruct.pwmLimits(2), ... 
  // 'interpretRcInputs_function:194'     max( rcParamsStruct.pwmLimits(1), double(rcInCmds.joystickZCmd_nd) ) ); 
  yCmd = std::fmin(2000.0, std::fmax(1000.0, static_cast<real_T>
    (ExtUPointer_ref_U->rcCmdsIn.joystickZCmd_nd)));

  //  Use yaw stick to also pick a Yaw angle in ALT or POS control mode
  // 'interpretRcInputs_function:197' if (flightMode == enumFlightMode.STABILIZE) 
  if (flightMode == enumFlightMode::STABILIZE) {
    // 'interpretRcInputs_function:198' yCmd_unitRange = -3 + yCmd/500;
    yCmd = yCmd / 500.0 - 3.0;

    // 'interpretRcInputs_function:199' else
    // 'interpretRcInputs_function:200' if ((yCmd <= rcParamsStruct.pwmYawStickMidHigh) && ... 
    // 'interpretRcInputs_function:201'             yCmd >= rcParamsStruct.pwmYawStickMidLow) 
  } else if ((yCmd <= 1600.0) && (yCmd >= 1400.0)) {
    // 'interpretRcInputs_function:202' yCmd_unitRange  = 0;
    yCmd = 0.0;
  } else {
    // 'interpretRcInputs_function:203' else
    // 'interpretRcInputs_function:204' yCmd_unitRange = -3 + yCmd/500;
    yCmd = yCmd / 500.0 - 3.0;
  }

  //  Usually expo is set in the Tx hence simply use a linear map here
  // 'interpretRcInputs_function:209' rcOutCmds.throttleStick = (rcParamsStruct.fCmdRangeMapCoeff.a*tCmd_unitRange + ... 
  // 'interpretRcInputs_function:210'     rcParamsStruct.fCmdRangeMapCoeff.c)/ ... 
  // 'interpretRcInputs_function:211'     rcParamsStruct.fCmdRangeMapCoeff.b;
  fcsModel_DW.rcOutCmds.throttleStick = (static_cast<real_T>(tCmd) - 1000.0) /
    900.0 * -20.0 - 15.0;

  // 'interpretRcInputs_function:212' rcOutCmds.rollStick = rCmd_unitRange*rlim; 
  fcsModel_DW.rcOutCmds.rollStick = (static_cast<real_T>(rCmd) / 500.0 - 3.0) *
    rlim;

  // 'interpretRcInputs_function:213' rcOutCmds.pitchStick = pCmd_unitRange*plim; 
  fcsModel_DW.rcOutCmds.pitchStick = -(static_cast<real_T>(pCmd) / 500.0 - 3.0) *
    plim;

  // 'interpretRcInputs_function:214' rcOutCmds.yawStick = yCmd_unitRange*ylim;
  fcsModel_DW.rcOutCmds.yawStick = yCmd * 1.0471975511965976;

  // 'interpretRcInputs_function:215' rcOutCmds.vxStick_mps = vxCmd_unitRange*vxlim; 
  fcsModel_DW.rcOutCmds.vxStick_mps = vxCmd_unitRange * 5.0;

  // 'interpretRcInputs_function:216' rcOutCmds.vyStick_mps = vyCmd_unitRange*vylim; 
  fcsModel_DW.rcOutCmds.vyStick_mps = ylim * 5.0;
  if (tmp) {
    // SignalConversion generated from: '<S114>/Vector Concatenate1'
    rtb_VectorConcatenate1[0] = fcsModel_DW.rcOutCmds.vxStick_mps;

    // SignalConversion generated from: '<S114>/Vector Concatenate1'
    rtb_VectorConcatenate1[1] = fcsModel_DW.rcOutCmds.vyStick_mps;

    // SignalConversion generated from: '<S114>/Vector Concatenate1'
    rtb_VectorConcatenate1[2] = 0.0;

    // Product: '<S114>/Matrix Multiply' incorporates:
    //   Math: '<S114>/Transpose'

    rtb_Product_i = 0.0;
    az = 0.0;
    tCmd = 0;
    for (rCmd = 0; rCmd < 3; rCmd++) {
      vxCmd_unitRange = rtb_VectorConcatenate1[rCmd];
      rtb_Product_i += rtb_Transpose[tCmd] * vxCmd_unitRange;
      az += rtb_Transpose[tCmd + 1] * vxCmd_unitRange;
      tCmd += 3;
    }

    // RelationalOperator: '<S123>/Compare' incorporates:
    //   Constant: '<S123>/Constant'

    rtb_Compare = (state > enumStateMachine::MTR_ARMED);

    // Outputs for Atomic SubSystem: '<S114>/holdOutputAtCenter1'
    // Inport: '<Root>/stateEstimate' incorporates:
    //   Product: '<S114>/Matrix Multiply'

    fcsModel_holdOutputAtCenter1(ExtUPointer_ref_U->stateEstimate.nedPos_m[0],
      rtb_Product_i, rtb_Compare, &ylim, &rtb_AND_p,
      &fcsModel_DW.holdOutputAtCenter1);

    // End of Outputs for SubSystem: '<S114>/holdOutputAtCenter1'

    // Concatenate: '<S114>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[0], 0, sizeof(busCtrlInputs));

    // Switch: '<S114>/Switch1' incorporates:
    //   RelationalOperator: '<S118>/Compare'

    if (rtb_AND_p) {
      // BusAssignment: '<S114>/Bus Assignment1' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'
      //   Constant: '<S114>/Constant1'

      rtb_VectorConcatenate[0].feedForwardCmd = 0.0;
    } else {
      // BusAssignment: '<S114>/Bus Assignment1' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'
      //   Product: '<S114>/Matrix Multiply'

      rtb_VectorConcatenate[0].feedForwardCmd = rtb_Product_i;
    }

    // End of Switch: '<S114>/Switch1'

    // BusAssignment: '<S114>/Bus Assignment1' incorporates:
    //   Concatenate: '<S114>/Vector Concatenate'
    //   Inport: '<Root>/stateEstimate'

    rtb_VectorConcatenate[0].cmd = ylim;
    rtb_VectorConcatenate[0].meas = ExtUPointer_ref_U->stateEstimate.nedPos_m[0];

    // Logic: '<S114>/Logical Operator2' incorporates:
    //   Constant: '<S121>/Constant'
    //   Logic: '<S108>/Logical Operator2'
    //   Logic: '<S114>/Logical Operator3'
    //   MATLAB Function: '<S4>/Interpret RC In Cmds'
    //   RelationalOperator: '<S121>/Compare'

    rtb_Compare_od = (resetIntegrator || (flightMode != enumFlightMode::
      POS_CONTROL));

    // BusAssignment: '<S114>/Bus Assignment1' incorporates:
    //   Concatenate: '<S114>/Vector Concatenate'
    //   Logic: '<S114>/Logical Operator2'

    rtb_VectorConcatenate[0].integratorReset = rtb_Compare_od;

    // Outputs for Atomic SubSystem: '<S114>/holdOutputAtCenter2'
    // Inport: '<Root>/stateEstimate' incorporates:
    //   Product: '<S114>/Matrix Multiply'

    fcsModel_holdOutputAtCenter1(ExtUPointer_ref_U->stateEstimate.nedPos_m[1],
      az, rtb_Compare, &ylim, &rtb_AND_p, &fcsModel_DW.holdOutputAtCenter2);

    // End of Outputs for SubSystem: '<S114>/holdOutputAtCenter2'

    // Concatenate: '<S114>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[1], 0, sizeof(busCtrlInputs));

    // Switch: '<S114>/Switch2' incorporates:
    //   RelationalOperator: '<S119>/Compare'

    if (rtb_AND_p) {
      // BusAssignment: '<S114>/Bus Assignment2' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'
      //   Constant: '<S114>/Constant3'

      rtb_VectorConcatenate[1].feedForwardCmd = 0.0;
    } else {
      // BusAssignment: '<S114>/Bus Assignment2' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'
      //   Product: '<S114>/Matrix Multiply'

      rtb_VectorConcatenate[1].feedForwardCmd = az;
    }

    // End of Switch: '<S114>/Switch2'

    // BusAssignment: '<S114>/Bus Assignment2' incorporates:
    //   Concatenate: '<S114>/Vector Concatenate'
    //   Inport: '<Root>/stateEstimate'

    rtb_VectorConcatenate[1].cmd = ylim;
    rtb_VectorConcatenate[1].meas = ExtUPointer_ref_U->stateEstimate.nedPos_m[1];
    rtb_VectorConcatenate[1].integratorReset = rtb_Compare_od;

    // Outputs for Atomic SubSystem: '<S114>/holdOutputAtCenter'
    // MATLAB Function: '<S124>/holdOutputAtCenter' incorporates:
    //   Inport: '<Root>/stateEstimate'

    // MATLAB Function 'holdOutputAtCenter/holdOutputAtCenter': '<S127>:1'
    // '<S127>:1:2' [output, atCenter] = holdOutputAtCenter_function(input, trigger, auxTrigger, params); 
    // HOLDOUTPUTATCENTER_FUNCTION holds the output constant at last input if the 
    // trigger value is within user defined delta from the center
    //
    // auxTrigger is an additional trigger that is used to indicate when to
    // actually start holding the output
    // 'holdOutputAtCenter_function:8' if isempty(last_input)
    // 'holdOutputAtCenter_function:12' if(trigger <= (params.center + params.posDeltaFromCenter) && ... 
    // 'holdOutputAtCenter_function:13'         trigger >=(params.center - params.negDeltaFromCenter)) && auxTrigger 
    if ((fcsModel_DW.rcOutCmds.vzStick_mps > 0.05) ||
        (fcsModel_DW.rcOutCmds.vzStick_mps < -0.05) || (!rtb_Compare)) {
      // 'holdOutputAtCenter_function:15' else
      // 'holdOutputAtCenter_function:16' atCenter = false;
      rtb_Compare = false;

      // 'holdOutputAtCenter_function:17' last_input = input;
      fcsModel_DW.last_input_l = ExtUPointer_ref_U->stateEstimate.aglEst_m;
    } else {
      // 'holdOutputAtCenter_function:14' atCenter = true;
    }

    // End of Outputs for SubSystem: '<S114>/holdOutputAtCenter'

    // Gain: '<S114>/Gain1' incorporates:
    //   Inport: '<Root>/stateEstimate'

    // 'holdOutputAtCenter_function:20' output = last_input;
    rtb_Product_i = -ExtUPointer_ref_U->stateEstimate.aglEst_m;

    // Concatenate: '<S114>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[2], 0, sizeof(busCtrlInputs));

    // Switch: '<S114>/Switch' incorporates:
    //   RelationalOperator: '<S116>/Compare'

    if (rtb_Compare) {
      // BusAssignment: '<S114>/Bus Assignment3' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'
      //   Constant: '<S114>/Constant5'

      rtb_VectorConcatenate[2].feedForwardCmd = 0.0;
    } else {
      // BusAssignment: '<S114>/Bus Assignment3' incorporates:
      //   Concatenate: '<S114>/Vector Concatenate'

      rtb_VectorConcatenate[2].feedForwardCmd =
        fcsModel_DW.rcOutCmds.vzStick_mps;
    }

    // End of Switch: '<S114>/Switch'

    // Outputs for Atomic SubSystem: '<S114>/holdOutputAtCenter'
    // BusAssignment: '<S114>/Bus Assignment3' incorporates:
    //   Concatenate: '<S114>/Vector Concatenate'
    //   Gain: '<S114>/Gain'
    //   Gain: '<S114>/Gain1'
    //   Inport: '<Root>/stateEstimate'
    //   MATLAB Function: '<S124>/holdOutputAtCenter'

    rtb_VectorConcatenate[2].cmd = -fcsModel_DW.last_input_l;

    // End of Outputs for SubSystem: '<S114>/holdOutputAtCenter'
    rtb_VectorConcatenate[2].meas = -ExtUPointer_ref_U->stateEstimate.aglEst_m;

    // Logic: '<S114>/Logical Operator' incorporates:
    //   Constant: '<S117>/Constant'
    //   Constant: '<S120>/Constant'
    //   Logic: '<S108>/Logical Operator'
    //   Logic: '<S114>/Logical Operator1'
    //   Logic: '<S168>/Logical Operator'
    //   MATLAB Function: '<S4>/Interpret RC In Cmds'
    //   RelationalOperator: '<S117>/Compare'
    //   RelationalOperator: '<S120>/Compare'

    tmp_0 = (resetIntegrator || ((flightMode != enumFlightMode::ALT_CONTROL) &&
              (flightMode != enumFlightMode::POS_CONTROL)));

    // BusAssignment: '<S114>/Bus Assignment3' incorporates:
    //   Concatenate: '<S114>/Vector Concatenate'
    //   Logic: '<S114>/Logical Operator'

    rtb_VectorConcatenate[2].integratorReset = tmp_0;

    // Outputs for Iterator SubSystem: '<S110>/NED Position Control' incorporates:
    //   ForEach: '<S115>/For Each'

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block'
    // ForEachSliceSelector generated from: '<S115>/ctrlInputs' incorporates:
    //   BusAssignment: '<S114>/Bus Assignment'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S115>/Unit Delay'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[0].cmd,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.cmdSignalConditioningParamsArray
      [0], &ylim, 0.008, &fcsModel_DW.CoreSubsys_g[0].SignalConditioningBlock);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[0].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.measSignalConditioningParamsArray
      [0], &rlim, 0.008, &fcsModel_DW.CoreSubsys_g[0].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S115>/pidWithDebug'
    fcsModel_pidWithDebug_j(rtb_VectorConcatenate[0].feedForwardCmd, ylim, rlim,
      rtb_VectorConcatenate[0].integratorReset, 0.0,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray
      [0], fcsModel_DW.CoreSubsys_g[0].UnitDelay_DSTATE, &rtb_Product_i,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_g[0].pidWithDebug);

    // End of Outputs for SubSystem: '<S115>/pidWithDebug'

    // Update for UnitDelay: '<S115>/Unit Delay'
    fcsModel_DW.CoreSubsys_g[0].UnitDelay_DSTATE = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDeb_m[0] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S115>/neVelCmd_mps'
    rtb_ImpAsg_InsertedFor_neVelCmd[0] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/meas'
    rtb_ImpAsg_InsertedFor_meas_at_[0] = rlim;

    // ForEachSliceAssignment generated from: '<S115>/cmd'
    rtb_ImpAsg_InsertedFor_cmd_at_i[0] = ylim;

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block'
    // ForEachSliceSelector generated from: '<S115>/ctrlInputs' incorporates:
    //   BusAssignment: '<S114>/Bus Assignment'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S115>/Unit Delay'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[1].cmd,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.cmdSignalConditioningParamsArray
      [1], &ylim, 0.008, &fcsModel_DW.CoreSubsys_g[1].SignalConditioningBlock);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[1].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.measSignalConditioningParamsArray
      [1], &rlim, 0.008, &fcsModel_DW.CoreSubsys_g[1].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S115>/pidWithDebug'
    fcsModel_pidWithDebug_j(rtb_VectorConcatenate[1].feedForwardCmd, ylim, rlim,
      rtb_VectorConcatenate[1].integratorReset, 0.0,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray
      [1], fcsModel_DW.CoreSubsys_g[1].UnitDelay_DSTATE, &rtb_Product_i,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_g[1].pidWithDebug);

    // End of Outputs for SubSystem: '<S115>/pidWithDebug'

    // Update for UnitDelay: '<S115>/Unit Delay'
    fcsModel_DW.CoreSubsys_g[1].UnitDelay_DSTATE = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDeb_m[1] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S115>/neVelCmd_mps'
    rtb_ImpAsg_InsertedFor_neVelCmd[1] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/meas'
    rtb_ImpAsg_InsertedFor_meas_at_[1] = rlim;

    // ForEachSliceAssignment generated from: '<S115>/cmd'
    rtb_ImpAsg_InsertedFor_cmd_at_i[1] = ylim;

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block'
    // ForEachSliceSelector generated from: '<S115>/ctrlInputs' incorporates:
    //   BusAssignment: '<S114>/Bus Assignment'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S115>/Unit Delay'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[2].cmd,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.cmdSignalConditioningParamsArray
      [2], &ylim, 0.008, &fcsModel_DW.CoreSubsys_g[2].SignalConditioningBlock);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[2].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.measSignalConditioningParamsArray
      [2], &rlim, 0.008, &fcsModel_DW.CoreSubsys_g[2].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S115>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S115>/pidWithDebug'
    fcsModel_pidWithDebug_j(rtb_VectorConcatenate[2].feedForwardCmd, ylim, rlim,
      rtb_VectorConcatenate[2].integratorReset, 0.0,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.posCtrlParams.ctrlParamsArray
      [2], fcsModel_DW.CoreSubsys_g[2].UnitDelay_DSTATE, &rtb_Product_i,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_g[2].pidWithDebug);

    // End of Outputs for SubSystem: '<S115>/pidWithDebug'

    // Update for UnitDelay: '<S115>/Unit Delay'
    fcsModel_DW.CoreSubsys_g[2].UnitDelay_DSTATE = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDeb_m[2] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S115>/neVelCmd_mps'
    rtb_ImpAsg_InsertedFor_neVelCmd[2] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S115>/meas'
    rtb_ImpAsg_InsertedFor_meas_at_[2] = rlim;

    // ForEachSliceAssignment generated from: '<S115>/cmd'
    rtb_ImpAsg_InsertedFor_cmd_at_i[2] = ylim;

    // End of Outputs for SubSystem: '<S110>/NED Position Control'

    // Concatenate: '<S108>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[0], 0, sizeof(busCtrlInputs));

    // BusAssignment: '<S108>/Bus Assignment' incorporates:
    //   Concatenate: '<S108>/Vector Concatenate'
    //   Inport: '<Root>/stateEstimate'

    rtb_VectorConcatenate[0].cmd = rtb_ImpAsg_InsertedFor_neVelCmd[0];
    rtb_VectorConcatenate[0].meas = ExtUPointer_ref_U->stateEstimate.nedVel_mps
      [0];
    rtb_VectorConcatenate[0].integratorReset = rtb_Compare_od;

    // Concatenate: '<S108>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[1], 0, sizeof(busCtrlInputs));

    // BusAssignment: '<S108>/Bus Assignment1' incorporates:
    //   Concatenate: '<S108>/Vector Concatenate'
    //   Inport: '<Root>/stateEstimate'

    rtb_VectorConcatenate[1].cmd = rtb_ImpAsg_InsertedFor_neVelCmd[1];
    rtb_VectorConcatenate[1].meas = ExtUPointer_ref_U->stateEstimate.nedVel_mps
      [1];
    rtb_VectorConcatenate[1].integratorReset = rtb_Compare_od;

    // Concatenate: '<S108>/Vector Concatenate'
    std::memset(&rtb_VectorConcatenate[2], 0, sizeof(busCtrlInputs));

    // BusAssignment: '<S108>/Bus Assignment2' incorporates:
    //   Concatenate: '<S108>/Vector Concatenate'
    //   Gain: '<S108>/Gain'
    //   Inport: '<Root>/stateEstimate'

    rtb_VectorConcatenate[2].cmd = rtb_ImpAsg_InsertedFor_neVelCmd[2];
    rtb_VectorConcatenate[2].meas =
      -ExtUPointer_ref_U->stateEstimate.climbRateEst_mps;
    rtb_VectorConcatenate[2].integratorReset = tmp_0;

    // Product: '<S111>/Matrix Multiply' incorporates:
    //   Inport: '<Root>/stateEstimate'

    rtb_Product_i = ExtUPointer_ref_U->stateEstimate.bodyAccels_mps2[1];
    az = ExtUPointer_ref_U->stateEstimate.bodyAccels_mps2[0];
    fE = ExtUPointer_ref_U->stateEstimate.bodyAccels_mps2[2];

    // Sum: '<S111>/Sum' incorporates:
    //   Constant: '<S111>/Constant'
    //   Math: '<S111>/Transpose'
    //   Product: '<S111>/Matrix Multiply'

    for (tCmd = 0; tCmd < 3; tCmd++) {
      rtb_VectorConcatenate1[tCmd] = ((rtb_Transpose_h[tCmd + 3] * rtb_Product_i
        + rtb_Transpose_h[tCmd] * az) + rtb_Transpose_h[tCmd + 6] * fE) +
        fcsModel_ConstP.Constant_Value_h[tCmd];
    }

    // End of Sum: '<S111>/Sum'

    // SignalConversion generated from: '<S111>/For Each Subsystem'
    rtb_MatrixMultiply[0] = 0.0;
    rtb_MatrixMultiply[1] = 0.0;
    rtb_MatrixMultiply[2] = 0.0;

    // Outputs for Iterator SubSystem: '<S111>/For Each Subsystem' incorporates:
    //   ForEach: '<S169>/For Each'

    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'

    plim = rtb_VectorConcatenate[0].cmd;

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block'
    // MATLAB Function: '<S193>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [0].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [0].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S193>/Compute Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeNumeratorAndDenominator(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[0].rateNum[0],
      &rtb_ImpAsg_InsertedFor_neVelCmd[0], &fcsModel_DW.CoreSubsys_i[0].den_e[0],
      0.008);

    // DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_tmp = (plim -
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states[0] *
      fcsModel_DW.CoreSubsys_i[0].den_e[1]) - fcsModel_DW.CoreSubsys_i[0].
      DiscreteTransferFcn_states[1] * fcsModel_DW.CoreSubsys_i[0].den_e[2];
    az = (fcsModel_DW.CoreSubsys_i[0].rateNum[0] * fcsModel_DW.CoreSubsys_i[0].
          DiscreteTransferFcn_tmp + fcsModel_DW.CoreSubsys_i[0].
          DiscreteTransferFcn_states[0] * fcsModel_DW.CoreSubsys_i[0].rateNum[1])
      + fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states[1] *
      fcsModel_DW.CoreSubsys_i[0].rateNum[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    rtb_Product_i =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filteredInputRateLimits[1];

    // Switch: '<S199>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    if (az <= rtb_Product_i) {
      rtb_Product_i =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [0].filteredInputRateLimits[0];

      // Switch: '<S199>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S199>/UpperRelop'

      if (az >= rtb_Product_i) {
        rtb_Product_i = az;
      }
    }

    // Delay: '<S196>/Delay'
    if (fcsModel_DW.CoreSubsys_i[0].icLoad) {
      fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE = rtb_Product_i;
    }

    // Sum: '<S196>/Difference Inputs1' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs1':
    //
    //   Add in CPU

    rtb_Product_i -= fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE;

    // Product: '<S196>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S196>/sample time'
    //
    //  About '<S196>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filteredInputAccelLimits[1] * 0.008;

    // Switch: '<S206>/Switch2' incorporates:
    //   RelationalOperator: '<S206>/LowerRelop1'

    if (rtb_Product_i <= rlim) {
      // Product: '<S196>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S196>/sample time'
      //
      //  About '<S196>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [0].filteredInputAccelLimits[0];

      // Switch: '<S206>/Switch' incorporates:
      //   RelationalOperator: '<S206>/UpperRelop'

      if (rtb_Product_i >= rlim) {
        rlim = rtb_Product_i;
      }
    }

    // Sum: '<S196>/Difference Inputs2' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs2':
    //
    //   Add in CPU

    rtb_Product_i = rlim + fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE;

    // MATLAB Function: '<S194>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [0].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [0].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeFilterNumeratorAndDenomi(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[0].num[0],
      &fcsModel_DW.CoreSubsys_i[0].den[0], 0.008);

    // DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_tmp_l = (plim -
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states_h[0] *
      fcsModel_DW.CoreSubsys_i[0].den[1]) - fcsModel_DW.CoreSubsys_i[0].
      DiscreteTransferFcn_states_h[1] * fcsModel_DW.CoreSubsys_i[0].den[2];
    fE = (fcsModel_DW.CoreSubsys_i[0].num[0] * fcsModel_DW.CoreSubsys_i[0].
          DiscreteTransferFcn_tmp_l + fcsModel_DW.CoreSubsys_i[0].
          DiscreteTransferFcn_states_h[0] * fcsModel_DW.CoreSubsys_i[0].num[1])
      + fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states_h[1] *
      fcsModel_DW.CoreSubsys_i[0].num[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filteredInputLimits[1];

    // Switch: '<S198>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    if (fE <= rlim) {
      rlim =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [0].filteredInputLimits[0];

      // Switch: '<S198>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S198>/UpperRelop'

      if (fE >= rlim) {
        rlim = fE;
      }
    }

    // Delay: '<S195>/Delay'
    if (fcsModel_DW.CoreSubsys_i[0].icLoad_a) {
      fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE_m = rlim;
    }

    // Sum: '<S195>/Difference Inputs1' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs1':
    //
    //   Add in CPU

    plim = rlim - fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE_m;

    // Product: '<S195>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S195>/sample time'
    //
    //  About '<S195>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [0].filteredInputRateLimits[1] * 0.008;

    // Switch: '<S205>/Switch2' incorporates:
    //   RelationalOperator: '<S205>/LowerRelop1'

    if (plim <= rlim) {
      // Product: '<S195>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S195>/sample time'
      //
      //  About '<S195>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [0].filteredInputRateLimits[0];

      // Switch: '<S205>/Switch' incorporates:
      //   RelationalOperator: '<S205>/UpperRelop'

      if (plim >= rlim) {
        rlim = plim;
      }
    }

    // Sum: '<S195>/Difference Inputs2' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs2':
    //
    //   Add in CPU

    plim = rlim + fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE_m;

    // Update for DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states[1] =
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states[0];
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states[0] =
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_tmp;

    // Update for Delay: '<S196>/Delay'
    fcsModel_DW.CoreSubsys_i[0].icLoad = false;
    fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE = rtb_Product_i;

    // Update for DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states_h[1] =
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states_h[0];
    fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_states_h[0] =
      fcsModel_DW.CoreSubsys_i[0].DiscreteTransferFcn_tmp_l;

    // Update for Delay: '<S195>/Delay'
    fcsModel_DW.CoreSubsys_i[0].icLoad_a = false;
    fcsModel_DW.CoreSubsys_i[0].Delay_DSTATE_m = plim;

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[0].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.measSignalConditioningParamsArray
      [0], &ylim, 0.008, &fcsModel_DW.CoreSubsys_i[0].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
    // ForEachSliceSelector generated from: '<S169>/nedAccel_mps2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   Sum: '<S111>/Sum'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate1[0],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelSignalConditioningParamsArray
      [0], &rlim, 0.008, &fcsModel_DW.CoreSubsys_i[0].SignalConditioningBlock2);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block2'

    // Sum: '<S169>/Sum' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/ffGain'
    //   Product: '<S169>/Product1'
    //   Product: '<S169>/Product2'
    //   Product: '<S169>/Product3'

    rtb_Product_i =
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ffGainsArray
       [0] * rtb_Product_i -
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray
       [0] * rlim) +
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ff2GainsArray
      [0] * plim;

    // Outputs for Atomic SubSystem: '<S169>/pidWithDebug'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S169>/Unit Delay'

    fcsModel_pidWithDebug_j(rtb_Product_i, plim, ylim, rtb_VectorConcatenate[0].
      integratorReset, rtb_MatrixMultiply[0],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray
      [0], fcsModel_DW.CoreSubsys_i[0].UnitDelay_DSTATE, &rlim,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_i[0].pidWithDebug);

    // End of Outputs for SubSystem: '<S169>/pidWithDebug'

    // Update for UnitDelay: '<S169>/Unit Delay'
    fcsModel_DW.CoreSubsys_i[0].UnitDelay_DSTATE = rlim;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlOut '
    rtb_ImpAsg_InsertedFor_velCtrlO[0] = rlim;

    // ForEachSliceAssignment generated from: '<S169>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDebug[0] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlFf'
    rtb_ImpAsg_InsertedFor_velCtrlF[0] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S169>/filtMeas'
    rtb_ImpAsg_InsertedFor_filtMeas[0] = ylim;

    // ForEachSliceAssignment generated from: '<S169>/filtCmd'
    rtb_ImpAsg_InsertedFor_filtCmd_[0] = plim;

    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'

    plim = rtb_VectorConcatenate[1].cmd;

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block'
    // MATLAB Function: '<S193>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [1].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [1].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S193>/Compute Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeNumeratorAndDenominator(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[1].rateNum[0],
      &rtb_ImpAsg_InsertedFor_neVelCmd[0], &fcsModel_DW.CoreSubsys_i[1].den_e[0],
      0.008);

    // DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_tmp = (plim -
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states[0] *
      fcsModel_DW.CoreSubsys_i[1].den_e[1]) - fcsModel_DW.CoreSubsys_i[1].
      DiscreteTransferFcn_states[1] * fcsModel_DW.CoreSubsys_i[1].den_e[2];
    az = (fcsModel_DW.CoreSubsys_i[1].rateNum[0] * fcsModel_DW.CoreSubsys_i[1].
          DiscreteTransferFcn_tmp + fcsModel_DW.CoreSubsys_i[1].
          DiscreteTransferFcn_states[0] * fcsModel_DW.CoreSubsys_i[1].rateNum[1])
      + fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states[1] *
      fcsModel_DW.CoreSubsys_i[1].rateNum[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    rtb_Product_i =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filteredInputRateLimits[1];

    // Switch: '<S199>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    if (az <= rtb_Product_i) {
      rtb_Product_i =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [1].filteredInputRateLimits[0];

      // Switch: '<S199>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S199>/UpperRelop'

      if (az >= rtb_Product_i) {
        rtb_Product_i = az;
      }
    }

    // Delay: '<S196>/Delay'
    if (fcsModel_DW.CoreSubsys_i[1].icLoad) {
      fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE = rtb_Product_i;
    }

    // Sum: '<S196>/Difference Inputs1' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs1':
    //
    //   Add in CPU

    rtb_Product_i -= fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE;

    // Product: '<S196>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S196>/sample time'
    //
    //  About '<S196>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filteredInputAccelLimits[1] * 0.008;

    // Switch: '<S206>/Switch2' incorporates:
    //   RelationalOperator: '<S206>/LowerRelop1'

    if (rtb_Product_i <= rlim) {
      // Product: '<S196>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S196>/sample time'
      //
      //  About '<S196>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [1].filteredInputAccelLimits[0];

      // Switch: '<S206>/Switch' incorporates:
      //   RelationalOperator: '<S206>/UpperRelop'

      if (rtb_Product_i >= rlim) {
        rlim = rtb_Product_i;
      }
    }

    // Sum: '<S196>/Difference Inputs2' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs2':
    //
    //   Add in CPU

    rtb_Product_i = rlim + fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE;

    // MATLAB Function: '<S194>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [1].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [1].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeFilterNumeratorAndDenomi(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[1].num[0],
      &fcsModel_DW.CoreSubsys_i[1].den[0], 0.008);

    // DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_tmp_l = (plim -
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states_h[0] *
      fcsModel_DW.CoreSubsys_i[1].den[1]) - fcsModel_DW.CoreSubsys_i[1].
      DiscreteTransferFcn_states_h[1] * fcsModel_DW.CoreSubsys_i[1].den[2];
    fE = (fcsModel_DW.CoreSubsys_i[1].num[0] * fcsModel_DW.CoreSubsys_i[1].
          DiscreteTransferFcn_tmp_l + fcsModel_DW.CoreSubsys_i[1].
          DiscreteTransferFcn_states_h[0] * fcsModel_DW.CoreSubsys_i[1].num[1])
      + fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states_h[1] *
      fcsModel_DW.CoreSubsys_i[1].num[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filteredInputLimits[1];

    // Switch: '<S198>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    if (fE <= rlim) {
      rlim =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [1].filteredInputLimits[0];

      // Switch: '<S198>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S198>/UpperRelop'

      if (fE >= rlim) {
        rlim = fE;
      }
    }

    // Delay: '<S195>/Delay'
    if (fcsModel_DW.CoreSubsys_i[1].icLoad_a) {
      fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE_m = rlim;
    }

    // Sum: '<S195>/Difference Inputs1' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs1':
    //
    //   Add in CPU

    plim = rlim - fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE_m;

    // Product: '<S195>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S195>/sample time'
    //
    //  About '<S195>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [1].filteredInputRateLimits[1] * 0.008;

    // Switch: '<S205>/Switch2' incorporates:
    //   RelationalOperator: '<S205>/LowerRelop1'

    if (plim <= rlim) {
      // Product: '<S195>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S195>/sample time'
      //
      //  About '<S195>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [1].filteredInputRateLimits[0];

      // Switch: '<S205>/Switch' incorporates:
      //   RelationalOperator: '<S205>/UpperRelop'

      if (plim >= rlim) {
        rlim = plim;
      }
    }

    // Sum: '<S195>/Difference Inputs2' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs2':
    //
    //   Add in CPU

    plim = rlim + fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE_m;

    // Update for DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states[1] =
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states[0];
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states[0] =
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_tmp;

    // Update for Delay: '<S196>/Delay'
    fcsModel_DW.CoreSubsys_i[1].icLoad = false;
    fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE = rtb_Product_i;

    // Update for DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states_h[1] =
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states_h[0];
    fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_states_h[0] =
      fcsModel_DW.CoreSubsys_i[1].DiscreteTransferFcn_tmp_l;

    // Update for Delay: '<S195>/Delay'
    fcsModel_DW.CoreSubsys_i[1].icLoad_a = false;
    fcsModel_DW.CoreSubsys_i[1].Delay_DSTATE_m = plim;

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[1].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.measSignalConditioningParamsArray
      [1], &ylim, 0.008, &fcsModel_DW.CoreSubsys_i[1].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
    // ForEachSliceSelector generated from: '<S169>/nedAccel_mps2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   Sum: '<S111>/Sum'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate1[1],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelSignalConditioningParamsArray
      [1], &rlim, 0.008, &fcsModel_DW.CoreSubsys_i[1].SignalConditioningBlock2);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block2'

    // Sum: '<S169>/Sum' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/ffGain'
    //   Product: '<S169>/Product1'
    //   Product: '<S169>/Product2'
    //   Product: '<S169>/Product3'

    rtb_Product_i =
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ffGainsArray
       [1] * rtb_Product_i -
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray
       [1] * rlim) +
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ff2GainsArray
      [1] * plim;

    // Outputs for Atomic SubSystem: '<S169>/pidWithDebug'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S169>/Unit Delay'

    fcsModel_pidWithDebug_j(rtb_Product_i, plim, ylim, rtb_VectorConcatenate[1].
      integratorReset, rtb_MatrixMultiply[1],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray
      [1], fcsModel_DW.CoreSubsys_i[1].UnitDelay_DSTATE, &rlim,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_i[1].pidWithDebug);

    // End of Outputs for SubSystem: '<S169>/pidWithDebug'

    // Update for UnitDelay: '<S169>/Unit Delay'
    fcsModel_DW.CoreSubsys_i[1].UnitDelay_DSTATE = rlim;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlOut '
    rtb_ImpAsg_InsertedFor_velCtrlO[1] = rlim;

    // ForEachSliceAssignment generated from: '<S169>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDebug[1] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlFf'
    rtb_ImpAsg_InsertedFor_velCtrlF[1] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S169>/filtMeas'
    rtb_ImpAsg_InsertedFor_filtMeas[1] = ylim;

    // ForEachSliceAssignment generated from: '<S169>/filtCmd'
    rtb_ImpAsg_InsertedFor_filtCmd_[1] = plim;

    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'

    plim = rtb_VectorConcatenate[2].cmd;

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block'
    // MATLAB Function: '<S193>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [2].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [2].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S193>/Compute Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeNumeratorAndDenominator(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[2].rateNum[0],
      &rtb_ImpAsg_InsertedFor_neVelCmd[0], &fcsModel_DW.CoreSubsys_i[2].den_e[0],
      0.008);

    // DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_tmp = (plim -
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states[0] *
      fcsModel_DW.CoreSubsys_i[2].den_e[1]) - fcsModel_DW.CoreSubsys_i[2].
      DiscreteTransferFcn_states[1] * fcsModel_DW.CoreSubsys_i[2].den_e[2];
    az = (fcsModel_DW.CoreSubsys_i[2].rateNum[0] * fcsModel_DW.CoreSubsys_i[2].
          DiscreteTransferFcn_tmp + fcsModel_DW.CoreSubsys_i[2].
          DiscreteTransferFcn_states[0] * fcsModel_DW.CoreSubsys_i[2].rateNum[1])
      + fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states[1] *
      fcsModel_DW.CoreSubsys_i[2].rateNum[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    rtb_Product_i =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filteredInputRateLimits[1];

    // Switch: '<S199>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S199>/LowerRelop1'

    if (az <= rtb_Product_i) {
      rtb_Product_i =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [2].filteredInputRateLimits[0];

      // Switch: '<S199>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S199>/UpperRelop'

      if (az >= rtb_Product_i) {
        rtb_Product_i = az;
      }
    }

    // Delay: '<S196>/Delay'
    if (fcsModel_DW.CoreSubsys_i[2].icLoad) {
      fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE = rtb_Product_i;
    }

    // Sum: '<S196>/Difference Inputs1' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs1':
    //
    //   Add in CPU

    rtb_Product_i -= fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE;

    // Product: '<S196>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S196>/sample time'
    //
    //  About '<S196>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filteredInputAccelLimits[1] * 0.008;

    // Switch: '<S206>/Switch2' incorporates:
    //   RelationalOperator: '<S206>/LowerRelop1'

    if (rtb_Product_i <= rlim) {
      // Product: '<S196>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S196>/sample time'
      //
      //  About '<S196>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [2].filteredInputAccelLimits[0];

      // Switch: '<S206>/Switch' incorporates:
      //   RelationalOperator: '<S206>/UpperRelop'

      if (rtb_Product_i >= rlim) {
        rlim = rtb_Product_i;
      }
    }

    // Sum: '<S196>/Difference Inputs2' incorporates:
    //   Delay: '<S196>/Delay'
    //
    //  Block description for '<S196>/Difference Inputs2':
    //
    //   Add in CPU

    rtb_Product_i = rlim + fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE;

    // MATLAB Function: '<S194>/Compute Natural Frequency' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    fcsMode_ComputeNaturalFrequency
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [2].filterParams.filterBandwidth_radps,
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
       [2].filterParams.dampingRatio_nd, &ylim);

    // MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'

    ComputeFilterNumeratorAndDenomi(ylim,
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filterParams.dampingRatio_nd, &fcsModel_DW.CoreSubsys_i[2].num[0],
      &fcsModel_DW.CoreSubsys_i[2].den[0], 0.008);

    // DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_tmp_l = (plim -
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states_h[0] *
      fcsModel_DW.CoreSubsys_i[2].den[1]) - fcsModel_DW.CoreSubsys_i[2].
      DiscreteTransferFcn_states_h[1] * fcsModel_DW.CoreSubsys_i[2].den[2];
    fE = (fcsModel_DW.CoreSubsys_i[2].num[0] * fcsModel_DW.CoreSubsys_i[2].
          DiscreteTransferFcn_tmp_l + fcsModel_DW.CoreSubsys_i[2].
          DiscreteTransferFcn_states_h[0] * fcsModel_DW.CoreSubsys_i[2].num[1])
      + fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states_h[1] *
      fcsModel_DW.CoreSubsys_i[2].num[2];

    // ForEachSliceSelector generated from: '<S169>/cmdCndParams' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filteredInputLimits[1];

    // Switch: '<S198>/Switch2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   RelationalOperator: '<S198>/LowerRelop1'

    if (fE <= rlim) {
      rlim =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [2].filteredInputLimits[0];

      // Switch: '<S198>/Switch' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   RelationalOperator: '<S198>/UpperRelop'

      if (fE >= rlim) {
        rlim = fE;
      }
    }

    // Delay: '<S195>/Delay'
    if (fcsModel_DW.CoreSubsys_i[2].icLoad_a) {
      fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE_m = rlim;
    }

    // Sum: '<S195>/Difference Inputs1' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs1':
    //
    //   Add in CPU

    plim = rlim - fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE_m;

    // Product: '<S195>/delta rise limit' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
    //   SampleTimeMath: '<S195>/sample time'
    //
    //  About '<S195>/sample time':
    //   y = K where K = ( w * Ts )
    //
    rlim =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
      [2].filteredInputRateLimits[1] * 0.008;

    // Switch: '<S205>/Switch2' incorporates:
    //   RelationalOperator: '<S205>/LowerRelop1'

    if (plim <= rlim) {
      // Product: '<S195>/delta fall limit' incorporates:
      //   Constant: '<Root>/fcsParams'
      //   ForEachSliceSelector generated from: '<S169>/cmdCndParams'
      //   SampleTimeMath: '<S195>/sample time'
      //
      //  About '<S195>/sample time':
      //   y = K where K = ( w * Ts )
      //
      rlim = 0.008 *
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.cmdSignalConditioningParamsArray
        [2].filteredInputRateLimits[0];

      // Switch: '<S205>/Switch' incorporates:
      //   RelationalOperator: '<S205>/UpperRelop'

      if (plim >= rlim) {
        rlim = plim;
      }
    }

    // Sum: '<S195>/Difference Inputs2' incorporates:
    //   Delay: '<S195>/Delay'
    //
    //  Block description for '<S195>/Difference Inputs2':
    //
    //   Add in CPU

    plim = rlim + fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE_m;

    // Update for DiscreteTransferFcn: '<S193>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states[1] =
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states[0];
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states[0] =
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_tmp;

    // Update for Delay: '<S196>/Delay'
    fcsModel_DW.CoreSubsys_i[2].icLoad = false;
    fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE = rtb_Product_i;

    // Update for DiscreteTransferFcn: '<S194>/Discrete Transfer Fcn'
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states_h[1] =
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states_h[0];
    fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_states_h[0] =
      fcsModel_DW.CoreSubsys_i[2].DiscreteTransferFcn_tmp_l;

    // Update for Delay: '<S195>/Delay'
    fcsModel_DW.CoreSubsys_i[2].icLoad_a = false;
    fcsModel_DW.CoreSubsys_i[2].Delay_DSTATE_m = plim;

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate[2].meas,
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.measSignalConditioningParamsArray
      [2], &ylim, 0.008, &fcsModel_DW.CoreSubsys_i[2].SignalConditioningBlock1);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block1'

    // Outputs for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
    // ForEachSliceSelector generated from: '<S169>/nedAccel_mps2' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   Sum: '<S111>/Sum'

    fcsM_SignalConditioningBlock1_f(rtb_VectorConcatenate1[2],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelSignalConditioningParamsArray
      [2], &rlim, 0.008, &fcsModel_DW.CoreSubsys_i[2].SignalConditioningBlock2);

    // End of Outputs for SubSystem: '<S169>/Signal Conditioning Block2'

    // Sum: '<S169>/Sum' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   ForEachSliceSelector generated from: '<S169>/ffGain'
    //   Product: '<S169>/Product1'
    //   Product: '<S169>/Product2'
    //   Product: '<S169>/Product3'

    rtb_Product_i =
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ffGainsArray
       [2] * rtb_Product_i -
       fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.accelFbGainsArray
       [2] * rlim) +
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ff2GainsArray
      [2] * plim;

    // Outputs for Atomic SubSystem: '<S169>/pidWithDebug'
    // ForEachSliceSelector generated from: '<S169>/ctrlInputs' incorporates:
    //   BusAssignment: '<S108>/Bus Assignment3'
    //   Concatenate: '<S3>/Vector Concatenate'
    //   Constant: '<Root>/fcsParams'
    //   UnitDelay: '<S169>/Unit Delay'

    fcsModel_pidWithDebug_j(rtb_Product_i, plim, ylim, rtb_VectorConcatenate[2].
      integratorReset, rtb_MatrixMultiply[2],
      &fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.ctrlParamsArray
      [2], fcsModel_DW.CoreSubsys_i[2].UnitDelay_DSTATE, &rlim,
      &rtb_BusCreator_og, 0.008, &fcsModel_DW.CoreSubsys_i[2].pidWithDebug);

    // End of Outputs for SubSystem: '<S169>/pidWithDebug'

    // Update for UnitDelay: '<S169>/Unit Delay'
    fcsModel_DW.CoreSubsys_i[2].UnitDelay_DSTATE = rlim;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlOut '
    rtb_ImpAsg_InsertedFor_velCtrlO[2] = rlim;

    // ForEachSliceAssignment generated from: '<S169>/pidDebug'
    rtb_ImpAsg_InsertedFor_pidDebug[2] = rtb_BusCreator_og;

    // ForEachSliceAssignment generated from: '<S169>/velCtrlFf'
    rtb_ImpAsg_InsertedFor_velCtrlF[2] = rtb_Product_i;

    // ForEachSliceAssignment generated from: '<S169>/filtMeas'
    rtb_ImpAsg_InsertedFor_filtMeas[2] = ylim;

    // ForEachSliceAssignment generated from: '<S169>/filtCmd'
    rtb_ImpAsg_InsertedFor_filtCmd_[2] = plim;

    // End of Outputs for SubSystem: '<S111>/For Each Subsystem'

    // DiscreteTransferFcn: '<S186>/Discrete Transfer Fcn' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   Inport: '<Root>/stateEstimate'
    //   Math: '<S186>/Transpose'
    //   Math: '<S186>/Transpose1'

    fcsModel_DW.DiscreteTransferFcn_tmp =
      ExtUPointer_ref_U->stateEstimate.attitude_rad[2] -
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.firstOrderHeadingFilterDen
      [1] * fcsModel_DW.DiscreteTransferFcn_states;
    DiscreteTransferFcn =
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.firstOrderHeadingFilterNum
      [0] * fcsModel_DW.DiscreteTransferFcn_tmp +
      fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.firstOrderHeadingFilterNum
      [1] * fcsModel_DW.DiscreteTransferFcn_states;

    // MATLAB Function: '<S186>/NE Accel Cmds To Roll Pitch Cmds' incorporates:
    //   Constant: '<Root>/fcsParams'
    //   Constant: '<S186>/g'
    //   SignalConversion generated from: '<S187>/ SFunction '
    //   Sum: '<S186>/Sum'

    // MATLAB Function 'Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd/kinematicInversion/NE Accel Cmds To Roll Pitch Cmds': '<S187>:1' 
    // '<S187>:1:3' [rollCmd_rad, pitchCmd_rad, ~, ~] = ...
    // '<S187>:1:4'     accelsToDesiredRollPitchAngle_function(accelCmds_mps2, heading_rad, tiltMax_rad, gravity_mps2); 
    // ACCELSTODESIREDROLLPITCHANGLE  NED specific-force command -> roll/pitch setpoints. 
    //
    //    Converts a commanded NED specific force (gravity-INCLUDED vertical
    //    component, i.e. sfCmd(3) = aD_cmd - g; hover input is [0 0 -9.80665])
    //    into ZYX-Euler roll/pitch setpoints, with tilt limiting and edge-case
    //    guards suitable for embedded deployment.
    //
    //    Frames / signs (must match vehicle conventions):
    //      NED: x North, y East, z Down.  Body: FRD.  Euler: ZYX (yaw-pitch-roll). 
    //      +North force -> pitch < 0 (nose down).  +East force -> roll > 0.
    //
    //    Inputs:
    //      sfCmd_mps2   [3x1] commanded NED specific force, m/s^2.
    //                   sfCmd(3) MUST include gravity (aD - g); enforced <= -0.1g. 
    //      heading_rad  scalar, current yaw, rad, +ve North->East. Avoid heavy
    //                   filtering upstream (phase lag misdirects tilt during yaw). 
    //      tiltMax_rad  scalar, max tilt-from-vertical (circular cone), rad,
    //                   enforced into (0, 80 deg].
    //      gravity_mps2 scalar gravity acceleration magnitude
    //
    //    Outputs:
    //      rollCmd_rad, pitchCmd_rad  Euler setpoints, rad. |tilt| <= tiltMax_rad. 
    //      sfApplied_mps2  [3x1] post-saturation NED specific force actually
    //                   represented by the returned attitude. Feed this to the
    //                   velocity-loop anti-windup so the integrators see what was 
    //                   achievable, not what was asked.
    //      satFlags     [3x1] logical: [inputInvalid; azClamped; tiltLimited].
    //
    //    Guarantees (see property tests):
    //      - No division by zero, no NaN/Inf out for any finite or non-finite input. 
    //      - Horizontal force direction atan2(fE,fN) preserved by the tilt limiter. 
    //      - Vertical channel sfCmd(3) never altered by the tilt limiter.
    //      - Non-finite input returns level attitude + hover force (fail-safe). 
    //
    //    Codegen: fixed-size, no dynamic memory, no persistent state, single or 
    //    double precision via input class.
    //
    //  ---- constants --------------------------------------------------------- 
    // 'accelsToDesiredRollPitchAngle_function:43' AZ_MAX      = cast(-0.1,    'like', sfCmd_mps2) * gravity_mps2; 
    //  vertical clamp
    // 'accelsToDesiredRollPitchAngle_function:44' TILT_FLOOR  = cast(1e-3,    'like', sfCmd_mps2); 
    //  rad, > 0
    // 'accelsToDesiredRollPitchAngle_function:45' TILT_CEIL   = cast(80*pi/180, 'like', sfCmd_mps2); 
    //  rad, keep tan finite
    // 'accelsToDesiredRollPitchAngle_function:47' satFlags = false(3,1);
    //  ---- 1. input validity gate (fail level, hover force) ------------------ 
    // 'accelsToDesiredRollPitchAngle_function:50' if ~(isfinite(sfCmd_mps2(1)) && isfinite(sfCmd_mps2(2)) && ... 
    // 'accelsToDesiredRollPitchAngle_function:51'      isfinite(sfCmd_mps2(3)) && isfinite(heading_rad)) 
    // 'accelsToDesiredRollPitchAngle_function:59' fN = sfCmd_mps2(1);
    rtb_Product_i = rtb_ImpAsg_InsertedFor_velCtrlO[0];

    // 'accelsToDesiredRollPitchAngle_function:60' fE = sfCmd_mps2(2);
    fE = rtb_ImpAsg_InsertedFor_velCtrlO[1];

    //  ---- 2. vertical clamp: always a positive 'up' force ------------------- 
    // 'accelsToDesiredRollPitchAngle_function:63' az = sfCmd_mps2(3);
    az = rtb_ImpAsg_InsertedFor_velCtrlO[2] - 9.806;

    // 'accelsToDesiredRollPitchAngle_function:64' if az > AZ_MAX
    if (rtb_ImpAsg_InsertedFor_velCtrlO[2] - 9.806 > -0.98059999999999992) {
      // 'accelsToDesiredRollPitchAngle_function:65' az = AZ_MAX;
      az = -0.98059999999999992;

      // 'accelsToDesiredRollPitchAngle_function:66' satFlags(2) = true;
    }

    //  ---- 3. tilt limit: circular cone, vertical channel untouched ---------- 
    // 'accelsToDesiredRollPitchAngle_function:70' tiltMax = min(max(tiltMax_rad, TILT_FLOOR), TILT_CEIL); 
    // 'accelsToDesiredRollPitchAngle_function:71' fhMax   = (-az) * tan(tiltMax); 
    rlim = -az * std::tan(std::fmin(std::fmax
      (fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.tiltMax_rad,
       0.001), 1.3962634015954636));

    //  -az > 0 by construction
    // 'accelsToDesiredRollPitchAngle_function:72' fhSq    = fN*fN + fE*fE;
    plim = rtb_ImpAsg_InsertedFor_velCtrlO[0] * rtb_ImpAsg_InsertedFor_velCtrlO
      [0] + rtb_ImpAsg_InsertedFor_velCtrlO[1] *
      rtb_ImpAsg_InsertedFor_velCtrlO[1];

    // 'accelsToDesiredRollPitchAngle_function:73' if fhSq > fhMax*fhMax
    if (plim > rlim * rlim) {
      // 'accelsToDesiredRollPitchAngle_function:74' scale = fhMax / sqrt(fhSq); 
      fE = rlim / std::sqrt(plim);

      //  fhSq > 0 here, sqrt safe
      // 'accelsToDesiredRollPitchAngle_function:75' fN = fN * scale;
      rtb_Product_i = rtb_ImpAsg_InsertedFor_velCtrlO[0] * fE;

      //  common scale preserves
      // 'accelsToDesiredRollPitchAngle_function:76' fE = fE * scale;
      fE *= rtb_ImpAsg_InsertedFor_velCtrlO[1];

      //  horizontal direction
      // 'accelsToDesiredRollPitchAngle_function:77' satFlags(3) = true;
    }

    //  ---- 4. exact ZYX extraction in the heading frame ---------------------- 
    // 'accelsToDesiredRollPitchAngle_function:81' ch = cos(heading_rad);
    rlim = std::cos(DiscreteTransferFcn);

    // 'accelsToDesiredRollPitchAngle_function:82' sh = sin(heading_rad);
    DiscreteTransferFcn = std::sin(DiscreteTransferFcn);

    // 'accelsToDesiredRollPitchAngle_function:83' fx =  ch*fN + sh*fE;
    //  heading-frame components
    // 'accelsToDesiredRollPitchAngle_function:84' fy = -sh*fN + ch*fE;
    // 'accelsToDesiredRollPitchAngle_function:86' pitchCmd_rad = atan2(-fx, -az); 
    plim = std::atan2(-(rlim * rtb_Product_i + DiscreteTransferFcn * fE), -az);

    // BusAssignment: '<S168>/Bus Assignment1' incorporates:
    //   Inport: '<Root>/stateEstimate'
    //   MATLAB Function: '<S186>/NE Accel Cmds To Roll Pitch Cmds'

    //  az < 0  =>  -az > 0
    // 'accelsToDesiredRollPitchAngle_function:87' rollCmd_rad  = atan2( fy * cos(pitchCmd_rad), -az); 
    //  ---- 5. report what was actually applied (for anti-windup) ------------- 
    // 'accelsToDesiredRollPitchAngle_function:90' sfApplied_mps2 = [fN; fE; az]; 
    std::memset(&rtb_BusAssignment3_g, 0, sizeof(busCtrlInputs));
    rtb_BusAssignment3_g.cmd = std::atan2((-DiscreteTransferFcn * rtb_Product_i
      + rlim * fE) * std::cos(plim), -az);
    rtb_BusAssignment3_g.meas = ExtUPointer_ref_U->stateEstimate.attitude_rad[0];
    rtb_BusAssignment3_g.integratorReset = resetIntegrator;

    // BusAssignment: '<S168>/Bus Assignment2' incorporates:
    //   Inport: '<Root>/stateEstimate'
    //   MATLAB Function: '<S186>/NE Accel Cmds To Roll Pitch Cmds'

    std::memset(&rtb_BusAssignment2, 0, sizeof(busCtrlInputs));
    rtb_BusAssignment2.cmd = plim;
    rtb_BusAssignment2.meas = ExtUPointer_ref_U->stateEstimate.attitude_rad[1];
    rtb_BusAssignment2.integratorReset = resetIntegrator;

    // Outputs for Atomic SubSystem: '<S168>/holdOutputAtCenter'
    // MATLAB Function: '<S175>/holdOutputAtCenter' incorporates:
    //   Constant: '<S173>/Constant'
    //   Inport: '<Root>/stateEstimate'
    //   RelationalOperator: '<S173>/Compare'

    // MATLAB Function 'holdOutputAtCenter/holdOutputAtCenter': '<S180>:1'
    // '<S180>:1:2' [output, atCenter] = holdOutputAtCenter_function(input, trigger, auxTrigger, params); 
    // HOLDOUTPUTATCENTER_FUNCTION holds the output constant at last input if the 
    // trigger value is within user defined delta from the center
    //
    // auxTrigger is an additional trigger that is used to indicate when to
    // actually start holding the output
    // 'holdOutputAtCenter_function:8' if isempty(last_input)
    // 'holdOutputAtCenter_function:12' if(trigger <= (params.center + params.posDeltaFromCenter) && ... 
    // 'holdOutputAtCenter_function:13'         trigger >=(params.center - params.negDeltaFromCenter)) && auxTrigger 
    if ((fcsModel_DW.rcOutCmds.yawStick <= 0.017453292519943295) &&
        (fcsModel_DW.rcOutCmds.yawStick >= -0.017453292519943295) && (state >
         enumStateMachine::MTR_ARMED)) {
      // 'holdOutputAtCenter_function:14' atCenter = true;
      rtb_AND_p = true;
    } else {
      // 'holdOutputAtCenter_function:15' else
      // 'holdOutputAtCenter_function:16' atCenter = false;
      rtb_AND_p = false;

      // 'holdOutputAtCenter_function:17' last_input = input;
      fcsModel_DW.last_input = ExtUPointer_ref_U->stateEstimate.attitude_rad[2];
    }

    // End of Outputs for SubSystem: '<S168>/holdOutputAtCenter'

    // BusAssignment: '<S168>/Bus Assignment4'
    // 'holdOutputAtCenter_function:20' output = last_input;
    std::memset(&rtb_BusAssignment4, 0, sizeof(busCtrlInputs));

    // Switch: '<S168>/Switch' incorporates:
    //   RelationalOperator: '<S170>/Compare'

    if (rtb_AND_p) {
      // BusAssignment: '<S168>/Bus Assignment4' incorporates:
      //   Constant: '<S168>/Constant5'

      rtb_BusAssignment4.feedForwardCmd = 0.0;
    } else {
      // BusAssignment: '<S168>/Bus Assignment4'
      rtb_BusAssignment4.feedForwardCmd = fcsModel_DW.rcOutCmds.yawStick;
    }

    // End of Switch: '<S168>/Switch'

    // Outputs for Atomic SubSystem: '<S168>/holdOutputAtCenter'
    // BusAssignment: '<S168>/Bus Assignment4' incorporates:
    //   Inport: '<Root>/stateEstimate'
    //   MATLAB Function: '<S175>/holdOutputAtCenter'

    rtb_BusAssignment4.cmd = fcsModel_DW.last_input;

    // End of Outputs for SubSystem: '<S168>/holdOutputAtCenter'
    rtb_BusAssignment4.meas = ExtUPointer_ref_U->stateEstimate.attitude_rad[2];
    rtb_BusAssignment4.integratorReset = tmp_0;

    // Delay: '<S181>/Delay' incorporates:
    //   Constant: '<Root>/fcsParams'

    if (fcsModel_DW.icLoad) {
      fcsModel_DW.Delay_DSTATE =
        fcsModel_InstP_ref->fcsParamsArg.outerLoopCtrlParams.velCtrlParams.baseMass_kg;
    }

    DiscreteTransferFcn = fcsModel_DW.Delay_DSTATE;

    // Product: '<S181>/Product' incorporates:
    //   Constant: '<S181>/g'
    //   Delay: '<S181>/Delay'

    rtb_Product_i = -9.806 * fcsModel_DW.Delay_DSTATE;

    // Product: '<S168>/Divide1' incorporates:
    //   BusAssignment: '<S168>/Bus Assignment3'
    //   Inport: '<Root>/stateEstimate'
    //   Product: '<S168>/Product5'
    //   Sum: '<S168>/Sum2'
    //   Trigonometry: '<S168>/Sin2'
    //   Trigonometry: '<S168>/Sin3'

    az = 1.0 / (std::cos(ExtUPointer_ref_U->stateEstimate.attitude_rad[0]) * std::
                cos(ExtUPointer_ref_U->stateEstimate.attitude_rad[1])) *
      (rtb_ImpAsg_InsertedFor_velCtrlO[2] + rtb_Product_i);

    // RelationalOperator: '<S105>/Compare' incorporates:
    //   Constant: '<S105>/Constant'
    //   MATLAB Function: '<S4>/Interpret RC In Cmds'

    rtb_AND_p = (flightMode == enumFlightMode::POS_CONTROL);

    // MATLAB Function: '<S3>/assembleOuterLoopToInnerLoopBus' incorporates:
    //   BusCreator generated from: '<S3>/assembleOuterLoopToInnerLoopBus'
    //   Constant: '<S3>/Constant'
    //   Inport: '<Root>/stateEstimate'

    std::memcpy(&rtb_VectorConcatenate[0],
                &fcsModel_ConstP.pooled3.attCtrlInputs.ctrlInputsArray[0], 3U *
                sizeof(busCtrlInputs));

    // MATLAB Function 'Outer Loop Controller/assembleOuterLoopToInnerLoopBus': '<S107>:1' 
    // '<S107>:1:2' outBus.outerLoopCmds.thrustCmd_N = throttleCmd_N;
    // '<S107>:1:3' outDebug = throttleCmd_N;
    fE = fcsModel_DW.rcOutCmds.throttleStick;

    //  This is a stop gap setup where we are only assuming that rate control
    //  is active and therefore not setting up attCtrlInputs for Euler angle
    //  control
    // '<S107>:1:7' outBus.attCtrlInputs.ctrlInputsArray(1).cmd = rcOutCmds.rollStick; 
    rtb_VectorConcatenate[0].cmd = fcsModel_DW.rcOutCmds.rollStick;

    // '<S107>:1:8' outBus.attCtrlInputs.ctrlInputsArray(1).meas = stateEstimate.attitude_rad(1); 
    rtb_VectorConcatenate[0].meas =
      ExtUPointer_ref_U->stateEstimate.attitude_rad[0];

    // '<S107>:1:9' outBus.attCtrlInputs.ctrlInputsArray(2).cmd = rcOutCmds.pitchStick; 
    rtb_VectorConcatenate[1].cmd = fcsModel_DW.rcOutCmds.pitchStick;

    // '<S107>:1:10' outBus.attCtrlInputs.ctrlInputsArray(2).meas = stateEstimate.attitude_rad(2); 
    rtb_VectorConcatenate[1].meas =
      ExtUPointer_ref_U->stateEstimate.attitude_rad[1];

    // '<S107>:1:11' outBus.attCtrlInputs.ctrlInputsArray(3).cmd = rcOutCmds.yawStick; 
    rtb_VectorConcatenate[2].cmd = fcsModel_DW.rcOutCmds.yawStick;

    // '<S107>:1:12' outBus.attCtrlInputs.ctrlInputsArray(3).meas = stateEstimate.attitude_rad(3); 
    rtb_VectorConcatenate[2].meas =
      ExtUPointer_ref_U->stateEstimate.attitude_rad[2];

    // '<S107>:1:14' outBus.attCtrlInputs.ctrlInputsArray(1).integratorReset = resetIntegrator; 
    rtb_VectorConcatenate[0].integratorReset = resetIntegrator;

    // '<S107>:1:15' outBus.attCtrlInputs.ctrlInputsArray(2).integratorReset = resetIntegrator; 
    rtb_VectorConcatenate[1].integratorReset = resetIntegrator;

    // '<S107>:1:16' outBus.attCtrlInputs.ctrlInputsArray(3).integratorReset = true; 
    rtb_VectorConcatenate[2].integratorReset = true;

    // RelationalOperator: '<S104>/Compare' incorporates:
    //   Constant: '<S104>/Constant'
    //   MATLAB Function: '<S4>/Interpret RC In Cmds'

    rtb_Compare_od = (flightMode != enumFlightMode::ALT_CONTROL);

    // Switch: '<S3>/Switch2' incorporates:
    //   Switch: '<S3>/Switch'

    if (rtb_AND_p) {
      // Switch: '<S3>/Switch2' incorporates:
      //   BusAssignment: '<S168>/Bus Assignment'
      //   Concatenate: '<S168>/Vector Concatenate'

      fcsModel_DW.Switch2.outerLoopCmds.thrustCmd_N = az;
      fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0] =
        rtb_BusAssignment3_g;
      fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[1] = rtb_BusAssignment2;
      fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2] = rtb_BusAssignment4;
    } else if (rtb_Compare_od) {
      // Switch: '<S3>/Switch2' incorporates:
      //   MATLAB Function: '<S3>/assembleOuterLoopToInnerLoopBus'
      //   Switch: '<S3>/Switch'

      fcsModel_DW.Switch2.outerLoopCmds.thrustCmd_N =
        fcsModel_DW.rcOutCmds.throttleStick;
      std::memcpy(&fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0],
                  &rtb_VectorConcatenate[0], 3U * sizeof(busCtrlInputs));
    } else {
      // Switch: '<S3>/Switch2' incorporates:
      //   BusAssignment: '<S168>/Bus Assignment'
      //   Concatenate: '<S168>/Vector Concatenate'
      //   Concatenate: '<S3>/Vector Concatenate'
      //   Switch: '<S3>/Switch'

      fcsModel_DW.Switch2.outerLoopCmds.thrustCmd_N = az;
      std::memcpy(&fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0],
                  &rtb_VectorConcatenate[0], sizeof(busCtrlInputs) << 1U);
      fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2] = rtb_BusAssignment4;
    }

    // End of Switch: '<S3>/Switch2'
  }

  // Outputs for Iterator SubSystem: '<S20>/Attitude Control' incorporates:
  //   ForEach: '<S62>/For Each'

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'

  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].cmd,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.cmdSignalConditioningParamsArray
     [0], &plim, 0.004, &fcsModel_DW.CoreSubsys_p[0].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].meas,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.measSignalConditioningParamsArray
     [0], &ylim, 0.004, &fcsModel_DW.CoreSubsys_p[0].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block1'

  // MATLAB Function: '<S62>/pickAttitudeCmdAndMeas' incorporates:
  //   Constant: '<S20>/Constant'
  //   ForEachSliceSelector generated from: '<S62>/index'

  yCmd = plim;
  vylim = ylim;

  //  Passes cmd and meas as it is for roll and pitch channel
  //  but for yaw channel computes shortest angular distance between cmd Yaw
  //  and meas Yaw and overwrites Yaw cmd with that error and sets the meas Yaw to 
  //  zero for PID block
  // MATLAB Function 'Attitude Controller/Attitude Control/pickAttitudeCmdAndMeas': '<S67>:1' 
  // '<S67>:1:6' if index == cast(3, 'uint8')
  if (fcsModel_ConstP.Constant_Value_e[0] == 3) {
    // '<S67>:1:7' diff = mod(( cmd - meas + pi ), 2*pi) - pi;
    rlim = (plim - ylim) + 3.1415926535897931;
    yCmd = std::abs(rlim / 6.2831853071795862);
    if (std::abs(yCmd - std::floor(yCmd + 0.5)) > 2.2204460492503131E-16 * yCmd)
    {
      rlim = std::fmod(rlim, 6.2831853071795862);
    } else {
      rlim = 0.0;
    }

    if (rlim == 0.0) {
      rlim = 0.0;
    } else if (rlim < 0.0) {
      rlim += 6.2831853071795862;
    }

    // '<S67>:1:8' if diff < -pi
    // '<S67>:1:12' cmd = diff;
    yCmd = rlim - 3.1415926535897931;

    // '<S67>:1:13' meas = 0;
    vylim = 0.0;
  }

  // Outputs for Atomic SubSystem: '<S62>/pidWithDebug'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   MATLAB Function: '<S62>/pickAttitudeCmdAndMeas'
  //   UnitDelay: '<S62>/Unit Delay'

  fcsModel_pidWithDebug(fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].
                        feedForwardCmd, yCmd, vylim,
                        fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.ctrlParamsArray
                        [0], fcsModel_DW.CoreSubsys_p[0].UnitDelay_DSTATE, &rlim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_p[0].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S62>/pidWithDebug'

  // Update for UnitDelay: '<S62>/Unit Delay'
  fcsModel_DW.CoreSubsys_p[0].UnitDelay_DSTATE = rlim;

  // ForEachSliceAssignment generated from: '<S62>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[0] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S62>/angRateCmd '
  rtb_ImpAsg_InsertedFor_neVelCmd[0] = rlim;

  // ForEachSliceAssignment generated from: '<S62>/measFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[0] = ylim;

  // ForEachSliceAssignment generated from: '<S62>/cmdFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[0] = plim;

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'

  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[1].cmd,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.cmdSignalConditioningParamsArray
     [1], &plim, 0.004, &fcsModel_DW.CoreSubsys_p[1].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[1].meas,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.measSignalConditioningParamsArray
     [1], &ylim, 0.004, &fcsModel_DW.CoreSubsys_p[1].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block1'

  // MATLAB Function: '<S62>/pickAttitudeCmdAndMeas' incorporates:
  //   Constant: '<S20>/Constant'
  //   ForEachSliceSelector generated from: '<S62>/index'

  yCmd = plim;
  vylim = ylim;

  //  Passes cmd and meas as it is for roll and pitch channel
  //  but for yaw channel computes shortest angular distance between cmd Yaw
  //  and meas Yaw and overwrites Yaw cmd with that error and sets the meas Yaw to 
  //  zero for PID block
  // MATLAB Function 'Attitude Controller/Attitude Control/pickAttitudeCmdAndMeas': '<S67>:1' 
  // '<S67>:1:6' if index == cast(3, 'uint8')
  if (fcsModel_ConstP.Constant_Value_e[1] == 3) {
    // '<S67>:1:7' diff = mod(( cmd - meas + pi ), 2*pi) - pi;
    rlim = (plim - ylim) + 3.1415926535897931;
    yCmd = std::abs(rlim / 6.2831853071795862);
    if (std::abs(yCmd - std::floor(yCmd + 0.5)) > 2.2204460492503131E-16 * yCmd)
    {
      rlim = std::fmod(rlim, 6.2831853071795862);
    } else {
      rlim = 0.0;
    }

    if (rlim == 0.0) {
      rlim = 0.0;
    } else if (rlim < 0.0) {
      rlim += 6.2831853071795862;
    }

    // '<S67>:1:8' if diff < -pi
    // '<S67>:1:12' cmd = diff;
    yCmd = rlim - 3.1415926535897931;

    // '<S67>:1:13' meas = 0;
    vylim = 0.0;
  }

  // Outputs for Atomic SubSystem: '<S62>/pidWithDebug'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   MATLAB Function: '<S62>/pickAttitudeCmdAndMeas'
  //   UnitDelay: '<S62>/Unit Delay'

  fcsModel_pidWithDebug(fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[1].
                        feedForwardCmd, yCmd, vylim,
                        fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[1].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.ctrlParamsArray
                        [1], fcsModel_DW.CoreSubsys_p[1].UnitDelay_DSTATE, &rlim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_p[1].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S62>/pidWithDebug'

  // Update for UnitDelay: '<S62>/Unit Delay'
  fcsModel_DW.CoreSubsys_p[1].UnitDelay_DSTATE = rlim;

  // ForEachSliceAssignment generated from: '<S62>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[1] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S62>/angRateCmd '
  rtb_ImpAsg_InsertedFor_neVelCmd[1] = rlim;

  // ForEachSliceAssignment generated from: '<S62>/measFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[1] = ylim;

  // ForEachSliceAssignment generated from: '<S62>/cmdFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[1] = plim;

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'

  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2].cmd,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.cmdSignalConditioningParamsArray
     [2], &plim, 0.004, &fcsModel_DW.CoreSubsys_p[2].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1
    (fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2].meas,
     &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.measSignalConditioningParamsArray
     [2], &ylim, 0.004, &fcsModel_DW.CoreSubsys_p[2].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S62>/Signal Conditioning Block1'

  // MATLAB Function: '<S62>/pickAttitudeCmdAndMeas' incorporates:
  //   Constant: '<S20>/Constant'
  //   ForEachSliceSelector generated from: '<S62>/index'

  yCmd = plim;
  vylim = ylim;

  //  Passes cmd and meas as it is for roll and pitch channel
  //  but for yaw channel computes shortest angular distance between cmd Yaw
  //  and meas Yaw and overwrites Yaw cmd with that error and sets the meas Yaw to 
  //  zero for PID block
  // MATLAB Function 'Attitude Controller/Attitude Control/pickAttitudeCmdAndMeas': '<S67>:1' 
  // '<S67>:1:6' if index == cast(3, 'uint8')
  if (fcsModel_ConstP.Constant_Value_e[2] == 3) {
    // '<S67>:1:7' diff = mod(( cmd - meas + pi ), 2*pi) - pi;
    rlim = (plim - ylim) + 3.1415926535897931;
    yCmd = std::abs(rlim / 6.2831853071795862);
    if (std::abs(yCmd - std::floor(yCmd + 0.5)) > 2.2204460492503131E-16 * yCmd)
    {
      rlim = std::fmod(rlim, 6.2831853071795862);
    } else {
      rlim = 0.0;
    }

    if (rlim == 0.0) {
      rlim = 0.0;
    } else if (rlim < 0.0) {
      rlim += 6.2831853071795862;
    }

    // '<S67>:1:8' if diff < -pi
    // '<S67>:1:12' cmd = diff;
    yCmd = rlim - 3.1415926535897931;

    // '<S67>:1:13' meas = 0;
    vylim = 0.0;
  }

  // Outputs for Atomic SubSystem: '<S62>/pidWithDebug'
  // ForEachSliceSelector generated from: '<S62>/ctrlInputs' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   MATLAB Function: '<S62>/pickAttitudeCmdAndMeas'
  //   UnitDelay: '<S62>/Unit Delay'

  fcsModel_pidWithDebug(fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2].
                        feedForwardCmd, yCmd, vylim,
                        fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.attCtrlParams.ctrlParamsArray
                        [2], fcsModel_DW.CoreSubsys_p[2].UnitDelay_DSTATE, &rlim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_p[2].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S62>/pidWithDebug'

  // Update for UnitDelay: '<S62>/Unit Delay'
  fcsModel_DW.CoreSubsys_p[2].UnitDelay_DSTATE = rlim;

  // ForEachSliceAssignment generated from: '<S62>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.pidDebug[2] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S62>/angRateCmd '
  rtb_ImpAsg_InsertedFor_neVelCmd[2] = rlim;

  // ForEachSliceAssignment generated from: '<S62>/measFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.meas[2] = ylim;

  // ForEachSliceAssignment generated from: '<S62>/cmdFlt'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.attCtrlDebug.cmd[2] = plim;

  // End of Outputs for SubSystem: '<S20>/Attitude Control'

  // Switch: '<S19>/Switch'
  rtb_MatrixMultiply[0] = rtb_ImpAsg_InsertedFor_neVelCmd[0];
  rtb_MatrixMultiply[1] = rtb_ImpAsg_InsertedFor_neVelCmd[1];

  // Switch: '<S20>/Switch' incorporates:
  //   Constant: '<S63>/Constant'
  //   MATLAB Function: '<S4>/Interpret RC In Cmds'
  //   RelationalOperator: '<S63>/Compare'
  //   Switch: '<S19>/Switch'

  if (flightMode == enumFlightMode::STABILIZE) {
    rtb_MatrixMultiply[2] = fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[2]
      .cmd;
  } else {
    rtb_MatrixMultiply[2] = rtb_ImpAsg_InsertedFor_neVelCmd[2];
  }

  // End of Switch: '<S20>/Switch'

  // MATLAB Function: '<S19>/EulerRates2BodyRates' incorporates:
  //   Inport: '<Root>/stateEstimate'
  //   Switch: '<S19>/Switch'

  // MATLAB Function 'EulerRates2BodyRates': '<S61>:1'
  // '<S61>:1:3' bodyRates_radps = eulerRates2bodyRates_function(taitBryanRates_radps,shipOrientation_rad); 
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Converts  rate of change of TaitBryan angles in the globle frame to
  // rotational rate of change in the body frame
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Inputs:
  // -taitBryanRates_radps:= {3x1 [omegaX;omegaY;omegaZ] describing the rate
  // of change in the space axes of the TaitBryan angles}
  // -shipOrientation_rad:= {3x1 [phi;theta;psi] vector describing orentation
  // of the body frame in the space frame}
  // Output:
  // -bodyRates_radps:= {3x1 [omegaX;omegaY;omegaZ] vector of rotation
  // rates of the body in the body frame}
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // 'eulerRates2bodyRates_function:18' roll = shipOrientation_rad(1);
  // 'eulerRates2bodyRates_function:19' pitch = shipOrientation_rad(2);
  rlim = ExtUPointer_ref_U->stateEstimate.attitude_rad[1];

  // 'eulerRates2bodyRates_function:20' eps = 10^(-12);
  // 'eulerRates2bodyRates_function:21' limit = pi/740;
  // Check for pm pi/2 rotation to avoid NaNs
  // 'eulerRates2bodyRates_function:24' if( abs( abs(pitch)- pi/2 ) <= limit || abs( abs(pitch) - 3*pi/2 ) <= limit) 
  if ((std::abs(std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) -
                1.5707963267948966) <= 0.004245395477824045) || (std::abs(std::
        abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) - 4.71238898038469)
       <= 0.004245395477824045)) {
    // 'eulerRates2bodyRates_function:25' if((abs(pitch)- pi/2) <= 0 || (abs(pitch) - 3*pi/2) <= 0) 
    if (std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) -
        1.5707963267948966 <= 0.0) {
      // 'eulerRates2bodyRates_function:26' pitch = sign(pitch)*( abs(pitch) - limit); 
      if (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] < 0.0) {
        tCmd = -1;
      } else {
        tCmd = (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] > 0.0);
      }

      rlim = (std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) -
              0.004245395477824045) * static_cast<real_T>(tCmd);
    } else if (std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) -
               4.71238898038469 <= 0.0) {
      // 'eulerRates2bodyRates_function:26' pitch = sign(pitch)*( abs(pitch) - limit); 
      if (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] < 0.0) {
        tCmd = -1;
      } else {
        tCmd = (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] > 0.0);
      }

      rlim = (std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) -
              0.004245395477824045) * static_cast<real_T>(tCmd);
    } else {
      // 'eulerRates2bodyRates_function:27' else
      // 'eulerRates2bodyRates_function:28' pitch = sign(pitch)*( abs(pitch) + limit); 
      if (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] < 0.0) {
        tCmd = -1;
      } else {
        tCmd = (ExtUPointer_ref_U->stateEstimate.attitude_rad[1] > 0.0);
      }

      rlim = (std::abs(ExtUPointer_ref_U->stateEstimate.attitude_rad[1]) +
              0.004245395477824045) * static_cast<real_T>(tCmd);
    }
  }

  // Construct conversion matrix
  // 'eulerRates2bodyRates_function:33' conversionMatrix = [1, 0, -sin(pitch);
  // 'eulerRates2bodyRates_function:34'     0, cos(roll), sin(roll)*cos(pitch);
  // 'eulerRates2bodyRates_function:35'     0, -sin(roll), cos(roll)*cos(pitch)]; 
  plim = std::cos(rlim);
  rtb_Transpose[0] = 1.0;
  rtb_Transpose[3] = 0.0;
  rtb_Transpose[6] = -std::sin(rlim);
  rtb_Transpose[1] = 0.0;
  rtb_Transpose[4] = std::cos(ExtUPointer_ref_U->stateEstimate.attitude_rad[0]);
  rtb_Transpose[7] = std::sin(ExtUPointer_ref_U->stateEstimate.attitude_rad[0]) *
    plim;
  rtb_Transpose[2] = 0.0;
  rtb_Transpose[5] = -std::sin(ExtUPointer_ref_U->stateEstimate.attitude_rad[0]);
  rtb_Transpose[8] = std::cos(ExtUPointer_ref_U->stateEstimate.attitude_rad[0]) *
    plim;

  // 'eulerRates2bodyRates_function:37' conversionMatrix = zeroSmallValues(conversionMatrix,eps); 
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // sets values in the M = zero if abs(values) is below this_eps
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // Inputs: M:={Any real valued verable}, eps:={values in M below the abs of this 
  // esp are set to zero}
  // Ouputs: M:={with values below the esp set to zero}
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // 'zeroSmallValues:10' for ii=1:size(M,1)
  // Convert rotation rate to change in TaitBryan angles
  // 'eulerRates2bodyRates_function:40' bodyRates_radps =  conversionMatrix * taitBryanRates_radps; 
  for (tCmd = 0; tCmd < 3; tCmd++) {
    // 'zeroSmallValues:11' for jj = 1:size(M,2)
    // 'zeroSmallValues:12' if(abs(M(ii,jj))<= abs(eps))
    if (rtb_Transpose[tCmd] <= 1.0E-12) {
      // 'zeroSmallValues:13' M(ii,jj) = 0;
      rtb_Transpose[tCmd] = 0.0;
    }

    // 'zeroSmallValues:12' if(abs(M(ii,jj))<= abs(eps))
    if (std::abs(rtb_Transpose[tCmd + 3]) <= 1.0E-12) {
      // 'zeroSmallValues:13' M(ii,jj) = 0;
      rtb_Transpose[tCmd + 3] = 0.0;
    }

    // 'zeroSmallValues:12' if(abs(M(ii,jj))<= abs(eps))
    if (std::abs(rtb_Transpose[tCmd + 6]) <= 1.0E-12) {
      // 'zeroSmallValues:13' M(ii,jj) = 0;
      rtb_Transpose[tCmd + 6] = 0.0;
    }
  }

  plim = 0.0;
  ylim = 0.0;
  yCmd = 0.0;
  tCmd = 0;
  for (rCmd = 0; rCmd < 3; rCmd++) {
    vxCmd_unitRange = rtb_MatrixMultiply[rCmd];
    plim += rtb_Transpose[tCmd] * vxCmd_unitRange;
    ylim += rtb_Transpose[tCmd + 1] * vxCmd_unitRange;
    yCmd += rtb_Transpose[tCmd + 2] * vxCmd_unitRange;
    tCmd += 3;
  }

  // BusCreator: '<S19>/Bus Creator' incorporates:
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Inport: '<Root>/stateEstimate'
  //   MATLAB Function: '<S19>/EulerRates2BodyRates'

  rtb_VectorConcatenate[0].feedForwardCmd = 0.0;
  rtb_VectorConcatenate[0].cmd = plim;
  rtb_VectorConcatenate[0].meas =
    ExtUPointer_ref_U->stateEstimate.bodyAngRates_radps[0];
  rtb_VectorConcatenate[0].integratorReset =
    fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].integratorReset;
  rtb_VectorConcatenate[0].trackingCtrlCmd = 0.0;

  // BusCreator: '<S19>/Bus Creator3' incorporates:
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Inport: '<Root>/stateEstimate'
  //   MATLAB Function: '<S19>/EulerRates2BodyRates'

  rtb_VectorConcatenate[1].feedForwardCmd = 0.0;
  rtb_VectorConcatenate[1].cmd = ylim;
  rtb_VectorConcatenate[1].meas =
    ExtUPointer_ref_U->stateEstimate.bodyAngRates_radps[1];
  rtb_VectorConcatenate[1].integratorReset =
    fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].integratorReset;
  rtb_VectorConcatenate[1].trackingCtrlCmd = 0.0;

  // BusCreator: '<S19>/Bus Creator4' incorporates:
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Inport: '<Root>/stateEstimate'
  //   MATLAB Function: '<S19>/EulerRates2BodyRates'

  rtb_VectorConcatenate[2].feedForwardCmd = 0.0;
  rtb_VectorConcatenate[2].cmd = yCmd;
  rtb_VectorConcatenate[2].meas =
    ExtUPointer_ref_U->stateEstimate.bodyAngRates_radps[2];
  rtb_VectorConcatenate[2].integratorReset =
    fcsModel_DW.Switch2.attCtrlInputs.ctrlInputsArray[0].integratorReset;
  rtb_VectorConcatenate[2].trackingCtrlCmd = 0.0;

  // Outputs for Atomic SubSystem: '<S2>/Angular Rate Controller'
  // Outputs for Iterator SubSystem: '<S18>/For Each Subsystem' incorporates:
  //   ForEach: '<S21>/For Each'

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S21>/ctrlInputs' incorporates:
  //   BusCreator: '<S19>/Bus Creator1'
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Constant: '<Root>/fcsParams'
  //   UnitDelay: '<S21>/Unit Delay'

  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[0].cmd,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.cmdSignalConditioningParamsArray
    [0], &rlim, 0.004, &fcsModel_DW.CoreSubsys_a[0].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[0].meas,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.measSignalConditioningParamsArray
    [0], &plim, 0.004, &fcsModel_DW.CoreSubsys_a[0].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block1'

  // Outputs for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug(0.0, rlim, plim, rtb_VectorConcatenate[0].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.ctrlParamsArray
                        [0], fcsModel_DW.CoreSubsys_a[0].UnitDelay_DSTATE, &ylim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_a[0].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S21>/pidWithDebug'

  // Update for UnitDelay: '<S21>/Unit Delay'
  fcsModel_DW.CoreSubsys_a[0].UnitDelay_DSTATE = ylim;

  // ForEachSliceAssignment generated from: '<S21>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[0] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S21>/angAccelCmd_radps2'
  rtb_MatrixMultiply[0] = ylim;

  // ForEachSliceAssignment generated from: '<S21>/filtMeas'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[0] = plim;

  // ForEachSliceAssignment generated from: '<S21>/filtCmd'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[0] = rlim;

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S21>/ctrlInputs' incorporates:
  //   BusCreator: '<S19>/Bus Creator1'
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Constant: '<Root>/fcsParams'
  //   UnitDelay: '<S21>/Unit Delay'

  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[1].cmd,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.cmdSignalConditioningParamsArray
    [1], &rlim, 0.004, &fcsModel_DW.CoreSubsys_a[1].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[1].meas,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.measSignalConditioningParamsArray
    [1], &plim, 0.004, &fcsModel_DW.CoreSubsys_a[1].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block1'

  // Outputs for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug(0.0, rlim, plim, rtb_VectorConcatenate[1].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.ctrlParamsArray
                        [1], fcsModel_DW.CoreSubsys_a[1].UnitDelay_DSTATE, &ylim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_a[1].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S21>/pidWithDebug'

  // Update for UnitDelay: '<S21>/Unit Delay'
  fcsModel_DW.CoreSubsys_a[1].UnitDelay_DSTATE = ylim;

  // ForEachSliceAssignment generated from: '<S21>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[1] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S21>/angAccelCmd_radps2'
  rtb_MatrixMultiply[1] = ylim;

  // ForEachSliceAssignment generated from: '<S21>/filtMeas'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[1] = plim;

  // ForEachSliceAssignment generated from: '<S21>/filtCmd'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[1] = rlim;

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  // ForEachSliceSelector generated from: '<S21>/ctrlInputs' incorporates:
  //   BusCreator: '<S19>/Bus Creator1'
  //   Concatenate: '<S19>/Vector Concatenate'
  //   Constant: '<Root>/fcsParams'
  //   UnitDelay: '<S21>/Unit Delay'

  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[2].cmd,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.cmdSignalConditioningParamsArray
    [2], &rlim, 0.004, &fcsModel_DW.CoreSubsys_a[2].SignalConditioningBlock);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block'

  // Outputs for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  fcsMod_SignalConditioningBlock1(rtb_VectorConcatenate[2].meas,
    &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.measSignalConditioningParamsArray
    [2], &plim, 0.004, &fcsModel_DW.CoreSubsys_a[2].SignalConditioningBlock1);

  // End of Outputs for SubSystem: '<S21>/Signal Conditioning Block1'

  // Outputs for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug(0.0, rlim, plim, rtb_VectorConcatenate[2].
                        integratorReset, 0.0,
                        &fcsModel_InstP_ref->fcsParamsArg.innerLoopCtrlParams.angRateCtrlParams.ctrlParamsArray
                        [2], fcsModel_DW.CoreSubsys_a[2].UnitDelay_DSTATE, &ylim,
                        &rtb_BusCreator_og, 0.004, &fcsModel_DW.CoreSubsys_a[2].
                        pidWithDebug);

  // End of Outputs for SubSystem: '<S21>/pidWithDebug'

  // Update for UnitDelay: '<S21>/Unit Delay'
  fcsModel_DW.CoreSubsys_a[2].UnitDelay_DSTATE = ylim;

  // ForEachSliceAssignment generated from: '<S21>/pidDebug'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.pidDebug[2] =
    rtb_BusCreator_og;

  // ForEachSliceAssignment generated from: '<S21>/angAccelCmd_radps2'
  rtb_MatrixMultiply[2] = ylim;

  // ForEachSliceAssignment generated from: '<S21>/filtMeas'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.meas[2] = plim;

  // ForEachSliceAssignment generated from: '<S21>/filtCmd'
  fcsModel_Y.fcsDebug.innerLoopCtrlDebug.angRateCtrlDebug.cmd[2] = rlim;

  // End of Outputs for SubSystem: '<S18>/For Each Subsystem'
  // End of Outputs for SubSystem: '<S2>/Angular Rate Controller'

  // Product: '<S2>/Matrix Multiply' incorporates:
  //   Constant: '<S2>/Constant'
  //   ForEachSliceAssignment generated from: '<S21>/angAccelCmd_radps2'

  rtb_ImpAsg_InsertedFor_angRat_0 = 0.0;
  rtb_ImpAsg_InsertedFor_angRat_1 = 0.0;
  rtb_ImpAsg_InsertedFor_angRat_2 = 0.0;
  tCmd = 0;
  for (rCmd = 0; rCmd < 3; rCmd++) {
    vxCmd_unitRange = rtb_MatrixMultiply[rCmd];
    rtb_ImpAsg_InsertedFor_angRat_0 += fcsModel_ConstP.Constant_Value_n[tCmd] *
      vxCmd_unitRange;
    rtb_ImpAsg_InsertedFor_angRat_1 += fcsModel_ConstP.Constant_Value_n[tCmd + 1]
      * vxCmd_unitRange;
    rtb_ImpAsg_InsertedFor_angRat_2 += fcsModel_ConstP.Constant_Value_n[tCmd + 2]
      * vxCmd_unitRange;
    tCmd += 3;
  }

  // RelationalOperator: '<S9>/Compare' incorporates:
  //   Constant: '<S9>/Constant'

  resetIntegrator = (rtb_chirpTrigger == enumChirpTrigger::ON);

  // MATLAB Function: '<S15>/Generate Chirp' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   UnitDelay: '<S15>/Unit Delay'
  //   UnitDelay: '<S15>/Unit Delay1'

  // MATLAB Function 'sysIdInputGeneration/chirpInjection/Generate Chirp': '<S17>:1' 
  // '<S17>:1:4' [chirpSignal, chirpTime_s, theta_rad] = generateChirp_function(trigger, amp, fStart_Hz, fEnd_Hz, tRec_s, ... 
  // '<S17>:1:5'                                        fadeInTime_s, fadeOutTime_s, c1, c2, inElpsdChirpTime_s, ... 
  // '<S17>:1:6'                                        inTheta_rad, sampleTime_s); 
  // GENERATECHIRP_FUNCTION generates chirp for sysId purposes
  //
  // Inputs:
  // trigger: Binary signal, if it is 0 this function outputs 0
  // amp: Chirp amplitude
  // fStart_Hz: Chirp starting frequency
  // fEnd_Hz: Chirp end frequency
  // tRec_s: Total chirp duratin
  // fadeInTime_s: Duration at the beginning during which amplitude will be increase to 1 
  // fadeOutTime_s: Duration at the end during which amplitude will decrease to 0 
  // c1: Chirp parameter 1
  // c2: Chirp parameter 2
  // inElpsdDhirpTime_s: Previous value of elapsed time since the chirp was triggered else it is set to 0 
  // inTheta_rad: Previous chirp phase angle
  // sampleTime_s: Sample time
  //
  // Outputs:
  // chirpSignal: Chirp signal at the current time step
  // chirpTime_s: Elapsed time since chirp was triggerd else it is set to 0
  // theta_rad: Chirp phase angle
  //  trim time
  // 'generateChirp_function:26' if(trigger == 0)
  if (!resetIntegrator) {
    // 'generateChirp_function:27' chirpSignal = 0;
    rlim = 0.0;

    // 'generateChirp_function:28' chirpTime_s = 0;
    ylim = 0.0;

    // 'generateChirp_function:29' theta_rad = 0;
    plim = 0.0;
  } else {
    //  Chirp is triggered
    //  Convert frequencies to rad/s
    // 'generateChirp_function:36' fStart_radps = 2*pi*fStart_Hz;
    plim = 6.2831853071795862 *
      fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fStart_Hz;

    // 'generateChirp_function:37' fEnd_radps = 2*pi*fEnd_Hz;
    //  Find the slope of amplitude during the fade in and fade out time
    // 'generateChirp_function:40' fadeInSlope = 1/fadeInTime_s;
    // 'generateChirp_function:41' fadeOutSlope = 1/fadeOutTime_s;
    //  Find the time when the fade out starts
    // 'generateChirp_function:44' tFadeOutStart_s = tRec_s - fadeOutTime_s;
    rlim = fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.tRec_s -
      fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fadeOutTime_s;

    //  Time required for 2 full cycles at starting frequency
    // 'generateChirp_function:47' timeLowCycles_s = 2*(2*pi/fStart_radps);
    ylim = 6.2831853071795862 / plim * 2.0;

    //  Do fade in and fade out at appropriate times
    // 'generateChirp_function:50' if(inElpsdChirpTime_s <= fadeInTime_s)
    if (fcsModel_DW.UnitDelay1_DSTATE <=
        fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fadeInTime_s) {
      // 'generateChirp_function:51' a = fadeInSlope*inElpsdChirpTime_s;
      rlim = 1.0 /
        fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fadeInTime_s *
        fcsModel_DW.UnitDelay1_DSTATE;
    } else if (fcsModel_DW.UnitDelay1_DSTATE >= rlim) {
      // 'generateChirp_function:52' elseif(inElpsdChirpTime_s >= tFadeOutStart_s) 
      // 'generateChirp_function:53' a = 1 - fadeOutSlope*(inElpsdChirpTime_s - tFadeOutStart_s); 
      rlim = 1.0 - 1.0 /
        fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fadeOutTime_s *
        (fcsModel_DW.UnitDelay1_DSTATE - rlim);
    } else {
      // 'generateChirp_function:54' else
      // 'generateChirp_function:55' a = 1;
      rlim = 1.0;
    }

    //  Create chirps
    // 'generateChirp_function:59' chirpSignal = amp*a*sin(inTheta_rad);
    rlim *= std::sin(fcsModel_DW.UnitDelay_DSTATE);

    //  Increment theta
    // 'generateChirp_function:62' if(inElpsdChirpTime_s < timeLowCycles_s)
    if (fcsModel_DW.UnitDelay1_DSTATE < ylim) {
      // 'generateChirp_function:63' theta_rad = wrapTo2Pi_(inTheta_rad + fStart_radps*sampleTime_s); 
      plim = plim * 0.004 + fcsModel_DW.UnitDelay_DSTATE;

      // WRAPTO2PI_ custom implementation of matlab's wrapTp2Pi
      // 'wrapTo2Pi_:3' twoPi = 2*pi;
      // 'wrapTo2Pi_:4' outAng = inAng -  twoPi* floor( inAng / twoPi );
      plim -= std::floor(plim / 6.2831853071795862) * 6.2831853071795862;
    } else {
      // 'generateChirp_function:64' else
      // 'generateChirp_function:65' k = c2*(exp(c1*(inElpsdChirpTime_s - timeLowCycles_s)/(tRec_s - timeLowCycles_s)) - 1); 
      // 'generateChirp_function:66' theta_rad = wrapTo2Pi_(inTheta_rad + (fStart_radps + k*(fEnd_radps - fStart_radps))*sampleTime_s); 
      plim = ((std::exp((fcsModel_DW.UnitDelay1_DSTATE - ylim) *
                        fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.c1
                        /
                        (fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.tRec_s
                         - ylim)) - 1.0) *
              fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.c2 *
              (6.2831853071795862 *
               fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fEnd_hz -
               plim) + plim) * 0.004 + fcsModel_DW.UnitDelay_DSTATE;

      // WRAPTO2PI_ custom implementation of matlab's wrapTp2Pi
      // 'wrapTo2Pi_:3' twoPi = 2*pi;
      // 'wrapTo2Pi_:4' outAng = inAng -  twoPi* floor( inAng / twoPi );
      plim -= std::floor(plim / 6.2831853071795862) * 6.2831853071795862;
    }

    //  Increment chirp time
    // 'generateChirp_function:70' chirpTime_s = inElpsdChirpTime_s + sampleTime_s; 
    ylim = fcsModel_DW.UnitDelay1_DSTATE + 0.004;

    // 'generateChirp_function:72' if(inElpsdChirpTime_s > tRec_s)
    if (fcsModel_DW.UnitDelay1_DSTATE >
        fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.tRec_s) {
      // 'generateChirp_function:73' chirpSignal = 0;
      rlim = 0.0;

      // 'generateChirp_function:74' theta_rad = 0;
      plim = 0.0;
    }
  }

  // End of MATLAB Function: '<S15>/Generate Chirp'

  // DiscreteTransferFcn: '<S15>/Discrete Transfer Fcn' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   Gain: '<S16>/Output'
  //   Math: '<S8>/Transpose1'
  //   RandomNumber: '<S16>/White Noise'

  yCmd = 0.5 * fcsModel_DW.NextOutput -
    fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.filterDen[1] *
    fcsModel_DW.DiscreteTransferFcn_states_c;

  // Product: '<S15>/Product' incorporates:
  //   Constant: '<Root>/fcsParams'
  //   DiscreteTransferFcn: '<S15>/Discrete Transfer Fcn'
  //   Math: '<S8>/Transpose'
  //   Sum: '<S15>/Sum'

  vylim = resetIntegrator ?
    (fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.filterNum[0] * yCmd +
     fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.filterNum[1] *
     fcsModel_DW.DiscreteTransferFcn_states_c) + rlim : 0.0;

  // SignalConversion generated from: '<S1>/Matrix Multiply' incorporates:
  //   BusCreator: '<S2>/Bus Creator1'
  //   Constant: '<Root>/fcsParams'
  //   Constant: '<S10>/Constant'
  //   Constant: '<S11>/Constant'
  //   Constant: '<S12>/Constant'
  //   Constant: '<S13>/Constant'
  //   Gain: '<S1>/Gain'
  //   Product: '<S2>/Matrix Multiply'
  //   Product: '<S8>/Product'
  //   Product: '<S8>/Product1'
  //   Product: '<S8>/Product2'
  //   Product: '<S8>/Product3'
  //   RelationalOperator: '<S10>/Compare'
  //   RelationalOperator: '<S11>/Compare'
  //   RelationalOperator: '<S12>/Compare'
  //   RelationalOperator: '<S13>/Compare'
  //   Sum: '<S8>/Sum'
  //   Sum: '<S8>/Sum1'
  //   Sum: '<S8>/Sum2'
  //   Sum: '<S8>/Sum3'

  rtb_DiscreteTransferFcn_e[0] = (rtb_chirpType == enumChirpType::FZ ?
    fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.fzAmp * vylim : 0.0) +
    fcsModel_DW.Switch2.outerLoopCmds.thrustCmd_N;
  rtb_DiscreteTransferFcn_e[1] = (rtb_chirpType == enumChirpType::MX ?
    fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.mxAmp * vylim : 0.0) +
    rtb_ImpAsg_InsertedFor_angRat_0;
  rtb_DiscreteTransferFcn_e[2] = (rtb_chirpType == enumChirpType::MY ?
    fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.myAmp * vylim : 0.0) +
    rtb_ImpAsg_InsertedFor_angRat_1;
  rtb_DiscreteTransferFcn_e[3] = (rtb_chirpType == enumChirpType::MY ?
    fcsModel_InstP_ref->fcsParamsArg.sysIdInjectionParams.myAmp * vylim : 0.0) +
    rtb_ImpAsg_InsertedFor_angRat_2 * 0.7;

  // Product: '<S1>/Matrix Multiply' incorporates:
  //   Constant: '<S1>/Constant'
  //   DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'

  rlim = 0.0;
  rlim_0 = 0.0;
  rlim_1 = 0.0;
  rlim_2 = 0.0;
  tCmd = 0;
  for (rCmd = 0; rCmd < 4; rCmd++) {
    vxCmd_unitRange = rtb_DiscreteTransferFcn_e[rCmd];
    rlim += fcsModel_ConstP.Constant_Value_c[tCmd] * vxCmd_unitRange;
    rlim_0 += fcsModel_ConstP.Constant_Value_c[tCmd + 1] * vxCmd_unitRange;
    rlim_1 += fcsModel_ConstP.Constant_Value_c[tCmd + 2] * vxCmd_unitRange;
    rlim_2 += fcsModel_ConstP.Constant_Value_c[tCmd + 3] * vxCmd_unitRange;
    tCmd += 4;
  }

  // RelationalOperator: '<S6>/Compare' incorporates:
  //   Constant: '<S6>/Constant'

  // Unit Conversion - from: rad/s to: rpm
  // Expression: output = (9.5493*input) + (0)
  resetIntegrator = (state == enumStateMachine::INACTIVE);

  // Saturate: '<S1>/Saturation' incorporates:
  //   DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
  //   Product: '<S1>/Matrix Multiply'

  if (rlim > 792309.908865229) {
    rlim = 792309.908865229;
  } else if (rlim < 0.0) {
    rlim = 0.0;
  }

  // DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
  //   Saturate: '<S1>/Saturation'
  //   Sqrt: '<S1>/Sqrt'
  //   UnitConversion: '<S5>/Unit Conversion'

  vxCmd_unitRange = 9.5492965855137211 * std::sqrt(rlim) - -0.029612798683843349
    * fcsModel_DW.DiscreteTransferFcn_states_d[0];
  rlim = 0.48519360065807832 * vxCmd_unitRange + 0.48519360065807832 *
    fcsModel_DW.DiscreteTransferFcn_states_d[0];

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant1'

  if (resetIntegrator) {
    rlim = -1.0;
  }

  rtb_DiscreteTransferFcn_e[0] = rlim;

  // Outport: '<Root>/actuatorsCmds' incorporates:
  //   Switch: '<S1>/Switch'

  fcsModel_Y.actuatorsCmds[0] = rlim;

  // Saturate: '<S1>/Saturation' incorporates:
  //   DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
  //   Product: '<S1>/Matrix Multiply'

  if (rlim_0 > 792309.908865229) {
    rlim_0 = 792309.908865229;
  } else if (rlim_0 < 0.0) {
    rlim_0 = 0.0;
  }

  // DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
  //   Saturate: '<S1>/Saturation'
  //   Sqrt: '<S1>/Sqrt'
  //   UnitConversion: '<S5>/Unit Conversion'

  rlim_0 = 9.5492965855137211 * std::sqrt(rlim_0) - -0.029612798683843349 *
    fcsModel_DW.DiscreteTransferFcn_states_d[1];
  rlim = 0.48519360065807832 * rlim_0 + 0.48519360065807832 *
    fcsModel_DW.DiscreteTransferFcn_states_d[1];

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant1'

  if (resetIntegrator) {
    rlim = -1.0;
  }

  rtb_DiscreteTransferFcn_e[1] = rlim;

  // Outport: '<Root>/actuatorsCmds' incorporates:
  //   Switch: '<S1>/Switch'

  fcsModel_Y.actuatorsCmds[1] = rlim;

  // Saturate: '<S1>/Saturation' incorporates:
  //   DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
  //   Product: '<S1>/Matrix Multiply'

  if (rlim_1 > 792309.908865229) {
    rlim_1 = 792309.908865229;
  } else if (rlim_1 < 0.0) {
    rlim_1 = 0.0;
  }

  // DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
  //   Saturate: '<S1>/Saturation'
  //   Sqrt: '<S1>/Sqrt'
  //   UnitConversion: '<S5>/Unit Conversion'

  rlim_1 = 9.5492965855137211 * std::sqrt(rlim_1) - -0.029612798683843349 *
    fcsModel_DW.DiscreteTransferFcn_states_d[2];
  rlim = 0.48519360065807832 * rlim_1 + 0.48519360065807832 *
    fcsModel_DW.DiscreteTransferFcn_states_d[2];

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant1'

  if (resetIntegrator) {
    rlim = -1.0;
  }

  rtb_DiscreteTransferFcn_e[2] = rlim;

  // Outport: '<Root>/actuatorsCmds' incorporates:
  //   Switch: '<S1>/Switch'

  fcsModel_Y.actuatorsCmds[2] = rlim;

  // Saturate: '<S1>/Saturation' incorporates:
  //   DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
  //   Product: '<S1>/Matrix Multiply'

  if (rlim_2 > 792309.908865229) {
    rlim_2 = 792309.908865229;
  } else if (rlim_2 < 0.0) {
    rlim_2 = 0.0;
  }

  // DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn' incorporates:
  //   Saturate: '<S1>/Saturation'
  //   Sqrt: '<S1>/Sqrt'
  //   UnitConversion: '<S5>/Unit Conversion'

  rlim_2 = 9.5492965855137211 * std::sqrt(rlim_2) - -0.029612798683843349 *
    fcsModel_DW.DiscreteTransferFcn_states_d[3];
  rlim = 0.48519360065807832 * rlim_2 + 0.48519360065807832 *
    fcsModel_DW.DiscreteTransferFcn_states_d[3];

  // Switch: '<S1>/Switch' incorporates:
  //   Constant: '<S1>/Constant1'

  if (resetIntegrator) {
    rlim = -1.0;
  }

  rtb_DiscreteTransferFcn_e[3] = rlim;

  // Outport: '<Root>/actuatorsCmds' incorporates:
  //   Switch: '<S1>/Switch'

  fcsModel_Y.actuatorsCmds[3] = rlim;

  // Outputs for Iterator SubSystem: '<S1>/For Each Subsystem' incorporates:
  //   ForEach: '<S7>/For Each'

  // ForEachSliceSelector generated from: '<S7>/propellerSpdCmds_rpm' incorporates:
  //   Switch: '<S1>/Switch'

  rlim = rtb_DiscreteTransferFcn_e[0];

  // Saturate: '<S7>/Saturation'
  if (rlim > 9325.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 9325.0;
  } else if (rlim < 2250.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 2250.0;
  }

  // PreLookup: '<S7>/Prelookup'
  rtb_Prelookup_o1 = plook_bincpag(rlim,
    &fcsModel_ConstP.Prelookup_BreakpointsData[0], 14U, &rlim,
    &fcsModel_DW.CoreSubsys[0].Prelookup_DWORK1);

  // ForEachSliceAssignment generated from: '<S7>/mtrPwmCmds' incorporates:
  //   Interpolation_n-D: '<S7>/Interpolation Using Prelookup'

  rtb_ImpAsg_InsertedFor_mtrPwmCm[0] = intrp1d_la(rtb_Prelookup_o1, rlim,
    &fcsModel_ConstP.InterpolationUsingPrelookup_Tab[0], 14U);

  // ForEachSliceSelector generated from: '<S7>/propellerSpdCmds_rpm' incorporates:
  //   Switch: '<S1>/Switch'

  rlim = rtb_DiscreteTransferFcn_e[1];

  // Saturate: '<S7>/Saturation'
  if (rlim > 9325.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 9325.0;
  } else if (rlim < 2250.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 2250.0;
  }

  // PreLookup: '<S7>/Prelookup'
  rtb_Prelookup_o1 = plook_bincpag(rlim,
    &fcsModel_ConstP.Prelookup_BreakpointsData[0], 14U, &rlim,
    &fcsModel_DW.CoreSubsys[1].Prelookup_DWORK1);

  // ForEachSliceAssignment generated from: '<S7>/mtrPwmCmds' incorporates:
  //   Interpolation_n-D: '<S7>/Interpolation Using Prelookup'

  rtb_ImpAsg_InsertedFor_mtrPwmCm[1] = intrp1d_la(rtb_Prelookup_o1, rlim,
    &fcsModel_ConstP.InterpolationUsingPrelookup_Tab[0], 14U);

  // ForEachSliceSelector generated from: '<S7>/propellerSpdCmds_rpm' incorporates:
  //   Switch: '<S1>/Switch'

  rlim = rtb_DiscreteTransferFcn_e[2];

  // Saturate: '<S7>/Saturation'
  if (rlim > 9325.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 9325.0;
  } else if (rlim < 2250.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 2250.0;
  }

  // PreLookup: '<S7>/Prelookup'
  rtb_Prelookup_o1 = plook_bincpag(rlim,
    &fcsModel_ConstP.Prelookup_BreakpointsData[0], 14U, &rlim,
    &fcsModel_DW.CoreSubsys[2].Prelookup_DWORK1);

  // ForEachSliceAssignment generated from: '<S7>/mtrPwmCmds' incorporates:
  //   Interpolation_n-D: '<S7>/Interpolation Using Prelookup'

  rtb_ImpAsg_InsertedFor_mtrPwmCm[2] = intrp1d_la(rtb_Prelookup_o1, rlim,
    &fcsModel_ConstP.InterpolationUsingPrelookup_Tab[0], 14U);

  // ForEachSliceSelector generated from: '<S7>/propellerSpdCmds_rpm' incorporates:
  //   Switch: '<S1>/Switch'

  rlim = rtb_DiscreteTransferFcn_e[3];

  // Saturate: '<S7>/Saturation'
  if (rlim > 9325.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 9325.0;
  } else if (rlim < 2250.0) {
    // PreLookup: '<S7>/Prelookup'
    rlim = 2250.0;
  }

  // PreLookup: '<S7>/Prelookup'
  rtb_Prelookup_o1 = plook_bincpag(rlim,
    &fcsModel_ConstP.Prelookup_BreakpointsData[0], 14U, &rlim,
    &fcsModel_DW.CoreSubsys[3].Prelookup_DWORK1);

  // ForEachSliceAssignment generated from: '<S7>/mtrPwmCmds' incorporates:
  //   Interpolation_n-D: '<S7>/Interpolation Using Prelookup'

  rtb_ImpAsg_InsertedFor_mtrPwmCm[3] = intrp1d_la(rtb_Prelookup_o1, rlim,
    &fcsModel_ConstP.InterpolationUsingPrelookup_Tab[0], 14U);

  // End of Outputs for SubSystem: '<S1>/For Each Subsystem'

  // Outport: '<Root>/actuatorsPwmCmds' incorporates:
  //   ForEachSliceAssignment generated from: '<S7>/mtrPwmCmds'

  fcsModel_Y.actuatorsPwmCmds[0] = rtb_ImpAsg_InsertedFor_mtrPwmCm[0];
  fcsModel_Y.actuatorsPwmCmds[1] = rtb_ImpAsg_InsertedFor_mtrPwmCm[1];
  fcsModel_Y.actuatorsPwmCmds[2] = rtb_ImpAsg_InsertedFor_mtrPwmCm[2];
  fcsModel_Y.actuatorsPwmCmds[3] = rtb_ImpAsg_InsertedFor_mtrPwmCm[3];

  // BusCreator: '<Root>/Bus Creator1'
  fcsModel_Y.fcsDebug.sysIdDebug.chirpTrigger = rtb_chirpTrigger;
  fcsModel_Y.fcsDebug.sysIdDebug.chirpType = rtb_chirpType;
  fcsModel_Y.fcsDebug.sysIdDebug.chirpSignal = vylim;

  // RateTransition: '<Root>/Rate Transition'
  if (tmp) {
    fcsModel_Y.fcsDebug.outerLoopCtrlDebug = fcsModel_DW.RateTransition_Buffer0;
  }

  // BusCreator: '<Root>/Bus Creator' incorporates:
  //   BusCreator: '<S2>/Bus Creator1'
  //   MATLAB Function: '<S4>/Interpret RC In Cmds'
  //   Outport: '<Root>/fcsDebug'
  //   Product: '<S2>/Matrix Multiply'

  fcsModel_Y.fcsDebug.allocDebug.thrustCmd_N =
    fcsModel_DW.Switch2.outerLoopCmds.thrustCmd_N;
  fcsModel_Y.fcsDebug.allocDebug.xMomCmd_Nm = rtb_ImpAsg_InsertedFor_angRat_0;
  fcsModel_Y.fcsDebug.allocDebug.yMomCmd_Nm = rtb_ImpAsg_InsertedFor_angRat_1;
  fcsModel_Y.fcsDebug.allocDebug.zMomCmd_Nm = rtb_ImpAsg_InsertedFor_angRat_2;
  fcsModel_Y.fcsDebug.state = state;
  fcsModel_Y.fcsDebug.flightMode = flightMode;

  // Update for RateTransition: '<Root>/Rate Transition' incorporates:
  //   Abs: '<S168>/Abs'
  //   BusAssignment: '<S168>/Bus Assignment5'
  //   BusCreator: '<S110>/Bus Creator'
  //   BusCreator: '<S111>/Bus Creator'
  //   BusCreator: '<S3>/Bus Creator'
  //   ForEachSliceAssignment generated from: '<S115>/cmd'
  //   ForEachSliceAssignment generated from: '<S169>/filtCmd'
  //   Inport: '<Root>/stateEstimate'
  //
  if (tmp) {
    // Switch: '<S3>/Switch3' incorporates:
    //   Switch: '<S3>/Switch1'

    if (rtb_AND_p || (!rtb_Compare_od)) {
    } else {
      // Switch: '<S3>/Switch1' incorporates:
      //   BusCreator: '<S3>/Bus Creator'

      az = fE;
    }

    // End of Switch: '<S3>/Switch3'

    // Logic: '<S106>/AND' incorporates:
    //   Constant: '<S109>/Constant'
    //   RelationalOperator: '<S109>/Compare'

    rtb_AND_p = (rtb_Compare && (state == enumStateMachine::INFLIGHT));

    // BusCreator: '<S3>/Bus Creator' incorporates:
    //   Abs: '<S168>/Abs1'
    //   DataTypeConversion: '<S168>/Cast To Double'

    rlim = std::abs(rtb_VectorConcatenate1[2]);
    tCmd = rtb_AND_p;

    // Logic: '<S181>/AND' incorporates:
    //   Abs: '<S181>/Abs'
    //   Abs: '<S181>/Abs1'
    //   Constant: '<S182>/Constant'
    //   Constant: '<S183>/Constant'
    //   Constant: '<S184>/Constant'
    //   Constant: '<S185>/Constant'
    //   Inport: '<Root>/stateEstimate'
    //   Logic: '<S181>/OR'
    //   MATLAB Function: '<S4>/Interpret RC In Cmds'
    //   RelationalOperator: '<S182>/Compare'
    //   RelationalOperator: '<S183>/Compare'
    //   RelationalOperator: '<S184>/Compare'
    //   RelationalOperator: '<S185>/Compare'

    rtb_AND_p = (((flightMode == enumFlightMode::ALT_CONTROL) || (flightMode ==
      enumFlightMode::POS_CONTROL)) && (std::abs
      (ExtUPointer_ref_U->stateEstimate.nedVel_mps[2]) < 0.1) && (rlim < 0.5) &&
                 rtb_AND_p);

    // Switch: '<S181>/Switch2' incorporates:
    //   Constant: '<S181>/g1'
    //   Gain: '<S181>/Gain'
    //   Gain: '<S181>/Gain1'
    //   Gain: '<S181>/Gain2'
    //   Product: '<S181>/Divide'
    //   Sum: '<S181>/Sum'
    //   Sum: '<S181>/Sum1'
    //   Switch: '<S181>/Switch'
    //   Switch: '<S181>/Switch1'

    if (rtb_AND_p) {
      DiscreteTransferFcn = (0.10197838058331635 *
        -rtb_ImpAsg_InsertedFor_velCtrlO[2] + DiscreteTransferFcn) * 0.001 +
        0.999 * DiscreteTransferFcn;
    }

    // End of Switch: '<S181>/Switch2'

    // Update for DiscreteTransferFcn: '<S186>/Discrete Transfer Fcn'
    fcsModel_DW.DiscreteTransferFcn_states = fcsModel_DW.DiscreteTransferFcn_tmp;

    // Update for Delay: '<S181>/Delay'
    fcsModel_DW.icLoad = false;
    fcsModel_DW.Delay_DSTATE = DiscreteTransferFcn;
    fcsModel_DW.RateTransition_Buffer0.frcCmd_N = az;
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.cmd[0] =
      rtb_ImpAsg_InsertedFor_filtCmd_[0];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.meas[0] =
      rtb_ImpAsg_InsertedFor_filtMeas[0];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.velCtrlFf[0] =
      rtb_ImpAsg_InsertedFor_velCtrlF[0];
    std::memcpy(&fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.pidDebug[0],
                &rtb_ImpAsg_InsertedFor_pidDebug[0], 3U * sizeof(busPidDebug));
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.cmd[0] =
      rtb_ImpAsg_InsertedFor_cmd_at_i[0];
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.meas[0] =
      rtb_ImpAsg_InsertedFor_meas_at_[0];
    std::memcpy(&fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.pidDebug[0],
                &rtb_ImpAsg_InsertedFor_pidDeb_m[0], 3U * sizeof(busPidDebug));
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.cmd[1] =
      rtb_ImpAsg_InsertedFor_filtCmd_[1];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.meas[1] =
      rtb_ImpAsg_InsertedFor_filtMeas[1];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.velCtrlFf[1] =
      rtb_ImpAsg_InsertedFor_velCtrlF[1];
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.cmd[1] =
      rtb_ImpAsg_InsertedFor_cmd_at_i[1];
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.meas[1] =
      rtb_ImpAsg_InsertedFor_meas_at_[1];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.cmd[2] =
      rtb_ImpAsg_InsertedFor_filtCmd_[2];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.meas[2] =
      rtb_ImpAsg_InsertedFor_filtMeas[2];
    fcsModel_DW.RateTransition_Buffer0.velCtrlDebug.velCtrlFf[2] =
      rtb_ImpAsg_InsertedFor_velCtrlF[2];
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.cmd[2] =
      rtb_ImpAsg_InsertedFor_cmd_at_i[2];
    fcsModel_DW.RateTransition_Buffer0.posCtrlDebug.meas[2] =
      rtb_ImpAsg_InsertedFor_meas_at_[2];
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.cmd = std::abs
      (ExtUPointer_ref_U->stateEstimate.nedVel_mps[2]);
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.meas = rlim;
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.pidDebug.output =
      rtb_Product_i;
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.pidDebug.proportionalOutput
      = tCmd;
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.pidDebug.integralOutput =
      0.0;
    fcsModel_DW.RateTransition_Buffer0.zAccelCtrlDebug.pidDebug.derivativeOutput
      = 0.0;
    fcsModel_DW.RateTransition_Buffer0.xyBodyAccelCtrlDebug =
      fcsModel_rtZbusXyBodyAccelCtrIDebug;
  }

  // Update for UnitDelay: '<S4>/Unit Delay'
  fcsModel_DW.UnitDelay_DSTATE_g = rtb_chirpTrigger;

  // Update for UnitDelay: '<S15>/Unit Delay1'
  fcsModel_DW.UnitDelay1_DSTATE = ylim;

  // Update for UnitDelay: '<S15>/Unit Delay'
  fcsModel_DW.UnitDelay_DSTATE = plim;

  // Update for RandomNumber: '<S16>/White Noise'
  fcsModel_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw(&fcsModel_DW.RandSeed);

  // Update for DiscreteTransferFcn: '<S15>/Discrete Transfer Fcn'
  fcsModel_DW.DiscreteTransferFcn_states_c = yCmd;

  // Update for DiscreteTransferFcn: '<S1>/Discrete Transfer Fcn'
  fcsModel_DW.DiscreteTransferFcn_states_d[0] = vxCmd_unitRange;
  fcsModel_DW.DiscreteTransferFcn_states_d[1] = rlim_0;
  fcsModel_DW.DiscreteTransferFcn_states_d[2] = rlim_1;
  fcsModel_DW.DiscreteTransferFcn_states_d[3] = rlim_2;
  rate_scheduler((&fcsModel_M));
}

// Model initialize function
void fcsModel::initialize()
{
  // Registration code
  {
    // external outputs
    fcsModel_Y.fcsDebug = fcsModel_rtZbusFcsDebug;
  }

  // InitializeConditions for Delay: '<S181>/Delay'
  fcsModel_DW.icLoad = true;

  // InitializeConditions for RandomNumber: '<S16>/White Noise'
  fcsModel_DW.RandSeed = 1529675776U;
  fcsModel_DW.NextOutput = rt_nrand_Upu32_Yd_f_pw(&fcsModel_DW.RandSeed);

  // SystemInitialize for Iterator SubSystem: '<S110>/NED Position Control'
  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block'
  // 'interpretRcInputs_function:25' throttle_is_up = false;
  // 'interpretRcInputs_function:26' chirpCount_ = uint8(0);
  // 'holdOutputAtCenter_function:9' last_input = 0;
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[0].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[0].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S115>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_g[0].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S115>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[1].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[1].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S115>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_g[1].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S115>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[2].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S115>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_g[2].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S115>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S115>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_g[2].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S115>/pidWithDebug'
  // End of SystemInitialize for SubSystem: '<S110>/NED Position Control'

  // SystemInitialize for Iterator SubSystem: '<S111>/For Each Subsystem'
  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block'
  // InitializeConditions for Delay: '<S196>/Delay'
  fcsModel_DW.CoreSubsys_i[0].icLoad = true;

  // InitializeConditions for Delay: '<S195>/Delay'
  fcsModel_DW.CoreSubsys_i[0].icLoad_a = true;

  // SystemInitialize for MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' 
  ComputeFilterNumeratorAndD_Init(&fcsModel_DW.CoreSubsys_i[0].num[0],
    &fcsModel_DW.CoreSubsys_i[0].den[0]);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[0].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[0].
    SignalConditioningBlock2);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block2'

  // SystemInitialize for Atomic SubSystem: '<S169>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_i[0].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S169>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block'
  // InitializeConditions for Delay: '<S196>/Delay'
  fcsModel_DW.CoreSubsys_i[1].icLoad = true;

  // InitializeConditions for Delay: '<S195>/Delay'
  fcsModel_DW.CoreSubsys_i[1].icLoad_a = true;

  // SystemInitialize for MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' 
  ComputeFilterNumeratorAndD_Init(&fcsModel_DW.CoreSubsys_i[1].num[0],
    &fcsModel_DW.CoreSubsys_i[1].den[0]);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[1].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[1].
    SignalConditioningBlock2);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block2'

  // SystemInitialize for Atomic SubSystem: '<S169>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_i[1].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S169>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block'
  // InitializeConditions for Delay: '<S196>/Delay'
  fcsModel_DW.CoreSubsys_i[2].icLoad = true;

  // InitializeConditions for Delay: '<S195>/Delay'
  fcsModel_DW.CoreSubsys_i[2].icLoad_a = true;

  // SystemInitialize for MATLAB Function: '<S194>/Compute Filter Numerator And Denominator' 
  ComputeFilterNumeratorAndD_Init(&fcsModel_DW.CoreSubsys_i[2].num[0],
    &fcsModel_DW.CoreSubsys_i[2].den[0]);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block1'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[2].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S169>/Signal Conditioning Block2'
  SignalConditioningBlock1_c_Init(&fcsModel_DW.CoreSubsys_i[2].
    SignalConditioningBlock2);

  // End of SystemInitialize for SubSystem: '<S169>/Signal Conditioning Block2'

  // SystemInitialize for Atomic SubSystem: '<S169>/pidWithDebug'
  fcsModel_pidWithDebug_m_Init(&fcsModel_DW.CoreSubsys_i[2].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S169>/pidWithDebug'
  // End of SystemInitialize for SubSystem: '<S111>/For Each Subsystem'

  // SystemInitialize for Iterator SubSystem: '<S20>/Attitude Control'
  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  // 'holdOutputAtCenter_function:9' last_input = 0;
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[0].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[0].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S62>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_p[0].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S62>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[1].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[1].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S62>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_p[1].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S62>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[2].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S62>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_p[2].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S62>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S62>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_p[2].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S62>/pidWithDebug'
  // End of SystemInitialize for SubSystem: '<S20>/Attitude Control'

  // SystemInitialize for Atomic SubSystem: '<S2>/Angular Rate Controller'
  // SystemInitialize for Iterator SubSystem: '<S18>/For Each Subsystem'
  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[0].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[0].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_a[0].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S21>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[1].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[1].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_a[1].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S21>/pidWithDebug'

  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[2].
    SignalConditioningBlock);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block'

  // SystemInitialize for Atomic SubSystem: '<S21>/Signal Conditioning Block1'
  f_SignalConditioningBlock1_Init(&fcsModel_DW.CoreSubsys_a[2].
    SignalConditioningBlock1);

  // End of SystemInitialize for SubSystem: '<S21>/Signal Conditioning Block1'

  // SystemInitialize for Atomic SubSystem: '<S21>/pidWithDebug'
  fcsModel_pidWithDebug_Init(&fcsModel_DW.CoreSubsys_a[2].pidWithDebug);

  // End of SystemInitialize for SubSystem: '<S21>/pidWithDebug'
  // End of SystemInitialize for SubSystem: '<S18>/For Each Subsystem'
  // End of SystemInitialize for SubSystem: '<S2>/Angular Rate Controller'
}

// Model terminate function
void fcsModel::terminate()
{
  // (no terminate code required)
}

// Constructor
fcsModel::fcsModel(ExtUPointer_fcsModel_T *fcsModel_ExtUPointer,InstP_fcsModel_T
                   *fcsModel_InstP) :
  fcsModel_Y(),
  fcsModel_DW(),
  ExtUPointer_ref_U(),
  fcsModel_InstP_ref(),
  fcsModel_M()
{
  ExtUPointer_ref_U = fcsModel_ExtUPointer;
  fcsModel_InstP_ref = fcsModel_InstP;
}

// Destructor
// Currently there is no destructor body generated.
fcsModel::~fcsModel() = default;

// Real-Time Model get method
fcsModel::RT_MODEL_fcsModel_T * fcsModel::getRTM()
{
  return (&fcsModel_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
