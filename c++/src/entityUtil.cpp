#include "entity.hpp"
using predictions::_offset, predictions::_numOfFrames;

void Entity::calcAndSetVelocity(){
    if (this->trajectory->length >= _numOfFrames){
        const Rect& startRect = this->trajectory->getItem(_numOfFrames).rect; //the place before 
        const Rect& endRect = this->trajectory->getItem(0).rect; //current place
        Velocity2D rawVelocity((endRect.x - startRect.x) / _numOfFrames, (endRect.y - startRect.y) / _numOfFrames);
        this->velocity.x = applyDeadband(rawVelocity.x, core::_velocityDeadBand);
        this->velocity.y = applyDeadband(rawVelocity.y, core::_velocityDeadBand);
    }
}

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
    Point tl = this->getBoundingRect().tl();

    const int w = this->getBoundingRect().width*predictions::_sizeCoefficient;
    const int h = this->getBoundingRect().height*predictions::_sizeCoefficient;
    const int vX = this->velocity.x*predictions::_velocityCoefficient;
    const int vY = this->velocity.y*predictions::_velocityCoefficient;
    Rect possibleLocations = Rect(tl + Point(vX, vY),  tl - Point(vX, vY));

    // if (vX > 0 && vY > 0){
    //     possibleLocations = Rect(tl, tl + Point(vX+w+_offset, vY+h+_offset));
    // }
    // if (vX < 0 && vY < 0){
    //     possibleLocations = Rect(tl, tl + Point(vX+w+_offset, vY+h+_offset));
    // }
    // if (vX < 0 && vY > 0){
    //     possibleLocations = Rect(tl - Point(vX+w, vY+h), tl + Point(vX+w+_offset, vY+h+_offset));
    // }
    // if (vY > 0 && vY < 0){
    //     possibleLocations = Rect(tl - Point(vX+w, vY+h), tl + Point(vX+w+_offset, vY+h+_offset));
    // }

    return possibleLocations;
}

Entity generateEntity(Rect r, uint16_t type){
	core::_startingId++;
	return Entity(core::_startingId, type, r);
}


void Entity::draw(cv::Mat& frame, cv::Scalar color){
		this->getBoundingRect().draw(frame, color);
		cv::putText(frame, std::to_string(this->id), this->boundingRect.tl(), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(255, 255, 0), 2);
}

std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingRect() << "\n";
    return os;
}
