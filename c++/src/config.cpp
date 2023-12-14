#include "config.hpp"
// Define global variables
//TODO add type cheking on all of the config data. important!!!!

uint8_t _pointSize;  
bool _visualize;
uint16_t _waitKey;
uint8_t _numOfFrames;
float _predictionOffsetCoefficient;
float _predictionSizeCoefficient;
vector<cv::Scalar> _colors;
float _convolutionThresh;
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

     //REGULAR DATA

    _pointSize = config["pointSize"];
    _visualize = config["visualize"];
    _waitKey = config["waitKey"];
    _predictionOffsetCoefficient = config["predictionOffsetCoefficient"];
    _predictionSizeCoefficient = config["predictionSizeCoefficient"];
    _numOfFrames = config["numOfFrames"];
    _convolutionThresh = config["convolutionThresh"];
    //NESTED DATA;

    _colors = readNestedData<cv::Scalar>(config["colors"]);

    input_file.close();
}

