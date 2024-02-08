#include "rect.hpp"

using std::cout;

uint Rect::squareDistanceTo(const Rect& r) const {
    return squareDistance(this->center, r.center);
}

bool Rect::isCloseTo(Rect& r, double distance){
	return this->squareDistanceTo(r) <  distance;
}

void Rect::draw(cv::Mat& frame, cv::Scalar color){
    cv::rectangle(frame, *this, color, 2);
}

vector<Rect> pointsToRects(int32_t *points, uint16_t size){
	
	std::vector<Rect> rects;
    rects.reserve(size);
	for (uint16_t i = 0; i < size; i++){
        int32_t* currentPoint = points + i*core::_elementsInPoint;
        Point A = Point(currentPoint[0], currentPoint[1]);
        Point B = Point(currentPoint[2], currentPoint[3]);
        rects.emplace_back(A, B);
	}
    
	return rects;  

}

float32 Rect::iouPercentage(const Rect& other) const {
    int Xoverlap = MIN((this->x + this->width), (other.x + other.width)) - MAX(this->x, other.x);
    int Yoverlap = MIN((this->y + this->height), (other.y + other.height)) - MAX(this->y, other.y);
    if (Xoverlap <= 0.0 || Yoverlap <= 0.0) {
        return 0.0;
    }
    float32 intersectionArea = Xoverlap * Yoverlap;

    return intersectionArea / (float32) this->area();
}

std::ostream& operator<<(std::ostream& os, Rect& rect){
    // Printing all the elements
    os << "[" << rect.x << " " << rect.y << " " << rect.width << " " << rect.height << "]";
    os << "Area: " << rect.area() << "\n"; 
    return os;
}

