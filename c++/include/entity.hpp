#ifndef _Entity_
#define _Entity_

#include "linkedList.hpp"
#include <limits>
#include "vectorFuncs.hpp"
#include "imgUtils.hpp"
#include "recognition.hpp"
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
        uint16_t type; //The Type/Class of the Entity **cannot be changed outside of Entity after creation**
        Velocity2D velocity; //The Velocity (x, y) of the Entity in pixels per frame
        Rect boundingRect; //The bounding rectangle of the Entity in pixels
        std::shared_ptr<LinkedList> trajectory; //the Trajectory of the Entity which contains previous boundingRect and velocity
        //countour

    public:
        
///////////////////////////////////////// Constructors //////////////////////////////////////////////////

        /**
         * @file entity.cpp
         * @brief Entity constractor from raw data.
         * @param id The id of the Entity.
         * @param type The type of the Entity.
         * @param BoundingRect The boundingRect of the Entity.
         * @author Sagi Or
        */
        Entity(uint16_t id, uint16_t type, Rect boundingRect);

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
         * @brief Get Entity's BoundingRect
         * @returns Reference to the Entity's BoundingRect.
         * @author Sagi Or
        */
        Rect& getBoundingRect();
        
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
        std::weak_ptr<LinkedList> getTrajectory();
        
        /**
         * @file entity.cpp
         * @brief Get Entity's a shared_ptr to the Entity's Trajectory.
         * @returns Shared_ptr to the Entity's Trajectory.
         * @author Sagi Or
        */
        std::shared_ptr<LinkedList> copyTrajectory();

        /**
         * @file entity.cpp
         * @brief Get Entity's BoundingRect.
         * @returns copy of the Entity's BoundingRect.
         * @author Sagi Or
        */
        Rect getBoundingRect() const;

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
         * @brief Get Entity's a weak_ptr to the Entity's Trajectory.
         * @returns Weak_ptr the Entity's Trajectory.
         * @author Sagi Or
        */
        std::weak_ptr<LinkedList> getTrajectory() const;

        /**
         * @file entity.cpp
         * @brief Get Entity's a shared_ptr to the Entity's Trajectory.
         * @returns Shared_ptr the Entity's Trajectory.
         * @author Sagi Or
        */
        std::shared_ptr<LinkedList> copyTrajectory() const;


///////////////////////////////////////// Set Functions //////////////////////////////////////////////////

        /**
         * @file entity.cpp
         * @brief Sets the boundingRect of the Entity to a given Rect.
         * @param newBoundingRect The new BoundingRect to set.
         * @author Sagi Or
        */
        void setBoundingRect(Rect newBoundingRect);


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
         * @brief Addes the current BoundingRect and Velocity to the Trajectory.
         * @author Sagi Or
        */
        void addToTrajectory();     

        /**
         * @file entity.cpp
         * @brief Sets the Entity's BoundingRect to EmptyRect.
         * @author Sagi Or
        */
        void emptyBoundingRect();

        /**
         * @file entity.cpp
         * @brief Calculates the Square distance between two Entities BoundingRect.
         * @returns The Squared distance between the Entities.
         * @author Sagi Or
        */
        uint squareDistanceTo(const Entity &e);
        
        uint squareDistanceTo(const Rect& r);
        /**
         * @brief Predicts possible future locations for the entity based on its current trajectory and velocity.
         * @param numOfFrames The number of frames into the future for which to predict possible locations.
         * @return A rectangular region representing the possible locations of the entity.
         *
         * This function calculates the possible locations of the entity in the future based on its current trajectory
         * and velocity. It considers the bounding rectangle of the entity and applies an offset and size coefficient
         * to create a region that represents the potential locations the entity topLeft corner of the boundingBox might occupy. 
         * The coefficients K and J control the offset and size of the region, respectively.
         *
         * The calculation takes into account the width (W) and height (H) of the current bounding rectangle. The region
         * is computed as a rectangle with its top-left corner shifted by an offset (K * W, K * H) and a size of (J * W, J * H).
         *
         * Note: The commented-out code block provides an alternative method for adjusting the predicted locations based on
         * the entity's velocity. Depending on the use case, you may choose to uncomment and modify that part of the code.
         *
         * @see Entity::calcVelocity(uint8_t numOfFrames) for calculating the entity's velocity.
         */
        Rect predictPossibleLocations();
        
        Rect predictNextBoundingRect();
        
        
        /**
         * @file entity.cpp
         * @brief draws the BoundingRect of the Entity on a given frame 
         * @param frame The frame to draw the BoundingRect on.
         * @param color The color of the BoundingRect.
         * @author Sagi Or
        */
        void draw(cv::Mat& frame, cv::Scalar color);
};

Entity generateEntity(Rect r, uint16_t type);

/**
 * @file entity.cpp
 * @brief Defines the operator << to print an Entity
 * @param e The Entity to print.
 * @author Sagi Or
*/
std::ostream& operator<<(std::ostream& os, const Entity& e);

#endif