function [x, y, z] = SpiralePoints(num_points, num_coils, coil_length, r1,rotation, inner_shift, outer_shift)
% i - набор точек, для вычисления апроксимации окружности
% num_coils - количество ветков на одном слое
% coil_length - длина катушкиs
alpha = (num_coils*2*pi)/(num_points);
k = coil_length / num_points;

x = [];
y = [];
z = [];
r = r1;
XRot = [1, 0, 0; 0 cos(rotation(1)) -sin(rotation(1)); 0 sin(rotation(1)) cos(rotation(1))];
YRot = [cos(rotation(2)) 0 sin(rotation(2)); 0 1 0; -sin(rotation(2)) 0 cos(rotation(2))];
ZRot = [cos(rotation(3)) -sin(rotation(3)) 0; sin(rotation(3)) cos(rotation(3)) 0; 0 0 1];
mRotate = XRot*YRot*ZRot;
for j=1:num_points   
    x(j) = r * cos(alpha*j)+inner_shift(1); 
    y(j) = r* sin(alpha*j)+inner_shift(2);
    z(j) = k*j+inner_shift(3);
    v = mRotate * [x(j); y(j); z(j)] + outer_shift;
    x(j) = v(1);
    y(j) = v(2);
    z(j) = v(3);
end



end

