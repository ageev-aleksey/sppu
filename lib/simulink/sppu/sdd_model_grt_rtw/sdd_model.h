/*
 * sdd_model.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "sdd_model".
 *
 * Model version              : 1.18
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C++ source code generated on : Sun Aug  2 12:12:03 2020
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_sdd_model_h_
#define RTW_HEADER_sdd_model_h_
#include <cmath>
#include <cfloat>
#include <cstring>
#include <stddef.h>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "nesl_rtw_lti.h"
#include "sdd_model_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rtGetNaN.h"
#include "rt_nonfinite.h"
#include "rt_zcfcn.h"
#include "rtGetInf.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
# define rtmGetContStateDisabled(rtm)  ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
# define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
# define rtmGetContStates(rtm)         ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
# define rtmSetContStates(rtm, val)    ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
# define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
# define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
# define rtmGetIntgData(rtm)           ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
# define rtmSetIntgData(rtm, val)      ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
# define rtmGetOdeF(rtm)               ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
# define rtmSetOdeF(rtm, val)          ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
# define rtmGetOdeY(rtm)               ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
# define rtmSetOdeY(rtm, val)          ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
# define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
# define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
# define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
# define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
# define rtmGetZCCacheNeedsReset(rtm)  ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
# define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
# define rtmGetdX(rtm)                 ((rtm)->derivs)
#endif

#ifndef rtmSetdX
# define rtmSetdX(rtm, val)            ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T Plus;                         /* '<S22>/Plus' */
  real_T signal;                       /* '<S22>/Switch' */
  real_T signal_m;                     /* '<S22>/Product1' */
  real_T Switch;                       /* '<S5>/Switch' */
  real_T v;                            /* '<S5>/Integrator' */
  real_T Gain1;                        /* '<S2>/Gain1' */
  real_T INPUT_1_1_1[4];               /* '<S19>/INPUT_1_1_1' */
  real_T LTI_STATE_1;                  /* '<S19>/LTI_STATE_1' */
  real_T LTI_OUTPUT_1_1;               /* '<S19>/LTI_OUTPUT_1_1' */
  real_T Gain;                         /* '<S5>/Gain' */
  real_T Product[3];                   /* '<S6>/Product' */
  real_T Fb;                           /* '<S5>/Divide' */
  real_T s;                            /* '<S4>/Integrator1' */
  real_T Plus_b;                       /* '<S20>/Plus' */
  real_T signal_d;                     /* '<S20>/Switch' */
  real_T signal_l;                     /* '<S20>/Product1' */
  real_T Switch_k;                     /* '<S4>/Switch' */
  real_T v_l;                          /* '<S4>/Integrator' */
  real_T Gain1_j;                      /* '<S1>/Gain1' */
  real_T INPUT_1_1_1_b[4];             /* '<S13>/INPUT_1_1_1' */
  real_T LTI_STATE_1_g;                /* '<S13>/LTI_STATE_1' */
  real_T LTI_OUTPUT_1_1_h;             /* '<S13>/LTI_OUTPUT_1_1' */
  real_T Product_j[3];                 /* '<S3>/Product' */
  real_T Gain_o;                       /* '<S4>/Gain' */
  real_T Fb_g;                         /* '<S4>/Divide' */
} B_sdd_model_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T INPUT_1_1_1_Discrete[2];      /* '<S19>/INPUT_1_1_1' */
  real_T LTI_STATE_1_FirstOutput;      /* '<S19>/LTI_STATE_1' */
  real_T INPUT_1_1_1_Discrete_j[2];    /* '<S13>/INPUT_1_1_1' */
  real_T LTI_STATE_1_FirstOutput_k;    /* '<S13>/LTI_STATE_1' */
  int_T Integrator_IWORK;              /* '<S5>/Integrator' */
  int_T Integrator_IWORK_c;            /* '<S4>/Integrator' */
} DW_sdd_model_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator1_CSTATE;           /* '<S5>/Integrator1' */
  real_T Integrator_CSTATE;            /* '<S5>/Integrator' */
  real_T sdd_modelElectricSchene1Inducto;/* '<S19>/LTI_STATE_1' */
  real_T Integrator1_CSTATE_i;         /* '<S4>/Integrator1' */
  real_T Integrator_CSTATE_f;          /* '<S4>/Integrator' */
  real_T sdd_modelElectricScheneInductor;/* '<S13>/LTI_STATE_1' */
} X_sdd_model_T;

/* StatePackage derivatives (default storage) */
typedef struct {
  real_T Integrator1_CSTATE;           /* '<S5>/Integrator1' */
  real_T Integrator_CSTATE;            /* '<S5>/Integrator' */
  real_T sdd_modelElectricSchene1Inducto;/* '<S19>/LTI_STATE_1' */
  real_T Integrator1_CSTATE_i;         /* '<S4>/Integrator1' */
  real_T Integrator_CSTATE_f;          /* '<S4>/Integrator' */
  real_T sdd_modelElectricScheneInductor;/* '<S13>/LTI_STATE_1' */
} XDot_sdd_model_T;

/* StatePackage disabled  */
typedef struct {
  boolean_T Integrator1_CSTATE;        /* '<S5>/Integrator1' */
  boolean_T Integrator_CSTATE;         /* '<S5>/Integrator' */
  boolean_T sdd_modelElectricSchene1Inducto;/* '<S19>/LTI_STATE_1' */
  boolean_T Integrator1_CSTATE_i;      /* '<S4>/Integrator1' */
  boolean_T Integrator_CSTATE_f;       /* '<S4>/Integrator' */
  boolean_T sdd_modelElectricScheneInductor;/* '<S13>/LTI_STATE_1' */
} XDis_sdd_model_T;

/* Zero-crossing (trigger) state */
typedef struct {
  ZCSigState Integrator_Reset_ZCE;     /* '<S5>/Integrator' */
  ZCSigState Integrator_Reset_ZCE_h;   /* '<S4>/Integrator' */
} PrevZCX_sdd_model_T;

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
} ODE4_IntgData;

#endif

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T pwmOx;                        /* '<Root>/Input' */
  real_T pwmOz;                        /* '<Root>/Input1' */
} ExtU_sdd_model_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
    real_T positionOz;                   /* '<Root>/Output2' */
    real_T speedOz;                      /* '<Root>/Output3' */
    real_T speedOX;                      /* '<Root>/Output' */
    real_T positionOx;                   /* '<Root>/Output1' */
    real_T IOx;                          /* '<Root>/Output4' */
    real_T IOz;                          /* '<Root>/Output5' */
} ExtY_sdd_model_T;

/* Parameters (default storage) */
struct P_sdd_model_T_ {
  real_T Bh[3];                        /* Variable: Bh
                                        * Referenced by:
                                        *   '<S3>/BHorizontal'
                                        *   '<S6>/BHorizontal'
                                        */
  real_T StructModelConst;             /* Variable: StructModelConst
                                        * Referenced by:
                                        *   '<S4>/Constant'
                                        *   '<S5>/Constant'
                                        */
  real_T VX0;                          /* Variable: VX0
                                        * Referenced by: '<S4>/Constant2'
                                        */
  real_T VZ0;                          /* Variable: VZ0
                                        * Referenced by: '<S5>/Constant2'
                                        */
  real_T degX0;                        /* Variable: degX0
                                        * Referenced by: '<S4>/Integrator1'
                                        */
  real_T degZ0;                        /* Variable: degZ0
                                        * Referenced by: '<S5>/Integrator1'
                                        */
  real_T frictionCoef;                 /* Variable: frictionCoef
                                        * Referenced by:
                                        *   '<S4>/Gain'
                                        *   '<S5>/Gain'
                                        */
  real_T frictionLinearCoef;           /* Variable: frictionLinearCoef
                                        * Referenced by:
                                        *   '<S21>/k1'
                                        *   '<S23>/k1'
                                        */
  real_T frictionQuadraticCoef;        /* Variable: frictionQuadraticCoef
                                        * Referenced by:
                                        *   '<S21>/k2'
                                        *   '<S23>/k2'
                                        */
  real_T Constant_Value;               /* Expression: 1
                                        * Referenced by: '<S22>/Constant'
                                        */
  real_T Constant2_Value;              /* Expression: 0
                                        * Referenced by: '<S22>/Constant2'
                                        */
  real_T Switch1_Threshold;            /* Expression: 1
                                        * Referenced by: '<S22>/Switch1'
                                        */
  real_T Constant3_Value;              /* Expression: 0
                                        * Referenced by: '<S22>/Constant3'
                                        */
  real_T Constant1_Value;              /* Expression: pi/6
                                        * Referenced by: '<S22>/Constant1'
                                        */
  real_T Constant4_Value;              /* Expression: 1
                                        * Referenced by: '<S22>/Constant4'
                                        */
  real_T Switch_Threshold;             /* Expression: 0.001
                                        * Referenced by: '<S22>/Switch'
                                        */
  real_T Constant1_Value_e;            /* Expression: 0
                                        * Referenced by: '<S5>/Constant1'
                                        */
  real_T Switch_Threshold_b;           /* Expression: 0
                                        * Referenced by: '<S5>/Switch'
                                        */
  real_T Gain1_Gain;                   /* Expression: 12
                                        * Referenced by: '<S2>/Gain1'
                                        */
  real_T k0_Value;                     /* Expression: 0
                                        * Referenced by: '<S23>/k0'
                                        */
  real_T Constant_Value_l;             /* Expression: 1
                                        * Referenced by: '<S20>/Constant'
                                        */
  real_T Constant2_Value_b;            /* Expression: 0
                                        * Referenced by: '<S20>/Constant2'
                                        */
  real_T Switch1_Threshold_l;          /* Expression: 1
                                        * Referenced by: '<S20>/Switch1'
                                        */
  real_T Constant3_Value_h;            /* Expression: 0
                                        * Referenced by: '<S20>/Constant3'
                                        */
  real_T Constant1_Value_a;            /* Expression: pi/6
                                        * Referenced by: '<S20>/Constant1'
                                        */
  real_T Constant4_Value_k;            /* Expression: 1
                                        * Referenced by: '<S20>/Constant4'
                                        */
  real_T Switch_Threshold_br;          /* Expression: 0.001
                                        * Referenced by: '<S20>/Switch'
                                        */
  real_T Constant1_Value_m;            /* Expression: 0
                                        * Referenced by: '<S4>/Constant1'
                                        */
  real_T Switch_Threshold_a;           /* Expression: 0
                                        * Referenced by: '<S4>/Switch'
                                        */
  real_T Gain1_Gain_j;                 /* Expression: 12
                                        * Referenced by: '<S1>/Gain1'
                                        */
  real_T Constant2_Value_g;            /* Expression: pi/6
                                        * Referenced by: '<Root>/Constant2'
                                        */
  real_T k0_Value_m;                   /* Expression: 0
                                        * Referenced by: '<S21>/k0'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_sdd_model_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_sdd_model_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  boolean_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[6];
  real_T odeF[4][6];
  ODE4_IntgData intgData;

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    boolean_T firstInitCondFlag;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Class declaration for model sdd_model */
class sdd_modelModelClass {
  /* public data and function members */
 public:
  /* model initialize function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  void terminate();

  /* Constructor */
  sdd_modelModelClass();

  /* Destructor */
  ~sdd_modelModelClass();

  static P_sdd_model_T& getModelParameters() {
      return sdd_model_P;
  }
  static void backToDefaultModelParameters() {
      sdd_model_P = defaultValue;
  }

  /* Root-level structure-based inputs set method */

  /* Root inports set method */
  void setExternalInputs(const ExtU_sdd_model_T* pExtU_sdd_model_T)
  {
    sdd_model_U = *pExtU_sdd_model_T;
  }

  /* Root-level structure-based outputs get method */

  /* Root outports get method */
  const ExtY_sdd_model_T & getExternalOutputs() const
  {
    return sdd_model_Y;
  }

  /* Real-Time Model get method */
  RT_MODEL_sdd_model_T * getRTM();

  /* private data and function members */
 private:
  /* Tunable parameters */
  static P_sdd_model_T sdd_model_P;
  static P_sdd_model_T defaultValue;

  /* Block signals */
  B_sdd_model_T sdd_model_B;

  /* Block states */
  DW_sdd_model_T sdd_model_DW;
  X_sdd_model_T sdd_model_X;           /* Block continuous states */
  PrevZCX_sdd_model_T sdd_model_PrevZCX;/* Triggered events */

  /* External inputs */
  ExtU_sdd_model_T sdd_model_U;

  /* External outputs */
  ExtY_sdd_model_T sdd_model_Y;

  /* Real-Time Model */
  RT_MODEL_sdd_model_T sdd_model_M;

  /* Continuous states update member function*/
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  /* Derivatives member function */
  void sdd_model_derivatives();
};

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S1>/Cast To Double' : Eliminate redundant data type conversion
 * Block '<S2>/Cast To Double' : Eliminate redundant data type conversion
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'sdd_model'
 * '<S1>'   : 'sdd_model/ElectricSchene'
 * '<S2>'   : 'sdd_model/ElectricSchene1'
 * '<S3>'   : 'sdd_model/HorizontalSolenoids'
 * '<S4>'   : 'sdd_model/MechanicsOx'
 * '<S5>'   : 'sdd_model/MechanicsOx1'
 * '<S6>'   : 'sdd_model/VerticalSolenoids'
 * '<S7>'   : 'sdd_model/XY Graph'
 * '<S8>'   : 'sdd_model/ElectricSchene/PS-Simulink Converter1'
 * '<S9>'   : 'sdd_model/ElectricSchene/Simulink-PS Converter2'
 * '<S10>'  : 'sdd_model/ElectricSchene/Solver Configuration1'
 * '<S11>'  : 'sdd_model/ElectricSchene/PS-Simulink Converter1/EVAL_KEY'
 * '<S12>'  : 'sdd_model/ElectricSchene/Simulink-PS Converter2/EVAL_KEY'
 * '<S13>'  : 'sdd_model/ElectricSchene/Solver Configuration1/EVAL_KEY'
 * '<S14>'  : 'sdd_model/ElectricSchene1/PS-Simulink Converter1'
 * '<S15>'  : 'sdd_model/ElectricSchene1/Simulink-PS Converter2'
 * '<S16>'  : 'sdd_model/ElectricSchene1/Solver Configuration1'
 * '<S17>'  : 'sdd_model/ElectricSchene1/PS-Simulink Converter1/EVAL_KEY'
 * '<S18>'  : 'sdd_model/ElectricSchene1/Simulink-PS Converter2/EVAL_KEY'
 * '<S19>'  : 'sdd_model/ElectricSchene1/Solver Configuration1/EVAL_KEY'
 * '<S20>'  : 'sdd_model/MechanicsOx/NormalForce'
 * '<S21>'  : 'sdd_model/MechanicsOx/friction force'
 * '<S22>'  : 'sdd_model/MechanicsOx1/NormalForce'
 * '<S23>'  : 'sdd_model/MechanicsOx1/friction force'
 */
#endif                                 /* RTW_HEADER_sdd_model_h_ */
