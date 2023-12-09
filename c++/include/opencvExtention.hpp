#ifndef _OpencvExtention_
#define _OpenCvExtension_

#include "mathUtils.hpp"
#include "vectorFuncs.hpp"

#define POINT_SIZE  (4)
#define POINTCLASS_SIZE (5)

/**
 * @file opencvExtention.hpp
 * @brief This is an extension to the Rect opencv class which allows more functionallity.
 * @author Sagi Or
*/
class Rect : public cv::Rect_<uint16_t>{
    using Base = cv::Rect_<uint16_t>;
    using Base::Base;
    
    public:

/////////////////////////////////////////////////////// Util Functions ////////////////////////////////////////////////////////////////////////

    /**
     * @file opencvExtention.cpp
     * @brief Claculates The Square Distance from this TL corner to a given rectangle TL cornrer.
     * @param r Rectangle reference to calculate the distance to
     * @returns The Squared distance.
     * @author Sagi Or
    */
    uint squareDistanceTo(Rect& r);
    
    /**
     * @file opencvExtention.cpp
     * @brief Checks if a rectangle is at a given distance to This rectangle.
     * @param r Rectangle reference to check.
     * @param distance The distance to check on.
     * @returns True if in distance False if not.
     * @author Sagi Or
    */
    bool isCloseTo(Rect& r, double distance);

    /**
     * @file opencvExtention.cpp
     * @brief Draws the rectangle on a frame.
     * @param frame The frame to draw on.
     * @param color The color to draw the Rect.
     * @returns True if in distance False if not.
     * @author Sagi Or
    */
    void drawRect(cv::Mat& frame, cv::Scalar color);
    // bool isIntersectingTo(Rect& r);
};

/**
 * @file opencvExtention.cpp
 * @brief Define the << operator to easly print Rects.
 * @param r Prints the rect [x, y, w, h] and its area.
 * @returns A Point of the First type that represents the difference in the Points.
 * @author Sagi Or
*/
std::ostream& operator<<(std::ostream& os, Rect& r);

/**
 * @file opencvExtention.hpp
 * @brief Define method to easly add Points.
 * @param a The Point to add to.
 * @param b The Point that is being added.
 * @returns A Point of the First type that represents the sum of the Points.
 * @author Sagi Or
*/
template<typename Ty1_, typename Ty2_> Ty1_ addPoints(const cv::Point_<Ty1_>& a, const cv::Point_<Ty2_>& b){
    return Point(cv::saturate_cast<Ty1_>(a.x + b.x), cv::saturate_cast<Ty1_>(a.y + b.y));
}

/**
 * @file opencvExtention.hpp
 * @brief Define the Plus operator to do the method addPoints.
 * @param a The Point to add to.
 * @param b The Point that is being added.
 * @returns A Point of the First type that represents the sum of the Points.
 * @author Sagi Or
*/
template<typename Ty1_, typename Ty2_> Ty1_ operator+(const cv::Point_<Ty1_>& a, const cv::Point_<Ty2_>& b){
    return addPoints(a, b);
}

/**
 * @file opencvExtention.hpp
 * @brief Define method to easly subtrackt Points.
 * @param a The Point to substract from.
 * @param b The Points the is being substracted.
 * @returns A Point of the First type that represents the difference in the Points.
 * @author Sagi Or
*/
template<typename Ty1_, typename Ty2_> Ty1_ substractPoints(const cv::Point_<Ty1_>& a, const cv::Point_<Ty2_>& b){
    return Point(cv::saturate_cast<Ty1_>(a.x - b.x), cv::saturate_cast<Ty1_>(a.y - b.y));
}

/**
 * @file opencvExtention.hpp
 * @brief Define method operator to easly subtract Points.
 * @param a The Point to substract from.
 * @param b The Points the is being substracted.
 * @returns A Point of the First type that represents the difference in the Points.
 * @author Sagi Or
*/
template<typename Ty1_, typename Ty2_> Ty1_ operator-(const cv::Point_<Ty1_>& a, const cv::Point_<Ty2_>& b){
    return Point(cv::saturate_cast<Ty1_>(a.x + b.x), cv::saturate_cast<Ty1_>(a.y + b.y));
}

/**
 * @file opencvExtention.hpp
 * @brief Turns a stream of points of boundingBoxes from yolo into a vector of Rects.
 * @param points array of points in order of [x1, y1, x2, y2, type, x1 ....].
 * @param size How many points are in the array.
 * @returns A vector of Rects contains all the points as Rect object.
 * @author Sagi Or
*/
std::vector<Rect> pointsToRects(uint16_t *points, uint16_t size);

#endif 