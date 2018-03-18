#ifndef LANE_H
#define LANE_H

#include <iostream>
#include "line.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define ESP_VX 0.001
class Lane{
public:
    std::vector<cv::Point> cnt;
    Line line;
    Lane();
    Lane(std::vector<cv::Point> contour);
};
#endif // LANE_H
