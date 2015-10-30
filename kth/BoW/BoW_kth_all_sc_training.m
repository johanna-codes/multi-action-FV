function BoW_kth_all_sc_training(Ncent, RUN,DIM)
%%Calcular Histogram for all videos

path_run_folders = '/home/johanna/codes/codes-git/multi-action-FV/trunk/kth';

fprintf('Ng %d \n',Ncent);


Ng = int2str(Ncent);
dim = int2str(DIM);


for r=1:RUN
    run = int2str(r);
    fprintf('RUN %s \n',run);
    
    mu =    load(strcat('./run', run, '/visual_vocabulary/means_Ng'  , Ng, '_dim', dim,  '_all_sc.dat'));
    people_train = importdata(strcat(path_run_folders,'/run', run, '/rand_selection_run', run, '.dat'));
    actionNames = importdata('actionNames.txt');
    people_train = people_train(1:16);
    n_people  = length(people_train);
    n_actions = length(actionNames);
    
    num_iter = 4*n_people*n_actions;
    %par_for_variables= cell(num_iter,2); %(name_feat,save_name);
    par_for_name_feat = cell(num_iter,1);
    par_for_save_name = cell(num_iter,1);
    
    k=1;
    for d=1:4
        
        sc = int2str(d);
        
        
        for i=1:n_people
            for j=1:n_actions
                name_feat = strcat(path_run_folders,'/features_all_nor/feat_vec_', people_train{i},'_',actionNames{j},'_d', sc);
                save_name = strcat('./run', run,  '/Histograms_BoW/hist_', people_train(i),'_',actionNames(j),'_sc', sc, '_Ng', Ng, '.h5');
                
                par_for_name_feat{k,1} = name_feat;
                par_for_save_name{k,1} = save_name;
                
            end
        end
    end
    
    
    
    parfor p=1:num_iter
        name_feat = par_for_name_feat{p,1};
        save_name = par_for_save_name{p,1};
        
        S = char(name_feat);
        data_onevideo = load(S);
        hist_n = get_hist(data_onevideo,mu);
        
        
        sSave = char(save_name);
        display(sSave);
        my_parsave(sSave, hist_n);
        
    end
    
    
    
end

end
