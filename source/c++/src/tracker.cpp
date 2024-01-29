#include "tracker.hpp"

Tracker::Tracker(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentRecognition(points, types, confidences, size, frame); // sets this current recognition
	this->generateEntites();
	this->addToTrajectory();
}

Tracker::~Tracker(){
	cv::destroyAllWindows();
}

void Tracker::setCurrentRecognition(int32_t *points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){
	this->currentRecognition = generateBoundingBoxes(points, types, confidences, size);
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

	//TODO maybe the minConfidance can be automated.

	if (this->currentRecognition.back().confidence < core::_minconfidence){
		cout << "Recognition INITIAL Size: " << currentRecognition.size() << "\n";
		vector<BoundingBox> lowThresh = splitByThreshold(this->currentRecognition, core::_minconfidence);
		cout << "Recognition AFTER Size: " << currentRecognition.size() << "\n";
	}


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

void Tracker::track(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){

	this->setCurrentRecognition(points, types, confidences, size, frame);
	for (BoundingBox& b : this->currentRecognition){
		cout << b.confidence << " ";
	}
	cout << "\n";
	this->matchEntity();
	if (visualization::_toVisualize){	
		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	this->addToTrajectory();

	cout << "\n";
}	