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
	vector<int> matchingIndexes(currentRecognition.size(), -1);
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
        vector<uint16_t> sortedIndexes; //boxes in the currentRecogintion, that are sorted by distance to the box of current entity box
		currentEntity.predictPossibleLocations();
        currentEntity.getPossibleLocation().draw(this->frame, CV_RGB(255, 255, 255));
        for (uint16_t i = 0; i < currentRecognition.size(); i++){
			BoundingBox& box = currentRecognition[i];
			if (currentEntity.predictionContains(box)){
 				auto it = std::lower_bound(sortedIndexes.begin(), sortedIndexes.end(), box,
				[&](uint16_t index, const BoundingBox& element) {
					return element.rect.squareDistanceTo(currentEntity.getBoundingBox().rect) < 
					currentRecognition[sortedIndexes[index]].rect.squareDistanceTo(currentEntity.getBoundingBox().rect); //TODO write more readable
				});

				sortedIndexes.insert(it, i);
			}

            
        }
        if (sortedIndexes.size() > 0){
            currentEntity.setBoundingBox(currentRecognition[sortedIndexes[0]]); // set to the closet box
        	for (uint16_t i = 1; i < sortedIndexes.size(); i++){ 
				currentEntity.combineBoundingBox(currentRecognition[sortedIndexes[i]]); 
				cout << "Combined\n";
			}
			std::sort(sortedIndexes.rbegin(), sortedIndexes.rend());
			for (uint16_t index : sortedIndexes) {
				currentRecognition.erase(currentRecognition.begin() + index);
			}

        }
        else {
            currentEntity.predictNextBoundingBox();
        }
		traverse = traverse->next;
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