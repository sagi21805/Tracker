#include "entity.hpp"


void Entity::calcVelocity(){
    const Rect& startRect = this->trajectory->getItem(1).rect; //the place before that
    const Rect& endRect = this->trajectory->getItem(0).rect; //current place
    this->velocity = Velocity2D(endRect.x - startRect.x, endRect.y - startRect.y);
}

vector<Point> Entity::calcContour(Mat& frameInside){
    
    vector<Point> contours;
    Rect cutRect = Rect(this->boundingRect.tl(), this->boundingRect.size() - cv::Size_<uint16_t>(5, 5));
    Mat entityInFrame = frameInside(cutRect);
    uint8_t hue = getDominantHueValue(entityInFrame);
    Mat c = fillterColor(entityInFrame, cv::Scalar(hue-30, 0, 0), cv::Scalar(hue+30, 255, 255));
    std::vector<cv::Mat> channels = {c, c, c};
    test(entityInFrame, channels);

    return contours;
} //TODO make this function.

uint Entity::squareDistanceTo(Entity& e){
    return this->getBoundingRect().squareDistanceTo(e.getBoundingRect());
}

Rect Entity::predictNextBoundingRect(uint8_t numOfFrames){
    Velocity2D vel = this->calcVelocity(numOfFrames);
    Rect prediction = this->boundingRect;
    prediction.x += vel.x;
    prediction.y += vel.y;
    return prediction;
}

Rect Entity::predictPossibleLocations(uint8_t numOfFrames){
    this->calcVelocity(numOfFrames);
    Rect& currentRect = this->getBoundingRect();
    const int& W = currentRect.width;
    const int& H = currentRect.height;
    const float& K = predictions::_offsetCoefficient; 
    const float& J = predictions::_sizeCoefficient;
    Rect possibleLocations = Rect(currentRect.tl() - Point(K * W,  K * H), cv::Size2i(J * W, J * H));
    // if (this->velocity.x != 0 || this->velocity.y != 0){
    //     cout << this->id << "\n";
    //     uint16_t x = currentRect.tl().x + N * this->velocity.x;
    //     uint16_t y = currentRect.tl().y + N * this->velocity.y;
    //     Point A = Point(x, y);
    //     x = currentRect.tl().x - M * this->velocity.x;
    //     y = currentRect.tl().y - M * this->velocity.y;
    //     Point B = Point(x, y);
    //     cout << "tl: " << currentRect.tl() << " A: " << A << " B: " << B << " vel: " << this->velocity << "\n";
    //     possibleLocations = Rect(A, B);
    // }
    return possibleLocations;
}

uint16_t Entity::matchEntity(std::vector<Entity>& entityVector, uint8_t numOfFrames){

    // uint maxDistanceSquared = 500; //TODO MAKE A SOMEHOW CALCULATED ONE
    uint distanceSquared = UINT32_MAX;
    uint16_t idx = entityVector.size(); 

    Rect possibleLocations = this->predictPossibleLocations(numOfFrames);
    
    for (uint16_t i = 0, size = entityVector.size(); i < size; i++){
        Entity& checkedEntity = entityVector[i];

        if (this->getType() == checkedEntity.getType()) { 
            uint currentDistanceSquared = this->squareDistanceTo(checkedEntity);
            if (currentDistanceSquared < distanceSquared && 
                possibleLocations.contains(checkedEntity.getBoundingRect().tl())){
                idx = i;
                distanceSquared = currentDistanceSquared;
            }
        }       
    }
    return idx;
}

void Entity::draw(cv::Mat& frame, cv::Scalar color){
		this->getBoundingRect().drawRect(frame, color);
		cv::putText(frame, std::to_string(this->id), this->boundingRect.tl(), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(255, 255, 0), 2);
}

std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingRect() << "\n";
    return os;
}
