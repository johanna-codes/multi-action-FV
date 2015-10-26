function FV_cmu_training_2(Ncent, RUN)
%%Calcular 2 FVs for each video
%Doing for all videos

fprintf('Ng %d \n',Ncent);

for r=1:RUN
    run = int2str(r);
    Ng = int2str(Ncent);
    dim = int2str(14);
    %fprintf('FV for training data RUN %s \n',run);
    
    w  =    load(strcat('./run', run, '/universal_GMM/weights_Ng', Ng, '_dim', dim ));
    mu =    load(strcat('./run', run, '/universal_GMM/means_Ng'  , Ng, '_dim', dim ));
    sigma = load(strcat('./run', run, '/universal_GMM/covs_Ng'   , Ng, '_dim', dim ));
    
    people_train = importdata(strcat('./run',run,'/train_list_run',run,'.dat'));
    actionNames = importdata('actionNames.txt');
    
    
    n_people  = length(people_train);
    n_actions = length(actionNames);
    
    for i=1:n_people
        for j=1:n_actions
            name_feat = strcat('./features_training/feature_vectors_dim',dim, '_', people_train{i},'_',actionNames{j}, '.dat');
            display('*********************');
            fprintf('FV for training data RUN %s \n',run);
            display(strcat( people_train{i},'_',actionNames{j} ));
            S = char(name_feat);
            %data_onevideo = load(S);
            %[ di le] = size(data_onevideo);
            one_video = {load(S)};
            [ di le] = size(one_video{1}); % I only have one
            fprintf('le %d \n',le);
            if (le>3000000) %10.000.000
                %display(strcat( people_train{i},'_',actionNames{j} ));
                display('Matrix so big');
                max_n_frames = 1000000; %floor(le/2);
                one_video{1,1}(:,max_n_frames:end)=[];
            end
            
            if ( le>0 )
                
                [di le] = size(one_video{1,1}); % I only have one
                
                half = floor(le/2);
                fv_limits_1 = [ 1:half];
                fv_limits_2 = [ (half+1):le];
                
                
                %FV for first half
                vn_1 = get_fv( w, mu, sigma, one_video{1,1}(:,fv_limits_1) );
                save_name_1 = char( strcat('./run',run,'/FV_training_2/FV',int2str(1),'_', people_train(i),'_',actionNames(j), '_Ng', Ng, '.txt') );
                
                %FV for second half
                vn_2 = get_fv( w, mu, sigma, one_video{1,1}(:,fv_limits_2) );
                save_name_2 = char( strcat('./run',run,'/FV_training_2/FV',int2str(2),'_', people_train(i),'_',actionNames(j), '_Ng', Ng, '.txt') );
                
                display('Saving');
                
                fid1=fopen(save_name_1,'wt');
                fprintf(fid1,'%8.8f\n',vn_1);
                fclose(fid1);            
                
                fid2=fopen(save_name_2,'wt');
                fprintf(fid2,'%8.8f\n',vn_2);
                fclose(fid2);
            
        
            end
                    clear vn_1 vn_2 fid1 fd2;
        end
        
    end
end
