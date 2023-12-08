#ifndef __MATH_UTILS__HPP_
#define __MATH_UTILS__HPP_

#include <cmath>
#include <opencv4/opencv2/opencv.hpp>


using std::vector;
typedef cv::Point_<uint16_t> Point;

/**
 * @file mathUtils.cpp
 * @brief Calculates the Squared Distance between two Points.
 * @returns Return the Squared distance between the two points.
 * @author Sagi Or
*/
uint squareDistance(Point A, Point B);

#endif