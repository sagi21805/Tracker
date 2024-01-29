#ifndef _BoundingBox_
#define _BoundingBox_

#include "rect.hpp"

struct BoundingBox {
    
    Rect rect;
    uint16_t type;
    float32 confidance;
 
    BoundingBox(Rect rect, uint16_t type, float32 confidance): rect(rect), type(type), confidance(confidance) {}
    BoundingBox(const BoundingBox& box) : rect(box.rect), type(box.type), confidance(box.confidance) {}
    BoundingBox() : rect(Rect()), type(UINT16_MAX), confidance(0) {}

    bool isEmpty();

    bool isIntersecting(BoundingBox& b, float32 percentageAllowed);
};

extern "C" bool _isIntersecting(BoundingBox& b1, BoundingBox& b2, float32 percentageAllowed);

vector<BoundingBox> generateBoundingBoxes(int32_t* points, uint16_t* types, float32* confidances, uint16_t size);
#endif

