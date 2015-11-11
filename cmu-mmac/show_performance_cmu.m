clear all
clc

%% 
prompt = 'Segment Length? ';
L = input(prompt);

prompt = 'last _Run? ';
last_r = input(prompt);


perf = [];
for r=1:last_r
load_name = strcat('./run',int2str(r),'/SysA_test_info_ONEsvm_Ng32_L',int2str(L),'.mat');  
sLoad= char(load_name);
load(sLoad) ;                                                                               
perf = [perf test_info{4}]; 
end


perf'