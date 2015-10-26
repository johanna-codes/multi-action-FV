class opt_feat
{
public:  
    inline opt_feat(const string  in_actionNames, 
		    const uword in_col, 
		    const uword in_row,
		    const int in_run,
		    const int in_dim
 		  );
    //actionNames, co, ro, run, 
    
    inline void features_training( field <string> peo_train, string single_path,  int sc );
    inline void features_allpeople ( field <string> peo_train, string single_path,  int sc );
    inline void feature_multi_action( field <string> peo_test, string multi_path );
    inline void feature_video(string one_video);

    
double THRESH_2;


const string actionNames;


const uword col;
const uword row;
const field <string> peo_test;
const int run;
const int dim;

int  N_cent;
bool ismultiAction;
rowvec  arma_multi_labels;
//field <mat> featuresframe_video_i; // Each mat has all the vector features of that frame
vector < vec > multi_features; //All features for one video will be saved here
vector < uword > frame_index;

//vector < vec > feat_video_i; not used
vector < vec > feat_all_videos_action_i;
vector < uword > lab_feature_vectors;
//vector < mat > diag_covs_all_videos;


field<string> actions;
field<string> videos;

 
  
};