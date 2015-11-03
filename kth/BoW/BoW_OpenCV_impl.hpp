//Taken from: http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
//http://stackoverflow.com/questions/15611872/bow-in-opencv-using-precomputed-features

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
BoW::create_vocabulary(int N_cent, const string path_run_folders)
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
    const uword N_max = 100000;  // maximum number of vectors per action to create universal GMM
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
  
  
  cout << "Create Vocabulary" << endl; 
  
  fmat f_uni_features = conv_to< fmat >::from(uni_features);
  //uni_features.reset();
  
  
  cv::Mat featuresUnclustered(f_uni_features.n_cols, dim, CV_32FC1, f_uni_features.memptr() );
  
  //cv::Mat featuresUnclustered( featuresUnclusteredTMP.t() );
  
  int rows = featuresUnclustered.rows;
  int cols = featuresUnclustered.cols;
  
  cout << "OpenCV rows & cols " << rows << " & " << cols << endl;
  //cout << "Press a Key" << endl;
  //getchar();
  
  //cout << f_uni_features.col(1000) << endl;
  //cout << uni_features.col(1000) << endl;
  //cout << featuresUnclustered.row(1000) << endl;
  
  //cout << "Press a Key" << endl;
  //getchar();
  
  
  
  
  
  //Construct BOWKMeansTrainer
  //the number of bags
  int dictionarySize = N_cent;
  
  //define Term Criteria
  cv::TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
  //retries number
  int retries=1;
  //necessary flags
  int flags=cv::KMEANS_PP_CENTERS;
  //Create the BoW (or BoF) trainer
  cv::BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
  //cluster the feature vectors
  cv::Mat dictionary = bowTrainer.cluster(featuresUnclustered);  
  
  //Displaying # of Rows&Cols
  int rows_dic = dictionary.rows;
  int cols_dic = dictionary.cols;
  cout << "OpenCV Dict rows & cols " << rows_dic << " & " << cols_dic << endl;
  
  
  
  
  //store the vocabulary
  std::stringstream name_vocabulary;
  name_vocabulary << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".yml"; 
  
  cv::FileStorage fs(name_vocabulary.str(), cv::FileStorage::WRITE);
  fs << "vocabulary" << dictionary;
  fs.release();
  cout << "DONE"<< endl;
  
}




//Training
inline
void
BoW::create_histograms(int N_cent, const string path_run_folders) 
{
  
  //Hacer para todas las personas, luego en la parte de entrenamiento no se usan todos
  //Step 2 - Obtain the BoF descriptor for given image/video frame. 
  
  //prepare BOW descriptor extractor from the dictionary    
  cv::Mat dictionary; 
  std::stringstream name_vocabulary;
  name_vocabulary << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".yml"; 
  cout << name_vocabulary.str() << endl;
  cv::FileStorage fs(name_vocabulary.str(), cv::FileStorage::READ);
  fs["vocabulary"] >> dictionary;
  fs.release();    
  //cout << "Loaded" << endl;
  
  
  int rows_dic = dictionary.rows;
  int cols_dic = dictionary.cols;
  //cout << "OpenCV Dict rows & cols " << rows_dic << " & " << cols_dic << endl;
  
  vec hist;
  
  
  for (uword pe=0; pe<peo_train.n_rows; ++pe)    
  {
    for (uword act = 0 ; act < actions.n_rows;  ++act) 
    {
      for (uword sc = 1 ; sc <= 4;  ++sc) 
      { 
	
	mat mat_features_video_i;
	std::stringstream ssName_feat_video;
	//ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
	ssName_feat_video << path_run_folders <<"/features_all_nor/feat_vec_" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
	//cout << ssName_feat_video.str() << endl;
	
	mat_features_video_i.load( ssName_feat_video.str() );
	
	fmat f_mat_features_video_i = conv_to< fmat >::from(mat_features_video_i);
	mat_features_video_i.reset();
	
	
	cv::Mat features_video_i_OpenCV(f_mat_features_video_i.n_cols, dim, CV_32FC1, f_mat_features_video_i.memptr() );
	
	
	int rows = features_video_i_OpenCV.rows;
	int cols = features_video_i_OpenCV.cols;
	
	//cout << "Features rows & cols " << rows << " & " << cols << endl;
	
	
	// init the matcher with you pre-trained codebook
	cv::Ptr<cv::DescriptorMatcher > matcher = new cv::BFMatcher(cv::NORM_L2);
	matcher->add(std::vector<cv::Mat>(1, dictionary));
	
	
	// matches
	std::vector<cv::DMatch> matches;	 
	
	matcher->match(features_video_i_OpenCV,matches);
	
	
	//cout << matches.size() << endl;
	//Mira aqui: http://ttic.uchicago.edu/~mostajabi/Tutorial.html
	hist.zeros(N_cent) ;
	
	for (int i=0; i< matches.size(); ++i)
	{
	  //cout <<  matches[i].trainIdx << " " ;
	  int bin =  matches[i].trainIdx ;
	  hist(bin)++;
	  
	  
	}
	
	//getchar();
	//cout << hist.t() << endl;
	hist = hist/hist.max();
	//cout << hist.n_elem << endl;
	std::stringstream ssName_hist;
	
	
	ssName_hist << "./run"<<run << "/Histograms_BoW_OpenCV/hist_" << peo_train(pe) << "_" << actions(act) << "_d" << sc << "_Ng"<< N_cent << ".h5";
	
	hist.save(ssName_hist.str(), hdf5_binary);
      }
    }
  }
}


//Training
inline
void
BoW::create_histograms_testing(int N_cent, const string path_run_folders, int segm_length) 
{
  
  //prepare BOW descriptor extractor from the dictionary    
  cv::Mat dictionary; 
  std::stringstream name_vocabulary;
  name_vocabulary << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".yml"; 
  cout << name_vocabulary.str() << endl;
  cv::FileStorage fs(name_vocabulary.str(), cv::FileStorage::READ);
  fs["vocabulary"] >> dictionary;
  fs.release();    
  //cout << "Loaded" << endl;
  
  
  
  mat multi_features;
  vec real_labels, fr_idx, fr_idx_2;
  
  for (uword sc = 1 ; sc <= 4;  ++sc) 
  { 
    for (uword pe=0; pe<peo_test.n_rows; ++pe)    
    {
      
      //Loading matrix with all features (for all frames)
      std::stringstream ssName_feat_video;
      //ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
      ssName_feat_video << path_run_folders << "/run" << run <<  "/multi_features/feat_" << peo_test(pe) << "_d" << sc << ".dat";
      multi_features.load( ssName_feat_video.str() );
      
      //Loading labels. In a frame basis
      std::stringstream ssload_name_lab;       
      ssload_name_lab << path_run_folders << "/run" << run <<  "/multi_features/lab_" << peo_test(pe) << "_d" << sc << ".dat";
      real_labels.load( ssload_name_lab.str() );
      int n_frames = real_labels.n_elem;
      
      //Loading frame index for each of the feature vector in feat_video
      
      std::stringstream ssload_name_fr_idx;
      
      ssload_name_fr_idx << path_run_folders << "/run" << run <<  "/multi_features/fr_idx_" << peo_test(pe) << "_d" <<  sc << ".dat";
      fr_idx.load( ssload_name_fr_idx.str() );  // Solo uso las pares: 2,4,6...
      fr_idx_2 = fr_idx/2; // Empieza en uno
      
      
      for (int f=0; f<n_frames - segm_length; ++f)
      {
	int ini = f;
	int fin = ini + segm_length; 
	mat feat_frame_fr;
	
	for (int i=ini; i<=fin; ++i)
	{
	  
	  uvec q1 = find(fr_idx_2 == i);
	  mat sub_multi_features;
	  sub_multi_features = multi_features.cols(q1);
	  feat_frame_fr = join_rows( feat_frame_fr, sub_multi_features );
	}
	
	//Aqui Obtener el histogram y guardarlo!!!!!!!!!!!!!!!
	
	fmat f_feat_frame_fr = conv_to< fmat >::from(feat_frame_fr);
	feat_frame_fr.reset();
	
	
	cv::Mat features_segm_f_OpenCV(f_feat_frame_fr.n_cols, dim, CV_32FC1, f_feat_frame_fr.memptr() );
	
	
	int rows = features_segm_f_OpenCV.rows;
	int cols = features_segm_f_OpenCV.cols;
	
	//cout << "Features rows & cols " << rows << " & " << cols << endl;
	
	
	// init the matcher with you pre-trained codebook
	cv::Ptr<cv::DescriptorMatcher > matcher = new cv::BFMatcher(cv::NORM_L2);
	matcher->add(std::vector<cv::Mat>(1, dictionary));
	
	
	// matches
	std::vector<cv::DMatch> matches;	 
	
	matcher->match(features_segm_f_OpenCV,matches);
	
		//cout << matches.size() << endl;
	//Mira aqui: http://ttic.uchicago.edu/~mostajabi/Tutorial.html
	vec hist;
	hist.zeros(N_cent) ;
	
	for (int i=0; i< matches.size(); ++i)
	{
	  int bin =  matches[i].trainIdx ;
	  hist(bin)++;
	}
	

	hist = hist/hist.max();
	std::stringstream ssName_hist;
	ssName_hist << "./run"<<run << "/multi_Histograms_BoW_OpenCV/multi_hist_" << peo_test(pe) << "_d" << sc << "_Ng"<< N_cent << "fr_" << ini << "_"  << fin  << ".h5";
	cout << ssName_hist.str() << endl;
	hist.save(ssName_hist.str(), hdf5_binary);
      }
    }

  }
  
}


// Tengo que usarlo en Matlab, debido a que en el testing necesito las Probabilidades
// 
inline
void
BoW::train_svm(int N_cent, const string path_run_folders)
{
  
  
  vec hist;
  int k =0;
  int n_samples = 4*actions.n_rows*peo_train.n_rows;
  
  mat training_data_hist(N_cent, n_samples); 
  float fl_labels[n_samples] ;
  
  for (uword pe=0; pe<peo_train.n_rows; ++pe)    
  {
    for (uword act = 0 ; act < actions.n_rows;  ++act) 
    {
      for (uword sc = 1 ; sc <= 4;  ++sc) 
      { 
	
	std::stringstream ssName_hist;
	
	ssName_hist << "./run"<<run << "/Histograms_BoW_OpenCV/hist_" << peo_train(pe) << "_" << actions(act) << "_d" << sc << "_Ng"<< N_cent << ".h5";
	
	hist.load(ssName_hist.str(), hdf5_binary);
	
	training_data_hist.col(k) = hist;
	fl_labels[k] = act;
	k++;
	
      }
    }
  }
  
  
  fmat float_training_data_hist;
  float_training_data_hist = conv_to< fmat >::from(training_data_hist);
  training_data_hist.reset();
  
  cv::Mat cv_training_data_hist(float_training_data_hist.n_cols, dim, CV_32FC1, float_training_data_hist.memptr() );
  cv::Mat cv_Labels(n_samples, 1, CV_32FC1,fl_labels );
  
  float_training_data_hist.reset();
  
  cout << "Setting parameters" << endl;
  // http://ttic.uchicago.edu/~mostajabi/Tutorial.html
  
  
  CvSVMParams params;
  params.kernel_type=CvSVM::RBF;
  params.svm_type=CvSVM::C_SVC;
  params.gamma=0.50625000000000009;
  params.C=312.50000000000000;
  params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,0.000001);
  CvSVM svm;
  
  printf("%s\n","Training SVM classifier");
  
  bool res=svm.train(cv_training_data_hist,cv_Labels,cv::Mat(),cv::Mat(),params); 
  
  std::stringstream ssName_svm;
  
  ssName_svm <<  "./run"<<run << "svm_model_Ng" << N_cent;
  
  //svm.save( ssName_svm.str() );
  
  
}