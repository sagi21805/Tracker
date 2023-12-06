#ifndef __BOUNDING_BOX_HPP
#define __BOUNDING_BOX_HPP

#include "mathUtils.hpp"
#include "vectorFuncs.hpp"

#define POINT_SIZE  (4)
#define POINTCLASS_SIZE (5)

class Rect : public cv::Rect_<uint16_t>{
    using Base = cv::Rect_<uint16_t>;
    using Base::Base;
    
    public: 
    uint squareDistanceTo(Rect& r);
    bool isCloseTo(Rect& r, double distance);
    void drawRect(cv::Mat& frame, cv::Scalar color);
    // bool isIntersectingTo(Rect& r);
};

Point operator + (const Point& A, const cv::Point2f& B);

Point operator - (const Point& A, const cv::Point2f& B);

std::ostream& operator<<(std::ostream& os, Rect& r);

std::vector<Rect> pointsToRects(uint16_t *pointsWithClass, uint16_t size);

#endif 