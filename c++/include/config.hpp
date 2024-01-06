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

namespace imgProcessing{
    extern float _convolutionThresh;
}

namespace kmeans{
    extern uint8_t _k;
    extern float _epsilon;
    extern uint8_t _maxIterPerAttempt;
    extern uint8_t _maxAttempts;
}
 

// Function to initialize global variables from JSON
void config(const std::string& filename);

vector<cv::Scalar_<uint8_t>> getRgbVector(const json& j, const std::string& key);

cv::Scalar_<uint8_t> getRgbArray(const json& array, const std::string& key);
// template <typename T>
// T checkType(const T& value, std::string key) {
//     if (std::is_same_v<T, decltype(value)>) {
//         return value;
//     } else {
//         throw std::runtime_error("Type mismatch in \"" + key + "\".");
//     }
// }
#endif