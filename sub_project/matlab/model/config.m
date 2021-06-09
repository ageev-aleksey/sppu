%%constants of model SDD
modelStep = 0.00001;
degX0 = 0;
degZ0 = 0;
VX0 = 0;
VZ0 = 0;
H_coil = 0.0125; %coil height
R_coil = 0.035/2; % coil radius
I_magnetic = 0.0178; % Amper current strength of magnetic
R_magnetic = 0.006; % magnetic radius
len_magnetic = 0.014; %length magnetic
m_rs1 = 0.015; % лЮЯЯЮ БПЮЫЮЧЫЕИ ЯХЯРЕЛШ (rotating system): лЮЯЯЮ ЬРЮМЦХ + ЛЮЯЯЮ ЛЮЦМХРЮ + ЛЮЯЯЮ ЖЕКЕСЙЮГЮРЕКЪ
m_rs2 = 0.01;
len_rs = len_magnetic + 0.03; %length of the rotating system
Inert = (1/4)*0.15*0.006^2;%5.96667*10^(-7);%0.025;%2.1196*10^(-6); % moment of inertion
Disbalance = 0.00001; %couefecient of the force of gravity
pwmFrequency = [0 0.001];
StructModelConst = 15378.18722131129492333262;
frictionLinearCoef = 1;
frictionQuadraticCoef = 0.001;
frictionCoef = 0.00001;
%% Calculating magnetic field by Law of Bio-Savar-Laplas
x0 = 0.02975; y0 = 0; z0 = 0;
[x1, y1, z1] = DoubleSpirale(3000, 20, 0.0125, 0.0175, 0.0176, 0, 0, 0); %right solenoid
[x2, y2, z2] = DoubleSpirale(3000, 20, 0.0125, 0.0175, 0.0176, 0.0475, 0, 0); %left solenoid
[Bx_h1, By_h1, Bz_h1] = BioSavarLaplass(x1, y1, z1, x0, y0, z0, 1);
[Bx_h2, By_h2, Bz_h2] = BioSavarLaplass(x2, y2, z2, x0, y0, z0, 1);
Bh = [Bx_h1, By_h1, Bz_h1] + [Bx_h2, By_h2, Bz_h2];
B_vertical = 0;