#include "tracker.hpp"

Tracker::Tracker(uint16_t* points, uint16_t* types, float32* confidances, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentRecognition(points, types, confidances, size, frame); // sets this current recognition
	this->generateEntites();
	this->addToTrajectory();
}

Tracker::~Tracker(){
	cv::destroyAllWindows();
}

void Tracker::setCurrentRecognition(uint16_t *points, uint16_t* types, float32* confidances, uint16_t size, uint8_t* frame){
	this->currentRecognition = generateBoundingBoxes(points, types, confidances, size);
	this->stableRecognition();
	this->setFrame(frame);
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

void Tracker::stableRecognition() {
	uint16_t count = duplicatesCount(&this->currentRecognition.front(), this->currentRecognition.size(), 0.1, 0.2);
	this->currentRecognition.erase(this->currentRecognition.end() - count, this->currentRecognition.end());
}

void Tracker::matchEntity(){

	cout << "Recognition Size: " << currentRecognition.size() << "\n";

	for (BoundingBox& b : currentRecognition){
		b.rect.draw(this->frame, CV_RGB(0, 0, 0));
	}

    std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
        uint distanceSquared = UINT32_MAX;
		currentEntity.predictPossibleLocations();
        currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
        BoundingBox matchingBox = BoundingBox();
        for (BoundingBox box : currentRecognition){

            if (currentEntity.getType() == box.type) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(box.rect);
                if (currentDistanceSquared < distanceSquared && 
                    currentEntity.getPossibleLocation().contains(box.rect.center)){
                    distanceSquared = currentDistanceSquared;
					matchingBox = box;
                }
            }

            
        }
        if (!(matchingBox.isEmpty())){
            currentEntity.setBoundingBox(matchingBox);
        }
        else {
            currentEntity.predictNextBoundingBox();
        }
		traverse = traverse->next;
    }
}	

void Tracker::generateEntites(){

	for (BoundingBox& box : currentRecognition){
        this->entities.append(Entity(box));
	}

}

void Tracker::track(uint16_t* points, uint16_t* types, float32* confidances, uint16_t size, uint8_t* frame){


	this->setCurrentRecognition(points, types, confidances, size, frame);
	this->matchEntity();
	if (visualization::_toVisualize){	
		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	this->addToTrajectory();

}	