#include "stdafx.h"
#include "ScreenObj.h"
#pragma once
class DepthChecker
{
public:
    DepthChecker();
    ~DepthChecker();
    ScreenObj getObject(cv::Mat depth, int min = 10, int max = 110);
    long gray_to_distance(int value);
    void debug(cv::Mat depth, int min = 10, int max = 110);
private:
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat mask;
    cv::Scalar color = cv::Scalar(0, 125, 0);
    std::vector<std::vector<cv::Point> > findhull();
    cv::Rect findBigestRect();
    void getmask(cv::Mat depth, int min, int max);
};

