#include "rect.hpp"

/**
 *
 * PLEASE READ
 * This is a file to define all the node types, nodes are just struct so there is not a proper interface.
 * In oreder to create a struck, just add one here, and make sure it has a 
 * share_ptr into the same type called "next".
 * a constructor 
 * and a copy constructor
 * see the nodes below as an example.
 * 
*/  

typedef cv::Point2i Velocity2D;


typedef struct TrajectoryNode TrajectoryNode;

struct TrajectoryNode{
    
    const Rect rect;
    Velocity2D velocity;
    std::shared_ptr<struct TrajectoryNode> next;

///////////////////////////////////////// Constructors //////////////////////////////////////////////////

    TrajectoryNode(Rect rect, Velocity2D velocity): rect(rect), velocity(velocity), next(nullptr) {}

    TrajectoryNode(const TrajectoryNode& node) : rect(node.rect), velocity(node.velocity), next(nullptr) {}

    TrajectoryNode(): rect(Rect()), next(nullptr) {}

};

typedef struct EntityNode EntityNode;

struct EntityNode{

    const Entity entity;
    std::shared_ptr<struct EntityNode> next;

    EntityNode(const Entity entity) : entity(entity), next(nullptr) {}

    EntityNode() : entity(Entity()), next(nullptr) {}
};

