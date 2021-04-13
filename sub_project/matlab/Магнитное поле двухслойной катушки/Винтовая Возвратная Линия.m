i = 1:10000;
points_go = 100;
alpha = (10*2*pi)/(length(i));
k = 0.0005;
r1 = 5;
r2 = 6;
x = [];
y = [];
z = [];
r = r1;
for j=1:(length(i)/2)   
    x(j) = r * cos(alpha*j);
    y(j) = r* sin(alpha*j);
    z(j) = k*j;
end
r = r2;
delta_r = (r2-r1)/points_go;

% x(length(i)/2) = (r1+delta_r) * cos(alpha * length(i)/2);
% y(length(i)/2) = (r1+delta_r)* sin(alpha * length(i)/2);
% z(length(i)/2) = k * (length(i)- length(i)/2);
 
% x(1+length(i)/2) = (r1+2*delta_r) * cos(alpha * (1+length(i)/2));
% y(1+length(i)/2) = (r1+2*delta_r)* sin(alpha * (1+length(i)/2));
% z(1+length(i)/2) = k* (length(i)- (1+length(i)/2));
 
% x(2+length(i)/2) = (r1+3*delta_r) * cos(alpha * (2+length(i)/2));
% y(2+length(i)/2) = (r1+3*delta_r)* sin(alpha * (2+length(i)/2));
% z(2+length(i)/2) = k* (length(i)- (2+length(i)/2));

for j=(length(i)/2):(length(i)/2+points_go)
    s = j - (length(i)/2);
    x(j) = (r1+s*delta_r) * cos(alpha * j);
    y(j) = (r1+s*delta_r)* sin(alpha * j);
    z(j) = k* (length(i)- j);
end
 
 
for j=(length(i)/2+points_go+1):length(i)
    x(j) = r * cos(alpha*j);
    y(j) = r* sin(alpha*j);
    z(j) = k* (length(i)- j);
end
