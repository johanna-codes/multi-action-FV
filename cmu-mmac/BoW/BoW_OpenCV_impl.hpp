inline
BoW::BoW(const std::string in_path,
		   const std::string in_actionNames,  
		   const uword in_col, 
		   const uword in_row
)
:path(in_path), actionNames(in_actionNames), col(in_col), row(in_row)
{
  THRESH_2 = col*row*1/100;
  actions.load( actionNames );  //actions.print("All actions");
  ismultiAction = false; // per default is false
  dim = 14;
}


// //******************Universal GMM *******************************************
// //***************************************************************************

inline
void
BoW::vocabulary(field <string>  peo_train, int N_cent, int run, const string path_run_folders)
{ 
  
  
  cout << "Calculating Vocabulary " << endl;
  cout << "# clusters: " << N_cent << endl;
  //peo_train.print();
  mat uni_features;
  
   for (uword pe=0; pe<peo_train.n_rows; ++pe)    {
     
     //cout << "Doing for person: " << peo_train (pe) << endl;
     mat mat_features_tmp;
     mat mat_features;
     
     for (uword act = 0 ; act < actions.n_rows;  ++act) {
       
       
       mat mat_features_video_i;
       std::stringstream ssName_feat_video;
       ssName_feat_video << path_run_folders << "/features_training/feature_vectors_dim" << dim << "_" << peo_train (pe) << "_" << actions(act)<< ".dat";
       
       //cout << ssName_feat_video.str() << endl;
       //getchar();
       mat_features_video_i.load( ssName_feat_video.str() );
       
       if ( mat_features_video_i.n_cols>0 )
       {
	 mat_features_tmp	 = join_rows( mat_features_tmp, mat_features_video_i );
	 
      }
      else 
      {
	cout << "# vectors = 0 in " << ssName_feat_video.str() << endl;
	
      }
      
      mat_features_video_i.reset();

       
    }
    
    cout << "mat_features_tmp.n_cols "<< mat_features_tmp.n_cols << endl;
    const uword N_max = 100000;  
    
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
    
      //cout << "mat_features.n_cols "<< mat_features.n_cols << endl;
    
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
    
    fmat f_uni_features = conv_to< fmat >::from(uni_features);
  //uni_features.reset();
  
  
  cv::Mat featuresUnclustered(f_uni_features.n_cols, dim, CV_32FC1, f_uni_features.memptr() );
  
  //cv::Mat featuresUnclustered( featuresUnclusteredTMP.t() );
  
  int rows = featuresUnclustered.rows;
  int cols = featuresUnclustered.cols;
  
  cout << "OpenCV rows & cols " << rows << " & " << cols << endl;
 
  
  
  
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
BoW::create_hist_train(field <string>  peo_train, int N_cent, int run, const string path_run_folders) 
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

	
	mat mat_features_video_i;
	std::stringstream ssName_feat_video;
	//ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
	
	ssName_feat_video << path_run_folders << "/features_training/feature_vectors_dim" << dim << "_" << peo_train(pe) << "_" << actions(act) << ".dat";
            
	//cout << ssName_feat_video.str() << endl;
	
	mat_features_video_i.load( ssName_feat_video.str() );
	
	int n_r = mat_features_video_i.n_rows;
	int n_c = mat_features_video_i.n_cols;
	int max_n_frames = 1000000;
	if (n_c>max_n_frames)
	{
	  cout << "Matrix so big" << endl;
	  mat_features_video_i = mat_features_video_i.submat( 0, 0, dim-1, max_n_frames-1);
	  
	}
	
	
	
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
	
	
	ssName_hist << "./run"<<run << "/Histograms_BoW_OpenCV/hist_" << peo_train(pe) << "_" << actions(act) <<  "_Ng"<< N_cent << ".h5";
	
	hist.save(ssName_hist.str(), hdf5_binary);
 
    }
  }
}
