#ifndef _Entity_
#define _Entity_

#include "linkedList.hpp"
#include <limits>
#include "vectorFuncs.hpp"
#include "trajectory.hpp"
#include "boundingBox.hpp"
#include <format>

enum Types{
    BlueRobot = 0, RedRobot
};

//////////////////////////////////////////// Entity //////////////////////////////////////////////////////
/**
 * @file entity.hpp
 * @brief The Entity class.
 * @paragraph 
 *  The Entity class contains an easy way to create an Entity from YOLO_DATA 
 *  The contains easy way to gather information like trajectory, velocity, and position of the Entity, 
 *  as well as prediction methods based on that info.
 *  
 * @author Sagi Or
*/  
class Entity{

    private:

        uint16_t id; //The id of the Entity **cannot be changed outside of Entity after creation**
        BoundingBox boundingBox; //The bounding rectangle of the Entity in pixels
        Velocity2D velocity; //The Velocity (x, y) of the Entity in pixels per frame
        LinkedList<Trajectory> trajectory; //the Trajectory of the Entity which contains previous boundingBox and velocity
        Rect possibleLocation;
        cv::Scalar color;
        //countour

    public:
        
///////////////////////////////////////// Constructors //////////////////////////////////////////////////

        /**
         * @file entity.cpp
         * @brief Entity constractor from raw data.
         * @param id The id of the Entity.
         * @param type The type of the Entity.
         * @param boundingBox The boundingBox of the Entity.
         * @author Sagi Or
        */
        Entity(BoundingBox boundingBox);

        /**
         * @file entity.cpp
         * @brief Entity constractor from another Entity.
         * @param e The Entity to construct from.
         * @author Sagi Or
        */
        Entity(const Entity& e);

        /**
         * @file entity.cpp
         * @brief Entity defualt constractor.
         * @author Sagi Or
        */
        Entity();

        
///////////////////////////////////////// Get Functions //////////////////////////////////////////////////

        /**
         * @file entity.cpp
         * @brief Get Entity's boundingBox
         * @returns Reference to the Entity's boundingBox.
         * @author Sagi Or
        */
        BoundingBox& getBoundingBox();
        
        /**
         * @file entity.cpp
         * @brief Get Entity's id.
         * @returns The Entity's id.
         * @author Sagi Or
        */
        uint16_t getId();

        /**
         * @file entity.cpp
         * @brief Get Entity's type.
         * @returns The Entity's type.
         * @author Sagi Or
        */
        uint16_t getType();
        
        /**
         * @file entity.cpp
         * @brief Get Entity's Velocity.
         * @returns Reference To the Entity's velocity.
         * @author Sagi Or
        */
        Velocity2D& getVelocity();

        /**
         * @file entity.cpp
         * @brief Get Entity's weak_ptr to the Entity's Trajectory.
         * @returns Weak_ptr the Entity's Trajectory.
         * @author Sagi Or
        */
        LinkedList<Trajectory>& getTrajectory();
        
        /**
         * @file entity.cpp
         * @brief Get Entity's a shared_ptr to the Entity's Trajectory.
         * @returns Shared_ptr to the Entity's Trajectory.
         * @author Sagi Or
        */
        LinkedList<Trajectory> copyTrajectory();

        Rect& getPossibleLocation();

        /**
         * @file entity.cpp
         * @brief Get Entity's boundingBox.
         * @returns copy of the Entity's boundingBox.
         * @author Sagi Or
        */
        BoundingBox getBoundingBox() const;

        /**
         * @file entity.cpp
         * @brief Get the Entity's id.
         * @returns The Entity's id.
         * @author Sagi Or
        */
        uint16_t getId() const;

        /**
         * @file entity.cpp
         * @brief Get the Entity's type.
         * @returns The Entity's type.
         * @author Sagi Or
        */
        uint16_t getType() const;

        /**
         * @file entity.cpp
         * @brief Get the Entity's Velocity.
         * @returns The Entity's velocity.
         * @author Sagi Or
        */
        Velocity2D getVelocity() const;

        /**
         * @file entity.cpp
         * @brief Get Entity's a shared_ptr to the Entity's Trajectory.
         * @returns Shared_ptr the Entity's Trajectory.
         * @author Sagi Or
        */
        LinkedList<Trajectory> copyTrajectory() const;

        Rect getPossibleLocation() const;


///////////////////////////////////////// Set Functions //////////////////////////////////////////////////

        /**
         * @file entity.cpp
         * @brief Sets the boundingBox of the Entity to a given Rect.
         * @param newboundingBox The new boundingBox to set.
         * @author Sagi Or
        */
        void setBoundingBox(BoundingBox boundingBox);


///////////////////////////////////////// Util Functions //////////////////////////////////////////////////
        
        /**
         * @file entity.cpp
         * @brief Calculates the average Velocity on a given number of frames and returns it.
         * @param numOfFrames The number of frames to avrage velocity on.
         * @returns The calculated Velocity.
         * @author Sagi Or
        */
        void calcAndSetVelocity();

        // vector<Point> calcContour(Mat& frameInside);

        /**
         * @file entity.cpp
         * @brief Addes the current boundingBox and Velocity to the Trajectory.
         * @author Sagi Or
        */
        void addToTrajectory();     

        /**
         * @file entity.cpp
         * @brief Sets the Entity's boundingBox to EmptyRect.
         * @author Sagi Or
        */
        void emptyboundingBox();

        /**
         * @file entity.cpp
         * @brief Calculates the Square distance between two Entities boundingBox.
         * @returns The Squared distance between the Entities.
         * @author Sagi Or
        */
        uint squareDistanceTo(const Entity &e);
        
        uint squareDistanceTo(const Rect& r);

        void predictPossibleLocations();
        
        void predictNextBoundingBox();
        
        
        /**
         * @file entity.cpp
         * @brief draws the boundingBox of the Entity on a given frame 
         * @param frame The frame to draw the boundingBox on.
         * @param color The color of the boundingBox.
         * @author Sagi Or
        */
        void draw(cv::Mat& frame);

        cv::Scalar chooseColor();
};

/**
 * @file entity.cpp
 * @brief Defines the operator << to print an Entity
 * @param e The Entity to print.
 * @author Sagi Or
*/
std::ostream& operator<<(std::ostream& os, const Entity& e);

#endif