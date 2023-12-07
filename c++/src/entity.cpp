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

Rect Entity::possibleLocations(){
    this->clacVelocities(3);
    Rect& currentRect = this->getBoundingRect();
    const int& W = currentRect.width;
    const int& H = currentRect.height;
    const float K = 0.15; //TODO MAKE SOMEHOW CALCULATED
    const float J = 0.35; //TODO MAKE SOMEHOW CALCULATED
    // const float N = 5; //TODO MAKE SOMEHOW CALCULATED
    // const float M = 5; //TODO MAKE SOMEHOW CALCULATED
    Rect possibleLocations = Rect(currentRect.tl() - Point(K * W,  K * H), cv::Size2i(J * W, J * H));

    // if (this->velocities.x != 0 || this->velocities.y != 0){
    //     cout << this->id << "\n";
    //     uint16_t x = currentRect.tl().x + N * this->velocities.x;
    //     uint16_t y = currentRect.tl().y + N * this->velocities.y;
    //     Point A = Point(x, y);
    //     x = currentRect.tl().x - M * this->velocities.x;
    //     y = currentRect.tl().y - M * this->velocities.y;
    //     Point B = Point(x, y);
    //     cout << "tl: " << currentRect.tl() << " A: " << A << " B: " << B << " vel: " << this->velocities << "\n";
    //     possibleLocations = Rect(A, B);
    // }



    return possibleLocations;
}

uint16_t Entity::findClosestEntityIndex(std::vector<Entity>& entityVector){

    // uint maxDistanceSquared = 500; //TODO MAKE A SOMEHOW CALCULATED ONE
    uint distanceSquared = UINT32_MAX;
    uint16_t idx = entityVector.size(); 

    Rect possibleLocations = this->possibleLocations();

    for (uint16_t i = 0, size = entityVector.size(); i < size; i++){
        Entity& checkedEntity = entityVector[i];

        if (this->getType() == checkedEntity.getType()) { 
            uint currentDistanceSquared = this->squareDistanceTo(checkedEntity);
            if (currentDistanceSquared < distanceSquared && possibleLocations.contains(checkedEntity.getBoundingRect().tl())){
                idx = i;
                distanceSquared = currentDistanceSquared;
            }
        }       
    }
    return idx;
}

void Entity::clacVelocities(int numOfFrames){
    //units are pixels per frame   
    if (this->trajectory->length >= numOfFrames){
        const Rect& startRect = this->trajectory->getItem(numOfFrames - 1).rect;
        const Rect& endRect = this->trajectory->getItem(0).rect;
        cout << "endX: " << endRect.x << " startX: " << startRect.x << "\n";
        int velX = (endRect.x - startRect.x) / numOfFrames;
        int velY = (endRect.y - startRect.y) / numOfFrames;
        this->velocities = Velocity2D(velX, velY);
    } 
    
}


Rect Entity::predictNextBoundingRect(){
    Rect prediction = this->boundingRect;
    prediction.x += this->velocities.x;
    prediction.y += this->velocities.y;
    return prediction;
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
