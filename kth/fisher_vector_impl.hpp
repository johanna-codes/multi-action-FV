#include "optflow_feat_def.hpp"
#include "optflow_feat_impl.hpp"
#include "svm_def.hpp"
#include "svm_impl.hpp"

inline 
fisher_vector::fisher_vector(const string  in_Spath,
			 const string  in_Mpath,
			 const string  in_actionNames, 
			 const uword in_col, 
			 const uword in_row,
			 const field <string> in_peo_train,
			 const field <string> in_peo_test,
			 const int in_run)
:single_path(in_Spath), multi_path(in_Mpath), actionNames(in_actionNames), col(in_col), row(in_row), peo_train(in_peo_train), peo_test(in_peo_test), run(in_run)
{
  actions.load( actionNames );  //actions.print("All actions");
  dim =14;
}

inline
void
fisher_vector::features_allpeople(field <string>  people, int sc)
{
  opt_feat kth_optflow(actionNames, col, row, run, dim);
  kth_optflow.features_allpeople( people, single_path, sc ); 
}


inline
void
fisher_vector::features_training(int sc)
{
  opt_feat kth_optflow(actionNames, col, row, run, dim);
  kth_optflow.features_training( peo_train, single_path, sc ); 
}

inline
void
fisher_vector::features_testing()
{
  opt_feat kth_optflow(actionNames, col, row, run, dim);
  kth_optflow.feature_multi_action( peo_test, multi_path ); 
}


inline
void
fisher_vector::train_svm(int sc)
{
  my_svm my_svm(actionNames, peo_train, peo_test, run);
  my_svm.train(sc);
  
}

inline
void
fisher_vector::test_svm(int sc)
{
  my_svm my_svm(actionNames, peo_train, peo_test, run);
  my_svm.test(sc);
  
}

///Training and Testing all together

inline
void
fisher_vector::train_svm()
{
  my_svm my_svm(actionNames, peo_train, peo_test, run);
  my_svm.train();
  
}

inline
void
fisher_vector::test_svm()
{
  my_svm my_svm(actionNames, peo_train, peo_test, run);
  my_svm.test();
  
}

///Create ONE universal GMM Model for all scenarios


inline
void
fisher_vector::create_universal_gmm(int N_cent)
{ 
  
  cout << "Calculating Universal GMM " << endl;
  cout << "# clusters: " << N_cent << endl;
  
  mat uni_features;
  
   for (uword pe=0; pe<peo_train.n_rows; ++pe)    {
     
     mat mat_features_tmp;
     mat mat_features;
     
     for (uword act = 0 ; act < actions.n_rows;  ++act) {
       
       for (uword sc = 1 ; sc <= 4;  ++sc) {
	 
       mat mat_features_video_i;
       std::stringstream ssName_feat_video;
       //ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
       ssName_feat_video << "./features_all_nor/feat_vec_" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
       mat_features_video_i.load( ssName_feat_video.str() );
       
       if ( mat_features_video_i.n_cols>0 )
       {
	 mat_features_tmp	 = join_rows( mat_features_tmp, mat_features_video_i );
	 
      }
      else 
      {
	cout << "# vectors = 0 in " << ssName_feat_video.str() << endl;
	
      }
	 
      }
       
    }
    
    cout << "mat_features_tmp.n_cols "<< mat_features_tmp.n_cols << endl;   
    const uword N_max = 100000*4;  // 4 sc. maximum number of vectors per action to create universal GMM
    //const uword N_max = 100000; //???
    if (mat_features_tmp.n_cols > N_max)
    {
      ivec tmp1 = randi( N_max, distr_param(0,mat_features_tmp.n_cols-1) );
      ivec tmp2 = unique(tmp1);
      uvec my_indices = conv_to<uvec>::from(tmp2);
      mat_features = mat_features_tmp.cols(my_indices);  // extract a subset of the columns
      
    }
    else
    {
      mat_features = mat_features_tmp;
      
    }
    
    cout << "mat_features.n_cols "<< mat_features.n_cols << endl;
    
    
      if ( mat_features.n_cols>0 )
      {
	uni_features	 = join_rows( uni_features, mat_features );
	
      }
      else 
      {
	cout << "# vectors = 0 in uni_features" << endl;
	
      }
    
    //uni_features =  join_rows( uni_features, mat_features );
    mat_features_tmp.reset();
    mat_features.reset();
    
    
  }
  
  
  cout << "r&c "<<  uni_features.n_rows << " & " << uni_features.n_cols << endl;
  bool is_finite = uni_features.is_finite();
 
    if (!is_finite )
    {
      cout << "is_finite?? " << is_finite << endl;
      cout << uni_features.n_rows << " " << uni_features.n_cols << endl;
      getchar();
    
    }
  
  
  cout << "universal GMM" << endl;
  gmm_diag gmm_model;
  gmm_diag bg_model;
  
  
  
  bool status_em = false;
  int rep_em=0;
  
  
  int km_iter = 10;
  int em_iter  = 5;
  double var_floor = 1e-10;
  bool print_mode = true;
  
  
  
  
  while (!status_em)
  {
    bool status_kmeans = false;
    //int rep_km = 0;
    
    while (!status_kmeans)
    {
      arma_rng::set_seed_random();
      
      status_kmeans = gmm_model.learn(uni_features, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(uni_features, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./run"<< run <<"/universal_GMM/UniversalGMM_Ng" << N_cent << "_dim" <<dim << "_all_sc"; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
  
  means = gmm_model.means;
  covs  = gmm_model.dcovs;
  weights = gmm_model.hefts.t();	
  
  //Saving statistics
  std::stringstream ss_weigths;
  ss_weigths << "./run"<< run <<"/universal_GMM/weights_Ng" << N_cent << "_dim" << dim << "_all_sc" << ".dat"; 
  
  std::stringstream ss_means;
  ss_means << "./run"<< run <<"/universal_GMM/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".dat"; 
  
  std::stringstream ss_covs;
  ss_covs << "./run"<< run <<"/universal_GMM/covs_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".dat"; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii);

}



///Create universal GMM Model per scenario
inline
void
fisher_vector::create_universal_gmm(int N_cent, int sc)
{ 
  
  cout << "Calculating Universal GMM " << endl;
  cout << "# clusters: " << N_cent << endl;
  
  mat uni_features;
  
   for (uword pe=0; pe<peo_train.n_rows; ++pe)    {
     
     mat mat_features_tmp;
     mat mat_features;
     
     for (uword act = 0 ; act < actions.n_rows;  ++act) {
       
       mat mat_features_video_i;
      
      
      std::stringstream ssName_feat_video;
      //ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
      ssName_feat_video << "./features_all_nor/feat_vec_" << peo_train(pe) << "_" << actions(act) << "_d" << sc;

      
      mat_features_video_i.load( ssName_feat_video.str() );
      
      if ( mat_features_video_i.n_cols>0 )
      {
	mat_features_tmp	 = join_rows( mat_features_tmp, mat_features_video_i );
	
      }
      else 
      {
	cout << "# vectors = 0 in " << ssName_feat_video.str() << endl;
	
      }
       
    }
    
    const uword N_max = 100000;  // maximum number of vectors per action to create universal GMM
    
    if (mat_features_tmp.n_cols > N_max)
    {
      ivec tmp1 = randi( N_max, distr_param(0,mat_features_tmp.n_cols-1) );
      ivec tmp2 = unique(tmp1);
      uvec my_indices = conv_to<uvec>::from(tmp2);
      mat_features = mat_features_tmp.cols(my_indices);  // extract a subset of the columns
      
    }
    else
    {
      mat_features = mat_features_tmp;
      
    }
    
    
      if ( mat_features.n_cols>0 )
      {
	uni_features	 = join_rows( uni_features, mat_features );
	
      }
      else 
      {
	cout << "# vectors = 0 in uni_features" << endl;
	
      }
    
    //uni_features =  join_rows( uni_features, mat_features );
    mat_features_tmp.reset();
    mat_features.reset();
    
    
  }
  
  
  cout << "r&c "<<  uni_features.n_rows << " & " << uni_features.n_cols << endl;
  bool is_finite = uni_features.is_finite();
 
    if (!is_finite )
    {
      cout << "is_finite?? " << is_finite << endl;
      cout << uni_features.n_rows << " " << uni_features.n_cols << endl;
      getchar();
    
    }
  
  
  cout << "universal GMM" << endl;
  gmm_diag gmm_model;
  gmm_diag bg_model;
  
  
  
  bool status_em = false;
  int rep_em=0;
  
  
  int km_iter = 10;
  int em_iter  = 10;
  double var_floor = 1e-10;
  bool print_mode = true;
  
  
  
  
  while (!status_em)
  {
    bool status_kmeans = false;
    //int rep_km = 0;
    
    while (!status_kmeans)
    {
      arma_rng::set_seed_random();
      
      status_kmeans = gmm_model.learn(uni_features, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(uni_features, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./run"<< run <<"/gmm_models/UniversalGMM_Ng" << N_cent << "_dim" <<dim << "_sc"<< sc; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
  
  means = gmm_model.means;
  covs  = gmm_model.dcovs;
  weights = gmm_model.hefts.t();	
  
  //Saving statistics
  std::stringstream ss_weigths;
  ss_weigths << "./run"<< run <<"/gmm_models/weights_Ng" << N_cent << "_dim" << dim << "_sc"<< sc << ".dat"; 
  
  std::stringstream ss_means;
  ss_means << "./run"<< run <<"/gmm_models/means_Ng" << N_cent << "_dim" <<dim << "_sc"<< sc << ".dat"; 
  
  std::stringstream ss_covs;
  ss_covs << "./run"<< run <<"/gmm_models/covs_Ng" << N_cent << "_dim" <<dim << "_sc"<< sc << ".dat"; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii);


     
     
     
  
  
}



///Obtain Fisher vectors:
inline
void 
fisher_vector::create_vectors(int in_N_cent, int sc)
{
  N_cent = in_N_cent;
  std::stringstream ss_uniGMM;
  ss_uniGMM << "./run"<< run <<"/gmm_models/UniversalGMM_Ng" << N_cent << "_dim" <<dim << "_sc"<< sc; 
  cout << "Loading GMM in " << ss_uniGMM.str() << endl;
  
  gmm_diag gmm_uni_model;
  gmm_uni_model.load( ss_uniGMM.str() );
  
  
  
  for (uword pe = 0; pe< peo_train.n_rows; ++pe)
  {
    for (uword act = 0; act< actions.n_elem; ++act)
    {
      
      std::stringstream ssName_feat_video;
      ssName_feat_video << "./features_all_nor/feat_vec_" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
      cout << "Calculating FV for  " << ssName_feat_video.str() << endl;
      mat_features.load( ssName_feat_video.str(), raw_ascii );
    
      //aca crear la funcion que le entra mat_features y gmm_uni_model y devuelve un vector.
     
      get_fisher_vector(gmm_uni_model);
      
     }

  }

}



inline
vec
fisher_vector::get_fisher_vector(gmm_diag &gmm_uni_model)
{
  
  
   cout << "Applying PCA to the features" << endl;
   cout << "mat_features size " << mat_features.n_rows << " " << mat_features.n_cols << endl;
   mat Sigma_pca = cov(mat_features.t());
   mat U;   vec s;   mat V;
   svd( U, s, V, Sigma_pca ) ;
   //As dim is small (dim=14). I'll keep all
   // components for now. U
   mat_features = mat_features.t()*U;
   mat_features = mat_features.t();
   cout << "After PCA " << endl;
   cout << "mat_features size " << mat_features.n_rows << " " << mat_features.n_cols << endl;


  int T = mat_features.n_cols;
  N_data = mat_features.n_cols;
   
  cout << "N_cent " <<  N_cent << endl;
  cout << "dim "    <<  dim << endl;
  
 

  means = gmm_uni_model.means;
  covs  = gmm_uni_model.dcovs;
  weights = gmm_uni_model.hefts.t();	 
  
  mat prob_aposteriori = calc_prob_aposteriori(); //(Ncent x N_featureVectors)
  //cout << "prob_aposteriori size " << prob_aposteriori.n_rows << " " << prob_aposteriori.n_cols << endl;


     
  {
    //   cout << "weights size : " << weights.n_rows << "x"<< weights.n_cols << endl;
    //   cout << "means size : " << means.n_rows << "x"<< means.n_cols << endl;
    //   cout << "covs size : " << covs.n_rows << "x"<< covs.n_cols << endl;
    //   cout << "saving GMM" << endl;
    //   means.save("mu.dat", raw_ascii);
    //   covs.save("sigma.dat", raw_ascii);
    //   weights.save("w.dat", raw_ascii);
    //   mat_features.save("S.dat", raw_ascii);
  }

  ///Short way
  {
//   vec s0;// =  zeros<vec>(N_cent);
//   mat s1; //=  zeros<mat>(dim,N_cent);
//   mat s2; //=  zeros<mat>(dim,N_cent);
//   
//   
   //cout << "prob_aposteriori"  << endl;

//   
//   //cout << "Saving prob_aposteriori " << endl;
//   //prob_aposteriori.save("my_prob_aposteriori.dat", raw_ascii);
//   //cout << "??? " << endl;
//   
//   
//   s0 = sum(prob_aposteriori,1);
//   s1 = prob_aposteriori*mat_features.t();
//   s2 = prob_aposteriori*square(mat_features.t());
//    
//    //s1.save("s1_tmp.dat",raw_ascii);
//    //s2.save("s2_tmp.dat",raw_ascii);
//    //cout << "s1_tmp " << s1_tmp.n_rows << " & " << s1_tmp.n_cols << endl;
//    //cout << "s2_tmp " << s2_tmp.n_rows << " & " << s2_tmp.n_cols << endl;
  }
  
///Long  way:

  vec s0 =  zeros<vec>(N_cent);
  mat s1 =  zeros<mat>(dim,N_cent);
  mat s2 =  zeros<mat>(dim,N_cent); 
  
  for (int t=0; t<T; ++t)
  {
    
    vec gamma_t = prob_aposteriori.col(t);
    vec x_t = mat_features.col(t);
    
    for (int k=0; k<N_cent; ++k)
    {
//        cout << s0.t() << endl;
//        getchar();
//        cout << s1.t() << endl;
//        getchar();
//        cout << s2.t() << endl;
//        getchar();    
//        cout << "t: " << t << endl;
//        cout << "gamma_t " << gamma_t.t() << endl;
//        cout << "x_t " << x_t.t() << endl;
       
      
      s0(k) = s0(k) + gamma_t(k);
      s1.col(k)= s1.col(k) + gamma_t(k)*x_t;
      s2.col(k)= s2.col(k) + gamma_t(k)*square(x_t);
      
    }
    
  }
  

  cout << "Computing the Fisher Vector Signatures" << endl;
  vec sig_weights = zeros<vec>(N_cent); //Eq. (16) or (31) in Algorithm
  mat sig_means   = zeros<mat>(dim, N_cent);   //Eq. (17) or (32) in Algorithm
  mat sig_covs    = zeros<mat>(dim, N_cent);    //Eq. (18) or (33) in Algorithm


  for (int k=0; k<N_cent; ++k)
  {
    sig_weights(k)   =  ( s0(k) - T*weights(k) ) / sqrt( weights(k) );
    sig_means.col(k) =  ( s1(k) - means.col(k)*s0(k) ) / ( sqrt( weights(k) )*covs.col(k) );
    sig_covs.col(k)  =  ( s2(k) - 2*means.col(k)*s1(k) + ( square( means.col(k) ) - square( covs.col(k) ) )*s0(k) ) / ( sqrt( 2*weights(k) )*square( covs.col(k) ) );
    
  }
  
  
  ///Yo creo que de aca para adelante es donde esta mal. Tal vez no, pq no funciona :(
  {
  //mat tmp_means_s0( dim,N_cent);
  //mat tmp_means_s1( dim,N_cent);
  //mat tmp_subs( dim,N_cent);
  //mat tmp_covs_w( dim,N_cent);
  //mat tmp_covs_w2( dim,N_cent);
//   for (int k=0; i<N_cent; ++i)
//   {
//     tmp_means_s0.col(k) = means.col(k)*s0(k);
//     tmp_covs_w.col(k)   = sqrt( weights(k) )*covs.col(k);
//     
//     tmp_means_s1.col(k) = means.col(k)*s1(k);
//     tmp_subs.col(k)     = ( square( means.col(k) ) - square( covs.col(k) ) )*s0(k); 
// 
//     tmp_covs_w2.col(k)  = sqrt( 2*weights(k) )*square( covs.col(k) );
//     
//   }
// 
//   cout << "weights" << endl;
//   sig_weights = ( s0 - T*weights )/sqrt(weights);
//   
//   
//   cout << "means" << endl;
//   sig_means = ( s1-tmp_means_s0.t() )/ tmp_covs_w.t() ;
//   
//   
//   cout << "covs" << endl;
//   sig_covs = ( s2-2*tmp_means_s1.t() + tmp_subs.t() )/tmp_covs_w2.t();
  } 
 

  vec vec_sig_means = vectorise( sig_means );
  vec vec_sig_covs =  vectorise( sig_covs );
  
//  cout << "sig_means size " << sig_means.n_rows << " " << sig_means.n_cols << endl;
//   cout << "sig_covs size " << sig_covs.n_rows   << " " << sig_covs.n_cols << endl;
//   
//   cout << "vec_sig_means size " << vec_sig_means.n_elem << endl; 
//   cout << "vec_sig_covs size " << vec_sig_covs.n_elem << endl;
  
  
  vec fish_vec =  join_vert(vec_sig_means, vec_sig_covs );
  cout << "fish_vec size  " << fish_vec.n_elem << endl;
  fish_vec.save( "myFV_feat_nor.dat", raw_ascii);
 
  //fish_vec = join_vert ( fish_vec, sig_weights);
  
  //Power Normalisation
  fish_vec = sign(fish_vec)%sqrt(abs(fish_vec)); 
  
  //L2 normalisation
  double den = as_scalar(fish_vec.t()*fish_vec);
  fish_vec = fish_vec/den;
  fish_vec.save( "nor_myFV_feat_nor.dat", raw_ascii);
  
  
  cout << "Saving and "  << endl;
  //sig_means.save("mat_sig_means.dat", raw_ascii);
  //sig_covs.save("mat_sig_covs.dat", raw_ascii);
  //sig_weights.save("vec_sig_weights.dat", raw_ascii);
  //vec_sig_means.save("vec_sig_means.dat", raw_ascii);
  //vec_sig_covs.save("vec_sig_covs.dat", raw_ascii);
  
  cout << "end :) " << endl;
  getchar();
  
}

inline
mat
fisher_vector::calc_prob_aposteriori( ) //log(2.26)
{
  
  mat prob_aposteriori = zeros(N_cent,N_data);
  mat log_prob_aposteriori = zeros(N_cent,N_data);
  double log_gauss_function;
  double log1;
  double log2;   // aposteriori_prob = log1-log2
  vec xn;
  rowvec log_prob_X = calc_log_prob_X( );
  //cout << "log_prob_X: " << log_prob_X.t() << endl;
  
  for (uword gi=0; gi<N_cent; gi++)
  {
    for (uword  ni=0; ni<N_data ; ni++)
    {      
      xn=mat_features.col(ni);
      log_gauss_function = calc_log_gauss_func(xn,gi);
      log1= log(weights(gi)) + log_gauss_function;
      log2=log_prob_X(ni);
      //cout << "log1: " << log1 << " log2: " << log2 << ". log1-log2: " << log1-log2 << endl;
      log_prob_aposteriori(gi,ni)=log1-log2;
    }
  }
  //log_prob_aposteriori.print("log_prob_aposteriori: ");
  //log_prob_aposteriori.save("log_prob_aposteriori.dat",raw_ascii);
  prob_aposteriori =exp(log_prob_aposteriori);
  
  return prob_aposteriori;
}


inline
rowvec 
fisher_vector::calc_log_prob_X()
{
  
  //cout << "calc_log_prob_X"  << endl;
  double log_gauss_function;
  double log_prob_xn;
  double tmp;
  rowvec log_prob_X;
  mat cov;
  vec mu;
  vec x;
  
  log_prob_X.zeros(N_data);
  
  for (uword ni = 0; ni<N_data ; ++ni)
  {
    // Calculating the probability
    //log_prob_xn = 0; ERROR - DON'T USE
    x=mat_features.col(ni);
    
    for (uword gi=0; gi<N_cent; ++gi)
    {
      mu=means.col(gi);
      log_gauss_function=calc_log_gauss_func(x,gi);
      //cout << "log_gauss_function: " << log_gauss_function << ". ";
      if (gi == 0)
      {
	log_prob_xn =log(weights(gi))+log_gauss_function;
      }
      else
      {
	tmp = log(weights(gi))+log_gauss_function;
	log_prob_xn = log_add(log_prob_xn,tmp);
	//cout << "log_prob_xn: " << log_prob_xn << endl;
      }
    }
    log_prob_X(ni)=log_prob_xn;
  }
  
    //cout << "end calc_log_prob_X"  << endl;

  return log_prob_X;
}

inline
double 
fisher_vector::calc_log_gauss_func(const vec &xn, const uword g) //log(2.23)
{
    //cout << "calc_log_gauss_func"  << endl;

  double logDetCov;
  double logDen;
  double logNum;
  double logGauss;
  //mat cov=zeros(Dim,Dim);
  vec res;
  vec mu;
  
  //cov.diag() = covs.col(g); 
  mu=means.col(g);
  //mat Xd = diagmat(covs.col(g));
  double logDetCov2;
  double sign;
  
  log_det(logDetCov2, sign, diagmat(covs.col(g)));
  

  
  logDen=(-0.5)*(dim*log(2*datum::pi) + logDetCov2);
  res = (xn - mu);
  logNum=as_scalar((-0.5)*res.t()*inv( diagmat(covs.col(g)) )*res);

  logGauss = logNum + logDen;
   //cout << " end calc_log_gauss_func"  << endl;
  return logGauss;
}
