function BoW_test(Ng,RUN,L,path_run_folders)
all_run_acc = [];


for r=1:RUN
run = int2str(r);
fprintf('Testing RUN %d \n', r);
display('Loading SVM Model'); % loading 'model'
load_name = strcat('./run', run,  '/svm_hist_model_Ng', Ng);
sLoad= char(load_name);
display(sLoad);
load(sLoad);

ACC = [];

people_test = importdata(strcat(path_run_folders,'/run', run, '/test_list_run', run, '.dat'));
n_pe_te  = length(people_test);

test_info =  cell(n_pe_te,4);
k=1;


    for pe = 1:n_pe_te

        
        %Loading labels. In a frame basis
        load_name_lab = strcat(path_run_folders,'/multi_features/lab_', people_test(pe),'.dat');
        sLoad_lab = char(load_name_lab);
        real_labels = load(sLoad_lab);
        n_frames = length(real_labels);
        est_labels  = ones (n_frames,1)*(-1);
        
        %Loading frame index for each of the feature vector in feat_video
        load_name_fr_idx= strcat(path_run_folders,'/multi_features/fr_idx_', people_test(pe),'.dat');
        sLoad_fr_idx = char(load_name_fr_idx);
        fr_idx = load(sLoad_fr_idx); 
        display('Yo no uso todas las frames. Cada DOS');
        fr_idx_2 = fr_idx/2; % Yo no uso todas las frames. las uso cada dos
        prob_frames = zeros(n_frames, n_actions);
        
        display('Data loaded');
        for f=1:n_frames - L
            ini = f;
            fin = ini + L;   
            load_name = strcat('./run', run , '/multi_Histograms_BoW_OpenCV/multi_hist_', people_test(pe), '_Ng', Ng, 'fr_', int2str(ini), '_', int2str(fin), '.h5');
            sLoad = char(load_name);
            hinfo = hdf5info(sLoad);          
            hist_segment = hdf5read(hinfo.GroupHierarchy.Datasets(1));
 
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
      clear feat_video fr_idx ;
    end

display('Saving results');
save_info = strcat('./run', run, '/SysA_test_info_ONEsvm_Ng', Ng, '_L', int2str(L), '.mat');
sSave_info= char(save_info);
save(sSave_info, 'test_info');

all_run_acc = [all_run_acc ACC];
%C = myplotconfmatriz(n_actions,real_labels,est_labels );
end
all_run_acc'


