#ifndef _Config_
#define _Config_

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "vectorFuncs.hpp"
#include <opencv4/opencv2/opencv.hpp>


using json = nlohmann::json;

// Declare global variables
extern uint8_t _pointSize;  
extern bool _visualize;
extern uint16_t _waitKey;
extern uint8_t _numOfFrames;
extern float _predictionOffsetCoefficient;
extern float _predictionSizeCoefficient;
extern vector<cv::Scalar> _colors;
extern float _convolutionThresh;
// Function to initialize global variables from JSON
void config(const std::string& filename);

//the nested data must be the same type. 
template<typename T> vector<T> readNestedData(json nestedData){

    vector<T> outVector;
    cout << "size: " << nestedData.size() << "\n";
    outVector.reserve(nestedData.size());

    // add type checking on the value.
    for (auto it = nestedData.begin(); it != nestedData.end(); ++it) {
        const json& value = it.value();
        cv::Scalar color = CV_RGB(value[0], value[1], value[2]);
        outVector.push_back(color);
    }

    return outVector;
}

#endif