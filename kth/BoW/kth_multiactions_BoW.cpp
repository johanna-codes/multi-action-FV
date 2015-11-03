#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>

using namespace std;
using namespace arma;

#include "BoW_OpenCV_def.hpp"
#include "BoW_OpenCV_impl.hpp"



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


inline void create_training_vocabulary_hist(int N_cent, int RUN);
inline void create_testing_hist(int N_cent, int RUN, int segm_length);

int
main(int argc, char** argv)
{
 
  int RUN = 3;
  
  
/// Getting the Histograms for the Training Set  
//   vec vNcent; 
//   vNcent << 64 << 128 << 256 << endr ;
//   
//   
//   //Creating Vocaularies  and Histograms for Training
// 
//   for (int ng=0;ng<vNcent.n_elem; ng++)
//   {
//     int N_cent = vNcent(ng);
//     create_training_vocabulary_hist( N_cent, RUN);
//     
//   }
  
  
 /// Getting the Histograms for the Testing Set  
 
  
        if(argc < 3 )
	{
	  cout << "usage: " << argv[0] << " Ng Segm_length" << endl;
	  return -1;
	  
	  
	}
	
	int N_cent = atoi(argv[1]);
	int segm_length = atoi(argv[2]);
	
	
	create_testing_hist(N_cent,  RUN, segm_length)
       
   
   
   
   
  
  
  
  
  return 0;
  
}

inline void 
create_training_vocabulary_hist(int N_cent, int RUN)
{
  
  for (int run=1;run<=RUN; run++)
  {
    wall_clock timer;
    timer.tic();
    
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
    BofWords.create_vocabulary(N_cent, path_run_folders); 
    BofWords.create_histograms(N_cent, path_run_folders) ;
    double n = timer.toc();
    cout << "number of seconds: " << n << endl;
    //cout << "Press a Key" << endl;
    //getchar();
  }
  
}

inline
void
create_testing_hist(int N_cent, int RUN, int segm_length)
{
  
  for (int run=1;run<=RUN; run++)
  {
    wall_clock timer;
    timer.tic();
    
    field <string> rand_videos;
    stringstream run_pos;
    run_pos << path_run_folders << "/run" << run << "/rand_selection_run"<< run << ".dat";
    rand_videos.load( run_pos.str() );

    field <string>  people = rand_videos;
    field <string>  peo_train = rand_videos.rows (0,15);
    field <string>  peo_test  = rand_videos.rows (16,24);
    
    cout << "Doing for run= " << run << endl;
    cout << "N_cent " << N_cent << endl;
    cout << "segm_length " << segm_length << endl;
    
    
    BoW BofWords(single_path, multi_path, actionNames, co, ro, peo_train, peo_test, run);
    
    cout << "Multi_features" << endl;
    BofWords.create_histograms_testing(N_cent, path_run_folders, segm_length);
    double n = timer.toc();
    cout << "number of seconds: " << n << endl;
    //cout << "Press a Key" << endl;
    //getchar();
    
  }
}





