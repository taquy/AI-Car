#include "line.h"
#include "config.h"
#include <opencv2/highgui/highgui.hpp>

//! Init a line from two points with two Point type
Line::Line(){
    this->slope = 0.0000;
    this->c = 0;
    this->start = cv::Point();
    this->end = cv::Point();
    this->anchor = cv::Point();
}


Line::Line(double slope, cv::Point anchorPoint){
    this->slope = slope;
    this->anchor = anchorPoint;

    /*
    We want to find two points end of the line. Supose that F(x,y) is a point on the line,
    A(0, y_a) is start, B(WIDTH - 1, y_b) is end. What we need to do is find y_a and y_b.

    slope = (y_a - y)/(0 - x) = (y_b - y)/(WIDTH - x)
    slope = (hroi - ya)/(x - xa) = y/(xa - x)

    If you still confused, let visualize on graph.

    */

    int X_BOT = (int)((conf::H_ROI - anchorPoint.y)/this->slope + anchorPoint.x);
    int X_TOP = (int)(anchorPoint.x - (anchorPoint.y - conf::DOWN_Y)/this->slope);

    this->start = cv::Point(X_BOT, conf::H_ROI);
    this->end = cv::Point(X_TOP, conf::DOWN_Y);

    this->c = this->end.y - this->slope * this->end.x;
}


