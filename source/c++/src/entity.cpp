#include "entity.hpp"
using predictions::_offset, predictions::_smoothingFrames;

////////////////////////////////// constructors //////////////////////////////////////////////////

Entity::Entity(BoundingBox boundingBox)
    : id(core::_startingId), boundingBox(boundingBox), velocity(Velocity2D(0, 0)), color(this->chooseColor()){
    core::_startingId++;
    this->trajectory = LinkedList<Trajectory>(Trajectory(boundingBox, Velocity2D(0, 0)));
    this->predictPossibleLocations();
}

Entity::Entity(const Entity& e) 
    : id(e.getId()), boundingBox(e.getBoundingBox()), velocity(e.getVelocity()), 
    trajectory(e.copyTrajectory()), possibleLocation(e.getPossibleLocation()), color(this->chooseColor()){
}

Entity::Entity() : id(UINT16_MAX), boundingBox(BoundingBox()), trajectory(LinkedList<Trajectory>()){}

////////////////////////////////// Get Functions //////////////////////////////////////////////////

BoundingBox& Entity::getBoundingBox(){
    return this->boundingBox;
}

uint16_t Entity::getId(){
    return this->id;
}

uint16_t Entity::getType(){
    return this->boundingBox.type;
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

BoundingBox Entity::getBoundingBox() const{
    return this->boundingBox;
}

uint16_t Entity::getId() const{
    return this->id;
}

uint16_t Entity::getType() const{
    return this->boundingBox.type;
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

void Entity::setBoundingBox(BoundingBox boundingBox){
    if (this->trajectory.length >= _smoothingFrames){
        float32 confsum = 0;
        int32_t x = 0, y = 0, width = 0, height = 0;
        std::shared_ptr<Node<Trajectory>> traverse = this->trajectory.start;
        for (uint8_t i = 0; i < _smoothingFrames; i++){
            confsum += traverse->item.box.confidence;
            x += traverse->item.box.rect.x;
            y += traverse->item.box.rect.y;
            width += traverse->item.box.rect.width;
            height += traverse->item.box.rect.height;
        }
    } else {
        this->boundingBox = boundingBox;
    }
}

void Entity::addToTrajectory(){
    this->trajectory.prepend(Trajectory(this->boundingBox, this->velocity));
}

void Entity::emptyboundingBox(){
    this->setBoundingBox(BoundingBox());
}

