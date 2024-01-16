#include "entity.hpp"

////////////////////////////////// constructors //////////////////////////////////////////////////

Entity::Entity(uint16_t id, uint16_t type, Rect boundingRect)
    : id(id), type(type){
    this->setBoundingRect(boundingRect);
    this->predictPossibleLocations();
    this->trajectory = std::make_shared<LinkedList<TrajectoryNode>>(TrajectoryNode(boundingRect, Velocity2D(0, 0)));
    // this->calcContour(frameInside);
}

Entity::Entity(const Entity& e) 
    : id(e.getId()), type(e.getType()){
    this->setBoundingRect(e.getBoundingRect());
    this->trajectory = e.copyTrajectory();
}

Entity::Entity() 
    : id(UINT16_MAX), type(UINT16_MAX){
    this->boundingRect = Rect();
    this->trajectory = std::make_shared<LinkedList<TrajectoryNode>>();
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

std::weak_ptr<LinkedList<TrajectoryNode>> Entity::getTrajectory(){
    std::weak_ptr<LinkedList<TrajectoryNode>> weak_trajectory = this->trajectory;
    return weak_trajectory;
}

std::shared_ptr<LinkedList<TrajectoryNode>> Entity::copyTrajectory(){
    return this->trajectory;
}

Rect& Entity::getPossibleLocation(){
    return this->possibleLocation;
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

std::weak_ptr<LinkedList<TrajectoryNode>> Entity::getTrajectory() const{
    std::weak_ptr<LinkedList<TrajectoryNode>> weak_trajectory = this->trajectory;
    return weak_trajectory;
}

std::shared_ptr<LinkedList<TrajectoryNode>> Entity::copyTrajectory() const{
    return this->trajectory;
}

void Entity::setBoundingRect(Rect boundningRect){
    this->boundingRect = boundningRect;
}

void Entity::addToTrajectory(){
    this->trajectory->prepend(TrajectoryNode(this->boundingRect, this->velocity));
}

void Entity::emptyBoundingRect(){
    this->setBoundingRect(Rect());
}

