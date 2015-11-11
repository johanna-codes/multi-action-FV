clear all
close all
clc

%data [ training_sysA training_sysB; test_one_video_sysA test_one_video_sysB]
data = [ 501.6 67.6; 184.1 4.2];
%figure, bar([1; 2], data, 1)
figure, bar(data)
axis([0.5,2.5,0,510])
leg = {'training', 'testing'};
set(gca,'XTickLabel', leg);
set(gca,'XTickLabel',leg, 'FontSize',20); % Change x-axis ticks labels to desired values.
%ylabel('Performance (%)','FontSize',24, 'FontWeight','bold') % y-axis label
ylabel({'Time (minutes)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label

legend('proposed method','stochastic modelling', 'FontSize',30,'FontWeight','bold', 'Location','northeast');
colormap summer

%figure(2)
%subplot(2,2,[1 2 ]);
[im_hatch,colorlist] = applyhatch_pluscolor(1,'.x',1,[1 1]);



 %%Computational Cost mlsda_system and fv_system
figure
subplot(1,2,1)
mlsda = [501.6 184.1];
pie(mlsda)
colormap cool
legend({'Modelling Actions','Testing with One video'}, 'FontSize',15,'FontWeight','bold', 'Location','NorthOutside');


subplot(1,2,2)
fv_system = [24.6 42.7 0.3 4.2];
pie(fv_system)
colormap jet
legend({'Visual Dict (GMM)','FV training videos', 'SVM training', 'Testing with One Video'}, 'FontSize',15,'FontWeight','bold', 'Location','SouthOutside');






