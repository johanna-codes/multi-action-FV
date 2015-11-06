function BoW_train(RUN, Ng, path_run_folders)


for r=1:RUN
    run = int2str(r);
    fprintf('Training RUN %d \n', r);
    
    
    people_train = importdata(strcat(path_run_folders,'/run', run, '/train_list_run', run, '.dat'));
    actionNames = importdata('actionNames.txt');
    n_pe_tr  = length(people_train);
    n_actions = length(actionNames);
    
    data_train = [];
    labels_train = [];
    %data_test = [];
    %labels_test = [];
    
    
    %% TRAINING
    display('Loading Training data');
    for pe = 1: n_pe_tr
        
        for act = 1:n_actions
            
            load_name = strcat('./run', run,  '/Histograms_BoW_OpenCV/hist_', people_train(pe),'_',actionNames(act), '_Ng', Ng, '.h5');
            sLoad = char(load_name);
            
            if (exist(sLoad))
                hinfo = hdf5info(sLoad);
                hist = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                data_train = [data_train hist];
                labels_train = [labels_train (act)];
            end
        end
    end
    
    data_train = data_train';
    labels_train = labels_train';
    display('Training...');
    model = svmtrain(labels_train, data_train, ['-s 0 -t 2 -b 1 -g 0.005 -c 600' ]);
    save_name = strcat('./run', run,  '/svm_hist_model_Ng', Ng);
    display('Saving Model...');
    sSave= char(save_name);
    save(sSave, 'model');
end
