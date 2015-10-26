#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>

using namespace std;
using namespace arma;

#include "BoW_def.hpp"
#include "BoW_impl.hpp"



uword ro = 120;
uword co = 160;

//home
//const std::string single_path = "/media/sdb1/codes/multi-action/kth_single_action/"; 
//const std::string multi_path = "/media/sdb1/codes/multi-action/kth_multi_allVideos/stitched_dataset_run1/";


//NICTA
const string single_path = "/home/johanna/codes/datasets_codes/kth/single_action/"; 
const string multi_path =  "/home/johanna/codes/datasets_codes/kth/multi_action/"; 

const string path_run_folders = "/home/johanna/codes/codes-git/multi-action-FV/trunk/kth";

const std::string  actionNames = "actionNames.txt";



int
main(int argc, char** argv)
{
    
  //Cambios en Agosto 6/2015. Necesito correr para Ng=128. Spirit y Philae DEAD!!!! Necesito recalcular las caracteristicas
  //To obtain features for all scenarios to Create Universal GMM
  
  
  int r = 1;
  
  field <string> rand_videos;
  stringstream run_pos;
  run_pos << path_run_folders << "/run" << r << "/rand_selection_run"<< r << ".dat";
  rand_videos.load( run_pos.str() );
  
  //No importa el run. Son todos las personas 
  field <string>  people = rand_videos;
  field <string>  peo_train = rand_videos.rows (0,15);
  field <string>  peo_test  = rand_videos.rows (16,24);
   

  
  int RUN = 3;
  
  for (int run=1;run<=RUN; run++)
  {
    wall_clock timer;
    timer.tic();
      int N_cent = 256;
      

      field <string> rand_videos;
      stringstream run_pos;
      run_pos << path_run_folders << "/run" << run << "/rand_selection_run"<< run << ".dat";
      rand_videos.load( run_pos.str() );
      //rand_videos.t().print("rand_pos");
      //getchar();
      
      
      field <string>  people = rand_videos;
      field <string>  peo_train = rand_videos.rows (0,15);
      field <string>  peo_test  = rand_videos.rows (16,24);
      
      cout << "Doing for run= " << run << endl;
      cout << "N_cent " << N_cent << endl;
      
      
      BoW BofWords(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, run);
      
      BofWords.create_universal_gmm(N_cent, path_run_folders); //Only One GMM for all scenarios
      //fisher_vector.features_testing();
      double n = timer.toc();
      cout << "number of seconds: " << n << endl;
  }
  
  
  
  
  return 0;
  
}




