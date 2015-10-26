function bin = get_bin(data_onevideo,mu)

[dim n_mu] = size (mu);

prev_dist = inf;

for i=1:n_mu
   
    dist = norm(data_onevideo,mu(:,i);
    
    if dist < prev_dist
        prev_dist = dist;
        bin = i;
    end
    
end

bin