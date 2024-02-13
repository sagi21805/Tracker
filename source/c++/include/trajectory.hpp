#ifndef _RECOGNITION_
#define _RECOGNITION_

#include "boundingBox.hpp"

typedef cv::Point2i Velocity2D;

typedef struct Trajectory Trajectory;
struct Trajectory{
    const BoundingBox box;
    Velocity2D velocity;
    
    Trajectory(BoundingBox box, Velocity2D velocity): box(box), velocity(velocity) {}

    Trajectory() : box(BoundingBox()), velocity(Velocity2D()) {}
};

#endif
