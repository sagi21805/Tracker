#include "config.hpp"
// Define global variables

namespace core{
    uint8_t _pointSize;  
} 

namespace visualization{
    bool _toVisualize;
    uint16_t _waitKey;
    vector<cv::Scalar> _colors;
}

namespace predictions{
    uint8_t _numOfFrames;
    float _offsetCoefficient;
    float _sizeCoefficient;
}

namespace imgProcessing{
    float _convolutionThresh;
}

namespace kmeans{
    uint8_t _k;
    float _epsilon;
    uint8_t _maxIterPerAttempt;
    uint8_t _maxAttempts;
}
//TODO add type cheking on all of the config data. important!!!!
//the initialization function
void config(const std::string& filename){
    // Open the JSON file
    std::ifstream input_file(filename);

    // Check if the file is open
    if (!input_file.is_open()) {
        // Handle error
        return;
    }

    json config;
    // Parse the JSON content
    input_file >> config;

    //CORE DATA
    core::_pointSize = config["pointSize"];
    core::_startingId = config["startingId"];
    //Visualization
    predictions::_offsetCoefficient = config["predictions"]["offsetCoefficient"];
    predictions::_sizeCoefficient = config["predictions"]["sizeCoefficient"];
    predictions::_numOfFrames = config["predictions"]["numOfFrames"];

    //Predcitions
    visualization::_toVisualize = config["visualization"]["toVisualize"];
    visualization::_waitKey = config["visualization"]["waitKey"];
    visualization::_colors = readNestedData<cv::Scalar>(config["visualization"]["colors"]);
    //ImgProcessing
    imgProcessing::_convolutionThresh = config["imgProcessing"]["convolutionThresh"];

    //Kmeans
    kmeans::_k = config["kmeans"]["k"];
    kmeans::_epsilon = config["kmeans"]["epsilon"];
    kmeans::_maxIterPerAttempt = config["kmeans"]["maxIterPerAttempt"];
    kmeans::_maxAttempts = config["kmeans"]["maxAttemps"];

    cout << "[INFO]: Done initializing variables\n";

    input_file.close();
}

