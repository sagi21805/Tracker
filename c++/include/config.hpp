#ifndef _Config_
#define _Config_

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "vectorFuncs.hpp"
#include <opencv4/opencv2/opencv.hpp>
#include <stdexcept>  

using json = nlohmann::json;

// Declare global variables
namespace core{
    extern uint8_t  _pointSize;  
    extern uint16_t _startingId;
    extern uint8_t _velocityDeadBand;
} 

namespace visualization{
    extern bool _toVisualize;
    extern uint16_t _waitKey;
    extern vector<cv::Scalar_<uint8_t>> _colors;
}

namespace predictions{
    extern uint8_t _numOfFrames;
    extern float _offsetCoefficient;
    extern float _sizeCoefficient;
}

 

// Function to initialize global variables from JSON
void config(const std::string& filename);

vector<cv::Scalar_<uint8_t>> getRgbVector(const json& j, const std::string& key);

cv::Scalar_<uint8_t> getRgbArray(const json& array, const std::string& key);

#endif