function FV_kth_all_sc_training_100000(Ncent, RUN,DIM)
%%Calcular FV for all videos

fprintf('Ng %d \n',Ncent);


Ng = int2str(Ncent);
dim = int2str(DIM);


for r=1:RUN
    run = int2str(r);
    fprintf('RUN %s \n',run);

    w  =    load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/weights_Ng', Ng, '_dim', dim,'_all_sc.dat'));
    mu =    load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/means_Ng'  , Ng, '_dim', dim,  '_all_sc.dat'));
    sigma = load(strcat('./run', run, '/Compu_Cost_universal_GMM_100000/covs_Ng'   , Ng, '_dim', dim,'_all_sc.dat'));
    
    
    for d=1:4
        
        sc = int2str(d);
        people_train = importdata(strcat('./run', run, '/rand_selection_run', run, '.dat'));
        actionNames = importdata('actionNames.txt');
        people_train = people_train(1:16);
        
        
        n_people  = length(people_train);
        n_actions = length(actionNames);
        
        for i=1:n_people
            for j=1:n_actions
                name_feat = strcat('./features_all_nor/feat_vec_', people_train{i},'_',actionNames{j},'_d', sc);
                
                S = char(name_feat);
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
                
                save_name = strcat('./run', run,  '/Compu_Cost_FV_training_100mil/FV_', people_train(i),'_',actionNames(j),'_sc', sc, '_Ng', Ng, '.txt');
                sSave = char(save_name);
                %display(sSave);
                fid1=fopen(sSave,'wt');
                fprintf(fid1,'%8.8f\n',vn);
                fclose(fid1);
            end
            
        end
        
    end
end
