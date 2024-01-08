#include "tracker.hpp"

Tracker::Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentTrack(points, types, size, frame); // sets this current recognition
	this->entities = vector<Entity>(this->generateEntites(this->currentRecognition));
	this->addToTrajectory();
}

Tracker::~Tracker(){
	cv::destroyAllWindows();
}

void Tracker::setCurrentTrack(uint16_t *points, uint16_t* types, uint16_t size, uint8_t* frame){
	this->setFrame(frame);
	this->currentRecognition = Recognition(pointsToRects(points, size), types);
}

void Tracker::setFrame(uint8_t* frame){
	this->frame = cv::Mat(this->rows, this->cols, CV_8UC3, frame);	
}

void Tracker::drawEntities(){
	for (uint16_t i = 0, size = this->entities.size(); i < size; i++){
		Entity& drawnEntity = this->entities[i];
		cv::Scalar color = this->chooseColor(drawnEntity);
		drawnEntity.draw(this->frame, color);
	}
}

void Tracker::drawPredictions(){
	for (uint16_t i = 0, size = this->currentPrediction.size(); i < size; i++){
		Entity& drawnEntity = this->currentPrediction[i];
		drawnEntity.draw(this->frame, CV_RGB(255, 255, 255));
	}
}

void Tracker::addToTrajectory(){
	for (Entity& entity : this->entities){
		entity.addToTrajectory();
	}
}	

cv::Scalar Tracker::chooseColor(Entity& e){
	cv::Scalar color;
	switch (e.getType()){
			case RedRobot:
				color = visualization::_colors[RedRobot];
				break;
			case BlueRobot:
				color = visualization::_colors[BlueRobot];
				break;
			default:
				color = CV_RGB(0, 0, 0);
		}
	return color;
}

void Tracker::matchEntity(vector<Entity>& currentEntities, Recognition& currentRecognition, Mat& frame){
    
    for (Rect& r : currentRecognition.rects){
        r.drawRect(frame, CV_RGB(0, 0, 0));
    }

    for (uint16_t j = 0, size = MIN(currentEntities.size(), currentRecognition.size); j < size; j++){
        Entity& currentEntity = currentEntities[j];
        uint distanceSquared = UINT32_MAX;
        currentEntity.calcAndSetVelocity();
        Rect possibleLocations = currentEntity.predictPossibleLocations();
        possibleLocations.drawRect(frame, CV_RGB(255, 255, 255));
    
        uint16_t matchingEntityIndex = UINT16_MAX;
        for (uint16_t i = 0, size = currentRecognition.size; i < size; i++){
            const Rect& checkedRect = currentRecognition.rects[i];
            const uint16_t& checkedType = currentRecognition.types[i];

            if (currentEntity.getType() == checkedType) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(checkedRect);
                if (currentDistanceSquared < distanceSquared && 
                    possibleLocations.contains(checkedRect.tl())){
                    matchingEntityIndex = i;
                    distanceSquared = currentDistanceSquared;
                }
            }

            
        }
        if (matchingEntityIndex < UINT16_MAX){
            currentEntity.setBoundingRect(currentRecognition.rects[matchingEntityIndex]);
            currentRecognition.remove(matchingEntityIndex);
        }
        else {
            currentEntity.setBoundingRect(currentEntity.predictNextBoundingRect());
        }
    }

    // if (currentRecognition.size > 0){
    //     for (uint16_t i = 0; i < currentRecognition.size; i++){
    //         currentEntities.emplace_back(generateEntity(currentRecognition.rects[i], currentRecognition.types[i]));
    //     }
    //     currentRecognition.clear();
    // }
}

vector<Entity> Tracker::generateEntites(Recognition currentRecognition){;
    vector<Entity> generatedEntities;
	generatedEntities.reserve(currentRecognition.size);
	
	for (uint16_t i = 0, size = currentRecognition.size; i < size; i++){
        generatedEntities.emplace_back(generateEntity(currentRecognition.rects[i], currentRecognition.types[i]));
	}
	return generatedEntities;

}

//TODO using a lot of shared pointers - performence heavy.
//TODO imporve all function performence!.
void Tracker::track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame){
	this->setCurrentTrack(points, types, size, frame);
	this->matchEntity(this->entities, this->currentRecognition, this->frame);

	if (visualization::_toVisualize){

		this->drawPredictions();
		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	// std::cin.get();
	this->addToTrajectory();
	this->currentPrediction.clear();

}	