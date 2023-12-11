#include "tracker.hpp"

extern "C"{

    Tracker* _Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols){
        return new Tracker(points, types, size, frame, rows, cols);
    }

    void _track(Tracker *t, uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame){
        t -> track(points, types, size, frame);
    }

}