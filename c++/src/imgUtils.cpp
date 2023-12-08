#include "imgUtils.hpp"

Mat filterRed(Mat& frame){

	Mat filtered;

	cv::GaussianBlur(frame, filtered, {5, 5}, 1, 1);
	cv::inRange(filtered, cv::Scalar(90, 45, 45), cv::Scalar(150, 255, 255), filtered);

	Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 9));

	cv::morphologyEx(filtered, filtered, cv::MORPH_OPEN, kernel);

	return filtered;

}

Mat filterBlue(Mat& frame){

	Mat filtered;

	cv::GaussianBlur(frame, filtered, {5, 5}, 1, 1);
	cv::inRange(filtered, cv::Scalar(130, 50, 50), cv::Scalar(190, 255, 255), filtered);

	Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 9));

	cv::morphologyEx(filtered, filtered, cv::MORPH_OPEN, kernel);
	

	return filtered;
}



//TODO make alg that cuts only the field from the video