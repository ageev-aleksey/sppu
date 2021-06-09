% Вычисление магнитной индукции в полоскости поралельной земле на оси двух
% соостных пареллельных катушках расположенных на заданном расстоянии
% 
% Arguments
% R [int] радиус катушек
% Len [int] длина катушек
% Space [int] Расстояние между катушками
%
% Example
% MagneticToSolenoids(0.0175, 0.0125, 0.0175*2)
function   [] = MagneticToSolenoids(R, Len, Space)

% [x1, y1, z1] = SpiralePoints(1000, 20, 0.0125, 0.0175, [0 pi/2 0], [0 0 0], [0 0 0]);
% [x2, y2, z2] = SpiralePoints(1000, 20, 0.0125, 0.0175, [0 pi/2 0], [0 0 0.0475], [0 0 0]);

[x1, y1, z1] = DoubleSpirale(1000, 20, Len, R, R+0.0001, 0, 0, 0);
[x2, y2, z2] = DoubleSpirale(1000, 20, Len, R, R+0.0001, Space + Len, 0, 0);

%X = 0.012:0.001:0.0475;
%Y = -0.04:0.001:0.04;
X = Len:0.001:((Len+Space)+0.001);
Y = -2*R:0.001:2*R;
Z = 0;
Bx = [];
By = [];
Bz = [];
Ax = [];
Ay = [];
Az = [];
j = 1;
for x0=X
    i = 1;
    for y0=Y
            Bx(i, j) = 0;
            By(i, j) = 0;
            Bz(i, j) = 0;
            Ax(i, j) = 0;
            Ay(i, j) = 0;
            Az(i, j) = 0;
       [Bx(i, j), By(i, j), Bz(i,j)] = BioSavarLaplass(x1, y1, z1, x0, y0, 0, 3);
       [tx, ty, tz] = BioSavarLaplass(x2, y2, z2, x0, y0, 0, 3);
       [Ax(i, j), Ay(i, j), Az(i,j)] = A(x1, y1, z1, x0, y0, 0, 3);
       [tax, tay, taz] = A(x2, y2, z2, x0, y0, 0, 3);
      Bx(i, j) = Bx(i, j) + tx;
      By(i, j) = By(i, j) + ty;
      Bz(i, j) = Bz(i, j) + tz;
      Ax(i, j) = Ax(i, j) + tax;
      Ay(i, j) = Ay(i, j) + tay;
      Az(i, j) = Az(i, j) + taz;
       
       i = i + 1;
    end
    j = j +1;
end

figure
tiledlayout(2,2)
nexttile(1, [1, 2])

mp=(Bx.^2 + By.^2 + Bz.^2).^0.5;
mpa = (Ax.^2 + Ay.^2 + Az.^2).^0.5;
bx1=  (Bx./mp);
by1=  (By./mp);
bz1 = (Bz./mp);
hold on;
contourf(X,Y, mp, linspace(max(max(mp)), 0, 100), 'lineStyle','none');
colormap(jet)
lx = length(X); ly = length(Y); sp = 10;
%quiver(X(1:sp:lx), Y(1:sp:ly), bx1(1:sp:ly, 1:sp:lx), by1(1:sp:ly, 1:sp:lx), 'color', 'black');
contour(X, Y, mpa, linspace(max(max(mpa)), 0, 30), 'color', 'black');

%plot3(x1,y1,z1)
%plot3(x2,y2,z2)


j = 1;
while Y(j) ~= 0
    j = j + 1;
end
title('Магнитная индукция на оси катушек');

nexttile
plot(X, mp(j, :)); %Величина магнитной индукции на оси катушек
title('Величина магнитной индукции', 'на оси катушек')

nexttile
plot(X, Bx(j, :)); %Величина компоненты X вектора магнитной индукции на оси катушек
title('Величина компоненты X', 'вектора магнитной индукции на оси катушек')

end
    