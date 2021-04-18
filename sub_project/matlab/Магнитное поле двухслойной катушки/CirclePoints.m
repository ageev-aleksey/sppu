function [x, y, z] = CirclePoints(r, num_points, alpha, beta, theta, inner_shift, outer_shift)
% Точки окружности при alpha=beta=theta=0 лежащей в плоскости xOy.
% Нормаль окружности совпадает с осью Oz
%
% Arguments
% r - радиус
% num_points - количество точек
% theta- 
%(pi-theta) - угол между нормалью и осью Ox;
%(x0, y0, z0) - координаты центра окружности

gamma = 2*pi/num_points;
i = 1:num_points;
angle = i.*gamma;
x = r.*cos(angle) + inner_shift(1);
y = r.*sin(angle) + inner_shift(2);
z = zeros(num_points, 1) + inner_shift(3);
%matrixRotation = [    cos(theta),             0,           sin(theta);
%                  sin(alpha)*sin(theta), cos(alpha), -sin(alpha)*cos(theta);
%                 -cos(alpha)*sin(theta), sin(theta), cos(alpha)*cos(theta)];
Xrot = [1, 0, 0; 0 cos(alpha) -sin(alpha); 0 sin(alpha) cos(alpha)];
Yrot = [cos(beta) 0 sin(beta); 0 1 0; -sin(beta) 0 cos(beta)];
Zrot = [cos(theta) -sin(theta) 0; sin(theta) cos(theta) 0; 0 0 1];
rot = Xrot*Yrot*Zrot;
for j=i
   v = [x(j); y(j); z(j)];
   rotated = rot*v;
   x(j) = rotated(1) + outer_shift(1);
   y(j) = rotated(2) + outer_shift(2);
   z(j) = rotated(3) + outer_shift(2);
end
end

function [x, y, z] = mdot(v, Mrot)
   x = dot(v,Mrot(1, :));
   y = dot(v,Mrot(2, :)); 
   z = dot(v,Mrot(3, :)); 
end

