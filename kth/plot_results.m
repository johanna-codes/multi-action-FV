%Overlapping Segments and each two frames.
close all
clear all
clc
kth_overseg = [84.1	85.0	81.8	75.9];
t = [ 0.5 1 1.5 2];
%subplot(1,2,1)
figure(1)
plot(t, kth_overseg, '-rd','LineWidth',5,'MarkerSize',10)
axis([0.4,2.1,74,85])
%title('stitched KTH dataset','FontSize',16,'FontWeight','bold');
%title('K = 256','FontSize',30,'FontWeight','bold');
legend('K = 256','FontSize',30,'FontWeight','bold', 'Location','south');
set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
ylabel('Performance (%)','FontSize',24) % y-axis label
xlabel({'Segment Length (seconds)'; ' '},'FontSize',24) % x-axis label






 figure(2)
  kth_overseg_2 = [83.4	84.5	85.0	84.8];
  Ng = [ 64 128 256 512];
  %subplot(1,2,2)
  %plot(Ng, kth_overseg_2, '-bd','LineWidth',5,'MarkerSize',10)  
  semilogx(Ng, kth_overseg_2, '-bd','LineWidth',5,'MarkerSize',10)

  axis([60,514,83,85])
  %title('stitched KTH dataset','FontSize',14,'FontWeight','bold');
  %title('L = 1s','FontSize',30,'FontWeight','bold');  
  legend('L = 1s','FontSize',30,'FontWeight','bold','Location','south');
  xlabel({'Number of Gaussians (K)', ' '},'FontSize',24 ) % x-axis label
  ylabel('Performance (%)','FontSize',24) % y-axis label
  set(gca,'XTick',Ng); % Change x-axis ticks
  set(gca,'XTickLabel',Ng, 'FontSize',20); % Change x-axis ticks labels to desired values.

 
 
 %ha = axes('Position',[0 0 1 1],'Xlim',[0 1],'Ylim',[0 1],'Box','off','Visible','off','Units','normalized', 'clipping' , 'off');
 %text(0.5, 1,'stitched KTH dataset','HorizontalAlignment', 'center','VerticalAlignment', 'top', 'FontSize',30, 'FontWeight','bold')

 

