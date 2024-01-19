#ifndef _MathUtils_
#define _MathUtils_

#include <cmath>
#include <opencv4/opencv2/opencv.hpp>
#include "config.hpp"


using std::vector;
typedef cv::Point2i Point;

/**
 * @file mathUtils.cpp
 * @brief Calculates the Squared Distance between two Points.
 * @returns Return the Squared distance between the two points.
 * @author Sagi Or
*/
uint squareDistance(Point A, Point B);

//NOTICE IN THIS FUNCTION 0 COUNTS AS POSITIVE
//=
template <typename T> int signum(T val) {
    return (val >= T(0)) - (val < T(0));
}

template <typename T>
T applyDeadband(T value, uint8_t deadBand) {
    return (std::abs(value) > deadBand) ? value : 0;
} 
#endif