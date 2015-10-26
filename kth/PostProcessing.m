clear all
%clc
close all
mat_est_labels = importdata('est_label_list.txt');
mat_real_labels = importdata('real_label_list.txt');
ACC =[];
sm_ACC =[];

for k=1:length(mat_est_labels)
est = load(mat_est_labels{k});
real = load(mat_real_labels{k});
% figure
% for j=1:length(est) 
%     hold on
%     if (est(j)==0)
%         plot(j,est(j), 'rs', 'MarkerSize',3 );
%        
%     elseif (est(j)==1)
%         plot(j,est(j), 'bs','MarkerSize',3 );
%         
%     elseif (est(j)==2)
%         plot(j,est(j), 'gs'); 
%         
%     elseif (est(j)==3)
%         plot(j,est(j), 'ms', 'MarkerSize',3 ); 
%         
%     elseif (est(j)==4)
%         plot(j,est(j), 'cs','MarkerSize',3 ); 
%         
%     elseif(est(j)==5)
%         plot(j,est(j), 'ys','MarkerSize',3 ); 
%         
%     else
%         print('MAL');
%         %plot(i,person18_est(i), 'k*');
%     end
%  %plot (real,'ks');
%  
%        
% end
% %plot (real,'ks');

acc = (sum( (est==real) )/length(est) )*100;
smooth_acc = post_processing (est, real);
ACC = [ACC acc];
sm_ACC = [sm_ACC smooth_acc];
end
mean(ACC )
mean(sm_ACC)