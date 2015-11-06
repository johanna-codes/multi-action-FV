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
% prompt = 'Number of Gaussians? ';
% Ncent = input(prompt);
% prompt = 'What is segment length? ';
% L = input(prompt);

Ncent =64;
L = 15;
RUN=12;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%RUN = 12;
Ng = int2str(Ncent);
dim = int2str(14);
path_run_folders = '/home/johanna/codes/codes-git/multi-action-FV/trunk/cmu-mmac';

%% TRAINING
BoW_train_cmu(RUN, Ng, path_run_folders);

%% Testing
all_run_acc = BoW_test_cmu(Ng,RUN,L,path_run_folders);
all_run_acc'





