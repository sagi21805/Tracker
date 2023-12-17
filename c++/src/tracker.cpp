#include "tracker.hpp"

Tracker::Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentTrack(points, types, size, frame); // sets this current recognition
	this->entities = vector<Entity>(this->currentRecognition);
	this->addToTrajectory();
}

void Tracker::setCurrentTrack(uint16_t *points, uint16_t* types, uint16_t size, uint8_t* frame){
	this->setFrame(frame);
	this->generateEntites(pointsToRects(points, size), types);
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

void Tracker::distanceTrack(){
	uint16_t size = MIN(this->entities.size(), this->currentRecognition.size());
	for (uint16_t i = 0; i < size; i++){
		Entity& checkedEntity = this->entities[i];	
		Rect c = checkedEntity.predictPossibleLocations(predictions::_numOfFrames);
		c.drawRect(this->frame, CV_RGB(255, 255, 255));
		uint16_t closetEntityIndex = checkedEntity.matchEntity(this->currentRecognition, predictions::_numOfFrames);
		Entity& closetEntity = this->currentRecognition[closetEntityIndex];

		checkedEntity.setBoundingRect(closetEntity.getBoundingRect());
		currentRecognition.erase(currentRecognition.begin() + closetEntityIndex);	
	}

	//at the end of the above loop current recognitions needs to be empty. if it doesnt, this loop is done to add to entities.
	uint16_t entitiesLeft = this->currentRecognition.size();
	this->entities.reserve(this->entities.size() + entitiesLeft);
	for (uint16_t i = 0; i < entitiesLeft; i++){
		this->entities.emplace_back(this->currentRecognition[i]);
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

void Tracker::generateEntites(vector<Rect> rects, uint16_t* types){;
    this->currentRecognition.reserve(rects.size());
	
	for (uint16_t i = 0, size = rects.size(); i < size; i++){

        this->currentRecognition.emplace_back(i, types[i], rects[i], this->frame);
	}

}

//TODO using a lot of shared pointers - performence heavy.
//TODO imporve all function performence!.
void Tracker::track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame){
	this->setCurrentTrack(points, types, size, frame);
	this->distanceTrack();

	if (visualization::_toVisualize){

		this->drawPredictions();
		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	this->currentRecognition.clear();
	this->currentPrediction.clear();

}	