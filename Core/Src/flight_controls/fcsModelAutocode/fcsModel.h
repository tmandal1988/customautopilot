//
// File: fcsModel.h
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 1.117
// Simulink Coder version         : 9.7 (R2022a) 13-Nov-2021
// C/C++ source code generated on : Sat Jun 14 09:55:15 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#ifndef RTW_HEADER_fcsModel_h_
#define RTW_HEADER_fcsModel_h_
#include "rtwtypes.h"
#include "fcsModel_types.h"
#include <array>

// External data declarations for dependent source files
extern const busXyBodyAccelCtrIDebug fcsModel_rtZbusXyBodyAccelCtrIDebug;// busXyBodyAccelCtrIDebug ground 
extern const busFcsDebug fcsModel_rtZbusFcsDebug;// busFcsDebug ground

// Class declaration for model fcsModel
class fcsModel final
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<S1>/For Each Subsystem' 
  struct DW_CoreSubsys_fcsModel_T {
    uint32_T Prelookup_DWORK1;         // '<S7>/Prelookup'
  };

  // Block signals and states (default storage) for system '<S22>/Discrete First Order Deriv Filter' 
  struct DW_DiscreteFirstOrderDerivFil_T {
    std::array<real_T, 2> num;
                      // '<S53>/Compute Deriv Filter Numerator And Denominator'
    std::array<real_T, 2> den;
                      // '<S53>/Compute Deriv Filter Numerator And Denominator'
    real_T DiscreteTransferFcn_states; // '<S53>/Discrete Transfer Fcn'
  };

  // Block signals and states (default storage) for system '<S19>/pidWithDebug'
  struct DW_pidWithDebug_fcsModel_T {
    DW_DiscreteFirstOrderDerivFil_T DiscreteFirstOrderDerivFilter;
                                   // '<S22>/Discrete First Order Deriv Filter'
    real_T DiscreteTimeIntegrator_DSTATE;// '<S22>/Discrete-Time Integrator'
    real_T DelayInput2_DSTATE;         // '<S54>/Delay Input2'
    real_T UnitDelay_DSTATE;           // '<S22>/Unit Delay'
    real_T UnitDelay1_DSTATE;          // '<S22>/Unit Delay1'
    int8_T DiscreteTimeIntegrator_PrevRese;// '<S22>/Discrete-Time Integrator'
    uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S22>/Discrete-Time Integrator'
  };

  // Block signals and states (default storage) for system '<S19>/Signal Conditioning Block1' 
  struct DW_SignalConditioningBlock1_f_T {
    std::array<real_T, 3> num;
                            // '<S39>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> den;
                            // '<S39>/Compute Filter Numerator And Denominator'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S39>/Discrete Transfer Fcn' 
    real_T DelayInput2_DSTATE;         // '<S40>/Delay Input2'
    real_T DiscreteTransferFcn_tmp;    // '<S39>/Discrete Transfer Fcn'
  };

  // Block signals and states (default storage) for system '<S16>/For Each Subsystem' 
  struct DW_CoreSubsys_fcsModel_c_T {
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock;// '<S19>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock1;// '<S19>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_T pidWithDebug;// '<S19>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S19>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S18>/Attitude Control' 
  struct DW_CoreSubsys_fcsModel_i_T {
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock;// '<S60>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock1;// '<S60>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_T pidWithDebug;// '<S60>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S60>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S112>/holdOutputAtCenter1' 
  struct DW_holdOutputAtCenter1_fcsMod_T {
    real_T last_input;                 // '<S122>/holdOutputAtCenter'
  };

  // Block signals and states (default storage) for system '<S113>/pidWithDebug' 
  struct DW_pidWithDebug_fcsModel_i_T {
    DW_DiscreteFirstOrderDerivFil_T DiscreteFirstOrderDerivFilter;
                                  // '<S129>/Discrete First Order Deriv Filter'
    real_T DiscreteTimeIntegrator_DSTATE;// '<S129>/Discrete-Time Integrator'
    real_T DelayInput2_DSTATE;         // '<S161>/Delay Input2'
    real_T UnitDelay_DSTATE;           // '<S129>/Unit Delay'
    real_T UnitDelay1_DSTATE;          // '<S129>/Unit Delay1'
    int8_T DiscreteTimeIntegrator_PrevRese;// '<S129>/Discrete-Time Integrator'
    uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S129>/Discrete-Time Integrator' 
  };

  // Block signals and states (default storage) for system '<S113>/Signal Conditioning Block1' 
  struct DW_SignalConditioningBlock1_g_T {
    std::array<real_T, 3> num;
                           // '<S146>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> den;
                           // '<S146>/Compute Filter Numerator And Denominator'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S146>/Discrete Transfer Fcn' 
    real_T DelayInput2_DSTATE;         // '<S147>/Delay Input2'
    real_T DiscreteTransferFcn_tmp;    // '<S146>/Discrete Transfer Fcn'
  };

  // Block signals and states (default storage) for system '<S108>/NED Position Control' 
  struct DW_CoreSubsys_fcsModel_b_T {
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock;// '<S113>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock1;// '<S113>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_i_T pidWithDebug;// '<S113>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S113>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S109>/For Each Subsystem' 
  struct DW_CoreSubsys_fcsModel_p_T {
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock1;// '<S166>/Signal Conditioning Block1' 
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock2;// '<S166>/Signal Conditioning Block2' 
    DW_pidWithDebug_fcsModel_i_T pidWithDebug;// '<S166>/pidWithDebug'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S189>/Discrete Transfer Fcn' 
    std::array<real_T, 2> DiscreteTransferFcn_states_h;// '<S190>/Discrete Transfer Fcn' 
    real_T UnitDelay_DSTATE;           // '<S166>/Unit Delay'
    real_T DelayInput2_DSTATE;         // '<S192>/Delay Input2'
    real_T DelayInput2_DSTATE_k;       // '<S191>/Delay Input2'
    real_T DiscreteTransferFcn_tmp;    // '<S189>/Discrete Transfer Fcn'
    real_T DiscreteTransferFcn_tmp_l;  // '<S190>/Discrete Transfer Fcn'
  };

  // Block signals and states (default storage) for system '<Root>'
  struct DW_fcsModel_T {
    std::array<DW_CoreSubsys_fcsModel_p_T, 3> CoreSubsys_i;// '<S109>/For Each Subsystem' 
    std::array<DW_CoreSubsys_fcsModel_b_T, 3> CoreSubsys_g;// '<S108>/NED Position Control' 
    DW_holdOutputAtCenter1_fcsMod_T holdOutputAtCenter2;// '<S112>/holdOutputAtCenter2' 
    DW_holdOutputAtCenter1_fcsMod_T holdOutputAtCenter1;// '<S112>/holdOutputAtCenter1' 
    std::array<DW_CoreSubsys_fcsModel_i_T, 3> CoreSubsys_p;// '<S18>/Attitude Control' 
    std::array<DW_CoreSubsys_fcsModel_c_T, 3> CoreSubsys_a;// '<S16>/For Each Subsystem' 
    std::array<DW_CoreSubsys_fcsModel_T, 4> CoreSubsys;// '<S1>/For Each Subsystem' 
    busOuterLoopCtrlDebug RateTransition_Buffer0;// '<Root>/Rate Transition'
    busOuterLoopToInnerLoop Switch2;   // '<S3>/Switch2'
    busRcOutCmds rcOutCmds;            // '<S4>/Interpret RC In Cmds'
    std::array<real_T, 4> DiscreteTransferFcn_states_d;// '<S1>/Discrete Transfer Fcn' 
    real_T DiscreteTransferFcn_states; // '<S182>/Discrete Transfer Fcn'
    real_T Delay_DSTATE;               // '<S177>/Delay'
    real_T DiscreteTransferFcn_tmp;    // '<S182>/Discrete Transfer Fcn'
    real_T last_input;                 // '<S171>/holdOutputAtCenter'
    real_T last_input_c;               // '<S121>/holdOutputAtCenter'
    int32_T durationCounter_1;         // '<S4>/Chart'
    int32_T durationCounter_1_j;       // '<S4>/Chart'
    enumChirpTrigger UnitDelay_DSTATE; // '<S4>/Unit Delay'
    uint16_T temporalCounter_i1;       // '<S4>/Chart'
    uint8_T chirpCount_;               // '<S4>/Interpret RC In Cmds'
    uint8_T is_active_c1_rcInterpreter;// '<S4>/Chart'
    uint8_T is_c1_rcInterpreter;       // '<S4>/Chart'
    boolean_T icLoad;                  // '<S177>/Delay'
    boolean_T throttle_is_up;          // '<S4>/Interpret RC In Cmds'
    boolean_T rcCheckFlag;             // '<S4>/Chart'
  };

  // Constant parameters (default storage)
  struct ConstP_fcsModel_T {
    // Pooled Parameter (Mixed Expressions)
    //  Referenced by:
    //    '<S3>/Constant'
    //    '<S165>/Constant'

    busOuterLoopToInnerLoop pooled3;

    // Expression: rpmToPwmLut(:, 1)
    //  Referenced by: '<S7>/Prelookup'

    std::array<real_T, 15> Prelookup_BreakpointsData;

    // Expression: rpmToPwmLut(:, 2)
    //  Referenced by: '<S7>/Interpolation Using Prelookup'

    std::array<real_T, 15> InterpolationUsingPrelookup_Tab;

    // Expression: allocationDataStruct.allocationMatrix
    //  Referenced by: '<S1>/Constant'

    std::array<real_T, 16> Constant_Value_cu;

    // Expression: vehicleConstants.inertia_kgm2
    //  Referenced by: '<S2>/Constant'

    std::array<real_T, 9> Constant_Value_n;

    // Expression: [0; 0; g_mps2]
    //  Referenced by: '<S109>/Constant'

    std::array<real_T, 3> Constant_Value_h;

    // Computed Parameter: Constant_Value_e
    //  Referenced by: '<S18>/Constant'

    std::array<uint8_T, 3> Constant_Value_e;
  };

  // External inputs (root inport signals with default storage)
  struct ExtU_fcsModel_T {
    busRcInCmds rcCmdsIn;              // '<Root>/rcCmdsIn'
    busStateEstimate stateEstimate;    // '<Root>/stateEstimate'
    busFcsParams ctrlParams;           // '<Root>/ctrlParams'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY_fcsModel_T {
    std::array<real_T, 4> actuatorsCmds;// '<Root>/actuatorsCmds'
    busFcsDebug fcsDebug;              // '<Root>/fcsDebug'
    std::array<real_T, 4> actuatorsPwmCmds;// '<Root>/actuatorsPwmCmds'
  };

  // Real-time Model Data Structure
  struct RT_MODEL_fcsModel_T {
    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      struct {
        uint8_T TID[2];
      } TaskCounters;
    } Timing;
  };

  // Copy Constructor
  fcsModel(fcsModel const&) = delete;

  // Assignment Operator
  fcsModel& operator= (fcsModel const&) & = delete;

  // Move Constructor
  fcsModel(fcsModel &&) = delete;

  // Move Assignment Operator
  fcsModel& operator= (fcsModel &&) = delete;

  // Real-Time Model get method
  fcsModel::RT_MODEL_fcsModel_T * getRTM();

  // Root inports set method
  void setExternalInputs(const ExtU_fcsModel_T *pExtU_fcsModel_T)
  {
    fcsModel_U = *pExtU_fcsModel_T;
  }

  // Root outports get method
  const ExtY_fcsModel_T &getExternalOutputs() const
  {
    return fcsModel_Y;
  }

  // model initialize function
  void initialize();
  void ModelExternalOutputInit();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Constructor
  fcsModel();

  // Destructor
  ~fcsModel();

  // private data and function members
 private:
  // External inputs
  ExtU_fcsModel_T fcsModel_U;

  // External outputs
  ExtY_fcsModel_T fcsModel_Y;

  // Block states
  DW_fcsModel_T fcsModel_DW;

  // private member function(s) for subsystem '<S22>/Discrete First Order Deriv Filter'
  static void f_DiscreteFirstOrderDerivFilter(real_T rtu_input, real_T
    rtu_filterBandwidth_radps, real_T *rty_filteredInputRate, real_T
    rtp_sampleTime_s, DW_DiscreteFirstOrderDerivFil_T *localDW);

  // private member function(s) for subsystem '<S19>/pidWithDebug'
  static void fcsModel_pidWithDebug_Init(DW_pidWithDebug_fcsModel_T *localDW);
  static void fcsModel_pidWithDebug(real_T rtu_feedForward, real_T rtu_cmd,
    real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc,
    const busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T
    *rty_ctrlCmd, busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s,
    DW_pidWithDebug_fcsModel_T *localDW);

  // private member function(s) for subsystem '<S38>/Compute Natural Frequency'
  static void fcsMode_ComputeNaturalFrequency(real_T rtu_bandwidth_radps, real_T
    rtu_dampingRatio_nd, real_T *rty_naturalFrequency_radps);

  // private member function(s) for subsystem '<S38>/Compute Numerator And Denominator'
  static void ComputeNumeratorAndDenominator(real_T rtu_naturalFrequency_radps,
    real_T rtu_dampingRatio_nd, real_T rty_rateNum[3], real_T rty_accelNum[3],
    real_T rty_den[3], real_T rtp_sampleTime_s);

  // private member function(s) for subsystem '<S39>/Compute Filter Numerator And Denominator'
  static void ComputeFilterNumeratorAndD_Init(real_T rty_num[3], real_T rty_den
    [3]);
  static void ComputeFilterNumeratorAndDenomi(real_T rtu_naturalFrequency_radps,
    real_T rtu_dampingRatio_nd, real_T rty_num[3], real_T rty_den[3], real_T
    rtp_sampleTime_s);

  // private member function(s) for subsystem '<S19>/Signal Conditioning Block1'
  static void f_SignalConditioningBlock1_Init(DW_SignalConditioningBlock1_f_T
    *localDW);
  static void fcsMod_SignalConditioningBlock1(real_T rtu_input, const
    busSignalConditioningParams *rtu_params, real_T *rty_filteredInput, real_T
    rtp_sampleTime_s, DW_SignalConditioningBlock1_f_T *localDW);

  // private member function(s) for subsystem '<S112>/holdOutputAtCenter1'
  static void fcsModel_holdOutputAtCenter1(real_T rtu_input, real_T rtu_trigger,
    real_T *rty_output, boolean_T *rty_atCenter, DW_holdOutputAtCenter1_fcsMod_T
    *localDW);

  // private member function(s) for subsystem '<S113>/pidWithDebug'
  static void fcsModel_pidWithDebug_m_Init(DW_pidWithDebug_fcsModel_i_T *localDW);
  static void fcsModel_pidWithDebug_j(real_T rtu_feedForward, real_T rtu_cmd,
    real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc,
    const busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T
    *rty_ctrlCmd, busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s,
    DW_pidWithDebug_fcsModel_i_T *localDW);

  // private member function(s) for subsystem '<S113>/Signal Conditioning Block1'
  static void SignalConditioningBlock1_c_Init(DW_SignalConditioningBlock1_g_T
    *localDW);
  static void fcsM_SignalConditioningBlock1_f(real_T rtu_input, const
    busSignalConditioningParams *rtu_params, real_T *rty_filteredInput, real_T
    rtp_sampleTime_s, DW_SignalConditioningBlock1_g_T *localDW);

  // private member function(s) for subsystem '<Root>'
  boolean_T fcsModel_checkRcCmds(const busRcInCmds
    *BusConversion_InsertedFor_Chart);

  // Real-Time Model
  RT_MODEL_fcsModel_T fcsModel_M;
};

// Constant parameters (default storage)
extern const fcsModel::ConstP_fcsModel_T fcsModel_ConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S9>/Compare' : Unused code path elimination
//  Block '<S9>/Constant' : Unused code path elimination
//  Block '<S8>/Constant' : Unused code path elimination
//  Block '<S8>/Transpose' : Unused code path elimination
//  Block '<S8>/Transpose1' : Unused code path elimination
//  Block '<S23>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S23>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S25>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S35>/Data Type Duplicate' : Unused code path elimination
//  Block '<S35>/Data Type Propagation' : Unused code path elimination
//  Block '<S26>/Delay Input2' : Unused code path elimination
//  Block '<S26>/Difference Inputs1' : Unused code path elimination
//  Block '<S26>/Difference Inputs2' : Unused code path elimination
//  Block '<S26>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S36>/Data Type Duplicate' : Unused code path elimination
//  Block '<S36>/Data Type Propagation' : Unused code path elimination
//  Block '<S36>/LowerRelop1' : Unused code path elimination
//  Block '<S36>/Switch' : Unused code path elimination
//  Block '<S36>/Switch2' : Unused code path elimination
//  Block '<S36>/UpperRelop' : Unused code path elimination
//  Block '<S26>/delta fall limit' : Unused code path elimination
//  Block '<S26>/delta rise limit' : Unused code path elimination
//  Block '<S26>/sample time' : Unused code path elimination
//  Block '<S27>/Delay Input2' : Unused code path elimination
//  Block '<S27>/Difference Inputs1' : Unused code path elimination
//  Block '<S27>/Difference Inputs2' : Unused code path elimination
//  Block '<S27>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S37>/Data Type Duplicate' : Unused code path elimination
//  Block '<S37>/Data Type Propagation' : Unused code path elimination
//  Block '<S37>/LowerRelop1' : Unused code path elimination
//  Block '<S37>/Switch' : Unused code path elimination
//  Block '<S37>/Switch2' : Unused code path elimination
//  Block '<S37>/UpperRelop' : Unused code path elimination
//  Block '<S27>/delta fall limit' : Unused code path elimination
//  Block '<S27>/delta rise limit' : Unused code path elimination
//  Block '<S27>/sample time' : Unused code path elimination
//  Block '<S28>/Data Type Duplicate' : Unused code path elimination
//  Block '<S28>/Data Type Propagation' : Unused code path elimination
//  Block '<S29>/Data Type Duplicate' : Unused code path elimination
//  Block '<S29>/Data Type Propagation' : Unused code path elimination
//  Block '<S29>/LowerRelop1' : Unused code path elimination
//  Block '<S29>/Switch' : Unused code path elimination
//  Block '<S29>/Switch2' : Unused code path elimination
//  Block '<S29>/UpperRelop' : Unused code path elimination
//  Block '<S30>/Data Type Duplicate' : Unused code path elimination
//  Block '<S30>/Data Type Propagation' : Unused code path elimination
//  Block '<S30>/LowerRelop1' : Unused code path elimination
//  Block '<S30>/Switch' : Unused code path elimination
//  Block '<S30>/Switch2' : Unused code path elimination
//  Block '<S30>/UpperRelop' : Unused code path elimination
//  Block '<S38>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S38>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S40>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S50>/Data Type Duplicate' : Unused code path elimination
//  Block '<S50>/Data Type Propagation' : Unused code path elimination
//  Block '<S41>/Delay Input2' : Unused code path elimination
//  Block '<S41>/Difference Inputs1' : Unused code path elimination
//  Block '<S41>/Difference Inputs2' : Unused code path elimination
//  Block '<S41>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S51>/Data Type Duplicate' : Unused code path elimination
//  Block '<S51>/Data Type Propagation' : Unused code path elimination
//  Block '<S51>/LowerRelop1' : Unused code path elimination
//  Block '<S51>/Switch' : Unused code path elimination
//  Block '<S51>/Switch2' : Unused code path elimination
//  Block '<S51>/UpperRelop' : Unused code path elimination
//  Block '<S41>/delta fall limit' : Unused code path elimination
//  Block '<S41>/delta rise limit' : Unused code path elimination
//  Block '<S41>/sample time' : Unused code path elimination
//  Block '<S42>/Delay Input2' : Unused code path elimination
//  Block '<S42>/Difference Inputs1' : Unused code path elimination
//  Block '<S42>/Difference Inputs2' : Unused code path elimination
//  Block '<S42>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S52>/Data Type Duplicate' : Unused code path elimination
//  Block '<S52>/Data Type Propagation' : Unused code path elimination
//  Block '<S52>/LowerRelop1' : Unused code path elimination
//  Block '<S52>/Switch' : Unused code path elimination
//  Block '<S52>/Switch2' : Unused code path elimination
//  Block '<S52>/UpperRelop' : Unused code path elimination
//  Block '<S42>/delta fall limit' : Unused code path elimination
//  Block '<S42>/delta rise limit' : Unused code path elimination
//  Block '<S42>/sample time' : Unused code path elimination
//  Block '<S43>/Data Type Duplicate' : Unused code path elimination
//  Block '<S43>/Data Type Propagation' : Unused code path elimination
//  Block '<S44>/Data Type Duplicate' : Unused code path elimination
//  Block '<S44>/Data Type Propagation' : Unused code path elimination
//  Block '<S44>/LowerRelop1' : Unused code path elimination
//  Block '<S44>/Switch' : Unused code path elimination
//  Block '<S44>/Switch2' : Unused code path elimination
//  Block '<S44>/UpperRelop' : Unused code path elimination
//  Block '<S45>/Data Type Duplicate' : Unused code path elimination
//  Block '<S45>/Data Type Propagation' : Unused code path elimination
//  Block '<S45>/LowerRelop1' : Unused code path elimination
//  Block '<S45>/Switch' : Unused code path elimination
//  Block '<S45>/Switch2' : Unused code path elimination
//  Block '<S45>/UpperRelop' : Unused code path elimination
//  Block '<S54>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S57>/Data Type Duplicate' : Unused code path elimination
//  Block '<S57>/Data Type Propagation' : Unused code path elimination
//  Block '<S55>/Data Type Duplicate' : Unused code path elimination
//  Block '<S55>/Data Type Propagation' : Unused code path elimination
//  Block '<S67>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S67>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S69>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S79>/Data Type Duplicate' : Unused code path elimination
//  Block '<S79>/Data Type Propagation' : Unused code path elimination
//  Block '<S70>/Delay Input2' : Unused code path elimination
//  Block '<S70>/Difference Inputs1' : Unused code path elimination
//  Block '<S70>/Difference Inputs2' : Unused code path elimination
//  Block '<S70>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S80>/Data Type Duplicate' : Unused code path elimination
//  Block '<S80>/Data Type Propagation' : Unused code path elimination
//  Block '<S80>/LowerRelop1' : Unused code path elimination
//  Block '<S80>/Switch' : Unused code path elimination
//  Block '<S80>/Switch2' : Unused code path elimination
//  Block '<S80>/UpperRelop' : Unused code path elimination
//  Block '<S70>/delta fall limit' : Unused code path elimination
//  Block '<S70>/delta rise limit' : Unused code path elimination
//  Block '<S70>/sample time' : Unused code path elimination
//  Block '<S71>/Delay Input2' : Unused code path elimination
//  Block '<S71>/Difference Inputs1' : Unused code path elimination
//  Block '<S71>/Difference Inputs2' : Unused code path elimination
//  Block '<S71>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S81>/Data Type Duplicate' : Unused code path elimination
//  Block '<S81>/Data Type Propagation' : Unused code path elimination
//  Block '<S81>/LowerRelop1' : Unused code path elimination
//  Block '<S81>/Switch' : Unused code path elimination
//  Block '<S81>/Switch2' : Unused code path elimination
//  Block '<S81>/UpperRelop' : Unused code path elimination
//  Block '<S71>/delta fall limit' : Unused code path elimination
//  Block '<S71>/delta rise limit' : Unused code path elimination
//  Block '<S71>/sample time' : Unused code path elimination
//  Block '<S72>/Data Type Duplicate' : Unused code path elimination
//  Block '<S72>/Data Type Propagation' : Unused code path elimination
//  Block '<S73>/Data Type Duplicate' : Unused code path elimination
//  Block '<S73>/Data Type Propagation' : Unused code path elimination
//  Block '<S73>/LowerRelop1' : Unused code path elimination
//  Block '<S73>/Switch' : Unused code path elimination
//  Block '<S73>/Switch2' : Unused code path elimination
//  Block '<S73>/UpperRelop' : Unused code path elimination
//  Block '<S74>/Data Type Duplicate' : Unused code path elimination
//  Block '<S74>/Data Type Propagation' : Unused code path elimination
//  Block '<S74>/LowerRelop1' : Unused code path elimination
//  Block '<S74>/Switch' : Unused code path elimination
//  Block '<S74>/Switch2' : Unused code path elimination
//  Block '<S74>/UpperRelop' : Unused code path elimination
//  Block '<S82>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S82>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S84>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S94>/Data Type Duplicate' : Unused code path elimination
//  Block '<S94>/Data Type Propagation' : Unused code path elimination
//  Block '<S85>/Delay Input2' : Unused code path elimination
//  Block '<S85>/Difference Inputs1' : Unused code path elimination
//  Block '<S85>/Difference Inputs2' : Unused code path elimination
//  Block '<S85>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S95>/Data Type Duplicate' : Unused code path elimination
//  Block '<S95>/Data Type Propagation' : Unused code path elimination
//  Block '<S95>/LowerRelop1' : Unused code path elimination
//  Block '<S95>/Switch' : Unused code path elimination
//  Block '<S95>/Switch2' : Unused code path elimination
//  Block '<S95>/UpperRelop' : Unused code path elimination
//  Block '<S85>/delta fall limit' : Unused code path elimination
//  Block '<S85>/delta rise limit' : Unused code path elimination
//  Block '<S85>/sample time' : Unused code path elimination
//  Block '<S86>/Delay Input2' : Unused code path elimination
//  Block '<S86>/Difference Inputs1' : Unused code path elimination
//  Block '<S86>/Difference Inputs2' : Unused code path elimination
//  Block '<S86>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S96>/Data Type Duplicate' : Unused code path elimination
//  Block '<S96>/Data Type Propagation' : Unused code path elimination
//  Block '<S96>/LowerRelop1' : Unused code path elimination
//  Block '<S96>/Switch' : Unused code path elimination
//  Block '<S96>/Switch2' : Unused code path elimination
//  Block '<S96>/UpperRelop' : Unused code path elimination
//  Block '<S86>/delta fall limit' : Unused code path elimination
//  Block '<S86>/delta rise limit' : Unused code path elimination
//  Block '<S86>/sample time' : Unused code path elimination
//  Block '<S87>/Data Type Duplicate' : Unused code path elimination
//  Block '<S87>/Data Type Propagation' : Unused code path elimination
//  Block '<S88>/Data Type Duplicate' : Unused code path elimination
//  Block '<S88>/Data Type Propagation' : Unused code path elimination
//  Block '<S88>/LowerRelop1' : Unused code path elimination
//  Block '<S88>/Switch' : Unused code path elimination
//  Block '<S88>/Switch2' : Unused code path elimination
//  Block '<S88>/UpperRelop' : Unused code path elimination
//  Block '<S89>/Data Type Duplicate' : Unused code path elimination
//  Block '<S89>/Data Type Propagation' : Unused code path elimination
//  Block '<S89>/LowerRelop1' : Unused code path elimination
//  Block '<S89>/Switch' : Unused code path elimination
//  Block '<S89>/Switch2' : Unused code path elimination
//  Block '<S89>/UpperRelop' : Unused code path elimination
//  Block '<S98>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S101>/Data Type Duplicate' : Unused code path elimination
//  Block '<S101>/Data Type Propagation' : Unused code path elimination
//  Block '<S99>/Data Type Duplicate' : Unused code path elimination
//  Block '<S99>/Data Type Propagation' : Unused code path elimination
//  Block '<S130>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S130>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S132>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S142>/Data Type Duplicate' : Unused code path elimination
//  Block '<S142>/Data Type Propagation' : Unused code path elimination
//  Block '<S133>/Delay Input2' : Unused code path elimination
//  Block '<S133>/Difference Inputs1' : Unused code path elimination
//  Block '<S133>/Difference Inputs2' : Unused code path elimination
//  Block '<S133>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S143>/Data Type Duplicate' : Unused code path elimination
//  Block '<S143>/Data Type Propagation' : Unused code path elimination
//  Block '<S143>/LowerRelop1' : Unused code path elimination
//  Block '<S143>/Switch' : Unused code path elimination
//  Block '<S143>/Switch2' : Unused code path elimination
//  Block '<S143>/UpperRelop' : Unused code path elimination
//  Block '<S133>/delta fall limit' : Unused code path elimination
//  Block '<S133>/delta rise limit' : Unused code path elimination
//  Block '<S133>/sample time' : Unused code path elimination
//  Block '<S134>/Delay Input2' : Unused code path elimination
//  Block '<S134>/Difference Inputs1' : Unused code path elimination
//  Block '<S134>/Difference Inputs2' : Unused code path elimination
//  Block '<S134>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S144>/Data Type Duplicate' : Unused code path elimination
//  Block '<S144>/Data Type Propagation' : Unused code path elimination
//  Block '<S144>/LowerRelop1' : Unused code path elimination
//  Block '<S144>/Switch' : Unused code path elimination
//  Block '<S144>/Switch2' : Unused code path elimination
//  Block '<S144>/UpperRelop' : Unused code path elimination
//  Block '<S134>/delta fall limit' : Unused code path elimination
//  Block '<S134>/delta rise limit' : Unused code path elimination
//  Block '<S134>/sample time' : Unused code path elimination
//  Block '<S135>/Data Type Duplicate' : Unused code path elimination
//  Block '<S135>/Data Type Propagation' : Unused code path elimination
//  Block '<S136>/Data Type Duplicate' : Unused code path elimination
//  Block '<S136>/Data Type Propagation' : Unused code path elimination
//  Block '<S136>/LowerRelop1' : Unused code path elimination
//  Block '<S136>/Switch' : Unused code path elimination
//  Block '<S136>/Switch2' : Unused code path elimination
//  Block '<S136>/UpperRelop' : Unused code path elimination
//  Block '<S137>/Data Type Duplicate' : Unused code path elimination
//  Block '<S137>/Data Type Propagation' : Unused code path elimination
//  Block '<S137>/LowerRelop1' : Unused code path elimination
//  Block '<S137>/Switch' : Unused code path elimination
//  Block '<S137>/Switch2' : Unused code path elimination
//  Block '<S137>/UpperRelop' : Unused code path elimination
//  Block '<S145>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S145>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S147>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S157>/Data Type Duplicate' : Unused code path elimination
//  Block '<S157>/Data Type Propagation' : Unused code path elimination
//  Block '<S148>/Delay Input2' : Unused code path elimination
//  Block '<S148>/Difference Inputs1' : Unused code path elimination
//  Block '<S148>/Difference Inputs2' : Unused code path elimination
//  Block '<S148>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S158>/Data Type Duplicate' : Unused code path elimination
//  Block '<S158>/Data Type Propagation' : Unused code path elimination
//  Block '<S158>/LowerRelop1' : Unused code path elimination
//  Block '<S158>/Switch' : Unused code path elimination
//  Block '<S158>/Switch2' : Unused code path elimination
//  Block '<S158>/UpperRelop' : Unused code path elimination
//  Block '<S148>/delta fall limit' : Unused code path elimination
//  Block '<S148>/delta rise limit' : Unused code path elimination
//  Block '<S148>/sample time' : Unused code path elimination
//  Block '<S149>/Delay Input2' : Unused code path elimination
//  Block '<S149>/Difference Inputs1' : Unused code path elimination
//  Block '<S149>/Difference Inputs2' : Unused code path elimination
//  Block '<S149>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S159>/Data Type Duplicate' : Unused code path elimination
//  Block '<S159>/Data Type Propagation' : Unused code path elimination
//  Block '<S159>/LowerRelop1' : Unused code path elimination
//  Block '<S159>/Switch' : Unused code path elimination
//  Block '<S159>/Switch2' : Unused code path elimination
//  Block '<S159>/UpperRelop' : Unused code path elimination
//  Block '<S149>/delta fall limit' : Unused code path elimination
//  Block '<S149>/delta rise limit' : Unused code path elimination
//  Block '<S149>/sample time' : Unused code path elimination
//  Block '<S150>/Data Type Duplicate' : Unused code path elimination
//  Block '<S150>/Data Type Propagation' : Unused code path elimination
//  Block '<S151>/Data Type Duplicate' : Unused code path elimination
//  Block '<S151>/Data Type Propagation' : Unused code path elimination
//  Block '<S151>/LowerRelop1' : Unused code path elimination
//  Block '<S151>/Switch' : Unused code path elimination
//  Block '<S151>/Switch2' : Unused code path elimination
//  Block '<S151>/UpperRelop' : Unused code path elimination
//  Block '<S152>/Data Type Duplicate' : Unused code path elimination
//  Block '<S152>/Data Type Propagation' : Unused code path elimination
//  Block '<S152>/LowerRelop1' : Unused code path elimination
//  Block '<S152>/Switch' : Unused code path elimination
//  Block '<S152>/Switch2' : Unused code path elimination
//  Block '<S152>/UpperRelop' : Unused code path elimination
//  Block '<S161>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S164>/Data Type Duplicate' : Unused code path elimination
//  Block '<S164>/Data Type Propagation' : Unused code path elimination
//  Block '<S162>/Data Type Duplicate' : Unused code path elimination
//  Block '<S162>/Data Type Propagation' : Unused code path elimination
//  Block '<S177>/Divide3' : Unused code path elimination
//  Block '<S189>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S191>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S201>/Data Type Duplicate' : Unused code path elimination
//  Block '<S201>/Data Type Propagation' : Unused code path elimination
//  Block '<S192>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S202>/Data Type Duplicate' : Unused code path elimination
//  Block '<S202>/Data Type Propagation' : Unused code path elimination
//  Block '<S193>/Delay Input2' : Unused code path elimination
//  Block '<S193>/Difference Inputs1' : Unused code path elimination
//  Block '<S193>/Difference Inputs2' : Unused code path elimination
//  Block '<S193>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S203>/Data Type Duplicate' : Unused code path elimination
//  Block '<S203>/Data Type Propagation' : Unused code path elimination
//  Block '<S203>/LowerRelop1' : Unused code path elimination
//  Block '<S203>/Switch' : Unused code path elimination
//  Block '<S203>/Switch2' : Unused code path elimination
//  Block '<S203>/UpperRelop' : Unused code path elimination
//  Block '<S193>/delta fall limit' : Unused code path elimination
//  Block '<S193>/delta rise limit' : Unused code path elimination
//  Block '<S193>/sample time' : Unused code path elimination
//  Block '<S194>/Data Type Duplicate' : Unused code path elimination
//  Block '<S194>/Data Type Propagation' : Unused code path elimination
//  Block '<S195>/Data Type Duplicate' : Unused code path elimination
//  Block '<S195>/Data Type Propagation' : Unused code path elimination
//  Block '<S196>/Data Type Duplicate' : Unused code path elimination
//  Block '<S196>/Data Type Propagation' : Unused code path elimination
//  Block '<S196>/LowerRelop1' : Unused code path elimination
//  Block '<S196>/Switch' : Unused code path elimination
//  Block '<S196>/Switch2' : Unused code path elimination
//  Block '<S196>/UpperRelop' : Unused code path elimination
//  Block '<S204>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S204>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S206>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S216>/Data Type Duplicate' : Unused code path elimination
//  Block '<S216>/Data Type Propagation' : Unused code path elimination
//  Block '<S207>/Delay Input2' : Unused code path elimination
//  Block '<S207>/Difference Inputs1' : Unused code path elimination
//  Block '<S207>/Difference Inputs2' : Unused code path elimination
//  Block '<S207>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S217>/Data Type Duplicate' : Unused code path elimination
//  Block '<S217>/Data Type Propagation' : Unused code path elimination
//  Block '<S217>/LowerRelop1' : Unused code path elimination
//  Block '<S217>/Switch' : Unused code path elimination
//  Block '<S217>/Switch2' : Unused code path elimination
//  Block '<S217>/UpperRelop' : Unused code path elimination
//  Block '<S207>/delta fall limit' : Unused code path elimination
//  Block '<S207>/delta rise limit' : Unused code path elimination
//  Block '<S207>/sample time' : Unused code path elimination
//  Block '<S208>/Delay Input2' : Unused code path elimination
//  Block '<S208>/Difference Inputs1' : Unused code path elimination
//  Block '<S208>/Difference Inputs2' : Unused code path elimination
//  Block '<S208>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S218>/Data Type Duplicate' : Unused code path elimination
//  Block '<S218>/Data Type Propagation' : Unused code path elimination
//  Block '<S218>/LowerRelop1' : Unused code path elimination
//  Block '<S218>/Switch' : Unused code path elimination
//  Block '<S218>/Switch2' : Unused code path elimination
//  Block '<S218>/UpperRelop' : Unused code path elimination
//  Block '<S208>/delta fall limit' : Unused code path elimination
//  Block '<S208>/delta rise limit' : Unused code path elimination
//  Block '<S208>/sample time' : Unused code path elimination
//  Block '<S209>/Data Type Duplicate' : Unused code path elimination
//  Block '<S209>/Data Type Propagation' : Unused code path elimination
//  Block '<S210>/Data Type Duplicate' : Unused code path elimination
//  Block '<S210>/Data Type Propagation' : Unused code path elimination
//  Block '<S210>/LowerRelop1' : Unused code path elimination
//  Block '<S210>/Switch' : Unused code path elimination
//  Block '<S210>/Switch2' : Unused code path elimination
//  Block '<S210>/UpperRelop' : Unused code path elimination
//  Block '<S211>/Data Type Duplicate' : Unused code path elimination
//  Block '<S211>/Data Type Propagation' : Unused code path elimination
//  Block '<S211>/LowerRelop1' : Unused code path elimination
//  Block '<S211>/Switch' : Unused code path elimination
//  Block '<S211>/Switch2' : Unused code path elimination
//  Block '<S211>/UpperRelop' : Unused code path elimination
//  Block '<S219>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S219>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S221>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S231>/Data Type Duplicate' : Unused code path elimination
//  Block '<S231>/Data Type Propagation' : Unused code path elimination
//  Block '<S222>/Delay Input2' : Unused code path elimination
//  Block '<S222>/Difference Inputs1' : Unused code path elimination
//  Block '<S222>/Difference Inputs2' : Unused code path elimination
//  Block '<S222>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S232>/Data Type Duplicate' : Unused code path elimination
//  Block '<S232>/Data Type Propagation' : Unused code path elimination
//  Block '<S232>/LowerRelop1' : Unused code path elimination
//  Block '<S232>/Switch' : Unused code path elimination
//  Block '<S232>/Switch2' : Unused code path elimination
//  Block '<S232>/UpperRelop' : Unused code path elimination
//  Block '<S222>/delta fall limit' : Unused code path elimination
//  Block '<S222>/delta rise limit' : Unused code path elimination
//  Block '<S222>/sample time' : Unused code path elimination
//  Block '<S223>/Delay Input2' : Unused code path elimination
//  Block '<S223>/Difference Inputs1' : Unused code path elimination
//  Block '<S223>/Difference Inputs2' : Unused code path elimination
//  Block '<S223>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S233>/Data Type Duplicate' : Unused code path elimination
//  Block '<S233>/Data Type Propagation' : Unused code path elimination
//  Block '<S233>/LowerRelop1' : Unused code path elimination
//  Block '<S233>/Switch' : Unused code path elimination
//  Block '<S233>/Switch2' : Unused code path elimination
//  Block '<S233>/UpperRelop' : Unused code path elimination
//  Block '<S223>/delta fall limit' : Unused code path elimination
//  Block '<S223>/delta rise limit' : Unused code path elimination
//  Block '<S223>/sample time' : Unused code path elimination
//  Block '<S224>/Data Type Duplicate' : Unused code path elimination
//  Block '<S224>/Data Type Propagation' : Unused code path elimination
//  Block '<S225>/Data Type Duplicate' : Unused code path elimination
//  Block '<S225>/Data Type Propagation' : Unused code path elimination
//  Block '<S225>/LowerRelop1' : Unused code path elimination
//  Block '<S225>/Switch' : Unused code path elimination
//  Block '<S225>/Switch2' : Unused code path elimination
//  Block '<S225>/UpperRelop' : Unused code path elimination
//  Block '<S226>/Data Type Duplicate' : Unused code path elimination
//  Block '<S226>/Data Type Propagation' : Unused code path elimination
//  Block '<S226>/LowerRelop1' : Unused code path elimination
//  Block '<S226>/Switch' : Unused code path elimination
//  Block '<S226>/Switch2' : Unused code path elimination
//  Block '<S226>/UpperRelop' : Unused code path elimination
//  Block '<S235>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S238>/Data Type Duplicate' : Unused code path elimination
//  Block '<S238>/Data Type Propagation' : Unused code path elimination
//  Block '<S236>/Data Type Duplicate' : Unused code path elimination
//  Block '<S236>/Data Type Propagation' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'fcsModel'
//  '<S1>'   : 'fcsModel/Allocation'
//  '<S2>'   : 'fcsModel/Inner Loop Controller'
//  '<S3>'   : 'fcsModel/Outer Loop Controller'
//  '<S4>'   : 'fcsModel/RC Interpreter'
//  '<S5>'   : 'fcsModel/Allocation/Angular Velocity Conversion'
//  '<S6>'   : 'fcsModel/Allocation/Compare To Constant'
//  '<S7>'   : 'fcsModel/Allocation/For Each Subsystem'
//  '<S8>'   : 'fcsModel/Allocation/sysIdInjection'
//  '<S9>'   : 'fcsModel/Allocation/sysIdInjection/Compare To Constant'
//  '<S10>'  : 'fcsModel/Allocation/sysIdInjection/Compare To Constant1'
//  '<S11>'  : 'fcsModel/Allocation/sysIdInjection/Compare To Constant2'
//  '<S12>'  : 'fcsModel/Allocation/sysIdInjection/Compare To Constant3'
//  '<S13>'  : 'fcsModel/Allocation/sysIdInjection/Compare To Constant4'
//  '<S14>'  : 'fcsModel/Allocation/sysIdInjection/sysIdInputGeneration'
//  '<S15>'  : 'fcsModel/Allocation/sysIdInjection/sysIdInputGeneration/zeroInjection'
//  '<S16>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller'
//  '<S17>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs'
//  '<S18>'  : 'fcsModel/Inner Loop Controller/Attitude Controller'
//  '<S19>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem'
//  '<S20>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block'
//  '<S21>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1'
//  '<S22>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug'
//  '<S23>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S24>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S25>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S26>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S27>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S28>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic'
//  '<S29>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic1'
//  '<S30>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic2'
//  '<S31>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S32>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S33>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S34>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S35>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S36>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S37>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S38>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S39>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S40>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S41>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S42>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S43>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic'
//  '<S44>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S45>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S46>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S47>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S48>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S49>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S50>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S51>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S52>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S53>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S54>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic'
//  '<S55>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Saturation Dynamic'
//  '<S56>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S57>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S58>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs/Compare To Constant'
//  '<S59>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs/EulerRates2BodyRates'
//  '<S60>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control'
//  '<S61>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Compare To Constant'
//  '<S62>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Compare To Constant1'
//  '<S63>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block'
//  '<S64>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1'
//  '<S65>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pickAttitudeCmdAndMeas'
//  '<S66>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug'
//  '<S67>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S68>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S69>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S70>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S71>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S72>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic'
//  '<S73>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic1'
//  '<S74>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic2'
//  '<S75>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S76>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S77>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S78>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S79>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S80>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S81>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S82>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S83>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S84>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S85>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S86>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S87>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic'
//  '<S88>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S89>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S90>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S91>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S92>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S93>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S94>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S95>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S96>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S97>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S98>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Rate Limiter Dynamic'
//  '<S99>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Saturation Dynamic'
//  '<S100>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S101>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S102>' : 'fcsModel/Outer Loop Controller/Compare To Constant'
//  '<S103>' : 'fcsModel/Outer Loop Controller/Compare To Constant1'
//  '<S104>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl'
//  '<S105>' : 'fcsModel/Outer Loop Controller/assembleOuterLoopToInnerLoopBus'
//  '<S106>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs'
//  '<S107>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Compare To Constant'
//  '<S108>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller'
//  '<S109>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller'
//  '<S110>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs/Compare To Constant1'
//  '<S111>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs/Compare To Constant2'
//  '<S112>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs'
//  '<S113>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control'
//  '<S114>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant'
//  '<S115>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant1'
//  '<S116>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant2'
//  '<S117>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant3'
//  '<S118>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant4'
//  '<S119>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant5'
//  '<S120>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant6'
//  '<S121>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter'
//  '<S122>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter1'
//  '<S123>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter2'
//  '<S124>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter/holdOutputAtCenter'
//  '<S125>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter1/holdOutputAtCenter'
//  '<S126>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter2/holdOutputAtCenter'
//  '<S127>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block'
//  '<S128>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1'
//  '<S129>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug'
//  '<S130>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S131>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S132>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S133>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S134>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S135>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic'
//  '<S136>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic1'
//  '<S137>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic2'
//  '<S138>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S139>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S140>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S141>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S142>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S143>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S144>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S145>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S146>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S147>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S148>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S149>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S150>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic'
//  '<S151>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S152>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S153>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S154>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S155>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S156>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S157>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S158>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S159>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S160>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S161>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Rate Limiter Dynamic'
//  '<S162>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Saturation Dynamic'
//  '<S163>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S164>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S165>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs'
//  '<S166>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem'
//  '<S167>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant'
//  '<S168>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant1'
//  '<S169>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant2'
//  '<S170>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/accelZKiSelectorVariantSubsystem'
//  '<S171>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/holdOutputAtCenter'
//  '<S172>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem'
//  '<S173>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd'
//  '<S174>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/zAccelCtrlVariantSubsystem'
//  '<S175>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/accelZKiSelectorVariantSubsystem/accelZCtrlKiPassThrough'
//  '<S176>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/holdOutputAtCenter/holdOutputAtCenter'
//  '<S177>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust'
//  '<S178>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant'
//  '<S179>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant1'
//  '<S180>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant2'
//  '<S181>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant3'
//  '<S182>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd/kinematicInversion'
//  '<S183>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd/kinematicInversion/NE Accel Cmds To Roll Pitch Cmds'
//  '<S184>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/zAccelCtrlVariantSubsystem/VelCtrlOutPassthrough'
//  '<S185>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block'
//  '<S186>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1'
//  '<S187>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2'
//  '<S188>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug'
//  '<S189>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S190>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S191>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S192>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S193>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S194>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic'
//  '<S195>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic1'
//  '<S196>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic2'
//  '<S197>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S198>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S199>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S200>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S201>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S202>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S203>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S204>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S205>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S206>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S207>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S208>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S209>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic'
//  '<S210>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S211>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S212>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S213>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S214>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S215>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S216>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S217>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S218>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S219>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter'
//  '<S220>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter'
//  '<S221>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic'
//  '<S222>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic1'
//  '<S223>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic2'
//  '<S224>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic'
//  '<S225>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic1'
//  '<S226>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic2'
//  '<S227>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S228>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S229>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S230>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S231>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S232>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S233>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S234>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S235>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic'
//  '<S236>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Saturation Dynamic'
//  '<S237>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S238>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S239>' : 'fcsModel/RC Interpreter/Chart'
//  '<S240>' : 'fcsModel/RC Interpreter/Interpret RC In Cmds'


//-
//  Requirements for '<Root>': fcsModel

#endif                                 // RTW_HEADER_fcsModel_h_

//
// File trailer for generated code.
//
// [EOF]
//
