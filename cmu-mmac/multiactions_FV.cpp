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

#include "optflow_feat_def.hpp"
#include "optflow_feat_impl.hpp"


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




void
rand_split(int run)
{
  
  field<string> people;
  people.load(peopleList);
  int num_peo = people.n_rows;
  //arma_rng::set_seed_random();
  
  field<string> rand_people_train(num_peo - 1 );
  field<string> rand_people_test(1);
  rand_people_test(0) = people (run - 1);
  
  int k=0;
  
  for (int i=0; i<num_peo; ++i)
  {
    if (i!=run-1)
    {
      rand_people_train(k) = people(i);
      k++;
    }
    
  }
  std::stringstream train_list;
  train_list<< "./run" << run << "/train_list_run"<< run << ".dat";
  
  std::stringstream test_list;
  test_list<< "./run" << run << "/test_list_run"<< run << ".dat";
  
  //rand_people_train.print("train_list");
  //rand_people_test.print("test_list");
  
  rand_people_train.save( train_list.str() );
  rand_people_test.save( test_list.str() );
  
}




int
main(int argc, char** argv)
{
  
  
  field<string> people;
  people.load(peopleList);
  arma_rng::set_seed_random();
  
  
  
  if(argc < 2 )
  {
    cout << "usage: " << argv[0] << " Ng " << endl;
    return -1;
  }
  
  int N_cent = atoi(argv[1]);
  //int L = atoi(argv[2]);
  //int in_run = atoi(argv[3]);
  
  cout << "Multi Action Classification " << endl;
  cout << "Testing for GMM with " << N_cent << " centroids" << endl;
  cout << "Using random_subset" << endl;
  cout << "If Kmeans or EM fail new_seed and repeat" << endl;
  cout << "Multi_features: Each two " << endl;
  cout << "Jumping two frames" << endl;
  cout << "co&ro " << co << " & " << ro << endl;
  cout << "Dim " << 14 << endl;
  opt_feat cmu_optflow(path, actionNames, co, ro);
  
  //pre-calcular solo una vez
  //cout << "Obtaining Features of training videos" << endl;
  //cmu_optflow.features_per_action_training(people); //calculating feature vector for all videos
  //cout <<"*****************************************************"<< endl;
  //cout << "Obtaining Features of testing videos" << endl;
  //cmu_optflow.feature_multi_action( people ); //calculating feature vector for all MULTI-videos
  //***end
  
  //cout << "Not running for run=8" << endl;
  
  int n_people = people.n_rows;
  
  
  for (int in_run=1; in_run<=n_people; in_run++ ) //n_people
  {
    
    cout << "RUN "<< in_run << endl;
    rand_split(in_run);
    field<string> peo_train;
    field<string> peo_test;
    std::stringstream train_list;
    train_list<< "./run" << in_run << "/train_list_run"<< in_run << ".dat";
    std::stringstream test_list;
    test_list<< "./run" << in_run << "/test_list_run"<< in_run << ".dat";
    
    peo_train.load( train_list.str() ); //esto lo necesito para create GMM
    peo_test.load(  test_list.str() );
    
    
    cout << "Modeling Actions" << endl;
    cmu_optflow.create_universal_gmm( peo_train, N_cent, in_run );
    
  }
  
  
  
  
  
  
  
  
  return 0;
  
}


