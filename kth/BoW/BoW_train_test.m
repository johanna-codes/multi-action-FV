clear all
close all
clc

%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');

%Home
addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab');
addpath('/media/johanna/HD1T/Toolbox/yael/matlab');

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'Dictionary Size?? ';
Ncent = input(prompt);
%Ncent = 4000;
prompt = 'What is segment length? ';
L = input(prompt);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
RUN = 3;
DIM =14;
path_run_folders = '/home/johanna/codes/codes-git/multi-action-FV/trunk/kth';


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Precalculate Histogram fortraining set
% display('Precaculating Histograms')
%It was done in OpenCV. Saved as run##/Histograms_BoW_OpenCV
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% TRAINING

%BoW_train(Ncent,RUN,DIM,path_run_folders);


%% TESTING.

BoW_test(Ncent, L, RUN,path_run_folders);
