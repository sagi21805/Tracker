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

void Entity::predictNextBoundingBox(){
    this->boundingBox.rect.y += this->velocity.x;
    this->boundingBox.rect.y += this->velocity.x;
}

uint Entity::squareDistanceTo(const Entity& e){
    return this->getBoundingBox().rect.squareDistanceTo(e.getBoundingBox().rect);
}

uint Entity::squareDistanceTo(const Rect& r){
    return this->getBoundingBox().rect.squareDistanceTo(r);
}

void Entity::predictPossibleLocations(){
    this->calcAndSetVelocity();
    Rect& r = this->getBoundingBox().rect;
    const int vX = this->velocity.x*predictions::_velocityCoefficient;
    const int vY = this->velocity.y*predictions::_velocityCoefficient;
    const int w = r.width*predictions::_sizeCoefficient*signum(vX);
    const int h = r.height*predictions::_sizeCoefficient*signum(vY);
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
    this->boundingBox = this->boundingBox.combinedBoundingBox(b);
}

float32 Entity::clacScore(const BoundingBox& matchedPrediction){ // TODO SWITCH MAGIC NUMBERS
	// parameters should be built such that only one cant carry the score alone.
    if (this->boundingBox.rect.empty()) { return 0.0; }
	if (this->boundingBox.type != matchedPrediction.type) { return 0.0; }
	float32 score = 0.0;
	float32 iou = matchedPrediction.rect.iouPercentage(this->boundingBox.rect);
	score += (iou > 0) ? iou * 0.3 : 0;
    cout << "s1: " << score << " "; 
	float32 distance = pow(this->boundingBox.rect.width, 2) + pow(this->boundingBox.rect.height, 2);
	float32 slope = (1 - 0.35) / (-distance);
	float32 currentDistance = this->squareDistanceTo(matchedPrediction.rect);
	score += 0.7*(slope*currentDistance + 1);
    cout << "s2: " << score << " "; 
    return score;
}

std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingBox().rect << "\n";
    return os;
}
