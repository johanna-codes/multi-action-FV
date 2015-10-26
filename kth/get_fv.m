function nor_fv = get_fv(w, mu, sigma, data)
%data is the matrix with all feature vectors to calculate ONE Fisher Vector
S = {data};

fv = compute_fisher_joha (single(w), single(mu), single(sigma), S);
d_fisher = size (fv, 1);              % dimension of the Fisher vectors
% power "normalisation"
fv = sign(fv) .* sqrt(abs(fv));
%L2 normalization (may introduce NaN vectors)
nor_fv = yael_fvecs_normalize (fv);