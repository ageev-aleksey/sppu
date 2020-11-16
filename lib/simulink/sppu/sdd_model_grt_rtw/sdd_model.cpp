/*
 * sdd_model.cpp
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "sdd_model".
 *
 * Model version              : 1.20
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C++ source code generated on : Tue Aug  4 10:10:39 2020
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "sdd_model.h"
#include "sdd_model_private.h"

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
void sdd_modelModelClass::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE4_IntgData *id = static_cast<ODE4_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T *f3 = id->f[3];
  real_T temp;
  int_T i;
  int_T nXc = 6;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) std::memcpy(y, x,
                     static_cast<uint_T>(nXc)*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  sdd_model_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  this->step();
  sdd_model_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  this->step();
  sdd_model_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  this->step();
  sdd_model_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void sdd_modelModelClass::step()
{
  /* local block i/o variables */
  real_T rtb_UnaryMinus_m;
  boolean_T resetSolver;
  ZCEventType zcEvent;
  real_T x;
  PmRealVector xPr;
  real_T u;
  PmRealVector uPr;
  NeuDiagnosticManager *diagnosticManager;
  NeuDiagnosticTree *diagnosticTree;
  LtiIcParams icParams;
  int_T ir[2];
  int_T jc[3];
  real_T mX[2];
  int_T jc_0[2];
  real_T mX_0[2];
  int_T ir_0;
  int_T jc_1[2];
  real_T mX_1;
  int_T jc_2[2];
  real_T mX_2;
  int_T ir_1;
  int_T jc_3[2];
  real_T mX_3;
  int_T ir_2;
  int_T jc_4[2];
  real_T mX_4;
  real_T mX_5;
  real_T mX_6;
  char *msg;
  PmRealVector xPr_0;
  real_T u_0;
  PmRealVector uPr_0;
  NeuDiagnosticTree *diagnosticTree_0;
  LtiIcParams icParams_0;
  int_T ir_3[2];
  int_T jc_5[3];
  real_T mX_7[2];
  int_T jc_6[2];
  real_T mX_8[2];
  int_T ir_4;
  int_T jc_7[2];
  real_T mX_9;
  int_T jc_8[2];
  real_T mX_a;
  int_T ir_5;
  int_T jc_9[2];
  real_T mX_b;
  int_T ir_6;
  int_T jc_a[2];
  real_T mX_c;
  real_T mX_d;
  real_T mX_e;
  char *msg_0;
  real_T rtb_Product_k;
  real_T rtb_Product_p;
  real_T tmp;
  real_T tmp_0;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    /* set solver stop time */
    if (!((&sdd_model_M)->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&(&sdd_model_M)->solverInfo, (((&sdd_model_M)
        ->Timing.clockTickH0 + 1) * (&sdd_model_M)->Timing.stepSize0 *
        4294967296.0));
    } else {
      rtsiSetSolverStopTime(&(&sdd_model_M)->solverInfo, (((&sdd_model_M)
        ->Timing.clockTick0 + 1) * (&sdd_model_M)->Timing.stepSize0 +
        (&sdd_model_M)->Timing.clockTickH0 * (&sdd_model_M)->Timing.stepSize0 *
        4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&sdd_model_M))) {
    (&sdd_model_M)->Timing.t[0] = rtsiGetT(&(&sdd_model_M)->solverInfo);
  }

  /* Integrator: '<S5>/Integrator1' */
  rtb_Product_p = sdd_model_X.Integrator1_CSTATE;

  /* Outport: '<Root>/Output2' incorporates:
   *  Integrator: '<S5>/Integrator1'
   */
  sdd_model_Y.positionOz = sdd_model_X.Integrator1_CSTATE;

  /* Sum: '<S22>/Plus' incorporates:
   *  Abs: '<S22>/Abs'
   *  Constant: '<S22>/Constant1'
   *  Integrator: '<S5>/Integrator1'
   */
  sdd_model_B.Plus = sdd_model_P.Constant1_Value - std::abs
    (sdd_model_X.Integrator1_CSTATE);

  /* Switch: '<S22>/Switch' incorporates:
   *  Constant: '<S22>/Constant3'
   *  Constant: '<S22>/Constant4'
   */
  if (sdd_model_B.Plus > sdd_model_P.Switch_Threshold) {
    sdd_model_B.signal = sdd_model_P.Constant3_Value;
  } else {
    sdd_model_B.signal = sdd_model_P.Constant4_Value;
  }

  /* End of Switch: '<S22>/Switch' */

  /* Signum: '<S22>/Sign' incorporates:
   *  Integrator: '<S5>/Integrator1'
   */
  if (sdd_model_X.Integrator1_CSTATE < 0.0) {
    tmp = -1.0;
  } else if (sdd_model_X.Integrator1_CSTATE > 0.0) {
    tmp = 1.0;
  } else if (sdd_model_X.Integrator1_CSTATE == 0.0) {
    tmp = 0.0;
  } else {
    tmp = (rtNaN);
  }

  /* End of Signum: '<S22>/Sign' */

  /* Signum: '<S22>/Sign1' */
  if (sdd_model_X.Integrator_CSTATE < 0.0) {
    tmp_0 = -1.0;
  } else if (sdd_model_X.Integrator_CSTATE > 0.0) {
    tmp_0 = 1.0;
  } else if (sdd_model_X.Integrator_CSTATE == 0.0) {
    tmp_0 = 0.0;
  } else {
    tmp_0 = (rtNaN);
  }

  /* End of Signum: '<S22>/Sign1' */

  /* Switch: '<S22>/Switch1' incorporates:
   *  Constant: '<S22>/Constant'
   *  Constant: '<S22>/Constant2'
   *  Product: '<S22>/Product'
   */
  if (tmp * tmp_0 >= sdd_model_P.Switch1_Threshold) {
    tmp = sdd_model_P.Constant_Value;
  } else {
    tmp = sdd_model_P.Constant2_Value;
  }

  /* End of Switch: '<S22>/Switch1' */

  /* Product: '<S22>/Product1' */
  sdd_model_B.signal_m = tmp * sdd_model_B.signal;

  /* Clock: '<S5>/Clock' incorporates:
   *  Clock: '<S4>/Clock'
   */
  tmp = (&sdd_model_M)->Timing.t[0];

  /* Switch: '<S5>/Switch' incorporates:
   *  Clock: '<S5>/Clock'
   *  Constant: '<S5>/Constant1'
   *  Constant: '<S5>/Constant2'
   */
  if (tmp > sdd_model_P.Switch_Threshold_b) {
    sdd_model_B.Switch = sdd_model_P.Constant1_Value_e;
  } else {
    sdd_model_B.Switch = sdd_model_P.VZ0;
  }

  /* End of Switch: '<S5>/Switch' */

  /* Integrator: '<S5>/Integrator' */
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    zcEvent = rt_ZCFcn(ANY_ZERO_CROSSING,&sdd_model_PrevZCX.Integrator_Reset_ZCE,
                       (sdd_model_B.signal_m));

    /* evaluate zero-crossings and the level of the reset signal */
    if ((zcEvent != NO_ZCEVENT) || (sdd_model_B.signal_m != 0.0) ||
        (sdd_model_DW.Integrator_IWORK != 0)) {
      sdd_model_X.Integrator_CSTATE = sdd_model_B.Switch;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&sdd_model_M)->solverInfo,
        true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&sdd_model_M)
        ->solverInfo, true);
    }
  }

  sdd_model_B.v = sdd_model_X.Integrator_CSTATE;

  /* End of Integrator: '<S5>/Integrator' */

  /* Outport: '<Root>/Output3' */
  sdd_model_Y.speedOz = sdd_model_B.v;

  /* Gain: '<S2>/Gain1' incorporates:
   *  Inport: '<Root>/Input1'
   */
  sdd_model_B.Gain1 = sdd_model_P.Gain1_Gain * sdd_model_U.pwmOz;

  /* SimscapeInputBlock: '<S19>/INPUT_1_1_1' */
  sdd_model_B.INPUT_1_1_1[0] = sdd_model_B.Gain1;
  sdd_model_B.INPUT_1_1_1[1] = 0.0;
  sdd_model_B.INPUT_1_1_1[2] = 0.0;
  sdd_model_B.INPUT_1_1_1[3] = 0.0;

  /* SimscapeLtiState: '<S19>/LTI_STATE_1' */
  x = sdd_model_X.sdd_modelElectricSchene1Inducto;
  if (sdd_model_DW.LTI_STATE_1_FirstOutput == 0.0) {
    sdd_model_DW.LTI_STATE_1_FirstOutput = 1.0;
    xPr.mN = 1;
    xPr.mX = &x;
    u = sdd_model_B.INPUT_1_1_1[0];
    uPr.mN = 1;
    uPr.mX = &u;
    diagnosticManager = rtw_create_diagnostics();
    diagnosticTree = neu_diagnostic_manager_get_initial_tree(diagnosticManager);
    icParams.mFirstSolve.mStatesPattern.mNumCol = 2;
    icParams.mFirstSolve.mStatesPattern.mNumRow = 2;
    ir[0] = 1;
    ir[1] = 0;
    icParams.mFirstSolve.mStatesPattern.mIr = &ir[0];
    jc[0] = 0;
    jc[1] = 1;
    jc[2] = 2;
    icParams.mFirstSolve.mStatesPattern.mJc = &jc[0];
    icParams.mFirstSolve.mStatesPr.mN = 2;
    mX[0] = 1.0;
    mX[1] = 1.0;
    icParams.mFirstSolve.mStatesPr.mX = &mX[0];
    icParams.mFirstSolve.mInputsPattern.mNumCol = 1;
    icParams.mFirstSolve.mInputsPattern.mNumRow = 2;
    jc_0[0] = 0;
    jc_0[1] = 0;
    icParams.mFirstSolve.mInputsPattern.mJc = &jc_0[0];
    icParams.mFirstSolve.mInputsPr.mN = 0;
    icParams.mFirstSolve.mConsts.mN = 2;
    mX_0[0] = 0.0;
    mX_0[1] = 0.0;
    icParams.mFirstSolve.mConsts.mX = &mX_0[0];
    icParams.mSecondSolve.mStatesPattern.mNumCol = 1;
    icParams.mSecondSolve.mStatesPattern.mNumRow = 1;
    ir_0 = 0;
    icParams.mSecondSolve.mStatesPattern.mIr = &ir_0;
    jc_1[0] = 0;
    jc_1[1] = 1;
    icParams.mSecondSolve.mStatesPattern.mJc = &jc_1[0];
    icParams.mSecondSolve.mStatesPr.mN = 1;
    mX_1 = 1.0;
    icParams.mSecondSolve.mStatesPr.mX = &mX_1;
    icParams.mSecondSolve.mInputsPattern.mNumCol = 1;
    icParams.mSecondSolve.mInputsPattern.mNumRow = 1;
    jc_2[0] = 0;
    jc_2[1] = 0;
    icParams.mSecondSolve.mInputsPattern.mJc = &jc_2[0];
    icParams.mSecondSolve.mInputsPr.mN = 0;
    icParams.mSecondSolve.mConsts.mN = 1;
    mX_2 = 0.0;
    icParams.mSecondSolve.mConsts.mX = &mX_2;
    icParams.mDcSolve.mAPattern.mNumCol = 1;
    icParams.mDcSolve.mAPattern.mNumRow = 1;
    ir_1 = 0;
    icParams.mDcSolve.mAPattern.mIr = &ir_1;
    jc_3[0] = 0;
    jc_3[1] = 1;
    icParams.mDcSolve.mAPattern.mJc = &jc_3[0];
    icParams.mDcSolve.mAPr.mN = 1;
    mX_3 = -4400.2560773036994;
    icParams.mDcSolve.mAPr.mX = &mX_3;
    icParams.mDcSolve.mBPattern.mNumCol = 1;
    icParams.mDcSolve.mBPattern.mNumRow = 1;
    ir_2 = 0;
    icParams.mDcSolve.mBPattern.mIr = &ir_2;
    jc_4[0] = 0;
    jc_4[1] = 1;
    icParams.mDcSolve.mBPattern.mJc = &jc_4[0];
    icParams.mDcSolve.mBPr.mN = 1;
    mX_4 = 1100.0640193259248;
    icParams.mDcSolve.mBPr.mX = &mX_4;
    icParams.mDcSolve.mFa.mN = 1;
    mX_5 = 0.0;
    icParams.mDcSolve.mFa.mX = &mX_5;
    icParams.mGuess.mN = 1;
    mX_6 = 0.0;
    icParams.mGuess.mX = &mX_6;
    icParams.mResTol = 0.001;
    icParams.mDoDC = false;
    resetSolver = cgIcSolve(&xPr, &uPr, &icParams, diagnosticManager);
    if (!resetSolver) {
      resetSolver = error_buffer_is_empty(rtmGetErrorStatus((&sdd_model_M)));
      if (resetSolver) {
        msg = rtw_diagnostics_msg(diagnosticTree);
        rtmSetErrorStatus((&sdd_model_M), msg);
      }
    }

    neu_destroy_diagnostic_manager(diagnosticManager);
    sdd_model_B.LTI_STATE_1 = x;
    sdd_model_X.sdd_modelElectricSchene1Inducto = x;
  } else {
    sdd_model_B.LTI_STATE_1 = sdd_model_X.sdd_modelElectricSchene1Inducto;
  }

  /* End of SimscapeLtiState: '<S19>/LTI_STATE_1' */

  /* SimscapeLtiOutput: '<S19>/LTI_OUTPUT_1_1' */
  sdd_model_B.LTI_OUTPUT_1_1 = 0.0;
  sdd_model_B.LTI_OUTPUT_1_1 += 0.99999999599999989 * sdd_model_B.LTI_STATE_1;
  sdd_model_B.LTI_OUTPUT_1_1 += 1.0000000272292198E-9 * sdd_model_B.INPUT_1_1_1
    [0];

  /* Outport: '<Root>/Output5' */
  sdd_model_Y.IOz = sdd_model_B.LTI_OUTPUT_1_1;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  /* Abs: '<S23>/Abs' */
  rtb_Product_k = std::abs(sdd_model_B.v);

  /* Signum: '<S23>/Sign' */
  if (sdd_model_B.v < 0.0) {
    tmp_0 = -1.0;
  } else if (sdd_model_B.v > 0.0) {
    tmp_0 = 1.0;
  } else if (sdd_model_B.v == 0.0) {
    tmp_0 = 0.0;
  } else {
    tmp_0 = (rtNaN);
  }

  /* End of Signum: '<S23>/Sign' */

  /* Gain: '<S5>/Gain' incorporates:
   *  Constant: '<S23>/k0'
   *  Constant: '<S23>/k1'
   *  Constant: '<S23>/k2'
   *  Product: '<S23>/Multiply'
   *  Product: '<S23>/Product'
   *  Product: '<S23>/Product1'
   *  Product: '<S23>/Product2'
   *  Sum: '<S23>/Sum1'
   *  Sum: '<S23>/Sum2'
   *  UnaryMinus: '<S23>/Unary Minus'
   */
  sdd_model_B.Gain = ((sdd_model_P.frictionLinearCoef * rtb_Product_k +
                       sdd_model_P.k0_Value) + rtb_Product_k * rtb_Product_k *
                      sdd_model_P.frictionQuadraticCoef) * -tmp_0 *
    sdd_model_P.frictionCoef;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  /* Product: '<S6>/Product' incorporates:
   *  Constant: '<S6>/BHorizontal'
   */
  sdd_model_B.Product[0] = sdd_model_P.Bh[0] * sdd_model_B.LTI_OUTPUT_1_1;
  sdd_model_B.Product[1] = sdd_model_P.Bh[1] * sdd_model_B.LTI_OUTPUT_1_1;
  sdd_model_B.Product[2] = sdd_model_P.Bh[2] * sdd_model_B.LTI_OUTPUT_1_1;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  /* Trigonometry: '<S5>/Cos' */
  rtb_Product_p = std::cos(rtb_Product_p);

  /* Product: '<S5>/Product' */
  rtb_Product_p *= sdd_model_B.Product[0];

  /* Product: '<S5>/Divide' incorporates:
   *  Constant: '<S5>/Constant'
   *  Sum: '<S5>/Sum'
   */
  sdd_model_B.Fb = (rtb_Product_p + sdd_model_B.Gain) *
    sdd_model_P.StructModelConst;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  /* Integrator: '<S4>/Integrator1' */
  sdd_model_B.s = sdd_model_X.Integrator1_CSTATE_i;

  /* Sum: '<S20>/Plus' incorporates:
   *  Abs: '<S20>/Abs'
   *  Constant: '<S20>/Constant1'
   */
  sdd_model_B.Plus_b = sdd_model_P.Constant1_Value_a - std::abs(sdd_model_B.s);

  /* Switch: '<S20>/Switch' incorporates:
   *  Constant: '<S20>/Constant3'
   *  Constant: '<S20>/Constant4'
   */
  if (sdd_model_B.Plus_b > sdd_model_P.Switch_Threshold_br) {
    sdd_model_B.signal_d = sdd_model_P.Constant3_Value_h;
  } else {
    sdd_model_B.signal_d = sdd_model_P.Constant4_Value_k;
  }

  /* End of Switch: '<S20>/Switch' */

  /* Signum: '<S20>/Sign' */
  if (sdd_model_B.s < 0.0) {
    tmp_0 = -1.0;
  } else if (sdd_model_B.s > 0.0) {
    tmp_0 = 1.0;
  } else if (sdd_model_B.s == 0.0) {
    tmp_0 = 0.0;
  } else {
    tmp_0 = (rtNaN);
  }

  /* End of Signum: '<S20>/Sign' */

  /* Signum: '<S20>/Sign1' */
  if (sdd_model_X.Integrator_CSTATE_f < 0.0) {
    rtb_Product_p = -1.0;
  } else if (sdd_model_X.Integrator_CSTATE_f > 0.0) {
    rtb_Product_p = 1.0;
  } else if (sdd_model_X.Integrator_CSTATE_f == 0.0) {
    rtb_Product_p = 0.0;
  } else {
    rtb_Product_p = (rtNaN);
  }

  /* End of Signum: '<S20>/Sign1' */

  /* Switch: '<S20>/Switch1' incorporates:
   *  Constant: '<S20>/Constant'
   *  Constant: '<S20>/Constant2'
   *  Product: '<S20>/Product'
   */
  if (tmp_0 * rtb_Product_p >= sdd_model_P.Switch1_Threshold_l) {
    tmp_0 = sdd_model_P.Constant_Value_l;
  } else {
    tmp_0 = sdd_model_P.Constant2_Value_b;
  }

  /* End of Switch: '<S20>/Switch1' */

  /* Product: '<S20>/Product1' */
  sdd_model_B.signal_l = tmp_0 * sdd_model_B.signal_d;

  /* Switch: '<S4>/Switch' incorporates:
   *  Constant: '<S4>/Constant1'
   *  Constant: '<S4>/Constant2'
   */
  if (tmp > sdd_model_P.Switch_Threshold_a) {
    sdd_model_B.Switch_k = sdd_model_P.Constant1_Value_m;
  } else {
    sdd_model_B.Switch_k = sdd_model_P.VX0;
  }

  /* End of Switch: '<S4>/Switch' */

  /* Integrator: '<S4>/Integrator' */
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    zcEvent = rt_ZCFcn(ANY_ZERO_CROSSING,
                       &sdd_model_PrevZCX.Integrator_Reset_ZCE_h,
                       (sdd_model_B.signal_l));

    /* evaluate zero-crossings and the level of the reset signal */
    if ((zcEvent != NO_ZCEVENT) || (sdd_model_B.signal_l != 0.0) ||
        (sdd_model_DW.Integrator_IWORK_c != 0)) {
      sdd_model_X.Integrator_CSTATE_f = sdd_model_B.Switch_k;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&sdd_model_M)->solverInfo,
        true);
      rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&sdd_model_M)
        ->solverInfo, true);
    }
  }

  sdd_model_B.v_l = sdd_model_X.Integrator_CSTATE_f;

  /* End of Integrator: '<S4>/Integrator' */

  /* Outport: '<Root>/Output' */
  sdd_model_Y.speedOX = sdd_model_B.v_l;

  /* Outport: '<Root>/Output1' */
  sdd_model_Y.positionOx = sdd_model_B.s;

  /* Gain: '<S1>/Gain1' incorporates:
   *  Inport: '<Root>/Input'
   */
  sdd_model_B.Gain1_j = sdd_model_P.Gain1_Gain_j * sdd_model_U.pwmOx;

  /* SimscapeInputBlock: '<S13>/INPUT_1_1_1' */
  sdd_model_B.INPUT_1_1_1_b[0] = sdd_model_B.Gain1_j;
  sdd_model_B.INPUT_1_1_1_b[1] = 0.0;
  sdd_model_B.INPUT_1_1_1_b[2] = 0.0;
  sdd_model_B.INPUT_1_1_1_b[3] = 0.0;

  /* SimscapeLtiState: '<S13>/LTI_STATE_1' */
  rtb_Product_k = sdd_model_X.sdd_modelElectricScheneInductor;
  if (sdd_model_DW.LTI_STATE_1_FirstOutput_k == 0.0) {
    sdd_model_DW.LTI_STATE_1_FirstOutput_k = 1.0;
    xPr_0.mN = 1;
    xPr_0.mX = &rtb_Product_k;
    u_0 = sdd_model_B.INPUT_1_1_1_b[0];
    uPr_0.mN = 1;
    uPr_0.mX = &u_0;
    diagnosticManager = rtw_create_diagnostics();
    diagnosticTree_0 = neu_diagnostic_manager_get_initial_tree(diagnosticManager);
    icParams_0.mFirstSolve.mStatesPattern.mNumCol = 2;
    icParams_0.mFirstSolve.mStatesPattern.mNumRow = 2;
    ir_3[0] = 1;
    ir_3[1] = 0;
    icParams_0.mFirstSolve.mStatesPattern.mIr = &ir_3[0];
    jc_5[0] = 0;
    jc_5[1] = 1;
    jc_5[2] = 2;
    icParams_0.mFirstSolve.mStatesPattern.mJc = &jc_5[0];
    icParams_0.mFirstSolve.mStatesPr.mN = 2;
    mX_7[0] = 1.0;
    mX_7[1] = 1.0;
    icParams_0.mFirstSolve.mStatesPr.mX = &mX_7[0];
    icParams_0.mFirstSolve.mInputsPattern.mNumCol = 1;
    icParams_0.mFirstSolve.mInputsPattern.mNumRow = 2;
    jc_6[0] = 0;
    jc_6[1] = 0;
    icParams_0.mFirstSolve.mInputsPattern.mJc = &jc_6[0];
    icParams_0.mFirstSolve.mInputsPr.mN = 0;
    icParams_0.mFirstSolve.mConsts.mN = 2;
    mX_8[0] = 0.0;
    mX_8[1] = 0.0;
    icParams_0.mFirstSolve.mConsts.mX = &mX_8[0];
    icParams_0.mSecondSolve.mStatesPattern.mNumCol = 1;
    icParams_0.mSecondSolve.mStatesPattern.mNumRow = 1;
    ir_4 = 0;
    icParams_0.mSecondSolve.mStatesPattern.mIr = &ir_4;
    jc_7[0] = 0;
    jc_7[1] = 1;
    icParams_0.mSecondSolve.mStatesPattern.mJc = &jc_7[0];
    icParams_0.mSecondSolve.mStatesPr.mN = 1;
    mX_9 = 1.0;
    icParams_0.mSecondSolve.mStatesPr.mX = &mX_9;
    icParams_0.mSecondSolve.mInputsPattern.mNumCol = 1;
    icParams_0.mSecondSolve.mInputsPattern.mNumRow = 1;
    jc_8[0] = 0;
    jc_8[1] = 0;
    icParams_0.mSecondSolve.mInputsPattern.mJc = &jc_8[0];
    icParams_0.mSecondSolve.mInputsPr.mN = 0;
    icParams_0.mSecondSolve.mConsts.mN = 1;
    mX_a = 0.0;
    icParams_0.mSecondSolve.mConsts.mX = &mX_a;
    icParams_0.mDcSolve.mAPattern.mNumCol = 1;
    icParams_0.mDcSolve.mAPattern.mNumRow = 1;
    ir_5 = 0;
    icParams_0.mDcSolve.mAPattern.mIr = &ir_5;
    jc_9[0] = 0;
    jc_9[1] = 1;
    icParams_0.mDcSolve.mAPattern.mJc = &jc_9[0];
    icParams_0.mDcSolve.mAPr.mN = 1;
    mX_b = -4400.2560773036994;
    icParams_0.mDcSolve.mAPr.mX = &mX_b;
    icParams_0.mDcSolve.mBPattern.mNumCol = 1;
    icParams_0.mDcSolve.mBPattern.mNumRow = 1;
    ir_6 = 0;
    icParams_0.mDcSolve.mBPattern.mIr = &ir_6;
    jc_a[0] = 0;
    jc_a[1] = 1;
    icParams_0.mDcSolve.mBPattern.mJc = &jc_a[0];
    icParams_0.mDcSolve.mBPr.mN = 1;
    mX_c = 1100.0640193259248;
    icParams_0.mDcSolve.mBPr.mX = &mX_c;
    icParams_0.mDcSolve.mFa.mN = 1;
    mX_d = 0.0;
    icParams_0.mDcSolve.mFa.mX = &mX_d;
    icParams_0.mGuess.mN = 1;
    mX_e = 0.0;
    icParams_0.mGuess.mX = &mX_e;
    icParams_0.mResTol = 0.001;
    icParams_0.mDoDC = false;
    resetSolver = cgIcSolve(&xPr_0, &uPr_0, &icParams_0, diagnosticManager);
    if (!resetSolver) {
      resetSolver = error_buffer_is_empty(rtmGetErrorStatus((&sdd_model_M)));
      if (resetSolver) {
        msg_0 = rtw_diagnostics_msg(diagnosticTree_0);
        rtmSetErrorStatus((&sdd_model_M), msg_0);
      }
    }

    neu_destroy_diagnostic_manager(diagnosticManager);
    sdd_model_B.LTI_STATE_1_g = rtb_Product_k;
    sdd_model_X.sdd_modelElectricScheneInductor = rtb_Product_k;
  } else {
    sdd_model_B.LTI_STATE_1_g = sdd_model_X.sdd_modelElectricScheneInductor;
  }

  /* End of SimscapeLtiState: '<S13>/LTI_STATE_1' */

  /* SimscapeLtiOutput: '<S13>/LTI_OUTPUT_1_1' */
  sdd_model_B.LTI_OUTPUT_1_1_h = 0.0;
  sdd_model_B.LTI_OUTPUT_1_1_h += 0.99999999599999989 *
    sdd_model_B.LTI_STATE_1_g;
  sdd_model_B.LTI_OUTPUT_1_1_h += 1.0000000272292198E-9 *
    sdd_model_B.INPUT_1_1_1_b[0];

  /* Outport: '<Root>/Output4' */
  sdd_model_Y.IOx = sdd_model_B.LTI_OUTPUT_1_1_h;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    /* UnaryMinus: '<Root>/Unary Minus' incorporates:
     *  Constant: '<Root>/Constant2'
     */
    rtb_UnaryMinus_m = -sdd_model_P.Constant2_Value_g;
  }

  /* Product: '<S3>/Product' incorporates:
   *  Constant: '<S3>/BHorizontal'
   */
  sdd_model_B.Product_j[0] = sdd_model_P.Bh[0] * sdd_model_B.LTI_OUTPUT_1_1_h;
  sdd_model_B.Product_j[1] = sdd_model_P.Bh[1] * sdd_model_B.LTI_OUTPUT_1_1_h;
  sdd_model_B.Product_j[2] = sdd_model_P.Bh[2] * sdd_model_B.LTI_OUTPUT_1_1_h;

  /* Abs: '<S21>/Abs' */
  rtb_Product_k = std::abs(sdd_model_B.v_l);

  /* Signum: '<S21>/Sign' */
  if (sdd_model_B.v_l < 0.0) {
    tmp = -1.0;
  } else if (sdd_model_B.v_l > 0.0) {
    tmp = 1.0;
  } else if (sdd_model_B.v_l == 0.0) {
    tmp = 0.0;
  } else {
    tmp = (rtNaN);
  }

  /* End of Signum: '<S21>/Sign' */

  /* Gain: '<S4>/Gain' incorporates:
   *  Constant: '<S21>/k0'
   *  Constant: '<S21>/k1'
   *  Constant: '<S21>/k2'
   *  Product: '<S21>/Multiply'
   *  Product: '<S21>/Product'
   *  Product: '<S21>/Product1'
   *  Product: '<S21>/Product2'
   *  Sum: '<S21>/Sum1'
   *  Sum: '<S21>/Sum2'
   *  UnaryMinus: '<S21>/Unary Minus'
   */
  sdd_model_B.Gain_o = ((sdd_model_P.frictionLinearCoef * rtb_Product_k +
    sdd_model_P.k0_Value_m) + rtb_Product_k * rtb_Product_k *
                        sdd_model_P.frictionQuadraticCoef) * -tmp *
    sdd_model_P.frictionCoef;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  /* Product: '<S4>/Divide' incorporates:
   *  Constant: '<S4>/Constant'
   *  Product: '<S4>/Product'
   *  Sum: '<S4>/Sum'
   *  Trigonometry: '<S4>/Cos'
   */
  sdd_model_B.Fb_g = (sdd_model_B.Product_j[0] * std::cos(sdd_model_B.s) +
                      sdd_model_B.Gain_o) * sdd_model_P.StructModelConst;
  if (rtmIsMajorTimeStep((&sdd_model_M))) {
  }

  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    /* Matfile logging */
    rt_UpdateTXYLogVars((&sdd_model_M)->rtwLogInfo, ((&sdd_model_M)->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    /* Update for Integrator: '<S5>/Integrator' */
    sdd_model_DW.Integrator_IWORK = 0;

    /* Update for Integrator: '<S4>/Integrator' */
    sdd_model_DW.Integrator_IWORK_c = 0;

    /* ContTimeOutputInconsistentWithStateAtMajorOutputFlag is set, need to run a minor output */
    if (rtmIsMajorTimeStep((&sdd_model_M))) {
      if (rtsiGetContTimeOutputInconsistentWithStateAtMajorStep(&(&sdd_model_M
           )->solverInfo)) {
        rtsiSetSimTimeStep(&(&sdd_model_M)->solverInfo,MINOR_TIME_STEP);
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&sdd_model_M)
          ->solverInfo, false);
        sdd_modelModelClass::step();
        rtsiSetSimTimeStep(&(&sdd_model_M)->solverInfo, MAJOR_TIME_STEP);
      }
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&sdd_model_M))) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal((&sdd_model_M))!=-1) &&
          !((rtmGetTFinal((&sdd_model_M))-((((&sdd_model_M)->Timing.clockTick1+(
                &sdd_model_M)->Timing.clockTickH1* 4294967296.0)) * 1.0E-5)) >
            ((((&sdd_model_M)->Timing.clockTick1+(&sdd_model_M)
               ->Timing.clockTickH1* 4294967296.0)) * 1.0E-5) * (DBL_EPSILON)))
      {
        rtmSetErrorStatus((&sdd_model_M), "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&(&sdd_model_M)->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++(&sdd_model_M)->Timing.clockTick0)) {
      ++(&sdd_model_M)->Timing.clockTickH0;
    }

    (&sdd_model_M)->Timing.t[0] = rtsiGetSolverStopTime(&(&sdd_model_M)
      ->solverInfo);

    {
      /* Update absolute timer for sample time: [1.0E-5s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 1.0E-5, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      (&sdd_model_M)->Timing.clockTick1++;
      if (!(&sdd_model_M)->Timing.clockTick1) {
        (&sdd_model_M)->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void sdd_modelModelClass::sdd_model_derivatives()
{
  XDot_sdd_model_T *_rtXdot;
  _rtXdot = ((XDot_sdd_model_T *) (&sdd_model_M)->derivs);

  /* Derivatives for Integrator: '<S5>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = sdd_model_B.v;

  /* Derivatives for Integrator: '<S5>/Integrator' */
  if (sdd_model_B.signal_m == 0.0) {
    _rtXdot->Integrator_CSTATE = sdd_model_B.Fb;
  } else {
    /* level reset is active */
    _rtXdot->Integrator_CSTATE = 0.0;
  }

  /* End of Derivatives for Integrator: '<S5>/Integrator' */

  /* Derivatives for SimscapeLtiState: '<S19>/LTI_STATE_1' */
  _rtXdot->sdd_modelElectricSchene1Inducto = 0.0;
  _rtXdot->sdd_modelElectricSchene1Inducto += -4400.2560773036994 *
    sdd_model_X.sdd_modelElectricSchene1Inducto;
  _rtXdot->sdd_modelElectricSchene1Inducto += 1100.0640193259248 *
    sdd_model_B.INPUT_1_1_1[0];

  /* Derivatives for Integrator: '<S4>/Integrator1' */
  _rtXdot->Integrator1_CSTATE_i = sdd_model_B.v_l;

  /* Derivatives for Integrator: '<S4>/Integrator' */
  if (sdd_model_B.signal_l == 0.0) {
    _rtXdot->Integrator_CSTATE_f = sdd_model_B.Fb_g;
  } else {
    /* level reset is active */
    _rtXdot->Integrator_CSTATE_f = 0.0;
  }

  /* End of Derivatives for Integrator: '<S4>/Integrator' */

  /* Derivatives for SimscapeLtiState: '<S13>/LTI_STATE_1' */
  _rtXdot->sdd_modelElectricScheneInductor = 0.0;
  _rtXdot->sdd_modelElectricScheneInductor += -4400.2560773036994 *
    sdd_model_X.sdd_modelElectricScheneInductor;
  _rtXdot->sdd_modelElectricScheneInductor += 1100.0640193259248 *
    sdd_model_B.INPUT_1_1_1_b[0];
}

/* Model initialize function */
void sdd_modelModelClass::initialize()
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&sdd_model_M)->solverInfo, &(&sdd_model_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&sdd_model_M)->solverInfo, &rtmGetTPtr((&sdd_model_M)));
    rtsiSetStepSizePtr(&(&sdd_model_M)->solverInfo, &(&sdd_model_M)
                       ->Timing.stepSize0);
    rtsiSetdXPtr(&(&sdd_model_M)->solverInfo, &(&sdd_model_M)->derivs);
    rtsiSetContStatesPtr(&(&sdd_model_M)->solverInfo, (real_T **) &(&sdd_model_M)
                         ->contStates);
    rtsiSetNumContStatesPtr(&(&sdd_model_M)->solverInfo, &(&sdd_model_M)
      ->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&sdd_model_M)->solverInfo, &(&sdd_model_M
      )->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&sdd_model_M)->solverInfo,
      &(&sdd_model_M)->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&sdd_model_M)->solverInfo,
      &(&sdd_model_M)->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&sdd_model_M)->solverInfo, (&rtmGetErrorStatus
      ((&sdd_model_M))));
    rtsiSetRTModelPtr(&(&sdd_model_M)->solverInfo, (&sdd_model_M));
  }

  rtsiSetSimTimeStep(&(&sdd_model_M)->solverInfo, MAJOR_TIME_STEP);
  (&sdd_model_M)->intgData.y = (&sdd_model_M)->odeY;
  (&sdd_model_M)->intgData.f[0] = (&sdd_model_M)->odeF[0];
  (&sdd_model_M)->intgData.f[1] = (&sdd_model_M)->odeF[1];
  (&sdd_model_M)->intgData.f[2] = (&sdd_model_M)->odeF[2];
  (&sdd_model_M)->intgData.f[3] = (&sdd_model_M)->odeF[3];
  (&sdd_model_M)->contStates = ((X_sdd_model_T *) &sdd_model_X);
  rtsiSetSolverData(&(&sdd_model_M)->solverInfo, static_cast<void *>
                    (&(&sdd_model_M)->intgData));
  rtsiSetSolverName(&(&sdd_model_M)->solverInfo,"ode4");
  rtmSetTPtr((&sdd_model_M), &(&sdd_model_M)->Timing.tArray[0]);
  rtmSetTFinal((&sdd_model_M), 0.1);
  (&sdd_model_M)->Timing.stepSize0 = 1.0E-5;
  rtmSetFirstInitCond((&sdd_model_M), 1);

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    (&sdd_model_M)->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo((&sdd_model_M)->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs((&sdd_model_M)->rtwLogInfo, (NULL));
    rtliSetLogT((&sdd_model_M)->rtwLogInfo, "tout");
    rtliSetLogX((&sdd_model_M)->rtwLogInfo, "");
    rtliSetLogXFinal((&sdd_model_M)->rtwLogInfo, "");
    rtliSetLogVarNameModifier((&sdd_model_M)->rtwLogInfo, "rt_");
    rtliSetLogFormat((&sdd_model_M)->rtwLogInfo, 4);
    rtliSetLogMaxRows((&sdd_model_M)->rtwLogInfo, 0);
    rtliSetLogDecimation((&sdd_model_M)->rtwLogInfo, 1);
    rtliSetLogY((&sdd_model_M)->rtwLogInfo, "");
    rtliSetLogYSignalInfo((&sdd_model_M)->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs((&sdd_model_M)->rtwLogInfo, (NULL));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime((&sdd_model_M)->rtwLogInfo, 0.0, rtmGetTFinal
    ((&sdd_model_M)), (&sdd_model_M)->Timing.stepSize0, (&rtmGetErrorStatus
    ((&sdd_model_M))));
  sdd_model_PrevZCX.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;
  sdd_model_PrevZCX.Integrator_Reset_ZCE_h = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for Integrator: '<S5>/Integrator1' */
  sdd_model_X.Integrator1_CSTATE = sdd_model_P.degZ0;

  /* InitializeConditions for Integrator: '<S5>/Integrator' incorporates:
   *  Integrator: '<S4>/Integrator'
   */
  if (rtmIsFirstInitCond((&sdd_model_M))) {
    sdd_model_X.Integrator_CSTATE = 0.0;
    sdd_model_X.Integrator_CSTATE_f = 0.0;
  }

  sdd_model_DW.Integrator_IWORK = 1;

  /* End of InitializeConditions for Integrator: '<S5>/Integrator' */

  /* InitializeConditions for Integrator: '<S4>/Integrator1' */
  sdd_model_X.Integrator1_CSTATE_i = sdd_model_P.degX0;

  /* InitializeConditions for Integrator: '<S4>/Integrator' */
  sdd_model_DW.Integrator_IWORK_c = 1;

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond((&sdd_model_M))) {
    rtmSetFirstInitCond((&sdd_model_M), 0);
  }
}

/* Model terminate function */
void sdd_modelModelClass::terminate()
{
  /* (no terminate code required) */
}

/* Constructor */
sdd_modelModelClass::sdd_modelModelClass():
  sdd_model_B()
  ,sdd_model_DW()
  ,sdd_model_X()
  ,sdd_model_PrevZCX()
  ,sdd_model_U()
  ,sdd_model_Y()
  ,sdd_model_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
sdd_modelModelClass::~sdd_modelModelClass()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_sdd_model_T * sdd_modelModelClass::getRTM()
{
  return (&sdd_model_M);
}
