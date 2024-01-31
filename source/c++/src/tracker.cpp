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

	// cout << "Recognition Size: " << currentRecognition.size() << "\n";


    std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
        uint distanceSquared = UINT32_MAX;
		currentEntity.predictPossibleLocations();
        currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
        uint16_t matchingBoxIndex = UINT16_MAX;
        for (uint16_t i = 0; i < currentRecognition.size(); i++){
			
			BoundingBox& box = currentRecognition[i];

            if (currentEntity.getType() == box.type) { 
                uint currentDistanceSquared = currentEntity.squareDistanceTo(box.rect);
                if (currentDistanceSquared < distanceSquared && 
                    currentEntity.getPossibleLocation().contains(box.rect.center) &&
					currentEntity.getPossibleLocation().contains(box.rect.tl()) &&
					currentEntity.getPossibleLocation().contains(box.rect.br())){
                    distanceSquared = currentDistanceSquared;
					matchingBoxIndex = i;
                }
            }

            
        }
        if (matchingBoxIndex < UINT16_MAX){
            currentEntity.setBoundingBox(currentRecognition[matchingBoxIndex]);
			currentRecognition.erase(currentRecognition.begin() + matchingBoxIndex);
        }
        else {
            currentEntity.predictNextBoundingBox();
        }
		traverse = traverse->next;
    }

	traverse = this->entities.start;
	if (currentRecognition.size() > 0){
		while (traverse != nullptr){
			Entity& currentEntity = traverse->item;
			uint distanceSquared = UINT32_MAX;
			uint16_t matchingBoxIndex = UINT16_MAX;
			currentEntity.predictPossibleLocations();
			for (uint16_t i = 0; i < currentRecognition.size(); i++){
				
				BoundingBox& box = currentRecognition[i];

				if (currentEntity.getType() == box.type) { 
					uint currentDistanceSquared = currentEntity.squareDistanceTo(box.rect);
					if (currentDistanceSquared < distanceSquared && 
						currentEntity.getPossibleLocation().contains(box.rect.center) &&
						currentEntity.getPossibleLocation().contains(box.rect.tl()) &&
						currentEntity.getPossibleLocation().contains(box.rect.br()) ){
						distanceSquared = currentDistanceSquared;
						matchingBoxIndex = i;
					}
				}

				
			}
			if (matchingBoxIndex < UINT16_MAX){
				currentEntity.combineBoundingBox(currentRecognition[matchingBoxIndex]);
				currentRecognition.erase(currentRecognition.begin() + matchingBoxIndex);
				currentEntity.predictPossibleLocations();
				currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(0, 255, 0));
			}
			traverse = traverse->next;
		}
	}
	for (BoundingBox& b : currentRecognition){
		b.rect.draw(this->frame, CV_RGB(0, 0, 0));
		cout << "BADDDD\n";
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

	cout << "\n";
}	