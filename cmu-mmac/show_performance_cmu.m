clear all
clc

%%
prompt = 'Segment Length? ';
L = input(prompt);

prompt = 'last _Run? ';
last_r = input(prompt);


perf = zeros(1,12);
for r=1:12
    load_name = strcat('./run',int2str(r),'/SysA_test_info_ONEsvm_Ng32_L',int2str(L),'.mat');
    sLoad= char(load_name);
    if  (exist(sLoad))
        load(sLoad);
        perf(r) = test_info{4};
    end
    
end


perf'
mean(perf)