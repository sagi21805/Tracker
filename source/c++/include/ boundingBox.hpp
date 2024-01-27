#ifndef _BoundingBox_
#define _BoundingBox_

#include "rect.hpp"

class BoundingBox {

    private:
        Rect rect;
        uint16_t type;
        float32 confidance;

    public: 

        BoundingBox(Rect rect, uint16_t type, float32 confidance): rect(rect), type(type), confidance(confidance) {}
        BoundingBox() {}
};

#endif