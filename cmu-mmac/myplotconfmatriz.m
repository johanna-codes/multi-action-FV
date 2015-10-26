function C = myplotconfmatriz(n_actions,real_labels,est_labels );

n_actions = 14;
n_frames  =  length(real_labels);
mat_re_lab = zeros(n_actions, n_frames);
mat_est_lab = zeros(n_actions, n_frames);

for i=1:n_frames
   pos_real =  ( real_labels(i) );
   pos_est  =  ( est_labels(i)  );
   
   
   mat_re_lab(pos_real,i) = 1;
   mat_est_lab(pos_est,i) = 1;
   
end

plotconfusion(mat_re_lab,mat_est_lab);
[C,order] = confusionmat(real_labels,est_labels);