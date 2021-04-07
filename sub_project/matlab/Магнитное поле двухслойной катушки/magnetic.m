[x1, y1, z1] = DoubleSpirale(1000, 20, 0.0125, 0.0175, 0.0176, 0, 0, 0);
[x2, y2, z2] = DoubleSpirale(1000, 20, 0.0125, 0.0175, 0.0176, 0.0475, 0, 0);
x3 = {};
y3 = {};
z3 = {};
num_circles = 10;
step = 0.014 / num_circles;
max_angle_rotation =  pi/4 - acos(sqrt(576.5/612.5));
for i=1:num_circles
    
[x3{i}, y3{i}, z3{i}] = CirclePoints(0.006, 100, pi/2, max_angle_rotation, 0, [0, 0, -step*(i-1)], [0.0295, 0, 0]);
end
X = 0.012:0.0005:0.0475;
Y = -0.04:0.0005:0.04;
Z = 0;
Bx = [];
By = [];
Bz = [];
j = 1;
for x0=X
    i = 1;
    for y0=Y
            Bx(i, j) = 0;
            By(i, j) = 0;
            Bz(i, j) = 0;
       [Bx(i, j), By(i, j), Bz(i,j)] = A(x1, y1, z1, x0, y0, 0, 3);
       [tx, ty, tz] = A(x2, y2, z2, x0, y0, 0, 3);
%         for k=1:num_circles
%             [cx, cy, cz] = BioSavarLaplass(x3{k}, y3{k}, z3{k}, x0, y0, 0, 1.28);
%             Bx(i, j) = Bx(i, j) + cx;
%             By(i, j) = By(i, j) + cy;
%             Bz(i, j) = Bz(i, j) + cz;
%         end
      Bx(i, j) = Bx(i, j) + tx; %RBx;
      By(i, j) = By(i, j) + ty; %RBy;
      Bz(i, j) = Bz(i, j) + tz; %RBz;
       
       i = i + 1;
    end
    j = j +1;
end
coef = 1;
mp=(Bx.^2 + By.^2 + Bz.^2).^0.5;
bx1= coef.*(Bx./mp);
by1=coef.*(By./mp);
bz1 = coef.*(Bz./mp);

plot3(x1,y1,z1);
hold on;
plot3(x2,y2,z2);
for i = 1:num_circles
    plot3(x3{i},y3{i},z3{i});
end
%holl1 position
plot3(0.0295 - 0.009, 0.015, 0, 'g.', 'MarkerSize', 20)
plot3(0.0295 + 0.009, 0.015, 0, 'g.', 'MarkerSize', 20)
%Line of holl detector
plot3([0.012, 0.0475], [0.015, 0.015], [0, 0]);


axis tight % removes the empty space after 3.3
set(gca,'XTick',-0.06:0.01:0.06);

for i = 1:num_circles
    plot3(x3{i},y3{i},z3{i});
end
set(gca,'YTick',-0.06:0.01:0.06);
set(gca,'ZTick',-0.06:0.01:0.06);
quiver(X, Y, bx1, by1, 'color', 'black');
hold off;

cut_value = zeros(size(mp));
level_cut = 1.25;
for i=1:length(X)
    for j = 1:length(Y)
        if abs(By(j, i)) > level_cut
            cut_value(j, i) = sign(By(j, i))*level_cut;
        else
            cut_value(j, i) = By(j, i);
        end
    end
end
surf(X, Y, cut_value)