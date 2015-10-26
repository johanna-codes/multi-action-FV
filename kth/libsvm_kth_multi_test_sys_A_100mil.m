clear all
close all
clc

%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');

%Home
addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab');
addpath('/media/johanna/HD1T/Toolbox/yael/matlab');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'What is number of Gaussians?? ';
Ncent = input(prompt);
%prompt = 'What is segment length? ';
L = 25
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
RUN = 1;
DIM =14;
Ng = int2str(Ncent);
dim = int2str(DIM);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Precalculate FV_training set
%display('Precaculating FVs')
%FV_kth_all_sc_training_100000(Ncent, RUN,DIM);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% TRAINING

for r=1:RUN
%fprintf('RUN %d \n', r);
run = int2str(r);

%sc = int2str(4);

people = importdata(strcat('./run', run, '/rand_selection_run', run, '.dat'));
actionNames = importdata('actionNames.txt');
people_train = people(1:16);


n_pe_tr  = length(people_train);
n_actions = length(actionNames);

data_train = [];
labels_train = [];

%display('Loading Training data');
for d = 1:4
    sc = int2str(d);
    for pe = 1: n_pe_tr

        for act = 1:n_actions

            load_name = strcat('./run', run,  '/Compu_Cost_FV_training_100mil/FV_', people_train(pe),'_',actionNames(act),'_sc', sc, '_Ng', Ng, '.txt');
            sLoad = char(load_name);
            FV = load(sLoad);
            data_train = [data_train FV];
            labels_train = [labels_train (act - 1)]; % labels starts at 0

        end
    end
end

data_train = data_train';
labels_train = labels_train';
%display('Training...');
model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
save_name = strcat('./run', run,  '/Compu_Cost_100mil_svm_model_Ng', Ng);
sSave= char(save_name);
display('Saving Model...');
save(sSave, 'model');
end

%% TESTING.
all_run_acc = [];
tic
for r=1:RUN
%fprintf('RUN %d \n', r);
run = int2str(r);
people = importdata(strcat('./run', run, '/rand_selection_run', run, '.dat'));
actionNames = importdata('actionNames.txt');
people_test = people(17:25);
people_test =  people(23); %Obtaining computational cost

n_pe_te  = length(people_test);
n_actions = length(actionNames);

data_test = [];
labels_test = [];

%display('Loading SVM Model'); % loading 'model'
load_name = strcat('./run', run,  '/Compu_Cost_100mil_svm_model_Ng', Ng);
sLoad= char(load_name);
load(sLoad);


w  =    load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/weights_Ng', Ng, '_dim', dim,'_all_sc.dat'));
mu =    load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/means_Ng'  , Ng, '_dim', dim,  '_all_sc.dat'));
sigma = load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/covs_Ng'   , Ng, '_dim', dim,'_all_sc.dat'));
sc = 4; %Total # of scenarios
ACC = [];


%test_info =  cell(sc*n_pe_te,4);
k=1;

for d = 1:1%sc
    sc = int2str(d);
    for pe = 1:1%n_pe_te
        %display('Loading data');
        
        %Loading matrix with all features (for all frames)
        %display(strcat(people_test(pe),'_d', sc));
        load_name = strcat('./run', run,  '/multi_features/feat_', people_test(pe),'_d', sc, '.dat')
        sLoad = char(load_name);
        feat_video = load(sLoad);
        
        %Loading labels. In a frame basis
        load_name_lab = strcat('./run', run,  '/multi_features/lab_', people_test(pe),'_d', sc, '.dat');
        sLoad_lab = char(load_name_lab);
        real_labels = load(sLoad_lab);
        n_frames = length(real_labels);
        est_labels  = ones (length(real_labels),1)*(-1);
        
        %Loading frame index for each of the feature vector in feat_video
        load_name_fr_idx= strcat('./run', run,  '/multi_features/fr_idx_', people_test(pe),'_d', sc, '.dat');
        sLoad_fr_idx = char(load_name_fr_idx);
        fr_idx = load(sLoad_fr_idx); % Solo uso las pares: 2,4,6...
        fr_idx_2 = fr_idx/2; % Empieza en uno
        
        prob_frames = zeros(n_frames, n_actions);
        %display('Data loaded');
        
        for f=1:n_frames - L
            ini = f;
            fin = ini + L;            
            prob_estimates = classify_segment_sysA(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model  );
            n_rows = length(f:f+L);
            prob_frames(f:f + L,:)=prob_frames(f:f + L,:) + repmat(prob_estimates, [n_rows 1]);
        end
        
         for f=1:n_frames
             
             frame_f = prob_frames(f,:);
             [maxi posi]= max(frame_f );
             %clases empeizan en 0 o en 1
             est_labels(f) = model.Label(posi);
         end
    
        
      acc = sum(est_labels == real_labels)*100/length(real_labels);
      ACC =  [ACC acc]
      %test_info{k,1} = strcat(people_test(pe));
      %test_info{k,2} = real_labels;
      %test_info{k,3} = est_labels;
      %test_info{k,4} = acc;
      
     % k = k+1;
        
    end
end
%display('Saving results');
%save_info = strcat('./run', run, '/Comp_Cost_100mil_SysA_test_info_ONEsvm_Ng', Ng, '_L', int2str(L), '.mat');
%sSave_info= char(save_info);
%save(sSave_info, 'test_info');

%all_run_acc = [all_run_acc mean(ACC)];
toc
end

