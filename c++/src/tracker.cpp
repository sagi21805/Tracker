#include "tracker.hpp"

Tracker::Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* img, uint16_t rows, uint16_t cols, bool visualize) 
	: visualize(visualize), rows(rows), cols(cols){
	setTrackPoints(points, types, size); // sets this current recognition
	setImg(img);
	this->entities = vector<Entity>(this->currentRecognition);
	this->addToTrajectory();
}

void Tracker::setImg(uint8_t* img){
	this->img = cv::Mat(this->rows, this->cols, CV_8UC3, img);	
}

std::vector<Entity> Tracker::rectsToEntites(std::vector<Rect> rects, uint16_t* classes){
	std::vector<Entity> recognition;
    recognition.reserve(rects.size());

	for (uint16_t i = 0, size = rects.size(); i < size; i++){
        recognition.emplace_back(i, classes[i], rects[i]);
	}

	return recognition;
}

/*
Sets the current Rectes of the tracker
-
Args: 
 - `pointsWithClass (uint[])` -> xywh points with classes [x, y, w, h, c].
 - `size (uint)` -> How many points are in the array (Size of the array / size of a point).
*/
void Tracker::setTrackPoints(uint16_t *points, uint16_t* types, uint16_t size){
	this->currentRecognition = rectsToEntites(pointsToRects(points, size), types); //sets the currentStableStack inside stablePoints.
}

void Tracker::addToTrajectory(){
	for (Entity& entity : this->entities){
		entity.addToTrajectory();
		cout << entity.getTrajetctory()->length << " ";
		if (entity.getTrajetctory()->length == 1){
			std::cin.get();
		}
	}
	cout << "\n";
}	

cv::Scalar Tracker::chooseColor(Entity& e){
	cv::Scalar color;
	switch (e.getType()){
			case RedRobot:
				color = CV_RGB(255, 0, 0);
				break;
			case BlueRobot:
				color = CV_RGB(0, 0, 255);
				break;
			default:
				color = CV_RGB(0, 0, 0);
		}
	return color;
}

void Tracker::calcVelocities(int numOfFrames){
	for (Entity& e : this->entities){
		e.clacAndSetVelocities(numOfFrames);
	}
}

void Tracker::makePredictions(int numOfFrames){
	this->calcVelocities(numOfFrames);
	this->currentPrediction = std::vector<Entity>(this->entities);
	for (Entity& e : this->currentPrediction){
		e.setBoundingRect(e.getBoundingRect());
	}
	
}

void Tracker::distanceTrack(){

	this->makePredictions(3);
	uint16_t size = MIN(this->entities.size(), this->currentRecognition.size());
	for (uint16_t i = 0; i < size; i++){
		Entity& checkedEntity = this->entities[i];

		uint16_t closetEntityIndex = checkedEntity.findClosestEntityIndex(this->currentRecognition);
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


void Tracker::drawEntities(){
	for (uint16_t i = 0, size = this->entities.size(); i < size; i++){
		Entity& drawnEntity = this->entities[i];
		cv::Scalar color = this->chooseColor(drawnEntity);
		drawnEntity.draw(this->img, color);
	}
}

void Tracker::drawPredictions(){
	for (uint16_t i = 0, size = this->currentPrediction.size(); i < size; i++){
		Entity& drawnEntity = this->currentPrediction[i];
		drawnEntity.draw(this->img, CV_RGB(255, 255, 255));
	}
}


//TODO using a lot of shared pointers - performence heavy.
//TODO imporve all function performence!.
void Tracker::track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* img){
	
	this->setTrackPoints(points, types, size);
	this->setImg(img);
	this->addToTrajectory();
	this->distanceTrack();

	if (this->visualize){

		this->drawPredictions();
		this->drawEntities();
		cv::imshow("frame", this->img);
		cv::waitKey(1);
	}
	this->currentRecognition.clear();
	this->currentPrediction.clear();

}	