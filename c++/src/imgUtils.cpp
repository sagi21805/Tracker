#include "imgUtils.hpp"
#include <chrono>

Mat filterBlue(Mat& frame){

	Mat filtered = fillterColor(frame, cv::Scalar(90, 35, 35), cv::Scalar(150, 255, 255));

	return filtered;

}

Mat filterRed(Mat& frame){

	Mat filtered = fillterColor(frame, cv::Scalar(130, 30, 30), cv::Scalar(190, 255, 255));

	return filtered;
}

Mat fillterColor(Mat& frame, cv::Scalar lowerB, cv::Scalar upperB){
    Mat filtered;
    cv::cvtColor(frame, filtered, cv::COLOR_BGR2HSV);
	cv::GaussianBlur(filtered, filtered, {5, 5}, 1, 1);
	cv::inRange(filtered, lowerB, upperB, filtered);

	Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 9));

	cv::morphologyEx(filtered, filtered, cv::MORPH_OPEN, kernel);
	

	return filtered;
}

Mat convolutionThreshold(Mat& grayImg, uint8_t blockSize, float threshold){
    auto start_time = std::chrono::high_resolution_clock::now();
    const uint16_t rows = grayImg.rows; // y axis
    const uint16_t cols = grayImg.cols; // x axis
    std::cout << cols << " " << rows << "\n";
    int thresh = 255*blockSize*blockSize*threshold;
    Mat newImg = Mat::zeros(cv::Size(cols, rows), CV_8UC1);
    for (uint16_t row = 0, endRow = rows - blockSize; row < endRow; ++row){
        for (uint16_t col = 0, endCol = cols - blockSize; col < endCol; ++col){
            cv::Rect_<uint16_t> wantedArea(col, row, blockSize, blockSize);
            Mat block = grayImg(wantedArea);
            if (cv::sum(block)[0] > thresh){
                newImg.at<uint8_t>(row, col) = 255;
            }
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Elapsed time: " << duration.count() << " milliseconds" << std::endl;
    return newImg;
}
