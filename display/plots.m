close all;
vals = [14.382;-0.1404;0.0005;-6e-7;-8e-10;2e-12;-2e-15];

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
   pitch(i) = tbl(i,4)/2800*2*pi;
   yaw(i) = tbl(i,3)/(2800*2)*2*pi;
   
   x(i) = cos(pitch(i))*d(i)*cos(yaw(i));
   y(i) = cos(pitch(i))*d(i)*sin(yaw(i));
   z(i) = -sin(pitch(i))*d(i);
end

plot3(x,y,z,".");
hold on;
plot3(0,0,0,"o");

xlabel("x (m)");
ylabel("y (m)");
zlabel("z (m)");

function [d] = distance(v,fit)
    d = 0;
    for i = 0:1:(size(fit)-1)
        d = d + fit(i+1)*v^i;
    end
end