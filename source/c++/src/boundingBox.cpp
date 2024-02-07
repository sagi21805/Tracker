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

BoundingBox BoundingBox::combinedBoundingBox(BoundingBox& b){

    if (this->isEmpty()) { return b; }
    if (b.isEmpty()) {return *this; }
    int32_t x = MIN(rect.x, b.rect.x); 
    int32_t y = MIN(rect.y, b.rect.y); 
    int32_t x2 = MAX(rect.width + rect.x, b.rect.width + b.rect.x); 
    int32_t y2 = MAX(rect.height + rect.y, b.rect.height + b.rect.y); 
    cout << "MERGED\n\n\n\n\n";
    return BoundingBox(Rect(Point(x, y), Point(x2, y2)), b.type, (confidence + b.confidence) / 2); 
}

bool BoundingBox::isEmpty(){
    return this->rect.empty();
}
