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
   
    
    for d=1:4
        
        sc = int2str(d);
        people_train = importdata(strcat(path_run_folders,'/run', run, '/rand_selection_run', run, '.dat'));
        actionNames = importdata('actionNames.txt');
        people_train = people_train(1:16);
        
        
        n_people  = length(people_train);
        n_actions = length(actionNames);
        
        for i=1:n_people
            for j=1:n_actions
                name_feat = strcat(path_run_folders,'/features_all_nor/feat_vec_', people_train{i},'_',actionNames{j},'_d', sc);
                
                S = char(name_feat);
                data_onevideo = load(S);
               
                
                
                %%Calcular aca el vector de Histograms y normalisar
                
                bin = get_bin(data_onevideo,mu);
                pause
     
                hist_n
                
                save_name = strcat('./run', run,  '/Histograms_BoW/hist_', people_train(i),'_',actionNames(j),'_sc', sc, '_Ng', Ng, '.txt');
                sSave = char(save_name);
                display(sSave);
                fid1=fopen(sSave,'wt');
                fprintf(fid1,'%8.8f\n',hist_n);
                fclose(fid1);
            end
            
        end
        
    end
end
