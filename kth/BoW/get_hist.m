function bin = get_hist(data_onevideo,mu)

[dim n_mu] = size (mu)
[dim2 n_vectors] =  size(data_onevideo)

hist = zeros(n_mu,1);

for  i=1:n_vectors
 
    one_vector = data_onevideo(:,i);

prev_dist = inf;

for m=1:n_mu
   
    dist = norm(one_vector - mu(:,m),2);
    
    if dist < prev_dist
        prev_dist = dist;
        bin = m;
    end
    
    
end
hist(bin) = hist(bin) + 1;

end

save('probando.mat', 'hist');