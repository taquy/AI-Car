#ifndef DEBUG_H
#define DEBUG_H

#include <opencv2/highgui/highgui.hpp>
#include "road.h"

void draw(cv::Mat &src, cv::Mat &roi, cv::Mat &bird, Road &road, std::vector<Lane> lanes);

#endif // DEBUG_H
