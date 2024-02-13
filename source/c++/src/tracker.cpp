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
		// cout << "recognition " << i << "\ncurrentScore: ";
    	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
		while (traverse != nullptr){
			Entity& currentEntity = traverse->item;
			float32 currentScore = currentEntity.clacScore(currentRecognition[i]);
			// cout << currentScore << " ";
			if (currentScore > maxScore) { maxScore = currentScore; matchedEntityPtr = traverse; }
			// matchedBox.combinedBoundingBox(currentRecognition[i]);
			// 	currentRecognition.erase(currentRecognition.begin() + i);
			// } //TODO make sure that the dup dont have a high score anywhere else
			traverse = traverse->next;
		}
		// cout << "\nmaxScore: " << maxScore << "\n";

		if (maxScore > core::_minScore && matchedEntityPtr != nullptr){
			Entity& matchedEntity = matchedEntityPtr->item;
			if (matchedEntity.foundRecognition){
				matchedEntity.setBoundingBox(currentRecognition[i].combinedBoundingBox(matchedEntity.getBoundingBox()));
			} 
			else {
				matchedEntity.setBoundingBox(currentRecognition[i]);
			}
			matchedEntity.foundRecognition = true;
			currentRecognition.erase(currentRecognition.begin() + i);
		} 	
	}
	for (BoundingBox& b : currentRecognition){
		b.rect.draw(this->frame, CV_RGB(0, 0, 0));
		cout << "EXTRA\n";
		std::shared_ptr<Node<Entity>> traverse = this->entities.start;
		cout << "scores: ";
		while (traverse != nullptr){
			Entity& currentEntity = traverse->item;
			float32 currentScore = currentEntity.clacScore(b);
			cout << currentScore << " ";
			traverse = traverse->next;
		}
	}	
	
}

void Tracker::generateEntites(){

	for (BoundingBox& box : currentRecognition){
        this->entities.append(Entity(box));
	}

}

void Tracker::startCycle(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){
	this->setCurrentRecognition(points, types, confidences, size, frame);
	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
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

void Tracker::endCycle(){
	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
		Entity& currentEntity = traverse->item;
		if (visualization::_toVisualize){	
			currentEntity.draw(this->frame);
			currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
		}
		currentEntity.addToTrajectory();
		if (currentEntity.foundRecognition){
			currentEntity.foundRecognition = false;
		} 
		else {
			currentEntity.predictNextBoundingBox();
		}
		traverse = traverse->next;
	}
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