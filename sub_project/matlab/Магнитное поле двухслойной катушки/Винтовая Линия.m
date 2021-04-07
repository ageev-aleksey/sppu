i = 1:10000;
alpha = (10*2*pi)/(length(i));
k = 0.0005;
r = 5;
x = [];
y = [];
z = [];

for j=i
    x(i) = r * cos(alpha*i);
    y(i) = r* sin(alpha*i);
    z(i) = k*i;
end
