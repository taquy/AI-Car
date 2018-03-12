#include "stdafx.h"
#include "Detector.h"


Detector::Detector()
{
}


Detector::~Detector()
{
}

void Detector::debug(cv::Mat image)
{
    cv::Mat mask = findmask(image, cv::Scalar(0, 87,83), cv::Scalar(255, 255, 255));
    cv::blur(mask, mask, cv::Size(5, 5));
    std::vector<std::vector<cv::Point>> hull = findcontour(mask);
    for (int i = 0; i < hull.size(); ++i)
    {
	   cv::Rect current_rect = cv::boundingRect(hull[i]);
	   long ratio = current_rect.size().width / current_rect.size().height;
	   if (current_rect.area() > 1000)
	   {
		  drawContours(image, hull, i, cv::Scalar(255, 255, 120), 2, 8, hierarchy, 0, cv::Point());
	   }
    }
    cv::imshow("mask", mask);
    cv::imshow("frame", image);
}

std::vector<cv::Rect> Detector::detect(cv::Mat image)
{
    cv::Mat mask = findmask(image, cv::Scalar(107, 87, 83), cv::Scalar(255, 255, 255));
    cv::blur(mask, mask, cv::Size(5, 5));
    std::vector<cv::Rect> rectangles;
    std::vector<std::vector<cv::Point>> hull = findcontour(mask);
    for (int i = 0; i < hull.size(); ++i)
    {
	   cv::Rect current_rect = cv::boundingRect(hull[i]);
	   long ratio = current_rect.size().width / current_rect.size().height;
	   if (current_rect.area() > 150)
	   {
		  rectangles.push_back(cv::boundingRect(hull[i]));
	   }
    }
    return rectangles;
}

std::vector<std::vector<cv::Point>> Detector::findcontour(cv::Mat mask)
{
    
    findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<std::vector<cv::Point>> hull(contours.size());


    for (int i = 0; i < contours.size(); i++)
    {
	   convexHull(cv::Mat(contours[i]), hull[i], false);
    }
    return hull;
}


cv::Mat Detector::findmask(cv::Mat image, cv::Scalar min, cv::Scalar max)
{
    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_RGB2HSV);
    cv::Mat mask;
    cv::inRange(hsv, min, max, mask);
    return mask;
}
