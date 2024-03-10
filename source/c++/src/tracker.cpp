#include "tracker.hpp"

Tracker::Tracker(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame, uint16_t rows, uint16_t cols) 
	: rows(rows), cols(cols){
	config("config.json");
	this->setCurrentRecognition(points, types, confidences, size, frame); // sets this current recognition
	this->generateEntites();
}

Tracker::~Tracker(){
	cv::destroyAllWindows();
}

void Tracker::setCurrentRecognition(int32_t *points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){
	this->currentRecognition = generateBoundingBoxes(points, types, confidences, size);
	this->setFrame(frame);
}

void Tracker::setFrame(uint8_t* frame){
	this->frame = cv::Mat(this->rows, this->cols, CV_8UC3, frame);	
}


void Tracker::matchEntity(){

	for (int32_t i = currentRecognition.size()-1; i >= 0; i--) {
		float32 maxScore = 0;
		std::shared_ptr<Node<Entity>> matchedEntityPtr = nullptr;
    	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
		while (traverse != nullptr){
			Entity& currentEntity = traverse->item;
			float32 currentScore = currentEntity.clacScore(currentRecognition[i]);
			if (currentScore > maxScore) { maxScore = currentScore; matchedEntityPtr = traverse; }
			traverse = traverse->next;
		}

		if (maxScore > core::_minScore && matchedEntityPtr != nullptr){
			Entity& matchedEntity = matchedEntityPtr->item;
			if (matchedEntity.foundRecognition){
				matchedEntity.combineBoundingBox(currentRecognition[i]);
			} 
			else {
				matchedEntity.setBoundingBox(currentRecognition[i]);
			}
			matchedEntity.foundRecognition = true;
			currentRecognition.erase(currentRecognition.begin() + i);
		} 	
	}

}

void Tracker::generateEntites(){

	for (BoundingBox& box : currentRecognition){
        this->entities.prepend(Entity(box));
	}

}

void Tracker::startCycle(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){
	this->setCurrentRecognition(points, types, confidences, size, frame);
	auto traverse = this->entities.start;
	while (traverse != nullptr){
		Entity& currentEntity = traverse->item;
		currentEntity.calcAndSetVelocity();
		currentEntity.predictPossibleLocations();
		traverse = traverse->next;
	}
	if (visualization::_toVisualize){
		for (BoundingBox& b : currentRecognition){
			b.rect.draw(this->frame, CV_RGB(0, 255, 0));
		}	
	}
}

void Tracker::manageLastSeen(){
	auto traverse = &this->lastSeen.start;
	while (*traverse != nullptr){
		Entity& currentEntity = (*traverse)->item;
		

	}
	cout << "lastSeen length: " << lastSeen.length << "\n";

}


void Tracker::endCycle(){	
	
	auto traverse = &this->entities.start;
	//TODO make a remove by entry function in the linked list - test with numbers
	while (*traverse != nullptr){
		Entity& currentEntity = (*traverse)->item;
		if (visualization::_toVisualize){	
			currentEntity.draw(this->frame);
			currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
		}
		currentEntity.addToTrajectory();
		if (currentEntity.foundRecognition){ currentEntity.foundRecognition = false; currentEntity.timesNotFound = 0; } 
		else { currentEntity.predictNextBoundingBox(); currentEntity.timesNotFound++; }

		if (currentEntity.timesNotFound >= 5){
			entities.moveNode(lastSeen, traverse);
			cout << "Moved!\n";
			*traverse = (*traverse)->next;  //Remove the entity from the list.
			if (*traverse == nullptr){ break; }
		}
			
		traverse = &(*traverse)->next;
	}
	manageLastSeen();

	this->generateEntites(); //TODO make more sophisticated

	if (visualization::_toVisualize){	
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}

}
void Tracker::track(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){

	this->startCycle(points, types, confidences, size, frame);
	this->matchEntity();
	this->endCycle();
}	