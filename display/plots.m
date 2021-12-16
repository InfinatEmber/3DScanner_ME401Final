vals = [-1.07716E-20;4.35263E-17;-7.26628E-14;6.48489E-11;-3.33216E-08;9.77453E-06;-0.001465032;0.06569285;5.31114646];
d_limit = 0.5;
zero_correction = 200;

str = input("What file do you want to plot? ",'s');
tbl = readtable(str);
tbl = tbl{:,:};

n = size(tbl,1);
d = zeros(n,1);
pitch = zeros(n,1);
yaw = zeros(n,1);
x = zeros(n,1);
y = zeros(n,1);
z = zeros(n,1);
v = zeros(n,1);

for i = 1:1:n
   v(i) = tbl(i,5);
   if v(i) > 580 || v(i) < 180
       v(i) = NaN;
   end
   d(i) = distance(v(i),vals);
   if d(i) < 0.1 || d(i) > d_limit
       d(i) = NaN;
   end
   
   pitch(i) = -(tbl(i,2)+zero_correction)/2800*2*pi;
   yaw(i) = tbl(i,1)/(2800*2)*2*pi;
   
   x(i) = cos(pitch(i))*d(i)*cos(yaw(i));
   y(i) = cos(pitch(i))*d(i)*sin(yaw(i));
   z(i) = sin(pitch(i))*d(i);
end

figure;
plot3(x,y,z,".");
%copied from https://www.mathworks.com/matlabcentral/answers/267468-change-colour-of-points-in-plot3-with-increasing-z-value
%cla
%patch([x.' nan],[y.' nan],[z.' nan],[z.' nan],'EdgeColor','interp','FaceColor','none')
%end copy
hold on;
%plot3(0,0,0,"o");
[cX,cY,cZ] = cylinder(0.025);
cZ = cZ/20-0.04;
surf(cX,cY,cZ);

xlabel("x (m)");
ylabel("y (m)");
zlabel("z (m)");
axis([-d_limit d_limit -d_limit d_limit -0.04 d_limit]);
title(str);

num_total = size(x,1);
num_healthy = 0;
X = zeros(num_total,1);
Y = zeros(num_total,1);
Z = zeros(num_total,1);
for i = 1:num_total
    if ~isnan(x(i))
        num_healthy = num_healthy+1;
        X(num_healthy) = x(i);
        Y(num_healthy) = y(i);
        Z(num_healthy) = z(i);
    end
end

[n, V, p] = affine_fit([X Y Z]);

dev = zeros(num_healthy,1);
for i = 1:num_healthy
    v = p - [X(i) Y(i) Z(i)];
    dev(i) = dot(v,n);
end

mean(dev);

lambda = dot(n,p);

planex = [min(x) max(x)];
planey = [min(y) max(y)];

planez = zeros(2);
for i = 1:2
    for j = i:2
        planez(j,i) = -(n(1) * planex(i) + n(2) * planey(i) - lambda)/n(3);
    end
end

figure;
plot3(X,Y,Z);
hold on;
surf(planex,planey,planez);



function [d] = distance(v,fit)
    d = 0;
    for i = 0:1:(size(fit)-1)
        d = d + fit(i+1)*v^(size(fit,1)-i-1);
    end
end