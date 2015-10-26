
function [act n_fr] = n_labels_action(real_labels)

%function to analyse n_frames per action
%Input real label
%Output: 
%act: labels
%n_fr: n# of frames per each act(i)
ini_label = real_labels(1) ;

k=1;
act = [];
n_fr = [];
for i=2:length(real_labels)
    if ( real_labels(i) == ini_label)
        k=k+1;
        %pause
    else
       act =  [act ini_label];
       n_fr = [ n_fr k ]   ;
       k=0;
       ini_label = real_labels(i) ;
      %pause
    end
end
    
act =  [act ini_label];
n_fr = [ n_fr k ];

% clear all
% actionNames = importdata('actionNames.txt');
% 
% real_labels = load('/home/johanna/codes/datasets_codes/CMU-MMAC/Brownie/S24_unique_verbs_labels.dat');
% [act n_fr] = n_labels_action(real_labels);
% maxi  = max(n_fr);
% mini  = min(n_fr);
% maxi_posi = find(n_fr==maxi);
% mini_posi = find(n_fr==mini);
% [maxi mini ] 
% [actionNames( act(maxi_posi) ) actionNames( act(mini_posi) )] 
