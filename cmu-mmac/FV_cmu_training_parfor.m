function FV_cmu_training(Ncent, RUN)
%%Calcular FV for all videos

fprintf('Ng %d \n',Ncent);




l =1;
actionNames = importdata('actionNames.txt')
ACT =  length(actionNames);

total = RUN*(RUN-1)*ACT;
parfor_variables  = cell(total,3);

for rr=1:RUN 
    run_rr = int2str(rr); 
    people_train = importdata(strcat('./run',run_rr,'/train_list_run',run_rr,'.dat'));
    PEO  = length(people_train);  
    for ii=1:PEO
        for jj=1:ACT
                     
            parfor_variables {l,1} = run_rr; %string
            parfor_variables {l,2} = people_train{ii} ;
            parfor_variables {l,3} = actionNames{jj} ;
            l = l+1;
        end
    end
end

    Ng = int2str(Ncent);
    dim = int2str(14);

parfor k=1:length(parfor_variables)
   

    run =parfor_variables{k,1};
    per_train = parfor_variables{k,2};
    action_name = parfor_variables{k,3};
    

    %fprintf('FV for training data RUN %s \n',run);
    
    w  =    load(strcat('./run', run, '/universal_GMM/weights_Ng', Ng, '_dim', dim ));
    mu =    load(strcat('./run', run, '/universal_GMM/means_Ng'  , Ng, '_dim', dim ));
    sigma = load(strcat('./run', run, '/universal_GMM/covs_Ng'   , Ng, '_dim', dim ));
    
    
    
    name_feat = strcat('./features_training/feature_vectors_dim',dim, '_', per_train,'_',action_name, '.dat');
    %display('*********************');
    %display(strcat( per_train,'_',action_name ));
    S = char(name_feat);
    %data_onevideo = load(S);
    %[ di le] = size(data_onevideo);
    one_video = {load(S)};
    [ di le] = size(one_video{1}); % I only have one
    %fprintf('le %d \n',le);
    if (le>3000000) %10.000.000
        display('Matrix so big');
        max_n_frames = 1000000; %floor(le/2);
        one_video{1,1}(:,max_n_frames:end)=[];
    end
    
    [di le] = size(one_video{1,1}); % I only have one
    
    
    if ( le>0 )
        
        %one_video = {data_onevideo};
        %data_onevideo = [];
        
        v = compute_fisher_joha (single(w), single(mu), single(sigma), one_video);
        pclear one_video;
        d_fisher = size (v, 1);              % dimension of the Fisher vectors
        % power "normalisation"
        %display('Power Normalisation');
        v = sign(v) .* sqrt(abs(v));
        %L2 normalization (may introduce NaN vectors)
        %display('L2 Normalisation');
        vn = yael_fvecs_normalize (v);
        pclear v;
        % replace NaN vectors with a large value that is far from everything else
        % For normalized vectors in high dimension, vector (0, ..., 0) is *close* to
        % many vectors.
        %vn(find(isnan(vn))) = 123;
        if ( length( find( isnan(vn) ) )> 0 )
            disp('Que hago??????');
            
        end
        
        %to save
        save_name = strcat('./run',run,'/FV_training/FV_', per_train,'_',action_names, '_Ng', Ng, '_forpar.txt');
        
        sSave = char(save_name);
        %display('Saving');
        
        fid1=fopen(sSave,'wt');
        fprintf(fid1,'%8.8f\n',vn);
        fclose(fid1);
    end
    pclear vn fid1;
    
end
