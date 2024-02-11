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

void Entity::setBoundingBox(BoundingBox box){
    if (this->trajectory.length >= _smoothingFrames){
        float32 confsum = box.confidence;
        float32 x = box.rect.x*confsum, y = box.rect.y*confsum, width = box.rect.width*confsum, height = box.rect.height*confsum;
        std::shared_ptr<Node<Trajectory>> traverse = this->trajectory.start;
        for (uint8_t i = 0; i < _smoothingFrames-1; i++){
            confsum += traverse->item.box.confidence;
            x += traverse->item.box.rect.x * traverse->item.box.confidence;
            y += traverse->item.box.rect.y * traverse->item.box.confidence;
            width += traverse->item.box.rect.width * traverse->item.box.confidence;
            height += traverse->item.box.rect.height * traverse->item.box.confidence;
            traverse = traverse->next;
        }
        this->boundingBox = BoundingBox(Rect(x/confsum, y/confsum, width/confsum, height/confsum), getType(), confsum/_smoothingFrames);
    } else {
        this->boundingBox = box;
    }
}

void Entity::addToTrajectory(){
    this->trajectory.prepend(Trajectory(this->boundingBox, this->velocity));
}

void Entity::emptyboundingBox(){
    this->setBoundingBox(BoundingBox());
}

