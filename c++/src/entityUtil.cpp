#include "entity.hpp"


void Entity::calcAndSetVelocity(){
    if (this->trajectory->length > 3){
        uint8_t numOfFrames = 3;
        const Rect& startRect = this->trajectory->getItem(numOfFrames).rect; //the place before 
        const Rect& endRect = this->trajectory->getItem(0).rect; //current place
        Velocity2D rawVelocity((endRect.x - startRect.x) / numOfFrames, (endRect.y - startRect.y) / numOfFrames);
        this->velocity.x = applyDeadband(rawVelocity.x, core::_velocityDeadBand);
        this->velocity.y = applyDeadband(rawVelocity.y, core::_velocityDeadBand);
    }
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

uint Entity::squareDistanceTo(const Entity& e){
    return this->getBoundingRect().squareDistanceTo(e.getBoundingRect());
}

uint Entity::squareDistanceTo(const Rect& r){
    return this->getBoundingRect().squareDistanceTo(r);
}

Rect Entity::predictPossibleLocations(){
    Rect& currentRect = this->getBoundingRect();
    const int& W = currentRect.width;
    const int& H = currentRect.height;
    const float& K = predictions::_offsetCoefficient; 
    const float& J = predictions::_sizeCoefficient;
    // const float M = 3;
    Rect possibleLocations = Rect(currentRect.tl() - Point(K * W,  K * H), cv::Size2i(J * W, J * H));
    // if (this->velocity.x != 0 || this->velocity.y != 0){
    //     Point A = Point(currentRect.tl().x + M * this->velocity.x, currentRect.tl().y + M * this->velocity.y);
    //     Point B = Point(currentRect.tl().x - M * this->velocity.x, currentRect.tl().y - M * this->velocity.y);
    //     possibleLocations = Rect(A, B);
    // }
    return possibleLocations;
}

Entity generateEntity(Rect r, uint16_t type){
	core::_startingId++;
	return Entity(core::_startingId, type, r);
}

void matchEntity(vector<Entity>& currentEntities, Recognition& currentRecognition, Mat& frame){
    
    for (Rect& r : currentRecognition.rects){
        r.drawRect(frame, CV_RGB(0, 0, 0));
    }

    for (uint16_t j = 0, size = MIN(currentEntities.size(), currentRecognition.size); j < size; j++){
        Entity& currentEntity = currentEntities[j];
        uint distanceSquared = UINT32_MAX;
        currentEntity.calcAndSetVelocity();
        cout << "vel: " << currentEntity.getVelocity() << "\n";
        Rect possibleLocations = currentEntity.predictPossibleLocations();
        possibleLocations.drawRect(frame, CV_RGB(255, 255, 255));
    
        uint16_t matchingEntityIndex = NULL;

        for (uint16_t i = 0, size = currentRecognition.size; i < size; i++){
            const Rect& checkedRect = currentRecognition.rects[i];
            
            const uint16_t& checkedType = currentRecognition.types[i];
            if (currentEntity.getType() == checkedType) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(checkedRect);
                if (currentDistanceSquared < distanceSquared && 
                    possibleLocations.contains(checkedRect.tl())){
                    matchingEntityIndex = i;
                    distanceSquared = currentDistanceSquared;
                }
            }       
        }

        if (matchingEntityIndex != NULL){
            currentEntity.setBoundingRect(currentRecognition.rects[matchingEntityIndex]);
            currentRecognition.remove(matchingEntityIndex);
        }
        else {
            currentEntity.setBoundingRect(currentEntity.predictNextBoundingRect());
        }
    }

    // if (currentRecognition.size > 0){
    //     for (uint16_t i = 0; i < currentRecognition.size; i++){
    //         currentEntities.emplace_back(generateEntity(currentRecognition.rects[i], currentRecognition.types[i]));
    //     }
    //     currentRecognition.clear();
    // }
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
