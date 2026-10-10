clearvars; close all; clc;

%% ============================================================
%% PARAMETROS GLOBALES DEL MODELO
%% ============================================================
Cp_motor = 0.57;
D_prop = 0.1270;
rho = 1.225;
Sp_prop = pi*(D_prop^2/4);
km_motor = 25.0;
Cp = Cp_motor;
D = D_prop;
Sp = pi*(D^2/4);
km = km_motor;

max_thrust_force_per_motor = 0.5*rho*Sp_prop*Cp_motor*km_motor^2;
fprintf('Thrust max por motor: %.3f N\n', max_thrust_force_per_motor);
fprintf('Thrust max total: %.3f N\n', 2*max_thrust_force_per_motor);

%% ============================================================
%% CONDICIONES INICIALES
%% ============================================================
tsim = 20;
step = 0.01;

x_nom = zeros(12,1);
x_nom(1) = 1.0;
x_nom(3) = 0;
x_nom(8)  = -1.5*(pi/180);   % theta trimado (~-1.5 deg)
x_nom(10) = 0;
x_nom(11) = 0;
x_nom(12) = -0.250;          % z_NED; h_trim = -z = 0.250 m
u_nom = zeros(5,1);
u_nom(1) = 0.0;
u_nom(2) = 0.0;
u_nom(3) = 0.0;
u_nom(4:5) = 0.52;

x0 = zeros(12,1);

%% FASE DE RODAJE / ACELERACION
u_enable_flight = 15.0;      % [m/s] activa control de altura normal
u_disable_flight = 12.0;     % [m/s] desactiva si cae bajo este valor

%% ============================================================
%% PARAMETROS DE CONTROL PID
%% ============================================================
u_sp = 20.0;
Kp_u = 0.02;
Ki_u = 0.015;
Kd_u = 0.005;

h_sp = 0.80;
Kp_h = 0.50;
Ki_h = 0.02;
Kd_h = 0.25;

theta_max = 0.20;
theta_min = -0.20;
Kp_pitch = -0.50;
Ki_pitch = -0.015;
Kd_pitch = -0.30;

psi_sp = 0*(pi/180);
Kp_yaw = -1.2;
Ki_yaw = -0.04;
Kd_yaw = -1.30;

phi_sp = 0.0;
Kp_roll = -0.4;
Ki_roll = -0.05;
Kd_roll = -0.15;

%% ============================================================
%% SIMULACION
%% ============================================================
modelName = 'pid_control';
load_system(modelName);
set_param(modelName, 'StopTime', num2str(tsim));
out = sim(modelName);

%% ============================================================
%% LECTURA DE SEÑALES
%% ============================================================
if isprop(out,'tout')
    t = out.tout;
else
    error('No se encontro out.tout en la simulacion.');
end

S = read_sim_signal(out, 'states');
C = read_sim_signal(out, 'control_out');
LD_ratio = read_sim_signal(out, 'LD');
Fbx = read_sim_signal(out, 'Fbx');
Fby = read_sim_signal(out, 'Fby');
Fbz = read_sim_signal(out, 'Fbz');
Mx  = read_sim_signal(out, 'Mbx');
My  = read_sim_signal(out, 'Mby');
Mz  = read_sim_signal(out, 'Mbz');
CQ = read_sim_signal(out, 'CQ');
Cl = read_sim_signal(out, 'Cl');
Cn = read_sim_signal(out, 'Cn');
Cm = read_sim_signal(out, 'Cm');
alpha = read_sim_signal(out, 'alpha');
beta  = read_sim_signal(out, 'beta');
CL_w_OGE  = read_sim_signal(out, 'CL_w_OGE');
CL_h_OGE  = read_sim_signal(out, 'CL_h_OGE');
CL_w_IGE  = read_sim_signal(out, 'CL_w_IGE');
CL_h_IGE  = read_sim_signal(out, 'CL_h_IGE');
CD_iw_IGE = read_sim_signal(out, 'CD_iw_IGE');
CD_ih_IGE = read_sim_signal(out, 'CD_ih_IGE');
Fgx = read_sim_signal(out, 'Fgx');
Fgy = read_sim_signal(out, 'Fgy');
Fgz = read_sim_signal(out, 'Fgz');
Fax = read_sim_signal(out, 'Fax');
Fay = read_sim_signal(out, 'Fay');
Faz = read_sim_signal(out, 'Faz');
Ftx = read_sim_signal(out, 'Ftx');
Fty = read_sim_signal(out, 'Fty');
Ftz = read_sim_signal(out, 'Ftz');
P  = read_sim_signal(out, 'Power');
E  = read_sim_signal(out, 'Energy');
LT = read_sim_signal(out, 'Load_Torque');

SUMOLA1_raw = read_sim_signal(out, 'SumOLA1');
SUMOLA_raw  = read_sim_signal(out, 'SumOLA');

sp_u     = read_sim_signal(out, 'u_set');
sp_h     = read_sim_signal(out, 'altura_sp');
sp_theta = read_sim_signal(out, 'theta_sp');
sp_yaw   = read_sim_signal(out, 'yaw_sp');
sp_roll  = read_sim_signal(out, 'roll_sp');
act_u     = read_sim_signal(out, 'estado_u');
act_z     = read_sim_signal(out, 'estado_z');
act_theta = read_sim_signal(out, 'estado_theta');
act_yaw   = read_sim_signal(out, 'estado_psi');
act_roll  = read_sim_signal(out, 'estado_phi');
pid_u     = read_sim_signal(out, 'pid_u');
pid_h     = read_sim_signal(out, 'pid_h');
pid_theta = read_sim_signal(out, 'pid_theta');
pid_yaw   = read_sim_signal(out, 'pid_yaw');
pid_roll  = read_sim_signal(out, 'pid_roll');

%% ============================================================
%% NORMALIZACION DE FORMATOS (CON SU PROPIO VECTOR DE TIEMPO)
%% ============================================================
t = force_column(t);
S = force_matrix_cols(S, 12, 'states');

C_raw_check = C;
if isa(C_raw_check,'timeseries')
    raw_sz = size(C_raw_check.Data);
elseif isstruct(C_raw_check)
    if isfield(C_raw_check,'signals') && isfield(C_raw_check.signals,'values')
        raw_sz = size(C_raw_check.signals.values);
    else
        raw_sz = size(C_raw_check);
    end
else
    raw_sz = size(C_raw_check);
end
fprintf('\n--- DIAGNOSTICO control_out (crudo, antes de normalizar) ---\n');
fprintf('Tamaño detectado: [%d %d]\n', raw_sz(1), raw_sz(min(2,numel(raw_sz))));

C = force_control_matrix(C, 'control_out');

fprintf('\n--- DIAGNOSTICO control_out (tras force_control_matrix) ---\n');
fprintf('Tamaño de C: [%d %d]\n', size(C,1), size(C,2));
col_labels = {'delta_a (ailerón)','delta_e (elevador)','delta_r (timón)','delta_t1 (throttle1)','delta_t2 (throttle2)'};
for col = 1:size(C,2)
    fprintf('%-22s -> min=%.6g max=%.6g media=%.6g\n', col_labels{col}, min(C(:,col)), max(C(:,col)), mean(C(:,col)));
end

[LD_ratio, t_LD]       = force_column_time(LD_ratio, t);
[alpha, t_alpha]       = force_column_time(alpha, t);
[beta, t_beta]         = force_column_time(beta, t);
[P, t_P]               = force_column_time(P, t);
[E, t_E]               = force_column_time(E, t);
[LT, t_LT]             = force_column_time(LT, t);
[Fbx, t_Fbx] = force_column_time(Fbx, t); [Fby, t_Fby] = force_column_time(Fby, t); [Fbz, t_Fbz] = force_column_time(Fbz, t);
[Fax, t_Fax] = force_column_time(Fax, t); [Fay, t_Fay] = force_column_time(Fay, t); [Faz, t_Faz] = force_column_time(Faz, t);
[Ftx, t_Ftx] = force_column_time(Ftx, t); [Fty, t_Fty] = force_column_time(Fty, t); [Ftz, t_Ftz] = force_column_time(Ftz, t);
[Fgx, t_Fgx] = force_column_time(Fgx, t); [Fgy, t_Fgy] = force_column_time(Fgy, t); [Fgz, t_Fgz] = force_column_time(Fgz, t);
[Mx, t_Mx]  = force_column_time(Mx, t);  [My, t_My]  = force_column_time(My, t);  [Mz, t_Mz]  = force_column_time(Mz, t);
[CL_w_OGE, t_CLwOGE] = force_column_time(CL_w_OGE, t);
[CL_h_OGE, t_CLhOGE] = force_column_time(CL_h_OGE, t);
[CL_w_IGE, t_CLwIGE] = force_column_time(CL_w_IGE, t);
[CL_h_IGE, t_CLhIGE] = force_column_time(CL_h_IGE, t);
[CD_iw_IGE, t_CDiw]   = force_column_time(CD_iw_IGE, t);
[CD_ih_IGE, t_CDih]   = force_column_time(CD_ih_IGE, t);
[CQ, t_CQ] = force_column_time(CQ, t);
[Cl, t_Cl] = force_column_time(Cl, t);
[Cm, t_Cm] = force_column_time(Cm, t);
[Cn, t_Cn] = force_column_time(Cn, t);

t_SUMOLA1 = SUMOLA1_raw.Time;
t_SUMOLA  = SUMOLA_raw.Time;
SUMOLA1 = force_matrix_cols(SUMOLA1_raw.Data, 3, 'SUMOLA1');
SUMOLA  = force_matrix_cols(SUMOLA_raw.Data,  3, 'SUMOLA');

% PID: conservar el tiempo real del To Workspace (no inventar 0:step:tend)
[sp_u,      t_sp_u]      = force_column_time(sp_u, t);
[act_u,     t_act_u]     = force_column_time(act_u, t);
[sp_h,      t_sp_h]      = force_column_time(sp_h, t);
[act_z,     t_act_z]     = force_column_time(act_z, t);
[sp_theta,  t_sp_theta]  = force_column_time(sp_theta, t);
[act_theta, t_act_theta] = force_column_time(act_theta, t);
[sp_yaw,    t_sp_yaw]    = force_column_time(sp_yaw, t);
[act_yaw,   t_act_yaw]   = force_column_time(act_yaw, t);
[sp_roll,   t_sp_roll]   = force_column_time(sp_roll, t);
[act_roll,  t_act_roll]  = force_column_time(act_roll, t);
[pid_u,     t_pid_u]     = force_column_time(pid_u, t);
[pid_h,     t_pid_h]     = force_column_time(pid_h, t);
[pid_theta, t_pid_theta] = force_column_time(pid_theta, t);
[pid_yaw,   t_pid_yaw]   = force_column_time(pid_yaw, t);
[pid_roll,  t_pid_roll]  = force_column_time(pid_roll, t);

%% ============================================================
%% DIAGNOSTICO DE LONGITUDES (ANTES DE REMUESTREAR)
%% ============================================================
fprintf('\n--- LONGITUDES ORIGINALES DE SEÑALES DEL MODELO ---\n');
diag_names = {'t','states','control_out','LD_ratio','alpha','beta','P','E','LT', ...
    'Fbx','Fby','Fbz','Fax','Fay','Faz','Ftx','Fty','Ftz','Fgx','Fgy','Fgz', ...
    'Mx','My','Mz','CL_w_OGE','CL_h_OGE','CL_w_IGE','CL_h_IGE','CD_iw_IGE','CD_ih_IGE', ...
    'CQ','Cl','Cm','Cn','SUMOLA','SUMOLA1', ...
    'sp_u','act_u','sp_h','act_z','sp_theta','act_theta','sp_yaw','act_yaw','sp_roll','act_roll'};
diag_lens = [length(t), size(S,1), size(C,1), length(LD_ratio), length(alpha), length(beta), ...
    length(P), length(E), length(LT), length(Fbx), length(Fby), length(Fbz), ...
    length(Fax), length(Fay), length(Faz), length(Ftx), length(Fty), length(Ftz), ...
    length(Fgx), length(Fgy), length(Fgz), length(Mx), length(My), length(Mz), ...
    length(CL_w_OGE), length(CL_h_OGE), length(CL_w_IGE), length(CL_h_IGE), ...
    length(CD_iw_IGE), length(CD_ih_IGE), length(CQ), length(Cl), length(Cm), length(Cn), ...
    size(SUMOLA,1), size(SUMOLA1,1), ...
    length(sp_u), length(act_u), length(sp_h), length(act_z), length(sp_theta), length(act_theta), ...
    length(sp_yaw), length(act_yaw), length(sp_roll), length(act_roll)];
for k = 1:numel(diag_names)
    fprintf('%-14s -> %d muestras\n', diag_names{k}, diag_lens(k));
end

%% ============================================================
%% REMUESTREO DE TODAS LAS SEÑALES AL VECTOR DE TIEMPO MAESTRO 't'
%% ============================================================
N_master = length(t);
t_model = t;

LD_model  = resample_to(t_LD, LD_ratio, t);
alpha_model = resample_to(t_alpha, alpha, t);
beta_model  = resample_to(t_beta, beta, t);
P_model  = resample_to(t_P, P, t);
E_model  = resample_to(t_E, E, t);
LT_model = resample_to(t_LT, LT, t);
Fbx_model = resample_to(t_Fbx, Fbx, t); Fby_model = resample_to(t_Fby, Fby, t); Fbz_model = resample_to(t_Fbz, Fbz, t);
Fax_model = resample_to(t_Fax, Fax, t); Fay_model = resample_to(t_Fay, Fay, t); Faz_model = resample_to(t_Faz, Faz, t);
Ftx_model = resample_to(t_Ftx, Ftx, t); Fty_model = resample_to(t_Fty, Fty, t); Ftz_model = resample_to(t_Ftz, Ftz, t);
Fgx_model = resample_to(t_Fgx, Fgx, t); Fgy_model = resample_to(t_Fgy, Fgy, t); Fgz_model = resample_to(t_Fgz, Fgz, t);
Mx_model = resample_to(t_Mx, Mx, t); My_model = resample_to(t_My, My, t); Mz_model = resample_to(t_Mz, Mz, t);
CL_w_OGE_model = resample_to(t_CLwOGE, CL_w_OGE, t);
CL_h_OGE_model = resample_to(t_CLhOGE, CL_h_OGE, t);
CL_w_IGE_model = resample_to(t_CLwIGE, CL_w_IGE, t);
CL_h_IGE_model = resample_to(t_CLhIGE, CL_h_IGE, t);
CD_iw_IGE_model = resample_to(t_CDiw, CD_iw_IGE, t);
CD_ih_IGE_model = resample_to(t_CDih, CD_ih_IGE, t);
CQ_model = resample_to(t_CQ, CQ, t);
Cl_model = resample_to(t_Cl, Cl, t);
Cm_model = resample_to(t_Cm, Cm, t);
Cn_model = resample_to(t_Cn, Cn, t);

SUMOLA1_model = resample_matrix(t_SUMOLA1, SUMOLA1, t);
SUMOLA_model  = resample_matrix(t_SUMOLA,  SUMOLA,  t);

S_model = S;

if size(C,1) ~= N_master
    warning('control_out tiene %d muestras (esperadas %d). Se remuestreará al tiempo maestro.', size(C,1), N_master);
    t_C = linspace(0, t(end), size(C,1))';
    C_model = resample_matrix(t_C, C, t);
else
    C_model = C;
end

sp_u      = resample_to(t_sp_u, sp_u, t);
act_u     = resample_to(t_act_u, act_u, t);
sp_h      = resample_to(t_sp_h, sp_h, t);
act_z     = resample_to(t_act_z, act_z, t);
sp_theta  = resample_to(t_sp_theta, sp_theta, t);
act_theta = resample_to(t_act_theta, act_theta, t);
sp_yaw    = resample_to(t_sp_yaw, sp_yaw, t);
act_yaw   = resample_to(t_act_yaw, act_yaw, t);
sp_roll   = resample_to(t_sp_roll, sp_roll, t);
act_roll  = resample_to(t_act_roll, act_roll, t);
pid_u     = resample_to(t_pid_u, pid_u, t);
pid_h     = resample_to(t_pid_h, pid_h, t);
pid_theta = resample_to(t_pid_theta, pid_theta, t);
pid_yaw   = resample_to(t_pid_yaw, pid_yaw, t);
pid_roll  = resample_to(t_pid_roll, pid_roll, t);

gama_model = S_model(:,8) - atan2(S_model(:,3), S_model(:,1));
Va_model = sqrt(S_model(:,1).^2 + S_model(:,2).^2 + S_model(:,3).^2);
h_model = -S_model(:,12);          % h = -z_NED
do_model_plots = true;

fprintf('\nN_master (referencia de tiempo, tsim = %.1f s) = %d muestras\n', tsim, N_master);

%% ============================================================
%% SEÑALES PID YA ALINEADAS A t
%% estado_z es altura positiva, NO z_NED.
%% Comprobado: act_z(1)=0.250, S(1,12)=-0.250, h_sp=0.800
%% ============================================================
N = N_master;
t_pid = t;
act_h = act_z;

if abs(act_h(1) - h_model(1)) > 1e-3
    warning('estado_z(1)=%.4f no coincide con -S(1,12)=%.4f. Revisa el cableado.', act_h(1), h_model(1));
end
fprintf('Check altura t=0: act_h=%.3f m | h_planta=%.3f m | h_sp_log=%.3f | h_sp=%.3f\n', ...
    act_h(1), h_model(1), sp_h(1), h_sp);

err_u     = sp_u     - act_u;
err_h     = sp_h     - act_h;
err_theta = sp_theta - act_theta;
err_yaw   = sp_yaw   - act_yaw;
err_roll  = sp_roll  - act_roll;
S_pid = S;

dt_med = median(diff(t_pid));
step_mk = max(1, round(1/max(dt_med, eps)));
idx_mk = unique([1, 1:step_mk:N, N]);

idx_on = find(act_u >= u_enable_flight, 1, 'first');
idx_off = [];
if ~isempty(idx_on)
    idx_rel = find(act_u(idx_on:end) <= u_disable_flight, 1, 'first');
    if ~isempty(idx_rel)
        idx_off = idx_rel + idx_on - 1;
    end
end

%% ============================================================
%% PLOTTING DEL MODELO GENERAL
%% ============================================================
if do_model_plots
    figure
    plot(t_model, LD_model(:,1))
    xlabel('t [s]'); ylabel('L/D'); title('LD ratio'); grid on

    figure
    subplot(2,1,1)
    plot(t_model, alpha_model(:,1))
    xlabel('t [s]'); ylabel('\alpha [rad]'); title('Angle of attack'); grid on
    subplot(2,1,2)
    plot(t_model, beta_model(:,1))
    xlabel('t [s]'); ylabel('\beta [rad]'); title('Side-slip Angle'); grid on

    figure
    subplot(3,1,1)
    plot(t_model, P_model(:,1), 'LineWidth', 1.2)
    xlabel('t[s]'); ylabel('P [kW]'); grid on; title('Power')
    subplot(3,1,2)
    plot(t_model, E_model(:,1), 'LineWidth', 1.2)
    xlabel('t[s]'); ylabel('E [kWh]'); grid on; title('Energy')
    subplot(3,1,3)
    plot(t_model, LT_model(:,1), 'LineWidth', 1.2)
    xlabel('t[s]'); ylabel('T_L [Nm]'); grid on; title('Load Torque')

    figure
    subplot(2,2,1)
    hold on
    plot(t_model, Fbx_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fby_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fbz_model(:,1), 'LineWidth', 1.2)
    hold off
    xlabel('t [s]'); ylabel('F_b [N]'); title('F_b'); legend('Fbx','Fby','Fbz'); grid on
    subplot(2,2,2)
    hold on
    plot(t_model, Fax_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fay_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Faz_model(:,1), 'LineWidth', 1.2)
    hold off
    xlabel('t [s]'); ylabel('F_a [N]'); title('F_a'); legend('Fax','Fay','Faz'); grid on
    subplot(2,2,3)
    hold on
    plot(t_model, Ftx_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fty_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Ftz_model(:,1), 'LineWidth', 1.2)
    hold off
    xlabel('t [s]'); ylabel('F_t [N]'); title('F_t'); legend('Ftx','Fty','Ftz'); grid on
    subplot(2,2,4)
    hold on
    plot(t_model, Fgx_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fgy_model(:,1), 'LineWidth', 1.2)
    plot(t_model, Fgz_model(:,1), 'LineWidth', 1.2)
    hold off
    xlabel('t [s]'); ylabel('F_g [N]'); title('F_g'); legend('Fgx','Fgy','Fgz'); grid on

    figure
    subplot(3,1,1)
    plot(t_model, Mx_model(:,1))
    xlabel('t [s]'); ylabel('M_b_x [N]'); title('M_b_x'); grid on
    subplot(3,1,2)
    plot(t_model, My_model(:,1))
    xlabel('t [s]'); ylabel('M_b_y [N]'); title('M_b_y'); grid on
    subplot(3,1,3)
    plot(t_model, Mz_model(:,1))
    xlabel('t [s]'); ylabel('M_b_z [N]'); title('M_b_z'); grid on
    sgtitle('Torque vector M_b')

    figure
    subplot(1,2,1)
    plot(t_model, CL_w_OGE_model(:,1))
    xlabel('t [s]'); ylabel('CL_w_{OGE}'); title('CL_w_{OGE}'); grid on
    subplot(1,2,2)
    plot(t_model, CL_h_OGE_model(:,1))
    xlabel('t [s]'); ylabel('CL_h_{OGE}'); title('CL_h_{OGE}'); grid on
    sgtitle('Aerodyn Coefficient from lift forces OGE')

    figure
    subplot(1,2,1)
    plot(t_model, CL_w_IGE_model(:,1))
    xlabel('t [s]'); ylabel('CL_w_{IGE}'); title('CL_w_{IGE}'); grid on
    subplot(1,2,2)
    plot(t_model, CL_h_IGE_model(:,1))
    xlabel('t [s]'); ylabel('CL_h_{IGE}'); title('CL_h_{IGE}'); grid on
    sgtitle('Aerodyn Coefficient from lift forces IGE')

    figure
    subplot(1,2,1)
    plot(t_model, CD_iw_IGE_model(:,1))
    xlabel('t [s]'); ylabel('CD_{iw}_{IGE}'); title('CD_{iw}_{IGE}'); grid on
    subplot(1,2,2)
    plot(t_model, CD_ih_IGE_model(:,1))
    xlabel('t [s]'); ylabel('CD_{ih}_{IGE}'); title('CD_{ih}_{IGE}'); grid on
    sgtitle('Aerodyn Coefficient from drag forces IGE')

    figure
    plot(t_model, CQ_model(:,1))
    xlabel('t [s]'); ylabel('C_Q'); title('Aerodyn Coefficient from side forces'); grid on

    figure
    subplot(3,1,1)
    plot(t_model, Cl_model(:,1))
    xlabel('t [s]'); ylabel('C_l'); title('C_l'); grid on
    subplot(3,1,2)
    plot(t_model, Cm_model(:,1))
    xlabel('t [s]'); ylabel('C_m'); title('C_m'); grid on
    subplot(3,1,3)
    plot(t_model, Cn_model(:,1))
    xlabel('t [s]'); ylabel('C_n'); title('C_n'); grid on
    sgtitle('Aerodyn Coeff from torques')

    figure
    plot(t_model, S_model(:,1), 'b', 'LineWidth', 1.2); hold on
    plot(t_model, S_model(:,2), 'LineWidth', 1.2)
    plot(t_model, S_model(:,3), 'LineWidth', 1.2)
    plot(t_model, sp_u, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(u_enable_flight, 'g--', 'enable 15 m/s')
    yline(u_disable_flight, 'k--', 'disable 12 m/s')
    if ~isempty(idx_on),  xline(t_model(idx_on),  'k-',  'flight ON');  end
    if ~isempty(idx_off), xline(t_model(idx_off), 'k--', 'flight OFF'); end
    hold off
    xlabel('t [s]'); ylabel('[m/s]'); title('Linear velocities'); grid on
    legend('u','v','w','u_{sp}', 'Location', 'best')

    figure
    plot(t_model, S_model(:,4), t_model, S_model(:,5), t_model, S_model(:,6))
    xlabel('t [s]'); ylabel('[rad/s]'); title('Angular velocities'); grid on
    legend('p','q','r')

    figure
    plot(t_model, S_model(:,7), 'b', 'LineWidth', 1.1); hold on
    plot(t_model, S_model(:,8), 'Color', [0.85 0.33 0.10], 'LineWidth', 1.1)
    plot(t_model, S_model(:,9), 'Color', [0.47 0.67 0.19], 'LineWidth', 1.1)
    plot(t_model, sp_roll,  'b--', 'LineWidth', 1.2)
    plot(t_model, sp_theta, 'r--', 'LineWidth', 1.3, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    plot(t_model, sp_yaw,   'g--', 'LineWidth', 1.2)
    yline(x_nom(8), ':', 'Color', [0.4 0.4 0.4], 'LineWidth', 1.1, ...
        'Label', '\theta trim -1.5°')
    yline(theta_max, 'k--', '\theta_{max}')
    yline(theta_min, 'k--', '\theta_{min}')
    hold off
    xlabel('t [s]'); ylabel('[rad]'); title('Euler angles'); grid on
    legend('\phi','\theta','\psi','\phi_{sp}','\theta_{sp}','\psi_{sp}', 'Location', 'best')

    figure
    plot(t_model, h_model, 'b', 'LineWidth', 1.3); hold on
    plot(t_model, sp_h, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(h_sp, ':', 'Color', [0.85 0.33 0.10], 'LineWidth', 1.2, 'Label', 'h_{sp} = 0.80 m')
    yline(-x_nom(12), ':', 'Color', [0.4 0.4 0.4], 'LineWidth', 1.1, 'Label', 'h trim = 0.25 m')
    hold off
    xlabel('t [s]'); ylabel('h [m]'); title('Altura h = -z_{NED}'); grid on
    legend('h planta', 'h_{sp} log', 'Location', 'best')

    figure
    plot(t_model, S_model(:,10), 'LineWidth', 1.1); hold on
    plot(t_model, S_model(:,11), 'LineWidth', 1.1)
    plot(t_model, S_model(:,12), 'LineWidth', 1.1)
    yline(-h_sp, 'r--', 'z_{sp} = -h_{sp}', 'LineWidth', 1.2)
    yline(x_nom(12), ':', 'Color', [0.4 0.4 0.4], 'Label', 'z trim')
    hold off
    xlabel('t [s]'); ylabel('[m]'); title('Positions in NED')
    legend('x_{NED}','y_{NED}','z_{NED}', 'Location', 'best'); grid on

    figure
    hold on
    plot3(S_model(:,10), S_model(:,11), h_model, 'b', 'LineWidth', 1.2)
    plot3(S_model(1,10), S_model(1,11), h_model(1), 'rx', ...
        'MarkerSize', 12, 'LineWidth', 2)
    plot3(S_model(end,10), S_model(end,11), h_model(end), 'gs', ...
        'MarkerSize', 10, 'LineWidth', 2)
    plot3(x_nom(10), x_nom(11), -x_nom(12), 'kd', ...
        'MarkerSize', 10, 'LineWidth', 1.5)
    x_span = [min(S_model(:,10)), max(S_model(:,10))];
    if abs(diff(x_span)) < 1e-6
        x_span = x_span(1) + [-1, 1];
    end
    plot3(x_span, [0 0], [h_sp h_sp], 'r--', 'LineWidth', 1.3)
    hold off
    xlabel('x_{NED}'); ylabel('y_{NED}'); zlabel('h [m]')
    title('Trayectoria 3D'); grid on; axis equal; view(3)
    legend('trayectoria', 'inicio', 'final', 'trim h=0.25 m', 'h_{sp}=0.80 m', ...
        'Location', 'best')

    figure
    plot(t_model, gama_model)
    xlabel('t [s]'); ylabel('\gamma [rad]'); title('Flight Path Angle \gamma'); grid on

    figure
    plot(t_model, Va_model, 'b', 'LineWidth', 1.2); hold on
    yline(u_sp, 'r--', 'u_{sp}', 'LineWidth', 1.2)
    yline(u_enable_flight, 'g--', 'enable')
    yline(u_disable_flight, 'k--', 'disable')
    hold off
    xlabel('t [s]'); ylabel('V_a [m/s]'); title('True Airspeed V_a'); grid on
    legend('V_a', 'Location', 'best')

    figure
    subplot(5,1,1)
    plot(t_model, pid_roll*180/pi)
    xlabel('t [s]'); ylabel('\delta_a [deg]'); title('Aileron Deflection (PID roll)'); grid on
    subplot(5,1,2)
    plot(t_model, pid_theta*180/pi)
    xlabel('t [s]'); ylabel('\delta_e [deg]'); title('Elevator Deflection (PID pitch)'); grid on
    subplot(5,1,3)
    plot(t_model, pid_yaw*180/pi)
    xlabel('t [s]'); ylabel('\delta_r [deg]'); title('Rudder Deflection (PID yaw)'); grid on
    subplot(5,1,4)
    plot(t_model, C_model(:,4).*100./max_thrust_force_per_motor)
    xlabel('t [s]'); ylabel('\delta_{t1} [%]'); title('Throttle 1'); grid on
    subplot(5,1,5)
    plot(t_model, C_model(:,5).*100./max_thrust_force_per_motor)
    xlabel('t [s]'); ylabel('\delta_{t2} [%]'); title('Throttle 2'); grid on

    nombres_sumola  = {'Base fija (roll)', 'Turbulencia activa (roll)', 'Oleaje activo (roll)'};
    nombres_sumola1 = {'Base fija (pitch)', 'Oleaje activo (heave)', 'Oleaje activo (pitch)'};
    figure
    subplot(2,1,1)
    plot(t_model, SUMOLA_model(:,1), 'LineWidth', 1.2); hold on
    plot(t_model, SUMOLA_model(:,2), 'LineWidth', 1.2)
    plot(t_model, SUMOLA_model(:,3), 'LineWidth', 1.2); hold off
    xlabel('t [s]'); ylabel('SUMOLA'); title('Señal SumOLA (turbulencia eje roll)')
    legend(nombres_sumola, 'Location', 'best'); grid on
    subplot(2,1,2)
    plot(t_model, SUMOLA1_model(:,1), 'LineWidth', 1.2); hold on
    plot(t_model, SUMOLA1_model(:,2), 'LineWidth', 1.2)
    plot(t_model, SUMOLA1_model(:,3), 'LineWidth', 1.2); hold off
    xlabel('t [s]'); ylabel('SUMOLA1'); title('Señal SumOLA1 (turbulencia eje pitch + oleaje heave)')
    legend(nombres_sumola1, 'Location', 'best'); grid on
    sgtitle('PID - Salidas de los sumadores SumOLA / SumOLA1 (vectoriales)')
end

%% ============================================================
%% PLOTS PID
%% ============================================================
figure
subplot(5,1,1)
plot(t_pid, err_u)
xlabel('t [s]'); ylabel('e_u [m/s]'); title('Error Velocidad (u)'); grid on
subplot(5,1,2)
plot(t_pid, err_h)
xlabel('t [s]'); ylabel('e_h [m]'); title('Error Altura (h)'); grid on
subplot(5,1,3)
plot(t_pid, err_theta)
xlabel('t [s]'); ylabel('e_\theta [rad]'); title('Error Pitch (\theta)'); grid on
subplot(5,1,4)
plot(t_pid, err_yaw)
xlabel('t [s]'); ylabel('e_\psi [rad]'); title('Error Yaw (\psi)'); grid on
subplot(5,1,5)
plot(t_pid, err_roll)
xlabel('t [s]'); ylabel('e_\phi [rad]'); title('Error Roll (\phi)'); grid on
sgtitle('PID - Señales de Error')

figure
subplot(5,1,1)
plot(t_pid, pid_u)
xlabel('t [s]'); ylabel('out_u'); title('Salida PID Velocidad'); grid on
subplot(5,1,2)
plot(t_pid, pid_h)
xlabel('t [s]'); ylabel('out_h'); title('Salida PID Altura'); grid on
subplot(5,1,3)
plot(t_pid, pid_theta)
xlabel('t [s]'); ylabel('out_\theta'); title('Salida PID Pitch'); grid on
subplot(5,1,4)
plot(t_pid, pid_yaw)
xlabel('t [s]'); ylabel('out_\psi'); title('Salida PID Yaw'); grid on
subplot(5,1,5)
plot(t_pid, pid_roll)
xlabel('t [s]'); ylabel('out_\phi'); title('Salida PID Roll'); grid on
sgtitle('PID - Salidas de los Controladores')

if ~isempty(S_pid) && size(S_pid,2) >= 12
    figure
    subplot(5,1,1)
    plot(t_pid, act_u, 'b', 'LineWidth', 1.2); hold on
    plot(t_pid, sp_u, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(u_sp, ':', 'Color', [0.85 0.33 0.10], 'LineWidth', 1.1, 'Label', 'u_{sp}')
    yline(u_enable_flight, 'g--', 'enable 15 m/s')
    yline(u_disable_flight, 'k--', 'disable 12 m/s')
    if ~isempty(idx_on),  xline(t_pid(idx_on),  'k-',  'flight ON');  end
    if ~isempty(idx_off), xline(t_pid(idx_off), 'k--', 'flight OFF'); end
    hold off
    xlabel('t [s]'); ylabel('u [m/s]'); title('Velocidad: Ref vs Actual')
    legend('u', 'u_{sp} log', 'Location', 'best'); grid on

    subplot(5,1,2)
    plot(t_pid, act_h, 'b', 'LineWidth', 1.2); hold on
    plot(t_pid, sp_h, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(h_sp, ':', 'Color', [0.85 0.33 0.10], 'LineWidth', 1.1, 'Label', 'h_{sp}=0.80')
    yline(-x_nom(12), ':', 'Color', [0.4 0.4 0.4], 'LineWidth', 1.1, 'Label', 'h trim=0.25')
    hold off
    xlabel('t [s]'); ylabel('h [m]'); title('Altura: Ref vs Actual')
    legend('h', 'h_{sp} log', 'Location', 'best'); grid on

    subplot(5,1,3)
    plot(t_pid, act_theta, 'b', 'LineWidth', 1.2); hold on
    plot(t_pid, sp_theta, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(x_nom(8), ':', 'Color', [0.4 0.4 0.4], 'LineWidth', 1.1, 'Label', '\theta trim')
    yline(theta_max, 'k--', '\theta_{max}')
    yline(theta_min, 'k--', '\theta_{min}')
    hold off
    xlabel('t [s]'); ylabel('\theta [rad]'); title('Pitch: Ref dinámica vs Actual')
    legend('\theta', '\theta_{sp}(din)', 'Location', 'best'); grid on

    subplot(5,1,4)
    plot(t_pid, act_yaw, 'b', 'LineWidth', 1.2); hold on
    plot(t_pid, sp_yaw, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(psi_sp, ':', 'Color', [0.85 0.33 0.10], 'LineWidth', 1.1, 'Label', '\psi_{sp}')
    hold off
    xlabel('t [s]'); ylabel('\psi [rad]'); title('Yaw: Ref vs Actual')
    legend('\psi', '\psi_{sp}', 'Location', 'best'); grid on

    subplot(5,1,5)
    plot(t_pid, act_roll, 'b', 'LineWidth', 1.2); hold on
    plot(t_pid, sp_roll, 'r--', 'LineWidth', 1.4, 'Marker', 'o', ...
        'MarkerIndices', idx_mk, 'MarkerSize', 4)
    yline(phi_sp, ':', 'Color', [0.85 0.33 0.10], 'LineWidth', 1.1, 'Label', '\phi_{sp}')
    hold off
    xlabel('t [s]'); ylabel('\phi [rad]'); title('Roll: Ref vs Actual')
    legend('\phi', '\phi_{sp}', 'Location', 'best'); grid on
    sgtitle('PID - Referencia encima del estado')
end

%% ============================================================
%% EXPORTAR SEÑALES PID + SumOLA/SumOLA1 A EXCEL
%% ============================================================
fname_pid = 'PID_TESTVEL18.xlsx';
if exist(fname_pid, 'file') == 2
    delete(fname_pid);
end

act_h_out = act_h;
pid_table = table( ...
    t_pid, ...
    sp_h, act_h_out, err_h, pid_h, ...
    sp_roll, act_roll, err_roll, pid_roll, ...
    sp_theta, act_theta, err_theta, pid_theta, ...
    sp_yaw, act_yaw, err_yaw, pid_yaw, ...
    sp_u, act_u, err_u, pid_u, ...
    'VariableNames', { ...
    't', ...
    'sp_h', 'actual_h', 'error_h', 'pid_h', ...
    'sp_roll', 'actual_roll', 'error_roll', 'pid_roll', ...
    'sp_theta', 'actual_theta', 'error_theta', 'pid_theta', ...
    'sp_yaw', 'actual_yaw', 'error_yaw', 'pid_yaw', ...
    'sp_u', 'actual_u', 'error_u', 'pid_u'});

sumola_table = table( ...
    SUMOLA_model(:,1), SUMOLA_model(:,2), SUMOLA_model(:,3), ...
    SUMOLA1_model(:,1), SUMOLA1_model(:,2), SUMOLA1_model(:,3), ...
    'VariableNames', { ...
    'SUMOLA_TurbVientoP_Dryden', 'SUMOLA_TurbulenciaROS_switch', 'SUMOLA_SinUso', ...
    'SUMOLA1_TurbVientoQ_Dryden', 'SUMOLA1_OleajeHeave_ROS', 'SUMOLA1_SinUso'});

combined_pid_table = [pid_table, sumola_table];
writetable(combined_pid_table, fname_pid, 'Sheet', 'PID_only');
fprintf('\nOK: Archivo Excel PID exportado: %s\n', fname_pid);

%% FUNCIONES LOCALES
function x = read_sim_signal(out, varName)
    if isprop(out, varName)
        x = out.(varName);
        return;
    end
    try
        x = out.get(varName);
        return;
    catch
    end
    error('No se encontro la variable "%s" en la salida de simulacion.', varName);
end

function v = force_column(v)
    if isa(v, 'timeseries')
        v = v.Data;
    end
    if isstruct(v)
        if isfield(v, 'signals') && isfield(v.signals, 'values')
            v = v.signals.values;
        elseif isfield(v, 'values')
            v = v.values;
        elseif isfield(v, 'Data')
            v = v.Data;
        end
    end
    v = squeeze(v);
    if isempty(v)
        v = [];
        return;
    end
    if isvector(v)
        v = v(:);
        return;
    end
    if size(v,2) == 2
        v = v(:,2);
        return;
    end
    if size(v,1) == 2 && size(v,2) > 2
        v = v(2,:).';
        return;
    end
    if size(v,2) == 1
        v = v(:);
        return;
    end
    v = v(:,1);
end

function [v, tv] = force_column_time(raw, t_ref)
    tv = [];
    if isa(raw, 'timeseries')
        tv = raw.Time;
        v = raw.Data;
    elseif isstruct(raw)
        if isfield(raw, 'time')
            tv = raw.time;
        end
        if isfield(raw, 'signals') && isfield(raw.signals, 'values')
            v = raw.signals.values;
        elseif isfield(raw, 'values')
            v = raw.values;
        elseif isfield(raw, 'Data')
            v = raw.Data;
        else
            v = raw;
        end
    else
        v = raw;
    end
    v = squeeze(v);
    if isvector(v)
        v = v(:);
    elseif size(v,2) == 2
        if isempty(tv), tv = v(:,1); end
        v = v(:,2);
    else
        v = v(:,1);
    end
    if isempty(tv)
        if length(v) == length(t_ref)
            tv = t_ref;
        else
            dt_ref = mean(diff(t_ref));
            tv = (0:length(v)-1)' * dt_ref;
        end
    end
    tv = tv(:);
end

function v_out = resample_to(t_src, v_src, t_dst)
    if length(t_src) == length(t_dst) && max(abs(t_src - t_dst)) < 1e-9
        v_out = v_src;
        return;
    end
    if length(t_src) < 2
        v_out = repmat(v_src(1), length(t_dst), 1);
        return;
    end
    v_out = interp1(t_src, v_src, t_dst, 'linear', 'extrap');
    v_out = v_out(:);
end

function M_out = resample_matrix(t_src, M_src, t_dst)
    if length(t_src) == length(t_dst) && max(abs(t_src - t_dst)) < 1e-9
        M_out = M_src;
        return;
    end
    if size(M_src,1) < 2
        M_out = repmat(M_src(1,:), length(t_dst), 1);
        return;
    end
    M_out = interp1(t_src, M_src, t_dst, 'linear', 'extrap');
end

function M = force_matrix_cols(M, expectedCols, varName)
    if isa(M, 'timeseries')
        M = M.Data;
    end
    if isstruct(M)
        if isfield(M, 'signals') && isfield(M.signals, 'values')
            M = M.signals.values;
        elseif isfield(M, 'values')
            M = M.values;
        elseif isfield(M, 'Data')
            M = M.Data;
        end
    end
    M = squeeze(M);
    if isempty(M)
        error('La variable %s esta vacia.', varName);
    end
    if isvector(M)
        if expectedCols == 1
            M = M(:);
            return;
        else
            error('La variable %s llego como vector y se esperaban %d columnas.', varName, expectedCols);
        end
    end
    if size(M,2) ~= expectedCols && size(M,1) == expectedCols
        M = M.';
    end
    if size(M,2) ~= expectedCols
        error('La variable %s no tiene %d columnas. Tamaño detectado: [%d %d].', ...
            varName, expectedCols, size(M,1), size(M,2));
    end
end

function C = force_control_matrix(C, varName)
    if isa(C, 'timeseries')
        C = C.Data;
    end
    if isstruct(C)
        if isfield(C, 'signals') && isfield(C.signals, 'values')
            C = C.signals.values;
        elseif isfield(C, 'values')
            C = C.values;
        elseif isfield(C, 'Data')
            C = C.Data;
        end
    end
    C = squeeze(C);
    if isempty(C)
        warning('La variable %s esta vacia. Se rellenara con ceros.', varName);
        C = zeros(0,5);
        return;
    end
    if isvector(C)
        v = C(:);
        warning('La variable %s llego como vector. Se convertira a matriz Nx5 con columnas faltantes en cero.', varName);
        C = [v, zeros(length(v),4)];
        return;
    end
    if size(C,2) ~= 5 && size(C,1) == 5
        C = C.';
    end
    if size(C,2) < 5
        warning('La variable %s tiene %d columnas. Se completara hasta 5 con ceros.', varName, size(C,2));
        C = [C, zeros(size(C,1), 5-size(C,2))];
        return;
    end
    if size(C,2) > 5
        warning('La variable %s tiene %d columnas. Se usaran solo las primeras 5.', varName, size(C,2));
        C = C(:,1:5);
    end
end