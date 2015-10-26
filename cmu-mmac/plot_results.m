%Overlapping Segments and each two frames.
close all
clear all
clc
cmu_overseg = [39.60 40.75 40.49];
t = [ 1 2 3];
%subplot(1,2,1)
figure(1)
plot(t, cmu_overseg, '-bd','LineWidth',5,'MarkerSize',10)
axis([0.9,3.1,38,41])
title('CMU-MMAC dataset','FontSize',24,'FontWeight','bold');
%title('K = 256','FontSize',30,'FontWeight','bold');
legend('K = 64','FontSize',30,'FontWeight', 'Location','south');
set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
ylabel('Performance (%)','FontSize',24,'FontWeight','bold') % y-axis label
xlabel({'Segment Length (seconds)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label






 
 
 %ha = axes('Position',[0 0 1 1],'Xlim',[0 1],'Ylim',[0 1],'Box','off','Visible','off','Units','normalized', 'clipping' , 'off');
 %text(0.5, 1,'stitched KTH dataset','HorizontalAlignment', 'center','VerticalAlignment', 'top', 'FontSize',30, 'FontWeight','bold')

 

