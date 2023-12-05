#include "entity.hpp"

Entity::Entity(uint16_t id, uint16_t type, Rect boundingRect) 
    : id(id), type(type){
    this->setBoundingRect(boundingRect);
    this->trajectory = std::make_shared<LinkedList>(boundingRect, Velocity2D(0, 0));
}

Entity::Entity(const Entity& e) 
    : id(e.getId()), type(e.getType()){
    this->setBoundingRect(e.getBoundingRect());
    this->trajectory = e.getTrajetctory();
}

Entity::Entity() 
    : id(UINT16_MAX), type(UINT16_MAX){
    this->boundingRect = Rect();
    this->trajectory = std::make_shared<LinkedList>();
}

std::shared_ptr<LinkedList> Entity::getTrajetctory(){
    return this->trajectory;
}

void Entity::setBoundingRect(Rect boundningRect){
    this->boundingRect = boundningRect;
}

void Entity::addToTrajectory(){
    std::shared_ptr<Node> newNode = std::make_shared<Node>(this->boundingRect, this->velocities);
    this->trajectory->prepend(newNode);
}

Rect& Entity::getBoundingRect(){
    return this->boundingRect;
}

uint16_t Entity::getId(){
    return this->id;
}

uint16_t Entity::getType(){
    return this->type;
}

Velocity2D Entity::getVelocities(){
    return this->velocities;
}

std::shared_ptr<LinkedList> Entity::getTrajetctory() const{
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

uint Entity::squareDistanceTo(Entity& e){
    return this->getBoundingRect().squareDistanceTo(e.getBoundingRect());
}

std::ostream& operator<<(std::ostream& os, Entity& e){
    os << "id: " << e.getId() << "\n";
    os << "type: " << e.getType() << "\n";
    os << "box: " << e.getBoundingRect() << "\n";
    return os;
}

void Entity::emptyBoundingRect(){
    this->setBoundingRect(Rect());
}


uint16_t Entity::findClosestEntityIndex(std::vector<Entity>& entityVector){

    // uint maxDistanceSquared = 500; //TODO MAKE A SOMEHOW CALCULATED ONE
    uint distanceSquared = UINT32_MAX;
    uint16_t idx = entityVector.size(); 
    const int W = this->getBoundingRect().width; //TODO MAKE A SOMEHOW CALCULATED ONE
    const int H = this->getBoundingRect().height; //TODO MAKE A SOMEHOW CALCULATED ONE
    Rect& currentRect = this->getBoundingRect();

    Rect possibleLocations = Rect(currentRect.tl() + Point(-W, -H), cv::Size2i(2*W, 2*H));

    if (this->velocities.x != 0 && this->velocities.y != 0){
        const int N = 2;
        const int M = 0.5;
        Point A = Point(currentRect.x + N * this->velocities.x, currentRect.y - N * this->velocities.y);
        Point B = Point(currentRect.x - M * this->velocities.x, currentRect.y + M * this->velocities.y);
        possibleLocations = Rect(A, B);
    }

    for (uint16_t i = 0, size = entityVector.size(); i < size; i++){
        Entity& checkedEntity = entityVector[i];

        if (this->getType() == checkedEntity.getType()) { 
            uint currentDistanceSquared = this->squareDistanceTo(checkedEntity);
            cout << currentDistanceSquared << "\n";
            if (currentDistanceSquared < distanceSquared && possibleLocations.contains(checkedEntity.getBoundingRect().tl())){
                idx = i;
                distanceSquared = currentDistanceSquared;
            }
        }       
    }
    return idx;
}

Velocity2D Entity::clacVelocities(int numOfFrames){
    //units are pixels per frame   
    if (this->trajectory->length >= numOfFrames){
        const Rect& startRect = this->trajectory->getItem(numOfFrames - 1).rect;
        const Rect& endRect = this->trajectory->getItem(0).rect;
        int velX = ((endRect.x - startRect.x) / numOfFrames);
        int velY = ((endRect.y - startRect.y) / numOfFrames);
        return Velocity2D(velX, velY);
    } 
    return Velocity2D();
}

void Entity::clacAndSetVelocities(int numOfFrames){
    this->velocities = this->clacVelocities(numOfFrames); //TODO make this number with logic
}

Rect Entity::predictNextBoundingRect(){
    Rect prediction = this->boundingRect;
    prediction.x += this->velocities.x;
    prediction.y += this->velocities.y;
    return prediction;
}

void Entity::draw(cv::Mat& frame, cv::Scalar color){
		cv::rectangle(frame, this->boundingRect, color, 2);
		cv::putText(frame, std::to_string(this->id), this->boundingRect.tl(), cv::FONT_HERSHEY_DUPLEX, 1, CV_RGB(255, 255, 0), 2);
}


std::ostream& operator<<(std::ostream& os, const Entity& t){
    os << "id: " << t.getId() << "\n";
    os << "type: " << t.getType() << "\n";
    os << "box: " << t.getBoundingRect() << "\n";
    return os;
}
