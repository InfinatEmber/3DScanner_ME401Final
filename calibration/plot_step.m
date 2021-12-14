str = input("What file do you want to analyze? ",'s');
nums = readtable(str);
nums = nums{:,:};
s = stepinfo(nums(:,5),nums(:,7)/1000)
plot(nums(:,7),nums(:,5));
xlabel("Time (ms)");
ylabel("Angle (encoder ticks)");
%xlim([8.4 8.5]*10^4)
