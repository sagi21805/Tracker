#ifndef _BoundingBox_
#define _BoundingBox_

#include "rect.hpp"

struct BoundingBox {
    
    Rect rect;
    uint16_t type;
    float32 confidance;
 
    BoundingBox(Rect rect, uint16_t type, float32 confidance): rect(rect), type(type), confidance(confidance) {}
    BoundingBox(const BoundingBox& box) : rect(box.rect), type(box.type), confidance(box.confidance) {}
    BoundingBox() : rect(Rect()) {}
    BoundingBox& operator=(const BoundingBox& box){
    if (this != &box){
        rect = box.rect;
        type = box.type;
        confidance = box.confidance;
    }
    return *this;
}

    bool isEmpty(){
        return this->rect.empty();
    }

};

vector<BoundingBox> generateBoundingBoxes(uint16_t* points, uint16_t* types, float32* confidances, uint16_t size);
#endif

