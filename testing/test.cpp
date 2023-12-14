#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include "imgUtils.hpp"

using cv::VideoCapture, cv::Mat, std::cout;

int main(){

    VideoCapture cap("/home/sagi21805/dcmp4.mp4");
    cout << cap.isOpened() << "\n";
    while (1){
        Mat frame;
        cap.read(frame);
        Mat x =  convolutionThreshold(frame, 5, 0.4);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(x, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
        for (uint i = 0; i < contours.size(); ++i){
            cv::drawContours(frame, contours, i, cv::Scalar(0, 255, 255));
        }
        cv::imshow("red", x);
        cv::imshow("name", frame);
        cv::waitKey(1);
    }

    return 0;
}