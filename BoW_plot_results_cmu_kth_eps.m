%Overlapping Segments and each two frames.
close all
clear all
clc

%% KTH
close all
clear all
clc

kth_overseg_1 = [48.3	48.3	44.4	42.6]; % 256
kth_overseg_2 = [46.4	47.1	43.6	40.8]; % 128. %Added on August 10/2015
kth_overseg_3 = [42.7	43.9	41.1	38.2]; % 64
t = [ 0.5 1 1.5 2];
%subplot(1,2,1)
figure(1)
plot(t, kth_overseg_1, '-rx','LineWidth',3,'MarkerSize',10)
hold on
plot(t, kth_overseg_2, '-.bd','LineWidth',3,'MarkerSize',10)
plot(t, kth_overseg_3, ':ko','LineWidth',3,'MarkerSize',10)
grid
axis([0.4,2.1,36,50])
%title('s-KTH dataset','FontSize',24,'FontWeight','bold');

set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
h_legend = legend('K = 256','K = 128','K = 64', 'Location','southwest');
set(h_legend,'FontSize',16);
%ylabel('Performance (%)','FontSize',24, 'FontWeight','bold') % y-axis label
%xlabel({'Segment Length (seconds)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label

%To save
width = 4;     % Width in inches
height = 3;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

 print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/BoW_kth_K256_128_64_Nov','-depsc2','-r300');


%% CMU
cmu_overseg_1 = [19.0	19.7	20.1	20.0	20.1	20.1];  %256
cmu_overseg_2 = [15.3	15.1	15.3	15.0	15.0	15.1];  %128
cmu_overseg_3 = [12.6	12.6	12.6	12.8	13.1	13.1];  %64


%t = [0.5 1 1.5 2];
t = [0.5 1 1.5 2 2.5 3];

figure(2)
plot(t, cmu_overseg_1, '-rx','LineWidth',3,'MarkerSize',10)
hold on
plot(t, cmu_overseg_2, '-.bd','LineWidth',3,'MarkerSize',10)
plot(t, cmu_overseg_3, ':ko','LineWidth',3,'MarkerSize',10)
grid
axis([0.4,3.1,2,25])
%title('CMU-MMAC dataset','FontSize',24,'FontWeight','bold');
set(gca,'XTick',t); % Change x-axis ticks
set(gca,'XTickLabel',t, 'FontSize',20); % Change x-axis ticks labels to desired values.
h_legend = legend('K = 256','K = 128','K = 64', 'Location','southwest');
set(h_legend,'FontSize',16);%ylabel('Performance (%)','FontSize',24,'FontWeight','bold') % y-axis label
%xlabel({'Segment Length (seconds)'; ' '},'FontSize',24,'FontWeight','bold') % x-axis label

%To save
width = 4;     % Width in inches
height = 3;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
print('/media/johanna/HD1T/latex-svn/assembla/wacv_2016_a/images/BoW_cmu_K256_128_64_Nov','-depsc2','-r300');

close all