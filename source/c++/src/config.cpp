#include "config.hpp"

namespace core {
    uint8_t _elementsInPoint;
    size_t _startingId;
    size_t _velocityDeadBand;
    float32 _minScore;
    uint8_t _moveToLastSeen;
    float32 _minConfidence;
}

namespace visualization {
    bool _toVisualize;
    uint16_t _waitKey;
    std::vector<cv::Scalar_<uint8_t>> _colors;
}

namespace predictions {
    uint8_t _smoothingFrames;
    float _sizeCoefficient;
    float _velocityCoefficient;
    uint16_t _offset;
}


cv::Scalar_<uint8_t> getRgbArray(const json& array, const std::string& key) {
    if (array.size() == 3) {
        for (uint8_t i = 0; i < 3; ++i){
            if (array[i].is_number_unsigned() && array[i] <= 255) {}
            else { throw error("Invalid RGB array format in key \"" + key + "\". Values are too big."); }
        }
        return cv::Scalar_<uint8_t>(
            array[2],
            array[1],
           array[0]
        );
    } else {
        throw error("Invalid RGB array format in key \"" + key + "\". The Array is too long.");
    }
}

vector<cv::Scalar_<uint8_t>> getRgbVector(const json& j, const std::string& key) {
    std::vector<cv::Scalar_<uint8_t>> rgbVector;
    rgbVector.reserve(j.size());
    for (auto it = j.begin(); it != j.end(); ++it) {
        std::string key = it.key();
        if (j.find(key) != j.end() && j[key].is_array()) { rgbVector.push_back(getRgbArray(j[key], key)); } 
        else {throw error("Key '" + key + "' not found or not an array in the JSON object.");}
    }
    return rgbVector;
}

void config(const std::string& filename) {
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        return;
    }
    json config;
    input_file >> config;
    
    // CORE DATA
    if (config["elementsInPoint"].is_number_unsigned()) {core::_elementsInPoint = config["elementsInPoint"].get<uint8_t>();} 
    else { throw error("'pointSize' is not an unsigned number."); }

    if (config["startingId"].is_number_unsigned()) {core::_startingId = config["startingId"].get<uint16_t>();} 
    else { throw error("'startingId' is not an unsigned number.");}

    if (config["velocityDeadBand"].is_number_unsigned()) 
    {core::_velocityDeadBand = config["velocityDeadBand"].get<uint8_t>();}
    else { throw error("'velocityDeadBand' is not an unsigned number.");}

    if (config["minScore"].is_number_float() && config["minScore"] <= 1.0) {core::_minScore = config["minScore"];}
    else { throw error("'minScore' is not float OR greater then 1");}

    if (config["moveToLastSeen"].is_number_unsigned()) {core::_moveToLastSeen = config["moveToLastSeen"];}
    else { throw error("'moveToLastSeen' is not an unsigned number");}

    if (config["minConfidence"].is_number_float() && config["minConfidence"] < 1.0) 
    { core::_minConfidence = config["minConfidence"];}
    else { throw error("'minConfidence' is not an unsigned number OR greater then 1");}
    
    // Predictions
    if (config["predictions"]["sizeCoefficient"].is_number_float() && config["predictions"]["sizeCoefficient"] > 0) 
    { predictions::_sizeCoefficient = config["predictions"]["sizeCoefficient"].get<float>();} 
    else { throw error("'sizeCoefficient' is not a float.");}

    predictions::_smoothingFrames = 2;

    if (config["predictions"]["velocityCoefficient"].is_number_float() && config["predictions"]["velocityCoefficient"] > 0)
    { predictions::_velocityCoefficient = config["predictions"]["velocityCoefficient"].get<float32>();}
    else { throw error("'velocityCoefficient' is not a float.");}
    
    // Visualization
    if (config["visualization"]["toVisualize"].is_boolean()) 
    { visualization::_toVisualize = config["visualization"]["toVisualize"].get<bool>(); } 
    else { throw error("'toVisualize' is not a boolean."); }

    if (config["visualization"]["waitKey"].is_number_unsigned()) 
    { visualization::_waitKey = config["visualization"]["waitKey"].get<uint16_t>();}
    else { throw error("'waitKey' is not an unsigned number."); }

    visualization::_colors = getRgbVector(config["visualization"]["colors"], "colors");

    std::cout << "[INFO]: Done initializing variables\n";

    input_file.close();
}

