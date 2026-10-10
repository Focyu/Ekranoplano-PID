// Copyright 2022-2024 The MathWorks, Inc.
// Generated 10-Oct-2026 16:43:28
#include "slros2_initialize.h"
const std::string SLROSNodeName("pid_control");
// pid_control/Publish
SimulinkPublisher<sensor_msgs::msg::Imu,SL_Bus_sensor_msgs_Imu> Pub_pid_control_548;
// pid_control/Publish1
SimulinkPublisher<nav_msgs::msg::Odometry,SL_Bus_nav_msgs_Odometry> Pub_pid_control_697;
// pid_control/Publish2
SimulinkPublisher<geometry_msgs::msg::PoseStamped,SL_Bus_geometry_msgs_PoseStamped> Pub_pid_control_811;
// pid_control/Publish3
SimulinkPublisher<geometry_msgs::msg::TwistStamped,SL_Bus_geometry_msgs_TwistStamped> Pub_pid_control_835;
// pid_control/SUBSYSTEM_MODEL/Subscribe1_TURBULENCIA1
SimulinkSubscriber<std_msgs::msg::Bool,SL_Bus_std_msgs_Bool> Sub_pid_control_423;
// pid_control/SUBSYSTEM_MODEL/Subscribe_HEAVE
SimulinkSubscriber<std_msgs::msg::Float64,SL_Bus_std_msgs_Float64> Sub_pid_control_443;
// pid_control/SUBSYSTEM_MODEL/Subscribe_RATE
SimulinkSubscriber<std_msgs::msg::Float64,SL_Bus_std_msgs_Float64> Sub_pid_control_445;
// pid_control/SUBSYSTEM_MODEL/Subscribe_TURBULENCIA
SimulinkSubscriber<std_msgs::msg::Bool,SL_Bus_std_msgs_Bool> Sub_pid_control_417;
// pid_control/Subscribe-ALTURA
SimulinkSubscriber<std_msgs::msg::Float64,SL_Bus_std_msgs_Float64> Sub_pid_control_435;
// pid_control/Subscribe-YAW
SimulinkSubscriber<std_msgs::msg::Float64,SL_Bus_std_msgs_Float64> Sub_pid_control_377;
// pid_control/Call Service
SimulinkServiceCaller<gazebo_msgs::srv::SetEntityState,SL_Bus_gazebo_msgs_SetEntityStateRequest,SL_Bus_gazebo_msgs_SetEntityStateResponse> ServCall_pid_control_326;
