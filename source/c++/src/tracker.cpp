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

	// cout << "Recognition Size: " << currentRecognition.size() << "\n";


    std::shared_ptr<Node<Entity>> traverse = this->entities.start;
	while (traverse != nullptr){
        Entity& currentEntity = traverse->item;
		bool foundPrediction = false;
		if (currentRecognition.size() > 0) {
			vector<std::pair<float32, uint16_t>> scores; scores.reserve(currentRecognition.size() - 1);
			currentEntity.predictPossibleLocations();
			for (uint16_t boxIndex = 0; boxIndex < currentRecognition.size(); boxIndex++) {
				const BoundingBox& e = currentRecognition[boxIndex];
				scores.push_back({currentEntity.clacScore(e), boxIndex});
			}
			std::sort(scores.begin(), scores.end(), [](std::pair<float32, uint16_t> s1, std::pair<float32, uint16_t> s2) { 
				return s1.first > s2.first;
			});
			cout << "scores: [";
			for (uint16_t i = 0; i < currentRecognition.size(); i++) {
				cout << scores[i].first << ", ";
			}
			cout << "]\n";
				
			if (scores[0].first > 0){
				currentEntity.setBoundingBox(currentRecognition[scores[0].second]); // set to the closet box
				currentRecognition.erase(currentRecognition.begin() + scores[0].second);
				foundPrediction = true;
			}
		}
        if (!foundPrediction){
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

	cout << "\n";
}	