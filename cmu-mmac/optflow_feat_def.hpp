class opt_feat
{
public:  
    inline opt_feat(const std::string  in_path, 
		    const std::string  in_actionNames, 
		    const uword in_col, 
		    const uword in_row
 		  );
    
    inline void features_per_action_training( field<string>peo_train );
    inline void create_universal_gmm( field<string>peo_train, int in_Ncent,  int run );
    
    inline void feature_multi_action( field<string> peo_test );
    inline void feature_video( std::string one_video, std::string ini_end_frames );


    

const std::string path;
const std::string actionNames;

double THRESH_2;
const uword col;
const uword row;
//const field<string>peo_train;
//const field<string> peo_test;
//const int run;
int dim;

int  N_cent;
bool ismultiAction;
vec  arma_multi_labels;
vector < uword > lab_feature_vectors;

vector<vector<vec> > all_actions_matrix; //the feat_vector of action i are stored in row i
//field <mat> featuresframe_video_i; // Each mat has all the vector features of that frame
vector < vec > multi_features; //All features for one video will be saved here
vector < uword > frame_index;



field<std::string> actions;
field<std::string> videos;


  private:
    inline void features_per_action();

 
  
};