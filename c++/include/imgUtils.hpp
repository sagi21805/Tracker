#ifndef _ImgUtils_
#define _ImgUtils_

#include "rect.hpp"

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

Mat fillterColor(Mat& frame, cv::Scalar lowerB, cv::Scalar upperB);

Mat convolutionThreshold(Mat& grayImg, uint8_t blockSize, float threshold);

uint8_t getDominantHueValue(Mat& image);

cv::Mat filterImageByColorRange(const cv::Mat& inputImage, const uint8_t dominantHue, int blockSize, double c);

void test(Mat& mat, vector<Mat> channels);
#endif
