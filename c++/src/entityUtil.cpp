#include "entity.hpp"


void Entity::calcAndSetVelocity(){
    const Rect& startRect = this->trajectory->getItem(1).rect; //the place before that
    const Rect& endRect = this->trajectory->getItem(0).rect; //current place
    this->velocity =  Velocity2D(endRect.x - startRect.x, endRect.y - startRect.y);
}

// vector<Point> Entity::calcContour(Mat& frameInside){
    
//     vector<Point> contours;
//     Rect cutRect = Rect(this->boundingRect.tl(), this->boundingRect.size() - cv::Size_<uint16_t>(5, 5));
//     Mat entityInFrame = frameInside(cutRect);
//     uint8_t hue = getDominantHueValue(entityInFrame);
//     Mat c = fillterColor(entityInFrame, cv::Scalar(hue-30, 0, 0), cv::Scalar(hue+30, 255, 255));
//     std::vector<cv::Mat> channels = {c, c, c};
//     test(entityInFrame, channels);

//     return contours;
// } //TODO make this function.

Rect Entity::predictNextBoundingRect(){
    Rect prediction = this->boundingRect;
    prediction.x += this->velocity.x;
    prediction.y += this->velocity.y;
    return prediction;
}

uint Entity::squareDistanceTo(Entity& e){
    return this->getBoundingRect().squareDistanceTo(e.getBoundingRect());
}

Rect Entity::predictPossibleLocations(){
    Rect& currentRect = this->getBoundingRect();
    const int& W = currentRect.width;
    const int& H = currentRect.height;
    const float& K = predictions::_offsetCoefficient; 
    const float& J = predictions::_sizeCoefficient;
    const float M = 2;
    Rect possibleLocations = Rect(currentRect.tl() - Point(K * W,  K * H), cv::Size2i(J * W, J * H));
    if (this->velocity.x != 0 || this->velocity.y != 0){
        uint16_t x = currentRect.tl().x + M * this->velocity.x;
        uint16_t y = currentRect.tl().y + M * this->velocity.y;
        Point A = Point(x, y);
        x = currentRect.tl().x - M * this->velocity.x;
        y = currentRect.tl().y - M * this->velocity.y;
        Point B = Point(x, y);
        cout << "tl: " << currentRect.tl() << " A: " << A << " B: " << B << " vel: " << this->velocity << "\n";
        possibleLocations = Rect(A, B);
    }
    return possibleLocations;
}

void matchEntity(vector<Entity>& currentEntities, vector<Entity>& currentRecognition){

    for (uint16_t j = 0, size = MIN(currentEntities.size(), currentRecognition.size()); j < size; j++){
        Entity& currentEntity = currentEntities[j];
        uint distanceSquared = UINT32_MAX;
        Rect possibleLocations = currentEntity.predictPossibleLocations();
        uint16_t matchingEntityIndex = NULL;
        for (uint16_t i = 0, size = currentRecognition.size(); i < size; i++){
            Entity& checkedEntity = currentRecognition[i];

            if (currentEntity.getType() == checkedEntity.getType()) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(checkedEntity);
                if (currentDistanceSquared < distanceSquared && 
                    possibleLocations.contains(checkedEntity.getBoundingRect().tl())){
                    matchingEntityIndex = i;
                    distanceSquared = currentDistanceSquared;
                }
            }       
        }

        if (matchingEntityIndex != NULL){
            currentEntities[j].setBoundingRect(currentRecognition[matchingEntityIndex].getBoundingRect());
            removeAtIndex(currentRecognition, matchingEntityIndex);
        }
    }
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
