%%
clear all
clc 
close all

Silver=   [0.75	0.75 	0.75];
Gray=     [0.5	0.5	0.5];
Black=    [0	0	0];
Red=   	  [1	0	0];
Maroon=   [0.5	0	0];
Yellow=   [1	1	0];
Olive=    [0.5	0.5	0];
Lime=     [0	0.5	0];
Green=    [0	1	0];
Aqua=     [0	1	1];
Teal=     [0	0.5	0.5];
Blue=     [0	0	1];
Navy=     [0	0	0.5];
Fuchsia=  [1	0	1];
Purple=   [0.5	0	0.5];
Orange = [1 0.5 0];


C = {Purple, Gray, Orange,Red,Maroon,Yellow,Olive,Lime,Green,Aqua,Teal,Blue,Navy,Fuchsia,Purple};

%% BoW

load CMU_BoW_test_info_Ng128_L45_run12.mat


BoW_real_labels_cmu = test_info{1,2} ;


BoW_est_labels_cmu  = test_info{1,3} ;


%REAL LABELS
for i=1:length(BoW_real_labels_cmu)    
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{BoW_real_labels_cmu(i)}, 'EdgeColor',C{BoW_real_labels_cmu(i)},'LineWidth',3); 
   hold on
end

axis([-50, length(BoW_real_labels_cmu)+50, -0.1, 0.6])
set(gca, 'XTickLabelMode', 'manual', 'XTickLabel', []);
set(gca, 'YTickLabelMode', 'manual', 'YTickLabel', []);
set(gca,'Visible','off')

%To save
width = 5;     % Width in inches
height = 4;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_real_labels','-depsc2','-r300');



%Estimate Labels
figure
for i=1:length(BoW_est_labels_cmu)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{BoW_est_labels_cmu(i)}, 'EdgeColor',C{BoW_est_labels_cmu(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(BoW_est_labels_cmu)+50, -0.1, 0.6])
set(gca, 'XTickLabelMode', 'manual', 'XTickLabel', []);
set(gca, 'YTickLabelMode', 'manual', 'YTickLabel', []);
set(gca,'Visible','off')

%To save
width  = 5;     % Width in inches
height = 4;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_est_labels_BoW','-depsc2','-r300');

%% FV

load CMU_FV_test_Ng128_L45_run12.mat
FV_real_labels_cmu = test_info{1,2} ;
FV_est_labels_cmu  = test_info{1,3} ;


acc = sum(BoW_real_labels_cmu == FV_real_labels_cmu)*100/length(BoW_real_labels_cmu);



%Estimate Labels
figure
for i=1:length(FV_est_labels_cmu)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{FV_est_labels_cmu(i)}, 'EdgeColor',C{FV_est_labels_cmu(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(FV_est_labels_cmu)+50, -0.1, 0.6])
set(gca, 'XTickLabelMode', 'manual', 'XTickLabel', []);
set(gca, 'YTickLabelMode', 'manual', 'YTickLabel', []);
set(gca,'Visible','off')

%To save
width  = 5;     % Width in inches
height = 4;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_est_labels_FV','-depsc2','-r300');

 



