#include "stdafx.h"
#include "DepthChecker.h"


DepthChecker::DepthChecker()
{
}


DepthChecker::~DepthChecker()
{
}

ScreenObj DepthChecker::getObject(cv::Mat depth, int min, int max)
{
    getmask(depth, min, max);
    return ScreenObj(findBigestRect(), depth);
}

long DepthChecker::gray_to_distance(int value)
{
    long distance = 0;
    long flag; //moc khoang cach thuc
    long pi;
    int top;
    int bot;
    if (value < 64)
    {
	   flag = 80;
	   pi = 1.3;
	   top = 67;
	   bot = 64;
	   distance = flag - pi * (bot - value);
    }
    else if (value >= 64 && value <= 95)
    {
	   flag = 80;
	   pi = 1.3;
	   top = 95;
	   bot = 64;
	   distance = flag + pi * (value - bot);
    }else if (value>95 && value<=133)
    {
	   flag = 120;
	   pi = 1.3;
	   top = 67;
	   bot = 64;
	   distance = flag + pi * (value - 95);
    }
    return distance;
}

void DepthChecker::debug(cv::Mat depth, int min, int max)
{
    getmask(depth, min, max);
    std::vector<std::vector<cv::Point> > hull = findhull();
    cv::Rect bigest = findBigestRect();
    cv::Mat rgb = depth.clone();
    cv::cvtColor(depth, rgb, cv::COLOR_GRAY2RGB);
    for (int i = 0; i < contours.size(); i++)
    {
	   //	   drawContours(rgb, hull, i, color, 2, 8, hierarchy, 0, cv::Point());
    }
    ScreenObj screen_obj = ScreenObj(bigest, depth);
//    cv::Point center = cv::Point(int(depth.size().width / 2), int(depth.size().height / 2));
    cv::Point center = cv::Point();
    cv::circle(rgb, center, 50, cv::Scalar(255, 255, 120), 2, 8);
    std::string text = "d : ";
    long gray_value = screen_obj.getdistance();
    text.append(std::to_string(gray_to_distance(gray_value)));
    text.append(std::string(" cm"));
    cv::putText(rgb, text, cv::Point2f(cv::Point(center.x - 30, center.y)), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 255, 0, 0), 2);
    rectangle(rgb, bigest.tl(), bigest.br(), cv::Scalar(255, 255, 120), 2);
    cv::imshow("debug", rgb);
}

std::vector<std::vector<cv::Point>> DepthChecker::findhull()
{
    findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    std::vector<std::vector<cv::Point>> hull(contours.size());


    for (int i = 0; i < contours.size(); i++)
    {
	   //	   if contourArea(contours[i])>     
	   convexHull(cv::Mat(contours[i]), hull[i], false);
    }
    return hull;
}

cv::Rect DepthChecker::findBigestRect()
{
    double maxsize = 0;
    cv::Rect bigest;
    std::vector<std::vector<cv::Point> > hull = findhull();
    for (int i = 0; i < hull.size(); ++i)
    {
	   cv::Rect current_rect = cv::boundingRect(hull[i]);
	   long ratio = current_rect.size().width / current_rect.size().height;
	   if (current_rect.area() > maxsize&&ratio < 3)
	   {
		  bigest = cv::boundingRect(hull[i]);
		  maxsize = cv::boundingRect(hull[i]).area();
	   }
    }
    return bigest;
}

void DepthChecker::getmask(cv::Mat depth, int min, int max)
{
    cv::Mat1b mask1;
    inRange(depth, cv::Scalar(min), cv::Scalar(max), mask1);
    cv::Mat denoised;
    medianBlur(mask1, denoised, 5);
    mask = denoised;
}
