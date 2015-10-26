%%Calcular FV for all videos in the testing set
%% This FVs are only used to see the performance in the single-action recognition test
clear all
close all
clc
%Add path where the Fisher Implementation is stored
addpath('/home/johanna/toolbox/yael/matlab');
run = int2str(1);
Ng = int2str(256);
dim = int2str(14);

 w  =    load(strcat('./run', run, '/universal_GMM/weights_Ng', Ng, '_dim', dim,'_all_sc.dat'));
 mu =    load(strcat('./run', run, '/universal_GMM/means_Ng'  , Ng, '_dim', dim,  '_all_sc.dat'));
 sigma = load(strcat('./run', run, '/universal_GMM/covs_Ng'   , Ng, '_dim', dim,'_all_sc.dat'));

for d=1:4 %%OJO!!!!!!!!!!
    
    sc = int2str(d);
    people = importdata(strcat('./run', run, '/rand_selection_run', run, '.dat'));
    actionNames = importdata('actionNames.txt');
    people_test = people(17:25);

    
    n_people  = length(people_test);
    n_actions = length(actionNames);
    
    for i=1:n_people
        for j=1:n_actions
            if ( ~(i == 7 && j==5 && d==3)  )%i=7 --> 'person13'
            name_feat = strcat('./features_all_nor/feat_vec_', people_test{i},'_',actionNames{j},'_d', sc);
            
            S = char(name_feat)
            data_onevideo = load(S);
            one_video = {data_onevideo};
            
            v = compute_fisher_joha (single(w), single(mu), single(sigma), one_video);
            d_fisher = size (v, 1);              % dimension of the Fisher vectors
            
            % power "normalisation"
            v = sign(v) .* sqrt(abs(v));
            
            %L2 normalization (may introduce NaN vectors)
            vn = yael_fvecs_normalize (v);
            
            % replace NaN vectors with a large value that is far from everything else
            % For normalized vectors in high dimension, vector (0, ..., 0) is *close* to
            % many vectors.
            %vn(find(isnan(vn))) = 123;
            
            if ( length( find( isnan(vn) ) )> 0 )
                disp('Que hago??????');
                
            end
            %to save
            
            save_name = strcat('./run', run,  '/FV_testing/FV_', people_test(i),'_',actionNames(j),'_sc', sc, '.txt');
            sSave = char(save_name);
            display(sSave);
            fid1=fopen(sSave,'wt');
            fprintf(fid1,'%8.8f\n',vn);
            fclose(fid1);
            
            %save(sSave, 'vn', '-ascii');
        end
        end
        
    end
    
    %feat_vec_person02_boxing_d1
end
