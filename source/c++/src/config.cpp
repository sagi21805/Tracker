#include "config.hpp"

namespace core {
    uint8_t _elementsInPoint;
    size_t _startingId;
    size_t _velocityDeadBand;
    float32 _minScore;
    uint8_t _moveToLastSeen;
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
    if (config["elementsInPoint"].is_number_unsigned()) {
        core::_elementsInPoint = config["elementsInPoint"].get<uint8_t>();
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

    core::_minScore = config["minScore"];
    core::_moveToLastSeen = config["moveToLastSeen"];
    // Predictions

    if (config["predictions"]["sizeCoefficient"].is_number_float()) {
        predictions::_sizeCoefficient = config["predictions"]["sizeCoefficient"].get<float>();
    } else {
        throw std::runtime_error("Value for key 'sizeCoefficient' is not a number.");
    }

    if (config["predictions"]["smoothingFrames"].is_number_unsigned()) {
        predictions::_smoothingFrames = config["predictions"]["smoothingFrames"].get<uint8_t>();
    } else {
        throw std::runtime_error("Value for key 'smoothingFrames' is not an unsigned number.");
    }

    predictions::_velocityCoefficient = config["predictions"]["velocityCoefficient"];

    predictions::_offset = config["predictions"]["offset"];

    // Visualization
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

