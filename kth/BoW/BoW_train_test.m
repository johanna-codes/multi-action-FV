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
%prompt = 'What is number of Gaussians?? ';
%Ncent = input(prompt);
Ncent = 4000;
prompt = 'What is segment length? ';
L = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
RUN = 3;
DIM =14;
Ng = int2str(Ncent);
dim = int2str(DIM);



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Precalculate Histogram fortraining set
% display('Precaculating Histograms')
%It was done in OpenCV. Saved as run##/Histograms_BoW_OpenCV
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
path_run_folders = '/home/johanna/codes/codes-git/multi-action-FV/trunk/kth';

% %% TRAINING
% for r=1:RUN
% fprintf('RUN %d \n', r);
% run = int2str(r);
% 
% %sc = int2str(4);
% 
% people = importdata(strcat(path_run_folders, '/run', run, '/rand_selection_run', run, '.dat'));
% actionNames = importdata('actionNames.txt');
% people_train = people(1:16);
% 
% 
% n_pe_tr  = length(people_train);
% n_actions = length(actionNames);
% 
% data_train = [];
% labels_train = [];
% 
% display('Loading Training data');
% for d = 1:4
%     sc = int2str(d);
%     for pe = 1: n_pe_tr
% 
%         for act = 1:n_actions
% 
%             load_name = strcat('./run', run,  '/Histograms_BoW_OpenCV/hist_', people_train(pe),'_',actionNames(act),'_d', sc, '_Ng', Ng, '.h5');
%             sLoad = char(load_name);
%             hinfo = hdf5info(sLoad);
%             FV = hdf5read(hinfo.GroupHierarchy.Datasets(1));
%             data_train = [data_train FV];
%             labels_train = [labels_train (act - 1)]; % labels starts at 0
% 
%         end
%     end
% end
% 
% data_train = data_train';
% labels_train = labels_train';
% display('Training...');
% model = svmtrain(labels_train, data_train, ['-s 0 -t 2 -b 1' ]); %CvSVM::RBF
% save_name = strcat('./run', run,  '/svm_hist_model_Ng', Ng);
% sSave= char(save_name);
% display('Saving Model...');
% save(sSave, 'model');
% end

%% TESTING.
%  Loading Testing data
%Haciendo para un solo video
all_run_acc = [];
%display('OJO EMPEZANDO EN DOS');

for r=1:RUN
fprintf('RUN %d \n', r);
run = int2str(r);
people = importdata(strcat(path_run_folders, '/run', run, '/rand_selection_run', run, '.dat'));
actionNames = importdata('actionNames.txt');
people_test = people(17:25);

n_pe_te  = length(people_test);
n_actions = length(actionNames);

data_test = [];
labels_test = [];

display('Loading SVM Model'); % loading 'model'
load_name = strcat('./run', run,  '/svm_hist_model_Ng', Ng);
sLoad= char(load_name);
load(sLoad);


sc = 4; %Total # of scenarios
ACC = [];


test_info =  cell(sc*n_pe_te,4);
k=1;

for d = 1:sc
    sc = int2str(d);
    for pe = 1:n_pe_te
        %display('Loading data');
        
        %Loading labels. In a frame basis
        load_name_lab = strcat(path_run_folders,'/run', run,  '/multi_features/lab_', people_test(pe),'_d', sc, '.dat');
        sLoad_lab = char(load_name_lab);
        real_labels = load(sLoad_lab);
        n_frames = length(real_labels);
        est_labels  = ones (length(real_labels),1)*(-1);
        
        %Loading frame index for each of the feature vector in feat_video
        load_name_fr_idx= strcat(path_run_folders,'/run', run,  '/multi_features/fr_idx_', people_test(pe),'_d', sc, '.dat');
        sLoad_fr_idx = char(load_name_fr_idx);
        fr_idx = load(sLoad_fr_idx); % Solo uso las pares: 2,4,6...
        fr_idx_2 = fr_idx/2; % Empieza en uno
        
        prob_frames = zeros(n_frames, n_actions);
        display('Data loaded');
        
        for f=1:n_frames - L
            ini = f;
            fin = ini + L;                    
            %Aca cargar el histogram obtenido con OpenCV
            load_name = strcat('./run', run , '/multi_Histograms_BoW_OpenCV/multi_hist_', people_test(pe), '_d', sc, '_Ng', Ng, 'fr_', int2str(ini), '_', int2str(fin), '.h5');
            sLoad = char(load_name);
            hinfo = hdf5info(sLoad);          
            hist_segment = hdf5read(hinfo.GroupHierarchy.Datasets(1));
    
            prob_estimates = classify_segment(ini, fin, fr_idx_2, feat_video, mu, model  );
            n_rows = length(f:f+L);
            lab = [ 1 ]; %%% OJO!!!!!!!!!!!!

            [predicted_label, accuracy, prob_estimates] = svmpredict(double(lab'), double(hist_segment'), model, ['-b 1']);
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
end
display('Saving results');
save_info = strcat('./run', run, '/hist_test_info_ONEsvm_Ng', Ng, '_L', int2str(L), '.mat');
sSave_info= char(save_info);
save(sSave_info, 'test_info');

all_run_acc = [all_run_acc mean(ACC)];
end

