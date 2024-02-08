#include "tracker.hpp"

extern "C"{

    Tracker* _Tracker(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols){
        return new Tracker(points, types, confidences, size, frame, rows, cols);
    }

    void _track(Tracker *t, int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){
        t -> track(points, types, confidences, size, frame);
    }

}