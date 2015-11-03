function BoW_train(Ncent,RUN,DIM,path_run_folders)

Ng = int2str(Ncent);
dim = int2str(DIM);



% %% TRAINING
for r=1:RUN
fprintf('RUN %d \n', r);
run = int2str(r);

%sc = int2str(4);

people = importdata(strcat(path_run_folders, '/run', run, '/rand_selection_run', run, '.dat'));
actionNames = importdata('actionNames.txt');
people_train = people(1:16);


n_pe_tr  = length(people_train);
n_actions = length(actionNames);

data_train = [];
labels_train = [];

display('Loading Training data');
for d = 1:4
    sc = int2str(d);
    for pe = 1: n_pe_tr

        for act = 1:n_actions

            load_name = strcat('./run', run,  '/Histograms_BoW_OpenCV/hist_', people_train(pe),'_',actionNames(act),'_d', sc, '_Ng', Ng, '.h5');
            sLoad = char(load_name);
            hinfo = hdf5info(sLoad);
            hist = hdf5read(hinfo.GroupHierarchy.Datasets(1));
            data_train = [data_train hist];
            labels_train = [labels_train (act - 1)]; % labels starts at 0

        end
    end
end

data_train = data_train';
labels_train = labels_train';
display('Training...');
model = svmtrain(labels_train, data_train, ['-s 0 -t 2 -b 1 -g 0.0005 -c 600' ]); %CvSVM::RBF
save_name = strcat('./run', run,  '/svm_hist_model_Ng', Ng);
sSave= char(save_name);
display('Saving Model...');
save(sSave, 'model');
end