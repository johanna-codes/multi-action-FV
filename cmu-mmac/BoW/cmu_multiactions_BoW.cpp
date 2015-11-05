#include <omp.h>
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


///cmu-mmac
//uword ro = 512;
//uword co = 384;

//uword ro = 240;
//uword co = 320;

uword ro = 96;
uword co = 128;


//for gmm_cmu
//uword ro = 76;
//uword co = 102;

// uword ro = 64;
// uword co = 48;



//home path for dataset
//const std::string path = "/media/sdb1/codes/datasets_codes/CMU-MMAC/Brownie/"; 
///OJO
//const std::string  peopleList = "people_list2.txt";

///cambiar
//NICTA path for dataset
const std::string path = "/home/johanna/codes/datasets_codes/CMU-MMAC/Brownie/"; 
const std::string peopleList = "people_list.txt";

const std::string  actionNames = "actionNames.txt";

const string path_run_folders = "/home/johanna/codes/codes-git/multi-action-FV/trunk/cmu-mmac";


inline void create_training_vocabulary_hist(int N_cent);
inline void create_testing_hist(int N_cent, int segm_length);

int
main(int argc, char** argv)
{
  
    
  if(argc < 3 )
  {
    cout << "usage: " << argv[0] << " Ng Segm_length" << endl;
    return -1;    
  }
  
  
    int N_cent = atoi(argv[1]);
    int segm_length = atoi(argv[2]);

    create_training_vocabulary_hist( N_cent);
    //create_testing_hist( N_cent, segm_length);
    
    return 0;
  
}

inline void 
create_training_vocabulary_hist(int N_cent)
{
  
  field<string> people;
  people.load(peopleList);
  int n_people = people.n_rows;
  
  //Creating Visual Vocabulary
  for (int in_run=1; in_run<=n_people; in_run++ ) //n_people
  {
    wall_clock timer;
    timer.tic();
    cout << "RUN "<< in_run << endl;
    cout << "N_cent " << N_cent << endl;
    
    field<string> peo_train;
    field<string> peo_test;
    std::stringstream train_list;
    train_list<< path_run_folders << "/run" << in_run << "/train_list_run"<< in_run << ".dat";
    std::stringstream test_list;
    test_list<< path_run_folders <<  "/run" << in_run << "/test_list_run"<< in_run << ".dat";
    
    peo_train.load( train_list.str() ); //esto lo necesito para create GMM
    peo_test.load(  test_list.str() );
    
        
    BoW BofWords(path, actionNames, co, ro);
    //BofWords.vocabulary( peo_train, N_cent, in_run, path_run_folders );
    cout << "Histograms for Training sets" << endl;
    BofWords.create_hist_train( peo_train, N_cent,in_run, path_run_folders);
    
    double n = timer.toc();
    cout << "number of seconds: " << n << endl;
    
  }
}

inline
void
create_testing_hist(int N_cent, int segm_length)
{
  
  field<string> people;
  people.load(peopleList);
  int n_people = people.n_rows;
  
  
  for (int run=1;run<=n_people; run++)
  {
    wall_clock timer;
    timer.tic();
    
    field<string> peo_train;
    field<string> peo_test;
    std::stringstream train_list;
    train_list<< path_run_folders << "/run" << run << "/train_list_run"<< in_run << ".dat";
    std::stringstream test_list;
    test_list<< path_run_folders <<  "/run" << run << "/test_list_run"<< in_run << ".dat";

    
    cout << "Doing for run= " << run << endl;
    cout << "N_cent " << N_cent << endl;
    cout << "segm_length " << segm_length << endl;
    
    
    BoW BofWords(path, actionNames, co, ro);
    
    cout << "Multi_features" << endl;
    BofWords.create_histograms_testing(peo_test, N_cent, run, path_run_folders, segm_length);
    double n = timer.toc();
    cout << "number of seconds: " << n << endl;
    //cout << "Press a Key" << endl;
    //getchar();
    
  }
}



