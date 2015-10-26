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
prompt = 'Number of Gaussians? ';
Ncent = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'What is segment length? ';
L = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
display(' ');
%display('Calculating FV for Training');
%FV_cmu_training(Ncent, RUN);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

RUN = 12;
Ng = int2str(Ncent);
dim = int2str(14);
all_run_acc = [];

% for r=1:RUN
% run = int2str(r);
% fprintf('Training RUN %d \n', r);
% 
% 
% people_train = importdata(strcat('./run', run, '/train_list_run', run, '.dat'));
% actionNames = importdata('actionNames.txt');
% n_pe_tr  = length(people_train);
% n_actions = length(actionNames);
% 
% data_train = [];
% labels_train = [];
% %data_test = [];
% %labels_test = [];
% 
% 
% %% TRAINING
% display('Loading Training data');
%     for pe = 1: n_pe_tr
% 
%         for act = 1:n_actions
% 
%             load_name = strcat('./run', run,  '/FV_training/FV_', people_train(pe),'_',actionNames(act), '_Ng', Ng, '.txt');
%             sLoad = char(load_name);
%             if (exist(sLoad))
%             FV = load(sLoad);
%             data_train = [data_train FV];
%             labels_train = [labels_train (act)];
%             end
% 
%         end
%     end
% 
% data_train = data_train';
% labels_train = labels_train';
% display('Training...');
% model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
% save_name = strcat('./run', run,  '/svm_model_Ng', Ng);
% display('Saving Model...');
% sSave= char(save_name);
% save(sSave, 'model');
% end


for r=1:RUN
%% TESTING.
%  Loading Testing data
%Haciendo para un solo video
run = int2str(r);
fprintf('Testing RUN %d \n', r);
display('Loading SVM Model'); % loading 'model'
load_name = strcat('./run', run,  '/svm_model_Ng', Ng);
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
        est_labels  = ones (length(real_labels),1)*(-1);
        
        %Loading frame index for each of the feature vector in feat_video
        load_name_fr_idx= strcat('./multi_features/fr_idx_', people_test(pe),'.dat');
        sLoad_fr_idx = char(load_name_fr_idx);
        fr_idx = load(sLoad_fr_idx); 
        display('Yo no uso todas las frames. Cada DOS');
        fr_idx_2 = fr_idx/2; % Yo no uso todas las frames. las uso cada cuatro
        ini = 1;
        fin = ini + L;
        more = true;
       while(more) % 
           
           [pre_label pre_prob_label] = classify_segment(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model);
            
           fin = fin + 1; 
           [label prob_label] = classify_segment(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model);
           
           while (label == pre_label) && ( prob_label> pre_prob_label) && ( fin+1< length(real_labels))
               %display([ini fin]);
               pre_label = label;
               pre_prob_label = prob_label;
               fin = fin + 1; 
               [label prob_label] = classify_segment(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model);
           end
  
           if (ini >= length(real_labels) )
               more =false;
           
           elseif ( fin > length(real_labels) )

               fin = length(real_labels);
               [label prob_label] = classify_segment(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model);
               est_labels (ini:length(real_labels)) = label;
               more =false;
           else 
               est_labels (ini:fin) = pre_label;
               ini = fin;
               fin = ini + L;
               %display('Que caso es este???');
           end
           
           


       end
        
       is_negative = est_labels <0;
       is_negative = est_labels(is_negative);
       if (length(is_negative) > 0 )
          display('Que pasa Aqui???') ;
          pause
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
save_info = strcat('./run', run, '/test_info_ONEsvm_Ng', Ng, '.mat');
sSave_info= char(save_info);
save(sSave_info, 'test_info');

all_run_acc = [all_run_acc ACC];
end

