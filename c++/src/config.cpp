#include "config.hpp"

using json = nlohmann::json;

// Define global variables

uint8_t _pointSize;  
bool _visualize;
uint16_t _waitKey;

// Implementation of the initialization function
void config(const std::string& filename){
    // Open the JSON file
    std::ifstream input_file(filename);

    // Check if the file is open
    if (!input_file.is_open()) {
        // Handle error
        return;
    }

    // Parse the JSON content
    json config;
    input_file >> config;

    // Check if the key "globalString" exists and has a non-null value
    _pointSize = config["pointSize"];
    _visualize = config["visualize"];
    _waitKey = config["waitKey"];

    input_file.close();
}
