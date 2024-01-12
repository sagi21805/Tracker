#ifndef _Recognition_
#define _Recognition_

#include "vectorFuncs.hpp"
#include "rect.hpp"

typedef struct Recognition Recognition;

struct Recognition{
    vector<Rect> rects;
    vector<uint16_t> types;
    uint16_t size;

    Recognition(vector<Rect> rects, uint16_t* types) 
        : rects(rects), types(vector<uint16_t>(types, types + rects.size())), size(rects.size()) {}

    Recognition(uint16_t* points, uint16_t size, uint16_t* types)
        : rects(pointsToRects(points, size)), types(vector<uint16_t>(types, types + size)), size(size) {}

    Recognition() : rects(vector<Rect>()), types(vector<uint16_t>()), size(0) {}

    Recognition(const Recognition & rec) : rects(rec.rects), types(rec.types), size(rec.size){}

    Recognition& operator=(const Recognition & rec){
        if (this != &rec){
            rects = rec.rects;
            types = rec.types;
            size = rec.size;
        }
        return *this;
    }

    void remove(uint16_t __index){
        removeAtIndex(rects, __index);
        removeAtIndex(types, __index);
        size--;
    }

    void clear(){
        rects.clear();
        types.clear();
        size = 0;
    }
};


#endif