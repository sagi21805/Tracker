#include "entity.hpp"

using predictions::_offset, predictions::_smoothingFrames;

void Entity::calcAndSetVelocity(){
    if (this->trajectory.length >= _smoothingFrames){
        const Point& startPoint = this->trajectory[_smoothingFrames-1].box.rect.center; //the place before 
        const Point& endPoint = this->trajectory[0].box.rect.center; //current place
        Velocity2D rawVelocity((endPoint.x - startPoint.x) / _smoothingFrames, (endPoint.y - startPoint.y) / _smoothingFrames);
        this->velocity.x = applyDeadband(rawVelocity.x, core::_velocityDeadBand);
        this->velocity.y = applyDeadband(rawVelocity.y, core::_velocityDeadBand);
    }
}

void Entity::predictNextBoundingBox(){
    this->boundingBox.rect.x += this->velocity.x;
    this->boundingBox.rect.y += this->velocity.y;
}

uint Entity::squareDistanceTo(const Entity& e){
    return this->getBoundingBox().rect.squareDistanceTo(e.getBoundingBox().rect);
}

uint Entity::squareDistanceTo(const Rect& r){
    return this->getBoundingBox().rect.squareDistanceTo(r);
}

void Entity::predictPossibleLocations(){
    Rect& r = this->getBoundingBox().rect;
    const int vX = this->velocity.x*predictions::_velocityCoefficient;
    const int vY = this->velocity.y*predictions::_velocityCoefficient;
    const int w = r.width*predictions::_sizeCoefficient*signum(vX)*(timesNotFound+5)/5;
    const int h = r.height*predictions::_sizeCoefficient*signum(vY)*(timesNotFound+5)/5;
    this->possibleLocation = Rect(r.center + Point(vX+w, vY+h),  r.center - Point(w, h));
}

cv::Scalar Entity::chooseColor(){
	cv::Scalar color;
	switch (this->boundingBox.type){
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
	this->getBoundingBox().rect.draw(frame, this->color);
	cv::putText(frame, std::to_string(this->id), this->boundingBox.rect.tl(), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(255, 255, 0), 2);
    cv::putText(frame, std::format("{:.2f}", this->boundingBox.confidence), this->boundingBox.rect.center - Point(24, -12), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(0, 255, 255), 2);
}

void Entity::combineBoundingBox(BoundingBox& b){
    this->boundingBox = this->boundingBox.mergeBoundingBox(b);
}

float32 Entity::calcScore(const BoundingBox& matchedPrediction){ // TODO SWITCH MAGIC NUMBERS
	// parameters should be built such that only one cant carry the score alone.
    if (this->boundingBox.rect.empty()) { return 0.0; }
	if (this->boundingBox.type != matchedPrediction.type) { return 0.0; }
	float32 score = 0.0;
    score += this->getPossibleLocation().contains(matchedPrediction.rect.center) ? 0.15 : 0;
    score += this->getPossibleLocation().contains(matchedPrediction.rect.tl()) ? 0.1 : 0;
    score += this->getPossibleLocation().contains(matchedPrediction.rect.br()) ? 0.1 : 0;

	score += matchedPrediction.rect.iouPercentage(this->possibleLocation) * 0.15;
    /* (0, 1), (distance, 0.75)*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         ;
	float32 distance = pow(this->possibleLocation.width, 2) + pow(this->possibleLocation.height, 2);
	float32 slope = (0.75 - 1.0) / (distance);
	float32 currentDistance = this->squareDistanceTo(matchedPrediction.rect);
	score += MAX(0.5*(slope*currentDistance + 1), 0);
    return score;
}

bool Entity::outsideFrame(const uint16_t rows, const uint16_t cols) const {
    
}

std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingBox().rect << "\n";
    return os;
}
