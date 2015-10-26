
inline 
BoW::BoW(const string  in_Spath,
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
BoW::create_universal_gmm(int N_cent)
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
       ssName_feat_video << path_run_folders <<"/features_all_nor/feat_vec_" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
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
  double var_floor = 1e-10;
  bool print_mode = true;
  
  
  
  
 bool status_kmeans = false;

 int rep_km = 0;
 
 while (!status_kmeans)
 {
   arma_rng::set_seed_random();
   status_kmeans = gmm_model.learn(uni_features, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
   bg_model = gmm_model;
   
   rep_km++;
    }
    
 
  cout <<"K-means was repeated " << rep_km << endl;
   
  means = gmm_model.means;
 
  
  //Saving statistics

  std::stringstream ss_means;
  ss_means << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".dat"; 
  means.save( ss_means.str(), raw_ascii );

}





