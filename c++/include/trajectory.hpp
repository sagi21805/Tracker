#ifndef _RECOGNITION_
#define _RECOGNITION_

#include "rect.hpp"

typedef cv::Point2i Velocity2D;

typedef struct Trajectory Trajectory;
struct Trajectory{
    const Rect rect;
    Velocity2D velocity;
    
    Trajectory(Rect rect, Velocity2D velocity): rect(rect), velocity(velocity) {}

    Trajectory() : rect(Rect()), velocity(Velocity2D()) {}
};

#endif
