%Overlapping Segments and each two frames.
close all
clear all
clc

%% KTH
kth_overseg_1 = [84.1	85.0    81.8	75.9]; % 256
kth_overseg_2 = [82.7	83.4	80.3	74.4]; % 64
t = [ 0.5 1 1.5 2];
%subplot(1,2,1)
figure(1)
plot(t, kth_overseg_1, '-rx','LineWidth',3,'MarkerSize',10)
hold on
plot(t, kth_overseg_2, '-bd','LineWidth',3,'MarkerSize',10)
grid
axis([0.4,2.1,74,85.5])
title('s-KTH dataset','FontSize',24,'FontWeight','bold');
legend('K = 256','K = 64', 'FontSize',30,'FontWeight','bold', 'Location','southwest');
set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
ylabel('Performance (%)','FontSize',24, 'FontWeight','bold') % y-axis label
xlabel({'Segment Length (seconds)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label



%% CMU
cmu_overseg_1 = [32.2  33.1 33.8 34.0 34.5 34.6	];  %256
cmu_overseg_2 = [37.6  39.6 40.3 40.7 40.9 40.5 ];  %64


t = [0.5 1 1.5 2 2.5 3];
figure(2)
plot(t, cmu_overseg_1, '-rx','LineWidth',3,'MarkerSize',10)
hold on
plot(t, cmu_overseg_2, '-bd','LineWidth',3,'MarkerSize',10)
grid
axis([0.4,3.1,29,41])
title('CMU-MMAC dataset','FontSize',24,'FontWeight','bold');
legend('K = 256','K = 64','FontSize',30,'FontWeight', 'Location','southeast');
set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
ylabel('Performance (%)','FontSize',24,'FontWeight','bold') % y-axis label
xlabel({'Segment Length (seconds)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label


%%To save in eps
%figure(1)
%print -depsc2 /tmp/figure1.eps2

%figure(2)
%print -depsc2 /tmp/figure2.eps2


%In the ubuntu terminal:
%epstopdf figure2.eps2 %To convert to pdf


 
 
 %ha = axes('Position',[0 0 1 1],'Xlim',[0 1],'Ylim',[0 1],'Box','off','Visible','off','Units','normalized', 'clipping' , 'off');
 %text(0.5, 1,'stitched KTH dataset','HorizontalAlignment', 'center','VerticalAlignment', 'top', 'FontSize',30, 'FontWeight','bold')

 

