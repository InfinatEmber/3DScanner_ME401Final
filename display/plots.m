close all;
vals = [-1.07716E-20;4.35263E-17;-7.26628E-14;6.48489E-11;-3.33216E-08;9.77453E-06;-0.001465032;0.06569285;5.31114646];

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
   if d(i) < 0.1 || d(i) > 2
       d(i) = NaN;
   end
   pitch(i) = -tbl(i,2)/2800*2*pi;
   yaw(i) = tbl(i,1)/(2800*2)*2*pi;
   
   x(i) = cos(pitch(i))*d(i)*cos(yaw(i));
   y(i) = cos(pitch(i))*d(i)*sin(yaw(i));
   z(i) = sin(pitch(i))*d(i);
end

plot3(x,y,z,".");
%copied from https://www.mathworks.com/matlabcentral/answers/267468-change-colour-of-points-in-plot3-with-increasing-z-value
cla
patch([x.' nan],[y.' nan],[z.' nan],[z.' nan],'EdgeColor','interp','FaceColor','none')
%end copy
hold on;
plot3(0,0,0,"o");

xlabel("x (m)");
ylabel("y (m)");
zlabel("z (m)");
axis([-2 2 -2 2 0 2]);
title(str);

function [d] = distance(v,fit)
    d = 0;
    for i = 0:1:(size(fit)-1)
        d = d + fit(i+1)*v^(size(fit,1)-i-1);
    end
end