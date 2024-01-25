#include "tracker.hpp"

Tracker::Tracker(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentRecognition(points, types, size, frame); // sets this current recognition
	this->generateEntites();
	this->addToTrajectory();
}

Tracker::~Tracker(){
	cv::destroyAllWindows();
}

void Tracker::setCurrentRecognition(uint16_t *points, uint16_t* types, uint16_t size, uint8_t* frame){
	this->setFrame(frame);
	this->currentRecognition = Recognition(points, size, types);
}

void Tracker::setFrame(uint8_t* frame){
	this->frame = cv::Mat(this->rows, this->cols, CV_8UC3, frame);	
}

void Tracker::drawEntities(){
	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
		traverse->item.draw(this->frame);
		traverse = traverse->next;
	}
}

void Tracker::addToTrajectory(){
	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
		traverse->item.addToTrajectory();
		traverse = traverse->next;
	}
}	

void Tracker::matchEntity(){

	cout << "Recognition Size: " << currentRecognition.size << "\n";

	for (Rect& r : currentRecognition.rects){
		r.draw(this->frame, CV_RGB(0, 0, 0));
	}

    std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
        uint distanceSquared = UINT32_MAX;
		currentEntity.predictPossibleLocations();
        currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
        uint16_t matchingEntityIndex = UINT16_MAX;
        for (uint16_t i = 0, size = currentRecognition.size; i < size; i++){
            const Rect& checkedRect = currentRecognition.rects[i];
            const uint16_t& checkedType = currentRecognition.types[i];

            if (currentEntity.getType() == checkedType) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(checkedRect);
                if (currentDistanceSquared < distanceSquared && 
                    currentEntity.getPossibleLocation().contains(checkedRect.center)){
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
		traverse = traverse->next;
    }
}	

void Tracker::generateEntites(){

	for (uint16_t i = 0, size = currentRecognition.size; i < size; i++){
        this->entities.append(Entity(currentRecognition.types[i], currentRecognition.rects[i]));
	}

}

extern "C" void printRects(Rect* p, uint size);

void Tracker::track(uint16_t* points, uint16_t* types, uint16_t size, uint8_t* frame){


	this->setCurrentRecognition(points, types, size, frame);
	printRects(&this->currentRecognition.rects.front(), this->currentRecognition.size);
	this->matchEntity();
	if (visualization::_toVisualize){

		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	this->addToTrajectory();

}	