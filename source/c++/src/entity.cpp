#include "entity.hpp"

////////////////////////////////// constructors //////////////////////////////////////////////////

Entity::Entity(uint16_t type, Rect boundingRect)
    : id(core::_startingId), type(type), velocity(Velocity2D(0, 0)), boundingRect(boundingRect), color(this->chooseColor()){
    core::_startingId++;
    this->trajectory = LinkedList<Trajectory>(Trajectory(boundingRect, Velocity2D(0, 0)));
    this->predictPossibleLocations();
}

Entity::Entity(const Entity& e) 
    : id(e.getId()), type(e.getType()), velocity(e.getVelocity()), boundingRect(e.getBoundingRect()), 
    trajectory(e.copyTrajectory()), possibleLocation(e.getPossibleLocation()), color(this->chooseColor()){
}

Entity::Entity() 
    : id(UINT16_MAX), type(UINT16_MAX), color(this->chooseColor()){
    this->boundingRect = Rect();
    this->trajectory = LinkedList<Trajectory>();
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

LinkedList<Trajectory>& Entity::getTrajectory(){
    return this->trajectory;
}

LinkedList<Trajectory> Entity::copyTrajectory(){
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

LinkedList<Trajectory> Entity::copyTrajectory() const{
    return this->trajectory;
}

Rect Entity::getPossibleLocation() const{
    return this->possibleLocation;
}

void Entity::setBoundingRect(Rect boundningRect){
    this->boundingRect = boundningRect;
}

void Entity::addToTrajectory(){
    this->trajectory.prepend(Trajectory(this->boundingRect, this->velocity));
}

void Entity::emptyBoundingRect(){
    this->setBoundingRect(Rect());
}

