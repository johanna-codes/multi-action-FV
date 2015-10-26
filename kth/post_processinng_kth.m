clear all
close all
clc
load test_info.mat
%load test_info_February24.mat
ACC =[];
sm_ACC =[]; %smooth accuracy
L = 13;

for k=1:length(test_info)
est  = test_info{k,2};
real =  test_info{k,3};
%plot_both(real,est)

acc = (sum( (est==real) )/length(est) )*100;
smooth_acc = post_processing (est, real, L);
ACC = [ACC acc];
sm_ACC = [sm_ACC smooth_acc];
end
mean(ACC )
mean(sm_ACC)