addpath('/home/johanna/codes/FV_Matlab/libsvm-3.20/matlab')
clear all
close all
clc

run = int2str(1);
%sc = int2str(4);

people = importdata(strcat('./run', run, '/rand_selection_run', run, '.dat'));
actionNames = importdata('actionNames.txt');

people_train = people(1:16);
people_test = people(17:25);
n_pe_tr  = length(people_train);
n_pe_te  = length(people_test);
n_actions = length(actionNames);

data_train = [];
labels_train = [];
data_test = [];
labels_test = [];


%% TRAINING
%  Loading Training data
display('training');
for d = 1:4
    sc = int2str(d);
    for pe = 1: n_pe_tr
        
        for act = 1:n_actions
            
            load_name = strcat('./run', run,  '/FV_training/FV_', people_train(pe),'_',actionNames(act),'_sc', sc, '.txt');
            sLoad = char(load_name);
            FV = load(sLoad);
            data_train = [data_train FV];
            labels_train = [labels_train (act)];
            
        end
    end
end

data_train = data_train';
labels_train = labels_train';

model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);

%% TESTING. This testing is only for single actions
%  Loading Testing data
display('TESTING');
for d = 1:4
    sc = int2str(d);
    for pe = 1: n_pe_te
        
        for act = 1:n_actions
             if ( ~(pe == 7 && act==5 && d==3)  )%pe=7 --> 'person13', handclapping
            load_name = strcat('./run', run,  '/FV_testing/FV_', people_test(pe),'_',actionNames(act),'_sc', sc, '.txt');
            sLoad = char(load_name);
            FV = load(sLoad);
            %hist(FV)
            %pause
            data_test = [data_test FV];
            labels_test = [labels_test (act)];
             end
            
        end
    end
end

data_test = data_test';
labels_test = labels_test';


[predicted_label, accuracy, prob_estimates] = svmpredict(labels_test, data_test, model, ['-b 1']);
%acc = 100*sum(results ==labels_test)/length(labels_test)


