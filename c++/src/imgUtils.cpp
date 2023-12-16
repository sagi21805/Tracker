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

Mat getImageChannel(const Mat& image, uint8_t channelNum){
    std::vector<cv::Mat> channels; //H, S, V
    cv::split(image, channels);
    return channels[channelNum];
}

//TODO understand this algorithem.
uint8_t getDominantHueValue(Mat& image){
    cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
    Mat hue = getImageChannel(image, 0); // the Hue channel
    hue = hue.reshape(1, hue.total());
    // Convert to float32 for k-means
    hue.convertTo(hue, CV_32F);

    // Define criteria and apply kmeans()
    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, kmeans::_maxIterPerAttempt, kmeans::_epsilon);
    cv::Mat labels, centers;
    cv::kmeans(hue, kmeans::_k, labels, criteria, kmeans::_maxAttempts, cv::KMEANS_PP_CENTERS, centers);

    // Convert back to 8-bit values
    centers.convertTo(centers, CV_8U);

    return centers.at<uint8_t>(findMostOccurrence(centers, kmeans::_k));
}

cv::Mat filterImageByColorRange(const cv::Mat& inputImage, const uint8_t dominantHue, int blockSize, double c){
    // Convert the input image from BGR to HSV
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    // Compute lower and upper bounds based on the dominant color
    cv::Vec3b lowerBound(static_cast<uchar>(dominantHue - 20), 50, 50);
    cv::Vec3b upperBound(static_cast<uchar>(dominantHue + 20), 255, 255);

    // Create a binary mask based on the specified HSV range
    cv::Mat mask;
    cv::inRange(hsvImage, lowerBound, upperBound, mask);

    // Apply adaptive thresholding to the mask
    cv::Mat adaptiveThresholdMask;
    cv::adaptiveThreshold(mask, adaptiveThresholdMask, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blockSize, c);

    // Apply the adaptive threshold mask to the input image
    cv::Mat filteredImage;
    cv::bitwise_and(inputImage, inputImage, filteredImage, adaptiveThresholdMask);

    return filteredImage;
}
//TODO make alg that cuts only the field from the video