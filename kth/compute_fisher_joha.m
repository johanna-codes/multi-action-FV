% Compute the Fisher descriptors for a set of images
% Usage: V = compute_fisher (gmm, S)
%
% where
%   gmm     is the dictionary of centroids 
%   S       is a cell structure. Each cell is a set of descriptors for an image
%
% Both centroids and descriptors are stored per column
function V = compute_fisher (w, mu, sigma, S); 

nimg = length (S);
k = size (mu, 2);
d = size (mu, 1);
%V = zeros (k * ( 2*d +1) - 1, nimg, 'single'); %means and covs

V = zeros (k * 2*d, nimg, 'single'); %means and covs
%V = zeros (k * d, nimg, 'single');%only means
%V = zeros (k - 1, nimg, 'single');%only weights

for i = 1:nimg
  
  descs = single(S{i});

  V(:, i) = yael_fisher (descs, w', mu, sigma, 'sigma', 'nonorm');
    
end
