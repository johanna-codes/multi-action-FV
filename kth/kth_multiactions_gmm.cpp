#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>

using namespace std;
using namespace arma;

#include "fisher_vector_def.hpp"
#include "fisher_vector_impl.hpp"



uword ro = 120;
uword co = 160;

//home
//const std::string single_path = "/media/sdb1/codes/multi-action/kth_single_action/"; 
//const std::string multi_path = "/media/sdb1/codes/multi-action/kth_multi_allVideos/stitched_dataset_run1/";


//NICTA
const string single_path = "/home/johanna/codes/datasets_codes/kth/single_action/"; 
const string multi_path =  "/home/johanna/codes/datasets_codes/kth/multi_action/"; 


const std::string  actionNames = "actionNames.txt";



int
main(int argc, char** argv)
{
    
  //Cambios en Agosto 6/2015. Necesito correr para Ng=128. Spirit y Philae DEAD!!!! Necesito recalcular las caracteristicas
  //To obtain features for all scenarios to Create Universal GMM
  
  
  int r = 1;
  
  field <string> rand_videos;
  stringstream run_pos;
  run_pos << "./run" << r << "/rand_selection_run"<< r << ".dat";
  rand_videos.load( run_pos.str() );
  
  //No importa el run. Son todos las personas 
  field <string>  people = rand_videos;
  field <string>  peo_train = rand_videos.rows (0,15);
  field <string>  peo_test  = rand_videos.rows (16,24);
   
    //peo_train.print();
   
   
   
//   for  (int sc=1; sc <=4; ++sc)
//   {
//   fisher_vector fisher_vector(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, r); //r no importa aca
//   fisher_vector.features_allpeople(people, sc); 
//   }
//   
  
  int RUN = 3;
  
  for (int run=1;run<=RUN; run++)
  {
    wall_clock timer;
    timer.tic();
      int N_cent = 256;
      

      field <string> rand_videos;
      stringstream run_pos;
      run_pos << "./run" << run << "/rand_selection_run"<< run << ".dat";
      rand_videos.load( run_pos.str() );
      //rand_videos.t().print("rand_pos");
      //getchar();
      
      
      field <string>  people = rand_videos;
      field <string>  peo_train = rand_videos.rows (0,15);
      field <string>  peo_test  = rand_videos.rows (16,24);
      
      cout << "Doing for run= " << run << endl;
      cout << "N_cent " << N_cent << endl;
      
      
      fisher_vector fisher_vector(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, run);
      
      fisher_vector.create_universal_gmm(N_cent); //Only One GMM for all scenarios
      //fisher_vector.features_testing();
      double n = timer.toc();
      cout << "number of seconds: " << n << endl;
  }
  
  
  //////////////////
  
  
  //    if(argc < 2 )
  //   {
  //     cout << "usage: " << argv[0] << " Ng " << endl;
  //     return -1;
  //   }
  
  
  //vec Ng;
  //Ng << 64 << 128 << 256 << 512<< endr;
  //Ng << 256 << endr;
  //vec Ng= [64 128 256 512];
  //int N_cent = atoi(argv[1]);
//   int RUN = 3;
//     
//   int run = 1;
//   for  (int sc=1; sc <=4; ++sc)
//   {
//   fisher_vector fisher_vector(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, run);
//   fisher_vector.features_allpeople(people, sc); 
//   //fisher_vector.features_training(sc);  //don't use it
//   //fisher_vector.create_universal_gmm(N_cent, sc);
//   //fisher_vector.create_vectors(N_cent, sc); // Obtaining Fisher Vectors. Don't work. Use Matlab instead and load data
//   //fisher_vector.train_svm( sc ); //single train all scenarios. Use Matlab
//   //fisher_vector.test_svm( sc );  //single test all scenarios Use Matlab
//   //getchar();
//   }

  
//   for (int run=1;run<=RUN; run++)
//   {
//     wall_clock timer;
//     timer.tic();
//     //for (int n=0; n < 4 ; n++)
//     //{
//       int N_cent = 128;
//       
//       //To genarate data split: Training and Testing
//       //uvec rand_videos = rand_split(run);
//       
//       //If already generate, load the file:
//       field <string> rand_videos;
//       stringstream run_pos;
//       run_pos << "./run" << run << "/rand_selection_run"<< run << ".dat";
//       rand_videos.load( run_pos.str() );
//       //rand_videos.t().print("rand_pos");
//       //getchar();
//       
//       
//       field <string>  people = rand_videos;
//       field <string>  peo_train = rand_videos.rows (0,15);
//       field <string>  peo_test  = rand_videos.rows (16,24);
//       
//       cout << "Doing for run= " << run << endl;
//       cout << "N_cent " << N_cent << endl;
//       
//       
//       fisher_vector fisher_vector(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, run);
//       //fisher_vector.train_svm(); //single train all scenarios. Use Matlab
//       //fisher_vector.test_svm();  //single test all scenarios. Use Matlab
//       
//       fisher_vector.create_universal_gmm(N_cent); //Only One GMM for all scenarios
//       double n = timer.toc();
//       cout << "number of seconds: " << n << endl;
//     //}
//   }
  
  ///I Only need to run this once
  //fisher_vector.features_testing();
  
  return 0;
  
}




