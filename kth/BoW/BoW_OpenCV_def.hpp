class BoW
{
public:  
    inline BoW(const string  in_single_path,
			 const string  in_multi_path,
			 const string  in_actionNames, 
			 const uword in_col, 
			 const uword in_row,
			 const field <string> in_peo_train,
			 const field <string> in_peo_test,
			 const int in_run
			);
    
    

    inline void create_vocabulary(int N_cent, const string path_run_folders);
    inline void create_histograms(int N_cent, const string path_run_folders);
    inline void train_svm(int N_cent, const string path_run_folders);
    inline void create_histograms_testing(int N_cent, const string path_run_folders, int segm_length) ;


    
    
private:
  const string  single_path;
  const string  multi_path;
  const string  actionNames;
  const uword col;
  const uword row;
  const field <string> peo_train;
  const field <string> peo_test;
  const int run;
  
  int dim;
  int N_data;
  //int N_cent;
  mat mat_features;
  field<string> actions;
  
  mat means;
  mat covs;
  vec weights;	 
  

  
  
};