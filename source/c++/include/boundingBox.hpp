#ifndef _BoundingBox_
#define _BoundingBox_

#include "rect.hpp"
struct BoundingBox {
    
    Rect rect;
    uint16_t type;
    float32 confidence;
 
    BoundingBox(Rect rect, uint16_t type, float32 confidence): rect(rect), type(type), confidence(confidence) {}
    BoundingBox(const BoundingBox& box) : rect(box.rect), type(box.type), confidence(box.confidence) {}
    BoundingBox() : rect(Rect()), type(UINT16_MAX), confidence(0) {}

    bool isEmpty();

    BoundingBox combinedBoundingBox(BoundingBox& b);

};

vector<BoundingBox> splitByThreshold(vector<BoundingBox>& boundingBoxes, double threshold);

vector<BoundingBox> generateBoundingBoxes(int32_t* points, uint16_t* types, float32* confidences, uint16_t size);

#endif

