#include "entity.hpp"


void Entity::calcAndSetVelocity(){
    uint8_t numOfFrames = 2; //TODO replace the magic number with a var name.
    if (this->trajectory->length >= numOfFrames){
        const Rect& startRect = this->trajectory->getItem(numOfFrames).rect; //the place before 
        const Rect& endRect = this->trajectory->getItem(0).rect; //current place
        Velocity2D rawVelocity((endRect.x - startRect.x) / numOfFrames, (endRect.y - startRect.y) / numOfFrames);
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
    Rect& currentRect = this->getBoundingRect();

    const uint16_t DB = core::_velocityDeadBand;
    const int& w = currentRect.width;
    const int& h = currentRect.height;
    const int& vX = abs(this->velocity.x*3);
    const int& vY = abs(this->velocity.y*3);
    const int offset = 10;

    cout << this->velocity << " " << this->getId() <<"\n";
    Rect possibleLocations = Rect(currentRect.tl() - Point(DB+vX+w, DB+vY+h), currentRect.tl() + Point(DB+vX+w+offset, DB+vY+h+offset));
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
