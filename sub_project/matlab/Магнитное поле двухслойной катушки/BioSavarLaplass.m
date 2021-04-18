function [Bx,By, Bz] = BioSavarLaplass(px, py, pz, x0, y0, z0, I)
% Вычисление интеграла Био-Савара-Лапласса в по траеткории переданной в
% качетсве последовательности точек
%
% Arguments
% px [N] х компонента точек
% py [N] y компонента точек
% pz [N] z компонента точек
% x0 [1] x компонента точки в которой вычислять магнитную индукцию
% y0 [1] y компонента точки в которой вычислять магнитную индукцию
% z0 [1] z компонента точки в которой вычислять магнитную индукцию
% I [1] сила тока в проводе


plength = length(px);
mu_norm = 10^-7; 
Bx = 0;
By = 0;
Bz = 0;
for i = 1:(plength-1)
    dr = [px(i+1)-px(i), py(i+1)-py(i), pz(i+1)-pz(i)];
    %dr = dr/norm(dr);
    v = [x0, y0, z0] - [px(i), py(i), pz(i)];
    %v = v/norm(v);
    s = cross(dr, v)./(v*v')^(3/2);
    s = I*mu_norm.*s;
    Bx = Bx + s(1);
    By = By + s(2);
    Bz = Bz + s(3);
end
end

function [module] = module_vector(vec)
    module = 0;
    for i = 1:length(vec)
        module = module + vec(i)*vec(i);
    end
    module = sqrt(module);
end

