clearvars; close all; clc;

%% 1. PARÁMETROS BASE DEL ENTORNO
% (Asegúrate de que h_sp y x_nom(12) coincidan con el objetivo que quieres evaluar)
Cp_motor = 0.57; D_prop = 0.1270; rho = 1.225; Sp_prop = pi*(D_prop^2/4); km_motor = 25.0;
Cp = Cp_motor; D = D_prop; Sp = pi*(D^2/4); km = km_motor;

tsim = 100; % 15 segundos son suficientes para evaluar la estabilidad
step = 0.01;

x_nom = zeros(12,1);
x_nom(1) = 17.0;             
x_nom(8)  = -1.5*(pi/180);   
x_nom(12) = -1.0; % Altura inicial (debe coincidir con h_sp)

u_nom = zeros(5,1); u_nom(4:5) = 0.52; x0 = zeros(12,1);
u_enable_flight = 16.0; u_disable_flight = 10.0;     

u_sp = 20.0; Kp_u = 0.7; Ki_u = 0.1; Kd_u = 0.01;

h_sp = 1.0; % Setpoint a evaluar
Kp_h = 0.1; Ki_h = 0.02; Kd_h = 0.05;

theta_max = 0.15; theta_min = -0.15;
Kp_pitch = -0.050; Ki_pitch = -0.015; Kd_pitch = -0.030;

psi_sp = 0; Kp_yaw = -1.2; Ki_yaw = -0.04; Kd_yaw = -1.30;
phi_sp = 0; Kp_roll = -0.4; Ki_roll = -0.05; Kd_roll = -0.15;

%% 2. CARGAR EL AGENTE ENTRENADO
%% 2. CARGAR EL AGENTE ENTRENADO
load('REENTRENAMIENTOx4.mat'); 

% --- LA SOLUCIÓN AL ERROR ---
agentObj = agent; 
% ----------------------------

disp('Agente REENTRENAMIENTOx2 cargado. Iniciando vuelo de prueba...');

%% 3. EJECUTAR LA SIMULACIÓN
mdl = 'pid_control';
open_system(mdl);
set_param(mdl, 'StopTime', num2str(tsim));
out = sim(mdl);
disp('Simulación finalizada.');