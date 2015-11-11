clear all
clc

%%
prompt = 'Ng? ';
Ng = input(prompt);

prompt = 'Segment Length? ';
L = input(prompt);


perf = zeros(1,12);
for r=1:12
    load_name = strcat('./run',int2str(r),'/SysA_test_info_ONEsvm_Ng', int2str(L),'_L',int2str(L),'.mat');
    sLoad= char(load_name);
    if  (exist(sLoad))
        load(sLoad);
        perf(r) = test_info{4};
    end
    
end


perf'
mean(perf)