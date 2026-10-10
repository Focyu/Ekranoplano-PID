/*
 * pid_control.cpp
 *
 * Trial License - for use to evaluate programs for possible purchase as
 * an end-user only.
 *
 * Code generation for model "pid_control".
 *
 * Model version              : 13.20
 * Simulink Coder version : 26.2 (R2026b) 22-May-2026
 * C++ source code generated on : Sat Oct 10 16:43:23 2026
 *
 * Target selection: ert.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "pid_control.h"
#include "slros2_initialize.h"
#include "rtwtypes.h"
#include "pid_control_types.h"
#include "rmw/qos_profiles.h"
#include <string.h>
#include "pid_control_private.h"
#include <emmintrin.h>
#include <math.h>

extern "C"
{

#include "rt_nonfinite.h"

}

#include <stddef.h>
#include "zero_crossing_types.h"
#include <float.h>
#include "rt_defines.h"

real_T look1_binlxpw(real_T u0, const real_T bp0[], const real_T table[],
                     uint32_T maxIndex)
{
  real_T frac;
  real_T yL_0d0;
  uint32_T iLeft;

  /* Column-major Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear point-slope'
     Extrapolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Linear'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u0 <= bp0[0U]) {
    iLeft = 0U;
    frac = (u0 - bp0[0U]) / (bp0[1U] - bp0[0U]);
  } else if (u0 < bp0[maxIndex]) {
    uint32_T bpIdx;
    uint32_T iRght;

    /* Binary Search */
    bpIdx = maxIndex >> 1U;
    iLeft = 0U;
    iRght = maxIndex;
    while (iRght - iLeft > 1U) {
      if (u0 < bp0[bpIdx]) {
        iRght = bpIdx;
      } else {
        iLeft = bpIdx;
      }

      bpIdx = (iRght + iLeft) >> 1U;
    }

    frac = (u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft]);
  } else {
    iLeft = maxIndex - 1U;
    frac = (u0 - bp0[maxIndex - 1U]) / (bp0[maxIndex] - bp0[maxIndex - 1U]);
  }

  /* Column-major Interpolation 1-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'portable wrapping'
   */
  yL_0d0 = table[iLeft];
  return (table[iLeft + 1U] - yL_0d0) * frac + yL_0d0;
}

uint32_T plook_bincpa(real_T u, const real_T bp[], uint32_T maxIndex, real_T
                      *fraction, uint32_T *prevIndex)
{
  uint32_T bpIndex;

  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Extrapolation method: 'Clip'
     Use previous index: 'on'
     Use last breakpoint for index at or above upper limit: 'on'
     Remove protection against out-of-range input in generated code: 'off'
   */
  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = 0.0;
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d_prevIdx(u, bp, *prevIndex, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex;
    *fraction = 0.0;
  }

  *prevIndex = bpIndex;
  return bpIndex;
}

real_T intrp2d_la_pw(const uint32_T bpIndex[], const real_T frac[], const real_T
                     table[], const uint32_T stride, const uint32_T maxIndex[])
{
  real_T y;
  real_T yL_0d0;
  uint32_T offset_1d;

  /* Column-major Interpolation 2-D
     Interpolation method: 'Linear point-slope'
     Use last breakpoint for index at or above upper limit: 'on'
     Overflow mode: 'portable wrapping'
   */
  offset_1d = bpIndex[1U] * stride + bpIndex[0U];
  if (bpIndex[0U] == maxIndex[0U]) {
    y = table[offset_1d];
  } else {
    yL_0d0 = table[offset_1d];
    y = (table[offset_1d + 1U] - yL_0d0) * frac[0U] + yL_0d0;
  }

  if (bpIndex[1U] == maxIndex[1U]) {
  } else {
    offset_1d += stride;
    if (bpIndex[0U] == maxIndex[0U]) {
      yL_0d0 = table[offset_1d];
    } else {
      yL_0d0 = table[offset_1d];
      yL_0d0 += (table[offset_1d + 1U] - yL_0d0) * frac[0U];
    }

    y += (yL_0d0 - y) * frac[1U];
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

  /* Binary Search using Previous Index */
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

/*
 * This function updates continuous states using the ODE4 fixed-step
 * solver algorithm
 */
void pid_control::rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
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
  int_T nXc = 52;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                static_cast<uint_T>(nXc)*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  pid_control_derivatives();

  /* f1 = f(t + (h/2), y + (h/2)*f0) */
  temp = 0.5 * h;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f0[i]);
  }

  rtsiSetT(si, t + temp);
  rtsiSetdX(si, f1);
  this->step();
  pid_control_derivatives();

  /* f2 = f(t + (h/2), y + (h/2)*f1) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (temp*f1[i]);
  }

  rtsiSetdX(si, f2);
  this->step();
  pid_control_derivatives();

  /* f3 = f(t + h, y + h*f2) */
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (h*f2[i]);
  }

  rtsiSetT(si, tnew);
  rtsiSetdX(si, f3);
  this->step();
  pid_control_derivatives();

  /* tnew = t + h
     ynew = y + (h/6)*(f0 + 2*f1 + 2*f2 + 2*f3) */
  temp = h / 6.0;
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + temp*(f0[i] + 2.0*f1[i] + 2.0*f2[i] + f3[i]);
  }

  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/*
 * Output and update for atomic system:
 *    '<Root>/MATLAB Function-ned_to_PoseStamped'
 *    '<Root>/MATLAB Function-ned_to_TwistStamped'
 */
void pid_control::MATLABFunctionned_to_PoseStampe
  (B_MATLABFunctionned_to_PoseSt_T *localB)
{
  static const uint8_T b[5] = { 119U, 111U, 114U, 108U, 100U };

  memset(&localB->stringOut[0], 0, sizeof(uint8_T) << 7U);
  for (int32_T i = 0; i < 5; i++) {
    localB->stringOut[i] = b[i];
  }

  localB->lengthOut = 5U;
}

/*
 * System initialize for enable system:
 *    '<S156>/Enabled Subsystem'
 *    '<S159>/Enabled Subsystem'
 */
void pid_control::pid_contr_EnabledSubsystem_Init
  (B_EnabledSubsystem_pid_contro_T *localB)
{
  /* SystemInitialize for SignalConversion generated from: '<S199>/In1' */
  memset(&localB->In1, 0, sizeof(SL_Bus_std_msgs_Bool));
}

/*
 * Output and update for enable system:
 *    '<S156>/Enabled Subsystem'
 *    '<S159>/Enabled Subsystem'
 */
void pid_control::pid_control_EnabledSubsystem(boolean_T rtu_Enable, const
  SL_Bus_std_msgs_Bool *rtu_In1, B_EnabledSubsystem_pid_contro_T *localB)
{
  /* Outputs for Enabled SubSystem: '<S156>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S199>/Enable'
   */
  if (rtu_Enable) {
    /* SignalConversion generated from: '<S199>/In1' */
    localB->In1 = *rtu_In1;
  }

  /* End of Outputs for SubSystem: '<S156>/Enabled Subsystem' */
}

/*
 * System initialize for enable system:
 *    '<S157>/Enabled Subsystem'
 *    '<S158>/Enabled Subsystem'
 *    '<S35>/Enabled Subsystem'
 *    '<S36>/Enabled Subsystem'
 */
void pid_control::pid_con_EnabledSubsystem_p_Init
  (B_EnabledSubsystem_pid_cont_d_T *localB)
{
  /* SystemInitialize for SignalConversion generated from: '<S200>/In1' */
  memset(&localB->In1, 0, sizeof(SL_Bus_std_msgs_Float64));
}

/*
 * Output and update for enable system:
 *    '<S157>/Enabled Subsystem'
 *    '<S158>/Enabled Subsystem'
 *    '<S35>/Enabled Subsystem'
 *    '<S36>/Enabled Subsystem'
 */
void pid_control::pid_control_EnabledSubsystem_k(boolean_T rtu_Enable, const
  SL_Bus_std_msgs_Float64 *rtu_In1, B_EnabledSubsystem_pid_cont_d_T *localB)
{
  /* Outputs for Enabled SubSystem: '<S157>/Enabled Subsystem' incorporates:
   *  EnablePort: '<S200>/Enable'
   */
  if (rtu_Enable) {
    /* SignalConversion generated from: '<S200>/In1' */
    localB->In1 = *rtu_In1;
  }

  /* End of Outputs for SubSystem: '<S157>/Enabled Subsystem' */
}

void pid_control::pid_control_Publisher_setupImpl(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/ekranoplano/imu";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S27>/SinkBlock' */
  pid_control_B.deadline_j.sec = 0.0;
  pid_control_B.deadline_j.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_j,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S27>/SinkBlock' */
    pid_control_B.b_zeroDelimTopic_f[i] = b_zeroDelimTopic[i];
  }

  Pub_pid_control_548.createPublisher(&pid_control_B.b_zeroDelimTopic_f[0],
    qos_profile);
}

rmw_qos_profile_t pid_control::pi_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_t
  rmwProfile, real_T qosDepth, real_T qosDeadline, real_T qosLifespan, real_T
  qosLeaseDuration, boolean_T qosAvoidROSNamespaceConventions)
{
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  real_T input;
  real_T sec;
  input = qosDeadline;

  /* Start for MATLABSystem: '<S28>/SinkBlock' */
  if (qosDeadline == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  pid_control_B.deadline_dh.sec = sec;
  pid_control_B.deadline_dh.nsec = (input - sec) * 1.0E+9;
  input = qosLifespan;

  /* Start for MATLABSystem: '<S28>/SinkBlock' */
  if (qosLifespan == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  lifespan.sec = sec;
  lifespan.nsec = (input - sec) * 1.0E+9;
  input = qosLeaseDuration;

  /* Start for MATLABSystem: '<S28>/SinkBlock' */
  if (qosLeaseDuration == (rtInf)) {
    input = 0.0;
  }

  sec = floor(input);
  liveliness_lease_duration.sec = sec;
  liveliness_lease_duration.nsec = (input - sec) * 1.0E+9;
  SET_QOS_VALUES(rmwProfile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(qosDepth),
                 RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_dh,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 qosAvoidROSNamespaceConventions);
  return rmwProfile;
}

void pid_control::pid_contr_Publisher_setupImpl_m(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[25] = "/ekranoplano/PoseStamped";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S29>/SinkBlock' */
  pid_control_B.deadline_g1.sec = 0.0;
  pid_control_B.deadline_g1.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_g1,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 25; i++) {
    /* Start for MATLABSystem: '<S29>/SinkBlock' */
    pid_control_B.b_zeroDelimTopic_c[i] = b_zeroDelimTopic[i];
  }

  Pub_pid_control_811.createPublisher(&pid_control_B.b_zeroDelimTopic_c[0],
    qos_profile);
}

void pid_control::pid_cont_Publisher_setupImpl_m3(const
  ros_slros2_internal_block_Pub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[26] = "/ekranoplano/TwistStamped";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S30>/SinkBlock' */
  pid_control_B.deadline.sec = 0.0;
  pid_control_B.deadline.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST, static_cast<
                 size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 26; i++) {
    /* Start for MATLABSystem: '<S30>/SinkBlock' */
    pid_control_B.b_zeroDelimTopic[i] = b_zeroDelimTopic[i];
  }

  Pub_pid_control_835.createPublisher(&pid_control_B.b_zeroDelimTopic[0],
    qos_profile);
}

void pid_control::pid_c_Subscriber_setupImpl_m3hf(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/setpoint/altura";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S35>/SourceBlock' */
  pid_control_B.deadline_l.sec = 0.0;
  pid_control_B.deadline_l.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_l,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_435.setAsyncEnabled("off", "null");
  Sub_pid_control_435.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S35>/SourceBlock' */
    pid_control_B.b_zeroDelimTopic_cv[i] = b_zeroDelimTopic[i];
  }

  Sub_pid_control_435.createSubscriber(&pid_control_B.b_zeroDelimTopic_cv[0],
    qos_profile);
}

void pid_control::pid__Subscriber_setupImpl_m3hfp(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  char_T b_zeroDelimTopic[14];
  static const char_T b_zeroDelimTopic_0[14] = "/setpoint/yaw";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S36>/SourceBlock' */
  pid_control_B.deadline_d.sec = 0.0;
  pid_control_B.deadline_d.nsec = 0.0;
  pid_control_B.lifespan.sec = 0.0;
  pid_control_B.lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_d,
                 pid_control_B.lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_377.setAsyncEnabled("off", "null");
  Sub_pid_control_377.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 14; i++) {
    /* Start for MATLABSystem: '<S36>/SourceBlock' */
    b_zeroDelimTopic[i] = b_zeroDelimTopic_0[i];
  }

  Sub_pid_control_377.createSubscriber(&b_zeroDelimTopic[0], qos_profile);
}

void pid_control::pid_con_ServiceCaller_setupImpl
  (ros_slros2_internal_block_Ser_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[25] = "/gazebo/set_entity_state";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S6>/ServiceCaller' */
  pid_control_B.deadline_g.sec = 0.0;
  pid_control_B.deadline_g.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(1.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_g,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  for (int32_T i = 0; i < 25; i++) {
    /* Start for MATLABSystem: '<S6>/ServiceCaller' */
    pid_control_B.b_zeroDelimTopic_m[i] = b_zeroDelimTopic[i];
  }

  ServCall_pid_control_326.createServiceCaller
    (&pid_control_B.b_zeroDelimTopic_m[0], qos_profile);
}

void pid_control::pid_contro_Subscriber_setupImpl(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[22] = "/setpoint/turbulencia";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S156>/SourceBlock' */
  pid_control_B.deadline_n.sec = 0.0;
  pid_control_B.deadline_n.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_n,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_423.setAsyncEnabled("off", "null");
  Sub_pid_control_423.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 22; i++) {
    /* Start for MATLABSystem: '<S156>/SourceBlock' */
    pid_control_B.b_zeroDelimTopic_cx[i] = b_zeroDelimTopic[i];
  }

  Sub_pid_control_423.createSubscriber(&pid_control_B.b_zeroDelimTopic_cx[0],
    qos_profile);
}

void pid_control::pid_con_Subscriber_setupImpl_m3(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[17] = "/olas/pitch_rate";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S158>/SourceBlock' */
  pid_control_B.deadline_p.sec = 0.0;
  pid_control_B.deadline_p.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_p,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_445.setAsyncEnabled("off", "null");
  Sub_pid_control_445.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 17; i++) {
    /* Start for MATLABSystem: '<S158>/SourceBlock' */
    pid_control_B.b_zeroDelimTopic_p[i] = b_zeroDelimTopic[i];
  }

  Sub_pid_control_445.createSubscriber(&pid_control_B.b_zeroDelimTopic_p[0],
    qos_profile);
}

void pid_control::pid_co_Subscriber_setupImpl_m3h(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  static const char_T b_zeroDelimTopic[22] = "/setpoint/turbulencia";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S159>/SourceBlock' */
  pid_control_B.deadline_m.sec = 0.0;
  pid_control_B.deadline_m.nsec = 0.0;
  lifespan.sec = 0.0;
  lifespan.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_m,
                 lifespan, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_417.setAsyncEnabled("off", "null");
  Sub_pid_control_417.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 22; i++) {
    /* Start for MATLABSystem: '<S159>/SourceBlock' */
    pid_control_B.b_zeroDelimTopic_k[i] = b_zeroDelimTopic[i];
  }

  Sub_pid_control_417.createSubscriber(&pid_control_B.b_zeroDelimTopic_k[0],
    qos_profile);
}

void pid_control::pid_cont_Subscriber_setupImpl_m(const
  ros_slros2_internal_block_Sub_T *obj)
{
  rmw_qos_profile_t qos_profile;
  sJ4ih70VmKcvCeguWN0mNVF liveliness_lease_duration;
  char_T b_zeroDelimTopic[12];
  static const char_T b_zeroDelimTopic_0[12] = "/olas/heave";
  qos_profile = rmw_qos_profile_default;

  /* Start for MATLABSystem: '<S157>/SourceBlock' */
  pid_control_B.deadline_gu.sec = 0.0;
  pid_control_B.deadline_gu.nsec = 0.0;
  pid_control_B.lifespan_l.sec = 0.0;
  pid_control_B.lifespan_l.nsec = 0.0;
  liveliness_lease_duration.sec = 0.0;
  liveliness_lease_duration.nsec = 0.0;
  SET_QOS_VALUES(qos_profile, RMW_QOS_POLICY_HISTORY_KEEP_LAST,
                 static_cast<size_t>(10.0), RMW_QOS_POLICY_DURABILITY_VOLATILE,
                 RMW_QOS_POLICY_RELIABILITY_RELIABLE, pid_control_B.deadline_gu,
                 pid_control_B.lifespan_l, RMW_QOS_POLICY_LIVELINESS_AUTOMATIC,
                 liveliness_lease_duration, (bool)
                 obj->QOSAvoidROSNamespaceConventions);
  Sub_pid_control_443.setAsyncEnabled("off", "null");
  Sub_pid_control_443.setCallbackGroup("default", "callback_group_1");
  for (int32_T i = 0; i < 12; i++) {
    /* Start for MATLABSystem: '<S157>/SourceBlock' */
    b_zeroDelimTopic[i] = b_zeroDelimTopic_0[i];
  }

  Sub_pid_control_443.createSubscriber(&b_zeroDelimTopic[0], qos_profile);
}

real_T rt_urand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  uint32_T hi;
  uint32_T lo;

  /* Uniform random number generator (random number between 0 and 1)

     #define IA      16807                      magic multiplier = 7^5
     #define IM      2147483647                 modulus = 2^31-1
     #define IQ      127773                     IM div IA
     #define IR      2836                       IM modulo IA
     #define S       4.656612875245797e-10      reciprocal of 2^31-1
     test = IA * (seed % IQ) - IR * (seed/IQ)
     seed = test < 0 ? (test + IM) : test
     return (seed*S)
   */
  lo = *u % 127773U * 16807U;
  hi = *u / 127773U * 2836U;
  if (lo < hi) {
    *u = 2147483647U - (hi - lo);
  } else {
    *u = lo - hi;
  }

  return static_cast<real_T>(*u) * 4.656612875245797E-10;
}

real_T rt_nrand_Upu32_Yd_f_pw_snf(uint32_T *u)
{
  real_T si;
  real_T sr;
  real_T y;

  /* Normal (Gaussian) random number generator */
  do {
    sr = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = 2.0 * rt_urand_Upu32_Yd_f_pw_snf(u) - 1.0;
    si = sr * sr + si * si;
  } while (si > 1.0);

  y = sqrt(-2.0 * log(si) / si) * sr;
  return y;
}

real_T rt_roundd_snf(real_T u)
{
  real_T y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

real_T rt_remd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1) || rtIsInf(u0)) {
    y = (rtNaN);
  } else if (rtIsInf(u1)) {
    y = u0;
  } else {
    real_T q;
    if (u1 < 0.0) {
      q = ceil(u1);
    } else {
      q = floor(u1);
    }

    if ((u1 != 0.0) && (u1 != q)) {
      q = fabs(u0 / u1);
      if (!(fabs(q - floor(q + 0.5)) > DBL_EPSILON * q)) {
        y = 0.0 * u0;
      } else {
        y = fmod(u0, u1);
      }
    } else {
      y = fmod(u0, u1);
    }
  }

  return y;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = atan2(static_cast<real_T>(tmp), static_cast<real_T>(tmp_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

real_T rt_powd_snf(real_T u0, real_T u1)
{
  real_T y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else {
    real_T tmp;
    real_T tmp_0;
    tmp = fabs(u0);
    tmp_0 = fabs(u1);
    if (rtIsInf(u1)) {
      if (tmp == 1.0) {
        y = 1.0;
      } else if (tmp > 1.0) {
        if (u1 > 0.0) {
          y = (rtInf);
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = (rtInf);
      }
    } else if (tmp_0 == 0.0) {
      y = 1.0;
    } else if (tmp_0 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = (rtNaN);
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

/* Model step function */
void pid_control::step()
{
  /* local block i/o variables */
  SL_Bus_std_msgs_Float64 rtb_SourceBlock_o2;
  SL_Bus_std_msgs_Float64 rtb_SourceBlock_o2_k;
  SL_Bus_std_msgs_Float64 rtb_SourceBlock_o2_p;
  SL_Bus_std_msgs_Bool rtb_SourceBlock_o2_j;
  SL_Bus_std_msgs_Bool rtb_SourceBlock_o2_dd;
  if (rtmIsMajorTimeStep((&pid_control_M))) {
    /* set solver stop time */
    if (!((&pid_control_M)->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&(&pid_control_M)->solverInfo, (((&pid_control_M)
        ->Timing.clockTickH0 + 1) * (&pid_control_M)->Timing.stepSize0 *
        4294967296.0));
    } else {
      rtsiSetSolverStopTime(&(&pid_control_M)->solverInfo, (((&pid_control_M)
        ->Timing.clockTick0 + 1) * (&pid_control_M)->Timing.stepSize0 +
        (&pid_control_M)->Timing.clockTickH0 * (&pid_control_M)
        ->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep((&pid_control_M))) {
    (&pid_control_M)->Timing.t[0] = rtsiGetT(&(&pid_control_M)->solverInfo);
  }

  {
    SL_Bus_gazebo_msgs_SetEntityStateResponse tmp;
    int32_T i;
    int32_T rtb_sec_0;
    uint32_T rtb_nsec_0;
    int8_T rtAction;
    int8_T rtPrevAction;
    boolean_T serverAvailableOnTime;
    boolean_T tmp_0;
    static const uint8_T b[15] = { 101U, 107U, 114U, 97U, 110U, 111U, 112U, 108U,
      97U, 110U, 111U, 45U, 105U, 109U, 117U };

    static const uint8_T b_0[5] = { 119U, 111U, 114U, 108U, 100U };

    static const uint8_T b_1[11] = { 101U, 107U, 114U, 97U, 110U, 111U, 112U,
      108U, 97U, 110U, 111U };

    /* BusAssignment: '<Root>/Bus Assignment-IMU' */
    memset(&pid_control_B.BusAssignmentIMU, 0, sizeof(SL_Bus_sensor_msgs_Imu));

    /* RelationalOperator: '<S152>/Compare' incorporates:
     *  Constant: '<S152>/Constant'
     */
    pid_control_B.Compare = (pid_control_X.Integrator_CSTATE[11] >= 0.15);

    /* Outputs for Enabled SubSystem: '<S162>/Hugw(s)' incorporates:
     *  EnablePort: '<S175>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<S161>/Hrgw' incorporates:
     *  EnablePort: '<S174>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<S162>/Hvgw(s)' incorporates:
     *  EnablePort: '<S176>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<S161>/Hqgw' incorporates:
     *  EnablePort: '<S173>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<S162>/Hwgw(s)' incorporates:
     *  EnablePort: '<S177>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<S161>/Hpgw' incorporates:
     *  EnablePort: '<S172>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<Root>/Subsystem1' incorporates:
     *  EnablePort: '<S38>/Enable'
     */
    /* Outputs for Enabled SubSystem: '<Root>/Subsystem2' incorporates:
     *  EnablePort: '<S39>/Enable'
     */
    tmp_0 = (rtmIsMajorTimeStep((&pid_control_M)) &&
             (&pid_control_M)->Timing.TaskCounters.TID[1] == 0);

    /* End of Outputs for SubSystem: '<Root>/Subsystem2' */
    /* End of Outputs for SubSystem: '<Root>/Subsystem1' */
    /* End of Outputs for SubSystem: '<S161>/Hpgw' */
    /* End of Outputs for SubSystem: '<S162>/Hwgw(s)' */
    /* End of Outputs for SubSystem: '<S161>/Hqgw' */
    /* End of Outputs for SubSystem: '<S162>/Hvgw(s)' */
    /* End of Outputs for SubSystem: '<S161>/Hrgw' */
    /* End of Outputs for SubSystem: '<S162>/Hugw(s)' */
    if (tmp_0) {
      /* MATLAB Function: '<S34>/MATLAB Function-reset' incorporates:
       *  Memory: '<S34>/Memory2'
       */
      memcpy(&pid_control_B.IC[0], &pid_control_DW.Memory2_PreviousInput[0], 12U
             * sizeof(real_T));
      pid_control_B.IC[2] = 0.0;
      pid_control_B.IC[11] = 0.0;

      /* InitialCondition: '<S34>/IC' */
      if (pid_control_DW.IC_FirstOutputTime) {
        pid_control_DW.IC_FirstOutputTime = false;

        /* InitialCondition: '<S34>/IC' */
        memcpy(&pid_control_B.IC[0], &pid_control_ConstP.pooled17[0], 12U *
               sizeof(real_T));
      }

      /* End of InitialCondition: '<S34>/IC' */
    }

    /* Integrator: '<S34>/Integrator' incorporates:
     *  InitialCondition: '<S34>/IC'
     */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      serverAvailableOnTime = (((pid_control_PrevZCX.Integrator_Reset_ZCE ==
        POS_ZCSIG) != pid_control_B.Compare) &&
        (pid_control_PrevZCX.Integrator_Reset_ZCE != UNINITIALIZED_ZCSIG));
      pid_control_PrevZCX.Integrator_Reset_ZCE = pid_control_B.Compare;

      /* evaluate zero-crossings and the level of the reset signal */
      if (serverAvailableOnTime || pid_control_B.Compare ||
          pid_control_DW.Integrator_DWORK1) {
        memcpy(&pid_control_X.Integrator_CSTATE[0], &pid_control_B.IC[0], 12U *
               sizeof(real_T));
        rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
          ->solverInfo, true);
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&pid_control_M
          )->solverInfo, true);
      }
    }

    /* Integrator: '<S34>/Integrator' */
    memcpy(&pid_control_B.x[0], &pid_control_X.Integrator_CSTATE[0], 12U *
           sizeof(real_T));

    /* MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
    if (!pid_control_DW.regObj_not_empty) {
      pid_control_DW.regObj.FeatureControlFlag = true;
      pid_control_DW.regObj_not_empty = true;
    }

    /* SignalConversion generated from: '<Root>/Coordinate Transformation Conversion' incorporates:
     *  MATLABSystem: '<Root>/Coordinate Transformation Conversion'
     */
    _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_div_pd(_mm_set_pd
      (pid_control_B.x[6], pid_control_B.x[8]), _mm_set1_pd(2.0)));

    /* MATLABSystem: '<Root>/Coordinate Transformation Conversion' incorporates:
     *  SignalConversion generated from: '<Root>/Coordinate Transformation Conversion'
     */
    pid_control_B.wbe_b[2] = pid_control_B.x[7] / 2.0;
    pid_control_B.sina = sin(pid_control_B.wbe_b[0]);
    pid_control_B.sinb = sin(pid_control_B.wbe_b[1]);
    pid_control_B.sinc = sin(pid_control_B.wbe_b[2]);
    pid_control_B.cosa = cos(pid_control_B.wbe_b[0]);
    pid_control_B.cosb = cos(pid_control_B.wbe_b[1]);
    pid_control_B.cosc = cos(pid_control_B.wbe_b[2]);
    pid_control_B.rtb_CoordinateTransformationC_l = pid_control_B.cosa *
      pid_control_B.cosb;
    pid_control_B.rtb_CoordinateTransformationC_d = pid_control_B.sina *
      pid_control_B.sinb * pid_control_B.sinc +
      pid_control_B.rtb_CoordinateTransformationC_l * pid_control_B.cosc;
    pid_control_B.rtb_CoordinateTransformationC_l =
      pid_control_B.rtb_CoordinateTransformationC_l * pid_control_B.sinc -
      pid_control_B.cosc * pid_control_B.sina * pid_control_B.sinb;
    pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.cosa *
      pid_control_B.cosc * pid_control_B.sinb + pid_control_B.cosb *
      pid_control_B.sina * pid_control_B.sinc;
    pid_control_B.rtb_CoordinateTransformationC_b = pid_control_B.cosb *
      pid_control_B.cosc * pid_control_B.sina - pid_control_B.cosa *
      pid_control_B.sinb * pid_control_B.sinc;
    if (tmp_0) {
      /* MATLAB Function: '<Root>/MATLAB Function-IMU2' */
      memset(&pid_control_B.stringOut_n[0], 0, sizeof(uint8_T) << 7U);
      for (i = 0; i < 15; i++) {
        pid_control_B.stringOut_n[i] = b[i];
      }

      pid_control_B.lengthOut_i = 15U;

      /* End of MATLAB Function: '<Root>/MATLAB Function-IMU2' */
    }

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.cosb = 0.0 * pid_control_B.rtb_CoordinateTransformationC_o +
      pid_control_B.rtb_CoordinateTransformationC_l;

    /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
     */
    pid_control_B.BusAssignmentIMU.orientation.x = -pid_control_B.cosb;

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.sina = 0.0 * pid_control_B.rtb_CoordinateTransformationC_l -
      pid_control_B.rtb_CoordinateTransformationC_o;

    /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
     */
    pid_control_B.BusAssignmentIMU.orientation.y = pid_control_B.sina;

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.sinb = 0.0 * pid_control_B.rtb_CoordinateTransformationC_d -
      pid_control_B.rtb_CoordinateTransformationC_b;

    /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
     */
    pid_control_B.BusAssignmentIMU.orientation.z = -pid_control_B.sinb;

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_imu' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.sinc = 0.0 * pid_control_B.rtb_CoordinateTransformationC_b +
      pid_control_B.rtb_CoordinateTransformationC_d;

    /* BusAssignment: '<Root>/Bus Assignment-IMU' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_imu'
     */
    pid_control_B.BusAssignmentIMU.orientation.w = pid_control_B.sinc;
    pid_control_B.BusAssignmentIMU.angular_velocity.x = pid_control_B.x[0];
    pid_control_B.BusAssignmentIMU.angular_velocity.y = pid_control_B.x[1];
    pid_control_B.BusAssignmentIMU.angular_velocity.z = pid_control_B.x[2];
    memcpy(&pid_control_B.BusAssignmentIMU.header.frame_id[0],
           &pid_control_B.stringOut_n[0], sizeof(uint8_T) << 7U);
    pid_control_B.BusAssignmentIMU.header.frame_id_SL_Info.CurrentLength =
      pid_control_B.lengthOut_i;

    /* MATLABSystem: '<S27>/SinkBlock' */
    Pub_pid_control_548.publish(&pid_control_B.BusAssignmentIMU);

    /* Gain: '<Root>/Gain-Z' */
    pid_control_B.GainZ = -pid_control_B.x[11];

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo' */
    pid_control_B.cosa = pid_control_B.x[9];
    pid_control_B.cosc = pid_control_B.GainZ;
    pid_control_B.px_out_l = -pid_control_B.x[10];
    if (tmp_0) {
      /* MATLAB Function: '<Root>/MATLAB Function-Odometry' */
      memset(&pid_control_B.str_out_e[0], 0, sizeof(uint8_T) << 7U);
      for (i = 0; i < 5; i++) {
        pid_control_B.str_out_e[i] = b_0[i];
      }

      pid_control_B.currentLen_b = 5U;
      pid_control_B.receivedLen_b = 5U;

      /* End of MATLAB Function: '<Root>/MATLAB Function-Odometry' */

      /* MATLAB Function: '<Root>/MATLAB Function-Odometry1' */
      memset(&pid_control_B.str_out[0], 0, sizeof(uint8_T) << 7U);
      for (i = 0; i < 11; i++) {
        pid_control_B.str_out[i] = b_1[i];
      }

      pid_control_B.currentLen = 11U;
      pid_control_B.receivedLen = 11U;

      /* End of MATLAB Function: '<Root>/MATLAB Function-Odometry1' */
    }

    /* Clock: '<Root>/Clock' incorporates:
     *  Clock: '<S33>/Clock'
     */
    pid_control_B.SignPreSat = (&pid_control_M)->Timing.t[0];

    /* MATLAB Function: '<Root>/MATLAB Function-clocl' incorporates:
     *  Clock: '<Root>/Clock'
     */
    pid_control_B.IntegratorSecondOrderLimite = floor(pid_control_B.SignPreSat);

    /* BusAssignment: '<Root>/Bus Assignment-ODOM' */
    memset(&pid_control_B.BusAssignmentODOM, 0, sizeof(SL_Bus_nav_msgs_Odometry));

    /* MATLAB Function: '<Root>/MATLAB Function-clocl' incorporates:
     *  Clock: '<Root>/Clock'
     */
    if (pid_control_B.IntegratorSecondOrderLimite < 2.147483648E+9) {
      if (pid_control_B.IntegratorSecondOrderLimite >= -2.147483648E+9) {
        rtb_sec_0 = static_cast<int32_T>
          (pid_control_B.IntegratorSecondOrderLimite);
      } else {
        rtb_sec_0 = MIN_int32_T;
      }
    } else {
      rtb_sec_0 = MAX_int32_T;
    }

    pid_control_B.IntegratorSecondOrderLimite = rt_roundd_snf
      ((pid_control_B.SignPreSat - pid_control_B.IntegratorSecondOrderLimite) *
       1.0E+9);
    if (pid_control_B.IntegratorSecondOrderLimite < 4.294967296E+9) {
      if (pid_control_B.IntegratorSecondOrderLimite >= 0.0) {
        rtb_nsec_0 = static_cast<uint32_T>
          (pid_control_B.IntegratorSecondOrderLimite);
      } else {
        rtb_nsec_0 = 0U;
      }
    } else {
      rtb_nsec_0 = MAX_uint32_T;
    }

    /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.BusAssignmentODOM.pose.pose.position.x = pid_control_B.x[10];
    pid_control_B.BusAssignmentODOM.pose.pose.position.y = pid_control_B.x[9];
    pid_control_B.BusAssignmentODOM.pose.pose.position.z = -pid_control_B.GainZ;

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry' */
    pid_control_B.IntegratorSecondOrderLimite = 0.7071 * pid_control_B.sina;
    pid_control_B.Square1 = 0.7071 * -pid_control_B.cosb;

    /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.BusAssignmentODOM.pose.pose.orientation.x =
      pid_control_B.Square1 + pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.BusAssignmentODOM.pose.pose.orientation.y =
      pid_control_B.IntegratorSecondOrderLimite - pid_control_B.Square1;

    /* MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry' */
    pid_control_B.IntegratorSecondOrderLimite = 0.7071 * pid_control_B.sinc;
    pid_control_B.Square1 = 0.7071 * pid_control_B.sinb;

    /* BusAssignment: '<Root>/Bus Assignment-ODOM' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-clocl'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_odometry'
     */
    pid_control_B.BusAssignmentODOM.pose.pose.orientation.z =
      pid_control_B.Square1 + pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.BusAssignmentODOM.pose.pose.orientation.w =
      pid_control_B.IntegratorSecondOrderLimite - pid_control_B.Square1;
    pid_control_B.BusAssignmentODOM.header.frame_id_SL_Info.CurrentLength =
      pid_control_B.currentLen_b;
    pid_control_B.BusAssignmentODOM.header.frame_id_SL_Info.ReceivedLength =
      pid_control_B.receivedLen_b;
    memcpy(&pid_control_B.BusAssignmentODOM.header.frame_id[0],
           &pid_control_B.str_out_e[0], sizeof(uint8_T) << 7U);
    memcpy(&pid_control_B.BusAssignmentODOM.child_frame_id[0],
           &pid_control_B.str_out[0], sizeof(uint8_T) << 7U);
    pid_control_B.BusAssignmentODOM.child_frame_id_SL_Info.CurrentLength =
      pid_control_B.currentLen;
    pid_control_B.BusAssignmentODOM.child_frame_id_SL_Info.ReceivedLength =
      pid_control_B.receivedLen;
    pid_control_B.BusAssignmentODOM.header.stamp.sec = rtb_sec_0;
    pid_control_B.BusAssignmentODOM.header.stamp.nanosec = rtb_nsec_0;

    /* MATLABSystem: '<S28>/SinkBlock' */
    Pub_pid_control_697.publish(&pid_control_B.BusAssignmentODOM);
    if (tmp_0) {
      /* MATLAB Function: '<Root>/MATLAB Function-ned_to_PoseStamped' */
      MATLABFunctionned_to_PoseStampe
        (&pid_control_B.sf_MATLABFunctionned_to_PoseS_b);
    }

    /* BusAssignment: '<Root>/Bus Assignment-PoseStaamped' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-clocl'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_PoseStaamped'
     *  MATLAB Function: '<Root>/MATLAB Function-ned_to_gazebo'
     */
    memset(&pid_control_B.BusAssignmentPoseStaamped, 0, sizeof
           (SL_Bus_geometry_msgs_PoseStamped));
    memcpy(&pid_control_B.BusAssignmentPoseStaamped.header.frame_id[0],
           &pid_control_B.sf_MATLABFunctionned_to_PoseS_b.stringOut[0], sizeof
           (uint8_T) << 7U);
    pid_control_B.BusAssignmentPoseStaamped.header.frame_id_SL_Info.CurrentLength
      = pid_control_B.sf_MATLABFunctionned_to_PoseS_b.lengthOut;
    pid_control_B.BusAssignmentPoseStaamped.pose.position.x = -pid_control_B.x
      [10];
    pid_control_B.BusAssignmentPoseStaamped.pose.position.y = pid_control_B.x[9];
    pid_control_B.BusAssignmentPoseStaamped.pose.position.z =
      -pid_control_B.GainZ;
    pid_control_B.BusAssignmentPoseStaamped.pose.orientation.x = 0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_o + -0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_l;
    pid_control_B.BusAssignmentPoseStaamped.pose.orientation.y = 0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_l - -0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_o;
    pid_control_B.BusAssignmentPoseStaamped.pose.orientation.z = -(0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_b + -0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_d);
    pid_control_B.BusAssignmentPoseStaamped.pose.orientation.w = 0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_d - -0.7071 *
      pid_control_B.rtb_CoordinateTransformationC_b;
    pid_control_B.BusAssignmentPoseStaamped.header.stamp.sec = rtb_sec_0;
    pid_control_B.BusAssignmentPoseStaamped.header.stamp.nanosec = rtb_nsec_0;

    /* MATLABSystem: '<S29>/SinkBlock' */
    Pub_pid_control_811.publish(&pid_control_B.BusAssignmentPoseStaamped);
    if (tmp_0) {
      /* MATLAB Function: '<Root>/MATLAB Function-ned_to_TwistStamped' */
      MATLABFunctionned_to_PoseStampe
        (&pid_control_B.sf_MATLABFunctionned_to_TwistSt);
    }

    /* BusAssignment: '<Root>/Bus Assignment-TwistStamped' incorporates:
     *  MATLAB Function: '<Root>/MATLAB Function-clocl'
     *  MATLAB Function: '<Root>/MATLAB Function2'
     */
    memset(&pid_control_B.BusAssignmentTwistStamped, 0, sizeof
           (SL_Bus_geometry_msgs_TwistStamped));
    memcpy(&pid_control_B.BusAssignmentTwistStamped.header.frame_id[0],
           &pid_control_B.sf_MATLABFunctionned_to_TwistSt.stringOut[0], sizeof
           (uint8_T) << 7U);
    pid_control_B.BusAssignmentTwistStamped.header.frame_id_SL_Info.CurrentLength
      = pid_control_B.sf_MATLABFunctionned_to_TwistSt.lengthOut;
    pid_control_B.BusAssignmentTwistStamped.twist.linear.x = pid_control_B.x[1];
    pid_control_B.BusAssignmentTwistStamped.twist.linear.y = pid_control_B.x[0];
    pid_control_B.BusAssignmentTwistStamped.twist.linear.z = -pid_control_B.x[2];
    pid_control_B.BusAssignmentTwistStamped.header.stamp.sec = rtb_sec_0;
    pid_control_B.BusAssignmentTwistStamped.header.stamp.nanosec = rtb_nsec_0;

    /* MATLABSystem: '<S30>/SinkBlock' */
    Pub_pid_control_835.publish(&pid_control_B.BusAssignmentTwistStamped);

    /* Math: '<S33>/Math Function' incorporates:
     *  Constant: '<S33>/Constant'
     *  S-Function (sfun_tstart): '<S33>/startTime'
     *  Sum: '<S33>/Sum'
     */
    pid_control_B.rtb_CoordinateTransformationC_b = rt_remd_snf
      (pid_control_B.SignPreSat - (0.0), 300.0);

    /* Lookup_n-D: '<S33>/Look-Up Table1' incorporates:
     *  Gain: '<S127>/Integral Gain'
     */
    pid_control_B.Saturation1 = look1_binlxpw
      (pid_control_B.rtb_CoordinateTransformationC_b,
       pid_control_ConstP.LookUpTable1_bp01Data,
       pid_control_ConstP.LookUpTable1_tableData, 16U);

    /* Saturate: '<Root>/Saturation1' */
    if (pid_control_B.Saturation1 > 20.2) {
      /* Lookup_n-D: '<S33>/Look-Up Table1' incorporates:
       *  Saturate: '<Root>/Saturation1'
       */
      pid_control_B.Saturation1 = 20.2;
    } else if (pid_control_B.Saturation1 < 0.0) {
      /* Lookup_n-D: '<S33>/Look-Up Table1' incorporates:
       *  Saturate: '<Root>/Saturation1'
       */
      pid_control_B.Saturation1 = 0.0;
    }

    /* End of Saturate: '<Root>/Saturation1' */

    /* Sum: '<Root>/Sum3' */
    pid_control_B.rtb_CoordinateTransformationC_b = pid_control_B.Saturation1 -
      pid_control_B.x[0];

    /* Gain: '<S133>/Filter Coefficient' incorporates:
     *  Gain: '<S123>/Derivative Gain'
     *  Integrator: '<S125>/Filter'
     *  Sum: '<S125>/SumD'
     */
    pid_control_B.FilterCoefficient = (0.005 *
      pid_control_B.rtb_CoordinateTransformationC_b -
      pid_control_X.Filter_CSTATE) * 100.0;

    /* Sum: '<S139>/Sum' incorporates:
     *  Gain: '<S135>/Proportional Gain'
     *  Integrator: '<S130>/Integrator'
     */
    pid_control_B.SignPreSat = (0.02 *
      pid_control_B.rtb_CoordinateTransformationC_b +
      pid_control_X.Integrator_CSTATE_f) + pid_control_B.FilterCoefficient;

    /* Saturate: '<S137>/Saturation' */
    if (pid_control_B.SignPreSat > 1.0) {
      /* Saturate: '<S137>/Saturation' */
      pid_control_B.Saturation = 1.0;
    } else if (pid_control_B.SignPreSat < 0.0) {
      /* Saturate: '<S137>/Saturation' */
      pid_control_B.Saturation = 0.0;
    } else {
      /* Saturate: '<S137>/Saturation' */
      pid_control_B.Saturation = pid_control_B.SignPreSat;
    }

    /* End of Saturate: '<S137>/Saturation' */
    if (tmp_0) {
      /* MATLABSystem: '<S35>/SourceBlock' */
      pid_control_B.SourceBlock_o1_o = Sub_pid_control_435.getLatestMessage
        (&pid_control_B.SourceBlock_o2_d);

      /* Outputs for Enabled SubSystem: '<S35>/Enabled Subsystem' */
      pid_control_EnabledSubsystem_k(pid_control_B.SourceBlock_o1_o,
        &pid_control_B.SourceBlock_o2_d, &pid_control_B.EnabledSubsystem_b);

      /* End of Outputs for SubSystem: '<S35>/Enabled Subsystem' */

      /* Switch: '<Root>/Switch3' */
      if (pid_control_B.SourceBlock_o1_o) {
        /* Switch: '<Root>/Switch3' */
        pid_control_B.Switch3 = pid_control_B.EnabledSubsystem_b.In1.data;
      } else {
        /* Switch: '<Root>/Switch3' incorporates:
         *  UnitDelay: '<Root>/Unit Delay3'
         */
        pid_control_B.Switch3 = pid_control_DW.UnitDelay3_DSTATE;
      }

      /* End of Switch: '<Root>/Switch3' */
    }

    /* Sum: '<Root>/Sum2' */
    pid_control_B.rtb_CoordinateTransformationC_d = pid_control_B.Switch3 -
      pid_control_B.GainZ;

    /* Relay: '<Root>/Relay-Enable-Flight' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      pid_control_DW.RelayEnableFlight_Mode = ((pid_control_B.x[0] >= 15.0) || (
        !(pid_control_B.x[0] <= 12.0) && pid_control_DW.RelayEnableFlight_Mode));
    }

    if (pid_control_DW.RelayEnableFlight_Mode) {
      /* Relay: '<Root>/Relay-Enable-Flight' */
      pid_control_B.RelayEnableFlight = 1.0;
    } else {
      /* Relay: '<Root>/Relay-Enable-Flight' */
      pid_control_B.RelayEnableFlight = 0.0;
    }

    /* End of Relay: '<Root>/Relay-Enable-Flight' */

    /* Outputs for Enabled SubSystem: '<Root>/Subsystem2' incorporates:
     *  EnablePort: '<S39>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      if (pid_control_B.RelayEnableFlight > 0.0) {
        if (!pid_control_DW.Subsystem2_MODE) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_n), 0,
                        2*sizeof(boolean_T));
          pid_control_DW.Subsystem2_MODE = true;
        }
      } else {
        if (((((&pid_control_M)->Timing.clockTick1+(&pid_control_M)
               ->Timing.clockTickH1* 4294967296.0)) * 0.01) == rtmGetTStart
            ((&pid_control_M))) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_n), 1,
                        2*sizeof(boolean_T));
        }

        if (pid_control_DW.Subsystem2_MODE) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_n), 1,
                        2*sizeof(boolean_T));
          pid_control_DW.Subsystem2_MODE = false;
        }
      }
    }

    if (pid_control_DW.Subsystem2_MODE) {
      /* Gain: '<S349>/Filter Coefficient' incorporates:
       *  Gain: '<S339>/Derivative Gain'
       *  Integrator: '<S341>/Filter'
       *  Sum: '<S341>/SumD'
       */
      pid_control_B.FilterCoefficient_j = (0.25 *
        pid_control_B.rtb_CoordinateTransformationC_d -
        pid_control_X.Filter_CSTATE_fb) * 15.0;

      /* Sum: '<S355>/Sum' incorporates:
       *  Gain: '<S351>/Proportional Gain'
       *  Integrator: '<S346>/Integrator'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = (0.5 *
        pid_control_B.rtb_CoordinateTransformationC_d +
        pid_control_X.Integrator_CSTATE_n) + pid_control_B.FilterCoefficient_j;

      /* Saturate: '<S353>/Saturation' */
      if (pid_control_B.rtb_CoordinateTransformationC_o > 0.2) {
        /* Saturate: '<S353>/Saturation' */
        pid_control_B.Saturation_f = 0.2;
      } else if (pid_control_B.rtb_CoordinateTransformationC_o < -0.2) {
        /* Saturate: '<S353>/Saturation' */
        pid_control_B.Saturation_f = -0.2;
      } else {
        /* Saturate: '<S353>/Saturation' */
        pid_control_B.Saturation_f =
          pid_control_B.rtb_CoordinateTransformationC_o;
      }

      /* End of Saturate: '<S353>/Saturation' */

      /* Sum: '<S338>/SumI4' incorporates:
       *  Gain: '<S338>/Kb'
       *  Gain: '<S343>/Integral Gain'
       *  Sum: '<S338>/SumI2'
       */
      pid_control_B.SumI4_i = (pid_control_B.Saturation_f -
        pid_control_B.rtb_CoordinateTransformationC_o) * 0.1 + 0.02 *
        pid_control_B.rtb_CoordinateTransformationC_d;
    }

    /* End of Outputs for SubSystem: '<Root>/Subsystem2' */
    if (tmp_0) {
      /* MATLABSystem: '<S36>/SourceBlock' */
      pid_control_B.SourceBlock_o1 = Sub_pid_control_377.getLatestMessage
        (&rtb_SourceBlock_o2);

      /* Outputs for Enabled SubSystem: '<S36>/Enabled Subsystem' */
      pid_control_EnabledSubsystem_k(pid_control_B.SourceBlock_o1,
        &rtb_SourceBlock_o2, &pid_control_B.EnabledSubsystem_a);

      /* End of Outputs for SubSystem: '<S36>/Enabled Subsystem' */

      /* Switch: '<Root>/Switch2' */
      if (pid_control_B.SourceBlock_o1) {
        /* Switch: '<Root>/Switch2' */
        pid_control_B.Switch2 = pid_control_B.EnabledSubsystem_a.In1.data;
      } else {
        /* Switch: '<Root>/Switch2' incorporates:
         *  UnitDelay: '<Root>/Unit Delay2'
         */
        pid_control_B.Switch2 = pid_control_DW.UnitDelay2_DSTATE;
      }

      /* End of Switch: '<Root>/Switch2' */

      /* RateLimiter: '<Root>/Rate Limiter2' */
      pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.Switch2 -
        pid_control_DW.PrevY;
      if (pid_control_B.rtb_CoordinateTransformationC_o > 0.0026000000000000003)
      {
        /* RateLimiter: '<Root>/Rate Limiter2' */
        pid_control_B.RateLimiter2 = pid_control_DW.PrevY +
          0.0026000000000000003;
      } else if (pid_control_B.rtb_CoordinateTransformationC_o <
                 -0.0026000000000000003) {
        /* RateLimiter: '<Root>/Rate Limiter2' */
        pid_control_B.RateLimiter2 = pid_control_DW.PrevY -
          0.0026000000000000003;
      } else {
        /* RateLimiter: '<Root>/Rate Limiter2' */
        pid_control_B.RateLimiter2 = pid_control_B.Switch2;
      }

      pid_control_DW.PrevY = pid_control_B.RateLimiter2;

      /* End of RateLimiter: '<Root>/Rate Limiter2' */
    }

    /* Sum: '<Root>/Sum5' */
    pid_control_B.rtb_CoordinateTransformationC_d = pid_control_B.RateLimiter2 -
      pid_control_B.x[8];

    /* Gain: '<S243>/Filter Coefficient' incorporates:
     *  Gain: '<S233>/Derivative Gain'
     *  Integrator: '<S235>/Filter'
     *  Sum: '<S235>/SumD'
     */
    pid_control_B.FilterCoefficient_p = (-1.3 *
      pid_control_B.rtb_CoordinateTransformationC_d -
      pid_control_X.Filter_CSTATE_f) * 10.0;

    /* Sum: '<S249>/Sum' incorporates:
     *  Gain: '<S245>/Proportional Gain'
     *  Integrator: '<S240>/Integrator'
     */
    pid_control_B.rtb_CoordinateTransformationC_l = (-1.2 *
      pid_control_B.rtb_CoordinateTransformationC_d +
      pid_control_X.Integrator_CSTATE_d) + pid_control_B.FilterCoefficient_p;

    /* Saturate: '<S247>/Saturation' */
    if (pid_control_B.rtb_CoordinateTransformationC_l > 0.09) {
      /* Saturate: '<S247>/Saturation' */
      pid_control_B.Saturation_m = 0.09;
    } else if (pid_control_B.rtb_CoordinateTransformationC_l < -0.09) {
      /* Saturate: '<S247>/Saturation' */
      pid_control_B.Saturation_m = -0.09;
    } else {
      /* Saturate: '<S247>/Saturation' */
      pid_control_B.Saturation_m = pid_control_B.rtb_CoordinateTransformationC_l;
    }

    /* End of Saturate: '<S247>/Saturation' */

    /* Saturate: '<Root>/Saturation_roll_sp' incorporates:
     *  Gain: '<Root>/Gain-roll_sp'
     */
    if (-pid_control_B.Saturation_m > 0.1) {
      /* Saturate: '<Root>/Saturation_roll_sp' */
      pid_control_B.Saturation_roll_sp = 0.1;
    } else if (-pid_control_B.Saturation_m < -0.1) {
      /* Saturate: '<Root>/Saturation_roll_sp' */
      pid_control_B.Saturation_roll_sp = -0.1;
    } else {
      /* Saturate: '<Root>/Saturation_roll_sp' */
      pid_control_B.Saturation_roll_sp = -pid_control_B.Saturation_m;
    }

    /* End of Saturate: '<Root>/Saturation_roll_sp' */

    /* Gain: '<Root>/Gain' */
    pid_control_B.rtb_CoordinateTransformationC_o = 0.13 *
      pid_control_B.rtb_CoordinateTransformationC_d;

    /* Saturate: '<Root>/Saturation' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 0.14) {
      pid_control_B.rtb_CoordinateTransformationC_o = 0.14;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o < -0.14) {
      pid_control_B.rtb_CoordinateTransformationC_o = -0.14;
    }

    /* Sum: '<Root>/Sum4' incorporates:
     *  Saturate: '<Root>/Saturation'
     */
    pid_control_B.Square1 = (pid_control_B.Saturation_roll_sp +
      pid_control_B.rtb_CoordinateTransformationC_o) - pid_control_B.x[6];

    /* Gain: '<S79>/Filter Coefficient' incorporates:
     *  Gain: '<S69>/Derivative Gain'
     *  Integrator: '<S71>/Filter'
     *  Sum: '<S71>/SumD'
     */
    pid_control_B.FilterCoefficient_c = (-0.15 * pid_control_B.Square1 -
      pid_control_X.Filter_CSTATE_g) * 50.0;

    /* Sum: '<S85>/Sum' incorporates:
     *  Gain: '<S81>/Proportional Gain'
     *  Integrator: '<S76>/Integrator'
     */
    pid_control_B.IntegratorSecondOrderLimite = (-0.4 * pid_control_B.Square1 +
      pid_control_X.Integrator_CSTATE_m) + pid_control_B.FilterCoefficient_c;

    /* Saturate: '<S83>/Saturation' */
    if (pid_control_B.IntegratorSecondOrderLimite > 0.1) {
      /* Saturate: '<S83>/Saturation' */
      pid_control_B.Saturation_k = 0.1;
    } else if (pid_control_B.IntegratorSecondOrderLimite < -0.1) {
      /* Saturate: '<S83>/Saturation' */
      pid_control_B.Saturation_k = -0.1;
    } else {
      /* Saturate: '<S83>/Saturation' */
      pid_control_B.Saturation_k = pid_control_B.IntegratorSecondOrderLimite;
    }

    /* End of Saturate: '<S83>/Saturation' */

    /* Gain: '<Root>/Gain2' */
    pid_control_B.Gain2 = -pid_control_B.Saturation_k;

    /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
    if (pid_control_DW.LastMajorTime == (rtMinusInf)) {
      /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
      pid_control_B.RateLimitertheta_sp = pid_control_B.Saturation_f;
    } else {
      pid_control_B.rtb_CoordinateTransformationC_o = (&pid_control_M)->
        Timing.t[0];
      pid_control_B.Sum1 = pid_control_B.rtb_CoordinateTransformationC_o -
        pid_control_DW.LastMajorTime;
      if (pid_control_DW.LastMajorTime ==
          pid_control_B.rtb_CoordinateTransformationC_o) {
        if (pid_control_DW.PrevLimited) {
          /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
          pid_control_B.RateLimitertheta_sp = pid_control_DW.PrevY_g;
        } else {
          /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
          pid_control_B.RateLimitertheta_sp = pid_control_B.Saturation_f;
        }
      } else {
        pid_control_B.u2_deg = pid_control_B.Sum1 * 0.4;
        pid_control_B.rtb_CoordinateTransformationC_o =
          pid_control_B.Saturation_f - pid_control_DW.PrevY_g;
        if (pid_control_B.rtb_CoordinateTransformationC_o > pid_control_B.u2_deg)
        {
          /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
          pid_control_B.RateLimitertheta_sp = pid_control_DW.PrevY_g +
            pid_control_B.u2_deg;
          serverAvailableOnTime = true;
        } else {
          pid_control_B.Sum1 *= -0.4;
          if (pid_control_B.rtb_CoordinateTransformationC_o < pid_control_B.Sum1)
          {
            /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
            pid_control_B.RateLimitertheta_sp = pid_control_DW.PrevY_g +
              pid_control_B.Sum1;
            serverAvailableOnTime = true;
          } else {
            /* RateLimiter: '<Root>/Rate Limiter-theta_sp' */
            pid_control_B.RateLimitertheta_sp = pid_control_B.Saturation_f;
            serverAvailableOnTime = false;
          }
        }

        if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
          pid_control_DW.PrevLimited = serverAvailableOnTime;
        }
      }
    }

    /* End of RateLimiter: '<Root>/Rate Limiter-theta_sp' */

    /* Sum: '<Root>/Sum1' */
    pid_control_B.Sum1 = pid_control_B.RateLimitertheta_sp - pid_control_B.x[7];

    /* Outputs for Enabled SubSystem: '<Root>/Subsystem1' incorporates:
     *  EnablePort: '<S38>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      if (pid_control_B.RelayEnableFlight > 0.0) {
        if (!pid_control_DW.Subsystem1_MODE) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_p), 0,
                        2*sizeof(boolean_T));
          pid_control_DW.Subsystem1_MODE = true;
        }
      } else {
        /* Outputs for Enabled SubSystem: '<Root>/Subsystem2' incorporates:
         *  EnablePort: '<S39>/Enable'
         */
        if (((((&pid_control_M)->Timing.clockTick1+(&pid_control_M)
               ->Timing.clockTickH1* 4294967296.0)) * 0.01) == rtmGetTStart
            ((&pid_control_M))) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_p), 1,
                        2*sizeof(boolean_T));
        }

        /* End of Outputs for SubSystem: '<Root>/Subsystem2' */
        if (pid_control_DW.Subsystem1_MODE) {
          (void) memset(&(pid_control_XDis.Integrator_CSTATE_p), 1,
                        2*sizeof(boolean_T));
          pid_control_DW.Subsystem1_MODE = false;
        }
      }
    }

    /* End of Outputs for SubSystem: '<Root>/Subsystem1' */

    /* BusAssignment: '<Root>/Bus Assignment-MODELO' */
    memset(&pid_control_B.BusAssignmentMODELO, 0, sizeof
           (SL_Bus_gazebo_msgs_SetEntityStateRequest));

    /* Outputs for Enabled SubSystem: '<Root>/Subsystem1' incorporates:
     *  EnablePort: '<S38>/Enable'
     */
    if (pid_control_DW.Subsystem1_MODE) {
      /* Gain: '<S296>/Filter Coefficient' incorporates:
       *  Gain: '<S286>/Derivative Gain'
       *  Integrator: '<S288>/Filter'
       *  Sum: '<S288>/SumD'
       */
      pid_control_B.FilterCoefficient_m = (-0.2 * pid_control_B.Sum1 -
        pid_control_X.Filter_CSTATE_m) * 15.0;

      /* Sum: '<S302>/Sum' incorporates:
       *  Gain: '<S298>/Proportional Gain'
       *  Integrator: '<S293>/Integrator'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = (-0.35 *
        pid_control_B.Sum1 + pid_control_X.Integrator_CSTATE_p) +
        pid_control_B.FilterCoefficient_m;

      /* Saturate: '<S300>/Saturation' */
      if (pid_control_B.rtb_CoordinateTransformationC_o > 0.2) {
        /* Saturate: '<S300>/Saturation' */
        pid_control_B.Saturation_fh = 0.2;
      } else if (pid_control_B.rtb_CoordinateTransformationC_o < -0.2) {
        /* Saturate: '<S300>/Saturation' */
        pid_control_B.Saturation_fh = -0.2;
      } else {
        /* Saturate: '<S300>/Saturation' */
        pid_control_B.Saturation_fh =
          pid_control_B.rtb_CoordinateTransformationC_o;
      }

      /* End of Saturate: '<S300>/Saturation' */

      /* Sum: '<S285>/SumI4' incorporates:
       *  Gain: '<S285>/Kb'
       *  Gain: '<S290>/Integral Gain'
       *  Sum: '<S285>/SumI2'
       */
      pid_control_B.SumI4_ic = (pid_control_B.Saturation_fh -
        pid_control_B.rtb_CoordinateTransformationC_o) * 0.1 + -0.05 *
        pid_control_B.Sum1;
    }

    /* End of Outputs for SubSystem: '<Root>/Subsystem1' */

    /* Gain: '<Root>/Gain3' */
    pid_control_B.Gain3 = -pid_control_B.Saturation_fh;

    /* Gain: '<Root>/Gain4' */
    pid_control_B.Gain4 = -pid_control_B.Saturation_m;
    if (tmp_0) {
      /* MATLAB Function: '<Root>/MATLAB Function' */
      memset(&pid_control_B.stringOut_d[0], 0, sizeof(uint8_T) << 7U);
      for (i = 0; i < 11; i++) {
        pid_control_B.stringOut_d[i] = b_1[i];
      }

      pid_control_B.lengthOut_g = 11U;

      /* End of MATLAB Function: '<Root>/MATLAB Function' */

      /* MATLAB Function: '<Root>/MATLAB Function-MODELO' */
      memset(&pid_control_B.stringOut[0], 0, sizeof(uint8_T) << 7U);
      for (i = 0; i < 5; i++) {
        pid_control_B.stringOut[i] = b_0[i];
      }

      pid_control_B.lengthOut = 5U;

      /* End of MATLAB Function: '<Root>/MATLAB Function-MODELO' */
    }

    /* BusAssignment: '<Root>/Bus Assignment-MODELO' */
    pid_control_B.BusAssignmentMODELO.state.name_SL_Info.CurrentLength =
      pid_control_B.lengthOut_g;
    pid_control_B.BusAssignmentMODELO.state.pose.position.x =
      pid_control_B.px_out_l;
    pid_control_B.BusAssignmentMODELO.state.pose.position.y = pid_control_B.cosa;
    pid_control_B.BusAssignmentMODELO.state.pose.position.z = pid_control_B.cosc;
    pid_control_B.BusAssignmentMODELO.state.pose.orientation.w =
      pid_control_B.sinb;
    pid_control_B.BusAssignmentMODELO.state.pose.orientation.z =
      pid_control_B.sinc;
    pid_control_B.BusAssignmentMODELO.state.pose.orientation.y =
      pid_control_B.cosb;
    pid_control_B.BusAssignmentMODELO.state.pose.orientation.x =
      pid_control_B.sina;
    memcpy(&pid_control_B.BusAssignmentMODELO.state.name[0],
           &pid_control_B.stringOut_d[0], sizeof(uint8_T) << 7U);
    memcpy(&pid_control_B.BusAssignmentMODELO.state.reference_frame[0],
           &pid_control_B.stringOut[0], sizeof(uint8_T) << 7U);
    pid_control_B.BusAssignmentMODELO.state.reference_frame_SL_Info.CurrentLength
      = pid_control_B.lengthOut;

    /* Outputs for Atomic SubSystem: '<Root>/Call Service' */
    /* MATLABSystem: '<S6>/ServiceCaller' */
    serverAvailableOnTime = ServCall_pid_control_326.waitForServer(5.0);
    if (serverAvailableOnTime) {
      ServCall_pid_control_326.call(&pid_control_B.BusAssignmentMODELO, &tmp);
    }

    /* End of MATLABSystem: '<S6>/ServiceCaller' */
    /* End of Outputs for SubSystem: '<Root>/Call Service' */

    /* Gain: '<S66>/ZeroGain' */
    pid_control_B.sina = 0.0 * pid_control_B.IntegratorSecondOrderLimite;

    /* DeadZone: '<S68>/DeadZone' */
    if (pid_control_B.IntegratorSecondOrderLimite > 0.1) {
      pid_control_B.IntegratorSecondOrderLimite -= 0.1;
    } else if (pid_control_B.IntegratorSecondOrderLimite >= -0.1) {
      pid_control_B.IntegratorSecondOrderLimite = 0.0;
    } else {
      pid_control_B.IntegratorSecondOrderLimite -= -0.1;
    }

    /* End of DeadZone: '<S68>/DeadZone' */

    /* RelationalOperator: '<S66>/NotEqual' */
    serverAvailableOnTime = (pid_control_B.sina !=
      pid_control_B.IntegratorSecondOrderLimite);

    /* Signum: '<S66>/SignPreSat' */
    if (!rtIsNaN(pid_control_B.IntegratorSecondOrderLimite)) {
      if (pid_control_B.IntegratorSecondOrderLimite < 0.0) {
        pid_control_B.IntegratorSecondOrderLimite = -1.0;
      } else {
        pid_control_B.IntegratorSecondOrderLimite =
          (pid_control_B.IntegratorSecondOrderLimite > 0.0);
      }
    }

    /* End of Signum: '<S66>/SignPreSat' */

    /* Gain: '<S73>/Integral Gain' */
    pid_control_B.Square1 *= -0.05;

    /* DataTypeConversion: '<S66>/DataTypeConv1' */
    if (rtIsNaN(pid_control_B.IntegratorSecondOrderLimite)) {
      i = 0;
    } else {
      i = static_cast<int32_T>(fmod(pid_control_B.IntegratorSecondOrderLimite,
        256.0));
    }

    /* Signum: '<S66>/SignPreIntegrator' */
    if (rtIsNaN(pid_control_B.Square1)) {
      /* DataTypeConversion: '<S66>/DataTypeConv2' */
      rtb_sec_0 = 0;
    } else {
      if (pid_control_B.Square1 < 0.0) {
        /* DataTypeConversion: '<S66>/DataTypeConv2' */
        pid_control_B.cosb = -1.0;
      } else {
        /* DataTypeConversion: '<S66>/DataTypeConv2' */
        pid_control_B.cosb = (pid_control_B.Square1 > 0.0);
      }

      /* DataTypeConversion: '<S66>/DataTypeConv2' */
      rtb_sec_0 = static_cast<int32_T>(fmod(pid_control_B.cosb, 256.0));
    }

    /* End of Signum: '<S66>/SignPreIntegrator' */

    /* DataTypeConversion: '<S66>/DataTypeConv1' */
    if (i < 0) {
      i = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
        static_cast<real_T>(i))));
    }

    /* DataTypeConversion: '<S66>/DataTypeConv2' */
    if (rtb_sec_0 < 0) {
      rtb_sec_0 = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(
        -static_cast<real_T>(rtb_sec_0))));
    }

    /* Logic: '<S66>/AND3' incorporates:
     *  DataTypeConversion: '<S66>/DataTypeConv1'
     *  DataTypeConversion: '<S66>/DataTypeConv2'
     *  RelationalOperator: '<S66>/Equal1'
     */
    pid_control_B.AND3 = (serverAvailableOnTime && (i == rtb_sec_0));
    if (tmp_0) {
      /* Memory: '<S66>/Memory' */
      pid_control_B.Memory_a = pid_control_DW.Memory_PreviousInput_o;
    }

    /* Switch: '<S66>/Switch' */
    if (pid_control_B.Memory_a) {
      /* Switch: '<S66>/Switch' incorporates:
       *  Constant: '<S66>/Constant1'
       */
      pid_control_B.Switch = 0.0;
    } else {
      /* Switch: '<S66>/Switch' */
      pid_control_B.Switch = pid_control_B.Square1;
    }

    /* End of Switch: '<S66>/Switch' */

    /* Gain: '<S120>/ZeroGain' */
    pid_control_B.sina = 0.0 * pid_control_B.SignPreSat;

    /* DeadZone: '<S122>/DeadZone' */
    if (pid_control_B.SignPreSat > 1.0) {
      pid_control_B.SignPreSat--;
    } else if (pid_control_B.SignPreSat >= 0.0) {
      pid_control_B.SignPreSat = 0.0;
    }

    /* End of DeadZone: '<S122>/DeadZone' */

    /* Gain: '<S127>/Integral Gain' */
    pid_control_B.rtb_CoordinateTransformationC_b *= 0.015;

    /* Signum: '<S120>/SignPreSat' */
    if (rtIsNaN(pid_control_B.SignPreSat)) {
      /* DataTypeConversion: '<S120>/DataTypeConv1' */
      i = 0;
    } else {
      if (pid_control_B.SignPreSat < 0.0) {
        /* DataTypeConversion: '<S120>/DataTypeConv1' */
        pid_control_B.cosb = -1.0;
      } else {
        /* DataTypeConversion: '<S120>/DataTypeConv1' */
        pid_control_B.cosb = (pid_control_B.SignPreSat > 0.0);
      }

      /* DataTypeConversion: '<S120>/DataTypeConv1' */
      i = static_cast<int32_T>(fmod(pid_control_B.cosb, 256.0));
    }

    /* End of Signum: '<S120>/SignPreSat' */

    /* Signum: '<S120>/SignPreIntegrator' */
    if (rtIsNaN(pid_control_B.rtb_CoordinateTransformationC_b)) {
      /* DataTypeConversion: '<S120>/DataTypeConv2' */
      rtb_sec_0 = 0;
    } else {
      if (pid_control_B.rtb_CoordinateTransformationC_b < 0.0) {
        /* DataTypeConversion: '<S120>/DataTypeConv2' */
        pid_control_B.cosb = -1.0;
      } else {
        /* DataTypeConversion: '<S120>/DataTypeConv2' */
        pid_control_B.cosb = (pid_control_B.rtb_CoordinateTransformationC_b >
                              0.0);
      }

      /* DataTypeConversion: '<S120>/DataTypeConv2' */
      rtb_sec_0 = static_cast<int32_T>(fmod(pid_control_B.cosb, 256.0));
    }

    /* End of Signum: '<S120>/SignPreIntegrator' */

    /* DataTypeConversion: '<S120>/DataTypeConv1' */
    if (i < 0) {
      i = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(-
        static_cast<real_T>(i))));
    }

    /* DataTypeConversion: '<S120>/DataTypeConv2' */
    if (rtb_sec_0 < 0) {
      rtb_sec_0 = static_cast<int8_T>(-static_cast<int8_T>(static_cast<uint8_T>(
        -static_cast<real_T>(rtb_sec_0))));
    }

    /* Logic: '<S120>/AND3' incorporates:
     *  DataTypeConversion: '<S120>/DataTypeConv1'
     *  DataTypeConversion: '<S120>/DataTypeConv2'
     *  RelationalOperator: '<S120>/Equal1'
     *  RelationalOperator: '<S120>/NotEqual'
     */
    pid_control_B.AND3_c = ((pid_control_B.sina != pid_control_B.SignPreSat) &&
      (i == rtb_sec_0));
    if (tmp_0) {
      /* Memory: '<S120>/Memory' */
      pid_control_B.Memory_h = pid_control_DW.Memory_PreviousInput_a;
    }

    /* Switch: '<S120>/Switch' */
    if (pid_control_B.Memory_h) {
      /* Switch: '<S120>/Switch' incorporates:
       *  Constant: '<S120>/Constant1'
       */
      pid_control_B.Switch_j = 0.0;
    } else {
      /* Switch: '<S120>/Switch' */
      pid_control_B.Switch_j = pid_control_B.rtb_CoordinateTransformationC_b;
    }

    /* End of Switch: '<S120>/Switch' */

    /* UnitConversion: '<S163>/Unit Conversion' */
    /* Unit Conversion - from: m to: ft
       Expression: output = (3.28084*input) + (0) */
    pid_control_B.sina = 3.280839895013123 * pid_control_B.x[11];

    /* Saturate: '<S196>/Limit Function 10ft to 1000ft' */
    if (pid_control_B.sina > 1000.0) {
      pid_control_B.rtb_CoordinateTransformationC_o = 1000.0;
    } else if (pid_control_B.sina < 10.0) {
      pid_control_B.rtb_CoordinateTransformationC_o = 10.0;
    } else {
      pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.sina;
    }

    /* End of Saturate: '<S196>/Limit Function 10ft to 1000ft' */

    /* Gain: '<S168>/Lw' */
    pid_control_B.sinc = pid_control_ConstB.UnitConversion_c;

    /* Interpolation_n-D: '<S178>/Medium//High Altitude Intensity' incorporates:
     *  PreLookup: '<S178>/PreLook-Up Index Search  (altitude)'
     *  PreLookup: '<S178>/PreLook-Up Index Search  (prob of exceed)'
     *  SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited'
     *  UnitConversion: '<S163>/Unit Conversion'
     */
    pid_control_B.bpIndex[0] = plook_bincpa(pid_control_B.sina,
      pid_control_ConstP.PreLookUpIndexSearchaltitude_Br, 11U,
      &pid_control_B.IntegratorSecondOrderLimite,
      &pid_control_DW.PreLookUpIndexSearchaltitude_DW);
    pid_control_B.bpIndex[1] = pid_control_ConstB.PreLookUpIndexSearchprobo_g;
    pid_control_B.Product1_l[0] = pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.Product1_l[1] = pid_control_ConstB.PreLookUpIndexSearchprobofe;
    pid_control_B.sinb = intrp2d_la_pw(pid_control_B.bpIndex,
      pid_control_B.Product1_l,
      pid_control_ConstP.MediumHighAltitudeIntensity_Tab, 12U,
      pid_control_ConstP.MediumHighAltitudeIntensity_max);
    if (tmp_0) {
      /* Product: '<S171>/Product' incorporates:
       *  Product: '<S171>/Divide'
       *  RandomNumber: '<S171>/White Noise'
       */
      _mm_storeu_pd(&pid_control_B.Product[0], _mm_mul_pd(_mm_loadu_pd
        (&pid_control_ConstB.Divide[0]), _mm_loadu_pd
        (&pid_control_DW.NextOutput[0])));
      _mm_storeu_pd(&pid_control_B.Product[2], _mm_mul_pd(_mm_loadu_pd
        (&pid_control_ConstB.Divide[2]), _mm_loadu_pd
        (&pid_control_DW.NextOutput[2])));

      /* Memory: '<S34>/Memory' */
      pid_control_B.Memory[0] = pid_control_DW.Memory_PreviousInput[0];

      /* Memory: '<S34>/Memory1' */
      pid_control_B.Memory1[0] = pid_control_DW.Memory1_PreviousInput[0];

      /* Memory: '<S34>/Memory' */
      pid_control_B.Memory[1] = pid_control_DW.Memory_PreviousInput[1];

      /* Memory: '<S34>/Memory1' */
      pid_control_B.Memory1[1] = pid_control_DW.Memory1_PreviousInput[1];

      /* Memory: '<S34>/Memory' */
      pid_control_B.Memory[2] = pid_control_DW.Memory_PreviousInput[2];

      /* Memory: '<S34>/Memory1' */
      pid_control_B.Memory1[2] = pid_control_DW.Memory1_PreviousInput[2];
    }

    /* MATLAB Function: '<S34>/MATLAB Function - MODEL' incorporates:
     *  Memory: '<S34>/Memory'
     *  Memory: '<S34>/Memory1'
     *  SignalConversion generated from: '<S154>/ SFunction '
     */
    if (pid_control_B.Saturation_k <= 0.3490658503988659) {
      pid_control_B.u1 = pid_control_B.Saturation_k;
    } else {
      pid_control_B.u1 = 0.3490658503988659;
    }

    if (!(pid_control_B.u1 >= -0.3490658503988659)) {
      pid_control_B.u1 = -0.3490658503988659;
    }

    if (pid_control_B.Saturation_fh <= 0.3490658503988659) {
      pid_control_B.u2 = pid_control_B.Saturation_fh;
    } else {
      pid_control_B.u2 = 0.3490658503988659;
    }

    if (!(pid_control_B.u2 >= -0.3490658503988659)) {
      pid_control_B.u2 = -0.3490658503988659;
    }

    _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_loadu_pd
      (&pid_control_B.x[0]), _mm_loadu_pd(&pid_control_B.Memory[0])));
    pid_control_B.cosb = pid_control_B.x[2] + pid_control_B.Memory[2];
    pid_control_B.cosa = sqrt((pid_control_B.dv1[0] * pid_control_B.dv1[0] +
      pid_control_B.dv1[1] * pid_control_B.dv1[1]) + pid_control_B.cosb *
      pid_control_B.cosb);
    if (pid_control_B.cosa < 0.5) {
      pid_control_B.cosa = 0.5;
      pid_control_B.cosb = 0.0;
      pid_control_B.cosc = 0.0;
    } else {
      pid_control_B.cosb = rt_atan2d_snf(pid_control_B.cosb, pid_control_B.dv1[0]);
      pid_control_B.cosc = pid_control_B.dv1[1] / pid_control_B.cosa;
      if ((pid_control_B.cosc >= 1.0) || rtIsNaN(pid_control_B.cosc)) {
        pid_control_B.cosc = 1.0;
      }

      if (pid_control_B.cosc <= -1.0) {
        pid_control_B.cosc = -1.0;
      }

      pid_control_B.cosc = asin(pid_control_B.cosc);
    }

    pid_control_B.q_aero = pid_control_B.Memory1[1] + pid_control_B.x[4];
    if ((-pid_control_B.x[11] - 0.0505 <= 0.001) || rtIsNaN(-pid_control_B.x[11]
         - 0.0505)) {
      pid_control_B.rtb_CoordinateTransformationC_b = 0.001;
    } else {
      pid_control_B.rtb_CoordinateTransformationC_b = -pid_control_B.x[11] -
        0.0505;
    }

    if ((-pid_control_B.x[11] + 0.3475 <= 0.001) || rtIsNaN(-pid_control_B.x[11]
         + 0.3475)) {
      pid_control_B.px_out_l = 0.001;
    } else {
      pid_control_B.px_out_l = -pid_control_B.x[11] + 0.3475;
    }

    pid_control_B.Q = pid_control_B.cosa * pid_control_B.cosa * 0.6125;
    pid_control_B.wbe_b[0] = pid_control_B.x[3];
    pid_control_B.wbe_b[1] = pid_control_B.x[4];
    pid_control_B.wbe_b[2] = pid_control_B.x[5];
    pid_control_B.Vd1 = pid_control_B.rtb_CoordinateTransformationC_b / 0.6977;
    pid_control_B.Sum1 = pid_control_B.px_out_l / 0.3808;
    pid_control_B.px_out_l = ((pid_control_B.cosb - -0.06544984694978735) +
      0.02617993877991494) * 4.960409453036515;
    pid_control_B.rtb_CoordinateTransformationC_b = (((pid_control_B.cosb -
      -0.04363323129985824) + 0.008726646259971648) - (0.56 / pid_control_B.cosa
      * 0.35 * pid_control_B.q_aero + (pid_control_B.cosb - -0.06544984694978735)
      * 0.35)) * 4.838774891736003;
    pid_control_B.SignPreSat = (rt_powd_snf(pid_control_B.Vd1, 0.787) * 288.0 *
      exp(rt_powd_snf(pid_control_B.Vd1, 0.327) * -9.14) * 0.9798630886207249 /
      5.912947654095886 + 1.0) * pid_control_B.px_out_l;
    pid_control_B.IntegratorSecondOrderLimite = (rt_powd_snf(pid_control_B.Sum1,
      0.787) * 288.0 * exp(rt_powd_snf(pid_control_B.Sum1, 0.327) * -9.14) *
      0.9562859020012823 / 5.35300902982722 + 1.0) *
      pid_control_B.rtb_CoordinateTransformationC_b;
    pid_control_B.Square1 = ((1.0 - exp(rt_powd_snf(pid_control_B.Vd1, 0.814) *
      -4.74) * 0.9791664172678959) - exp(rt_powd_snf(pid_control_B.Vd1, 0.758) *
      -3.88) * (pid_control_B.Vd1 * pid_control_B.Vd1)) *
      (pid_control_B.SignPreSat * pid_control_B.SignPreSat / 21.205750411731103);
    pid_control_B.Sum1 = ((1.0 - exp(rt_powd_snf(pid_control_B.Sum1, 0.814) *
      -4.74) * 0.9677063475148586) - exp(rt_powd_snf(pid_control_B.Sum1, 0.758) *
      -3.88) * (pid_control_B.Sum1 * pid_control_B.Sum1)) *
      (pid_control_B.IntegratorSecondOrderLimite *
       pid_control_B.IntegratorSecondOrderLimite / 18.943803701146454);
    pid_control_B.u2_deg = pid_control_B.u2 * 180.0 / 3.141592653589793;
    pid_control_B.u2_deg = ((pid_control_B.u2_deg * pid_control_B.u2_deg *
      -1.08E-5 + 0.000715 * pid_control_B.u2_deg) * 0.02164 +
      ((pid_control_B.Square1 * 0.0649 + 0.0027258) + pid_control_B.Sum1 *
       0.02164)) * pid_control_B.Q;
    pid_control_B.Ltot = (pid_control_B.SignPreSat * 0.0649 +
                          pid_control_B.IntegratorSecondOrderLimite * 0.02164) *
      pid_control_B.Q;
    pid_control_B.CQ = -0.019 * pid_control_B.cosc * 180.0 / 3.141592653589793;
    pid_control_B.FA_b_idx_0 = sin(pid_control_B.cosb);
    pid_control_B.FA_b_idx_1 = cos(pid_control_B.cosb);
    pid_control_B.R[0] = pid_control_B.FA_b_idx_1;
    pid_control_B.R[3] = 0.0;
    pid_control_B.R[6] = -pid_control_B.FA_b_idx_0;
    pid_control_B.R[2] = pid_control_B.FA_b_idx_0;
    pid_control_B.R[5] = 0.0;
    pid_control_B.R[8] = pid_control_B.FA_b_idx_1;
    pid_control_B.Dtot[0] = -pid_control_B.u2_deg;
    pid_control_B.Dtot[1] = pid_control_B.CQ * pid_control_B.Q * 0.0649;
    pid_control_B.Dtot[2] = -pid_control_B.Ltot;
    pid_control_B.R[1] = 0.0;
    pid_control_B.FA_b_idx_0 = 0.0;
    pid_control_B.R[4] = 1.0;
    pid_control_B.FA_b_idx_1 = 0.0;
    pid_control_B.R[7] = 0.0;
    pid_control_B.FA_b_idx_2 = 0.0;
    for (i = 0; i < 3; i++) {
      _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&pid_control_B.R[3 * i]), _mm_set1_pd(pid_control_B.Dtot[i])),
        _mm_set_pd(pid_control_B.FA_b_idx_1, pid_control_B.FA_b_idx_0)));
      pid_control_B.FA_b_idx_0 = pid_control_B.dv1[0];
      pid_control_B.FA_b_idx_1 = pid_control_B.dv1[1];
      pid_control_B.FA_b_idx_2 += pid_control_B.R[3 * i + 2] *
        pid_control_B.Dtot[i];
    }

    pid_control_B.Fg_b_idx_2 = 2.0 * pid_control_B.cosa;
    pid_control_B.Cl = ((pid_control_B.Memory1[0] + pid_control_B.x[3]) * 0.6977
                        / pid_control_B.Fg_b_idx_2 * -2.0 + -0.12 *
                        pid_control_B.cosc) + -0.5 * pid_control_B.u1;
    pid_control_B.u2 = ((exp(pid_control_B.Vd1 * -4.0) * -0.05 + -1.14 *
                         pid_control_B.cosb) + pid_control_B.q_aero * 0.093 /
                        pid_control_B.Fg_b_idx_2 * -5.0) + -3.0 *
      pid_control_B.u2;
    if (pid_control_B.Saturation_m <= 0.2617993877991494) {
      pid_control_B.q_aero = pid_control_B.Saturation_m;
    } else {
      pid_control_B.q_aero = 0.2617993877991494;
    }

    if (!(pid_control_B.q_aero >= -0.2617993877991494)) {
      pid_control_B.q_aero = -0.2617993877991494;
    }

    pid_control_B.u1 = (((pid_control_B.Memory1[2] + pid_control_B.x[5]) *
                         0.6977 / pid_control_B.Fg_b_idx_2 * -1.5 + -0.1146 *
                         pid_control_B.cosc) + -0.3 * pid_control_B.q_aero) +
      -0.05 * pid_control_B.u1;
    if (pid_control_B.Saturation <= 1.0) {
      pid_control_B.q_aero = pid_control_B.Saturation;
    } else {
      pid_control_B.q_aero = 1.0;
    }

    if (!(pid_control_B.q_aero >= 0.0)) {
      pid_control_B.q_aero = 0.0;
    }

    pid_control_B.Vd1 = (32.0 - pid_control_B.cosa) * pid_control_B.q_aero +
      pid_control_B.cosa;
    if (pid_control_B.Saturation <= 1.0) {
      pid_control_B.q_aero = pid_control_B.Saturation;
    } else {
      pid_control_B.q_aero = 1.0;
    }

    if (!(pid_control_B.q_aero >= 0.0)) {
      pid_control_B.q_aero = 0.0;
    }

    pid_control_B.q_aero = (32.0 - pid_control_B.cosa) * pid_control_B.q_aero +
      pid_control_B.cosa;
    pid_control_B.Vd1 = 0.004422606215997847 * pid_control_B.Vd1 *
      (pid_control_B.Vd1 - pid_control_B.cosa);
    pid_control_B.q_aero = 0.004422606215997847 * pid_control_B.q_aero *
      (pid_control_B.q_aero - pid_control_B.cosa);
    pid_control_B.FE1_b[0] = pid_control_B.Vd1 * 0.9961946980917455;
    pid_control_B.FE1_b[2] = -pid_control_B.Vd1 * 0.08715574274765817;
    pid_control_B.FE2_b_idx_0 = pid_control_B.q_aero * 0.9961946980917455;
    pid_control_B.FE2_b_idx_2 = -pid_control_B.q_aero * 0.08715574274765817;
    pid_control_B.q_aero = -9.81 * sin(pid_control_B.x[7]) * 1.2;
    _mm_storeu_pd(&pid_control_B.dv1[0], _mm_mul_pd(_mm_mul_pd(_mm_mul_pd
      (_mm_set1_pd(9.81), _mm_set1_pd(cos(pid_control_B.x[7]))), _mm_set_pd(cos
      (pid_control_B.x[6]), sin(pid_control_B.x[6]))), _mm_set1_pd(1.2)));
    pid_control_B.Vd1 = pid_control_B.dv1[0];
    pid_control_B.Fg_b_idx_2 = pid_control_B.dv1[1];
    pid_control_B.FE_b = pid_control_B.FE1_b[0] + pid_control_B.FE2_b_idx_0;
    pid_control_B.FE_b_idx_0 = pid_control_B.FE_b;
    pid_control_B.F_b[0] = (pid_control_B.q_aero + pid_control_B.FE_b) +
      pid_control_B.FA_b_idx_0;
    pid_control_B.F_b[1] = pid_control_B.dv1[0] + pid_control_B.FA_b_idx_1;
    pid_control_B.FE_b = pid_control_B.FE1_b[2] + pid_control_B.FE2_b_idx_2;
    pid_control_B.F_b[2] = (pid_control_B.dv1[1] + pid_control_B.FE_b) +
      pid_control_B.FA_b_idx_2;
    pid_control_B.L_dec = 0.6977 * pid_control_B.Q * 0.0649;
    pid_control_B.Mcg_b_idx_0 = (0.0834 * pid_control_B.FE1_b[2] + -0.0834 *
      pid_control_B.FE2_b_idx_2) + pid_control_B.L_dec * pid_control_B.Cl;
    pid_control_B.Q = 0.093 * pid_control_B.Q * 0.0649 * pid_control_B.u2 +
      ((-0.0396 * pid_control_B.FE1_b[0] - 0.0721 * pid_control_B.FE1_b[2]) +
       (-0.0396 * pid_control_B.FE2_b_idx_0 - 0.0721 * pid_control_B.FE2_b_idx_2));
    pid_control_B.FE2_b_idx_0 = ((0.0 - 0.0834 * pid_control_B.FE1_b[0]) + (0.0
      - -0.0834 * pid_control_B.FE2_b_idx_0)) + pid_control_B.L_dec *
      pid_control_B.u1;
    pid_control_B.L_dec = pid_control_B.Mcg_b_idx_0 - 0.07841480417423635 *
      pid_control_B.FE2_b_idx_0;
    pid_control_B.FE2_b_idx_2 = pid_control_B.FE2_b_idx_0 - 0.2258731897286114 *
      pid_control_B.Mcg_b_idx_0;
    memcpy(&pid_control_B.R[0], &pid_control_B.x[0], 9U * sizeof(real_T));
    pid_control_B.FE1_b[0] = pid_control_B.R[0];
    pid_control_B.FE1_b[1] = pid_control_B.R[1];
    pid_control_B.FE1_b[2] = pid_control_B.R[2];
    pid_control_B.c_phi = cos(pid_control_B.R[6]);
    pid_control_B.s_phi = sin(pid_control_B.R[6]);
    pid_control_B.c_the = cos(pid_control_B.R[7]);
    pid_control_B.s_the = sin(pid_control_B.R[7]);
    pid_control_B.c_psi = cos(pid_control_B.R[8]);
    pid_control_B.s_psi = sin(pid_control_B.R[8]);
    pid_control_B.R[0] = pid_control_B.c_the * pid_control_B.c_psi;
    pid_control_B.R_tmp = pid_control_B.s_phi * pid_control_B.s_the;
    pid_control_B.R[3] = pid_control_B.R_tmp * pid_control_B.c_psi -
      pid_control_B.c_phi * pid_control_B.s_psi;
    pid_control_B.R_tmp_n = pid_control_B.c_phi * pid_control_B.s_the;
    pid_control_B.R[6] = pid_control_B.R_tmp_n * pid_control_B.c_psi +
      pid_control_B.s_phi * pid_control_B.s_psi;
    pid_control_B.R[1] = pid_control_B.c_the * pid_control_B.s_psi;
    pid_control_B.R[4] = pid_control_B.R_tmp * pid_control_B.s_psi +
      pid_control_B.c_phi * pid_control_B.c_psi;
    pid_control_B.R[7] = pid_control_B.R_tmp_n * pid_control_B.s_psi -
      pid_control_B.s_phi * pid_control_B.c_psi;
    pid_control_B.R[2] = -pid_control_B.s_the;
    pid_control_B.R[5] = pid_control_B.s_phi * pid_control_B.c_the;
    pid_control_B.R[8] = pid_control_B.c_phi * pid_control_B.c_the;
    _mm_storeu_pd(&pid_control_B.Dtot[0], _mm_sub_pd(_mm_mul_pd(_mm_set_pd
      (pid_control_B.x[0], pid_control_B.x[2]), _mm_loadu_pd(&pid_control_B.x[4])),
      _mm_mul_pd(_mm_loadu_pd(&pid_control_B.x[1]), _mm_set_pd(pid_control_B.x[3],
      pid_control_B.x[5]))));
    pid_control_B.Dtot[2] = pid_control_B.x[1] * pid_control_B.x[3] -
      pid_control_B.x[0] * pid_control_B.x[4];
    pid_control_B.dv[0] = 1.0;
    _mm_storeu_pd(&pid_control_B.dv1[0], _mm_mul_pd(_mm_set_pd(cos
      (pid_control_B.x[6]), sin(pid_control_B.x[6])), _mm_set1_pd(tan
      (pid_control_B.x[7]))));
    pid_control_B.dv[3] = pid_control_B.dv1[0];
    pid_control_B.dv[6] = pid_control_B.dv1[1];
    pid_control_B.dv[1] = 0.0;
    pid_control_B.dv[4] = cos(pid_control_B.x[6]);
    pid_control_B.dv[7] = -sin(pid_control_B.x[6]);
    pid_control_B.dv[2] = 0.0;
    _mm_storeu_pd(&pid_control_B.dv1[0], _mm_div_pd(_mm_set_pd(cos
      (pid_control_B.x[6]), sin(pid_control_B.x[6])), _mm_set1_pd(cos
      (pid_control_B.x[7]))));
    pid_control_B.dv[5] = pid_control_B.dv1[0];
    pid_control_B.dv[8] = pid_control_B.dv1[1];
    pid_control_B.c_phi = 0.0;
    pid_control_B.s_phi = 0.0;
    pid_control_B.c_the = 0.0;
    pid_control_B.s_the = 0.0;
    pid_control_B.c_psi = 0.0;
    pid_control_B.s_psi = 0.0;
    for (i = 0; i < 3; i++) {
      _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&pid_control_B.dv[3 * i]), _mm_set1_pd(pid_control_B.wbe_b[i])),
        _mm_set_pd(pid_control_B.s_phi, pid_control_B.c_phi)));
      pid_control_B.c_phi = pid_control_B.dv1[0];
      pid_control_B.s_phi = pid_control_B.dv1[1];
      _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
        (pid_control_B.R[3 * i], pid_control_B.dv[3 * i + 2]), _mm_set_pd
        (pid_control_B.FE1_b[i], pid_control_B.wbe_b[i])), _mm_set_pd
        (pid_control_B.s_the, pid_control_B.c_the)));
      pid_control_B.c_the = pid_control_B.dv1[0];
      pid_control_B.s_the = pid_control_B.dv1[1];
      _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd
        (&pid_control_B.R[3 * i + 1]), _mm_set1_pd(pid_control_B.FE1_b[i])),
        _mm_set_pd(pid_control_B.s_psi, pid_control_B.c_psi)));
      pid_control_B.c_psi = pid_control_B.dv1[0];
      pid_control_B.s_psi = pid_control_B.dv1[1];
      pid_control_B.XDOT[i] = 0.8333333333333334 * pid_control_B.F_b[i] -
        pid_control_B.Dtot[i];
    }

    pid_control_B.XDOT[3] = ((0.023669 * pid_control_B.L_dec + 0.001856 *
      pid_control_B.FE2_b_idx_2) - (-2.6338496E-5 * pid_control_B.x[3] +
      0.000144843342 * pid_control_B.x[5]) * pid_control_B.x[4]) /
      0.000191043437;
    pid_control_B.XDOT[4] = ((pid_control_B.Q - -0.015451999999999999 *
      pid_control_B.x[3] * pid_control_B.x[5]) - (pid_control_B.x[3] *
      pid_control_B.x[3] - pid_control_B.x[5] * pid_control_B.x[5]) * 0.001856) /
      0.017695;
    pid_control_B.XDOT[5] = ((-2.6338496E-5 * pid_control_B.x[5] +
      -7.443598999999999E-5 * pid_control_B.x[3]) * pid_control_B.x[4] +
      (0.001856 * pid_control_B.L_dec + 0.008217 * pid_control_B.FE2_b_idx_2)) /
      0.000191043437;
    pid_control_B.XDOT[9] = pid_control_B.s_the;
    pid_control_B.XDOT[10] = pid_control_B.c_psi;
    pid_control_B.XDOT[11] = pid_control_B.s_psi;
    if (!(pid_control_B.u2_deg >= 0.001)) {
      pid_control_B.u2_deg = 0.001;
    }

    pid_control_B.XDOT[12] = pid_control_B.Ltot / pid_control_B.u2_deg;
    pid_control_B.XDOT[19] = pid_control_B.CQ;
    pid_control_B.XDOT[20] = pid_control_B.Cl;
    pid_control_B.XDOT[21] = pid_control_B.u2;
    pid_control_B.XDOT[22] = pid_control_B.u1;
    pid_control_B.XDOT[23] = pid_control_B.cosb;
    pid_control_B.XDOT[24] = pid_control_B.cosc;
    pid_control_B.XDOT[25] = pid_control_B.px_out_l;
    pid_control_B.XDOT[26] = pid_control_B.rtb_CoordinateTransformationC_b;
    pid_control_B.XDOT[27] = pid_control_B.SignPreSat;
    pid_control_B.XDOT[28] = pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.XDOT[29] = pid_control_B.Square1;
    pid_control_B.XDOT[30] = pid_control_B.Sum1;
    pid_control_B.XDOT[6] = pid_control_B.c_phi;
    pid_control_B.XDOT[13] = pid_control_B.F_b[0];
    pid_control_B.XDOT[16] = pid_control_B.Mcg_b_idx_0;
    pid_control_B.XDOT[31] = pid_control_B.q_aero;
    pid_control_B.XDOT[34] = pid_control_B.FE_b_idx_0;
    pid_control_B.XDOT[37] = pid_control_B.FA_b_idx_0;
    pid_control_B.XDOT[7] = pid_control_B.s_phi;
    pid_control_B.XDOT[14] = pid_control_B.F_b[1];
    pid_control_B.XDOT[17] = pid_control_B.Q;
    pid_control_B.XDOT[32] = pid_control_B.Vd1;
    pid_control_B.XDOT[35] = 0.0;
    pid_control_B.XDOT[38] = pid_control_B.FA_b_idx_1;
    pid_control_B.XDOT[8] = pid_control_B.c_the;
    pid_control_B.XDOT[15] = pid_control_B.F_b[2];
    pid_control_B.XDOT[18] = pid_control_B.FE2_b_idx_0;
    pid_control_B.XDOT[33] = pid_control_B.Fg_b_idx_2;
    pid_control_B.XDOT[36] = pid_control_B.FE_b;
    pid_control_B.XDOT[39] = pid_control_B.FA_b_idx_2;

    /* UnitConversion: '<S169>/Unit Conversion' incorporates:
     *  MATLAB Function: '<S34>/MATLAB Function - MODEL'
     */
    /* Unit Conversion - from: m/s to: ft/s
       Expression: output = (3.28084*input) + (0) */
    pid_control_B.cosa *= 3.280839895013123;

    /* Outputs for Enabled SubSystem: '<S161>/Hpgw' incorporates:
     *  EnablePort: '<S172>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hpgw_MODE) {
      (void) memset(&(pid_control_XDis.pgw_p_CSTATE), 0,
                    2*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S172>/pgw_p' */
      pid_control_X.pgw_p_CSTATE[0] = 0.0;
      pid_control_X.pgw_p_CSTATE[1] = 0.0;
      pid_control_DW.Hpgw_MODE = true;
    }

    if (pid_control_DW.Hpgw_MODE) {
      /* Fcn: '<S172>/sqrt(0.8//V)' */
      pid_control_B.cosb = sqrt(0.8 / pid_control_B.cosa);

      /* Product: '<S172>/w3' */
      pid_control_B.cosc = pid_control_B.cosa * pid_control_ConstB.w4;

      /* Product: '<S172>/w' incorporates:
       *  Fcn: '<S172>/sqrt(0.8//V)'
       *  Gain: '<S168>/Lw'
       *  Integrator: '<S172>/pgw_p'
       *  Math: '<S172>/L^1//3'
       *  Product: '<S172>/Lug//V1'
       *  Product: '<S172>/w1'
       *  Product: '<S172>/w2'
       *  Sum: '<S172>/Sum'
       */
      pid_control_B.w_o[0] = (pid_control_B.cosb / rt_powd_snf
        (pid_control_B.rtb_CoordinateTransformationC_o, 0.3333333333333333) *
        pid_control_ConstB.u16 * pid_control_B.Product[3] -
        pid_control_X.pgw_p_CSTATE[0]) * pid_control_B.cosc;

      /* Math: '<S172>/L^1//3' */
      if (pid_control_B.sinc < 0.0) {
        pid_control_B.q_aero = -rt_powd_snf(-pid_control_B.sinc,
          0.3333333333333333);
      } else {
        pid_control_B.q_aero = rt_powd_snf(pid_control_B.sinc,
          0.3333333333333333);
      }

      /* Product: '<S172>/w' incorporates:
       *  Fcn: '<S172>/sqrt(0.8//V)'
       *  Integrator: '<S172>/pgw_p'
       *  Product: '<S172>/Lug//V1'
       *  Product: '<S172>/w1'
       *  Product: '<S172>/w2'
       *  Sum: '<S172>/Sum'
       */
      pid_control_B.w_o[1] = (pid_control_B.cosb / pid_control_B.q_aero *
        pid_control_ConstB.u16 * pid_control_B.Product[3] -
        pid_control_X.pgw_p_CSTATE[1]) * pid_control_B.cosc;

      /* Product: '<S172>/sigma_w' incorporates:
       *  Integrator: '<S172>/pgw_p'
       */
      _mm_storeu_pd(&pid_control_B.sigma_w[0], _mm_mul_pd(_mm_set_pd
        (pid_control_B.sinb, pid_control_ConstB.sigma_wg), _mm_loadu_pd
        (&pid_control_X.pgw_p_CSTATE[0])));
    }

    /* End of Outputs for SubSystem: '<S161>/Hpgw' */

    /* Outputs for Enabled SubSystem: '<S162>/Hwgw(s)' incorporates:
     *  EnablePort: '<S177>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hwgws_MODE) {
      (void) memset(&(pid_control_XDis.wg_p1_CSTATE), 0,
                    4*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S177>/wg_p1' */
      pid_control_X.wg_p1_CSTATE[0] = 0.0;

      /* InitializeConditions for Integrator: '<S177>/wg_p2' */
      pid_control_X.wg_p2_CSTATE[0] = 0.0;

      /* InitializeConditions for Integrator: '<S177>/wg_p1' */
      pid_control_X.wg_p1_CSTATE[1] = 0.0;

      /* InitializeConditions for Integrator: '<S177>/wg_p2' */
      pid_control_X.wg_p2_CSTATE[1] = 0.0;
      pid_control_DW.Hwgws_MODE = true;
    }

    if (pid_control_DW.Hwgws_MODE) {
      /* Product: '<S177>/Lwg//V' incorporates:
       *  Gain: '<S168>/Lw'
       */
      pid_control_B.px_out_l = pid_control_B.rtb_CoordinateTransformationC_o /
        pid_control_B.cosa;

      /* Product: '<S177>/w' incorporates:
       *  Gain: '<S177>/1//pi'
       *  Integrator: '<S177>/wg_p1'
       *  Product: '<S177>/Lug//V1'
       *  Sqrt: '<S177>/sqrt1'
       *  Sum: '<S177>/Sum'
       */
      pid_control_B.cosb = (sqrt(0.3183098861837907 * pid_control_B.px_out_l) *
                            pid_control_B.Product[2] -
                            pid_control_X.wg_p1_CSTATE[0]) /
        pid_control_B.px_out_l;
      pid_control_B.w[0] = pid_control_B.cosb;

      /* Product: '<S177>/w ' incorporates:
       *  Integrator: '<S177>/wg_p1'
       *  Integrator: '<S177>/wg_p2'
       *  Product: '<S177>/Lwg//V '
       *  Sum: '<S177>/Sum1'
       */
      pid_control_B.w_a[0] = (pid_control_B.cosb * pid_control_ConstB.sqrt_a *
        pid_control_B.px_out_l + (pid_control_X.wg_p1_CSTATE[0] -
        pid_control_X.wg_p2_CSTATE[0])) / pid_control_B.px_out_l;

      /* Product: '<S177>/Lwg//V' */
      pid_control_B.px_out_l = pid_control_B.sinc / pid_control_B.cosa;

      /* Product: '<S177>/w' incorporates:
       *  Gain: '<S177>/1//pi'
       *  Integrator: '<S177>/wg_p1'
       *  Product: '<S177>/Lug//V1'
       *  Sqrt: '<S177>/sqrt1'
       *  Sum: '<S177>/Sum'
       */
      pid_control_B.cosb = (sqrt(0.3183098861837907 * pid_control_B.px_out_l) *
                            pid_control_B.Product[2] -
                            pid_control_X.wg_p1_CSTATE[1]) /
        pid_control_B.px_out_l;
      pid_control_B.w[1] = pid_control_B.cosb;

      /* Product: '<S177>/w ' incorporates:
       *  Integrator: '<S177>/wg_p1'
       *  Integrator: '<S177>/wg_p2'
       *  Product: '<S177>/Lwg//V '
       *  Sum: '<S177>/Sum1'
       */
      pid_control_B.w_a[1] = (pid_control_B.cosb * pid_control_ConstB.sqrt_a *
        pid_control_B.px_out_l + (pid_control_X.wg_p1_CSTATE[1] -
        pid_control_X.wg_p2_CSTATE[1])) / pid_control_B.px_out_l;

      /* Product: '<S177>/Lwg//V 1' incorporates:
       *  Integrator: '<S177>/wg_p2'
       */
      _mm_storeu_pd(&pid_control_B.LwgV1[0], _mm_mul_pd(_mm_set_pd
        (pid_control_B.sinb, pid_control_ConstB.sigma_wg), _mm_loadu_pd
        (&pid_control_X.wg_p2_CSTATE[0])));
    }

    /* End of Outputs for SubSystem: '<S162>/Hwgw(s)' */

    /* Outputs for Enabled SubSystem: '<S161>/Hqgw' incorporates:
     *  EnablePort: '<S173>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hqgw_MODE) {
      (void) memset(&(pid_control_XDis.qgw_p_CSTATE), 0,
                    2*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S173>/qgw_p' */
      pid_control_X.qgw_p_CSTATE[0] = 0.0;
      pid_control_X.qgw_p_CSTATE[1] = 0.0;
      pid_control_DW.Hqgw_MODE = true;
    }

    if (pid_control_DW.Hqgw_MODE) {
      /* Gain: '<S173>/pi//4' */
      pid_control_B.sinc = 0.7853981633974483 * pid_control_B.cosa;

      /* Product: '<S173>/w' incorporates:
       *  Integrator: '<S173>/qgw_p'
       *  Product: '<S173>/wg//V'
       *  Sum: '<S173>/Sum'
       */
      pid_control_B.cosb = (pid_control_B.LwgV1[0] / pid_control_B.cosa -
                            pid_control_X.qgw_p_CSTATE[0]) * (pid_control_B.sinc
        / pid_control_ConstB.UnitConversion_n);
      pid_control_B.w_e0[0] = pid_control_B.cosb;

      /* UnaryMinus: '<S173>/Unary Minus' */
      pid_control_B.UnaryMinus[0] = -pid_control_B.cosb;

      /* Product: '<S173>/w' incorporates:
       *  Integrator: '<S173>/qgw_p'
       *  Product: '<S173>/wg//V'
       *  Sum: '<S173>/Sum'
       */
      pid_control_B.cosb = (pid_control_B.LwgV1[1] / pid_control_B.cosa -
                            pid_control_X.qgw_p_CSTATE[1]) * (pid_control_B.sinc
        / pid_control_ConstB.UnitConversion_n);
      pid_control_B.w_e0[1] = pid_control_B.cosb;

      /* UnaryMinus: '<S173>/Unary Minus' */
      pid_control_B.UnaryMinus[1] = -pid_control_B.cosb;
    }

    /* End of Outputs for SubSystem: '<S161>/Hqgw' */

    /* Saturate: '<S179>/Limit Height h<1000ft' */
    if (pid_control_B.sina > 1000.0) {
      pid_control_B.cosb = 1000.0;
    } else if (pid_control_B.sina < 0.0) {
      pid_control_B.cosb = 0.0;
    } else {
      pid_control_B.cosb = pid_control_B.sina;
    }

    /* Product: '<S179>/sigma_ug, sigma_vg' incorporates:
     *  Fcn: '<S179>/Low Altitude Intensity'
     *  Saturate: '<S179>/Limit Height h<1000ft'
     */
    pid_control_B.cosb = 1.0 / rt_powd_snf(0.000823 * pid_control_B.cosb + 0.177,
      0.4) * pid_control_ConstB.sigma_wg;

    /* Fcn: '<S196>/Low Altitude Scale Length' */
    pid_control_B.cosc = pid_control_B.rtb_CoordinateTransformationC_o /
      rt_powd_snf(0.000823 * pid_control_B.rtb_CoordinateTransformationC_o +
                  0.177, 1.2);

    /* Gain: '<S168>/Lv' */
    pid_control_B.sinc = pid_control_ConstB.UnitConversion_c;

    /* Outputs for Enabled SubSystem: '<S162>/Hvgw(s)' incorporates:
     *  EnablePort: '<S176>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hvgws_MODE) {
      (void) memset(&(pid_control_XDis.vg_p1_CSTATE), 0,
                    4*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S176>/vg_p1' */
      pid_control_X.vg_p1_CSTATE[0] = 0.0;

      /* InitializeConditions for Integrator: '<S176>/vgw_p2' */
      pid_control_X.vgw_p2_CSTATE[0] = 0.0;

      /* InitializeConditions for Integrator: '<S176>/vg_p1' */
      pid_control_X.vg_p1_CSTATE[1] = 0.0;

      /* InitializeConditions for Integrator: '<S176>/vgw_p2' */
      pid_control_X.vgw_p2_CSTATE[1] = 0.0;
      pid_control_DW.Hvgws_MODE = true;
    }

    if (pid_control_DW.Hvgws_MODE) {
      /* Product: '<S176>/Lvg//V' incorporates:
       *  Gain: '<S168>/Lv'
       */
      pid_control_B.px_out_l = pid_control_B.cosc / pid_control_B.cosa;

      /* Product: '<S176>/w' incorporates:
       *  Gain: '<S176>/(1//pi)'
       *  Integrator: '<S176>/vg_p1'
       *  Product: '<S176>/Lug//V1'
       *  Sqrt: '<S176>/sqrt'
       *  Sum: '<S176>/Sum'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = (sqrt(0.3183098861837907 *
        pid_control_B.px_out_l) * pid_control_B.Product[1] -
        pid_control_X.vg_p1_CSTATE[0]) / pid_control_B.px_out_l;
      pid_control_B.w_g[0] = pid_control_B.rtb_CoordinateTransformationC_o;

      /* Product: '<S176>/w ' incorporates:
       *  Gain: '<S176>/sqrt(3)'
       *  Integrator: '<S176>/vg_p1'
       *  Integrator: '<S176>/vgw_p2'
       *  Product: '<S176>/Lvg//V '
       *  Sum: '<S176>/Sum1'
       */
      pid_control_B.w_e[0] = (pid_control_B.rtb_CoordinateTransformationC_o *
        pid_control_B.px_out_l * 1.7320508075688772 +
        (pid_control_X.vg_p1_CSTATE[0] - pid_control_X.vgw_p2_CSTATE[0])) /
        pid_control_B.px_out_l;

      /* Product: '<S176>/Lvg//V' */
      pid_control_B.px_out_l = pid_control_B.sinc / pid_control_B.cosa;

      /* Product: '<S176>/w' incorporates:
       *  Gain: '<S176>/(1//pi)'
       *  Integrator: '<S176>/vg_p1'
       *  Product: '<S176>/Lug//V1'
       *  Sqrt: '<S176>/sqrt'
       *  Sum: '<S176>/Sum'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = (sqrt(0.3183098861837907 *
        pid_control_B.px_out_l) * pid_control_B.Product[1] -
        pid_control_X.vg_p1_CSTATE[1]) / pid_control_B.px_out_l;
      pid_control_B.w_g[1] = pid_control_B.rtb_CoordinateTransformationC_o;

      /* Product: '<S176>/w ' incorporates:
       *  Gain: '<S176>/sqrt(3)'
       *  Integrator: '<S176>/vg_p1'
       *  Integrator: '<S176>/vgw_p2'
       *  Product: '<S176>/Lvg//V '
       *  Sum: '<S176>/Sum1'
       */
      pid_control_B.w_e[1] = (pid_control_B.rtb_CoordinateTransformationC_o *
        pid_control_B.px_out_l * 1.7320508075688772 +
        (pid_control_X.vg_p1_CSTATE[1] - pid_control_X.vgw_p2_CSTATE[1])) /
        pid_control_B.px_out_l;

      /* Product: '<S176>/w 1' incorporates:
       *  Integrator: '<S176>/vgw_p2'
       */
      _mm_storeu_pd(&pid_control_B.w1[0], _mm_mul_pd(_mm_set_pd
        (pid_control_B.sinb, pid_control_B.cosb), _mm_loadu_pd
        (&pid_control_X.vgw_p2_CSTATE[0])));
    }

    /* End of Outputs for SubSystem: '<S162>/Hvgw(s)' */

    /* Outputs for Enabled SubSystem: '<S161>/Hrgw' incorporates:
     *  EnablePort: '<S174>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hrgw_MODE) {
      (void) memset(&(pid_control_XDis.rgw_p_CSTATE), 0,
                    2*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S174>/rgw_p' */
      pid_control_X.rgw_p_CSTATE[0] = 0.0;
      pid_control_X.rgw_p_CSTATE[1] = 0.0;
      pid_control_DW.Hrgw_MODE = true;
    }

    if (pid_control_DW.Hrgw_MODE) {
      /* Product: '<S174>/w' incorporates:
       *  Gain: '<S174>/pi//3'
       *  Integrator: '<S174>/rgw_p'
       *  Product: '<S174>/vg//V'
       */
      _mm_storeu_pd(&pid_control_B.w_d[0], _mm_mul_pd(_mm_sub_pd(_mm_div_pd
        (_mm_loadu_pd(&pid_control_B.w1[0]), _mm_set1_pd(pid_control_B.cosa)),
        _mm_loadu_pd(&pid_control_X.rgw_p_CSTATE[0])), _mm_div_pd(_mm_set1_pd
        (1.0471975511965976 * pid_control_B.cosa), _mm_set1_pd
        (pid_control_ConstB.UnitConversion_n))));
    }

    /* End of Outputs for SubSystem: '<S161>/Hrgw' */

    /* Angle2Dcm: '<S34>/Rotation Angles to Direction Cosine Matrix' */
    pid_control_B.rtb_CoordinateTransformationC_o = cos(pid_control_B.x[6]);
    pid_control_B.sinc = sin(pid_control_B.x[6]);
    pid_control_B.px_out_l = -sin(pid_control_B.x[6]);
    pid_control_B.rtb_CoordinateTransformationC_b = cos(pid_control_B.x[6]);
    pid_control_B.FA_b_idx_0 = cos(pid_control_B.x[7]);
    pid_control_B.u2 = -sin(pid_control_B.x[7]);
    pid_control_B.SignPreSat = sin(pid_control_B.x[7]);
    pid_control_B.IntegratorSecondOrderLimite = cos(pid_control_B.x[7]);
    pid_control_B.Square1 = cos(pid_control_B.x[8]);
    pid_control_B.Sum1 = sin(pid_control_B.x[8]);
    pid_control_B.u2_deg = -sin(pid_control_B.x[8]);
    pid_control_B.FA_b_idx_1 = cos(pid_control_B.x[8]);
    pid_control_B.Ltot = 0.0 * pid_control_B.SignPreSat +
      pid_control_B.FA_b_idx_0;
    pid_control_B.CQ = 0.0 * pid_control_B.IntegratorSecondOrderLimite +
      pid_control_B.u2;
    pid_control_B.FA_b_idx_2 = pid_control_B.Square1 * 0.0;
    pid_control_B.Cl = 0.0 * pid_control_B.FA_b_idx_0;
    pid_control_B.FA_b_idx_0 = (pid_control_B.Cl + pid_control_B.FA_b_idx_2) +
      pid_control_B.Sum1 * pid_control_B.SignPreSat;
    pid_control_B.Square1 += pid_control_B.Sum1 * 0.0;
    pid_control_B.u2 *= 0.0;
    pid_control_B.Sum1 = (pid_control_B.u2 + pid_control_B.FA_b_idx_2) +
      pid_control_B.Sum1 * pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.FA_b_idx_2 = pid_control_B.u2_deg * 0.0;
    pid_control_B.SignPreSat = (pid_control_B.Cl + pid_control_B.FA_b_idx_2) +
      pid_control_B.SignPreSat * pid_control_B.FA_b_idx_1;
    pid_control_B.u2_deg += pid_control_B.FA_b_idx_1 * 0.0;
    pid_control_B.IntegratorSecondOrderLimite = (pid_control_B.u2 +
      pid_control_B.FA_b_idx_2) + pid_control_B.FA_b_idx_1 *
      pid_control_B.IntegratorSecondOrderLimite;
    pid_control_B.FA_b_idx_1 = pid_control_B.CQ * 0.0;
    pid_control_B.RotationAnglestoDirectionCo[0] = (pid_control_B.Ltot *
      pid_control_B.rtb_CoordinateTransformationC_o + 0.0 *
      pid_control_B.px_out_l) + pid_control_B.FA_b_idx_1;
    pid_control_B.FA_b_idx_2 = pid_control_B.Sum1 * 0.0;
    pid_control_B.RotationAnglestoDirectionCo[1] =
      (pid_control_B.rtb_CoordinateTransformationC_o * pid_control_B.FA_b_idx_0
       + pid_control_B.px_out_l * pid_control_B.Square1) +
      pid_control_B.FA_b_idx_2;
    pid_control_B.Cl = pid_control_B.IntegratorSecondOrderLimite * 0.0;
    pid_control_B.RotationAnglestoDirectionCo[2] =
      (pid_control_B.rtb_CoordinateTransformationC_o * pid_control_B.SignPreSat
       + pid_control_B.px_out_l * pid_control_B.u2_deg) + pid_control_B.Cl;
    pid_control_B.RotationAnglestoDirectionCo[3] = (pid_control_B.Ltot *
      pid_control_B.sinc + 0.0 * pid_control_B.rtb_CoordinateTransformationC_b)
      + pid_control_B.FA_b_idx_1;
    pid_control_B.RotationAnglestoDirectionCo[4] = (pid_control_B.sinc *
      pid_control_B.FA_b_idx_0 + pid_control_B.Square1 *
      pid_control_B.rtb_CoordinateTransformationC_b) + pid_control_B.FA_b_idx_2;
    pid_control_B.RotationAnglestoDirectionCo[5] = (pid_control_B.sinc *
      pid_control_B.SignPreSat + pid_control_B.rtb_CoordinateTransformationC_b *
      pid_control_B.u2_deg) + pid_control_B.Cl;
    pid_control_B.RotationAnglestoDirectionCo[6] = pid_control_B.Ltot * 0.0 +
      pid_control_B.CQ;
    pid_control_B.RotationAnglestoDirectionCo[7] = (pid_control_B.FA_b_idx_0 *
      0.0 + pid_control_B.Square1 * 0.0) + pid_control_B.Sum1;
    pid_control_B.RotationAnglestoDirectionCo[8] = (pid_control_B.SignPreSat *
      0.0 + pid_control_B.u2_deg * 0.0) +
      pid_control_B.IntegratorSecondOrderLimite;

    /* If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
     *  Constant: '<S180>/max_height_low'
     *  Product: '<S180>/Product1'
     *  Product: '<S185>/Product1'
     *  Product: '<S185>/Product2'
     *  Product: '<S187>/Product1'
     *  Product: '<S187>/Product2'
     *  Sum: '<S180>/Sum1'
     *  Sum: '<S180>/Sum2'
     *  Sum: '<S180>/Sum3'
     *  Sum: '<S185>/Sum'
     *  Sum: '<S187>/Sum'
     */
    rtPrevAction = pid_control_DW.ifHeightMaxlowaltitudeelseifHei;
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      if (pid_control_B.sina <= 1000.0) {
        rtAction = 0;
      } else if (pid_control_B.sina >= 2000.0) {
        rtAction = 1;
      } else {
        rtAction = 2;
      }

      pid_control_DW.ifHeightMaxlowaltitudeelseifHei = rtAction;
    } else {
      rtAction = pid_control_DW.ifHeightMaxlowaltitudeelseifHei;
    }

    if (rtPrevAction != rtAction) {
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    switch (rtAction) {
     case 0:
      /* Outputs for IfAction SubSystem: '<S166>/Low altitude  rates' incorporates:
       *  ActionPort: '<S181>/Action Port'
       */
      /* SignalConversion generated from: '<S186>/Vector Concatenate' */
      pid_control_B.Reshape1_b[2] = pid_control_B.w_d[0];

      /* Trigonometry: '<S187>/Trigonometric Function1' */
      pid_control_B.rtb_CoordinateTransformationC_o = sin
        (pid_control_ConstB.UnitConversion);
      pid_control_B.sinc = cos(pid_control_ConstB.UnitConversion);
      _mm_storeu_pd(&pid_control_B.Reshape1_b[0], _mm_add_pd(_mm_mul_pd
        (_mm_set_pd(pid_control_B.rtb_CoordinateTransformationC_o,
                    pid_control_B.sigma_w[0]), _mm_set_pd(pid_control_B.sigma_w
        [0], pid_control_B.sinc)), _mm_mul_pd(_mm_mul_pd(_mm_set_pd
        (pid_control_B.UnaryMinus[0],
         pid_control_B.rtb_CoordinateTransformationC_o), _mm_set_pd
        (pid_control_B.sinc, pid_control_B.UnaryMinus[0])), _mm_set_pd(1.0, -1.0))));

      /* Product: '<S186>/Product' incorporates:
       *  Angle2Dcm: '<S34>/Rotation Angles to Direction Cosine Matrix'
       *  Product: '<S187>/Product1'
       *  Product: '<S187>/Product2'
       *  Reshape: '<S186>/Reshape'
       *  Reshape: '<S186>/Reshape1'
       *  Sum: '<S187>/Sum'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = 0.0;
      pid_control_B.sinc = 0.0;
      pid_control_B.px_out_l = 0.0;
      for (i = 0; i < 3; i++) {
        _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(
          &pid_control_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
          (pid_control_B.Reshape1_b[i])), _mm_set_pd(pid_control_B.sinc,
          pid_control_B.rtb_CoordinateTransformationC_o)));
        pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.dv1[0];
        pid_control_B.sinc = pid_control_B.dv1[1];
        pid_control_B.px_out_l += pid_control_B.RotationAnglestoDirectionCo[3 *
          i + 2] * pid_control_B.Reshape1_b[i];
      }

      pid_control_B.wbe_b[2] = pid_control_B.px_out_l;
      pid_control_B.wbe_b[1] = pid_control_B.sinc;
      pid_control_B.wbe_b[0] = pid_control_B.rtb_CoordinateTransformationC_o;

      /* End of Product: '<S186>/Product' */
      /* End of Outputs for SubSystem: '<S166>/Low altitude  rates' */
      break;

     case 1:
      /* Outputs for IfAction SubSystem: '<S166>/Medium//High  altitude rates' incorporates:
       *  ActionPort: '<S182>/Action Port'
       */
      /* Gain: '<S182>/Gain' */
      pid_control_B.wbe_b[0] = pid_control_B.sigma_w[1];
      pid_control_B.wbe_b[1] = pid_control_B.UnaryMinus[1];
      pid_control_B.wbe_b[2] = pid_control_B.w_d[1];

      /* End of Outputs for SubSystem: '<S166>/Medium//High  altitude rates' */
      break;

     default:
      /* Outputs for IfAction SubSystem: '<S166>/Interpolate  rates' incorporates:
       *  ActionPort: '<S180>/Action Port'
       */
      /* Trigonometry: '<S185>/Trigonometric Function' */
      pid_control_B.rtb_CoordinateTransformationC_o = sin
        (pid_control_ConstB.UnitConversion);
      pid_control_B.sinc = cos(pid_control_ConstB.UnitConversion);
      _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
        (pid_control_B.rtb_CoordinateTransformationC_o, pid_control_B.sigma_w[0]),
        _mm_set_pd(pid_control_B.sigma_w[0], pid_control_B.sinc)), _mm_mul_pd
        (_mm_mul_pd(_mm_set_pd(pid_control_B.UnaryMinus[0],
        pid_control_B.rtb_CoordinateTransformationC_o), _mm_set_pd
                    (pid_control_B.sinc, pid_control_B.UnaryMinus[0])),
         _mm_set_pd(1.0, -1.0))));

      /* SignalConversion generated from: '<S184>/Vector Concatenate' incorporates:
       *  Product: '<S185>/Product1'
       *  Product: '<S185>/Product2'
       *  Sum: '<S185>/Sum'
       */
      pid_control_B.wbe_b[2] = pid_control_B.w_d[0];

      /* Product: '<S184>/Product' incorporates:
       *  Angle2Dcm: '<S34>/Rotation Angles to Direction Cosine Matrix'
       *  Reshape: '<S184>/Reshape'
       *  Reshape: '<S184>/Reshape1'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = 0.0;
      pid_control_B.sinc = 0.0;
      pid_control_B.px_out_l = 0.0;
      for (i = 0; i < 3; i++) {
        _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(
          &pid_control_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
          (pid_control_B.wbe_b[i])), _mm_set_pd(pid_control_B.sinc,
          pid_control_B.rtb_CoordinateTransformationC_o)));
        pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.dv1[0];
        pid_control_B.sinc = pid_control_B.dv1[1];
        pid_control_B.px_out_l += pid_control_B.RotationAnglestoDirectionCo[3 *
          i + 2] * pid_control_B.wbe_b[i];
      }

      pid_control_B.Reshape1_b[2] = pid_control_B.px_out_l;
      pid_control_B.Reshape1_b[1] = pid_control_B.sinc;
      pid_control_B.Reshape1_b[0] =
        pid_control_B.rtb_CoordinateTransformationC_o;
      _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_add_pd(_mm_div_pd(_mm_mul_pd
        (_mm_sub_pd(_mm_set_pd(pid_control_B.UnaryMinus[1],
        pid_control_B.sigma_w[1]), _mm_loadu_pd(&pid_control_B.Reshape1_b[0])),
         _mm_sub_pd(_mm_set1_pd(pid_control_B.sina), _mm_set1_pd(1000.0))),
        _mm_set1_pd(pid_control_ConstB.Sum_a)), _mm_loadu_pd
        (&pid_control_B.Reshape1_b[0])));

      /* Sum: '<S180>/Sum3' incorporates:
       *  Constant: '<S180>/max_height_low'
       *  Product: '<S180>/Product1'
       *  Product: '<S184>/Product'
       *  Reshape: '<S184>/Reshape1'
       *  Sum: '<S180>/Sum1'
       *  Sum: '<S180>/Sum2'
       */
      pid_control_B.wbe_b[2] = (pid_control_B.w_d[1] - pid_control_B.px_out_l) *
        (pid_control_B.sina - 1000.0) / pid_control_ConstB.Sum_a +
        pid_control_B.px_out_l;

      /* End of Outputs for SubSystem: '<S166>/Interpolate  rates' */
      break;
    }

    /* End of If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    if (tmp_0) {
      /* MATLABSystem: '<S156>/SourceBlock' */
      pid_control_B.SourceBlock_o1_k = Sub_pid_control_423.getLatestMessage
        (&rtb_SourceBlock_o2_dd);

      /* Outputs for Enabled SubSystem: '<S156>/Enabled Subsystem' */
      pid_control_EnabledSubsystem(pid_control_B.SourceBlock_o1_k,
        &rtb_SourceBlock_o2_dd, &pid_control_B.EnabledSubsystem);

      /* End of Outputs for SubSystem: '<S156>/Enabled Subsystem' */

      /* Gain: '<S151>/Output' incorporates:
       *  RandomNumber: '<S151>/White Noise'
       */
      pid_control_B.Output = 10.0 * pid_control_DW.NextOutput_k;
    }

    /* Switch: '<S34>/Switch1' incorporates:
     *  Constant: '<S34>/Constant2'
     */
    if (!pid_control_B.EnabledSubsystem.In1.data) {
      pid_control_B.wbe_b[0] = 0.0;
      pid_control_B.wbe_b[1] = 0.0;
      pid_control_B.wbe_b[2] = 0.0;
    }

    /* End of Switch: '<S34>/Switch1' */

    /* TransferFcn: '<S34>/Transfer Fcn1' */
    pid_control_B.rtb_CoordinateTransformationC_o = -0.0003571 *
      pid_control_X.TransferFcn1_CSTATE + 0.03571 * pid_control_B.Output;
    if (tmp_0) {
      /* MATLABSystem: '<S158>/SourceBlock' */
      pid_control_B.SourceBlock_o1_d = Sub_pid_control_445.getLatestMessage
        (&rtb_SourceBlock_o2_k);

      /* Outputs for Enabled SubSystem: '<S158>/Enabled Subsystem' */
      pid_control_EnabledSubsystem_k(pid_control_B.SourceBlock_o1_d,
        &rtb_SourceBlock_o2_k, &pid_control_B.EnabledSubsystem_p);

      /* End of Outputs for SubSystem: '<S158>/Enabled Subsystem' */
    }

    /* Switch: '<S34>/Switch3' incorporates:
     *  Constant: '<S34>/Constant4'
     */
    if (!(pid_control_B.EnabledSubsystem_p.In1.data != 0.0)) {
      pid_control_B.rtb_CoordinateTransformationC_o = 0.0;
    }

    /* End of Switch: '<S34>/Switch3' */

    /* Sum: '<S34>/SumOLA1' */
    pid_control_B.SumOLA1[0] = pid_control_B.wbe_b[0];
    pid_control_B.SumOLA1[1] = pid_control_B.wbe_b[1] +
      pid_control_B.rtb_CoordinateTransformationC_o;
    pid_control_B.SumOLA1[2] = pid_control_B.wbe_b[2];

    /* Outputs for Enabled SubSystem: '<S162>/Hugw(s)' incorporates:
     *  EnablePort: '<S175>/Enable'
     */
    if (tmp_0 && rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo) &&
        !pid_control_DW.Hugws_MODE) {
      (void) memset(&(pid_control_XDis.ug_p_CSTATE), 0,
                    2*sizeof(boolean_T));

      /* InitializeConditions for Integrator: '<S175>/ug_p' */
      pid_control_X.ug_p_CSTATE[0] = 0.0;
      pid_control_X.ug_p_CSTATE[1] = 0.0;
      pid_control_DW.Hugws_MODE = true;
    }

    if (pid_control_DW.Hugws_MODE) {
      /* Product: '<S175>/Lug//V' */
      pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.cosc /
        pid_control_B.cosa;
      pid_control_B.sinc = pid_control_ConstB.UnitConversion_c /
        pid_control_B.cosa;

      /* Product: '<S175>/w' incorporates:
       *  Gain: '<S175>/(2//pi)'
       *  Integrator: '<S175>/ug_p'
       *  Product: '<S175>/Lug//V1'
       *  Sqrt: '<S175>/sqrt'
       */
      _mm_storeu_pd(&pid_control_B.w_n[0], _mm_div_pd(_mm_sub_pd(_mm_mul_pd
        (_mm_set_pd(sqrt(0.6366197723675814 * pid_control_B.sinc), sqrt
                    (0.6366197723675814 *
                     pid_control_B.rtb_CoordinateTransformationC_o)),
         _mm_set1_pd(pid_control_B.Product[0])), _mm_loadu_pd
        (&pid_control_X.ug_p_CSTATE[0])), _mm_set_pd(pid_control_B.sinc,
        pid_control_B.rtb_CoordinateTransformationC_o)));

      /* Product: '<S175>/w1' incorporates:
       *  Integrator: '<S175>/ug_p'
       */
      _mm_storeu_pd(&pid_control_B.w1_c[0], _mm_mul_pd(_mm_loadu_pd
        (&pid_control_X.ug_p_CSTATE[0]), _mm_set_pd(pid_control_B.sinb,
        pid_control_B.cosb)));
    }

    /* End of Outputs for SubSystem: '<S162>/Hugw(s)' */

    /* If: '<S167>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' incorporates:
     *  Constant: '<S188>/max_height_low'
     *  Product: '<S188>/Product1'
     *  Product: '<S193>/Product1'
     *  Product: '<S193>/Product2'
     *  Product: '<S195>/Product1'
     *  Product: '<S195>/Product2'
     *  Sum: '<S188>/Sum1'
     *  Sum: '<S188>/Sum2'
     *  Sum: '<S188>/Sum3'
     *  Sum: '<S193>/Sum'
     *  Sum: '<S195>/Sum'
     */
    rtPrevAction = pid_control_DW.ifHeightMaxlowaltitudeelseifH_k;
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      if (pid_control_B.sina <= 1000.0) {
        rtAction = 0;
      } else if (pid_control_B.sina >= 2000.0) {
        rtAction = 1;
      } else {
        rtAction = 2;
      }

      pid_control_DW.ifHeightMaxlowaltitudeelseifH_k = rtAction;
    } else {
      rtAction = pid_control_DW.ifHeightMaxlowaltitudeelseifH_k;
    }

    if (rtPrevAction != rtAction) {
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    switch (rtAction) {
     case 0:
      /* Outputs for IfAction SubSystem: '<S167>/Low altitude  velocities' incorporates:
       *  ActionPort: '<S189>/Action Port'
       */
      /* SignalConversion generated from: '<S194>/Vector Concatenate' */
      pid_control_B.Reshape1_b[2] = pid_control_B.LwgV1[0];

      /* Trigonometry: '<S195>/Trigonometric Function' */
      pid_control_B.sina = sin(pid_control_ConstB.UnitConversion);
      pid_control_B.sinb = cos(pid_control_ConstB.UnitConversion);
      _mm_storeu_pd(&pid_control_B.Reshape1_b[0], _mm_add_pd(_mm_mul_pd
        (_mm_set_pd(pid_control_B.sina, pid_control_B.w1_c[0]), _mm_set_pd
         (pid_control_B.w1_c[0], pid_control_B.sinb)), _mm_mul_pd(_mm_mul_pd
        (_mm_set_pd(pid_control_B.w1[0], pid_control_B.sina), _mm_set_pd
         (pid_control_B.sinb, pid_control_B.w1[0])), _mm_set_pd(1.0, -1.0))));

      /* Product: '<S194>/Product' incorporates:
       *  Angle2Dcm: '<S34>/Rotation Angles to Direction Cosine Matrix'
       *  Product: '<S195>/Product1'
       *  Product: '<S195>/Product2'
       *  Reshape: '<S194>/Reshape'
       *  Reshape: '<S194>/Reshape1'
       *  Sum: '<S195>/Sum'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = 0.0;
      pid_control_B.sinc = 0.0;
      pid_control_B.px_out_l = 0.0;
      for (i = 0; i < 3; i++) {
        _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(
          &pid_control_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
          (pid_control_B.Reshape1_b[i])), _mm_set_pd(pid_control_B.sinc,
          pid_control_B.rtb_CoordinateTransformationC_o)));
        pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.dv1[0];
        pid_control_B.sinc = pid_control_B.dv1[1];
        pid_control_B.px_out_l += pid_control_B.RotationAnglestoDirectionCo[3 *
          i + 2] * pid_control_B.Reshape1_b[i];
      }

      pid_control_B.wbe_b[2] = pid_control_B.px_out_l;
      pid_control_B.wbe_b[1] = pid_control_B.sinc;
      pid_control_B.wbe_b[0] = pid_control_B.rtb_CoordinateTransformationC_o;

      /* End of Product: '<S194>/Product' */
      /* End of Outputs for SubSystem: '<S167>/Low altitude  velocities' */
      break;

     case 1:
      /* Outputs for IfAction SubSystem: '<S167>/Medium//High  altitude velocities' incorporates:
       *  ActionPort: '<S190>/Action Port'
       */
      /* Gain: '<S190>/Gain' */
      pid_control_B.wbe_b[0] = pid_control_B.w1_c[1];
      pid_control_B.wbe_b[1] = pid_control_B.w1[1];
      pid_control_B.wbe_b[2] = pid_control_B.LwgV1[1];

      /* End of Outputs for SubSystem: '<S167>/Medium//High  altitude velocities' */
      break;

     default:
      /* Outputs for IfAction SubSystem: '<S167>/Interpolate  velocities' incorporates:
       *  ActionPort: '<S188>/Action Port'
       */
      /* Trigonometry: '<S193>/Trigonometric Function' */
      pid_control_B.sinb = sin(pid_control_ConstB.UnitConversion);
      pid_control_B.rtb_CoordinateTransformationC_o = cos
        (pid_control_ConstB.UnitConversion);
      _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_add_pd(_mm_mul_pd(_mm_set_pd
        (pid_control_B.sinb, pid_control_B.w1_c[0]), _mm_set_pd
        (pid_control_B.w1_c[0], pid_control_B.rtb_CoordinateTransformationC_o)),
        _mm_mul_pd(_mm_mul_pd(_mm_set_pd(pid_control_B.w1[0], pid_control_B.sinb),
        _mm_set_pd(pid_control_B.rtb_CoordinateTransformationC_o,
                   pid_control_B.w1[0])), _mm_set_pd(1.0, -1.0))));

      /* SignalConversion generated from: '<S192>/Vector Concatenate' incorporates:
       *  Product: '<S193>/Product1'
       *  Product: '<S193>/Product2'
       *  Sum: '<S193>/Sum'
       */
      pid_control_B.wbe_b[2] = pid_control_B.LwgV1[0];

      /* Product: '<S192>/Product' incorporates:
       *  Angle2Dcm: '<S34>/Rotation Angles to Direction Cosine Matrix'
       *  Reshape: '<S192>/Reshape'
       *  Reshape: '<S192>/Reshape1'
       */
      pid_control_B.rtb_CoordinateTransformationC_o = 0.0;
      pid_control_B.sinc = 0.0;
      pid_control_B.px_out_l = 0.0;
      for (i = 0; i < 3; i++) {
        _mm_storeu_pd(&pid_control_B.dv1[0], _mm_add_pd(_mm_mul_pd(_mm_loadu_pd(
          &pid_control_B.RotationAnglestoDirectionCo[3 * i]), _mm_set1_pd
          (pid_control_B.wbe_b[i])), _mm_set_pd(pid_control_B.sinc,
          pid_control_B.rtb_CoordinateTransformationC_o)));
        pid_control_B.rtb_CoordinateTransformationC_o = pid_control_B.dv1[0];
        pid_control_B.sinc = pid_control_B.dv1[1];
        pid_control_B.px_out_l += pid_control_B.RotationAnglestoDirectionCo[3 *
          i + 2] * pid_control_B.wbe_b[i];
      }

      pid_control_B.Reshape1_b[2] = pid_control_B.px_out_l;
      pid_control_B.Reshape1_b[1] = pid_control_B.sinc;
      pid_control_B.Reshape1_b[0] =
        pid_control_B.rtb_CoordinateTransformationC_o;
      _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_add_pd(_mm_div_pd(_mm_mul_pd
        (_mm_sub_pd(_mm_set_pd(pid_control_B.w1[1], pid_control_B.w1_c[1]),
                    _mm_loadu_pd(&pid_control_B.Reshape1_b[0])), _mm_sub_pd
         (_mm_set1_pd(pid_control_B.sina), _mm_set1_pd(1000.0))), _mm_set1_pd
        (pid_control_ConstB.Sum)), _mm_loadu_pd(&pid_control_B.Reshape1_b[0])));

      /* Sum: '<S188>/Sum3' incorporates:
       *  Constant: '<S188>/max_height_low'
       *  Product: '<S188>/Product1'
       *  Product: '<S192>/Product'
       *  Reshape: '<S192>/Reshape1'
       *  Sum: '<S188>/Sum1'
       *  Sum: '<S188>/Sum2'
       */
      pid_control_B.wbe_b[2] = (pid_control_B.LwgV1[1] - pid_control_B.px_out_l)
        * (pid_control_B.sina - 1000.0) / pid_control_ConstB.Sum +
        pid_control_B.px_out_l;

      /* End of Outputs for SubSystem: '<S167>/Interpolate  velocities' */
      break;
    }

    /* End of If: '<S167>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    /* Unit Conversion - from: ft/s to: m/s
       Expression: output = (0.3048*input) + (0) */
    if (tmp_0) {
      /* MATLABSystem: '<S159>/SourceBlock' */
      pid_control_B.SourceBlock_o1_h = Sub_pid_control_417.getLatestMessage
        (&rtb_SourceBlock_o2_j);

      /* Outputs for Enabled SubSystem: '<S159>/Enabled Subsystem' */
      pid_control_EnabledSubsystem(pid_control_B.SourceBlock_o1_h,
        &rtb_SourceBlock_o2_j, &pid_control_B.EnabledSubsystem_pt);

      /* End of Outputs for SubSystem: '<S159>/Enabled Subsystem' */
    }

    /* Switch: '<S34>/Switch' incorporates:
     *  Constant: '<S34>/Constant'
     *  UnitConversion: '<S153>/Unit Conversion'
     */
    if (pid_control_B.EnabledSubsystem_pt.In1.data) {
      _mm_storeu_pd(&pid_control_B.wbe_b[0], _mm_mul_pd(_mm_set1_pd(0.3048),
        _mm_loadu_pd(&pid_control_B.wbe_b[0])));
      pid_control_B.wbe_b[2] *= 0.3048;
    } else {
      pid_control_B.wbe_b[0] = 0.0;
      pid_control_B.wbe_b[1] = 0.0;
      pid_control_B.wbe_b[2] = 0.0;
    }

    /* End of Switch: '<S34>/Switch' */
    if (tmp_0) {
      /* MATLABSystem: '<S157>/SourceBlock' */
      pid_control_B.SourceBlock_o1_c = Sub_pid_control_443.getLatestMessage
        (&rtb_SourceBlock_o2_p);

      /* Outputs for Enabled SubSystem: '<S157>/Enabled Subsystem' */
      pid_control_EnabledSubsystem_k(pid_control_B.SourceBlock_o1_c,
        &rtb_SourceBlock_o2_p, &pid_control_B.EnabledSubsystem_k);

      /* End of Outputs for SubSystem: '<S157>/Enabled Subsystem' */
    }

    /* Sum: '<S34>/SumOLA' */
    pid_control_B.SumOLA[0] = pid_control_B.wbe_b[0];

    /* Switch: '<S34>/Switch2' incorporates:
     *  Constant: '<S34>/Constant3'
     *  TransferFcn: '<S34>/Transfer Fcn'
     */
    if (pid_control_B.EnabledSubsystem_k.In1.data != 0.0) {
      pid_control_B.cosb = 0.5303 * pid_control_X.TransferFcn_CSTATE[0] + 0.0 *
        pid_control_X.TransferFcn_CSTATE[1];
    } else {
      pid_control_B.cosb = 0.0;
    }

    /* Sum: '<S34>/SumOLA' incorporates:
     *  Switch: '<S34>/Switch2'
     */
    pid_control_B.SumOLA[1] = pid_control_B.wbe_b[1] + pid_control_B.cosb;
    pid_control_B.SumOLA[2] = pid_control_B.wbe_b[2];

    /* Product: '<S34>/Product2' incorporates:
     *  Math: '<S34>/Square'
     *  Math: '<S34>/Square1'
     *  Math: '<S34>/Square2'
     *  Sqrt: '<S34>/Sqrt'
     *  Sum: '<S34>/Sum2'
     */
    pid_control_B.Power = sqrt((pid_control_B.x[0] * pid_control_B.x[0] +
      pid_control_B.x[1] * pid_control_B.x[1]) + pid_control_B.x[2] *
      pid_control_B.x[2]) * pid_control_B.XDOT[34];

    /* Gain: '<S34>/Gain3' */
    pid_control_B.Gain3_f = 0.001 * pid_control_B.Power;

    /* Gain: '<S34>/Gain1' incorporates:
     *  Integrator: '<S34>/Integrator1'
     */
    pid_control_B.EnergykWh = 2.7777777777777776E-7 *
      pid_control_X.Integrator1_CSTATE;

    /* Product: '<S34>/Divide' incorporates:
     *  Constant: '<S34>/thrust efficiency Cp?'
     */
    pid_control_B.powerdemand = pid_control_B.Gain3_f / 0.57;

    /* Product: '<S34>/Divide1' */
    pid_control_B.loadtorque = pid_control_B.powerdemand /
      pid_control_ConstB.motorspeed;

    /* Sum: '<S232>/SumI4' incorporates:
     *  Gain: '<S232>/Kb'
     *  Gain: '<S237>/Integral Gain'
     *  Sum: '<S232>/SumI2'
     */
    pid_control_B.SumI4 = (pid_control_B.Saturation_m -
      pid_control_B.rtb_CoordinateTransformationC_l) * 0.2 + -0.04 *
      pid_control_B.rtb_CoordinateTransformationC_d;

    /* SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      switch (pid_control_DW.IntegratorSecondOrderLimited_MO) {
       case 0:
        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] <=
            -0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = -0.3490658503988659;
          if (pid_control_X.IntegratorSecondOrderLimited_CS[1] > 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited_MO = 1;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] >=
            0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = 0.2617993877991494;
          if (pid_control_X.IntegratorSecondOrderLimited_CS[1] < 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited_MO = 2;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited_CS[1] <=
            -8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = -8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited_MO = 3;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited_CS[1] >=
            8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited_MO = 4;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 1:
        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] >
            -0.3490658503988659) {
          if (pid_control_X.IntegratorSecondOrderLimited_CS[0] >=
              0.2617993877991494) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 2;
            pid_control_X.IntegratorSecondOrderLimited_CS[0] =
              0.2617993877991494;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = -0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
        }
        break;

       case 2:
        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] <
            0.2617993877991494) {
          if (pid_control_X.IntegratorSecondOrderLimited_CS[0] <=
              -0.3490658503988659) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 1;
            pid_control_X.IntegratorSecondOrderLimited_CS[0] =
              -0.3490658503988659;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = 0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
        }
        break;

       case 3:
        if (pid_control_X.IntegratorSecondOrderLimited_CS[1] >
            -8.726646259971647) {
          if (pid_control_X.IntegratorSecondOrderLimited_CS[1] >=
              8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 4;
            pid_control_X.IntegratorSecondOrderLimited_CS[1] = 8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = -8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] <=
            -0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = -0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited_MO = 1;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 4:
        if (pid_control_X.IntegratorSecondOrderLimited_CS[1] < 8.726646259971647)
        {
          if (pid_control_X.IntegratorSecondOrderLimited_CS[1] <=
              -8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 3;
            pid_control_X.IntegratorSecondOrderLimited_CS[1] =
              -8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited_MO = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited_CS[0] >=
            0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited_CS[0] = 0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited_MO = 2;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;
      }
    }

    if (pid_control_X.IntegratorSecondOrderLimited_CS[0] < -0.3490658503988659)
    {
      pid_control_B.cosb = -0.3490658503988659;
    } else if (pid_control_X.IntegratorSecondOrderLimited_CS[0] >
               0.2617993877991494) {
      pid_control_B.cosb = 0.2617993877991494;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited_CS[0];
    }

    /* Sum: '<S20>/Sum3' incorporates:
     *  SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited'
     */
    pid_control_B.rtb_CoordinateTransformationC_o =
      pid_control_ConstB.Demandlimits - pid_control_B.cosb;

    /* Saturate: '<S20>/rate limit*2*zeta//wn' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = 5.235987755982988;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o <
               -5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = -5.235987755982988;
    }

    /* SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */
    if (pid_control_X.IntegratorSecondOrderLimited_CS[1] < -8.726646259971647) {
      pid_control_B.cosb = -8.726646259971647;
    } else if (pid_control_X.IntegratorSecondOrderLimited_CS[1] >
               8.726646259971647) {
      pid_control_B.cosb = 8.726646259971647;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited_CS[1];
    }

    /* Sum: '<S20>/Sum2' incorporates:
     *  Gain: '<S20>/2*zeta * wn'
     *  Saturate: '<S20>/rate limit*2*zeta//wn'
     *  SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited'
     */
    pid_control_B.Sum2 = pid_control_B.rtb_CoordinateTransformationC_o - 0.6 *
      pid_control_B.cosb;

    /* SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      switch (pid_control_DW.IntegratorSecondOrderLimited__c) {
       case 0:
        if (pid_control_X.IntegratorSecondOrderLimited__k[0] <=
            -0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = -0.3490658503988659;
          if (pid_control_X.IntegratorSecondOrderLimited__k[1] > 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__c = 1;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__k[0] >=
            0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = 0.3490658503988659;
          if (pid_control_X.IntegratorSecondOrderLimited__k[1] < 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__c = 2;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__k[1] <=
            -8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__k[1] = -8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__c = 3;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__k[1] >=
            8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__c = 4;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 1:
        if (pid_control_X.IntegratorSecondOrderLimited__k[0] >
            -0.3490658503988659) {
          if (pid_control_X.IntegratorSecondOrderLimited__k[0] >=
              0.3490658503988659) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 2;
            pid_control_X.IntegratorSecondOrderLimited__k[0] =
              0.3490658503988659;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = -0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
        }
        break;

       case 2:
        if (pid_control_X.IntegratorSecondOrderLimited__k[0] <
            0.3490658503988659) {
          if (pid_control_X.IntegratorSecondOrderLimited__k[0] <=
              -0.3490658503988659) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 1;
            pid_control_X.IntegratorSecondOrderLimited__k[0] =
              -0.3490658503988659;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = 0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
        }
        break;

       case 3:
        if (pid_control_X.IntegratorSecondOrderLimited__k[1] >
            -8.726646259971647) {
          if (pid_control_X.IntegratorSecondOrderLimited__k[1] >=
              8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 4;
            pid_control_X.IntegratorSecondOrderLimited__k[1] = 8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__k[1] = -8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__k[0] <=
            -0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = -0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__c = 1;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 4:
        if (pid_control_X.IntegratorSecondOrderLimited__k[1] < 8.726646259971647)
        {
          if (pid_control_X.IntegratorSecondOrderLimited__k[1] <=
              -8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__c = 3;
            pid_control_X.IntegratorSecondOrderLimited__k[1] =
              -8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__c = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__k[0] >=
            0.3490658503988659) {
          pid_control_X.IntegratorSecondOrderLimited__k[0] = 0.3490658503988659;
          pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__c = 2;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;
      }
    }

    if (pid_control_X.IntegratorSecondOrderLimited__k[0] < -0.3490658503988659)
    {
      pid_control_B.cosb = -0.3490658503988659;
    } else if (pid_control_X.IntegratorSecondOrderLimited__k[0] >
               0.3490658503988659) {
      pid_control_B.cosb = 0.3490658503988659;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__k[0];
    }

    /* Sum: '<S21>/Sum3' incorporates:
     *  SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited'
     */
    pid_control_B.rtb_CoordinateTransformationC_o =
      pid_control_ConstB.Demandlimits_h - pid_control_B.cosb;

    /* Saturate: '<S21>/rate limit*2*zeta//wn' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = 5.235987755982988;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o <
               -5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = -5.235987755982988;
    }

    /* SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */
    if (pid_control_X.IntegratorSecondOrderLimited__k[1] < -8.726646259971647) {
      pid_control_B.cosb = -8.726646259971647;
    } else if (pid_control_X.IntegratorSecondOrderLimited__k[1] >
               8.726646259971647) {
      pid_control_B.cosb = 8.726646259971647;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__k[1];
    }

    /* Sum: '<S21>/Sum2' incorporates:
     *  Gain: '<S21>/2*zeta * wn'
     *  Saturate: '<S21>/rate limit*2*zeta//wn'
     *  SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited'
     */
    pid_control_B.Sum2_n = pid_control_B.rtb_CoordinateTransformationC_o - 0.6 *
      pid_control_B.cosb;

    /* SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      switch (pid_control_DW.IntegratorSecondOrderLimited__h) {
       case 0:
        if (pid_control_X.IntegratorSecondOrderLimited__b[0] <=
            -0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = -0.2617993877991494;
          if (pid_control_X.IntegratorSecondOrderLimited__b[1] > 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__h = 1;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__b[0] >=
            0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = 0.2617993877991494;
          if (pid_control_X.IntegratorSecondOrderLimited__b[1] < 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__h = 2;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__b[1] <=
            -8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__b[1] = -8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__h = 3;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__b[1] >=
            8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__h = 4;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 1:
        if (pid_control_X.IntegratorSecondOrderLimited__b[0] >
            -0.2617993877991494) {
          if (pid_control_X.IntegratorSecondOrderLimited__b[0] >=
              0.2617993877991494) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 2;
            pid_control_X.IntegratorSecondOrderLimited__b[0] =
              0.2617993877991494;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = -0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
        }
        break;

       case 2:
        if (pid_control_X.IntegratorSecondOrderLimited__b[0] <
            0.2617993877991494) {
          if (pid_control_X.IntegratorSecondOrderLimited__b[0] <=
              -0.2617993877991494) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 1;
            pid_control_X.IntegratorSecondOrderLimited__b[0] =
              -0.2617993877991494;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = 0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
        }
        break;

       case 3:
        if (pid_control_X.IntegratorSecondOrderLimited__b[1] >
            -8.726646259971647) {
          if (pid_control_X.IntegratorSecondOrderLimited__b[1] >=
              8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 4;
            pid_control_X.IntegratorSecondOrderLimited__b[1] = 8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__b[1] = -8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__b[0] <=
            -0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = -0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__h = 1;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 4:
        if (pid_control_X.IntegratorSecondOrderLimited__b[1] < 8.726646259971647)
        {
          if (pid_control_X.IntegratorSecondOrderLimited__b[1] <=
              -8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__h = 3;
            pid_control_X.IntegratorSecondOrderLimited__b[1] =
              -8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__h = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__b[0] >=
            0.2617993877991494) {
          pid_control_X.IntegratorSecondOrderLimited__b[0] = 0.2617993877991494;
          pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__h = 2;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;
      }
    }

    if (pid_control_X.IntegratorSecondOrderLimited__b[0] < -0.2617993877991494)
    {
      pid_control_B.cosb = -0.2617993877991494;
    } else if (pid_control_X.IntegratorSecondOrderLimited__b[0] >
               0.2617993877991494) {
      pid_control_B.cosb = 0.2617993877991494;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__b[0];
    }

    /* Sum: '<S22>/Sum3' incorporates:
     *  SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited'
     */
    pid_control_B.rtb_CoordinateTransformationC_o =
      pid_control_ConstB.Demandlimits_e - pid_control_B.cosb;

    /* Saturate: '<S22>/rate limit*2*zeta//wn' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = 5.235987755982988;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o <
               -5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = -5.235987755982988;
    }

    /* SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */
    if (pid_control_X.IntegratorSecondOrderLimited__b[1] < -8.726646259971647) {
      pid_control_B.cosb = -8.726646259971647;
    } else if (pid_control_X.IntegratorSecondOrderLimited__b[1] >
               8.726646259971647) {
      pid_control_B.cosb = 8.726646259971647;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__b[1];
    }

    /* Sum: '<S22>/Sum2' incorporates:
     *  Gain: '<S22>/2*zeta * wn'
     *  Saturate: '<S22>/rate limit*2*zeta//wn'
     *  SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited'
     */
    pid_control_B.Sum2_i = pid_control_B.rtb_CoordinateTransformationC_o - 0.6 *
      pid_control_B.cosb;

    /* SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      switch (pid_control_DW.IntegratorSecondOrderLimited__o) {
       case 0:
        if (pid_control_X.IntegratorSecondOrderLimited__l[0] <= 0.0) {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 0.0;
          if (pid_control_X.IntegratorSecondOrderLimited__l[1] > 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__o = 1;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__l[0] >= 100.0) {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 100.0;
          if (pid_control_X.IntegratorSecondOrderLimited__l[1] < 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__o = 2;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__l[1] <=
            -8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__l[1] = -8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__o = 3;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__l[1] >=
            8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__o = 4;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 1:
        if (pid_control_X.IntegratorSecondOrderLimited__l[0] > 0.0) {
          if (pid_control_X.IntegratorSecondOrderLimited__l[0] >= 100.0) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 2;
            pid_control_X.IntegratorSecondOrderLimited__l[0] = 100.0;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 0.0;
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
        }
        break;

       case 2:
        if (pid_control_X.IntegratorSecondOrderLimited__l[0] < 100.0) {
          if (pid_control_X.IntegratorSecondOrderLimited__l[0] <= 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 1;
            pid_control_X.IntegratorSecondOrderLimited__l[0] = 0.0;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 100.0;
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
        }
        break;

       case 3:
        if (pid_control_X.IntegratorSecondOrderLimited__l[1] >
            -8.726646259971647) {
          if (pid_control_X.IntegratorSecondOrderLimited__l[1] >=
              8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 4;
            pid_control_X.IntegratorSecondOrderLimited__l[1] = 8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__l[1] = -8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__l[0] <= 0.0) {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 0.0;
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__o = 1;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 4:
        if (pid_control_X.IntegratorSecondOrderLimited__l[1] < 8.726646259971647)
        {
          if (pid_control_X.IntegratorSecondOrderLimited__l[1] <=
              -8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__o = 3;
            pid_control_X.IntegratorSecondOrderLimited__l[1] =
              -8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__o = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__l[0] >= 100.0) {
          pid_control_X.IntegratorSecondOrderLimited__l[0] = 100.0;
          pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__o = 2;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;
      }
    }

    if (pid_control_X.IntegratorSecondOrderLimited__l[0] < 0.0) {
      pid_control_B.cosb = 0.0;
    } else if (pid_control_X.IntegratorSecondOrderLimited__l[0] > 100.0) {
      pid_control_B.cosb = 100.0;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__l[0];
    }

    /* Sum: '<S23>/Sum3' incorporates:
     *  SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited'
     */
    pid_control_B.rtb_CoordinateTransformationC_o =
      pid_control_ConstB.Demandlimits_g - pid_control_B.cosb;

    /* Saturate: '<S23>/rate limit*2*zeta//wn' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = 5.235987755982988;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o <
               -5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = -5.235987755982988;
    }

    /* SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */
    if (pid_control_X.IntegratorSecondOrderLimited__l[1] < -8.726646259971647) {
      pid_control_B.cosb = -8.726646259971647;
    } else if (pid_control_X.IntegratorSecondOrderLimited__l[1] >
               8.726646259971647) {
      pid_control_B.cosb = 8.726646259971647;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__l[1];
    }

    /* Sum: '<S23>/Sum2' incorporates:
     *  Gain: '<S23>/2*zeta * wn'
     *  Saturate: '<S23>/rate limit*2*zeta//wn'
     *  SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited'
     */
    pid_control_B.Sum2_l = pid_control_B.rtb_CoordinateTransformationC_o - 0.6 *
      pid_control_B.cosb;

    /* SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
    if (rtsiIsModeUpdateTimeStep(&(&pid_control_M)->solverInfo)) {
      switch (pid_control_DW.IntegratorSecondOrderLimited__j) {
       case 0:
        if (pid_control_X.IntegratorSecondOrderLimited__e[0] <= 0.0) {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 0.0;
          if (pid_control_X.IntegratorSecondOrderLimited__e[1] > 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__j = 1;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__e[0] >= 100.0) {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 100.0;
          if (pid_control_X.IntegratorSecondOrderLimited__e[1] < 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          } else {
            pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
            pid_control_DW.IntegratorSecondOrderLimited__j = 2;
          }

          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__e[1] <=
            -8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__e[1] = -8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__j = 3;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }

        if (pid_control_X.IntegratorSecondOrderLimited__e[1] >=
            8.726646259971647) {
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 8.726646259971647;
          pid_control_DW.IntegratorSecondOrderLimited__j = 4;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 1:
        if (pid_control_X.IntegratorSecondOrderLimited__e[0] > 0.0) {
          if (pid_control_X.IntegratorSecondOrderLimited__e[0] >= 100.0) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 2;
            pid_control_X.IntegratorSecondOrderLimited__e[0] = 100.0;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 0.0;
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
        }
        break;

       case 2:
        if (pid_control_X.IntegratorSecondOrderLimited__e[0] < 100.0) {
          if (pid_control_X.IntegratorSecondOrderLimited__e[0] <= 0.0) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 1;
            pid_control_X.IntegratorSecondOrderLimited__e[0] = 0.0;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 100.0;
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
        }
        break;

       case 3:
        if (pid_control_X.IntegratorSecondOrderLimited__e[1] >
            -8.726646259971647) {
          if (pid_control_X.IntegratorSecondOrderLimited__e[1] >=
              8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 4;
            pid_control_X.IntegratorSecondOrderLimited__e[1] = 8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__e[1] = -8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__e[0] <= 0.0) {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 0.0;
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__j = 1;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;

       case 4:
        if (pid_control_X.IntegratorSecondOrderLimited__e[1] < 8.726646259971647)
        {
          if (pid_control_X.IntegratorSecondOrderLimited__e[1] <=
              -8.726646259971647) {
            pid_control_DW.IntegratorSecondOrderLimited__j = 3;
            pid_control_X.IntegratorSecondOrderLimited__e[1] =
              -8.726646259971647;
          } else {
            pid_control_DW.IntegratorSecondOrderLimited__j = 0;
          }
        } else {
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 8.726646259971647;
        }

        if (pid_control_X.IntegratorSecondOrderLimited__e[0] >= 100.0) {
          pid_control_X.IntegratorSecondOrderLimited__e[0] = 100.0;
          pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;
          pid_control_DW.IntegratorSecondOrderLimited__j = 2;
          rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)
            ->solverInfo, true);
        }
        break;
      }
    }

    if (pid_control_X.IntegratorSecondOrderLimited__e[0] < 0.0) {
      pid_control_B.cosb = 0.0;
    } else if (pid_control_X.IntegratorSecondOrderLimited__e[0] > 100.0) {
      pid_control_B.cosb = 100.0;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__e[0];
    }

    /* Sum: '<S24>/Sum3' incorporates:
     *  SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited'
     */
    pid_control_B.rtb_CoordinateTransformationC_o =
      pid_control_ConstB.Demandlimits_n - pid_control_B.cosb;

    /* Saturate: '<S24>/rate limit*2*zeta//wn' */
    if (pid_control_B.rtb_CoordinateTransformationC_o > 5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = 5.235987755982988;
    } else if (pid_control_B.rtb_CoordinateTransformationC_o <
               -5.235987755982988) {
      pid_control_B.rtb_CoordinateTransformationC_o = -5.235987755982988;
    }

    /* SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
    if (pid_control_X.IntegratorSecondOrderLimited__e[1] < -8.726646259971647) {
      pid_control_B.cosb = -8.726646259971647;
    } else if (pid_control_X.IntegratorSecondOrderLimited__e[1] >
               8.726646259971647) {
      pid_control_B.cosb = 8.726646259971647;
    } else {
      pid_control_B.cosb = pid_control_X.IntegratorSecondOrderLimited__e[1];
    }

    /* Sum: '<S24>/Sum2' incorporates:
     *  Gain: '<S24>/2*zeta * wn'
     *  Saturate: '<S24>/rate limit*2*zeta//wn'
     *  SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited'
     */
    pid_control_B.Sum2_f = pid_control_B.rtb_CoordinateTransformationC_o - 0.6 *
      pid_control_B.cosb;

    /* S-Function (sfun_tstart): '<S31>/startTime' */
    pid_control_B.startTime = 0.0;
  }

  if (rtmIsMajorTimeStep((&pid_control_M))) {
    int32_T i;

    /* Update for Integrator: '<S34>/Integrator' */
    pid_control_DW.Integrator_DWORK1 = false;

    /* Update for RateLimiter: '<Root>/Rate Limiter-theta_sp' */
    pid_control_DW.PrevY_g = pid_control_B.RateLimitertheta_sp;
    pid_control_DW.LastMajorTime = (&pid_control_M)->Timing.t[0];
    if (rtmIsMajorTimeStep((&pid_control_M)) &&
        (&pid_control_M)->Timing.TaskCounters.TID[1] == 0) {
      /* Update for Memory: '<S34>/Memory2' incorporates:
       *  Integrator: '<S34>/Integrator'
       */
      memcpy(&pid_control_DW.Memory2_PreviousInput[0], &pid_control_B.x[0], 12U *
             sizeof(real_T));

      /* Update for UnitDelay: '<Root>/Unit Delay3' */
      pid_control_DW.UnitDelay3_DSTATE = pid_control_B.Switch3;

      /* Update for UnitDelay: '<Root>/Unit Delay2' */
      pid_control_DW.UnitDelay2_DSTATE = pid_control_B.Switch2;

      /* Update for Memory: '<S66>/Memory' */
      pid_control_DW.Memory_PreviousInput_o = pid_control_B.AND3;

      /* Update for Memory: '<S120>/Memory' */
      pid_control_DW.Memory_PreviousInput_a = pid_control_B.AND3_c;

      /* Update for RandomNumber: '<S171>/White Noise' */
      pid_control_DW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf
        (&pid_control_DW.RandSeed[0]);
      pid_control_DW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf
        (&pid_control_DW.RandSeed[1]);
      pid_control_DW.NextOutput[2] = rt_nrand_Upu32_Yd_f_pw_snf
        (&pid_control_DW.RandSeed[2]);
      pid_control_DW.NextOutput[3] = rt_nrand_Upu32_Yd_f_pw_snf
        (&pid_control_DW.RandSeed[3]);

      /* Update for Memory: '<S34>/Memory' incorporates:
       *  Sum: '<S34>/SumOLA'
       */
      pid_control_DW.Memory_PreviousInput[0] = pid_control_B.SumOLA[0];

      /* Update for Memory: '<S34>/Memory1' incorporates:
       *  Sum: '<S34>/SumOLA1'
       */
      pid_control_DW.Memory1_PreviousInput[0] = pid_control_B.SumOLA1[0];

      /* Update for Memory: '<S34>/Memory' incorporates:
       *  Sum: '<S34>/SumOLA'
       */
      pid_control_DW.Memory_PreviousInput[1] = pid_control_B.SumOLA[1];

      /* Update for Memory: '<S34>/Memory1' incorporates:
       *  Sum: '<S34>/SumOLA1'
       */
      pid_control_DW.Memory1_PreviousInput[1] = pid_control_B.SumOLA1[1];

      /* Update for Memory: '<S34>/Memory' incorporates:
       *  Sum: '<S34>/SumOLA'
       */
      pid_control_DW.Memory_PreviousInput[2] = pid_control_B.SumOLA[2];

      /* Update for Memory: '<S34>/Memory1' incorporates:
       *  Sum: '<S34>/SumOLA1'
       */
      pid_control_DW.Memory1_PreviousInput[2] = pid_control_B.SumOLA1[2];

      /* Update for RandomNumber: '<S151>/White Noise' */
      pid_control_DW.NextOutput_k = rt_nrand_Upu32_Yd_f_pw_snf
        (&pid_control_DW.RandSeed_a);
    }

    /* Update for SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */
    i = pid_control_DW.IntegratorSecondOrderLimited_MO;
    if ((((pid_control_DW.IntegratorSecondOrderLimited_MO == 1) ||
          (pid_control_DW.IntegratorSecondOrderLimited_MO == 3)) &&
         (pid_control_B.Sum2 > 0.0)) ||
        (((pid_control_DW.IntegratorSecondOrderLimited_MO == 2) ||
          (pid_control_DW.IntegratorSecondOrderLimited_MO == 4)) &&
         (pid_control_B.Sum2 < 0.0))) {
      i = 0;
    }

    if (pid_control_DW.IntegratorSecondOrderLimited_MO != i) {
      pid_control_DW.IntegratorSecondOrderLimited_MO = i;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    /* End of Update for SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */

    /* Update for SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */
    i = pid_control_DW.IntegratorSecondOrderLimited__c;
    if ((((pid_control_DW.IntegratorSecondOrderLimited__c == 1) ||
          (pid_control_DW.IntegratorSecondOrderLimited__c == 3)) &&
         (pid_control_B.Sum2_n > 0.0)) ||
        (((pid_control_DW.IntegratorSecondOrderLimited__c == 2) ||
          (pid_control_DW.IntegratorSecondOrderLimited__c == 4)) &&
         (pid_control_B.Sum2_n < 0.0))) {
      i = 0;
    }

    if (pid_control_DW.IntegratorSecondOrderLimited__c != i) {
      pid_control_DW.IntegratorSecondOrderLimited__c = i;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    /* End of Update for SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */

    /* Update for SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */
    i = pid_control_DW.IntegratorSecondOrderLimited__h;
    if ((((pid_control_DW.IntegratorSecondOrderLimited__h == 1) ||
          (pid_control_DW.IntegratorSecondOrderLimited__h == 3)) &&
         (pid_control_B.Sum2_i > 0.0)) ||
        (((pid_control_DW.IntegratorSecondOrderLimited__h == 2) ||
          (pid_control_DW.IntegratorSecondOrderLimited__h == 4)) &&
         (pid_control_B.Sum2_i < 0.0))) {
      i = 0;
    }

    if (pid_control_DW.IntegratorSecondOrderLimited__h != i) {
      pid_control_DW.IntegratorSecondOrderLimited__h = i;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    /* End of Update for SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */

    /* Update for SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */
    i = pid_control_DW.IntegratorSecondOrderLimited__o;
    if ((((pid_control_DW.IntegratorSecondOrderLimited__o == 1) ||
          (pid_control_DW.IntegratorSecondOrderLimited__o == 3)) &&
         (pid_control_B.Sum2_l > 0.0)) ||
        (((pid_control_DW.IntegratorSecondOrderLimited__o == 2) ||
          (pid_control_DW.IntegratorSecondOrderLimited__o == 4)) &&
         (pid_control_B.Sum2_l < 0.0))) {
      i = 0;
    }

    if (pid_control_DW.IntegratorSecondOrderLimited__o != i) {
      pid_control_DW.IntegratorSecondOrderLimited__o = i;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    /* End of Update for SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */

    /* Update for SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
    i = pid_control_DW.IntegratorSecondOrderLimited__j;
    if ((((pid_control_DW.IntegratorSecondOrderLimited__j == 1) ||
          (pid_control_DW.IntegratorSecondOrderLimited__j == 3)) &&
         (pid_control_B.Sum2_f > 0.0)) ||
        (((pid_control_DW.IntegratorSecondOrderLimited__j == 2) ||
          (pid_control_DW.IntegratorSecondOrderLimited__j == 4)) &&
         (pid_control_B.Sum2_f < 0.0))) {
      i = 0;
    }

    if (pid_control_DW.IntegratorSecondOrderLimited__j != i) {
      pid_control_DW.IntegratorSecondOrderLimited__j = i;
      rtsiSetBlockStateForSolverChangedAtMajorStep(&(&pid_control_M)->solverInfo,
        true);
    }

    /* End of Update for SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */

    /* ContTimeOutputInconsistentWithStateAtMajorOutputFlag is set, need to run a minor output */
    if (rtmIsMajorTimeStep((&pid_control_M))) {
      if (rtsiGetContTimeOutputInconsistentWithStateAtMajorStep(&(&pid_control_M)
           ->solverInfo)) {
        rtsiSetSimTimeStep(&(&pid_control_M)->solverInfo,MINOR_TIME_STEP);
        rtsiSetContTimeOutputInconsistentWithStateAtMajorStep(&(&pid_control_M
          )->solverInfo, false);
        pid_control::step();
        rtsiSetSimTimeStep(&(&pid_control_M)->solverInfo, MAJOR_TIME_STEP);
      }
    }
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep((&pid_control_M))) {
    rt_ertODEUpdateContinuousStates(&(&pid_control_M)->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if ((++(&pid_control_M)->Timing.clockTick0)==0) {
      ++(&pid_control_M)->Timing.clockTickH0;
    }

    (&pid_control_M)->Timing.t[0] = rtsiGetSolverStopTime(&(&pid_control_M)
      ->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      (&pid_control_M)->Timing.clockTick1++;
      if ((&pid_control_M)->Timing.clockTick1==0) {
        (&pid_control_M)->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void pid_control::pid_control_derivatives()
{
  XDot_pid_control_T *_rtXdot;
  real_T tmp[2];
  _rtXdot = ((XDot_pid_control_T *) (&pid_control_M)->derivs);

  /* Derivatives for Integrator: '<S34>/Integrator' */
  if (!pid_control_B.Compare) {
    memcpy(&_rtXdot->Integrator_CSTATE[0], &pid_control_B.XDOT[0], 12U * sizeof
           (real_T));
  } else {
    /* level reset is active */
    memset(&_rtXdot->Integrator_CSTATE[0], 0, 12U * sizeof(real_T));
  }

  /* End of Derivatives for Integrator: '<S34>/Integrator' */

  /* Derivatives for Integrator: '<S130>/Integrator' */
  _rtXdot->Integrator_CSTATE_f = pid_control_B.Switch_j;

  /* Derivatives for Integrator: '<S125>/Filter' */
  _rtXdot->Filter_CSTATE = pid_control_B.FilterCoefficient;

  /* Derivatives for Enabled SubSystem: '<Root>/Subsystem2' */
  if (pid_control_DW.Subsystem2_MODE) {
    /* Derivatives for Integrator: '<S346>/Integrator' */
    _rtXdot->Integrator_CSTATE_n = pid_control_B.SumI4_i;

    /* Derivatives for Integrator: '<S341>/Filter' */
    _rtXdot->Filter_CSTATE_fb = pid_control_B.FilterCoefficient_j;
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)
             ->Integrator_CSTATE_n);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<Root>/Subsystem2' */

  /* Derivatives for Integrator: '<S240>/Integrator' */
  _rtXdot->Integrator_CSTATE_d = pid_control_B.SumI4;

  /* Derivatives for Integrator: '<S235>/Filter' */
  _rtXdot->Filter_CSTATE_f = pid_control_B.FilterCoefficient_p;

  /* Derivatives for Integrator: '<S76>/Integrator' */
  _rtXdot->Integrator_CSTATE_m = pid_control_B.Switch;

  /* Derivatives for Integrator: '<S71>/Filter' */
  _rtXdot->Filter_CSTATE_g = pid_control_B.FilterCoefficient_c;

  /* Derivatives for Enabled SubSystem: '<Root>/Subsystem1' */
  if (pid_control_DW.Subsystem1_MODE) {
    /* Derivatives for Integrator: '<S293>/Integrator' */
    _rtXdot->Integrator_CSTATE_p = pid_control_B.SumI4_ic;

    /* Derivatives for Integrator: '<S288>/Filter' */
    _rtXdot->Filter_CSTATE_m = pid_control_B.FilterCoefficient_m;
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)
             ->Integrator_CSTATE_p);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<Root>/Subsystem1' */

  /* Derivatives for Enabled SubSystem: '<S161>/Hpgw' */
  if (pid_control_DW.Hpgw_MODE) {
    /* Derivatives for Integrator: '<S172>/pgw_p' */
    _rtXdot->pgw_p_CSTATE[0] = pid_control_B.w_o[0];
    _rtXdot->pgw_p_CSTATE[1] = pid_control_B.w_o[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->pgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hpgw' */

  /* Derivatives for Enabled SubSystem: '<S162>/Hwgw(s)' */
  if (pid_control_DW.Hwgws_MODE) {
    /* Derivatives for Integrator: '<S177>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[0] = pid_control_B.w[0];

    /* Derivatives for Integrator: '<S177>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[0] = pid_control_B.w_a[0];

    /* Derivatives for Integrator: '<S177>/wg_p1' */
    _rtXdot->wg_p1_CSTATE[1] = pid_control_B.w[1];

    /* Derivatives for Integrator: '<S177>/wg_p2' */
    _rtXdot->wg_p2_CSTATE[1] = pid_control_B.w_a[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->wg_p1_CSTATE[0]);
      for (i1=0; i1 < 4; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S162>/Hwgw(s)' */

  /* Derivatives for Enabled SubSystem: '<S161>/Hqgw' */
  if (pid_control_DW.Hqgw_MODE) {
    /* Derivatives for Integrator: '<S173>/qgw_p' */
    _rtXdot->qgw_p_CSTATE[0] = pid_control_B.w_e0[0];
    _rtXdot->qgw_p_CSTATE[1] = pid_control_B.w_e0[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->qgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hqgw' */

  /* Derivatives for Enabled SubSystem: '<S162>/Hvgw(s)' */
  if (pid_control_DW.Hvgws_MODE) {
    /* Derivatives for Integrator: '<S176>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[0] = pid_control_B.w_g[0];

    /* Derivatives for Integrator: '<S176>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[0] = pid_control_B.w_e[0];

    /* Derivatives for Integrator: '<S176>/vg_p1' */
    _rtXdot->vg_p1_CSTATE[1] = pid_control_B.w_g[1];

    /* Derivatives for Integrator: '<S176>/vgw_p2' */
    _rtXdot->vgw_p2_CSTATE[1] = pid_control_B.w_e[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->vg_p1_CSTATE[0]);
      for (i1=0; i1 < 4; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S162>/Hvgw(s)' */

  /* Derivatives for Enabled SubSystem: '<S161>/Hrgw' */
  if (pid_control_DW.Hrgw_MODE) {
    /* Derivatives for Integrator: '<S174>/rgw_p' */
    _rtXdot->rgw_p_CSTATE[0] = pid_control_B.w_d[0];
    _rtXdot->rgw_p_CSTATE[1] = pid_control_B.w_d[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->rgw_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S161>/Hrgw' */

  /* Derivatives for TransferFcn: '<S34>/Transfer Fcn1' */
  _rtXdot->TransferFcn1_CSTATE = 0.0;
  _rtXdot->TransferFcn1_CSTATE += -0.01 * pid_control_X.TransferFcn1_CSTATE;
  _rtXdot->TransferFcn1_CSTATE += pid_control_B.Output;

  /* Derivatives for Enabled SubSystem: '<S162>/Hugw(s)' */
  if (pid_control_DW.Hugws_MODE) {
    /* Derivatives for Integrator: '<S175>/ug_p' */
    _rtXdot->ug_p_CSTATE[0] = pid_control_B.w_n[0];
    _rtXdot->ug_p_CSTATE[1] = pid_control_B.w_n[1];
  } else {
    {
      real_T *dx;
      int_T i1;
      dx = &(((XDot_pid_control_T *) (&pid_control_M)->derivs)->ug_p_CSTATE[0]);
      for (i1=0; i1 < 2; i1++) {
        dx[i1] = 0.0;
      }
    }
  }

  /* End of Derivatives for SubSystem: '<S162>/Hugw(s)' */

  /* Derivatives for TransferFcn: '<S34>/Transfer Fcn' */
  _rtXdot->TransferFcn_CSTATE[0] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += -0.898 * pid_control_X.TransferFcn_CSTATE[0];
  _rtXdot->TransferFcn_CSTATE[1] = 0.0;
  _rtXdot->TransferFcn_CSTATE[0] += -0.806 * pid_control_X.TransferFcn_CSTATE[1];
  _mm_storeu_pd(&tmp[0], _mm_add_pd(_mm_set_pd(_rtXdot->TransferFcn_CSTATE[0],
    pid_control_X.TransferFcn_CSTATE[0]), _mm_set_pd(pid_control_B.Output,
    _rtXdot->TransferFcn_CSTATE[1])));
  _rtXdot->TransferFcn_CSTATE[1] = tmp[0];
  _rtXdot->TransferFcn_CSTATE[0] = tmp[1];

  /* Derivatives for Integrator: '<S34>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = pid_control_B.Power;

  /* Derivatives for SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */
  switch (pid_control_DW.IntegratorSecondOrderLimited_MO) {
   case 0:
    _rtXdot->IntegratorSecondOrderLimited_CS[0] =
      pid_control_X.IntegratorSecondOrderLimited_CS[1];
    _rtXdot->IntegratorSecondOrderLimited_CS[1] = pid_control_B.Sum2;
    break;

   case 3:
    _rtXdot->IntegratorSecondOrderLimited_CS[0] =
      pid_control_X.IntegratorSecondOrderLimited_CS[1];
    _rtXdot->IntegratorSecondOrderLimited_CS[1] = 0.0;
    break;

   case 4:
    _rtXdot->IntegratorSecondOrderLimited_CS[0] =
      pid_control_X.IntegratorSecondOrderLimited_CS[1];
    _rtXdot->IntegratorSecondOrderLimited_CS[1] = 0.0;
    break;

   case 1:
    _rtXdot->IntegratorSecondOrderLimited_CS[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited_CS[1] = 0.0;
    break;

   case 2:
    _rtXdot->IntegratorSecondOrderLimited_CS[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited_CS[1] = 0.0;
    break;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */

  /* Derivatives for SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */
  switch (pid_control_DW.IntegratorSecondOrderLimited__c) {
   case 0:
    _rtXdot->IntegratorSecondOrderLimited__k[0] =
      pid_control_X.IntegratorSecondOrderLimited__k[1];
    _rtXdot->IntegratorSecondOrderLimited__k[1] = pid_control_B.Sum2_n;
    break;

   case 3:
    _rtXdot->IntegratorSecondOrderLimited__k[0] =
      pid_control_X.IntegratorSecondOrderLimited__k[1];
    _rtXdot->IntegratorSecondOrderLimited__k[1] = 0.0;
    break;

   case 4:
    _rtXdot->IntegratorSecondOrderLimited__k[0] =
      pid_control_X.IntegratorSecondOrderLimited__k[1];
    _rtXdot->IntegratorSecondOrderLimited__k[1] = 0.0;
    break;

   case 1:
    _rtXdot->IntegratorSecondOrderLimited__k[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__k[1] = 0.0;
    break;

   case 2:
    _rtXdot->IntegratorSecondOrderLimited__k[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__k[1] = 0.0;
    break;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */

  /* Derivatives for SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */
  switch (pid_control_DW.IntegratorSecondOrderLimited__h) {
   case 0:
    _rtXdot->IntegratorSecondOrderLimited__b[0] =
      pid_control_X.IntegratorSecondOrderLimited__b[1];
    _rtXdot->IntegratorSecondOrderLimited__b[1] = pid_control_B.Sum2_i;
    break;

   case 3:
    _rtXdot->IntegratorSecondOrderLimited__b[0] =
      pid_control_X.IntegratorSecondOrderLimited__b[1];
    _rtXdot->IntegratorSecondOrderLimited__b[1] = 0.0;
    break;

   case 4:
    _rtXdot->IntegratorSecondOrderLimited__b[0] =
      pid_control_X.IntegratorSecondOrderLimited__b[1];
    _rtXdot->IntegratorSecondOrderLimited__b[1] = 0.0;
    break;

   case 1:
    _rtXdot->IntegratorSecondOrderLimited__b[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__b[1] = 0.0;
    break;

   case 2:
    _rtXdot->IntegratorSecondOrderLimited__b[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__b[1] = 0.0;
    break;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */

  /* Derivatives for SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */
  switch (pid_control_DW.IntegratorSecondOrderLimited__o) {
   case 0:
    _rtXdot->IntegratorSecondOrderLimited__l[0] =
      pid_control_X.IntegratorSecondOrderLimited__l[1];
    _rtXdot->IntegratorSecondOrderLimited__l[1] = pid_control_B.Sum2_l;
    break;

   case 3:
    _rtXdot->IntegratorSecondOrderLimited__l[0] =
      pid_control_X.IntegratorSecondOrderLimited__l[1];
    _rtXdot->IntegratorSecondOrderLimited__l[1] = 0.0;
    break;

   case 4:
    _rtXdot->IntegratorSecondOrderLimited__l[0] =
      pid_control_X.IntegratorSecondOrderLimited__l[1];
    _rtXdot->IntegratorSecondOrderLimited__l[1] = 0.0;
    break;

   case 1:
    _rtXdot->IntegratorSecondOrderLimited__l[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__l[1] = 0.0;
    break;

   case 2:
    _rtXdot->IntegratorSecondOrderLimited__l[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__l[1] = 0.0;
    break;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */

  /* Derivatives for SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
  switch (pid_control_DW.IntegratorSecondOrderLimited__j) {
   case 0:
    _rtXdot->IntegratorSecondOrderLimited__e[0] =
      pid_control_X.IntegratorSecondOrderLimited__e[1];
    _rtXdot->IntegratorSecondOrderLimited__e[1] = pid_control_B.Sum2_f;
    break;

   case 3:
    _rtXdot->IntegratorSecondOrderLimited__e[0] =
      pid_control_X.IntegratorSecondOrderLimited__e[1];
    _rtXdot->IntegratorSecondOrderLimited__e[1] = 0.0;
    break;

   case 4:
    _rtXdot->IntegratorSecondOrderLimited__e[0] =
      pid_control_X.IntegratorSecondOrderLimited__e[1];
    _rtXdot->IntegratorSecondOrderLimited__e[1] = 0.0;
    break;

   case 1:
    _rtXdot->IntegratorSecondOrderLimited__e[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__e[1] = 0.0;
    break;

   case 2:
    _rtXdot->IntegratorSecondOrderLimited__e[0] = 0.0;
    _rtXdot->IntegratorSecondOrderLimited__e[1] = 0.0;
    break;
  }

  /* End of Derivatives for SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
}

/* Model initialize function */
void pid_control::initialize()
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&(&pid_control_M)->solverInfo, &(&pid_control_M)
                          ->Timing.simTimeStep);
    rtsiSetTPtr(&(&pid_control_M)->solverInfo, &rtmGetTPtr((&pid_control_M)));
    rtsiSetStepSizePtr(&(&pid_control_M)->solverInfo, &(&pid_control_M)
                       ->Timing.stepSize0);
    rtsiSetdXPtr(&(&pid_control_M)->solverInfo, &(&pid_control_M)->derivs);
    rtsiSetContStatesPtr(&(&pid_control_M)->solverInfo, (real_T **)
                         &(&pid_control_M)->contStates);
    rtsiSetNumContStatesPtr(&(&pid_control_M)->solverInfo, &(&pid_control_M)
      ->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&pid_control_M)->solverInfo,
      &(&pid_control_M)->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&pid_control_M)->solverInfo,
      &(&pid_control_M)->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&pid_control_M)->solverInfo,
      &(&pid_control_M)->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&(&pid_control_M)->solverInfo, (boolean_T**)
      &(&pid_control_M)->contStateDisabled);
    rtsiSetErrorStatusPtr(&(&pid_control_M)->solverInfo, (&rtmGetErrorStatus
      ((&pid_control_M))));
    rtsiSetRTModelPtr(&(&pid_control_M)->solverInfo, (&pid_control_M));
  }

  rtsiSetSimTimeStep(&(&pid_control_M)->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&(&pid_control_M)->solverInfo, false);
  rtsiSetIsContModeFrozen(&(&pid_control_M)->solverInfo, false);
  (&pid_control_M)->intgData.y = (&pid_control_M)->odeY;
  (&pid_control_M)->intgData.f[0] = (&pid_control_M)->odeF[0];
  (&pid_control_M)->intgData.f[1] = (&pid_control_M)->odeF[1];
  (&pid_control_M)->intgData.f[2] = (&pid_control_M)->odeF[2];
  (&pid_control_M)->intgData.f[3] = (&pid_control_M)->odeF[3];
  (&pid_control_M)->contStates = ((X_pid_control_T *) &pid_control_X);
  (&pid_control_M)->contStateDisabled = ((XDis_pid_control_T *)
    &pid_control_XDis);
  (&pid_control_M)->Timing.tStart = (0.0);
  rtsiSetSolverData(&(&pid_control_M)->solverInfo, static_cast<void *>
                    (&(&pid_control_M)->intgData));
  rtsiSetSolverName(&(&pid_control_M)->solverInfo,"ode4");
  rtmSetTPtr((&pid_control_M), &(&pid_control_M)->Timing.tArray[0]);
  (&pid_control_M)->Timing.stepSize0 = 0.01;
  rtmSetFirstInitCond((&pid_control_M), 1);

  {
    rmw_qos_profile_t qos_profile;
    int32_T i;
    static const char_T b_zeroDelimTopic[18] = "/ekranoplano/odom";

    /* Start for InitialCondition: '<S34>/IC' */
    memcpy(&pid_control_B.IC[0], &pid_control_ConstP.pooled17[0], 12U * sizeof
           (real_T));

    /* Start for InitialCondition: '<S34>/IC' */
    pid_control_DW.IC_FirstOutputTime = true;

    /* Start for MATLABSystem: '<Root>/Coordinate Transformation Conversion' */
    pid_control_DW.objisempty_d = true;
    pid_control_DW.obj_c.isInitialized = 1;

    /* Start for MATLABSystem: '<S27>/SinkBlock' */
    pid_control_DW.obj_i.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_i.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_m = true;
    pid_control_DW.obj_i.isSetupComplete = false;
    pid_control_DW.obj_i.isInitialized = 1;
    pid_control_Publisher_setupImpl(&pid_control_DW.obj_i);
    pid_control_DW.obj_i.isSetupComplete = true;

    /* Start for MATLABSystem: '<S28>/SinkBlock' */
    pid_control_DW.obj_o.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_o.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_ev = true;
    pid_control_DW.obj_o.isSetupComplete = false;
    pid_control_DW.obj_o.isInitialized = 1;
    qos_profile = pi_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_default, 10.0,
      (rtInf), (rtInf), (rtInf),
      pid_control_DW.obj_o.QOSAvoidROSNamespaceConventions);
    for (i = 0; i < 18; i++) {
      pid_control_B.b_zeroDelimTopic_b[i] = b_zeroDelimTopic[i];
    }

    Pub_pid_control_697.createPublisher(&pid_control_B.b_zeroDelimTopic_b[0],
      qos_profile);
    pid_control_DW.obj_o.isSetupComplete = true;

    /* End of Start for MATLABSystem: '<S28>/SinkBlock' */

    /* Start for MATLABSystem: '<S29>/SinkBlock' */
    pid_control_DW.obj_dl.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_dl.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_ld = true;
    pid_control_DW.obj_dl.isSetupComplete = false;
    pid_control_DW.obj_dl.isInitialized = 1;
    pid_contr_Publisher_setupImpl_m(&pid_control_DW.obj_dl);
    pid_control_DW.obj_dl.isSetupComplete = true;

    /* Start for MATLABSystem: '<S30>/SinkBlock' */
    pid_control_DW.obj_d.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_d.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_cr = true;
    pid_control_DW.obj_d.isSetupComplete = false;
    pid_control_DW.obj_d.isInitialized = 1;
    pid_cont_Publisher_setupImpl_m3(&pid_control_DW.obj_d);
    pid_control_DW.obj_d.isSetupComplete = true;

    /* Start for MATLABSystem: '<S35>/SourceBlock' */
    pid_control_DW.obj_m.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_m.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_g = true;
    pid_control_DW.obj_m.isSetupComplete = false;
    pid_control_DW.obj_m.isInitialized = 1;
    pid_c_Subscriber_setupImpl_m3hf(&pid_control_DW.obj_m);
    pid_control_DW.obj_m.isSetupComplete = true;

    /* Start for Enabled SubSystem: '<Root>/Subsystem2' */
    (void) memset(&(pid_control_XDis.Integrator_CSTATE_n), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<Root>/Subsystem2' */

    /* Start for MATLABSystem: '<S36>/SourceBlock' */
    pid_control_DW.obj_k.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_k.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty = true;
    pid_control_DW.obj_k.isSetupComplete = false;
    pid_control_DW.obj_k.isInitialized = 1;
    pid__Subscriber_setupImpl_m3hfp(&pid_control_DW.obj_k);
    pid_control_DW.obj_k.isSetupComplete = true;

    /* Start for Enabled SubSystem: '<Root>/Subsystem1' */
    (void) memset(&(pid_control_XDis.Integrator_CSTATE_p), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<Root>/Subsystem1' */

    /* Start for Atomic SubSystem: '<Root>/Call Service' */
    /* Start for MATLABSystem: '<S6>/ServiceCaller' */
    pid_control_DW.obj.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_f = true;
    pid_control_DW.obj.isSetupComplete = false;
    pid_control_DW.obj.isInitialized = 1;
    pid_con_ServiceCaller_setupImpl(&pid_control_DW.obj);
    pid_control_DW.obj.isSetupComplete = true;

    /* End of Start for SubSystem: '<Root>/Call Service' */

    /* Start for Enabled SubSystem: '<S161>/Hpgw' */
    (void) memset(&(pid_control_XDis.pgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hpgw' */

    /* Start for Enabled SubSystem: '<S162>/Hwgw(s)' */
    (void) memset(&(pid_control_XDis.wg_p1_CSTATE), 1,
                  4*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S162>/Hwgw(s)' */

    /* Start for Enabled SubSystem: '<S161>/Hqgw' */
    (void) memset(&(pid_control_XDis.qgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hqgw' */

    /* Start for Enabled SubSystem: '<S162>/Hvgw(s)' */
    (void) memset(&(pid_control_XDis.vg_p1_CSTATE), 1,
                  4*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S162>/Hvgw(s)' */

    /* Start for Enabled SubSystem: '<S161>/Hrgw' */
    (void) memset(&(pid_control_XDis.rgw_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S161>/Hrgw' */

    /* Start for If: '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    pid_control_DW.ifHeightMaxlowaltitudeelseifHei = -1;

    /* Start for MATLABSystem: '<S156>/SourceBlock' */
    pid_control_DW.obj_h4.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_h4.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_c = true;
    pid_control_DW.obj_h4.isSetupComplete = false;
    pid_control_DW.obj_h4.isInitialized = 1;
    pid_contro_Subscriber_setupImpl(&pid_control_DW.obj_h4);
    pid_control_DW.obj_h4.isSetupComplete = true;

    /* Start for MATLABSystem: '<S158>/SourceBlock' */
    pid_control_DW.obj_p.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_p.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_e = true;
    pid_control_DW.obj_p.isSetupComplete = false;
    pid_control_DW.obj_p.isInitialized = 1;
    pid_con_Subscriber_setupImpl_m3(&pid_control_DW.obj_p);
    pid_control_DW.obj_p.isSetupComplete = true;

    /* Start for Enabled SubSystem: '<S162>/Hugw(s)' */
    (void) memset(&(pid_control_XDis.ug_p_CSTATE), 1,
                  2*sizeof(boolean_T));

    /* End of Start for SubSystem: '<S162>/Hugw(s)' */

    /* Start for If: '<S167>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
    pid_control_DW.ifHeightMaxlowaltitudeelseifH_k = -1;

    /* Start for MATLABSystem: '<S159>/SourceBlock' */
    pid_control_DW.obj_h.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_h.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_a = true;
    pid_control_DW.obj_h.isSetupComplete = false;
    pid_control_DW.obj_h.isInitialized = 1;
    pid_co_Subscriber_setupImpl_m3h(&pid_control_DW.obj_h);
    pid_control_DW.obj_h.isSetupComplete = true;

    /* Start for MATLABSystem: '<S157>/SourceBlock' */
    pid_control_DW.obj_hy.QOSAvoidROSNamespaceConventions = false;
    pid_control_DW.obj_hy.matlabCodegenIsDeleted = false;
    pid_control_DW.objisempty_l = true;
    pid_control_DW.obj_hy.isSetupComplete = false;
    pid_control_DW.obj_hy.isInitialized = 1;
    pid_cont_Subscriber_setupImpl_m(&pid_control_DW.obj_hy);
    pid_control_DW.obj_hy.isSetupComplete = true;
  }

  pid_control_PrevZCX.Integrator_Reset_ZCE = UNINITIALIZED_ZCSIG;

  /* InitializeConditions for Memory: '<S34>/Memory2' */
  memcpy(&pid_control_DW.Memory2_PreviousInput[0], &pid_control_ConstP.pooled17
         [0], 12U * sizeof(real_T));

  /* InitializeConditions for Integrator: '<S34>/Integrator' */
  if (rtmIsFirstInitCond((&pid_control_M))) {
    pid_control_X.Integrator_CSTATE[0] = 1.0;
    pid_control_X.Integrator_CSTATE[1] = 0.0;
    pid_control_X.Integrator_CSTATE[2] = 0.0;
    pid_control_X.Integrator_CSTATE[3] = 0.0;
    pid_control_X.Integrator_CSTATE[4] = 0.0;
    pid_control_X.Integrator_CSTATE[5] = 0.0;
    pid_control_X.Integrator_CSTATE[6] = 0.0;
    pid_control_X.Integrator_CSTATE[7] = -0.026179938779914945;
    pid_control_X.Integrator_CSTATE[8] = 0.0;
    pid_control_X.Integrator_CSTATE[9] = 0.0;
    pid_control_X.Integrator_CSTATE[10] = 0.0;
    pid_control_X.Integrator_CSTATE[11] = -0.25;
  }

  pid_control_DW.Integrator_DWORK1 = true;

  /* End of InitializeConditions for Integrator: '<S34>/Integrator' */

  /* InitializeConditions for Integrator: '<S130>/Integrator' */
  pid_control_X.Integrator_CSTATE_f = 0.0;

  /* InitializeConditions for Integrator: '<S125>/Filter' */
  pid_control_X.Filter_CSTATE = 0.0;

  /* InitializeConditions for UnitDelay: '<Root>/Unit Delay3' */
  pid_control_DW.UnitDelay3_DSTATE = 0.8;

  /* InitializeConditions for Integrator: '<S240>/Integrator' */
  pid_control_X.Integrator_CSTATE_d = 0.0;

  /* InitializeConditions for Integrator: '<S235>/Filter' */
  pid_control_X.Filter_CSTATE_f = 0.0;

  /* InitializeConditions for Integrator: '<S76>/Integrator' */
  pid_control_X.Integrator_CSTATE_m = 0.0;

  /* InitializeConditions for Integrator: '<S71>/Filter' */
  pid_control_X.Filter_CSTATE_g = 0.0;

  /* InitializeConditions for RateLimiter: '<Root>/Rate Limiter-theta_sp' */
  pid_control_DW.LastMajorTime = (rtMinusInf);

  /* InitializeConditions for RandomNumber: '<S171>/White Noise' */
  pid_control_DW.RandSeed[0] = 1529675776U;
  pid_control_DW.NextOutput[0] = rt_nrand_Upu32_Yd_f_pw_snf
    (&pid_control_DW.RandSeed[0]);
  pid_control_DW.RandSeed[1] = 1529741312U;
  pid_control_DW.NextOutput[1] = rt_nrand_Upu32_Yd_f_pw_snf
    (&pid_control_DW.RandSeed[1]);
  pid_control_DW.RandSeed[2] = 1529806848U;
  pid_control_DW.NextOutput[2] = rt_nrand_Upu32_Yd_f_pw_snf
    (&pid_control_DW.RandSeed[2]);
  pid_control_DW.RandSeed[3] = 1529872384U;
  pid_control_DW.NextOutput[3] = rt_nrand_Upu32_Yd_f_pw_snf
    (&pid_control_DW.RandSeed[3]);

  /* InitializeConditions for RandomNumber: '<S151>/White Noise' */
  pid_control_DW.RandSeed_a = 1529675776U;
  pid_control_DW.NextOutput_k = rt_nrand_Upu32_Yd_f_pw_snf
    (&pid_control_DW.RandSeed_a);

  /* InitializeConditions for TransferFcn: '<S34>/Transfer Fcn1' */
  pid_control_X.TransferFcn1_CSTATE = 0.0;

  /* InitializeConditions for TransferFcn: '<S34>/Transfer Fcn' */
  pid_control_X.TransferFcn_CSTATE[0] = 0.0;
  pid_control_X.TransferFcn_CSTATE[1] = 0.0;

  /* InitializeConditions for Integrator: '<S34>/Integrator1' */
  pid_control_X.Integrator1_CSTATE = 0.0;

  /* InitializeConditions for SecondOrderIntegrator: '<S20>/Integrator, Second-Order Limited' */
  pid_control_X.IntegratorSecondOrderLimited_CS[0] = 0.0;
  pid_control_X.IntegratorSecondOrderLimited_CS[1] = 0.0;

  /* InitializeConditions for SecondOrderIntegrator: '<S21>/Integrator, Second-Order Limited' */
  pid_control_X.IntegratorSecondOrderLimited__k[0] = 0.0;
  pid_control_X.IntegratorSecondOrderLimited__k[1] = 0.0;

  /* InitializeConditions for SecondOrderIntegrator: '<S22>/Integrator, Second-Order Limited' */
  pid_control_X.IntegratorSecondOrderLimited__b[0] = 0.0;
  pid_control_X.IntegratorSecondOrderLimited__b[1] = 0.0;

  /* InitializeConditions for SecondOrderIntegrator: '<S23>/Integrator, Second-Order Limited' */
  pid_control_X.IntegratorSecondOrderLimited__l[0] = 0.0;
  pid_control_X.IntegratorSecondOrderLimited__l[1] = 0.0;

  /* InitializeConditions for SecondOrderIntegrator: '<S24>/Integrator, Second-Order Limited' */
  pid_control_X.IntegratorSecondOrderLimited__e[0] = 0.0;
  pid_control_X.IntegratorSecondOrderLimited__e[1] = 0.0;

  /* SystemInitialize for Enabled SubSystem: '<S35>/Enabled Subsystem' */
  pid_con_EnabledSubsystem_p_Init(&pid_control_B.EnabledSubsystem_b);

  /* End of SystemInitialize for SubSystem: '<S35>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<Root>/Subsystem2' */
  /* InitializeConditions for Integrator: '<S346>/Integrator' */
  pid_control_X.Integrator_CSTATE_n = 0.0;

  /* InitializeConditions for Integrator: '<S341>/Filter' */
  pid_control_X.Filter_CSTATE_fb = 0.0;

  /* End of SystemInitialize for SubSystem: '<Root>/Subsystem2' */

  /* SystemInitialize for Enabled SubSystem: '<S36>/Enabled Subsystem' */
  pid_con_EnabledSubsystem_p_Init(&pid_control_B.EnabledSubsystem_a);

  /* End of SystemInitialize for SubSystem: '<S36>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<Root>/Subsystem1' */
  /* InitializeConditions for Integrator: '<S293>/Integrator' */
  pid_control_X.Integrator_CSTATE_p = 0.0;

  /* InitializeConditions for Integrator: '<S288>/Filter' */
  pid_control_X.Filter_CSTATE_m = 0.0;

  /* End of SystemInitialize for SubSystem: '<Root>/Subsystem1' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hpgw' */
  /* InitializeConditions for Integrator: '<S172>/pgw_p' */
  pid_control_X.pgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hpgw' */

  /* SystemInitialize for Enabled SubSystem: '<S162>/Hwgw(s)' */
  /* InitializeConditions for Integrator: '<S177>/wg_p1' */
  pid_control_X.wg_p1_CSTATE[0] = 0.0;

  /* InitializeConditions for Integrator: '<S177>/wg_p2' */
  pid_control_X.wg_p2_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S162>/Hwgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hqgw' */
  /* InitializeConditions for Integrator: '<S173>/qgw_p' */
  pid_control_X.qgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hqgw' */

  /* SystemInitialize for Enabled SubSystem: '<S162>/Hvgw(s)' */
  /* InitializeConditions for Integrator: '<S176>/vg_p1' */
  pid_control_X.vg_p1_CSTATE[0] = 0.0;

  /* InitializeConditions for Integrator: '<S176>/vgw_p2' */
  pid_control_X.vgw_p2_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S162>/Hvgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hrgw' */
  /* InitializeConditions for Integrator: '<S174>/rgw_p' */
  pid_control_X.rgw_p_CSTATE[0] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hrgw' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hpgw' */
  /* InitializeConditions for Integrator: '<S172>/pgw_p' */
  pid_control_X.pgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hpgw' */

  /* SystemInitialize for Enabled SubSystem: '<S162>/Hwgw(s)' */
  /* InitializeConditions for Integrator: '<S177>/wg_p1' */
  pid_control_X.wg_p1_CSTATE[1] = 0.0;

  /* InitializeConditions for Integrator: '<S177>/wg_p2' */
  pid_control_X.wg_p2_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S162>/Hwgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hqgw' */
  /* InitializeConditions for Integrator: '<S173>/qgw_p' */
  pid_control_X.qgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hqgw' */

  /* SystemInitialize for Enabled SubSystem: '<S162>/Hvgw(s)' */
  /* InitializeConditions for Integrator: '<S176>/vg_p1' */
  pid_control_X.vg_p1_CSTATE[1] = 0.0;

  /* InitializeConditions for Integrator: '<S176>/vgw_p2' */
  pid_control_X.vgw_p2_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S162>/Hvgw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S161>/Hrgw' */
  /* InitializeConditions for Integrator: '<S174>/rgw_p' */
  pid_control_X.rgw_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S161>/Hrgw' */

  /* SystemInitialize for Enabled SubSystem: '<S156>/Enabled Subsystem' */
  pid_contr_EnabledSubsystem_Init(&pid_control_B.EnabledSubsystem);

  /* End of SystemInitialize for SubSystem: '<S156>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S158>/Enabled Subsystem' */
  pid_con_EnabledSubsystem_p_Init(&pid_control_B.EnabledSubsystem_p);

  /* End of SystemInitialize for SubSystem: '<S158>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S162>/Hugw(s)' */
  /* InitializeConditions for Integrator: '<S175>/ug_p' */
  pid_control_X.ug_p_CSTATE[0] = 0.0;
  pid_control_X.ug_p_CSTATE[1] = 0.0;

  /* End of SystemInitialize for SubSystem: '<S162>/Hugw(s)' */

  /* SystemInitialize for Enabled SubSystem: '<S159>/Enabled Subsystem' */
  pid_contr_EnabledSubsystem_Init(&pid_control_B.EnabledSubsystem_pt);

  /* End of SystemInitialize for SubSystem: '<S159>/Enabled Subsystem' */

  /* SystemInitialize for Enabled SubSystem: '<S157>/Enabled Subsystem' */
  pid_con_EnabledSubsystem_p_Init(&pid_control_B.EnabledSubsystem_k);

  /* End of SystemInitialize for SubSystem: '<S157>/Enabled Subsystem' */

  /* set "at time zero" to false */
  if (rtmIsFirstInitCond((&pid_control_M))) {
    rtmSetFirstInitCond((&pid_control_M), 0);
  }
}

/* Model terminate function */
void pid_control::terminate()
{
  /* Terminate for MATLABSystem: '<S27>/SinkBlock' */
  if (!pid_control_DW.obj_i.matlabCodegenIsDeleted) {
    pid_control_DW.obj_i.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_i.isInitialized == 1) &&
        pid_control_DW.obj_i.isSetupComplete) {
      Pub_pid_control_548.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S27>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S28>/SinkBlock' */
  if (!pid_control_DW.obj_o.matlabCodegenIsDeleted) {
    pid_control_DW.obj_o.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_o.isInitialized == 1) &&
        pid_control_DW.obj_o.isSetupComplete) {
      Pub_pid_control_697.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S28>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S29>/SinkBlock' */
  if (!pid_control_DW.obj_dl.matlabCodegenIsDeleted) {
    pid_control_DW.obj_dl.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_dl.isInitialized == 1) &&
        pid_control_DW.obj_dl.isSetupComplete) {
      Pub_pid_control_811.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S29>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S30>/SinkBlock' */
  if (!pid_control_DW.obj_d.matlabCodegenIsDeleted) {
    pid_control_DW.obj_d.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_d.isInitialized == 1) &&
        pid_control_DW.obj_d.isSetupComplete) {
      Pub_pid_control_835.resetPublisherPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S30>/SinkBlock' */

  /* Terminate for MATLABSystem: '<S35>/SourceBlock' */
  if (!pid_control_DW.obj_m.matlabCodegenIsDeleted) {
    pid_control_DW.obj_m.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_m.isInitialized == 1) &&
        pid_control_DW.obj_m.isSetupComplete) {
      Sub_pid_control_435.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S35>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S36>/SourceBlock' */
  if (!pid_control_DW.obj_k.matlabCodegenIsDeleted) {
    pid_control_DW.obj_k.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_k.isInitialized == 1) &&
        pid_control_DW.obj_k.isSetupComplete) {
      Sub_pid_control_377.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S36>/SourceBlock' */

  /* Terminate for Atomic SubSystem: '<Root>/Call Service' */
  /* Terminate for MATLABSystem: '<S6>/ServiceCaller' */
  if (!pid_control_DW.obj.matlabCodegenIsDeleted) {
    pid_control_DW.obj.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj.isInitialized == 1) &&
        pid_control_DW.obj.isSetupComplete) {
      ServCall_pid_control_326.resetSvcClientPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S6>/ServiceCaller' */
  /* End of Terminate for SubSystem: '<Root>/Call Service' */

  /* Terminate for MATLABSystem: '<S156>/SourceBlock' */
  if (!pid_control_DW.obj_h4.matlabCodegenIsDeleted) {
    pid_control_DW.obj_h4.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_h4.isInitialized == 1) &&
        pid_control_DW.obj_h4.isSetupComplete) {
      Sub_pid_control_423.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S156>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S158>/SourceBlock' */
  if (!pid_control_DW.obj_p.matlabCodegenIsDeleted) {
    pid_control_DW.obj_p.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_p.isInitialized == 1) &&
        pid_control_DW.obj_p.isSetupComplete) {
      Sub_pid_control_445.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S158>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S159>/SourceBlock' */
  if (!pid_control_DW.obj_h.matlabCodegenIsDeleted) {
    pid_control_DW.obj_h.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_h.isInitialized == 1) &&
        pid_control_DW.obj_h.isSetupComplete) {
      Sub_pid_control_417.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S159>/SourceBlock' */

  /* Terminate for MATLABSystem: '<S157>/SourceBlock' */
  if (!pid_control_DW.obj_hy.matlabCodegenIsDeleted) {
    pid_control_DW.obj_hy.matlabCodegenIsDeleted = true;
    if ((pid_control_DW.obj_hy.isInitialized == 1) &&
        pid_control_DW.obj_hy.isSetupComplete) {
      Sub_pid_control_443.resetSubscriberPtr();//();
    }
  }

  /* End of Terminate for MATLABSystem: '<S157>/SourceBlock' */
}

/* Constructor */
pid_control::pid_control() :
  pid_control_B(),
  pid_control_DW(),
  pid_control_X(),
  pid_control_XDis(),
  pid_control_PrevZCX(),
  pid_control_M()
{
  /* Currently there is no constructor body generated.*/
}

/* Destructor */
pid_control::~pid_control()
{
  /* Currently there is no destructor body generated.*/
}

/* Real-Time Model get method */
RT_MODEL_pid_control_T * pid_control::getRTM()
{
  return (&pid_control_M);
}
