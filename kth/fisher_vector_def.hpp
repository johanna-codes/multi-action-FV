class fisher_vector
{
public:  
    inline fisher_vector(const string  in_single_path,
			 const string  in_multi_path,
			 const string  in_actionNames, 
			 const uword in_col, 
			 const uword in_row,
			 const field <string> in_peo_train,
			 const field <string> in_peo_test,
			 const int in_run
			);
    
    
    inline void features_training(int sc);
    inline void features_allpeople(field <string>  people, int sc);
    inline void features_testing();
    inline void create_universal_gmm(int N_cent);
    inline void create_universal_gmm(int N_cent, int sc);
    inline void create_vectors(int N_cent, int sc);
    
    inline void train_svm(int sc);
    inline void test_svm(int sc);
    inline void train_svm();
    inline void test_svm();
    
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
  int N_cent;
  mat mat_features;
  field<string> actions;
  
  mat means;
  mat covs;
  vec weights;	 
  

  
  inline vec get_fisher_vector( gmm_diag &gmm_uni_model);
  inline mat calc_prob_aposteriori( );
  inline rowvec calc_log_prob_X( );
  inline double calc_log_gauss_func( const vec &xn, const uword g );
  
  
};