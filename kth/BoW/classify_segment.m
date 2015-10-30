function [ prob_estimates] = classify_segment(ini, fin, fr_idx_2, feat_video, mu, model  )


feat_frame_fr = [];
for i=ini:fin
    idx = find(fr_idx_2==i);
    feat_frame_fr = [feat_frame_fr feat_video(:,idx)];
end


FV = get_hist(feat_frame_fr, mu);
lab = [ 1 ]; %%% OJO!!!!!!!!!!!!
[predicted_label, accuracy, prob_estimates] = svmpredict(double(lab'), double(FV'), model, ['-b 1']);




