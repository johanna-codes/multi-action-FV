//Taken from: http://www.codeproject.com/Articles/619039/Bag-of-Features-Descriptor-on-SIFT-Features-with-O
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
  //store the vocabulary
  std::stringstream name_vocabulary;
  name_vocabulary << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".yml"; 
  
  cv::FileStorage fs(name_vocabulary.str(), cv::FileStorage::WRITE);
  fs << "vocabulary" << dictionary;
  fs.release();
  cout << "DONE"<< endl;

}



/*

inline
void
BoW::create_histograms() 
{
  
  
    //Step 2 - Obtain the BoF descriptor for given image/video frame. 

    //prepare BOW descriptor extractor from the dictionary    
    cv::Mat dictionary; 
    std::stringstream name_vocabulary;
    name_vocabulary << "./run"<< run <<"/visual_vocabulary/means_Ng" << N_cent << "_dim" <<dim << "_all_sc" << ".yml"; 
    FileStorage fs(name_vocabulary.str(), FileStorage::READ);
    fs["vocabulary"] >> dictionary;
    fs.release();    
    
    
    
    //create a nearest neighbor matcher
    Ptr<DescriptorMatcher> matcher(new FlannBasedMatcher);
    //create Sift feature point extracter
    Ptr<FeatureDetector> detector(new SiftFeatureDetector());
    //create Sift descriptor extractor
    Ptr<DescriptorExtractor> extractor(new SiftDescriptorExtractor);    
    //create BoF (or BoW) descriptor extractor
    BOWImgDescriptorExtractor bowDE(extractor,matcher);
    //Set the dictionary with the vocabulary we created in the first step
    bowDE.setVocabulary(dictionary);
 
    //To store the image file name
    char * filename = new char[100];
    //To store the image tag name - only for save the descriptor in a file
    char * imageTag = new char[10];
 
    //open the file to write the resultant descriptor
    FileStorage fs1("descriptor.yml", FileStorage::WRITE);    
    
    //the image file with the location. change it according to your image file location
    sprintf(filename,"G:\\testimages\\image\\1.jpg");        
    //read the image
    Mat img=imread(filename,CV_LOAD_IMAGE_GRAYSCALE);        
    //To store the keypoints that will be extracted by SIFT
    vector<KeyPoint> keypoints;        
    //Detect SIFT keypoints (or feature points)
    detector->detect(img,keypoints);
    //To store the BoW (or BoF) representation of the image
    Mat bowDescriptor;        
    //extract BoW (or BoF) descriptor from given image
    bowDE.compute(img,keypoints,bowDescriptor);
 
    //prepare the yml (some what similar to xml) file
    sprintf(imageTag,"img1");            
    //write the new BoF descriptor to the file
    fs1 << imageTag << bowDescriptor;        
 
    //You may use this descriptor for classifying the image.
            
    //release the file storage
    fs1.release();
 

  
}*/