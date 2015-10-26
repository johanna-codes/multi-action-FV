function plot_both(real,est)
figure
 for j=1:length(est) 
     hold on
     if (est(j)==0)
         plot(j,est(j), 'rs', 'MarkerSize',3 );
        
     elseif (est(j)==1)
         plot(j,est(j), 'bs','MarkerSize',3 );
         
     elseif (est(j)==2)
         plot(j,est(j), 'gs'); 
         
     elseif (est(j)==3)
         plot(j,est(j), 'ms', 'MarkerSize',3 ); 
         
     elseif (est(j)==4)
         plot(j,est(j), 'cs','MarkerSize',3 ); 
         
     elseif(est(j)==5)
         plot(j,est(j), 'ys','MarkerSize',3 ); 
         
     else
         print('MAL');
         %plot(i,person18_est(i), 'k*');
     end
  %plot (real,'ks');
  
        
 end
 plot (real,'ks');