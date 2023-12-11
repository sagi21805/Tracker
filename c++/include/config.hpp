#ifndef _Config_
#define _Config_

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

// Declare global variables
extern uint8_t _pointSize;  
extern bool _visualize;
extern uint16_t _waitKey;
// Function to initialize global variables from JSON
void config(const std::string& filename);

#endif