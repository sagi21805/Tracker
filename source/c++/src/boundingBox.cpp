#include "boundingBox.hpp"

vector<BoundingBox> generateBoundingBoxes(int32_t* points, uint16_t* types, float32* confidences, uint16_t size) {
    vector<BoundingBox> boundingBoxes;
    boundingBoxes.reserve(size);
	for (uint16_t i = 0; i < size; i++){
        int32_t* currentPoint = points + i*core::_elementsInPoint;
        uint16_t currentType = types[i];
        float32 currentconfidence = confidences[i];
        boundingBoxes.emplace_back(Rect(currentPoint), currentType, currentconfidence);
	}

    return boundingBoxes;
}

bool BoundingBox::isEmpty(){
    return this->rect.empty();
}
