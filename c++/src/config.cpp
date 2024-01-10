#include "config.hpp"

namespace core {
    uint8_t _pointSize;
    uint16_t _startingId;
    uint8_t _velocityDeadBand;
}

namespace visualization {
    bool _toVisualize;
    uint16_t _waitKey;
    std::vector<cv::Scalar_<uint8_t>> _colors;
}

namespace predictions {
    uint8_t _numOfFrames;
    float _offsetCoefficient;
    float _sizeCoefficient;
}

namespace imgProcessing {
    float _convolutionThresh;
}

namespace kmeans {
    uint8_t _k;
    float _epsilon;
    uint8_t _maxIterPerAttempt;
    uint8_t _maxAttempts;
}

cv::Scalar_<uint8_t> getRgbArray(const json& array, const std::string& key) {
    if (array.size() == 3) {
        for (uint8_t i = 0; i < 3; ++i){
            if (array[i].is_number_unsigned() && array[i] < 255) {}
            else { std::runtime_error("Invalid RGB array format in key \"" + key + "\". Values are too big."); }
        }
        return cv::Scalar_<uint8_t>(
            array[2],
            array[1],
           array[0]
        );
    } else {
        throw std::runtime_error("Invalid RGB array format in key \"" + key + "\". The Array is too long.");
    }
}

vector<cv::Scalar_<uint8_t>> getRgbVector(const json& j, const std::string& key) {
    std::vector<cv::Scalar_<uint8_t>> rgbVector;
    rgbVector.reserve(j.size());
    for (auto it = j.begin(); it != j.end(); ++it) {
        std::string key = it.key();
        if (j.find(key) != j.end() && j[key].is_array()) {
            rgbVector.push_back(getRgbArray(j[key], key));
        } else {
            throw std::runtime_error("Key '" + key + "' not found or not an array in the JSON object.");
        }
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
    if (config["pointSize"].is_number_unsigned()) {
        core::_pointSize = config["pointSize"].get<uint8_t>();
    } else {
        throw std::runtime_error("Value for key 'pointSize' is not an unsigned number.");
    }

    if (config["startingId"].is_number_unsigned()) {
        core::_startingId = config["startingId"].get<uint16_t>();
    } else {
        throw std::runtime_error("Value for key 'startingId' is not an unsigned number.");
    }

    if (config["velocityDeadBand"].is_number_unsigned()) {
        core::_velocityDeadBand = config["velocityDeadBand"].get<uint8_t>();
    } else {
        throw std::runtime_error("Value for key 'velocityDeadBand' is not an unsigned number.");
    }

    // Visualization
    if (config["predictions"]["offsetCoefficient"].is_number_float()) {
        predictions::_offsetCoefficient = config["predictions"]["offsetCoefficient"].get<float>();
    } else {
        throw std::runtime_error("Value for key 'offsetCoefficient' is not a number.");
    }

    if (config["predictions"]["sizeCoefficient"].is_number_float()) {
        predictions::_sizeCoefficient = config["predictions"]["sizeCoefficient"].get<float>();
    } else {
        throw std::runtime_error("Value for key 'sizeCoefficient' is not a number.");
    }

    if (config["predictions"]["numOfFrames"].is_number_unsigned()) {
        predictions::_numOfFrames = config["predictions"]["numOfFrames"].get<uint8_t>();
    } else {
        throw std::runtime_error("Value for key 'numOfFrames' is not an unsigned number.");
    }

    // Predictions
    if (config["visualization"]["toVisualize"].is_boolean()) {
        visualization::_toVisualize = config["visualization"]["toVisualize"].get<bool>();
    } else {
        throw std::runtime_error("Value for key 'toVisualize' is not a boolean.");
    }

    if (config["visualization"]["waitKey"].is_number_unsigned()) {
        visualization::_waitKey = config["visualization"]["waitKey"].get<uint16_t>();
    } else {
        throw std::runtime_error("Value for key 'waitKey' is not an unsigned number.");
    }

    visualization::_colors = getRgbVector(config["visualization"]["colors"], "colors");


    std::cout << "[INFO]: Done initializing variables\n";

    input_file.close();
}

