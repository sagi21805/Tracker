#ifndef _ImgUtils
#define _ImgUtils

#include "opencvExtention.hpp"

using cv::Mat;

/**
 * @file imgUtils.cpp
 * @brief Filters FRC-RED color.  
 * @param frame The frame to filter.
 * @returns The filtered frame.
 * @author Sagi Or
*/
Mat filterRed(Mat& frame);

/**
 * @file imgUtils.cpp
 * @brief Filters FRC-BLUE.  
 * @param frame The frame to filter.
 * @returns The filtered frame.
 * @author Sagi Or
*/
Mat filterBlue(Mat& frame);

#endif
