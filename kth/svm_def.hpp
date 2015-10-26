class my_svm
{
public:  
    inline my_svm(const string  in_actionNames,
		  const field <string> in_peo_train,
		  const field <string> in_peo_test,
		  const int in_run	);
    
    
   inline void train(int sc);   
   inline void train(); //all scenarios
   inline void test(int sc);
   inline void test();//all scenarios
    
   private:
     const string  actionNames;
     const field <string> peo_train;
     const field <string> peo_test;
     const int run;
     
     field<string> actions;
  
};