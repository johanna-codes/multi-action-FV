function [ prob_estimates] = classify_segment_sysA(ini, fin, fr_idx_2, feat_video, w, mu, sigma, model  )


feat_frame_fr = [];
for i=ini:fin
    idx = find(fr_idx_2==i);
    feat_frame_fr = [feat_frame_fr feat_video(:,idx)];
end


FV = get_fv( single(w), single(mu), single(sigma), feat_frame_fr );
lab = [ 1 ]; %%% OJO!!!!!!!!!!!!
[predicted_label, accuracy, prob_estimates] = svmpredict(double(lab'), double(FV'), model, ['-b 1']);




