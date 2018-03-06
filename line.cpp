#include <line.h>
#include <config.h>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//! Init a line from two points with two Point type
Line::Line(){
    slope = 0.f;
    start = cv::Point(0,0);
    end = cv::Point(0, 0);
    anchor = cv::Point(0,0);
    A = 0;
    B = 0;
    C = 0;
}

//! Init line from two points with four cordinates
Line::Line(int x_A, int y_A, int x_B, int y_B){

    if(y_A < y_B){
        this->start = cv::Point(x_B, y_B);
        this->end = cv::Point(x_A, y_A);
    } else {
        this->start = cv::Point(x_A, y_A);
        this->end = cv::Point(x_B, y_B);
    }

    this->lineTransfrom();
}


// Init a line from its slope and an anchor point
Line::Line(cv::Point p1, cv::Point p2){
    // end is always higher than start
    if(p1.y < p2.y){
        this->start = p2;
        this->end = p1;
    } else {
        this->start = p1;
        this->end = p2;
    }

    // calculate slope
//    slope = getTheta(this->start, this->end);

    // convert to equation
    this->lineTransfrom();
}


Line::Line(float slope, cv::Point anchorPoint){
    this->slope = slope;
    this->anchor = anchorPoint;

    /*
    We want to find two points end of the line. Supose that F(x,y) is a point on the line,
    A(0, y_a) is start, B(WIDTH - 1, y_b) is end. What we need to do is find y_a and y_b.

    slope = (y_a - y)/(0 - x) = (y_b - y)/(WIDTH - x)

    If you still confused, let visualize on graph.

    */

    int lefty = (int)((-anchorPoint.x * this->slope) + anchorPoint.y);
    int righty = (int)((conf::WIDTH - anchorPoint.x)*this->slope + anchorPoint.y);

    if(lefty > righty){
        this->start = cv::Point(0, lefty);
        this->end = cv::Point(conf::WIDTH, righty);
    } else {
        this->end = cv::Point(0, lefty);
        this->start = cv::Point(conf::WIDTH, righty);
    }

    // ensure that the start point is not points of bottom corners;
    if(cv::abs(this->start.y - conf::H_ROI) < 10){
        this->start.y = conf::H_ROI - 10;
    }

    if(cv::abs(this->start.y) < 4){
        this->start.y = 4;
    }

    // convert to equation
    this->lineTransfrom();
}

//! Convert line to linear equation
//! Ax + By = C

void Line::lineTransfrom(){

    this->A = (long)start.y - (long)end.y;
    this->B = (long)end.x - (long)start.x;
    this->C = -((long)start.x * (long)end.y - (long)start.y * (long)end.x);

}

