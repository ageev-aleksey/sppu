/*
 * sdd_model_data.cpp
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

/* Block parameters (default storage) */
P_sdd_model_T sdd_modelModelClass::sdd_model_P = {
  /* Variable: Bh
   * Referenced by:
   *   '<S3>/BHorizontal'
   *   '<S6>/BHorizontal'
   */
  { -0.0006307335252346937, 1.5562328515170811E-8, -6.15283106903175E-8 },

  /* Variable: StructModelConst
   * Referenced by:
   *   '<S4>/Constant'
   *   '<S5>/Constant'
   */
  15378.187221311295,

  /* Variable: VX0
   * Referenced by: '<S4>/Constant2'
   */
  0.0,

  /* Variable: VZ0
   * Referenced by: '<S5>/Constant2'
   */
  0.0,

  /* Variable: degX0
   * Referenced by: '<S4>/Integrator1'
   */
  0.0,

  /* Variable: degZ0
   * Referenced by: '<S5>/Integrator1'
   */
  0.0,

  /* Variable: frictionCoef
   * Referenced by:
   *   '<S4>/Gain'
   *   '<S5>/Gain'
   */
  1.0E-5,

  /* Variable: frictionLinearCoef
   * Referenced by:
   *   '<S21>/k1'
   *   '<S23>/k1'
   */
  1.0,

  /* Variable: frictionQuadraticCoef
   * Referenced by:
   *   '<S21>/k2'
   *   '<S23>/k2'
   */
  0.001,

  /* Expression: 1
   * Referenced by: '<S22>/Constant'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S22>/Constant2'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S22>/Switch1'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S22>/Constant3'
   */
  0.0,

  /* Expression: pi/6
   * Referenced by: '<S22>/Constant1'
   */
  0.52359877559829882,

  /* Expression: 1
   * Referenced by: '<S22>/Constant4'
   */
  1.0,

  /* Expression: 0.001
   * Referenced by: '<S22>/Switch'
   */
  0.001,

  /* Expression: 0
   * Referenced by: '<S5>/Constant1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S5>/Switch'
   */
  0.0,

  /* Expression: 12
   * Referenced by: '<S2>/Gain1'
   */
  12.0,

  /* Expression: 0
   * Referenced by: '<S23>/k0'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S20>/Constant'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S20>/Constant2'
   */
  0.0,

  /* Expression: 1
   * Referenced by: '<S20>/Switch1'
   */
  1.0,

  /* Expression: 0
   * Referenced by: '<S20>/Constant3'
   */
  0.0,

  /* Expression: pi/6
   * Referenced by: '<S20>/Constant1'
   */
  0.52359877559829882,

  /* Expression: 1
   * Referenced by: '<S20>/Constant4'
   */
  1.0,

  /* Expression: 0.001
   * Referenced by: '<S20>/Switch'
   */
  0.001,

  /* Expression: 0
   * Referenced by: '<S4>/Constant1'
   */
  0.0,

  /* Expression: 0
   * Referenced by: '<S4>/Switch'
   */
  0.0,

  /* Expression: 12
   * Referenced by: '<S1>/Gain1'
   */
  12.0,

  /* Expression: pi/6
   * Referenced by: '<Root>/Constant2'
   */
  0.52359877559829882,

  /* Expression: 0
   * Referenced by: '<S21>/k0'
   */
  0.0
};


P_sdd_model_T sdd_modelModelClass::defaultValue = sdd_modelModelClass::sdd_model_P;