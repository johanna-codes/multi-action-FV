//Features as per Andres' WACV paper

inline
opt_feat::opt_feat(const std::string in_actionNames,  
		   const uword in_col, 
		   const uword in_row,
		   const int in_run,
		   const int in_dim
)
// //actionNames, co, ro, run, 
:actionNames(in_actionNames), col(in_col), row(in_row), run(in_run), dim(in_dim)
{
  THRESH_2 = col*row*1/100;
  actions.load( actionNames );  //actions.print("All actions");
  ismultiAction = false; // per default is false
  
}


/*
 * Use opt_feat::features_allpeople to pre-calculate all features videos just once
 * inline
 * void
 * opt_feat::features_training ( field <string> peo_train, string single_path,  int sc ) // Which scenario??
 * {
 * 
 * 
 *  for (uword pe = 0; pe< peo_train.n_rows; ++pe)
 *  {
 *    for (uword act = 0; act< actions.n_elem; ++act)
 *    {
 *      feat_all_videos_action_i.clear(); 
 *      std::stringstream ssVideo_path;
 *      ssVideo_path << single_path << peo_train(pe) << "_" << actions(act) << "_d" << sc <<"_uncomp.avi";
 *      //cout << ssVideo_path.str()<< endl;
 *      
 *      cout << "Calculating features vector per video:" << ssVideo_path.str() << endl;
 *      feature_video(ssVideo_path.str()); //feat_all_video_action_i is calculated inside this method
 *      cout << "# of Vectors: " << feat_all_videos_action_i.size() << endl;
 *      
 *      cout << "Converting to Arma:" << endl;
 *      cout << "# of Vectors: " << feat_all_videos_action_i.size() << endl;
 *      
 *      mat mat_features(dim,feat_all_videos_action_i.size());
 *      
 *      for (uword i = 0; i < feat_all_videos_action_i.size(); ++i)
 *      {
 *	mat_features.col(i) = feat_all_videos_action_i.at(i);
 *	
 }
 
 std::stringstream ssName_feat_video;
 ssName_feat_video << "./run"<< run <<"/features/train/feat_vec" << peo_train(pe) << "_" << actions(act) << "_d" << sc;
 cout << "Saving at " << ssName_feat_video.str() << endl;
 mat_features.save( ssName_feat_video.str(), raw_ascii );
 //getchar();
 
 }
 
 
 
 }
 
 
 
 }
 
 */

//L2 normalisation included on February 14th/2015

inline
void
opt_feat::features_allpeople ( field <string> people, string single_path,  int sc ) // Which scenario??
{
  
  
  for (uword pe = 0; pe< people.n_rows; ++pe)
  {
    for (uword act = 0; act< actions.n_elem; ++act)
    {
      feat_all_videos_action_i.clear(); 
      std::stringstream ssVideo_path;
      ssVideo_path << single_path << people(pe) << "_" << actions(act) << "_d" << sc <<"_uncomp.avi";
      //cout << ssVideo_path.str()<< endl;
      
      cout << "Calculating features vector per video:" << ssVideo_path.str() << endl;
      feature_video(ssVideo_path.str()); //feat_all_video_action_i is calculated inside this method
      cout << "# of Vectors: " << feat_all_videos_action_i.size() << endl;
      
      cout << "Converting to Arma:" << endl;
      cout << "# of Vectors: " << feat_all_videos_action_i.size() << endl;
      
      mat mat_features(dim,feat_all_videos_action_i.size());
      
      for (uword i = 0; i < feat_all_videos_action_i.size(); ++i)
      {
	mat_features.col(i) = feat_all_videos_action_i.at(i)/norm(feat_all_videos_action_i.at(i),2);
	
      }
      
      std::stringstream ssName_feat_video;
      ssName_feat_video << "./features_all_nor/feat_vec_" << people(pe) << "_" << actions(act) << "_d" << sc;
      cout << "Saving at " << ssName_feat_video.str() << endl;
      mat_features.save( ssName_feat_video.str(), raw_ascii );
      //getchar();
      
    }
    
    
    
  }
  
  
  
}


//*******************************************************************************************
//***************************Feature per video: Training and Testing ************************

inline 
void
opt_feat::feature_video(std::string one_video)
{
  
  std::vector < vec > feat_frame;
  
  
  cv::VideoCapture capVideo(one_video);
  //cout << one_video << endl;
  //double fps = capVideo.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video
  //cout << "Frame per seconds : " << fps << endl;
  
  //cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
  
  double frmcount = capVideo.get(CV_CAP_PROP_FRAME_COUNT);
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
  int t = -1;
  
  //Saving each two frames
  int par_fr = 0;
  int len = ceil(frmcount/2)-1; //0 and 1 not used
  //featuresframe_video_i.set_size( len );
  //cout << "Len " << len << endl;
  //lab_feature_vectors.set_size( frmcount );
  
  
  for(uword fr=0; fr<frmcount; fr++){
    
    //cout << t << " " ;
    
    bool bSuccess = capVideo.read(frame); // read a new frame from video
    
    if (!bSuccess) //if not success, break loop
	{
	  //cout << "Cannot read the frame from video file" << endl;
	  break;
	}
	t++;
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
	    
	    if (gm>40 && is_finite( features_one_pixel ) && is_zero!=0 ) // Empirically set to 40
			    {	
			      /*
			       *			      frame.at<cv::Vec3b>(y,x)[0] = 0;
			       *			      frame.at<cv::Vec3b>(y,x)[1] = 0;
			       *			      frame.at<cv::Vec3b>(y,x)[2] = 255;
			       */
			      
			      feat_frame.push_back(features_one_pixel);
			    }
	  }
	}
	
	
	string text;
	
	if (!ismultiAction) // For Single Videos
    {
      if (feat_frame.size() > THRESH_2) //frames has enough moving pixels
      {
	
	
	//fr es par?
	if ( (fr % 2 ) == 0 ) 
	{
	  feat_all_videos_action_i.insert(feat_all_videos_action_i.end(), feat_frame.begin(), feat_frame.end()  );
	  text = "Frame used";
	  par_fr++;//este contador es solo util en multi-action videos
	  
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
      //cout << "Frame " << t << ". Feature vectors "<< feat_frame.size() << endl;
      //cout << "Llega aqui? ";
      text = "Frame wasn't discarded. Multivideo";
      
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
    
    
    
    
    
    
    feat_frame.clear();
  }
  //getchar();
  
}

//******************TESTING MULTI ACTIONS***********************************
//***************************************************************************

inline 
void 
opt_feat::feature_multi_action(field <string> peo_test, string multi_path)
{
  ismultiAction = true; // per default is false
  
  for (int sc=1; sc <=4; ++sc)
  {
    for (uword vi = 0; vi <peo_test.n_rows; ++vi ){ //videos.n_rows

      
      multi_features.clear(); 
      lab_feature_vectors.clear();
      frame_index.clear();
      
      
      std::stringstream ssTest;
      ssTest << multi_path << peo_test(vi) << "_d" << sc << "_multiactions.avi";
      cout << ssTest.str()<< endl;
      
      std::stringstream tmp_lbs;
      tmp_lbs << multi_path << peo_test(vi) << "_d" << sc << "_MultiLabels.dat";
      cout << tmp_lbs.str() << endl;
      arma_multi_labels.load(tmp_lbs.str(), raw_ascii); //labels are in a frame basis.
      
      
      
      cout << "Calculating features vector per video ..." << peo_test(vi) << "_d" << sc << endl;;
      feature_video(ssTest.str()); //multi_features is calculated inside this method
      
      
      
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
      tmp_ss4     << "./run"<< run <<"/multi_features/feat_"<<  peo_test(vi) << "_d" << sc << ".dat";  
      
      std::stringstream tmp_vec_lab;
      tmp_vec_lab << "./run"<< run <<"/multi_features/lab_" <<  peo_test(vi) << "_d" << sc << ".dat";  
      
      std::stringstream tmp_vec_fr_index;
      tmp_vec_fr_index << "./run"<< run <<"/multi_features/fr_idx_" <<  peo_test(vi) << "_d" << sc << ".dat";  
      
      
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
  
}


