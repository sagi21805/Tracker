#include "entity.hpp"

////////////////////////////////// constructors //////////////////////////////////////////////////

Entity::Entity(uint16_t id, uint16_t type, Rect boundingRect, Mat& frameInside)
    : id(id), type(type){
    this->setBoundingRect(boundingRect);
    this->trajectory = std::make_shared<LinkedList>(boundingRect, Velocity2D(0, 0));
}

Entity::Entity(const Entity& e) 
    : id(e.getId()), type(e.getType()){
    this->setBoundingRect(e.getBoundingRect());
    this->trajectory = e.copyTrajectory();
}

Entity::Entity() 
    : id(UINT16_MAX), type(UINT16_MAX){
    this->boundingRect = Rect();
    this->trajectory = std::make_shared<LinkedList>();
}

////////////////////////////////// Get Functions //////////////////////////////////////////////////

Rect& Entity::getBoundingRect(){
    return this->boundingRect;
}

uint16_t Entity::getId(){
    return this->id;
}

uint16_t Entity::getType(){
    return this->type;
}

Velocity2D& Entity::getVelocity(){
    return this->velocity;
}

std::weak_ptr<LinkedList> Entity::getTrajectory(){
    std::weak_ptr<LinkedList> weak_trajectory = this->trajectory;
    return weak_trajectory;
}

std::shared_ptr<LinkedList> Entity::copyTrajectory(){
    return this->trajectory;
}

Rect Entity::getBoundingRect() const{
    return this->boundingRect;
}

uint16_t Entity::getId() const{
    return this->id;
}

uint16_t Entity::getType() const{
    return this->type;
}

Velocity2D Entity::getVelocity() const{
    return this->velocity;
}

std::weak_ptr<LinkedList> Entity::getTrajectory() const{
    std::weak_ptr<LinkedList> weak_trajectory = this->trajectory;
    return weak_trajectory;
}

std::shared_ptr<LinkedList> Entity::copyTrajectory() const{
    return this->trajectory;
}

void Entity::setBoundingRect(Rect boundningRect){
    this->boundingRect = boundningRect;
}

void Entity::setVelocity(Velocity2D velocity){
    this->velocity = velocity;
}

Velocity2D Entity::calcVelocity(uint8_t numOfFrames){

    if (this->trajectory->length >= numOfFrames){
        const Rect& startRect = this->trajectory->getItem(numOfFrames - 1).rect;
        const Rect& endRect = this->trajectory->getItem(0).rect;
        cout << "endX: " << endRect.x << " startX: " << startRect.x << "\n";
        float velX = (float) (endRect.x - startRect.x) / numOfFrames;
        float velY = (float) (endRect.y - startRect.y) / numOfFrames;
        return Velocity2D(velX, velY);
    } 

    return Velocity2D();
}

vector<Point> Entity::calcContour(Mat& frameInside){
    Mat entityInFrame = frameInside(this->boundingRect);
    
} //TODO make this function.

void Entity::addToTrajectory(){
    this->trajectory->prepend(Node(this->boundingRect, this->velocity));
}

void Entity::emptyBoundingRect(){
    this->setBoundingRect(Rect());
}

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
    const float& K = _predictionOffsetCoefficient; 
    const float& J = _predictionSizeCoefficient;
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
