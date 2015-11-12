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

load CMU_BoW_test_info_Ng128_L60_run3.mat


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

%title('Real Labels');
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

%Home
%print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_real_labels','-depsc2','-r300');

%NICTA
%print('/home/johanna/latex-svn/assembla/papers-svn/wacv_2016_a/images/segmentation/CMU_real_labels','-depsc2','-r300');




%Estimate Labels
figure
for i=1:length(BoW_est_labels_cmu)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{BoW_est_labels_cmu(i)}, 'EdgeColor',C{BoW_est_labels_cmu(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(BoW_est_labels_cmu)+50, -0.1, 0.6])
set(gca, 'XTickLabelMode', 'manual', 'XTickLabel', []);
set(gca, 'YTickLabelMode', 'manual', 'YTickLabel', []);
%title('Est labels for BoW');
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

%Home
%print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_est_labels_BoW','-depsc2','-r300');

%NICTA
%print('/home/johanna/latex-svn/assembla/papers-svn/wacv_2016_a/images/segmentation/CMU_est_labels_BoW','-depsc2','-r300');




%% FV

load CMU_FV_test_info_Ng128_L60_run3.mat
FV_real_labels_cmu = test_info{1,2} ;
FV_est_labels_cmu  = test_info{1,3} ;

%acc = sum(BoW_real_labels_cmu == FV_real_labels_cmu)*100/length(BoW_real_labels_cmu);



%Estimate Labels
figure
for i=1:length(FV_est_labels_cmu)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{FV_est_labels_cmu(i)}, 'EdgeColor',C{FV_est_labels_cmu(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(FV_est_labels_cmu)+50, -0.1, 0.6])
set(gca, 'XTickLabelMode', 'manual', 'XTickLabel', []);
set(gca, 'YTickLabelMode', 'manual', 'YTickLabel', []);
%title('Est Labels for FV')
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

%Home
%print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/CMU_est_labels_FV','-depsc2','-r300');
%NICTA
%print('/home/johanna/latex-svn/assembla/papers-svn/wacv_2016_a/images/segmentation/CMU_est_labels_FV','-depsc2','-r300');



close all

%% Confusion Matrix

figure
confMat_2 = confMatGet(FV_real_labels_cmu, FV_est_labels_cmu);
opt_2=confMatPlot('defaultOpt');
opt_2.className={'close', 'crack', 'none', 'open', 'pour', 'put', 'read', 'spray', 'stir', 'switch-on', 'take','twist-off','twist-on','walk'};
opt_2.mode='percentage';
opt_2.format='8.1f';
confMatPlot(confMat_2,opt_2);
%title('Confusion Matrix for FV')

%To save
width  = 10;     % Width in inches
height = 10;    % Height in inches
set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
%NICTA
print('/home/johanna/latex-svn/assembla/papers-svn/wacv_2016_a/images/CF_CMU_FV','-depsc2','-r300');



figure
confMat = confMatGet(BoW_real_labels_cmu, BoW_est_labels_cmu);
opt=confMatPlot('defaultOpt');
opt.className={'close', 'crack', 'none', 'open', 'pour', 'put', 'read', 'spray', 'stir', 'switch-on', 'take','twist-off','twist-on','walk'};
opt.mode='percentage';
opt.format='8.1f';
confMatPlot(confMat, opt);
%title('Confusion Matrix for BoW')
 
%To save
width  = 10;     % Width in inches
height = 10;   % Height in inches
set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
%NICTA
print('/home/johanna/latex-svn/assembla/papers-svn/wacv_2016_a/images/CF_CMU_BoW','-depsc2','-r300');





