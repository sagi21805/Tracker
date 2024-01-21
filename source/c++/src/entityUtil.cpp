#include "entity.hpp"
using predictions::_offset, predictions::_numOfFrames;

void Entity::calcAndSetVelocity(){
    if (this->trajectory.length >= _numOfFrames){
        const Point& startPoint = this->trajectory[_numOfFrames-1].rect.center; //the place before 
        const Point& endPoint = this->trajectory[0].rect.center; //current place
        Velocity2D rawVelocity((endPoint.x - startPoint.x) / _numOfFrames, (endPoint.y - startPoint.y) / _numOfFrames);
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

void Entity::predictPossibleLocations(){
    this->calcAndSetVelocity();
    Rect& r = this->getBoundingRect();
    const int vX = this->velocity.x*predictions::_velocityCoefficient;
    const int vY = this->velocity.y*predictions::_velocityCoefficient;
    const int w = r.width*predictions::_sizeCoefficient*signum(vX);
    const int h = r.height*predictions::_sizeCoefficient*signum(vY);
    this->possibleLocation = Rect(r.center + Point(vX+w, vY+h),  r.center - Point(w, h));
}

cv::Scalar Entity::chooseColor(){
	cv::Scalar color;
	switch (this->type){
			case RedRobot:
				color = visualization::_colors[RedRobot];
				break;
			case BlueRobot:
				color = visualization::_colors[BlueRobot];
				break;
			default:
				color = CV_RGB(0, 0, 0);
		}
	return color;
}

void Entity::draw(cv::Mat& frame){
	this->getBoundingRect().draw(frame, this->color);
	cv::putText(frame, std::to_string(this->id), this->boundingRect.tl(), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(255, 255, 0), 2);
}


std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingRect() << "\n";
    return os;
}