/*
 * pid_control.h
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

#ifndef pid_control_h_
#define pid_control_h_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "pid_control_types.h"
#include "rmw/qos_profiles.h"

extern "C"
{

#include "rt_nonfinite.h"

}

#include <string.h>

extern "C"
{

#include "rtGetInf.h"

}

extern "C"
{

#include "rtGetNaN.h"

}

#include <stddef.h>
#include "zero_crossing_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
#define rtmGetContStateDisabled(rtm)   ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
#define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
#define rtmGetContStates(rtm)          ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
#define rtmSetContStates(rtm, val)     ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
#define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
#define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
#define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetIntgData
#define rtmGetIntgData(rtm)            ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
#define rtmSetIntgData(rtm, val)       ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
#define rtmGetOdeF(rtm)                ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
#define rtmSetOdeF(rtm, val)           ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
#define rtmGetOdeY(rtm)                ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
#define rtmSetOdeY(rtm, val)           ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
#define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
#define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
#define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
#define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetZCCacheNeedsReset
#define rtmGetZCCacheNeedsReset(rtm)   ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
#define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
#define rtmGetdX(rtm)                  ((rtm)->derivs)
#endif

#ifndef rtmSetdX
#define rtmSetdX(rtm, val)             ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef rtmGetTStart
#define rtmGetTStart(rtm)              ((rtm)->Timing.tStart)
#endif

/* Block signals for system '<Root>/MATLAB Function-ned_to_PoseStamped' */
struct B_MATLABFunctionned_to_PoseSt_T {
  uint32_T lengthOut;          /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
  uint8_T stringOut[128];      /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
};

/* Block signals for system '<S156>/Enabled Subsystem' */
struct B_EnabledSubsystem_pid_contro_T {
  SL_Bus_std_msgs_Bool In1;            /* '<S199>/In1' */
};

/* Block signals for system '<S157>/Enabled Subsystem' */
struct B_EnabledSubsystem_pid_cont_d_T {
  SL_Bus_std_msgs_Float64 In1;         /* '<S200>/In1' */
};

/* Block signals (default storage) */
struct B_pid_control_T {
  SL_Bus_nav_msgs_Odometry BusAssignmentODOM;/* '<Root>/Bus Assignment-ODOM' */
  SL_Bus_sensor_msgs_Imu BusAssignmentIMU;/* '<Root>/Bus Assignment-IMU' */
  SL_Bus_gazebo_msgs_SetEntityStateRequest BusAssignmentMODELO;/* '<Root>/Bus Assignment-MODELO' */
  SL_Bus_geometry_msgs_PoseStamped BusAssignmentPoseStaamped;
                                      /* '<Root>/Bus Assignment-PoseStaamped' */
  SL_Bus_geometry_msgs_TwistStamped BusAssignmentTwistStamped;
                                      /* '<Root>/Bus Assignment-TwistStamped' */
  real_T IC[12];                       /* '<S34>/IC' */
  real_T x[12];                        /* '<S34>/Integrator' */
  real_T R[9];
  real_T RotationAnglestoDirectionCo[9];
                        /* '<S34>/Rotation Angles to Direction Cosine Matrix' */
  real_T dv[9];
  char_T b_zeroDelimTopic[26];
  char_T b_zeroDelimTopic_m[25];
  char_T b_zeroDelimTopic_c[25];
  real_T wbe_b[3];
  real_T FE1_b[3];
  real_T F_b[3];
  real_T Reshape1_b[3];                /* '<S184>/Reshape1' */
  real_T Dtot[3];
  char_T b_zeroDelimTopic_k[22];
  char_T b_zeroDelimTopic_cx[22];
  char_T b_zeroDelimTopic_b[18];
  char_T b_zeroDelimTopic_p[17];
  char_T b_zeroDelimTopic_cv[17];
  char_T b_zeroDelimTopic_f[17];
  sJ4ih70VmKcvCeguWN0mNVF deadline;
  sJ4ih70VmKcvCeguWN0mNVF deadline_g;
  sJ4ih70VmKcvCeguWN0mNVF deadline_g1;
  sJ4ih70VmKcvCeguWN0mNVF deadline_m;
  sJ4ih70VmKcvCeguWN0mNVF deadline_n;
  sJ4ih70VmKcvCeguWN0mNVF deadline_p;
  sJ4ih70VmKcvCeguWN0mNVF deadline_l;
  sJ4ih70VmKcvCeguWN0mNVF deadline_j;
  sJ4ih70VmKcvCeguWN0mNVF deadline_d;
  sJ4ih70VmKcvCeguWN0mNVF lifespan;
  sJ4ih70VmKcvCeguWN0mNVF deadline_gu;
  sJ4ih70VmKcvCeguWN0mNVF lifespan_l;
  sJ4ih70VmKcvCeguWN0mNVF deadline_dh;
  real_T Product1_l[2];                /* '<S185>/Product1' */
  real_T dv1[2];
  real_T GainZ;                        /* '<Root>/Gain-Z' */
  real_T Saturation1;                  /* '<Root>/Saturation1' */
  real_T FilterCoefficient;            /* '<S133>/Filter Coefficient' */
  real_T Saturation;                   /* '<S137>/Saturation' */
  real_T Switch3;                      /* '<Root>/Switch3' */
  real_T RelayEnableFlight;            /* '<Root>/Relay-Enable-Flight' */
  real_T RateLimiter2;                 /* '<Root>/Rate Limiter2' */
  real_T FilterCoefficient_p;          /* '<S243>/Filter Coefficient' */
  real_T Saturation_m;                 /* '<S247>/Saturation' */
  real_T Saturation_roll_sp;           /* '<Root>/Saturation_roll_sp' */
  real_T FilterCoefficient_c;          /* '<S79>/Filter Coefficient' */
  real_T Saturation_k;                 /* '<S83>/Saturation' */
  real_T Gain2;                        /* '<Root>/Gain2' */
  real_T RateLimitertheta_sp;          /* '<Root>/Rate Limiter-theta_sp' */
  real_T Gain3;                        /* '<Root>/Gain3' */
  real_T Gain4;                        /* '<Root>/Gain4' */
  real_T Switch;                       /* '<S66>/Switch' */
  real_T Switch_j;                     /* '<S120>/Switch' */
  real_T Product[4];                   /* '<S171>/Product' */
  real_T Memory[3];                    /* '<S34>/Memory' */
  real_T Memory1[3];                   /* '<S34>/Memory1' */
  real_T Output;                       /* '<S151>/Output' */
  real_T SumOLA1[3];                   /* '<S34>/SumOLA1' */
  real_T SumOLA[3];                    /* '<S34>/SumOLA' */
  real_T Power;                        /* '<S34>/Product2' */
  real_T Gain3_f;                      /* '<S34>/Gain3' */
  real_T EnergykWh;                    /* '<S34>/Gain1' */
  real_T powerdemand;                  /* '<S34>/Divide' */
  real_T loadtorque;                   /* '<S34>/Divide1' */
  real_T SumI4;                        /* '<S232>/SumI4' */
  real_T Sum2;                         /* '<S20>/Sum2' */
  real_T Sum2_n;                       /* '<S21>/Sum2' */
  real_T Sum2_i;                       /* '<S22>/Sum2' */
  real_T Sum2_l;                       /* '<S23>/Sum2' */
  real_T Sum2_f;                       /* '<S24>/Sum2' */
  real_T startTime;                    /* '<S31>/startTime' */
  real_T FilterCoefficient_j;          /* '<S349>/Filter Coefficient' */
  real_T Saturation_f;                 /* '<S353>/Saturation' */
  real_T SumI4_i;                      /* '<S338>/SumI4' */
  real_T FilterCoefficient_m;          /* '<S296>/Filter Coefficient' */
  real_T Saturation_fh;                /* '<S300>/Saturation' */
  real_T SumI4_ic;                     /* '<S285>/SumI4' */
  real_T XDOT[40];                     /* '<S34>/MATLAB Function - MODEL' */
  real_T w[2];                         /* '<S177>/w' */
  real_T w_a[2];                       /* '<S177>/w ' */
  real_T LwgV1[2];                     /* '<S177>/Lwg//V 1' */
  real_T w_g[2];                       /* '<S176>/w' */
  real_T w_e[2];                       /* '<S176>/w ' */
  real_T w1[2];                        /* '<S176>/w 1' */
  real_T w_n[2];                       /* '<S175>/w' */
  real_T w1_c[2];                      /* '<S175>/w1' */
  real_T w_d[2];                       /* '<S174>/w' */
  real_T w_e0[2];                      /* '<S173>/w' */
  real_T UnaryMinus[2];                /* '<S173>/Unary Minus' */
  real_T w_o[2];                       /* '<S172>/w' */
  real_T sigma_w[2];                   /* '<S172>/sigma_w' */
  real_T u1;
  real_T u2;
  real_T q_aero;
  real_T Q;
  real_T u2_deg;
  real_T Ltot;
  real_T CQ;
  real_T Cl;
  real_T Vd1;
  real_T L_dec;
  real_T c_phi;
  real_T s_phi;
  real_T c_the;
  real_T s_the;
  real_T c_psi;
  real_T s_psi;
  real_T sina;
  real_T sinb;
  real_T sinc;
  real_T cosa;
  real_T cosb;
  real_T cosc;
  real_T px_out_l;                  /* '<Root>/MATLAB Function-ned_to_gazebo' */
  real_T SignPreSat;                   /* '<S120>/SignPreSat' */
  real_T IntegratorSecondOrderLimite;
                                  /* '<S24>/Integrator, Second-Order Limited' */
  real_T Square1;                      /* '<S34>/Square1' */
  real_T Sum1;                         /* '<Root>/Sum1' */
  real_T FE_b;
  real_T rtb_CoordinateTransformationC_d;
  real_T rtb_CoordinateTransformationC_l;
  real_T rtb_CoordinateTransformationC_o;
  real_T rtb_CoordinateTransformationC_b;
  real_T FE2_b_idx_0;
  real_T FE2_b_idx_2;
  real_T Fg_b_idx_2;
  real_T Mcg_b_idx_0;
  real_T FE_b_idx_0;
  real_T FA_b_idx_0;
  real_T FA_b_idx_1;
  real_T FA_b_idx_2;
  real_T R_tmp;
  real_T R_tmp_n;
  real_T Switch2;                      /* '<Root>/Switch2' */
  SL_Bus_std_msgs_Float64 SourceBlock_o2_d;/* '<S35>/SourceBlock' */
  uint32_T bpIndex[2];
  uint32_T currentLen;                 /* '<Root>/MATLAB Function-Odometry1' */
  uint32_T receivedLen;                /* '<Root>/MATLAB Function-Odometry1' */
  uint32_T currentLen_b;               /* '<Root>/MATLAB Function-Odometry' */
  uint32_T receivedLen_b;              /* '<Root>/MATLAB Function-Odometry' */
  uint32_T lengthOut;                  /* '<Root>/MATLAB Function-MODELO' */
  uint32_T lengthOut_i;                /* '<Root>/MATLAB Function-IMU2' */
  uint32_T lengthOut_g;                /* '<Root>/MATLAB Function' */
  uint8_T str_out[128];                /* '<Root>/MATLAB Function-Odometry1' */
  uint8_T str_out_e[128];              /* '<Root>/MATLAB Function-Odometry' */
  uint8_T stringOut[128];              /* '<Root>/MATLAB Function-MODELO' */
  uint8_T stringOut_n[128];            /* '<Root>/MATLAB Function-IMU2' */
  uint8_T stringOut_d[128];            /* '<Root>/MATLAB Function' */
  boolean_T Compare;                   /* '<S152>/Compare' */
  boolean_T AND3;                      /* '<S66>/AND3' */
  boolean_T Memory_a;                  /* '<S66>/Memory' */
  boolean_T AND3_c;                    /* '<S120>/AND3' */
  boolean_T Memory_h;                  /* '<S120>/Memory' */
  boolean_T SourceBlock_o1;            /* '<S36>/SourceBlock' */
  boolean_T SourceBlock_o1_o;          /* '<S35>/SourceBlock' */
  boolean_T SourceBlock_o1_h;          /* '<S159>/SourceBlock' */
  boolean_T SourceBlock_o1_d;          /* '<S158>/SourceBlock' */
  boolean_T SourceBlock_o1_c;          /* '<S157>/SourceBlock' */
  boolean_T SourceBlock_o1_k;          /* '<S156>/SourceBlock' */
  B_EnabledSubsystem_pid_cont_d_T EnabledSubsystem_a;/* '<S36>/Enabled Subsystem' */
  B_EnabledSubsystem_pid_cont_d_T EnabledSubsystem_b;/* '<S35>/Enabled Subsystem' */
  B_EnabledSubsystem_pid_contro_T EnabledSubsystem_pt;/* '<S159>/Enabled Subsystem' */
  B_EnabledSubsystem_pid_cont_d_T EnabledSubsystem_p;/* '<S158>/Enabled Subsystem' */
  B_EnabledSubsystem_pid_cont_d_T EnabledSubsystem_k;/* '<S157>/Enabled Subsystem' */
  B_EnabledSubsystem_pid_contro_T EnabledSubsystem;/* '<S156>/Enabled Subsystem' */
  B_MATLABFunctionned_to_PoseSt_T sf_MATLABFunctionned_to_TwistSt;
                              /* '<Root>/MATLAB Function-ned_to_TwistStamped' */
  B_MATLABFunctionned_to_PoseSt_T sf_MATLABFunctionned_to_PoseS_b;
                               /* '<Root>/MATLAB Function-ned_to_PoseStamped' */
};

/* Block states (default storage) for system '<Root>' */
struct DW_pid_control_T {
  ros_slros2_internal_block_Ser_T obj; /* '<S6>/ServiceCaller' */
  ros_slros2_internal_block_Pub_T obj_d;/* '<S30>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_dl;/* '<S29>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_o;/* '<S28>/SinkBlock' */
  ros_slros2_internal_block_Pub_T obj_i;/* '<S27>/SinkBlock' */
  ros_slros2_internal_block_Sub_T obj_k;/* '<S36>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_m;/* '<S35>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_h;/* '<S159>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_p;/* '<S158>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_hy;/* '<S157>/SourceBlock' */
  ros_slros2_internal_block_Sub_T obj_h4;/* '<S156>/SourceBlock' */
  real_T UnitDelay3_DSTATE;            /* '<Root>/Unit Delay3' */
  real_T UnitDelay2_DSTATE;            /* '<Root>/Unit Delay2' */
  real_T Memory2_PreviousInput[12];    /* '<S34>/Memory2' */
  real_T PrevY;                        /* '<Root>/Rate Limiter2' */
  real_T PrevY_g;                      /* '<Root>/Rate Limiter-theta_sp' */
  real_T LastMajorTime;                /* '<Root>/Rate Limiter-theta_sp' */
  real_T NextOutput[4];                /* '<S171>/White Noise' */
  real_T Memory_PreviousInput[3];      /* '<S34>/Memory' */
  real_T Memory1_PreviousInput[3];     /* '<S34>/Memory1' */
  real_T NextOutput_k;                 /* '<S151>/White Noise' */
  uint32_T PreLookUpIndexSearchprobofexcee;
                        /* '<S178>/PreLook-Up Index Search  (prob of exceed)' */
  uint32_T PreLookUpIndexSearchaltitude_DW;
                              /* '<S178>/PreLook-Up Index Search  (altitude)' */
  uint32_T RandSeed[4];                /* '<S171>/White Noise' */
  uint32_T RandSeed_a;                 /* '<S151>/White Noise' */
  robotics_slcore_internal_bloc_T obj_c;
                             /* '<Root>/Coordinate Transformation Conversion' */
  int_T IntegratorSecondOrderLimited_MO;
                                  /* '<S20>/Integrator, Second-Order Limited' */
  int_T IntegratorSecondOrderLimited__c;
                                  /* '<S21>/Integrator, Second-Order Limited' */
  int_T IntegratorSecondOrderLimited__h;
                                  /* '<S22>/Integrator, Second-Order Limited' */
  int_T IntegratorSecondOrderLimited__o;
                                  /* '<S23>/Integrator, Second-Order Limited' */
  int_T IntegratorSecondOrderLimited__j;
                                  /* '<S24>/Integrator, Second-Order Limited' */
  int8_T ifHeightMaxlowaltitudeelseifHei;
  /* '<S166>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  int8_T ifHeightMaxlowaltitudeelseifH_k;
  /* '<S167>/if Height < Max low altitude  elseif Height > Min isotropic altitude ' */
  boolean_T IC_FirstOutputTime;        /* '<S34>/IC' */
  boolean_T Integrator_DWORK1;         /* '<S34>/Integrator' */
  boolean_T RelayEnableFlight_Mode;    /* '<Root>/Relay-Enable-Flight' */
  boolean_T PrevLimited;               /* '<Root>/Rate Limiter-theta_sp' */
  boolean_T Memory_PreviousInput_o;    /* '<S66>/Memory' */
  boolean_T Memory_PreviousInput_a;    /* '<S120>/Memory' */
  boolean_T IntegratorSecondOrderLimited_DW;
                                  /* '<S20>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__l;
                                  /* '<S21>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited_cm;
                                  /* '<S22>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__k;
                                  /* '<S23>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited_lc;
                                  /* '<S24>/Integrator, Second-Order Limited' */
  boolean_T objisempty;                /* '<S36>/SourceBlock' */
  boolean_T objisempty_g;              /* '<S35>/SourceBlock' */
  boolean_T objisempty_a;              /* '<S159>/SourceBlock' */
  boolean_T objisempty_e;              /* '<S158>/SourceBlock' */
  boolean_T objisempty_l;              /* '<S157>/SourceBlock' */
  boolean_T objisempty_c;              /* '<S156>/SourceBlock' */
  boolean_T objisempty_cr;             /* '<S30>/SinkBlock' */
  boolean_T objisempty_ld;             /* '<S29>/SinkBlock' */
  boolean_T objisempty_ev;             /* '<S28>/SinkBlock' */
  boolean_T objisempty_m;              /* '<S27>/SinkBlock' */
  boolean_T objisempty_d;    /* '<Root>/Coordinate Transformation Conversion' */
  boolean_T regObj_not_empty;/* '<Root>/Coordinate Transformation Conversion' */
  boolean_T objisempty_f;              /* '<S6>/ServiceCaller' */
  e_robotics_core_internal_vali_T regObj;
                             /* '<Root>/Coordinate Transformation Conversion' */
  boolean_T Subsystem2_MODE;           /* '<Root>/Subsystem2' */
  boolean_T Subsystem1_MODE;           /* '<Root>/Subsystem1' */
  boolean_T Hwgws_MODE;                /* '<S162>/Hwgw(s)' */
  boolean_T Hvgws_MODE;                /* '<S162>/Hvgw(s)' */
  boolean_T Hugws_MODE;                /* '<S162>/Hugw(s)' */
  boolean_T Hrgw_MODE;                 /* '<S161>/Hrgw' */
  boolean_T Hqgw_MODE;                 /* '<S161>/Hqgw' */
  boolean_T Hpgw_MODE;                 /* '<S161>/Hpgw' */
};

/* Continuous states (default storage) */
struct X_pid_control_T {
  real_T Integrator_CSTATE[12];        /* '<S34>/Integrator' */
  real_T Integrator_CSTATE_f;          /* '<S130>/Integrator' */
  real_T Filter_CSTATE;                /* '<S125>/Filter' */
  real_T Integrator_CSTATE_d;          /* '<S240>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S235>/Filter' */
  real_T Integrator_CSTATE_m;          /* '<S76>/Integrator' */
  real_T Filter_CSTATE_g;              /* '<S71>/Filter' */
  real_T TransferFcn1_CSTATE;          /* '<S34>/Transfer Fcn1' */
  real_T TransferFcn_CSTATE[2];        /* '<S34>/Transfer Fcn' */
  real_T Integrator1_CSTATE;           /* '<S34>/Integrator1' */
  real_T IntegratorSecondOrderLimited_CS[2];
                                  /* '<S20>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__k[2];
                                  /* '<S21>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__b[2];
                                  /* '<S22>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__l[2];
                                  /* '<S23>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__e[2];
                                  /* '<S24>/Integrator, Second-Order Limited' */
  real_T Integrator_CSTATE_n;          /* '<S346>/Integrator' */
  real_T Filter_CSTATE_fb;             /* '<S341>/Filter' */
  real_T Integrator_CSTATE_p;          /* '<S293>/Integrator' */
  real_T Filter_CSTATE_m;              /* '<S288>/Filter' */
  real_T wg_p1_CSTATE[2];              /* '<S177>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S177>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S176>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S176>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S175>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S174>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S173>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S172>/pgw_p' */
};

/* State derivatives (default storage) */
struct XDot_pid_control_T {
  real_T Integrator_CSTATE[12];        /* '<S34>/Integrator' */
  real_T Integrator_CSTATE_f;          /* '<S130>/Integrator' */
  real_T Filter_CSTATE;                /* '<S125>/Filter' */
  real_T Integrator_CSTATE_d;          /* '<S240>/Integrator' */
  real_T Filter_CSTATE_f;              /* '<S235>/Filter' */
  real_T Integrator_CSTATE_m;          /* '<S76>/Integrator' */
  real_T Filter_CSTATE_g;              /* '<S71>/Filter' */
  real_T TransferFcn1_CSTATE;          /* '<S34>/Transfer Fcn1' */
  real_T TransferFcn_CSTATE[2];        /* '<S34>/Transfer Fcn' */
  real_T Integrator1_CSTATE;           /* '<S34>/Integrator1' */
  real_T IntegratorSecondOrderLimited_CS[2];
                                  /* '<S20>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__k[2];
                                  /* '<S21>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__b[2];
                                  /* '<S22>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__l[2];
                                  /* '<S23>/Integrator, Second-Order Limited' */
  real_T IntegratorSecondOrderLimited__e[2];
                                  /* '<S24>/Integrator, Second-Order Limited' */
  real_T Integrator_CSTATE_n;          /* '<S346>/Integrator' */
  real_T Filter_CSTATE_fb;             /* '<S341>/Filter' */
  real_T Integrator_CSTATE_p;          /* '<S293>/Integrator' */
  real_T Filter_CSTATE_m;              /* '<S288>/Filter' */
  real_T wg_p1_CSTATE[2];              /* '<S177>/wg_p1' */
  real_T wg_p2_CSTATE[2];              /* '<S177>/wg_p2' */
  real_T vg_p1_CSTATE[2];              /* '<S176>/vg_p1' */
  real_T vgw_p2_CSTATE[2];             /* '<S176>/vgw_p2' */
  real_T ug_p_CSTATE[2];               /* '<S175>/ug_p' */
  real_T rgw_p_CSTATE[2];              /* '<S174>/rgw_p' */
  real_T qgw_p_CSTATE[2];              /* '<S173>/qgw_p' */
  real_T pgw_p_CSTATE[2];              /* '<S172>/pgw_p' */
};

/* State disabled  */
struct XDis_pid_control_T {
  boolean_T Integrator_CSTATE[12];     /* '<S34>/Integrator' */
  boolean_T Integrator_CSTATE_f;       /* '<S130>/Integrator' */
  boolean_T Filter_CSTATE;             /* '<S125>/Filter' */
  boolean_T Integrator_CSTATE_d;       /* '<S240>/Integrator' */
  boolean_T Filter_CSTATE_f;           /* '<S235>/Filter' */
  boolean_T Integrator_CSTATE_m;       /* '<S76>/Integrator' */
  boolean_T Filter_CSTATE_g;           /* '<S71>/Filter' */
  boolean_T TransferFcn1_CSTATE;       /* '<S34>/Transfer Fcn1' */
  boolean_T TransferFcn_CSTATE[2];     /* '<S34>/Transfer Fcn' */
  boolean_T Integrator1_CSTATE;        /* '<S34>/Integrator1' */
  boolean_T IntegratorSecondOrderLimited_CS[2];
                                  /* '<S20>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__k[2];
                                  /* '<S21>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__b[2];
                                  /* '<S22>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__l[2];
                                  /* '<S23>/Integrator, Second-Order Limited' */
  boolean_T IntegratorSecondOrderLimited__e[2];
                                  /* '<S24>/Integrator, Second-Order Limited' */
  boolean_T Integrator_CSTATE_n;       /* '<S346>/Integrator' */
  boolean_T Filter_CSTATE_fb;          /* '<S341>/Filter' */
  boolean_T Integrator_CSTATE_p;       /* '<S293>/Integrator' */
  boolean_T Filter_CSTATE_m;           /* '<S288>/Filter' */
  boolean_T wg_p1_CSTATE[2];           /* '<S177>/wg_p1' */
  boolean_T wg_p2_CSTATE[2];           /* '<S177>/wg_p2' */
  boolean_T vg_p1_CSTATE[2];           /* '<S176>/vg_p1' */
  boolean_T vgw_p2_CSTATE[2];          /* '<S176>/vgw_p2' */
  boolean_T ug_p_CSTATE[2];            /* '<S175>/ug_p' */
  boolean_T rgw_p_CSTATE[2];           /* '<S174>/rgw_p' */
  boolean_T qgw_p_CSTATE[2];           /* '<S173>/qgw_p' */
  boolean_T pgw_p_CSTATE[2];           /* '<S172>/pgw_p' */
};

/* Zero-crossing (trigger) state */
struct PrevZCX_pid_control_T {
  ZCSigState Integrator_Reset_ZCE;     /* '<S34>/Integrator' */
};

/* Invariant block signals (default storage) */
struct ConstB_pid_control_T {
  real_T UnitConversion;               /* '<S160>/Unit Conversion' */
  real_T UnitConversion_k;             /* '<S170>/Unit Conversion' */
  real_T sigma_wg;                     /* '<S179>/sigma_wg ' */
  real_T UnitConversion_n;             /* '<S164>/Unit Conversion' */
  real_T UnitConversion_c;             /* '<S198>/Unit Conversion' */
  real_T PreLookUpIndexSearchprobofe;
                        /* '<S178>/PreLook-Up Index Search  (prob of exceed)' */
  real_T constBlockForifHeightMaxlow;
  real_T constBlockForifHeightMaxl_e;
  real_T constBlockForifHeightMaxl_l;
  real_T constBlockForifHeightMax_eh;
  real_T Sqrt[4];                      /* '<S171>/Sqrt' */
  real_T Sqrt1;                        /* '<S171>/Sqrt1' */
  real_T Divide[4];                    /* '<S171>/Divide' */
  real_T motorspeed;                   /* '<S34>/Gain2' */
  real_T Demandlimits;                 /* '<S20>/Demand limits' */
  real_T Demandlimits_h;               /* '<S21>/Demand limits' */
  real_T Demandlimits_e;               /* '<S22>/Demand limits' */
  real_T Demandlimits_g;               /* '<S23>/Demand limits' */
  real_T Demandlimits_n;               /* '<S24>/Demand limits' */
  real_T Sum;                          /* '<S188>/Sum' */
  real_T Sum_a;                        /* '<S180>/Sum' */
  real_T sqrt_a;                       /* '<S177>/sqrt' */
  real_T w4;                           /* '<S172>/w4' */
  real_T u16;                          /* '<S172>/u^1//6' */
  uint32_T PreLookUpIndexSearchprobo_g;
                        /* '<S178>/PreLook-Up Index Search  (prob of exceed)' */
};

#ifndef ODE4_INTG
#define ODE4_INTG

/* ODE4 Integration Data */
struct ODE4_IntgData {
  real_T *y;                           /* output */
  real_T *f[4];                        /* derivatives */
};

#endif

/* Constant parameters (default storage) */
struct ConstP_pid_control_T {
  /* Pooled Parameter (Expression: x_nom)
   * Referenced by:
   *   '<S34>/IC'
   *   '<S34>/Memory2'
   */
  real_T pooled17[12];

  /* Expression: rep_seq_y
   * Referenced by: '<S33>/Look-Up Table1'
   */
  real_T LookUpTable1_tableData[17];

  /* Expression: rep_seq_t - min(rep_seq_t)
   * Referenced by: '<S33>/Look-Up Table1'
   */
  real_T LookUpTable1_bp01Data[17];

  /* Expression: h_vec
   * Referenced by: '<S178>/PreLook-Up Index Search  (altitude)'
   */
  real_T PreLookUpIndexSearchaltitude_Br[12];

  /* Expression: sigma_vec'
   * Referenced by: '<S178>/Medium//High Altitude Intensity'
   */
  real_T MediumHighAltitudeIntensity_Tab[84];

  /* Computed Parameter: MediumHighAltitudeIntensity_max
   * Referenced by: '<S178>/Medium//High Altitude Intensity'
   */
  uint32_T MediumHighAltitudeIntensity_max[2];
};

/* Real-time Model Data Structure */
struct tag_RTM_pid_control_T {
  const char_T *errorStatus;
  RTWSolverInfo solverInfo;
  X_pid_control_T *contStates;
  int_T *periodicContStateIndices;
  real_T *periodicContStateRanges;
  real_T *derivs;
  XDis_pid_control_T *contStateDisabled;
  boolean_T zCCacheNeedsReset;
  boolean_T derivCacheNeedsReset;
  boolean_T CTOutputIncnstWithState;
  real_T odeY[52];
  real_T odeF[4][52];
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
    struct {
      uint8_T TID[2];
    } TaskCounters;

    time_T tStart;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

extern const ConstB_pid_control_T pid_control_ConstB;/* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP_pid_control_T pid_control_ConstP;

/* Class declaration for model pid_control */
class pid_control
{
  /* public data and function members */
 public:
  /* Real-Time Model get method */
  RT_MODEL_pid_control_T * getRTM();
  void ModelPrevZCStateInit();

  /* model start function */
  void start();

  /* Initial conditions function */
  void initialize();

  /* model step function */
  void step();

  /* model terminate function */
  void terminate();

  /* Constructor */
  pid_control();

  /* Destructor */
  ~pid_control();

  /* private data and function members */
 private:
  /* Block signals */
  B_pid_control_T pid_control_B;

  /* Block states */
  DW_pid_control_T pid_control_DW;

  /* Block continuous states */
  X_pid_control_T pid_control_X;

  /* Block Continuous state disabled vector */
  XDis_pid_control_T pid_control_XDis;

  /* Triggered events */
  PrevZCX_pid_control_T pid_control_PrevZCX;

  /* private member function(s) for subsystem '<Root>/MATLAB Function-ned_to_PoseStamped'*/
  static void MATLABFunctionned_to_PoseStampe(B_MATLABFunctionned_to_PoseSt_T
    *localB);

  /* private member function(s) for subsystem '<S156>/Enabled Subsystem'*/
  static void pid_contr_EnabledSubsystem_Init(B_EnabledSubsystem_pid_contro_T
    *localB);
  static void pid_control_EnabledSubsystem(boolean_T rtu_Enable, const
    SL_Bus_std_msgs_Bool *rtu_In1, B_EnabledSubsystem_pid_contro_T *localB);

  /* private member function(s) for subsystem '<S157>/Enabled Subsystem'*/
  static void pid_con_EnabledSubsystem_p_Init(B_EnabledSubsystem_pid_cont_d_T
    *localB);
  static void pid_control_EnabledSubsystem_k(boolean_T rtu_Enable, const
    SL_Bus_std_msgs_Float64 *rtu_In1, B_EnabledSubsystem_pid_cont_d_T *localB);

  /* private member function(s) for subsystem '<Root>'*/
  void pid_control_Publisher_setupImpl(const ros_slros2_internal_block_Pub_T
    *obj);
  rmw_qos_profile_t pi_ROS2PubSubBase_setQOSProfile(rmw_qos_profile_t rmwProfile,
    real_T qosDepth, real_T qosDeadline, real_T qosLifespan, real_T
    qosLeaseDuration, boolean_T qosAvoidROSNamespaceConventions);
  void pid_contr_Publisher_setupImpl_m(const ros_slros2_internal_block_Pub_T
    *obj);
  void pid_cont_Publisher_setupImpl_m3(const ros_slros2_internal_block_Pub_T
    *obj);
  void pid_c_Subscriber_setupImpl_m3hf(const ros_slros2_internal_block_Sub_T
    *obj);
  void pid__Subscriber_setupImpl_m3hfp(const ros_slros2_internal_block_Sub_T
    *obj);
  void pid_con_ServiceCaller_setupImpl(ros_slros2_internal_block_Ser_T *obj);
  void pid_contro_Subscriber_setupImpl(const ros_slros2_internal_block_Sub_T
    *obj);
  void pid_con_Subscriber_setupImpl_m3(const ros_slros2_internal_block_Sub_T
    *obj);
  void pid_co_Subscriber_setupImpl_m3h(const ros_slros2_internal_block_Sub_T
    *obj);
  void pid_cont_Subscriber_setupImpl_m(const ros_slros2_internal_block_Sub_T
    *obj);

  /* Global mass matrix */

  /* Continuous states update member function*/
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  /* Derivatives member function */
  void pid_control_derivatives();

  /* Real-Time Model */
  RT_MODEL_pid_control_T pid_control_M;
};

extern volatile boolean_T stopRequested;
extern volatile boolean_T runModel;

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<Root>/Display' : Unused code path elimination
 * Block '<S148>/Data Type Propagation' : Unused code path elimination
 * Block '<S149>/FixPt Constant' : Unused code path elimination
 * Block '<S149>/FixPt Data Type Duplicate' : Unused code path elimination
 * Block '<S149>/FixPt Sum1' : Unused code path elimination
 * Block '<S148>/Output' : Unused code path elimination
 * Block '<S150>/Constant' : Unused code path elimination
 * Block '<S150>/FixPt Data Type Duplicate1' : Unused code path elimination
 * Block '<S150>/FixPt Switch' : Unused code path elimination
 * Block '<S32>/Out' : Unused code path elimination
 * Block '<S32>/Output' : Unused code path elimination
 * Block '<S32>/Vector' : Unused code path elimination
 * Block '<S34>/Gain4' : Unused code path elimination
 * Block '<S156>/OnNewReqCall' : Unused code path elimination
 * Block '<S157>/OnNewReqCall' : Unused code path elimination
 * Block '<S158>/OnNewReqCall' : Unused code path elimination
 * Block '<S159>/OnNewReqCall' : Unused code path elimination
 * Block '<S35>/OnNewReqCall' : Unused code path elimination
 * Block '<S36>/OnNewReqCall' : Unused code path elimination
 * Block '<Root>/Gain1' : Eliminated nontunable gain of 1
 * Block '<Root>/Gain5' : Eliminated nontunable gain of 1
 * Block '<Root>/Manual Switch' : Eliminated due to constant selection input
 * Block '<Root>/Manual Switch1' : Eliminated due to constant selection input
 * Block '<S20>/wn^2' : Eliminated nontunable gain of 1
 * Block '<S21>/wn^2' : Eliminated nontunable gain of 1
 * Block '<S22>/wn^2' : Eliminated nontunable gain of 1
 * Block '<S23>/wn^2' : Eliminated nontunable gain of 1
 * Block '<S24>/wn^2' : Eliminated nontunable gain of 1
 * Block '<S31>/Output' : Eliminate redundant signal conversion block
 * Block '<S33>/Output' : Eliminate redundant signal conversion block
 * Block '<S153>/Cast' : Eliminate redundant data type conversion
 * Block '<S153>/Cast To Double' : Eliminate redundant data type conversion
 * Block '<S153>/Cast To Double1' : Eliminate redundant data type conversion
 * Block '<S153>/Cast To Double2' : Eliminate redundant data type conversion
 * Block '<S153>/Cast To Double3' : Eliminate redundant data type conversion
 * Block '<S153>/Cast To Double4' : Eliminate redundant data type conversion
 * Block '<Root>/Constant_U' : Unused code path elimination
 * Block '<S31>/Clock' : Unused code path elimination
 * Block '<S31>/Constant' : Unused code path elimination
 * Block '<S31>/Look-Up Table1' : Unused code path elimination
 * Block '<S31>/Math Function' : Unused code path elimination
 * Block '<S31>/Sum' : Unused code path elimination
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
 * '<Root>' : 'pid_control'
 * '<S1>'   : 'pid_control/Blank Message'
 * '<S2>'   : 'pid_control/Blank Message1'
 * '<S3>'   : 'pid_control/Blank Message2'
 * '<S4>'   : 'pid_control/Blank Message3'
 * '<S5>'   : 'pid_control/Blank Message4'
 * '<S6>'   : 'pid_control/Call Service'
 * '<S7>'   : 'pid_control/MATLAB Function'
 * '<S8>'   : 'pid_control/MATLAB Function-IMU2'
 * '<S9>'   : 'pid_control/MATLAB Function-MODELO'
 * '<S10>'  : 'pid_control/MATLAB Function-Odometry'
 * '<S11>'  : 'pid_control/MATLAB Function-Odometry1'
 * '<S12>'  : 'pid_control/MATLAB Function-clocl'
 * '<S13>'  : 'pid_control/MATLAB Function-ned_to_PoseStaamped'
 * '<S14>'  : 'pid_control/MATLAB Function-ned_to_PoseStamped'
 * '<S15>'  : 'pid_control/MATLAB Function-ned_to_TwistStamped'
 * '<S16>'  : 'pid_control/MATLAB Function-ned_to_gazebo'
 * '<S17>'  : 'pid_control/MATLAB Function-ned_to_imu'
 * '<S18>'  : 'pid_control/MATLAB Function-ned_to_odometry'
 * '<S19>'  : 'pid_control/MATLAB Function2'
 * '<S20>'  : 'pid_control/Nonlinear Second-Order Actuator'
 * '<S21>'  : 'pid_control/Nonlinear Second-Order Actuator1'
 * '<S22>'  : 'pid_control/Nonlinear Second-Order Actuator2'
 * '<S23>'  : 'pid_control/Nonlinear Second-Order Actuator3'
 * '<S24>'  : 'pid_control/Nonlinear Second-Order Actuator4'
 * '<S25>'  : 'pid_control/PID ALERON'
 * '<S26>'  : 'pid_control/PID VELOCIDAD'
 * '<S27>'  : 'pid_control/Publish'
 * '<S28>'  : 'pid_control/Publish1'
 * '<S29>'  : 'pid_control/Publish2'
 * '<S30>'  : 'pid_control/Publish3'
 * '<S31>'  : 'pid_control/Repeating Sequence'
 * '<S32>'  : 'pid_control/Repeating Sequence Stair'
 * '<S33>'  : 'pid_control/Repeating Sequence1'
 * '<S34>'  : 'pid_control/SUBSYSTEM_MODEL'
 * '<S35>'  : 'pid_control/Subscribe-ALTURA'
 * '<S36>'  : 'pid_control/Subscribe-YAW'
 * '<S37>'  : 'pid_control/Subsystem'
 * '<S38>'  : 'pid_control/Subsystem1'
 * '<S39>'  : 'pid_control/Subsystem2'
 * '<S40>'  : 'pid_control/PID ALERON/Anti-windup'
 * '<S41>'  : 'pid_control/PID ALERON/D Gain'
 * '<S42>'  : 'pid_control/PID ALERON/External Derivative'
 * '<S43>'  : 'pid_control/PID ALERON/Filter'
 * '<S44>'  : 'pid_control/PID ALERON/Filter ICs'
 * '<S45>'  : 'pid_control/PID ALERON/I Gain'
 * '<S46>'  : 'pid_control/PID ALERON/Ideal P Gain'
 * '<S47>'  : 'pid_control/PID ALERON/Ideal P Gain Fdbk'
 * '<S48>'  : 'pid_control/PID ALERON/Integrator'
 * '<S49>'  : 'pid_control/PID ALERON/Integrator ICs'
 * '<S50>'  : 'pid_control/PID ALERON/N Copy'
 * '<S51>'  : 'pid_control/PID ALERON/N Gain'
 * '<S52>'  : 'pid_control/PID ALERON/P Copy'
 * '<S53>'  : 'pid_control/PID ALERON/Parallel P Gain'
 * '<S54>'  : 'pid_control/PID ALERON/Reset Signal'
 * '<S55>'  : 'pid_control/PID ALERON/Saturation'
 * '<S56>'  : 'pid_control/PID ALERON/Saturation Fdbk'
 * '<S57>'  : 'pid_control/PID ALERON/Sum'
 * '<S58>'  : 'pid_control/PID ALERON/Sum Fdbk'
 * '<S59>'  : 'pid_control/PID ALERON/Tracking Mode'
 * '<S60>'  : 'pid_control/PID ALERON/Tracking Mode Sum'
 * '<S61>'  : 'pid_control/PID ALERON/Tsamp - Integral'
 * '<S62>'  : 'pid_control/PID ALERON/Tsamp - Ngain'
 * '<S63>'  : 'pid_control/PID ALERON/postSat Signal'
 * '<S64>'  : 'pid_control/PID ALERON/preInt Signal'
 * '<S65>'  : 'pid_control/PID ALERON/preSat Signal'
 * '<S66>'  : 'pid_control/PID ALERON/Anti-windup/Cont. Clamping Parallel'
 * '<S67>'  : 'pid_control/PID ALERON/Anti-windup/Cont. Clamping Parallel/Dead Zone'
 * '<S68>'  : 'pid_control/PID ALERON/Anti-windup/Cont. Clamping Parallel/Dead Zone/Enabled'
 * '<S69>'  : 'pid_control/PID ALERON/D Gain/Internal Parameters'
 * '<S70>'  : 'pid_control/PID ALERON/External Derivative/Error'
 * '<S71>'  : 'pid_control/PID ALERON/Filter/Cont. Filter'
 * '<S72>'  : 'pid_control/PID ALERON/Filter ICs/Internal IC - Filter'
 * '<S73>'  : 'pid_control/PID ALERON/I Gain/Internal Parameters'
 * '<S74>'  : 'pid_control/PID ALERON/Ideal P Gain/Passthrough'
 * '<S75>'  : 'pid_control/PID ALERON/Ideal P Gain Fdbk/Disabled'
 * '<S76>'  : 'pid_control/PID ALERON/Integrator/Continuous'
 * '<S77>'  : 'pid_control/PID ALERON/Integrator ICs/Internal IC'
 * '<S78>'  : 'pid_control/PID ALERON/N Copy/Disabled'
 * '<S79>'  : 'pid_control/PID ALERON/N Gain/Internal Parameters'
 * '<S80>'  : 'pid_control/PID ALERON/P Copy/Disabled'
 * '<S81>'  : 'pid_control/PID ALERON/Parallel P Gain/Internal Parameters'
 * '<S82>'  : 'pid_control/PID ALERON/Reset Signal/Disabled'
 * '<S83>'  : 'pid_control/PID ALERON/Saturation/Enabled'
 * '<S84>'  : 'pid_control/PID ALERON/Saturation Fdbk/Disabled'
 * '<S85>'  : 'pid_control/PID ALERON/Sum/Sum_PID'
 * '<S86>'  : 'pid_control/PID ALERON/Sum Fdbk/Disabled'
 * '<S87>'  : 'pid_control/PID ALERON/Tracking Mode/Disabled'
 * '<S88>'  : 'pid_control/PID ALERON/Tracking Mode Sum/Passthrough'
 * '<S89>'  : 'pid_control/PID ALERON/Tsamp - Integral/TsSignalSpecification'
 * '<S90>'  : 'pid_control/PID ALERON/Tsamp - Ngain/Passthrough'
 * '<S91>'  : 'pid_control/PID ALERON/postSat Signal/Forward_Path'
 * '<S92>'  : 'pid_control/PID ALERON/preInt Signal/Internal PreInt'
 * '<S93>'  : 'pid_control/PID ALERON/preSat Signal/Forward_Path'
 * '<S94>'  : 'pid_control/PID VELOCIDAD/Anti-windup'
 * '<S95>'  : 'pid_control/PID VELOCIDAD/D Gain'
 * '<S96>'  : 'pid_control/PID VELOCIDAD/External Derivative'
 * '<S97>'  : 'pid_control/PID VELOCIDAD/Filter'
 * '<S98>'  : 'pid_control/PID VELOCIDAD/Filter ICs'
 * '<S99>'  : 'pid_control/PID VELOCIDAD/I Gain'
 * '<S100>' : 'pid_control/PID VELOCIDAD/Ideal P Gain'
 * '<S101>' : 'pid_control/PID VELOCIDAD/Ideal P Gain Fdbk'
 * '<S102>' : 'pid_control/PID VELOCIDAD/Integrator'
 * '<S103>' : 'pid_control/PID VELOCIDAD/Integrator ICs'
 * '<S104>' : 'pid_control/PID VELOCIDAD/N Copy'
 * '<S105>' : 'pid_control/PID VELOCIDAD/N Gain'
 * '<S106>' : 'pid_control/PID VELOCIDAD/P Copy'
 * '<S107>' : 'pid_control/PID VELOCIDAD/Parallel P Gain'
 * '<S108>' : 'pid_control/PID VELOCIDAD/Reset Signal'
 * '<S109>' : 'pid_control/PID VELOCIDAD/Saturation'
 * '<S110>' : 'pid_control/PID VELOCIDAD/Saturation Fdbk'
 * '<S111>' : 'pid_control/PID VELOCIDAD/Sum'
 * '<S112>' : 'pid_control/PID VELOCIDAD/Sum Fdbk'
 * '<S113>' : 'pid_control/PID VELOCIDAD/Tracking Mode'
 * '<S114>' : 'pid_control/PID VELOCIDAD/Tracking Mode Sum'
 * '<S115>' : 'pid_control/PID VELOCIDAD/Tsamp - Integral'
 * '<S116>' : 'pid_control/PID VELOCIDAD/Tsamp - Ngain'
 * '<S117>' : 'pid_control/PID VELOCIDAD/postSat Signal'
 * '<S118>' : 'pid_control/PID VELOCIDAD/preInt Signal'
 * '<S119>' : 'pid_control/PID VELOCIDAD/preSat Signal'
 * '<S120>' : 'pid_control/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel'
 * '<S121>' : 'pid_control/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel/Dead Zone'
 * '<S122>' : 'pid_control/PID VELOCIDAD/Anti-windup/Cont. Clamping Parallel/Dead Zone/Enabled'
 * '<S123>' : 'pid_control/PID VELOCIDAD/D Gain/Internal Parameters'
 * '<S124>' : 'pid_control/PID VELOCIDAD/External Derivative/Error'
 * '<S125>' : 'pid_control/PID VELOCIDAD/Filter/Cont. Filter'
 * '<S126>' : 'pid_control/PID VELOCIDAD/Filter ICs/Internal IC - Filter'
 * '<S127>' : 'pid_control/PID VELOCIDAD/I Gain/Internal Parameters'
 * '<S128>' : 'pid_control/PID VELOCIDAD/Ideal P Gain/Passthrough'
 * '<S129>' : 'pid_control/PID VELOCIDAD/Ideal P Gain Fdbk/Disabled'
 * '<S130>' : 'pid_control/PID VELOCIDAD/Integrator/Continuous'
 * '<S131>' : 'pid_control/PID VELOCIDAD/Integrator ICs/Internal IC'
 * '<S132>' : 'pid_control/PID VELOCIDAD/N Copy/Disabled'
 * '<S133>' : 'pid_control/PID VELOCIDAD/N Gain/Internal Parameters'
 * '<S134>' : 'pid_control/PID VELOCIDAD/P Copy/Disabled'
 * '<S135>' : 'pid_control/PID VELOCIDAD/Parallel P Gain/Internal Parameters'
 * '<S136>' : 'pid_control/PID VELOCIDAD/Reset Signal/Disabled'
 * '<S137>' : 'pid_control/PID VELOCIDAD/Saturation/Enabled'
 * '<S138>' : 'pid_control/PID VELOCIDAD/Saturation Fdbk/Disabled'
 * '<S139>' : 'pid_control/PID VELOCIDAD/Sum/Sum_PID'
 * '<S140>' : 'pid_control/PID VELOCIDAD/Sum Fdbk/Disabled'
 * '<S141>' : 'pid_control/PID VELOCIDAD/Tracking Mode/Disabled'
 * '<S142>' : 'pid_control/PID VELOCIDAD/Tracking Mode Sum/Passthrough'
 * '<S143>' : 'pid_control/PID VELOCIDAD/Tsamp - Integral/TsSignalSpecification'
 * '<S144>' : 'pid_control/PID VELOCIDAD/Tsamp - Ngain/Passthrough'
 * '<S145>' : 'pid_control/PID VELOCIDAD/postSat Signal/Forward_Path'
 * '<S146>' : 'pid_control/PID VELOCIDAD/preInt Signal/Internal PreInt'
 * '<S147>' : 'pid_control/PID VELOCIDAD/preSat Signal/Forward_Path'
 * '<S148>' : 'pid_control/Repeating Sequence Stair/LimitedCounter'
 * '<S149>' : 'pid_control/Repeating Sequence Stair/LimitedCounter/Increment Real World'
 * '<S150>' : 'pid_control/Repeating Sequence Stair/LimitedCounter/Wrap To Zero'
 * '<S151>' : 'pid_control/SUBSYSTEM_MODEL/Band-Limited White Noise'
 * '<S152>' : 'pid_control/SUBSYSTEM_MODEL/Compare To Constant'
 * '<S153>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))'
 * '<S154>' : 'pid_control/SUBSYSTEM_MODEL/MATLAB Function - MODEL'
 * '<S155>' : 'pid_control/SUBSYSTEM_MODEL/MATLAB Function-reset'
 * '<S156>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe1_TURBULENCIA1'
 * '<S157>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_HEAVE'
 * '<S158>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_RATE'
 * '<S159>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_TURBULENCIA'
 * '<S160>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Angle Conversion'
 * '<S161>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates'
 * '<S162>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities'
 * '<S163>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Length Conversion'
 * '<S164>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Length Conversion1'
 * '<S165>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities'
 * '<S166>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates'
 * '<S167>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities'
 * '<S168>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths'
 * '<S169>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Velocity Conversion'
 * '<S170>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Velocity Conversion2'
 * '<S171>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/White Noise'
 * '<S172>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hpgw'
 * '<S173>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hqgw'
 * '<S174>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on angular rates/Hrgw'
 * '<S175>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hugw(s)'
 * '<S176>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hvgw(s)'
 * '<S177>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Filters on velocities/Hwgw(s)'
 * '<S178>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities/High Altitude Intensity'
 * '<S179>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/RMS turbulence  intensities/Low Altitude Intensity'
 * '<S180>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates'
 * '<S181>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates'
 * '<S182>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Medium//High  altitude rates'
 * '<S183>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Merge Subsystems'
 * '<S184>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates/wind to body transformation'
 * '<S185>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Interpolate  rates/wind to body transformation/convert to earth coords'
 * '<S186>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates/wind to body transformation'
 * '<S187>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select angular rates/Low altitude  rates/wind to body transformation/convert to earth coords'
 * '<S188>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities'
 * '<S189>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities'
 * '<S190>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Medium//High  altitude velocities'
 * '<S191>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Merge Subsystems'
 * '<S192>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities/wind to body transformation'
 * '<S193>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Interpolate  velocities/wind to body transformation/convert to earth coords'
 * '<S194>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities/wind to body transformation'
 * '<S195>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Select velocities/Low altitude  velocities/wind to body transformation/convert to earth coords'
 * '<S196>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Low altitude scale length'
 * '<S197>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Medium//High altitude scale length'
 * '<S198>' : 'pid_control/SUBSYSTEM_MODEL/Dryden Wind Turbulence Model  (Continuous (-q +r))/Turbulence scale lengths/Medium//High altitude scale length/Length Conversion'
 * '<S199>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe1_TURBULENCIA1/Enabled Subsystem'
 * '<S200>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_HEAVE/Enabled Subsystem'
 * '<S201>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_RATE/Enabled Subsystem'
 * '<S202>' : 'pid_control/SUBSYSTEM_MODEL/Subscribe_TURBULENCIA/Enabled Subsystem'
 * '<S203>' : 'pid_control/Subscribe-ALTURA/Enabled Subsystem'
 * '<S204>' : 'pid_control/Subscribe-YAW/Enabled Subsystem'
 * '<S205>' : 'pid_control/Subsystem/PID TIIMON'
 * '<S206>' : 'pid_control/Subsystem/PID TIIMON/Anti-windup'
 * '<S207>' : 'pid_control/Subsystem/PID TIIMON/D Gain'
 * '<S208>' : 'pid_control/Subsystem/PID TIIMON/External Derivative'
 * '<S209>' : 'pid_control/Subsystem/PID TIIMON/Filter'
 * '<S210>' : 'pid_control/Subsystem/PID TIIMON/Filter ICs'
 * '<S211>' : 'pid_control/Subsystem/PID TIIMON/I Gain'
 * '<S212>' : 'pid_control/Subsystem/PID TIIMON/Ideal P Gain'
 * '<S213>' : 'pid_control/Subsystem/PID TIIMON/Ideal P Gain Fdbk'
 * '<S214>' : 'pid_control/Subsystem/PID TIIMON/Integrator'
 * '<S215>' : 'pid_control/Subsystem/PID TIIMON/Integrator ICs'
 * '<S216>' : 'pid_control/Subsystem/PID TIIMON/N Copy'
 * '<S217>' : 'pid_control/Subsystem/PID TIIMON/N Gain'
 * '<S218>' : 'pid_control/Subsystem/PID TIIMON/P Copy'
 * '<S219>' : 'pid_control/Subsystem/PID TIIMON/Parallel P Gain'
 * '<S220>' : 'pid_control/Subsystem/PID TIIMON/Reset Signal'
 * '<S221>' : 'pid_control/Subsystem/PID TIIMON/Saturation'
 * '<S222>' : 'pid_control/Subsystem/PID TIIMON/Saturation Fdbk'
 * '<S223>' : 'pid_control/Subsystem/PID TIIMON/Sum'
 * '<S224>' : 'pid_control/Subsystem/PID TIIMON/Sum Fdbk'
 * '<S225>' : 'pid_control/Subsystem/PID TIIMON/Tracking Mode'
 * '<S226>' : 'pid_control/Subsystem/PID TIIMON/Tracking Mode Sum'
 * '<S227>' : 'pid_control/Subsystem/PID TIIMON/Tsamp - Integral'
 * '<S228>' : 'pid_control/Subsystem/PID TIIMON/Tsamp - Ngain'
 * '<S229>' : 'pid_control/Subsystem/PID TIIMON/postSat Signal'
 * '<S230>' : 'pid_control/Subsystem/PID TIIMON/preInt Signal'
 * '<S231>' : 'pid_control/Subsystem/PID TIIMON/preSat Signal'
 * '<S232>' : 'pid_control/Subsystem/PID TIIMON/Anti-windup/Back Calculation'
 * '<S233>' : 'pid_control/Subsystem/PID TIIMON/D Gain/Internal Parameters'
 * '<S234>' : 'pid_control/Subsystem/PID TIIMON/External Derivative/Error'
 * '<S235>' : 'pid_control/Subsystem/PID TIIMON/Filter/Cont. Filter'
 * '<S236>' : 'pid_control/Subsystem/PID TIIMON/Filter ICs/Internal IC - Filter'
 * '<S237>' : 'pid_control/Subsystem/PID TIIMON/I Gain/Internal Parameters'
 * '<S238>' : 'pid_control/Subsystem/PID TIIMON/Ideal P Gain/Passthrough'
 * '<S239>' : 'pid_control/Subsystem/PID TIIMON/Ideal P Gain Fdbk/Disabled'
 * '<S240>' : 'pid_control/Subsystem/PID TIIMON/Integrator/Continuous'
 * '<S241>' : 'pid_control/Subsystem/PID TIIMON/Integrator ICs/Internal IC'
 * '<S242>' : 'pid_control/Subsystem/PID TIIMON/N Copy/Disabled'
 * '<S243>' : 'pid_control/Subsystem/PID TIIMON/N Gain/Internal Parameters'
 * '<S244>' : 'pid_control/Subsystem/PID TIIMON/P Copy/Disabled'
 * '<S245>' : 'pid_control/Subsystem/PID TIIMON/Parallel P Gain/Internal Parameters'
 * '<S246>' : 'pid_control/Subsystem/PID TIIMON/Reset Signal/Disabled'
 * '<S247>' : 'pid_control/Subsystem/PID TIIMON/Saturation/Enabled'
 * '<S248>' : 'pid_control/Subsystem/PID TIIMON/Saturation Fdbk/Disabled'
 * '<S249>' : 'pid_control/Subsystem/PID TIIMON/Sum/Sum_PID'
 * '<S250>' : 'pid_control/Subsystem/PID TIIMON/Sum Fdbk/Disabled'
 * '<S251>' : 'pid_control/Subsystem/PID TIIMON/Tracking Mode/Disabled'
 * '<S252>' : 'pid_control/Subsystem/PID TIIMON/Tracking Mode Sum/Passthrough'
 * '<S253>' : 'pid_control/Subsystem/PID TIIMON/Tsamp - Integral/TsSignalSpecification'
 * '<S254>' : 'pid_control/Subsystem/PID TIIMON/Tsamp - Ngain/Passthrough'
 * '<S255>' : 'pid_control/Subsystem/PID TIIMON/postSat Signal/Forward_Path'
 * '<S256>' : 'pid_control/Subsystem/PID TIIMON/preInt Signal/Internal PreInt'
 * '<S257>' : 'pid_control/Subsystem/PID TIIMON/preSat Signal/Forward_Path'
 * '<S258>' : 'pid_control/Subsystem1/PID ELEVATOR'
 * '<S259>' : 'pid_control/Subsystem1/PID ELEVATOR/Anti-windup'
 * '<S260>' : 'pid_control/Subsystem1/PID ELEVATOR/D Gain'
 * '<S261>' : 'pid_control/Subsystem1/PID ELEVATOR/External Derivative'
 * '<S262>' : 'pid_control/Subsystem1/PID ELEVATOR/Filter'
 * '<S263>' : 'pid_control/Subsystem1/PID ELEVATOR/Filter ICs'
 * '<S264>' : 'pid_control/Subsystem1/PID ELEVATOR/I Gain'
 * '<S265>' : 'pid_control/Subsystem1/PID ELEVATOR/Ideal P Gain'
 * '<S266>' : 'pid_control/Subsystem1/PID ELEVATOR/Ideal P Gain Fdbk'
 * '<S267>' : 'pid_control/Subsystem1/PID ELEVATOR/Integrator'
 * '<S268>' : 'pid_control/Subsystem1/PID ELEVATOR/Integrator ICs'
 * '<S269>' : 'pid_control/Subsystem1/PID ELEVATOR/N Copy'
 * '<S270>' : 'pid_control/Subsystem1/PID ELEVATOR/N Gain'
 * '<S271>' : 'pid_control/Subsystem1/PID ELEVATOR/P Copy'
 * '<S272>' : 'pid_control/Subsystem1/PID ELEVATOR/Parallel P Gain'
 * '<S273>' : 'pid_control/Subsystem1/PID ELEVATOR/Reset Signal'
 * '<S274>' : 'pid_control/Subsystem1/PID ELEVATOR/Saturation'
 * '<S275>' : 'pid_control/Subsystem1/PID ELEVATOR/Saturation Fdbk'
 * '<S276>' : 'pid_control/Subsystem1/PID ELEVATOR/Sum'
 * '<S277>' : 'pid_control/Subsystem1/PID ELEVATOR/Sum Fdbk'
 * '<S278>' : 'pid_control/Subsystem1/PID ELEVATOR/Tracking Mode'
 * '<S279>' : 'pid_control/Subsystem1/PID ELEVATOR/Tracking Mode Sum'
 * '<S280>' : 'pid_control/Subsystem1/PID ELEVATOR/Tsamp - Integral'
 * '<S281>' : 'pid_control/Subsystem1/PID ELEVATOR/Tsamp - Ngain'
 * '<S282>' : 'pid_control/Subsystem1/PID ELEVATOR/postSat Signal'
 * '<S283>' : 'pid_control/Subsystem1/PID ELEVATOR/preInt Signal'
 * '<S284>' : 'pid_control/Subsystem1/PID ELEVATOR/preSat Signal'
 * '<S285>' : 'pid_control/Subsystem1/PID ELEVATOR/Anti-windup/Back Calculation'
 * '<S286>' : 'pid_control/Subsystem1/PID ELEVATOR/D Gain/Internal Parameters'
 * '<S287>' : 'pid_control/Subsystem1/PID ELEVATOR/External Derivative/Error'
 * '<S288>' : 'pid_control/Subsystem1/PID ELEVATOR/Filter/Cont. Filter'
 * '<S289>' : 'pid_control/Subsystem1/PID ELEVATOR/Filter ICs/Internal IC - Filter'
 * '<S290>' : 'pid_control/Subsystem1/PID ELEVATOR/I Gain/Internal Parameters'
 * '<S291>' : 'pid_control/Subsystem1/PID ELEVATOR/Ideal P Gain/Passthrough'
 * '<S292>' : 'pid_control/Subsystem1/PID ELEVATOR/Ideal P Gain Fdbk/Disabled'
 * '<S293>' : 'pid_control/Subsystem1/PID ELEVATOR/Integrator/Continuous'
 * '<S294>' : 'pid_control/Subsystem1/PID ELEVATOR/Integrator ICs/Internal IC'
 * '<S295>' : 'pid_control/Subsystem1/PID ELEVATOR/N Copy/Disabled'
 * '<S296>' : 'pid_control/Subsystem1/PID ELEVATOR/N Gain/Internal Parameters'
 * '<S297>' : 'pid_control/Subsystem1/PID ELEVATOR/P Copy/Disabled'
 * '<S298>' : 'pid_control/Subsystem1/PID ELEVATOR/Parallel P Gain/Internal Parameters'
 * '<S299>' : 'pid_control/Subsystem1/PID ELEVATOR/Reset Signal/Disabled'
 * '<S300>' : 'pid_control/Subsystem1/PID ELEVATOR/Saturation/Enabled'
 * '<S301>' : 'pid_control/Subsystem1/PID ELEVATOR/Saturation Fdbk/Disabled'
 * '<S302>' : 'pid_control/Subsystem1/PID ELEVATOR/Sum/Sum_PID'
 * '<S303>' : 'pid_control/Subsystem1/PID ELEVATOR/Sum Fdbk/Disabled'
 * '<S304>' : 'pid_control/Subsystem1/PID ELEVATOR/Tracking Mode/Disabled'
 * '<S305>' : 'pid_control/Subsystem1/PID ELEVATOR/Tracking Mode Sum/Passthrough'
 * '<S306>' : 'pid_control/Subsystem1/PID ELEVATOR/Tsamp - Integral/TsSignalSpecification'
 * '<S307>' : 'pid_control/Subsystem1/PID ELEVATOR/Tsamp - Ngain/Passthrough'
 * '<S308>' : 'pid_control/Subsystem1/PID ELEVATOR/postSat Signal/Forward_Path'
 * '<S309>' : 'pid_control/Subsystem1/PID ELEVATOR/preInt Signal/Internal PreInt'
 * '<S310>' : 'pid_control/Subsystem1/PID ELEVATOR/preSat Signal/Forward_Path'
 * '<S311>' : 'pid_control/Subsystem2/PID ALTURA'
 * '<S312>' : 'pid_control/Subsystem2/PID ALTURA/Anti-windup'
 * '<S313>' : 'pid_control/Subsystem2/PID ALTURA/D Gain'
 * '<S314>' : 'pid_control/Subsystem2/PID ALTURA/External Derivative'
 * '<S315>' : 'pid_control/Subsystem2/PID ALTURA/Filter'
 * '<S316>' : 'pid_control/Subsystem2/PID ALTURA/Filter ICs'
 * '<S317>' : 'pid_control/Subsystem2/PID ALTURA/I Gain'
 * '<S318>' : 'pid_control/Subsystem2/PID ALTURA/Ideal P Gain'
 * '<S319>' : 'pid_control/Subsystem2/PID ALTURA/Ideal P Gain Fdbk'
 * '<S320>' : 'pid_control/Subsystem2/PID ALTURA/Integrator'
 * '<S321>' : 'pid_control/Subsystem2/PID ALTURA/Integrator ICs'
 * '<S322>' : 'pid_control/Subsystem2/PID ALTURA/N Copy'
 * '<S323>' : 'pid_control/Subsystem2/PID ALTURA/N Gain'
 * '<S324>' : 'pid_control/Subsystem2/PID ALTURA/P Copy'
 * '<S325>' : 'pid_control/Subsystem2/PID ALTURA/Parallel P Gain'
 * '<S326>' : 'pid_control/Subsystem2/PID ALTURA/Reset Signal'
 * '<S327>' : 'pid_control/Subsystem2/PID ALTURA/Saturation'
 * '<S328>' : 'pid_control/Subsystem2/PID ALTURA/Saturation Fdbk'
 * '<S329>' : 'pid_control/Subsystem2/PID ALTURA/Sum'
 * '<S330>' : 'pid_control/Subsystem2/PID ALTURA/Sum Fdbk'
 * '<S331>' : 'pid_control/Subsystem2/PID ALTURA/Tracking Mode'
 * '<S332>' : 'pid_control/Subsystem2/PID ALTURA/Tracking Mode Sum'
 * '<S333>' : 'pid_control/Subsystem2/PID ALTURA/Tsamp - Integral'
 * '<S334>' : 'pid_control/Subsystem2/PID ALTURA/Tsamp - Ngain'
 * '<S335>' : 'pid_control/Subsystem2/PID ALTURA/postSat Signal'
 * '<S336>' : 'pid_control/Subsystem2/PID ALTURA/preInt Signal'
 * '<S337>' : 'pid_control/Subsystem2/PID ALTURA/preSat Signal'
 * '<S338>' : 'pid_control/Subsystem2/PID ALTURA/Anti-windup/Back Calculation'
 * '<S339>' : 'pid_control/Subsystem2/PID ALTURA/D Gain/Internal Parameters'
 * '<S340>' : 'pid_control/Subsystem2/PID ALTURA/External Derivative/Error'
 * '<S341>' : 'pid_control/Subsystem2/PID ALTURA/Filter/Cont. Filter'
 * '<S342>' : 'pid_control/Subsystem2/PID ALTURA/Filter ICs/Internal IC - Filter'
 * '<S343>' : 'pid_control/Subsystem2/PID ALTURA/I Gain/Internal Parameters'
 * '<S344>' : 'pid_control/Subsystem2/PID ALTURA/Ideal P Gain/Passthrough'
 * '<S345>' : 'pid_control/Subsystem2/PID ALTURA/Ideal P Gain Fdbk/Disabled'
 * '<S346>' : 'pid_control/Subsystem2/PID ALTURA/Integrator/Continuous'
 * '<S347>' : 'pid_control/Subsystem2/PID ALTURA/Integrator ICs/Internal IC'
 * '<S348>' : 'pid_control/Subsystem2/PID ALTURA/N Copy/Disabled'
 * '<S349>' : 'pid_control/Subsystem2/PID ALTURA/N Gain/Internal Parameters'
 * '<S350>' : 'pid_control/Subsystem2/PID ALTURA/P Copy/Disabled'
 * '<S351>' : 'pid_control/Subsystem2/PID ALTURA/Parallel P Gain/Internal Parameters'
 * '<S352>' : 'pid_control/Subsystem2/PID ALTURA/Reset Signal/Disabled'
 * '<S353>' : 'pid_control/Subsystem2/PID ALTURA/Saturation/Enabled'
 * '<S354>' : 'pid_control/Subsystem2/PID ALTURA/Saturation Fdbk/Disabled'
 * '<S355>' : 'pid_control/Subsystem2/PID ALTURA/Sum/Sum_PID'
 * '<S356>' : 'pid_control/Subsystem2/PID ALTURA/Sum Fdbk/Disabled'
 * '<S357>' : 'pid_control/Subsystem2/PID ALTURA/Tracking Mode/Disabled'
 * '<S358>' : 'pid_control/Subsystem2/PID ALTURA/Tracking Mode Sum/Passthrough'
 * '<S359>' : 'pid_control/Subsystem2/PID ALTURA/Tsamp - Integral/TsSignalSpecification'
 * '<S360>' : 'pid_control/Subsystem2/PID ALTURA/Tsamp - Ngain/Passthrough'
 * '<S361>' : 'pid_control/Subsystem2/PID ALTURA/postSat Signal/Forward_Path'
 * '<S362>' : 'pid_control/Subsystem2/PID ALTURA/preInt Signal/Internal PreInt'
 * '<S363>' : 'pid_control/Subsystem2/PID ALTURA/preSat Signal/Forward_Path'
 */
#endif                                 /* pid_control_h_ */
