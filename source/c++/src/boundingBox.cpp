#include "boundingBox.hpp"

vector<BoundingBox> generateBoundingBoxes(uint16_t* points, uint16_t* types, float32* confidances, uint16_t size) {
    vector<BoundingBox> boundingBoxes;
    boundingBoxes.reserve(size);
	for (uint16_t i = 0; i < size; i++){
        uint16_t* currentPoint = points + i*core::_elementsInPoint;
        uint16_t currentType = types[i];
        uint16_t currentConfidance = confidances[i];
        boundingBoxes.emplace_back(Rect(currentPoint), currentType, currentConfidance);
	}

    return boundingBoxes;
}
