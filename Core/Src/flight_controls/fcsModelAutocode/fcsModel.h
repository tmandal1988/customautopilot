//
// File: fcsModel.h
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 7.9
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Wed Jul 29 08:20:05 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#ifndef fcsModel_h_
#define fcsModel_h_
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

  // Block signals and states (default storage) for system '<S24>/Discrete First Order Deriv Filter' 
  struct DW_DiscreteFirstOrderDerivFil_T {
    std::array<real_T, 2> num;
                      // '<S55>/Compute Deriv Filter Numerator And Denominator'
    std::array<real_T, 2> den;
                      // '<S55>/Compute Deriv Filter Numerator And Denominator'
    real_T DiscreteTransferFcn_states; // '<S55>/Discrete Transfer Fcn'
  };

  // Block signals and states (default storage) for system '<S21>/pidWithDebug'
  struct DW_pidWithDebug_fcsModel_T {
    DW_DiscreteFirstOrderDerivFil_T DiscreteFirstOrderDerivFilter;
                                   // '<S24>/Discrete First Order Deriv Filter'
    real_T DiscreteTimeIntegrator_DSTATE;// '<S24>/Discrete-Time Integrator'
    real_T Delay_DSTATE;               // '<S56>/Delay'
    real_T UnitDelay_DSTATE;           // '<S24>/Unit Delay'
    real_T UnitDelay1_DSTATE;          // '<S24>/Unit Delay1'
    int8_T DiscreteTimeIntegrator_PrevRese;// '<S24>/Discrete-Time Integrator'
    uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S24>/Discrete-Time Integrator'
    boolean_T icLoad;                  // '<S56>/Delay'
  };

  // Block signals and states (default storage) for system '<S21>/Signal Conditioning Block1' 
  struct DW_SignalConditioningBlock1_f_T {
    std::array<real_T, 3> num;
                            // '<S41>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> den;
                            // '<S41>/Compute Filter Numerator And Denominator'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S41>/Discrete Transfer Fcn' 
    real_T Delay_DSTATE;               // '<S42>/Delay'
    real_T DiscreteTransferFcn_tmp;    // '<S41>/Discrete Transfer Fcn'
    boolean_T icLoad;                  // '<S42>/Delay'
  };

  // Block signals and states (default storage) for system '<S18>/For Each Subsystem' 
  struct DW_CoreSubsys_fcsModel_c_T {
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock;// '<S21>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock1;// '<S21>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_T pidWithDebug;// '<S21>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S21>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S20>/Attitude Control' 
  struct DW_CoreSubsys_fcsModel_i_T {
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock;// '<S62>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_f_T SignalConditioningBlock1;// '<S62>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_T pidWithDebug;// '<S62>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S62>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S114>/holdOutputAtCenter1' 
  struct DW_holdOutputAtCenter1_fcsMod_T {
    real_T last_input;                 // '<S124>/holdOutputAtCenter'
  };

  // Block signals and states (default storage) for system '<S115>/pidWithDebug' 
  struct DW_pidWithDebug_fcsModel_i_T {
    DW_DiscreteFirstOrderDerivFil_T DiscreteFirstOrderDerivFilter;
                                  // '<S131>/Discrete First Order Deriv Filter'
    real_T DiscreteTimeIntegrator_DSTATE;// '<S131>/Discrete-Time Integrator'
    real_T Delay_DSTATE;               // '<S163>/Delay'
    real_T UnitDelay_DSTATE;           // '<S131>/Unit Delay'
    real_T UnitDelay1_DSTATE;          // '<S131>/Unit Delay1'
    int8_T DiscreteTimeIntegrator_PrevRese;// '<S131>/Discrete-Time Integrator'
    uint8_T DiscreteTimeIntegrator_IC_LOADI;// '<S131>/Discrete-Time Integrator' 
    boolean_T icLoad;                  // '<S163>/Delay'
  };

  // Block signals and states (default storage) for system '<S115>/Signal Conditioning Block1' 
  struct DW_SignalConditioningBlock1_g_T {
    std::array<real_T, 3> num;
                           // '<S148>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> den;
                           // '<S148>/Compute Filter Numerator And Denominator'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S148>/Discrete Transfer Fcn' 
    real_T Delay_DSTATE;               // '<S149>/Delay'
    real_T DiscreteTransferFcn_tmp;    // '<S148>/Discrete Transfer Fcn'
    boolean_T icLoad;                  // '<S149>/Delay'
  };

  // Block signals and states (default storage) for system '<S110>/NED Position Control' 
  struct DW_CoreSubsys_fcsModel_b_T {
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock;// '<S115>/Signal Conditioning Block' 
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock1;// '<S115>/Signal Conditioning Block1' 
    DW_pidWithDebug_fcsModel_i_T pidWithDebug;// '<S115>/pidWithDebug'
    real_T UnitDelay_DSTATE;           // '<S115>/Unit Delay'
  };

  // Block signals and states (default storage) for system '<S111>/For Each Subsystem' 
  struct DW_CoreSubsys_fcsModel_p_T {
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock1;// '<S168>/Signal Conditioning Block1' 
    DW_SignalConditioningBlock1_g_T SignalConditioningBlock2;// '<S168>/Signal Conditioning Block2' 
    DW_pidWithDebug_fcsModel_i_T pidWithDebug;// '<S168>/pidWithDebug'
    std::array<real_T, 3> num;
                           // '<S192>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> den;
                           // '<S192>/Compute Filter Numerator And Denominator'
    std::array<real_T, 3> rateNum;// '<S191>/Compute Numerator And Denominator'
    std::array<real_T, 3> den_e;  // '<S191>/Compute Numerator And Denominator'
    std::array<real_T, 2> DiscreteTransferFcn_states;// '<S191>/Discrete Transfer Fcn' 
    std::array<real_T, 2> DiscreteTransferFcn_states_h;// '<S192>/Discrete Transfer Fcn' 
    real_T UnitDelay_DSTATE;           // '<S168>/Unit Delay'
    real_T Delay_DSTATE;               // '<S194>/Delay'
    real_T Delay_DSTATE_m;             // '<S193>/Delay'
    real_T DiscreteTransferFcn_tmp;    // '<S191>/Discrete Transfer Fcn'
    real_T DiscreteTransferFcn_tmp_l;  // '<S192>/Discrete Transfer Fcn'
    boolean_T icLoad;                  // '<S194>/Delay'
    boolean_T icLoad_a;                // '<S193>/Delay'
  };

  // Block signals and states (default storage) for system '<Root>'
  struct DW_fcsModel_T {
    std::array<DW_CoreSubsys_fcsModel_p_T, 3> CoreSubsys_i;// '<S111>/For Each Subsystem' 
    std::array<DW_CoreSubsys_fcsModel_b_T, 3> CoreSubsys_g;// '<S110>/NED Position Control' 
    DW_holdOutputAtCenter1_fcsMod_T holdOutputAtCenter2;// '<S114>/holdOutputAtCenter2' 
    DW_holdOutputAtCenter1_fcsMod_T holdOutputAtCenter1;// '<S114>/holdOutputAtCenter1' 
    std::array<DW_CoreSubsys_fcsModel_i_T, 3> CoreSubsys_p;// '<S20>/Attitude Control' 
    std::array<DW_CoreSubsys_fcsModel_c_T, 3> CoreSubsys_a;// '<S18>/For Each Subsystem' 
    std::array<DW_CoreSubsys_fcsModel_T, 4> CoreSubsys;// '<S1>/For Each Subsystem' 
    busOuterLoopCtrlDebug RateTransition_Buffer0;// '<Root>/Rate Transition'
    busOuterLoopToInnerLoop Switch2;   // '<S3>/Switch2'
    busRcOutCmds rcOutCmds;            // '<S4>/Interpret RC In Cmds'
    std::array<real_T, 4> DiscreteTransferFcn_states_d;// '<S1>/Discrete Transfer Fcn' 
    real_T DiscreteTransferFcn_states; // '<S184>/Discrete Transfer Fcn'
    real_T Delay_DSTATE;               // '<S179>/Delay'
    real_T UnitDelay1_DSTATE;          // '<S15>/Unit Delay1'
    real_T UnitDelay_DSTATE;           // '<S15>/Unit Delay'
    real_T DiscreteTransferFcn_states_c;// '<S15>/Discrete Transfer Fcn'
    real_T DiscreteTransferFcn_tmp;    // '<S184>/Discrete Transfer Fcn'
    real_T NextOutput;                 // '<S16>/White Noise'
    real_T last_input;                 // '<S173>/holdOutputAtCenter'
    real_T last_input_l;               // '<S123>/holdOutputAtCenter'
    uint32_T RandSeed;                 // '<S16>/White Noise'
    uint32_T durationCounter_1;        // '<S4>/Chart'
    uint32_T durationCounter_1_d;      // '<S4>/Chart'
    enumChirpTrigger UnitDelay_DSTATE_g;// '<S4>/Unit Delay'
    uint16_T temporalCounter_i1;       // '<S4>/Chart'
    uint8_T chirpCount_;               // '<S4>/Interpret RC In Cmds'
    uint8_T is_active_c1_rcInterpreter;// '<S4>/Chart'
    uint8_T is_c1_rcInterpreter;       // '<S4>/Chart'
    boolean_T icLoad;                  // '<S179>/Delay'
    boolean_T throttle_is_up;          // '<S4>/Interpret RC In Cmds'
    boolean_T rcCheckFlag;             // '<S4>/Chart'
  };

  // External inputs (root inport signals), for system '<Root>'
  struct ExtUPointer_fcsModel_T {
    busStateEstimate stateEstimate;    // '<Root>/stateEstimate'
    busRcInCmds rcCmdsIn;              // '<Root>/rcCmdsIn'
    busExternalCmds externalCmds;      // '<Root>/externalCmds'
  };

  // instance parameters, for system '<Root>'
  struct InstP_fcsModel_T {
    busFcsParams fcsParamsArg;         // Variable: fcsParamsArg
                                          //  Referenced by: '<Root>/fcsParams'

  };

  // Constant parameters (default storage)
  struct ConstP_fcsModel_T {
    // Pooled Parameter (Mixed Expressions)
    //  Referenced by:
    //    '<S3>/Constant'
    //    '<S167>/Constant'

    busOuterLoopToInnerLoop pooled3;

    // Expression: rpmToPwmLut(:, 1)
    //  Referenced by: '<S7>/Prelookup'

    std::array<real_T, 15> Prelookup_BreakpointsData;

    // Expression: rpmToPwmLut(:, 2)
    //  Referenced by: '<S7>/Interpolation Using Prelookup'

    std::array<real_T, 15> InterpolationUsingPrelookup_Tab;

    // Expression: allocationDataStruct.allocationMatrix
    //  Referenced by: '<S1>/Constant'

    std::array<real_T, 16> Constant_Value_c;

    // Expression: vehicleConstants.inertia_kgm2
    //  Referenced by: '<S2>/Constant'

    std::array<real_T, 9> Constant_Value_n;

    // Expression: [0; 0; g_mps2]
    //  Referenced by: '<S111>/Constant'

    std::array<real_T, 3> Constant_Value_h;

    // Computed Parameter: Constant_Value_e
    //  Referenced by: '<S20>/Constant'

    std::array<uint8_T, 3> Constant_Value_e;
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

  // Constructor
  fcsModel(ExtUPointer_fcsModel_T *fcsModel_ExtUPointer, InstP_fcsModel_T
           *fcsModel_InstP);

  // Root outports get method
  const ExtY_fcsModel_T &getExternalOutputs() const
  {
    return fcsModel_Y;
  }

  // set method for External inputs (root inport signals)
  void setExternalInputs(const ExtUPointer_fcsModel_T &ExtUPointer_U)
  {
    *ExtUPointer_ref_U = ExtUPointer_U;
  }

  // get method for instance parameters
  const InstP_fcsModel_T &get_InstP() const
  {
    return *fcsModel_InstP_ref;
  }

  // set method for instance parameters
  void set_InstP(const InstP_fcsModel_T &fcsModel_InstP_arg)
  {
    *fcsModel_InstP_ref = fcsModel_InstP_arg;
  }

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  static void terminate();

  // Destructor
  ~fcsModel();

  // private data and function members
 private:
  // External outputs
  ExtY_fcsModel_T fcsModel_Y;

  // Block states
  DW_fcsModel_T fcsModel_DW;

  // External inputs (root inport signals)
  ExtUPointer_fcsModel_T *ExtUPointer_ref_U;

  // instance parameters
  InstP_fcsModel_T *fcsModel_InstP_ref;

  // private member function(s) for subsystem '<S24>/Discrete First Order Deriv Filter'
  static void f_DiscreteFirstOrderDerivFilter(real_T rtu_input, real_T
    rtu_filterBandwidth_radps, real_T *rty_filteredInputRate, real_T
    rtp_sampleTime_s, DW_DiscreteFirstOrderDerivFil_T *localDW);

  // private member function(s) for subsystem '<S21>/pidWithDebug'
  static void fcsModel_pidWithDebug_Init(DW_pidWithDebug_fcsModel_T *localDW);
  static void fcsModel_pidWithDebug(real_T rtu_feedForward, real_T rtu_cmd,
    real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc,
    const busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T
    *rty_ctrlCmd, busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s,
    DW_pidWithDebug_fcsModel_T *localDW);

  // private member function(s) for subsystem '<S40>/Compute Natural Frequency'
  static void fcsMode_ComputeNaturalFrequency(real_T rtu_bandwidth_radps, real_T
    rtu_dampingRatio_nd, real_T *rty_naturalFrequency_radps);

  // private member function(s) for subsystem '<S40>/Compute Numerator And Denominator'
  static void ComputeNumeratorAndDenominator(real_T rtu_naturalFrequency_radps,
    real_T rtu_dampingRatio_nd, real_T rty_rateNum[3], real_T rty_accelNum[3],
    real_T rty_den[3], real_T rtp_sampleTime_s);

  // private member function(s) for subsystem '<S41>/Compute Filter Numerator And Denominator'
  static void ComputeFilterNumeratorAndD_Init(real_T rty_num[3], real_T rty_den
    [3]);
  static void ComputeFilterNumeratorAndDenomi(real_T rtu_naturalFrequency_radps,
    real_T rtu_dampingRatio_nd, real_T rty_num[3], real_T rty_den[3], real_T
    rtp_sampleTime_s);

  // private member function(s) for subsystem '<S21>/Signal Conditioning Block1'
  static void f_SignalConditioningBlock1_Init(DW_SignalConditioningBlock1_f_T
    *localDW);
  static void fcsMod_SignalConditioningBlock1(real_T rtu_input, const
    busSignalConditioningParams *rtu_params, real_T *rty_filteredInput, real_T
    rtp_sampleTime_s, DW_SignalConditioningBlock1_f_T *localDW);

  // private member function(s) for subsystem '<S114>/holdOutputAtCenter1'
  static void fcsModel_holdOutputAtCenter1(real_T rtu_input, real_T rtu_trigger,
    real_T *rty_output, boolean_T *rty_atCenter, DW_holdOutputAtCenter1_fcsMod_T
    *localDW);

  // private member function(s) for subsystem '<S115>/pidWithDebug'
  static void fcsModel_pidWithDebug_m_Init(DW_pidWithDebug_fcsModel_i_T *localDW);
  static void fcsModel_pidWithDebug_j(real_T rtu_feedForward, real_T rtu_cmd,
    real_T rtu_meas, boolean_T rtu_integratorReset, real_T rtu_integratorIc,
    const busPidParams *rtu_pidParamBus, real_T rtu_trackingCtrlCmd, real_T
    *rty_ctrlCmd, busPidDebug *rty_pidDebug, real_T rtp_sampleTime_s,
    DW_pidWithDebug_fcsModel_i_T *localDW);

  // private member function(s) for subsystem '<S115>/Signal Conditioning Block1'
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
//  Block '<S25>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S25>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S27>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S37>/Data Type Duplicate' : Unused code path elimination
//  Block '<S37>/Data Type Propagation' : Unused code path elimination
//  Block '<S28>/Delay' : Unused code path elimination
//  Block '<S28>/Difference Inputs1' : Unused code path elimination
//  Block '<S28>/Difference Inputs2' : Unused code path elimination
//  Block '<S28>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S38>/Data Type Duplicate' : Unused code path elimination
//  Block '<S38>/Data Type Propagation' : Unused code path elimination
//  Block '<S38>/LowerRelop1' : Unused code path elimination
//  Block '<S38>/Switch' : Unused code path elimination
//  Block '<S38>/Switch2' : Unused code path elimination
//  Block '<S38>/UpperRelop' : Unused code path elimination
//  Block '<S28>/Zero-Order Hold' : Unused code path elimination
//  Block '<S28>/delta fall limit' : Unused code path elimination
//  Block '<S28>/delta rise limit' : Unused code path elimination
//  Block '<S28>/sample time' : Unused code path elimination
//  Block '<S29>/Delay' : Unused code path elimination
//  Block '<S29>/Difference Inputs1' : Unused code path elimination
//  Block '<S29>/Difference Inputs2' : Unused code path elimination
//  Block '<S29>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S39>/Data Type Duplicate' : Unused code path elimination
//  Block '<S39>/Data Type Propagation' : Unused code path elimination
//  Block '<S39>/LowerRelop1' : Unused code path elimination
//  Block '<S39>/Switch' : Unused code path elimination
//  Block '<S39>/Switch2' : Unused code path elimination
//  Block '<S39>/UpperRelop' : Unused code path elimination
//  Block '<S29>/Zero-Order Hold' : Unused code path elimination
//  Block '<S29>/delta fall limit' : Unused code path elimination
//  Block '<S29>/delta rise limit' : Unused code path elimination
//  Block '<S29>/sample time' : Unused code path elimination
//  Block '<S30>/Data Type Duplicate' : Unused code path elimination
//  Block '<S30>/Data Type Propagation' : Unused code path elimination
//  Block '<S31>/Data Type Duplicate' : Unused code path elimination
//  Block '<S31>/Data Type Propagation' : Unused code path elimination
//  Block '<S31>/LowerRelop1' : Unused code path elimination
//  Block '<S31>/Switch' : Unused code path elimination
//  Block '<S31>/Switch2' : Unused code path elimination
//  Block '<S31>/UpperRelop' : Unused code path elimination
//  Block '<S32>/Data Type Duplicate' : Unused code path elimination
//  Block '<S32>/Data Type Propagation' : Unused code path elimination
//  Block '<S32>/LowerRelop1' : Unused code path elimination
//  Block '<S32>/Switch' : Unused code path elimination
//  Block '<S32>/Switch2' : Unused code path elimination
//  Block '<S32>/UpperRelop' : Unused code path elimination
//  Block '<S40>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S40>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S42>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S52>/Data Type Duplicate' : Unused code path elimination
//  Block '<S52>/Data Type Propagation' : Unused code path elimination
//  Block '<S43>/Delay' : Unused code path elimination
//  Block '<S43>/Difference Inputs1' : Unused code path elimination
//  Block '<S43>/Difference Inputs2' : Unused code path elimination
//  Block '<S43>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S53>/Data Type Duplicate' : Unused code path elimination
//  Block '<S53>/Data Type Propagation' : Unused code path elimination
//  Block '<S53>/LowerRelop1' : Unused code path elimination
//  Block '<S53>/Switch' : Unused code path elimination
//  Block '<S53>/Switch2' : Unused code path elimination
//  Block '<S53>/UpperRelop' : Unused code path elimination
//  Block '<S43>/Zero-Order Hold' : Unused code path elimination
//  Block '<S43>/delta fall limit' : Unused code path elimination
//  Block '<S43>/delta rise limit' : Unused code path elimination
//  Block '<S43>/sample time' : Unused code path elimination
//  Block '<S44>/Delay' : Unused code path elimination
//  Block '<S44>/Difference Inputs1' : Unused code path elimination
//  Block '<S44>/Difference Inputs2' : Unused code path elimination
//  Block '<S44>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S54>/Data Type Duplicate' : Unused code path elimination
//  Block '<S54>/Data Type Propagation' : Unused code path elimination
//  Block '<S54>/LowerRelop1' : Unused code path elimination
//  Block '<S54>/Switch' : Unused code path elimination
//  Block '<S54>/Switch2' : Unused code path elimination
//  Block '<S54>/UpperRelop' : Unused code path elimination
//  Block '<S44>/Zero-Order Hold' : Unused code path elimination
//  Block '<S44>/delta fall limit' : Unused code path elimination
//  Block '<S44>/delta rise limit' : Unused code path elimination
//  Block '<S44>/sample time' : Unused code path elimination
//  Block '<S45>/Data Type Duplicate' : Unused code path elimination
//  Block '<S45>/Data Type Propagation' : Unused code path elimination
//  Block '<S46>/Data Type Duplicate' : Unused code path elimination
//  Block '<S46>/Data Type Propagation' : Unused code path elimination
//  Block '<S46>/LowerRelop1' : Unused code path elimination
//  Block '<S46>/Switch' : Unused code path elimination
//  Block '<S46>/Switch2' : Unused code path elimination
//  Block '<S46>/UpperRelop' : Unused code path elimination
//  Block '<S47>/Data Type Duplicate' : Unused code path elimination
//  Block '<S47>/Data Type Propagation' : Unused code path elimination
//  Block '<S47>/LowerRelop1' : Unused code path elimination
//  Block '<S47>/Switch' : Unused code path elimination
//  Block '<S47>/Switch2' : Unused code path elimination
//  Block '<S47>/UpperRelop' : Unused code path elimination
//  Block '<S56>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S59>/Data Type Duplicate' : Unused code path elimination
//  Block '<S59>/Data Type Propagation' : Unused code path elimination
//  Block '<S57>/Data Type Duplicate' : Unused code path elimination
//  Block '<S57>/Data Type Propagation' : Unused code path elimination
//  Block '<S69>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S69>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S71>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S81>/Data Type Duplicate' : Unused code path elimination
//  Block '<S81>/Data Type Propagation' : Unused code path elimination
//  Block '<S72>/Delay' : Unused code path elimination
//  Block '<S72>/Difference Inputs1' : Unused code path elimination
//  Block '<S72>/Difference Inputs2' : Unused code path elimination
//  Block '<S72>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S82>/Data Type Duplicate' : Unused code path elimination
//  Block '<S82>/Data Type Propagation' : Unused code path elimination
//  Block '<S82>/LowerRelop1' : Unused code path elimination
//  Block '<S82>/Switch' : Unused code path elimination
//  Block '<S82>/Switch2' : Unused code path elimination
//  Block '<S82>/UpperRelop' : Unused code path elimination
//  Block '<S72>/Zero-Order Hold' : Unused code path elimination
//  Block '<S72>/delta fall limit' : Unused code path elimination
//  Block '<S72>/delta rise limit' : Unused code path elimination
//  Block '<S72>/sample time' : Unused code path elimination
//  Block '<S73>/Delay' : Unused code path elimination
//  Block '<S73>/Difference Inputs1' : Unused code path elimination
//  Block '<S73>/Difference Inputs2' : Unused code path elimination
//  Block '<S73>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S83>/Data Type Duplicate' : Unused code path elimination
//  Block '<S83>/Data Type Propagation' : Unused code path elimination
//  Block '<S83>/LowerRelop1' : Unused code path elimination
//  Block '<S83>/Switch' : Unused code path elimination
//  Block '<S83>/Switch2' : Unused code path elimination
//  Block '<S83>/UpperRelop' : Unused code path elimination
//  Block '<S73>/Zero-Order Hold' : Unused code path elimination
//  Block '<S73>/delta fall limit' : Unused code path elimination
//  Block '<S73>/delta rise limit' : Unused code path elimination
//  Block '<S73>/sample time' : Unused code path elimination
//  Block '<S74>/Data Type Duplicate' : Unused code path elimination
//  Block '<S74>/Data Type Propagation' : Unused code path elimination
//  Block '<S75>/Data Type Duplicate' : Unused code path elimination
//  Block '<S75>/Data Type Propagation' : Unused code path elimination
//  Block '<S75>/LowerRelop1' : Unused code path elimination
//  Block '<S75>/Switch' : Unused code path elimination
//  Block '<S75>/Switch2' : Unused code path elimination
//  Block '<S75>/UpperRelop' : Unused code path elimination
//  Block '<S76>/Data Type Duplicate' : Unused code path elimination
//  Block '<S76>/Data Type Propagation' : Unused code path elimination
//  Block '<S76>/LowerRelop1' : Unused code path elimination
//  Block '<S76>/Switch' : Unused code path elimination
//  Block '<S76>/Switch2' : Unused code path elimination
//  Block '<S76>/UpperRelop' : Unused code path elimination
//  Block '<S84>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S84>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S86>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S96>/Data Type Duplicate' : Unused code path elimination
//  Block '<S96>/Data Type Propagation' : Unused code path elimination
//  Block '<S87>/Delay' : Unused code path elimination
//  Block '<S87>/Difference Inputs1' : Unused code path elimination
//  Block '<S87>/Difference Inputs2' : Unused code path elimination
//  Block '<S87>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S97>/Data Type Duplicate' : Unused code path elimination
//  Block '<S97>/Data Type Propagation' : Unused code path elimination
//  Block '<S97>/LowerRelop1' : Unused code path elimination
//  Block '<S97>/Switch' : Unused code path elimination
//  Block '<S97>/Switch2' : Unused code path elimination
//  Block '<S97>/UpperRelop' : Unused code path elimination
//  Block '<S87>/Zero-Order Hold' : Unused code path elimination
//  Block '<S87>/delta fall limit' : Unused code path elimination
//  Block '<S87>/delta rise limit' : Unused code path elimination
//  Block '<S87>/sample time' : Unused code path elimination
//  Block '<S88>/Delay' : Unused code path elimination
//  Block '<S88>/Difference Inputs1' : Unused code path elimination
//  Block '<S88>/Difference Inputs2' : Unused code path elimination
//  Block '<S88>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S98>/Data Type Duplicate' : Unused code path elimination
//  Block '<S98>/Data Type Propagation' : Unused code path elimination
//  Block '<S98>/LowerRelop1' : Unused code path elimination
//  Block '<S98>/Switch' : Unused code path elimination
//  Block '<S98>/Switch2' : Unused code path elimination
//  Block '<S98>/UpperRelop' : Unused code path elimination
//  Block '<S88>/Zero-Order Hold' : Unused code path elimination
//  Block '<S88>/delta fall limit' : Unused code path elimination
//  Block '<S88>/delta rise limit' : Unused code path elimination
//  Block '<S88>/sample time' : Unused code path elimination
//  Block '<S89>/Data Type Duplicate' : Unused code path elimination
//  Block '<S89>/Data Type Propagation' : Unused code path elimination
//  Block '<S90>/Data Type Duplicate' : Unused code path elimination
//  Block '<S90>/Data Type Propagation' : Unused code path elimination
//  Block '<S90>/LowerRelop1' : Unused code path elimination
//  Block '<S90>/Switch' : Unused code path elimination
//  Block '<S90>/Switch2' : Unused code path elimination
//  Block '<S90>/UpperRelop' : Unused code path elimination
//  Block '<S91>/Data Type Duplicate' : Unused code path elimination
//  Block '<S91>/Data Type Propagation' : Unused code path elimination
//  Block '<S91>/LowerRelop1' : Unused code path elimination
//  Block '<S91>/Switch' : Unused code path elimination
//  Block '<S91>/Switch2' : Unused code path elimination
//  Block '<S91>/UpperRelop' : Unused code path elimination
//  Block '<S100>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S103>/Data Type Duplicate' : Unused code path elimination
//  Block '<S103>/Data Type Propagation' : Unused code path elimination
//  Block '<S101>/Data Type Duplicate' : Unused code path elimination
//  Block '<S101>/Data Type Propagation' : Unused code path elimination
//  Block '<S132>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S132>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S134>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S144>/Data Type Duplicate' : Unused code path elimination
//  Block '<S144>/Data Type Propagation' : Unused code path elimination
//  Block '<S135>/Delay' : Unused code path elimination
//  Block '<S135>/Difference Inputs1' : Unused code path elimination
//  Block '<S135>/Difference Inputs2' : Unused code path elimination
//  Block '<S135>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S145>/Data Type Duplicate' : Unused code path elimination
//  Block '<S145>/Data Type Propagation' : Unused code path elimination
//  Block '<S145>/LowerRelop1' : Unused code path elimination
//  Block '<S145>/Switch' : Unused code path elimination
//  Block '<S145>/Switch2' : Unused code path elimination
//  Block '<S145>/UpperRelop' : Unused code path elimination
//  Block '<S135>/Zero-Order Hold' : Unused code path elimination
//  Block '<S135>/delta fall limit' : Unused code path elimination
//  Block '<S135>/delta rise limit' : Unused code path elimination
//  Block '<S135>/sample time' : Unused code path elimination
//  Block '<S136>/Delay' : Unused code path elimination
//  Block '<S136>/Difference Inputs1' : Unused code path elimination
//  Block '<S136>/Difference Inputs2' : Unused code path elimination
//  Block '<S136>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S146>/Data Type Duplicate' : Unused code path elimination
//  Block '<S146>/Data Type Propagation' : Unused code path elimination
//  Block '<S146>/LowerRelop1' : Unused code path elimination
//  Block '<S146>/Switch' : Unused code path elimination
//  Block '<S146>/Switch2' : Unused code path elimination
//  Block '<S146>/UpperRelop' : Unused code path elimination
//  Block '<S136>/Zero-Order Hold' : Unused code path elimination
//  Block '<S136>/delta fall limit' : Unused code path elimination
//  Block '<S136>/delta rise limit' : Unused code path elimination
//  Block '<S136>/sample time' : Unused code path elimination
//  Block '<S137>/Data Type Duplicate' : Unused code path elimination
//  Block '<S137>/Data Type Propagation' : Unused code path elimination
//  Block '<S138>/Data Type Duplicate' : Unused code path elimination
//  Block '<S138>/Data Type Propagation' : Unused code path elimination
//  Block '<S138>/LowerRelop1' : Unused code path elimination
//  Block '<S138>/Switch' : Unused code path elimination
//  Block '<S138>/Switch2' : Unused code path elimination
//  Block '<S138>/UpperRelop' : Unused code path elimination
//  Block '<S139>/Data Type Duplicate' : Unused code path elimination
//  Block '<S139>/Data Type Propagation' : Unused code path elimination
//  Block '<S139>/LowerRelop1' : Unused code path elimination
//  Block '<S139>/Switch' : Unused code path elimination
//  Block '<S139>/Switch2' : Unused code path elimination
//  Block '<S139>/UpperRelop' : Unused code path elimination
//  Block '<S147>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S147>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S149>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S159>/Data Type Duplicate' : Unused code path elimination
//  Block '<S159>/Data Type Propagation' : Unused code path elimination
//  Block '<S150>/Delay' : Unused code path elimination
//  Block '<S150>/Difference Inputs1' : Unused code path elimination
//  Block '<S150>/Difference Inputs2' : Unused code path elimination
//  Block '<S150>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S160>/Data Type Duplicate' : Unused code path elimination
//  Block '<S160>/Data Type Propagation' : Unused code path elimination
//  Block '<S160>/LowerRelop1' : Unused code path elimination
//  Block '<S160>/Switch' : Unused code path elimination
//  Block '<S160>/Switch2' : Unused code path elimination
//  Block '<S160>/UpperRelop' : Unused code path elimination
//  Block '<S150>/Zero-Order Hold' : Unused code path elimination
//  Block '<S150>/delta fall limit' : Unused code path elimination
//  Block '<S150>/delta rise limit' : Unused code path elimination
//  Block '<S150>/sample time' : Unused code path elimination
//  Block '<S151>/Delay' : Unused code path elimination
//  Block '<S151>/Difference Inputs1' : Unused code path elimination
//  Block '<S151>/Difference Inputs2' : Unused code path elimination
//  Block '<S151>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S161>/Data Type Duplicate' : Unused code path elimination
//  Block '<S161>/Data Type Propagation' : Unused code path elimination
//  Block '<S161>/LowerRelop1' : Unused code path elimination
//  Block '<S161>/Switch' : Unused code path elimination
//  Block '<S161>/Switch2' : Unused code path elimination
//  Block '<S161>/UpperRelop' : Unused code path elimination
//  Block '<S151>/Zero-Order Hold' : Unused code path elimination
//  Block '<S151>/delta fall limit' : Unused code path elimination
//  Block '<S151>/delta rise limit' : Unused code path elimination
//  Block '<S151>/sample time' : Unused code path elimination
//  Block '<S152>/Data Type Duplicate' : Unused code path elimination
//  Block '<S152>/Data Type Propagation' : Unused code path elimination
//  Block '<S153>/Data Type Duplicate' : Unused code path elimination
//  Block '<S153>/Data Type Propagation' : Unused code path elimination
//  Block '<S153>/LowerRelop1' : Unused code path elimination
//  Block '<S153>/Switch' : Unused code path elimination
//  Block '<S153>/Switch2' : Unused code path elimination
//  Block '<S153>/UpperRelop' : Unused code path elimination
//  Block '<S154>/Data Type Duplicate' : Unused code path elimination
//  Block '<S154>/Data Type Propagation' : Unused code path elimination
//  Block '<S154>/LowerRelop1' : Unused code path elimination
//  Block '<S154>/Switch' : Unused code path elimination
//  Block '<S154>/Switch2' : Unused code path elimination
//  Block '<S154>/UpperRelop' : Unused code path elimination
//  Block '<S163>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S166>/Data Type Duplicate' : Unused code path elimination
//  Block '<S166>/Data Type Propagation' : Unused code path elimination
//  Block '<S164>/Data Type Duplicate' : Unused code path elimination
//  Block '<S164>/Data Type Propagation' : Unused code path elimination
//  Block '<S179>/Divide3' : Unused code path elimination
//  Block '<S191>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S193>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S203>/Data Type Duplicate' : Unused code path elimination
//  Block '<S203>/Data Type Propagation' : Unused code path elimination
//  Block '<S194>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S204>/Data Type Duplicate' : Unused code path elimination
//  Block '<S204>/Data Type Propagation' : Unused code path elimination
//  Block '<S195>/Delay' : Unused code path elimination
//  Block '<S195>/Difference Inputs1' : Unused code path elimination
//  Block '<S195>/Difference Inputs2' : Unused code path elimination
//  Block '<S195>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S205>/Data Type Duplicate' : Unused code path elimination
//  Block '<S205>/Data Type Propagation' : Unused code path elimination
//  Block '<S205>/LowerRelop1' : Unused code path elimination
//  Block '<S205>/Switch' : Unused code path elimination
//  Block '<S205>/Switch2' : Unused code path elimination
//  Block '<S205>/UpperRelop' : Unused code path elimination
//  Block '<S195>/Zero-Order Hold' : Unused code path elimination
//  Block '<S195>/delta fall limit' : Unused code path elimination
//  Block '<S195>/delta rise limit' : Unused code path elimination
//  Block '<S195>/sample time' : Unused code path elimination
//  Block '<S196>/Data Type Duplicate' : Unused code path elimination
//  Block '<S196>/Data Type Propagation' : Unused code path elimination
//  Block '<S197>/Data Type Duplicate' : Unused code path elimination
//  Block '<S197>/Data Type Propagation' : Unused code path elimination
//  Block '<S198>/Data Type Duplicate' : Unused code path elimination
//  Block '<S198>/Data Type Propagation' : Unused code path elimination
//  Block '<S198>/LowerRelop1' : Unused code path elimination
//  Block '<S198>/Switch' : Unused code path elimination
//  Block '<S198>/Switch2' : Unused code path elimination
//  Block '<S198>/UpperRelop' : Unused code path elimination
//  Block '<S206>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S206>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S208>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S218>/Data Type Duplicate' : Unused code path elimination
//  Block '<S218>/Data Type Propagation' : Unused code path elimination
//  Block '<S209>/Delay' : Unused code path elimination
//  Block '<S209>/Difference Inputs1' : Unused code path elimination
//  Block '<S209>/Difference Inputs2' : Unused code path elimination
//  Block '<S209>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S219>/Data Type Duplicate' : Unused code path elimination
//  Block '<S219>/Data Type Propagation' : Unused code path elimination
//  Block '<S219>/LowerRelop1' : Unused code path elimination
//  Block '<S219>/Switch' : Unused code path elimination
//  Block '<S219>/Switch2' : Unused code path elimination
//  Block '<S219>/UpperRelop' : Unused code path elimination
//  Block '<S209>/Zero-Order Hold' : Unused code path elimination
//  Block '<S209>/delta fall limit' : Unused code path elimination
//  Block '<S209>/delta rise limit' : Unused code path elimination
//  Block '<S209>/sample time' : Unused code path elimination
//  Block '<S210>/Delay' : Unused code path elimination
//  Block '<S210>/Difference Inputs1' : Unused code path elimination
//  Block '<S210>/Difference Inputs2' : Unused code path elimination
//  Block '<S210>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S220>/Data Type Duplicate' : Unused code path elimination
//  Block '<S220>/Data Type Propagation' : Unused code path elimination
//  Block '<S220>/LowerRelop1' : Unused code path elimination
//  Block '<S220>/Switch' : Unused code path elimination
//  Block '<S220>/Switch2' : Unused code path elimination
//  Block '<S220>/UpperRelop' : Unused code path elimination
//  Block '<S210>/Zero-Order Hold' : Unused code path elimination
//  Block '<S210>/delta fall limit' : Unused code path elimination
//  Block '<S210>/delta rise limit' : Unused code path elimination
//  Block '<S210>/sample time' : Unused code path elimination
//  Block '<S211>/Data Type Duplicate' : Unused code path elimination
//  Block '<S211>/Data Type Propagation' : Unused code path elimination
//  Block '<S212>/Data Type Duplicate' : Unused code path elimination
//  Block '<S212>/Data Type Propagation' : Unused code path elimination
//  Block '<S212>/LowerRelop1' : Unused code path elimination
//  Block '<S212>/Switch' : Unused code path elimination
//  Block '<S212>/Switch2' : Unused code path elimination
//  Block '<S212>/UpperRelop' : Unused code path elimination
//  Block '<S213>/Data Type Duplicate' : Unused code path elimination
//  Block '<S213>/Data Type Propagation' : Unused code path elimination
//  Block '<S213>/LowerRelop1' : Unused code path elimination
//  Block '<S213>/Switch' : Unused code path elimination
//  Block '<S213>/Switch2' : Unused code path elimination
//  Block '<S213>/UpperRelop' : Unused code path elimination
//  Block '<S221>/Discrete Transfer Fcn' : Unused code path elimination
//  Block '<S221>/Discrete Transfer Fcn1' : Unused code path elimination
//  Block '<S223>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S233>/Data Type Duplicate' : Unused code path elimination
//  Block '<S233>/Data Type Propagation' : Unused code path elimination
//  Block '<S224>/Delay' : Unused code path elimination
//  Block '<S224>/Difference Inputs1' : Unused code path elimination
//  Block '<S224>/Difference Inputs2' : Unused code path elimination
//  Block '<S224>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S234>/Data Type Duplicate' : Unused code path elimination
//  Block '<S234>/Data Type Propagation' : Unused code path elimination
//  Block '<S234>/LowerRelop1' : Unused code path elimination
//  Block '<S234>/Switch' : Unused code path elimination
//  Block '<S234>/Switch2' : Unused code path elimination
//  Block '<S234>/UpperRelop' : Unused code path elimination
//  Block '<S224>/Zero-Order Hold' : Unused code path elimination
//  Block '<S224>/delta fall limit' : Unused code path elimination
//  Block '<S224>/delta rise limit' : Unused code path elimination
//  Block '<S224>/sample time' : Unused code path elimination
//  Block '<S225>/Delay' : Unused code path elimination
//  Block '<S225>/Difference Inputs1' : Unused code path elimination
//  Block '<S225>/Difference Inputs2' : Unused code path elimination
//  Block '<S225>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S235>/Data Type Duplicate' : Unused code path elimination
//  Block '<S235>/Data Type Propagation' : Unused code path elimination
//  Block '<S235>/LowerRelop1' : Unused code path elimination
//  Block '<S235>/Switch' : Unused code path elimination
//  Block '<S235>/Switch2' : Unused code path elimination
//  Block '<S235>/UpperRelop' : Unused code path elimination
//  Block '<S225>/Zero-Order Hold' : Unused code path elimination
//  Block '<S225>/delta fall limit' : Unused code path elimination
//  Block '<S225>/delta rise limit' : Unused code path elimination
//  Block '<S225>/sample time' : Unused code path elimination
//  Block '<S226>/Data Type Duplicate' : Unused code path elimination
//  Block '<S226>/Data Type Propagation' : Unused code path elimination
//  Block '<S227>/Data Type Duplicate' : Unused code path elimination
//  Block '<S227>/Data Type Propagation' : Unused code path elimination
//  Block '<S227>/LowerRelop1' : Unused code path elimination
//  Block '<S227>/Switch' : Unused code path elimination
//  Block '<S227>/Switch2' : Unused code path elimination
//  Block '<S227>/UpperRelop' : Unused code path elimination
//  Block '<S228>/Data Type Duplicate' : Unused code path elimination
//  Block '<S228>/Data Type Propagation' : Unused code path elimination
//  Block '<S228>/LowerRelop1' : Unused code path elimination
//  Block '<S228>/Switch' : Unused code path elimination
//  Block '<S228>/Switch2' : Unused code path elimination
//  Block '<S228>/UpperRelop' : Unused code path elimination
//  Block '<S237>/FixPt Data Type Duplicate' : Unused code path elimination
//  Block '<S240>/Data Type Duplicate' : Unused code path elimination
//  Block '<S240>/Data Type Propagation' : Unused code path elimination
//  Block '<S238>/Data Type Duplicate' : Unused code path elimination
//  Block '<S238>/Data Type Propagation' : Unused code path elimination
//  Block '<S27>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S42>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S56>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S71>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S86>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S100>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S134>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S149>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S163>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S193>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S194>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S208>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S223>/Zero-Order Hold' : Eliminated since input and output rates are identical
//  Block '<S237>/Zero-Order Hold' : Eliminated since input and output rates are identical


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
//  '<S15>'  : 'fcsModel/Allocation/sysIdInjection/sysIdInputGeneration/chirpInjection'
//  '<S16>'  : 'fcsModel/Allocation/sysIdInjection/sysIdInputGeneration/chirpInjection/Band-Limited White Noise'
//  '<S17>'  : 'fcsModel/Allocation/sysIdInjection/sysIdInputGeneration/chirpInjection/Generate Chirp'
//  '<S18>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller'
//  '<S19>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs'
//  '<S20>'  : 'fcsModel/Inner Loop Controller/Attitude Controller'
//  '<S21>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem'
//  '<S22>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block'
//  '<S23>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1'
//  '<S24>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug'
//  '<S25>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S26>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S27>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S28>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S29>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S30>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic'
//  '<S31>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic1'
//  '<S32>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic2'
//  '<S33>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S34>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S35>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S36>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S37>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S38>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S39>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S40>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S41>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S42>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S43>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S44>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S45>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic'
//  '<S46>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S47>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S48>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S49>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S50>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S51>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S52>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S53>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S54>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S55>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S56>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic'
//  '<S57>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Saturation Dynamic'
//  '<S58>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S59>'  : 'fcsModel/Inner Loop Controller/Angular Rate Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S60>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs/Compare To Constant'
//  '<S61>'  : 'fcsModel/Inner Loop Controller/Assemble Angular Rate Ctrl Inputs/EulerRates2BodyRates'
//  '<S62>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control'
//  '<S63>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Compare To Constant'
//  '<S64>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Compare To Constant1'
//  '<S65>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block'
//  '<S66>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1'
//  '<S67>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pickAttitudeCmdAndMeas'
//  '<S68>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug'
//  '<S69>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S70>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S71>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S72>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S73>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S74>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic'
//  '<S75>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic1'
//  '<S76>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Saturation Dynamic2'
//  '<S77>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S78>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S79>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S80>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S81>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S82>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S83>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S84>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S85>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S86>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S87>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S88>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S89>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic'
//  '<S90>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S91>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S92>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S93>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S94>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S95>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S96>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S97>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S98>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S99>'  : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S100>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Rate Limiter Dynamic'
//  '<S101>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Saturation Dynamic'
//  '<S102>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S103>' : 'fcsModel/Inner Loop Controller/Attitude Controller/Attitude Control/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S104>' : 'fcsModel/Outer Loop Controller/Compare To Constant'
//  '<S105>' : 'fcsModel/Outer Loop Controller/Compare To Constant1'
//  '<S106>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl'
//  '<S107>' : 'fcsModel/Outer Loop Controller/assembleOuterLoopToInnerLoopBus'
//  '<S108>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs'
//  '<S109>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Compare To Constant'
//  '<S110>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller'
//  '<S111>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller'
//  '<S112>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs/Compare To Constant1'
//  '<S113>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Assemble Vel Ctrl Inputs/Compare To Constant2'
//  '<S114>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs'
//  '<S115>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control'
//  '<S116>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant'
//  '<S117>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant1'
//  '<S118>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant2'
//  '<S119>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant3'
//  '<S120>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant4'
//  '<S121>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant5'
//  '<S122>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/Compare To Constant6'
//  '<S123>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter'
//  '<S124>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter1'
//  '<S125>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter2'
//  '<S126>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter/holdOutputAtCenter'
//  '<S127>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter1/holdOutputAtCenter'
//  '<S128>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/Assemble Position Controller Inputs/holdOutputAtCenter2/holdOutputAtCenter'
//  '<S129>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block'
//  '<S130>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1'
//  '<S131>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug'
//  '<S132>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S133>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S134>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S135>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S136>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S137>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic'
//  '<S138>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic1'
//  '<S139>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Saturation Dynamic2'
//  '<S140>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S141>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S142>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S143>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S144>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S145>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S146>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S147>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S148>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S149>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S150>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S151>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S152>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic'
//  '<S153>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S154>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S155>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S156>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S157>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S158>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S159>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S160>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S161>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S162>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S163>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Rate Limiter Dynamic'
//  '<S164>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Saturation Dynamic'
//  '<S165>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S166>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Position Controller/NED Position Control/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S167>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs'
//  '<S168>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem'
//  '<S169>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant'
//  '<S170>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant1'
//  '<S171>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/Compare To Constant2'
//  '<S172>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/accelZKiSelectorVariantSubsystem'
//  '<S173>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/holdOutputAtCenter'
//  '<S174>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem'
//  '<S175>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd'
//  '<S176>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/zAccelCtrlVariantSubsystem'
//  '<S177>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/accelZKiSelectorVariantSubsystem/accelZCtrlKiPassThrough'
//  '<S178>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/holdOutputAtCenter/holdOutputAtCenter'
//  '<S179>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust'
//  '<S180>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant'
//  '<S181>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant1'
//  '<S182>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant2'
//  '<S183>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/hoverThrustVariantSubsystem/constantHoverThrust/Compare To Constant3'
//  '<S184>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd/kinematicInversion'
//  '<S185>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/nedAccelToRollPitchCmd/kinematicInversion/NE Accel Cmds To Roll Pitch Cmds'
//  '<S186>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/Assemble Inner Loop Inputs/zAccelCtrlVariantSubsystem/VelCtrlOutPassthrough'
//  '<S187>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block'
//  '<S188>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1'
//  '<S189>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2'
//  '<S190>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug'
//  '<S191>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter'
//  '<S192>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter'
//  '<S193>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic'
//  '<S194>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1'
//  '<S195>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2'
//  '<S196>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic'
//  '<S197>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic1'
//  '<S198>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Saturation Dynamic2'
//  '<S199>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S200>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S201>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S202>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S203>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S204>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S205>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S206>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter'
//  '<S207>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter'
//  '<S208>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic'
//  '<S209>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1'
//  '<S210>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2'
//  '<S211>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic'
//  '<S212>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic1'
//  '<S213>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Saturation Dynamic2'
//  '<S214>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S215>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S216>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S217>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S218>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S219>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S220>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block1/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S221>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter'
//  '<S222>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter'
//  '<S223>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic'
//  '<S224>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic1'
//  '<S225>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic2'
//  '<S226>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic'
//  '<S227>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic1'
//  '<S228>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Saturation Dynamic2'
//  '<S229>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter/Compute Natural Frequency'
//  '<S230>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Deriv Filter/Compute Numerator And Denominator'
//  '<S231>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter/Compute Filter Numerator And Denominator'
//  '<S232>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Discrete Second Order Filter/Compute Natural Frequency'
//  '<S233>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S234>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic1/Saturation Dynamic'
//  '<S235>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/Signal Conditioning Block2/Rate Limiter Dynamic2/Saturation Dynamic'
//  '<S236>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter'
//  '<S237>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic'
//  '<S238>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Saturation Dynamic'
//  '<S239>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Discrete First Order Deriv Filter/Compute Deriv Filter Numerator And Denominator'
//  '<S240>' : 'fcsModel/Outer Loop Controller/PosAndVelCtrl/Velocity Controller/For Each Subsystem/pidWithDebug/Rate Limiter Dynamic/Saturation Dynamic'
//  '<S241>' : 'fcsModel/RC Interpreter/Chart'
//  '<S242>' : 'fcsModel/RC Interpreter/Interpret RC In Cmds'


//-
//  Requirements for '<Root>': fcsModel


#endif                                 // fcsModel_h_

//
// File trailer for generated code.
//
// [EOF]
//
