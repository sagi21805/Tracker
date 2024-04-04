#include "boundingBox.hpp"

vector<BoundingBox> generateBoundingBoxes(int32_t* points, uint16_t* types, float32* confidences, uint16_t size) {
    vector<BoundingBox> boundingBoxes;
    boundingBoxes.reserve(size);
	for (uint16_t i = 0; i < size; i++){
        int32_t* currentPoint = points + i*core::_elementsInPoint;
        uint16_t currentType = types[i];
        float32 currentConfidence = confidences[i];
        if (currentConfidence > core::_minConfidence) {
            boundingBoxes.emplace_back(Rect(currentPoint), currentType, currentConfidence);
        }
	}

    return boundingBoxes;
}

BoundingBox BoundingBox::combineBoundingBox(const BoundingBox& b) const{
    if (this->isEmpty()) { return b; }
    if (b.isEmpty()) {return *this; }
    return BoundingBox(Rect(Point(MIN(rect.x, b.rect.x), MIN(rect.y, b.rect.y)), 
           Point(MAX(rect.width + rect.x, b.rect.width + b.rect.x), MAX(rect.height + rect.y, b.rect.height + b.rect.y))), 
           b.type, (confidence + b.confidence) / 2); 
}


BoundingBox BoundingBox::mergeBoundingBox(const BoundingBox& b) const{

    if (this->isEmpty()) { return b; }
    if (b.isEmpty()) {return *this; }
    int32_t x = (rect.x + b.rect.x) / 2; 
    int32_t y = (rect.y + b.rect.y) / 2; 
    int32_t w = (rect.width + b.rect.width) / 2; 
    int32_t h = (rect.height + b.rect.height) / 2; 
    return BoundingBox(Rect(x, y, w, h), b.type, (confidence + b.confidence) / 2); 
}

bool BoundingBox::isEmpty() const{
    return this->rect.empty();
}
