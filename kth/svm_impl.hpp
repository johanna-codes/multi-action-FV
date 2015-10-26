inline 
my_svm::my_svm(	 const string  in_actionNames, 
		 const field <string> in_peo_train,
		 const field <string> in_peo_test,
		 const int in_run)
:actionNames(in_actionNames), peo_train(in_peo_train), peo_test(in_peo_test), run(in_run)
{
  actions.load( actionNames );  
  
  
}

///****************************************************************
///***********Training and Testing each scenario separately********
///****************************************************************



inline 
void
my_svm::train(int sc)
{
  
  //Using Means and Covs
  int dim = 7168; //OJO!!!!!!!!!
  
  //Using Weights, Means and Covs.
  //int dim = 7423;
  
  int n_videos = peo_train.n_rows*actions.n_elem;
  mat training_data(dim,n_videos);
  fvec labels = zeros<fvec>(n_videos);
  int k=0;
  
  //loading and creating training_data matrix and labels in Arma
  for (uword pe = 0; pe< peo_train.n_rows; ++pe)
  {
    for (uword act = 0; act< actions.n_elem; ++act)
    {
      std::stringstream ssFV;
      ssFV << "./run" << run << "/features/fv_matlab_nor/FV_" << peo_train(pe)<< "_" << actions(act) << "_sc" << sc <<".txt";
      vec FV;
      FV.load( ssFV.str() );
      //cout << ssFV.str() << endl;
      
      training_data.col(k) = FV;
      labels(k) = act;
      k++;
      //FV.t().print("FV");
      //FV.save("FV_arma.dat", raw_ascii);
      //getchar();
    }
  }
  
  
  //Converting to OpenCV
  cv::Mat cvMatTraining(n_videos, dim, CV_32FC1);
  float fl_labels[n_videos] ;
  
  for (uword m=0; m<n_videos; ++m)
  {
    for (uword d=0; d<dim; ++d)
    {
      cvMatTraining.at<float>(m,d) = training_data(d,m); 
      
    }
    fl_labels[m] = labels(m);
    
  }
  
  cv::Mat cvMatLabels(n_videos, 1, CV_32FC1,fl_labels );
  
  
  cout << "Setting parameters" << endl;
  CvSVMParams params;
  params.svm_type    = CvSVM::C_SVC;
  //params.kernel_type = CvSVM::RBF; 
  params.kernel_type = CvSVM::LINEAR; 
  //params.gamma = 1;
  params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER,  (int)1e7, 1e-6);
  
  
  // Train the SVM
  cout << "Training" << endl;
  CvSVM SVM;
  SVM.train( cvMatTraining , cvMatLabels, cv::Mat(), cv::Mat(), params);
  
  std::stringstream ssSVM;
  ssSVM<< "./run" << run << "/svm/svm_model_run" << run << "_sc" << sc;
  //cout << ssSVM.str() << endl;
  SVM.save( ssSVM.str().c_str() );
  cout << "End of Training" << endl;
  
}

inline 
void
my_svm::test(int sc)
{
  
   //Using Means and Covs
  int dim = 7168; //OJO!!!!!!!!!
  
  //Using Weights, Means and Covs.
  //int dim = 7423;
  
  int n_videos = peo_test.n_rows*actions.n_elem;
  mat testing_data(dim,n_videos);
  fvec labels = zeros<fvec>(n_videos);
  int k=0;
  
  CvSVM SVM;
  std::stringstream ssSVM;
  ssSVM<< "./run" << run << "/svm/svm_model_run" << run << "_sc" << sc;
  SVM.load( ssSVM.str().c_str() );

  //loading and creating testing_data matrix and labels in Arma
  for (uword pe = 0; pe< peo_test.n_rows; ++pe)
  {
    for (uword act = 0; act< actions.n_elem; ++act)
    {
      std::stringstream ssFV;
      ssFV << "./run" << run << "/features/fv_matlab_nor/FV_" << peo_test(pe)<< "_" << actions(act) << "_sc" << sc <<".txt";
      vec FV;
      FV.load( ssFV.str() );
      //cout << ssFV.str() << endl;
      
      testing_data.col(k) = FV;
      labels(k) = act;
      k++;
      //FV.t().print("FV");
      //FV.save("FV_arma.dat", raw_ascii);
      //getchar();
    }
  }
  
  //Converting to OpenCV
  cv::Mat cvMatTesting(n_videos, dim, CV_32FC1);
  float fl_labels[n_videos];
  double acc =0;
  
  for (uword m=0; m<n_videos; ++m)
  {
    for (uword d=0; d<dim; ++d)
    {
      cvMatTesting.at<float>(m,d) = testing_data(d,m); 
      
    }
    fl_labels[m] = labels(m);
    
  }
  
  cv::Mat cvMatLabels(n_videos, 1, CV_32FC1,fl_labels );
  
  
  for (uword i=0; i< labels.n_elem; ++i)
  {
    cv::Mat sampleMat =   cvMatTesting.row(i);
    float response = SVM.predict(sampleMat);
    
    if (labels(i)  ==  response )
    {
      acc++;
      //cout << "Label is " << labels(i) <<" and it was classified as " << response << endl ;
      
    }
    else
    {
      //cout << "Label is " << labels(i) <<" and it was classified as " << response <<  ". Incorrect" << endl;

    }
    
  }
  
   cout << "Performance for scenario: " << sc << " is " << acc*100/n_videos << "%" << endl;

}


///****************************************************************
///****************Training and Testing all together***************
///****************************************************************

inline 
void
my_svm::train()
{
  
  //Using Means and Covs
  int dim = 7168; //OJO!!!!!!!!!
  int max_sc = 4;
  //Using Weights, Means and Covs.
  //int dim = 7423;
  
  int n_videos = peo_train.n_rows*actions.n_elem*max_sc;
  mat training_data(dim,n_videos);
  fvec labels = zeros<fvec>(n_videos);
  int k=0;
  
  //loading and creating training_data matrix and labels in Arma
  for (int sc=1; sc <=max_sc; ++sc)
  {
    for (uword pe = 0; pe< peo_train.n_rows; ++pe)
    {
      for (uword act = 0; act< actions.n_elem; ++act)
      {
	std::stringstream ssFV;
	ssFV << "./run" << run << "/features/fv_matlab_nor/FV_" << peo_train(pe)<< "_" << actions(act) << "_sc" << sc <<".txt";
	vec FV;
	FV.load( ssFV.str() );
	//cout << ssFV.str() << endl;
	training_data.col(k) = FV;
	labels(k) = act;
	k++;
	//FV.t().print("FV");
	//FV.save("FV_arma.dat", raw_ascii);
	//getchar();
	
      }
      
    }
    
  }
  
  
  //Converting to OpenCV
  cv::Mat cvMatTraining(n_videos, dim, CV_32FC1);
  float fl_labels[n_videos] ;
  
  for (uword m=0; m<n_videos; ++m)
  {
    for (uword d=0; d<dim; ++d)
    {
      cvMatTraining.at<float>(m,d) = training_data(d,m); 
      
    }
    fl_labels[m] = labels(m);
    
  }
  
  cv::Mat cvMatLabels(n_videos, 1, CV_32FC1,fl_labels );
  
  
  cout << "Setting parameters" << endl;
  CvSVMParams params;
  params.svm_type    = CvSVM::C_SVC;
  //params.kernel_type = CvSVM::RBF; 
  params.kernel_type = CvSVM::LINEAR; 
  //params.gamma = 1;
  params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER,  (int)1e7, 1e-6);
  
  
  // Train the SVM
  cout << "Training" << endl;
  CvSVM SVM;
  SVM.train( cvMatTraining , cvMatLabels, cv::Mat(), cv::Mat(), params);
  
  std::stringstream ssSVM;
  ssSVM<< "./run" << run << "/svm/svm_model_run" << run << "all_sc";
  //cout << ssSVM.str() << endl;
  SVM.save( ssSVM.str().c_str() );
  cout << "End of Training" << endl;
  
}

inline 
void
my_svm::test()
{
  
   //Using Means and Covs
  int dim = 7168; //OJO!!!!!!!!!
  int max_sc = 4;
  
  //Using Weights, Means and Covs.
  //int dim = 7423;
  
  int n_videos = peo_test.n_rows*actions.n_elem*max_sc;
  mat testing_data(dim,n_videos);
  fvec labels = zeros<fvec>(n_videos);
  int k=0;
  
  CvSVM SVM;
  std::stringstream ssSVM;
  ssSVM<< "./run" << run << "/svm/svm_model_run" << run << "all_sc";
  SVM.load( ssSVM.str().c_str() );

  //loading and creating testing_data matrix and labels in Arma
   for (int sc=1; sc <=max_sc; ++sc)
   {
     for (uword pe = 0; pe< peo_test.n_rows; ++pe)
     {
       for (uword act = 0; act< actions.n_elem; ++act)
       {
	 std::stringstream ssFV;
	 ssFV << "./run" << run << "/features/fv_matlab_nor/FV_" << peo_test(pe)<< "_" << actions(act) << "_sc" << sc <<".txt";
	 vec FV;
	 FV.load( ssFV.str() );
	 //cout << ssFV.str() << endl;
	 testing_data.col(k) = FV;
	 labels(k) = act;
	 k++;
	 //FV.t().print("FV");
	 //FV.save("FV_arma.dat", raw_ascii);
	 //getchar();
	 
      }
       
    }
     
  }
  
  //Converting to OpenCV
  cv::Mat cvMatTesting(n_videos, dim, CV_32FC1);
  float fl_labels[n_videos];
  double acc =0;
  
  for (uword m=0; m<n_videos; ++m)
  {
    for (uword d=0; d<dim; ++d)
    {
      cvMatTesting.at<float>(m,d) = testing_data(d,m); 
      
    }
    fl_labels[m] = labels(m);
    
  }
  
  cv::Mat cvMatLabels(n_videos, 1, CV_32FC1,fl_labels );
  
  
  for (uword i=0; i< labels.n_elem; ++i)
  {
    cv::Mat sampleMat =   cvMatTesting.row(i);
    float response = SVM.predict(sampleMat);
    
    if (labels(i)  ==  response )
    {
      acc++;
      //cout << "Label is " << labels(i) <<" and it was classified as " << response << endl ;
      
    }
    else
    {
      //cout << "Label is " << labels(i) <<" and it was classified as " << response <<  ". Incorrect" << endl;

    }
    
  }
  
   cout << "Performance: " << acc*100/n_videos << "%" << endl;

}