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
	this->setFrame(frame);
}

void Tracker::setFrame(uint8_t* frame){
	this->frame = cv::Mat(this->rows, this->cols, CV_8UC3, frame);	
}

void Tracker::drawEntities(){
	std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
		traverse->item.draw(this->frame);
		traverse->item.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
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


    std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
		currentEntity.predictPossibleLocations();
		BoundingBox matchedBox = BoundingBox();
		cout << "Entity: \n";
		for (int16_t i = currentRecognition.size()-1; i >= 0; i--) {
			if (currentEntity.clacScore(currentRecognition[i]) >= core::_minScore){
				matchedBox = matchedBox.combinedBoundingBox(currentRecognition[i]);
				currentRecognition.erase(currentRecognition.begin() + i);
			}
		}
		cout << "\n";
        if (!(matchedBox.isEmpty())){
            currentEntity.setBoundingBox(matchedBox);
        } 
		else {
			currentEntity.predictNextBoundingBox();
		}
		traverse = traverse->next;
    }
	for (BoundingBox& b : currentRecognition){
		b.rect.draw(this->frame, CV_RGB(0, 0, 0));
		cout << "EXTRA\n";
	}
}	

void Tracker::generateEntites(){

	for (BoundingBox& box : currentRecognition){
        this->entities.append(Entity(box));
	}

}

void Tracker::track(int32_t* points, uint16_t* types, float32* confidences, uint16_t size, uint8_t* frame){

	this->setCurrentRecognition(points, types, confidences, size, frame);

	this->matchEntity();
	if (visualization::_toVisualize){	
		this->drawEntities();
		cv::imshow("frame", this->frame);
		cv::waitKey(visualization::_waitKey);
	}
	this->addToTrajectory();
}	