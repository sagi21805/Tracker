#include "mathUtils.hpp"

uint squareDistance(Point A, Point B){
    return pow(A.x - B.x, 2) + pow(A.y - B.y, 2);
}

uint16_t findMostOccurrence(const cv::Mat& labels, uint8_t k){
    std::vector<uint8_t> labelsVector(labels.data, labels.data + labels.total());

    std::vector<uint> labelCounts(k, 0);
    for (uint label : labelsVector) {
        labelCounts[static_cast<uint>(label)]++;
    }

    auto mostDominantCenter = std::max_element(labelCounts.begin(), labelCounts.end());

    uint16_t index = std::distance(labelCounts.begin(), mostDominantCenter);

    return index;
}