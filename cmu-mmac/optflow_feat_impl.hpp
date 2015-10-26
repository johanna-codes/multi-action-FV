//Features as per Andres' WACV paper

inline
opt_feat::opt_feat(const std::string in_path,
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



inline
void
opt_feat::features_per_action_training( field<string>peo_train ) 
{
  std::vector < vec > feat_all_videos_action_i;
  int n_actions = actions.n_rows;
  //cout << "size " << all_actions_matrix.size() << endl;
  
  /*for (uword b=0;  b< all_actions_matrix.size(); ++b)
   *    {
   *      cout << all_actions_matrix.at(b).size() << " ";
   }
   */cout << endl;
   
   
   int n_peo_tr =  peo_train.n_rows;
   peo_train.print("people");
   
   for (int pe = 0; pe< n_peo_tr; ++pe)
   {
     
     all_actions_matrix.clear();
     all_actions_matrix.resize(n_actions);
     
     std::stringstream ss_video_name;
     ss_video_name << path <<  peo_train (pe) << "_Brownie.avi";
     cout << ss_video_name.str() << endl;
     
     std::stringstream ss_label_name;
     ss_label_name << path <<  peo_train (pe) << "_unique_verbs_labels.dat";
     arma_multi_labels.load( ss_label_name.str(), raw_ascii ); //labels are in a frame basis.

     
     
     std::stringstream ss_range_frames;
     ss_range_frames << path <<  peo_train (pe) << "_start_end.dat";
     
     
     
     feature_video(ss_video_name.str(), ss_range_frames.str() ) ; //all_actions_matrix is calculated inside this method
     
     
     
     cout << "size " << all_actions_matrix.size() << endl;
     for (uword b=0;  b< all_actions_matrix.size(); ++b)
     {
       cout << all_actions_matrix.at(b).size() << " ";
       
    }
     cout << endl;
       
       for (int act=0; act<n_actions; ++act)
       {
	 feat_all_videos_action_i = all_actions_matrix.at(act);
	 all_actions_matrix.at(act).clear(); // releasing memory
	 //cout << feat_all_videos_action_i.size() << endl;
	 //cout << feat_all_videos_action_i.at(0).size() << endl;
	 
	 
	 mat mat_features;
	 if (feat_all_videos_action_i.size()>0)
	 {
	   mat_features.zeros(dim,feat_all_videos_action_i.size());
	   //cout << mat_features.n_rows << " " << mat_features.n_cols << endl;
	   
	   for (uword i = 0; i < feat_all_videos_action_i.size(); ++i)
	   {
	     mat_features.col(i) = feat_all_videos_action_i.at(i)/norm(feat_all_videos_action_i.at(i),2);
	     //cout << i << " " ;
	     
	   }
	 }
	 else
	 {
	   mat_features.zeros(dim,0);
	 }
	 std::stringstream tmp_ss4;
	 //tmp_ss4 << "./features_training_" << col << "x" << row << "/feature_vectors_dim" << dim << peo_train (pe) << "_" << actions(act)<< ".dat";
	 tmp_ss4 << "./features_training/feature_vectors_dim" << dim << "_" << peo_train (pe) << "_" << actions(act)<< ".dat";

	 cout << tmp_ss4.str() << endl;;
	 mat_features.save( tmp_ss4.str(), raw_ascii );
       }
   }
}

// //****************** Feature Extraction**************************************
// //***************************************************************************
inline 
void
opt_feat::feature_video(std::string one_video, std::string sRange_frames)
{
  //cout << "Entra aqui" << endl;
  cout << sRange_frames << endl;;
  std::vector < vec > feat_frame;
  rowvec range_frame;
  range_frame.load( sRange_frames );
  //range_frame.print("ini end");
  int ini_frame = range_frame(0);
  
  int n_frames = arma_multi_labels.n_elem;
  
  
  cv::VideoCapture capVideo(one_video);
  //cout << one_video << endl;
  //double fps = capVideo.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
  //cout << "Frame per seconds : " << fps << endl;
  
  //cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
  
  //double frmcount = capVideo.get(CV_CAP_PROP_FRAME_COUNT);
  capVideo.set(CV_CAP_PROP_POS_FRAMES,ini_frame);
  //cout << "# of frames is: " << frmcount << endl;
  
  if( !capVideo.isOpened() )
  {
    cout << "Video couldn't be opened" << endl;
    return;
  }
  
  cv::Mat prevgray, gray, flow, cflow, frame, prevflow;
  cv::Mat ixMat, iyMat, ixxMat, iyyMat;
  //cv::namedWindow("My Video", 1);
  //running_stat_vec<vec> stats_video(true);
  //cout << "Frame: ";
 
  //Saving each two frames
  int par_fr = 0;
  string text;
  
  
  for(int fr=0; fr<n_frames; fr++){
    
    //cout << fr << " " ;
    
    bool bSuccess = capVideo.read(frame); // read a new frame from video
    
    if (!bSuccess) //if not success, break loop
	{
	  //cout << "Cannot read the frame from video file" << endl;
	  break;
	}
    
    cv::resize( frame, frame, cv::Size(row,col) );
    cv::cvtColor(frame, gray, CV_BGR2GRAY);
    
    //cv::cvtColor(frame,gray, cv::COLOR_BGR2GRAY);//For Opencv 3.0 (installed at home)
    
    if( prevgray.data )
    {
      //cout << t << " " ;
      cv::calcOpticalFlowFarneback(prevgray, 
				   gray, 
				   flow, 
				   0.5, //pyr_scale
				   3,   //levels
				   9,   //winsize
				   1,   //iterations
				   5,   //poly_n
				   1.1, //poly_sigma
				   0);  //flags
      
      
      cv::Sobel(gray, ixMat, CV_32F, 1, 0, 1);
      cv::Sobel(gray, iyMat, CV_32F, 0, 1, 1);
      cv::Sobel(gray, ixxMat, CV_32F, 2, 0, 1);
      cv::Sobel(gray, iyyMat, CV_32F, 0, 2, 1);
      
      float  ux = 0, uy = 0, vx = 0,  vy = 0;
      float u, v;
      
      if( prevflow.data )
      {
	
	for (uword x = 0 ; x < col ; ++x ){
	  for (uword y = 0 ; y < row ; ++y ) {
	    
	    vec features_one_pixel(dim);
	    u = flow.at<cv::Vec2f>(y, x)[0];
	    v = flow.at<cv::Vec2f>(y, x)[1];
	    
	    //cout << "x= " << x << " - y= " << y << endl;
	    // x grad
	    //cout << " x y grad" << endl;
	    float ix = ixMat.at<float>(y, x);
	    //cout << " y grad" << endl;
	    float iy = iyMat.at<float>(y, x);
	    
	    // grad direction &  grad magnitude
	    //cout << "grad direction &  grad magnitude" << endl;
	    float gd = std::atan2(std::abs(iy), std::abs(ix));
	    float gm = std::sqrt(ix * ix + iy * iy);
	    
	    // x second grad
	    //cout << "x y  second grad " << endl;
	    float ixx = ixxMat.at<float>(y, x);
	    // y second grad
	    float iyy = iyyMat.at<float>(y, x);
	    
	    //du/dt
	    float ut = u - prevflow.at<cv::Vec2f>(y, x)[0];
	    // dv/dt
	    float vt = v - prevflow.at<cv::Vec2f>(y, x)[1];
	    
	    //// divergence &  vorticity
	    //cout << "divergence &  vorticity" << endl;
	    if (x>0 && y>0 )
	    {
	      ux = u - flow.at<cv::Vec2f>(y, x - 1)[0];
	      uy = u - flow.at<cv::Vec2f>(y - 1, x)[0];
	      vx = v - flow.at<cv::Vec2f>(y, x - 1)[1];
	      vy = v - flow.at<cv::Vec2f>(y - 1, x)[1];
	    }
	    //int x_submat = x + rec.x;
	    //int y_submat = y + rec.y;
	    //cout << x_submat << "&" << y_submat << endl;
	    
	    
	    
	    features_one_pixel  << x << y << abs(ix) << abs(iy) << abs(ixx) 
	    << abs(iyy) << gm << gd <<  u << v << abs(ut) 
	    << abs(vt) << (ux + vy)  << (vx - uy);
	    //features_one_pixel.t().print("Features Current Pixel: ");
	    //getchar();
	    
	    
	    if (!is_finite( features_one_pixel ) )
	    {
	      cout << "It's not FINITE... continue???" << endl;
	      getchar(); 
	    }
	    // Plotting Moving pixels
	    //cout << " " << gm;
	    
	    double is_zero = accu(abs(features_one_pixel));
	    //cout << gm << " " ;
	    
	    if (gm>40 && is_finite( features_one_pixel ) && is_zero!=0 ) // Empirically set to 40
			    {	
			      
			       			      //frame.at<cv::Vec3b>(y,x)[0] = 0;
			       			      //frame.at<cv::Vec3b>(y,x)[1] = 0;
			       			      //frame.at<cv::Vec3b>(y,x)[2] = 255;
			       
			      
			      feat_frame.push_back(features_one_pixel);
			    }
	  }
	}
	
	
	
	
	if (!ismultiAction) // For Single Videos
	{
	  if (feat_frame.size() > THRESH_2) //frames has enough moving pixels
	  {
	    //cout << feat_frame.size() << endl;
	    //fr es par?
	    if ( (fr % 2 ) == 0 ) 
	    {
	      //feat_all_videos_action_i.insert(feat_all_videos_action_i.end(), feat_frame.begin(), feat_frame.end()  );
	      uword pos = arma_multi_labels(fr) - 1;
	      all_actions_matrix.at(pos).insert(all_actions_matrix.at(pos).end(), feat_frame.begin(), feat_frame.end()  );
	      //text = "Frame used";
	    }
	    
	  }
	  else
	  {
	    text = "Frame discarded";
	    //cout << "Frame discarded" << endl;
	    
	  }
	  
	}
    
    
    else//For multiaction
    {

      //text = "Frame wasn't discarded. Multivideo";
      
      if ( (fr % 2 ) == 0 ) 
      {
	//cout << "fr= " << fr << " par_fr= " << par_fr << " " ;
	mat feat_frame_i( dim,feat_frame.size() );
	//multi_features
	for (uword l = 0; l<feat_frame.size(); ++l )
	{
	  vec tmp_vec = feat_frame.at(l);
	  multi_features.push_back(tmp_vec);
	  frame_index.push_back(fr);
	}
	uword lab = arma_multi_labels(fr);
	lab_feature_vectors.push_back( lab ); 
	par_fr++;
	
      }
      
    }
	
      }
      
    }
    
    if(cv::waitKey(30)>=0)
      break;
    
    
    std::swap(prevgray, gray);
    std::swap(prevflow, flow);
    
    
     //stringstream tostring;
     //tostring << fr << " " << actions( labels( fr ) - 1) ;
     //text = tostring.str();
//     int fontFace = cv::FONT_HERSHEY_PLAIN;
//     double fontScale = 1.5;
//     int thickness = 1;  
//     cv::Point textOrg(20, 20);
//     cv::putText(frame, text, textOrg, fontFace, fontScale, cv::Scalar::all(255), thickness,8);
    
    //cv::imshow("color", frame);
    //cv::waitKey(5);
    
    
    
    feat_frame.clear();
  }
  //getchar();
  
}

// //******************TESTING MULTI ACTIONS***********************************
// //***************************************************************************

inline 
void 
opt_feat::feature_multi_action( field<string> peo_test )
{
  ismultiAction = true; // per default is false
  
  int n_peo_test =  peo_test.n_rows;
  
  for (int pe = 0; pe <n_peo_test; ++pe ){ //videos.n_rows

      
         multi_features.clear(); 
	 lab_feature_vectors.clear();
	 frame_index.clear();
	 
	 
	std::stringstream ss_video_name;
	ss_video_name << path <<  peo_test(pe) << "_Brownie.avi";
	
	std::stringstream ss_label_name;
	ss_label_name << path <<  peo_test(pe) << "_unique_verbs_labels.dat";
	arma_multi_labels.load( ss_label_name.str(), raw_ascii); //labels are in a frame basis.
	
	
	
	std::stringstream ss_range_frames;
	ss_range_frames << path <<  peo_test (pe) << "_start_end.dat";
	
	
	cout << ss_video_name.str() << endl;
	feature_video(ss_video_name.str(), ss_range_frames.str() ) ;  
	
      //cout << "Total feature vectors: " << multi_features.size()<< endl;
      //cout << "Total labels: " << lab_feature_vectors.size() << endl;
      
      cout << "Converting to Arma:" << endl;
      cout << "# of Frames: "  << lab_feature_vectors.size() << endl;
      cout << "# of Vectors: " << multi_features.size() << endl;
      
      uvec lab_feature_vectors_arma( lab_feature_vectors.size() );
      uvec frame_index_arma( frame_index.size() );
      mat  feature_vectors_arma( dim, multi_features.size() );
      
      
      
      for (uword i = 0; i < multi_features.size(); ++i)
      {
	//cout << i << endl;
	frame_index_arma(i) = frame_index.at(i);
	feature_vectors_arma.col(i)     = multi_features.at(i)/norm(multi_features.at(i),2);
	
	//mat_features.col(i) = feat_all_videos_action_i.at(i)/norm(feat_all_videos_action_i.at(i),2);
	
      }
      
      for (uword i = 0; i < lab_feature_vectors.size(); ++i)
      {
	//cout << i << endl;
	lab_feature_vectors_arma(i) = lab_feature_vectors.at(i);
	
	
      }
      
      
      std::stringstream tmp_ss4;
      //tmp_ss4 << "./multi_features_2/feat_"<<  peo_test(pe) << ".dat"; 
      tmp_ss4 << "./multi_features/feat_" << peo_test(pe) << ".dat"; 
      //load_name = strcat('./multi_features/feat_', people_test(pe),'.dat');
      
      
	 
      
      std::stringstream tmp_vec_lab;
      tmp_vec_lab << "./multi_features/lab_" <<  peo_test(pe) << ".dat";  
      
      std::stringstream tmp_vec_fr_index;
      tmp_vec_fr_index << "./multi_features/fr_idx_" <<  peo_test(pe) <<  ".dat";  
       
	 
      
      //cout << tmp_ss4.str() << endl;
      //cout << "Press a key " << endl;
      //getchar();
      cout << "Saving " << endl;
      feature_vectors_arma.save( tmp_ss4.str(), raw_ascii );
      lab_feature_vectors_arma.save( tmp_vec_lab.str(), raw_ascii );
      frame_index_arma.save( tmp_vec_fr_index.str(), raw_ascii );
      //cout << "Press a key" << endl;
      //getchar();
	
	
  }
  
}

// //******************Universal GMM *******************************************
// //***************************************************************************

inline
void
opt_feat::create_universal_gmm(field <string>  peo_train, int N_cent, int run)
{ 
   mat means;
   mat covs;
   vec weights;	
  
  
  cout << "Calculating Universal GMM " << endl;
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
       ssName_feat_video << "./features_training/feature_vectors_dim" << dim << "_" << peo_train (pe) << "_" << actions(act)<< ".dat";
       
       //cout << ssName_feat_video.str() << endl;
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
  
  
  cout << "universal GMM" << endl;
  gmm_diag gmm_model;
  gmm_diag bg_model;
  
  bool status_em = false;
  int rep_em=0;
   
  int km_iter = 20;
  int em_iter  = 20;
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
  tmp_ss5 << "./run"<< run <<"/universal_GMM/UniversalGMM_Ng" << N_cent << "_dim" <<dim;
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
  
  means = gmm_model.means;
  covs  = gmm_model.dcovs;
  weights = gmm_model.hefts.t();	
  
  //Saving statistics
  std::stringstream ss_weigths;
  ss_weigths << "./run"<< run <<"/universal_GMM/weights_Ng" << N_cent << "_dim" << dim; 
  
  std::stringstream ss_means;
  ss_means << "./run"<< run <<"/universal_GMM/means_Ng" << N_cent << "_dim" << dim; 
  
  std::stringstream ss_covs;
  ss_covs << "./run"<< run <<"/universal_GMM/covs_Ng" << N_cent << "_dim" << dim; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii);

}