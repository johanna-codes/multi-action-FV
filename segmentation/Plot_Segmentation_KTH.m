clear all
clc 
close all
orange = [1 0.5 0];
C = {'k','b','r','m',orange,'g'};

%% FV

%Los resultados de aca los tome del PC de NICTA, ya que Philae murio y yo
%solo copiaba los resultados, no todos los otros archivos como features,
%uni_gmm, FV, models, etc. Run1, Video1


load KTH_FV_test_info_Ng256_L25.mat

FV_real_labels_kth = test_info{1,2} ;
FV_real_labels_kth = FV_real_labels_kth + 1; %My labels start at 0


FV_est_labels_kth  = test_info{1,3} ;
FV_est_labels_kth = FV_est_labels_kth + 1;%My labels start at 0


%REAL LABELS
for i=1:length(FV_real_labels_kth)    
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{FV_real_labels_kth(i)}, 'EdgeColor',C{FV_real_labels_kth(i)},'LineWidth',3); 
   hold on
end

axis([-50, length(FV_real_labels_kth)+50, -0.1, 0.6])
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
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/KTH_real_labels','-depsc2','-r300');


figure
for i=1:length(FV_real_labels_kth)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{FV_est_labels_kth(i)}, 'EdgeColor',C{FV_est_labels_kth(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(FV_real_labels_kth)+50, -0.1, 0.6])
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
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/KTH_est_labels_FV','-depsc2','-r300');




%% BoW
clear all
orange = [1 0.5 0];
C = {'k','b','r','m',orange,'g'};

load KTH_BoW_test_info_Ng256_L25.mat


BoW_est_labels_kth  = test_info{1,3} ;
BoW_est_labels_kth = BoW_est_labels_kth + 1;%My labels start at 0

figure
for i=1:length(BoW_est_labels_kth)
   rectangle('Position',[i,0,1,0.5],'FaceColor',C{BoW_est_labels_kth(i)}, 'EdgeColor',C{BoW_est_labels_kth(i)},'LineWidth',3); 
   hold on
end
axis([-50, length(BoW_est_labels_kth)+50, -0.1, 0.6])
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
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/segmentation/KTH_est_labels_BoW','-depsc2','-r300');

close all