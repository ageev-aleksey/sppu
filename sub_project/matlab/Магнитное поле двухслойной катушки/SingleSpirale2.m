function [x, y, z] = SingleSpirale2(num_points, num_coils, coil_length, r1, r2, x0, y0, z0)
% i - набор точек, для вычисления апроксимации окружности
% num_coils - количество ветков на одном слое
% coil_length - длина катушки
fs = num_points/2;
alpha = (num_coils*2*pi)/(fs);
k = coil_length / fs;

x = [];
y = [];
z = [];
r = r1;
for j=1:(round(fs))   
    x(j) = k*j+x0;
    y(j) = r* sin(alpha*j)+y0;
    %z(j) = k*j;
    z(j) = r * cos(alpha*j)+z0;
end

 r = r2;
for j=(round(fs)+1):num_points
    x(j) = k* (num_points - j)+x0;
    y(j) = r* sin(alpha*j)+y0;
    %z(j) = k* (length(i)- j);
    z(j) = r * cos(alpha*j)+z0;
end
end

