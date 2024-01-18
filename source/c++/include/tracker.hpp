#ifndef _Tracker_
#define _Tracker_

#include "entity.hpp"

/**
 * @file tracker.hpp
 * @brief The enum for the types, each type that gets in is a num in the enum.
 * @author Sagi Or
*/
enum Types{
    BlueRobot = 0, RedRobot
};

/**
 * @file tracker.hpp
 * @brief custom Tracker that is ment to get his input from Yolo Model and run Fast and Acurate custom SORT like algorithem.
 * @paragraph 
 *  This Tracker is ment to Track Acuratlly and Quicklly Tragets based on thier attributes
 *  and data that is collected along the way.
 *  This Tracker works by getting Recgonitions of the objects in certain points in time 
 *  and trys to match thease recognitions with exsiting ones. 
 * @author Sagi Or
*/  
class Tracker{
    
    public:
        const uint16_t rows; //The number of rows in the given Img (assumes The same for each input).
        const uint16_t cols; //The number of columns in the given Img (assumes The same for each input).
        cv::Mat frame; //The current frame that is being tracked.
        LinkedList<Entity> entities; //All the Entities that the Tracker keeps track on.
        Recognition currentRecognition; //The Entities from The current recognition.


/////////////////////////////////////////////////////// Constructors ///////////////////////////////////////////////////////////////////////////////////////
    
        /**
         * @file tracker.cpp
         * @brief The tracker constructor.
         * @param points The array of points of the Rectangles that are recognized by Yolo.
         * @param types The array of types of the Rectangles that are recognized by Yolo.
         * @param size The size of the types array (how many points are in the first array).
         * @param frame The current frame that the recognition is on.
         * @param rows The num of rows in the frame.
         * @param cols The num of cols in the frame.
         * @author Sagi Or
        */
        Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols);

        ~Tracker();

/////////////////////////////////////////////////////// Set Functions ///////////////////////////////////////////////////////////////////////////////////////

        /**
         * @file tracker.cpp
         * @brief Sets the Tracker current track to the given points and frame.
         * @param points The array of points of the Rectangles that are recognized by Yolo.
         * @param types The array of types of the Rectangles that are recognized by Yolo.
         * @param size The size of the types array (how many points are in the first array).
         * @param frame 
         * @author Sagi Or
        */
        void setCurrentRecognition(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame);

        /**
         * @file tracker.cpp
         * @brief Sets the Tracker current frame to the given frame.
         * @param points The frame in array form.
         * @author Sagi Or
        */
        void setFrame(uint8_t* frame);


/////////////////////////////////////////////////////// Util Functions ////////////////////////////////////////////////////////////////////////

        /**
         * @file tracker.cpp
         * @brief Draws the Entities Vector on the current frame.
         * @author Sagi Or
        */
        void drawEntities();

        /**
         * @file tracker.cpp
         * @brief For all the entities in the entity vector adds Thier parameters to thier trajectory.
         * @author Sagi Or
        */
        void addToTrajectory();

        /**
         * @file tracker.cpp
         * @brief Choose the color of an Entitiy by their type.
         * @param e The entity to choose his color.
         * @author Sagi Or
        */
        cv::Scalar chooseColor(Entity& e);

        /**
         * @file tracker.cpp
         * @brief Turn a rect vector into a Entity vector.
         * @param rects The rect vector to turn.
         * @param types The types corresponding to the rect vector.
         * @author Sagi Or
        */
        void generateEntites(Recognition currentRecognition);
        
        /**
         * @file tracker.cpp
         * @brief Calls the track commend which performs track on the data that acquired as well as the new data that gets in.
         * @param points The array of points of the Rectangles that are recognized by Yolo.
         * @param types The array of types of the Rectangles that are recognized by Yolo.
         * @param size The size of the types array (how many points are in the first array).
         * @param frame The current frame that the recognition is on.
         * @author Sagi Or
        */
        void track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame);

        void matchEntity(std::vector<Entity> &currentEntities, Recognition& currentRecognition);
        

};  


#endif