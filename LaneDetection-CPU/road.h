#ifndef ROAD_H
#define ROAD_H

#include <lane.h>
#include <opencv2/highgui/highgui.hpp>
#include <config.h>

class Road{
public:
    cv::Point pointBot;
    cv::Point pointTop;
    int widthBot;
    int widthTop;
    Lane right;
    Lane left;

    int num_lanes = 0;

    Road();

    void setRight(Lane lane);

    void setLeft(Lane lane);

};

#endif // ROAD_H
