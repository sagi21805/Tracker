#ifndef __TRACKER__HPP
#define __TRACKER__HPP

#include "entity.hpp"
#include <string>
#include <sstream>
#include <iostream>

enum Types{
    BlueRobot = 0, RedRobot
};


class Tracker{
    
    public:
        const bool visualize;
        const uint16_t rows;
        const uint16_t cols;
        cv::Mat frame;
        vector<Entity> entities;
        vector<Entity> currentRecognition;
        vector<Entity> currentPrediction;

        Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols, bool visualize);

        void setTrackPoints(uint16_t* points, uint16_t* types, uint16_t size);

        void setFrame(uint8_t* frame);

        void drawEntities();

        void drawPredictions();
        
        vector<Entity> rectsToEntites(vector<Rect> rects, uint16_t* pointsWithClass);

        void distanceTrack();

        void addToTrajectory();

        void track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame);

        cv::Scalar chooseColor(Entity& e);


};  


#endif