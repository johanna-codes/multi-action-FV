clear all
clc 
close all
real_labels = randi(7,1,100);
C = {'k','b','r','g','y',[.5 .6 .7],[.8 .2 .6]};

for i=1:length(real_labels)
    
    
   rectangle('Position',[i,0,1,2],'FaceColor',C{real_labels(i)}, 'EdgeColor',C{real_labels(i)},'LineWidth',3); 
   rectangle('Position',[i,2.5,1,2],'FaceColor',C{real_labels(i)}, 'EdgeColor',C{real_labels(i)},'LineWidth',3); 

    hold on
end

axis([0 100 0 4.5])