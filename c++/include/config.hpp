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
    extern uint8_t  _elementsInPoint;  
    extern size_t _startingId;
    extern size_t _velocityDeadBand;
} 

namespace visualization{
    extern bool _toVisualize;
    extern uint16_t _waitKey;
    extern vector<cv::Scalar_<uint8_t>> _colors;
}

namespace predictions{
    extern uint8_t _numOfFrames;
    extern float _sizeCoefficient;
    extern float _velocityCoefficient;
    extern uint16_t _offset;
}

 

// Function to initialize global variables from JSON
void config(const std::string& filename);

vector<cv::Scalar_<uint8_t>> getRgbVector(const json& j, const std::string& key);

cv::Scalar_<uint8_t> getRgbArray(const json& array, const std::string& key);

#endif