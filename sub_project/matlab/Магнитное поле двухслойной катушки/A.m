function [Ax,Ay, Az] = A(px, py, pz, x0, y0, z0, I)
% Вычисление потенциала электрического поля по траектории заданной
% последовательностью точек

plength = length(px);
mu_norm = 10^-7; 
Ax = 0;
Ay = 0;
Az = 0;
for i = 1:(plength-1)
    dr = [px(i+1)-px(i), py(i+1)-py(i), pz(i+1)-pz(i)];
    %dr = dr/norm(dr);
    v = [x0, y0, z0] - [px(i), py(i), pz(i)];
    %v = v/norm(v);
    s = dr./abs(v*v')^(1/2);
    s = I*mu_norm.*s;
    Ax = Ax + s(1);
    Ay = Ay + s(2);
    Az = Az + s(3);
end
end

function [module] = module_vector(vec)
    module = 0;
    for i = 1:length(vec)
        module = module + vec(i)*vec(i);
    end
    module = sqrt(module);
end

