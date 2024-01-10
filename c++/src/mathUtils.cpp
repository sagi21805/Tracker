#include "mathUtils.hpp"

uint squareDistance(Point A, Point B){
    return pow(A.x - B.x, 2) + pow(A.y - B.y, 2);
}
