function [label prob_label] = classify_segment(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model  )


feat_frame_fr = [];
for i=ini:fin
    idx = find(fr_idx_2==i);
    feat_frame_fr = [feat_frame_fr feat_video(:,idx)];
end


FV = get_fv( single(w), single(mu), single(sigma), feat_frame_fr );
lab = [ 1 ]; %%% OJO!!!!!!!!!!!!
[predicted_label, accuracy, prob_estimates] = svmpredict(double(lab'), double(FV'), model, ['-b 1']);
label = predicted_label;

%display('Mirar bien los labels :(');
%Mis labels empiezan en Uno. En Matlab los arreglos se enumeran desde uno
if (label <=0)
prob_label = -1;
else
prob_label = prob_estimates(label);
end



