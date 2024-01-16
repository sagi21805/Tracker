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

typedef struct TrajectoryNode TrajectoryNode;
typedef cv::Point2i Velocity2D;

struct TrajectoryNode{
    
    const Rect rect;
    Velocity2D velocity;
    std::shared_ptr<struct TrajectoryNode> next;

///////////////////////////////////////// Constructors //////////////////////////////////////////////////

    /**
     * @file linkedList.hpp
     * @brief Node constructor from raw data.
     * @author Sagi Or
    */
    TrajectoryNode(Rect rect, Velocity2D velocity): rect(rect), velocity(velocity), next(nullptr) {}

    TrajectoryNode(const TrajectoryNode& node) : rect(node.rect), velocity(node.velocity), next(nullptr) {}

    /**
     * @file linkedList.hpp
     * @brief Node defualt constructor.
     * @author Sagi Or
    */
    TrajectoryNode(): rect(Rect()), next(nullptr) {}

};

