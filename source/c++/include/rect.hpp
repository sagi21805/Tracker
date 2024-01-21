#ifndef _OpencvExtention_
#define _OpencvExtention_

#include "mathUtils.hpp"

using cv::Mat;

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
    uint squareDistanceTo(const Rect& r);
    
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
    void draw(cv::Mat& frame, cv::Scalar color);
    // bool isIntersectingTo(Rect& r);
};

/**
 * @file opencvExtention.hpp
 * @brief Turns a stream of points of boundingBoxes from yolo into a vector of Rects.
 * @param points array of points in order of [x1, y1, x2, y2, type, x1 ....].
 * @param size How many points are in the array.
 * @returns A vector of Rects contains all the points as Rect object.
 * @author Sagi Or
*/
std::vector<Rect> pointsToRects(uint16_t *points, uint16_t size);

/**
 * @file opencvExtention.cpp
 * @brief Define the << operator to easly print Rects.
 * @param r Prints the rect [x, y, w, h] and its area.
 * @returns A Point of the First type that represents the difference in the Points.
 * @author Sagi Or
*/
std::ostream& operator<<(std::ostream& os, Rect& r);



#endif 