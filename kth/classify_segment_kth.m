function [label prob_label] = classify_segment_kth(ini, fin, fr_idx_2, feat_video, w, mu, sigma, kth_models  )


feat_frame_fr = [];
for i=ini:fin
    idx = find(fr_idx_2==i);
    feat_frame_fr = [feat_frame_fr feat_video(:,idx)];
end


FV = get_fv( single(w), single(mu), single(sigma), feat_frame_fr );
lab = [ 1 ]; %%% OJO!!!!!!!!!!!!
predicted_label= zeros(4,1);
prob_estimates = zeros(4,1);


for m=1:4 % 4 scenarios in the KTH dataset
    [tmp_pred, accuracy, tmp_prob] = svmpredict(double(lab'), double(FV'), kth_models{m}.model, ['-b 1']);
    
    predicted_label(m) = tmp_pred;
    prob_estimates(m) = tmp_prob(tmp_pred + 1);
end



mo = mode(predicted_label);
n_mo = length(find(predicted_label==mo));

if (n_mo ==4)% if all 4 labels are equal
    
    label = mo;
    prob_label = 1;
    
elseif(n_mo==3)
    
    label = mo;
    prob_label = 0.75;
elseif (n_mo==2)
    label = mo;
    prob_label = 0.5;
else
    [maxi ind] = max (prob_estimates);
    label = predicted_label(ind);
    prob_label = maxi;
    
end
    


if (label <0)
display('is it possible???')
pause;
end



