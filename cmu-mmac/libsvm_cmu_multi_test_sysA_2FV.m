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
display('*********Classification as per System A***************');
display('*********Obtaining 2 Fisher Vectors per Video*********');
display('******************************************************');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'Number of Gaussians? ';
Ncent = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'What is segment length? ';
L = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%RUN = 1; %Probando solo con uno
RUN = 12;
%Ncent = 64;

actionNames = importdata('actionNames.txt');
n_actions = length(actionNames);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%display(' ');
display('Calculating 2 FVs per each Training Video');
FV_cmu_training_2(Ncent, RUN);
%display('pause');
%pause();
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Ng = int2str(Ncent);
dim = int2str(14);
all_run_acc = [];

%% TRAINING
for r=1:RUN
    run = int2str(r);
    fprintf('2FV Training RUN %d \n', r);
    people_train = importdata(strcat('./run', run, '/train_list_run', run, '.dat'));
    n_pe_tr  = length(people_train);
    data_train = [];
    labels_train = [];
    
    display('Loading Training data');
    for pe = 1: n_pe_tr
        
        for act = 1:n_actions
            for v=1:2
                load_name = strcat('./run', run, '/FV_training_2/FV',int2str(v),'_', people_train(pe),'_',actionNames(act), '_Ng', Ng, '.txt');
                sLoad = char(load_name);
                if (exist(sLoad))
                    FV = load(sLoad);
                    data_train = [data_train FV];
                    labels_train = [labels_train (act)];
                    
                end
            end
            
        end
    end
    
    data_train = data_train';
    labels_train = labels_train';
    display('Training...');
    model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
    save_name = strcat('./run', run,  '/svm_model_Ng', Ng, '2FV');
    display('Saving Model...');
    sSave= char(save_name);
    save(sSave, 'model');
end



%% TESTING.
for r=1:RUN
    run = int2str(r);
    fprintf('Testing RUN %d \n', r);
    display('Loading SVM Model'); % loading 'model'
    load_name = strcat('./run', run,  '/svm_model_Ng', Ng, '2FV');
    sLoad= char(load_name);
    display(sLoad);
    load(sLoad);
    
    
    w  =    load(strcat('./run', run, '/universal_GMM/weights_Ng', Ng, '_dim', dim ));
    mu =    load(strcat('./run', run, '/universal_GMM/means_Ng'  , Ng, '_dim', dim ));
    sigma = load(strcat('./run', run, '/universal_GMM/covs_Ng'   , Ng, '_dim', dim ));
    ACC = [];
    
    people_test = importdata(strcat('./run', run, '/test_list_run', run, '.dat'));
    actionNames = importdata('actionNames.txt');
    n_pe_te  = length(people_test);
    
    test_info =  cell(n_pe_te,4);
    k=1;

    for pe = 1:n_pe_te
        %display('Loading data');
        
        %Loading matrix with all features (for all frames)
        display(strcat(people_test(pe)));
        load_name = strcat('./multi_features/feat_', people_test(pe),'.dat');
        sLoad = char(load_name);
        feat_video = load(sLoad);
        
        %Loading labels. In a frame basis
        load_name_lab = strcat('./multi_features/lab_', people_test(pe),'.dat');
        sLoad_lab = char(load_name_lab);
        real_labels = load(sLoad_lab);
        n_frames = length(real_labels);
        est_labels  = ones (n_frames,1)*(-1);
        
        %Loading frame index for each of the feature vector in feat_video
        load_name_fr_idx= strcat('./multi_features/fr_idx_', people_test(pe),'.dat');
        sLoad_fr_idx = char(load_name_fr_idx);
        fr_idx = load(sLoad_fr_idx);
        fr_idx_2 = fr_idx/4; % Yo no uso todas las frames. las uso cada cuatro
        prob_frames = zeros(n_frames, n_actions);
        
        display('Data loaded');
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
        test_info{k,1} = strcat(people_test(pe));
        test_info{k,2} = real_labels;
        test_info{k,3} = est_labels;
        test_info{k,4} = acc;
        
        k = k+1;
    end
    
    display('Saving results');
    save_info = strcat('./run', run, '/SysA_test_info_ONEsvm_Ng_2FVs', Ng, '.mat');
    sSave_info= char(save_info);
    save(sSave_info, 'test_info');
    
    all_run_acc = [all_run_acc ACC];
    %C = myplotconfmatriz(n_actions,real_labels,est_labels );
end

