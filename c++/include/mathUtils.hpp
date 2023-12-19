#ifndef _MathUtils_
#define _MathUtils_

#include <cmath>
#include <opencv4/opencv2/opencv.hpp>
#include "config.hpp"


using std::vector;
typedef cv::Point_<uint16_t> Point;

/**
 * @file mathUtils.cpp
 * @brief Calculates the Squared Distance between two Points.
 * @returns Return the Squared distance between the two points.
 * @author Sagi Or
*/
uint squareDistance(Point A, Point B);

uint16_t findMostOccurrence(const cv::Mat& labels, uint8_t k);

template <typename T>
T applyDeadband(T value, uint8_t deadBand) {
    return (std::abs(value) < deadBand) ? T(0) : value;
}
#endif