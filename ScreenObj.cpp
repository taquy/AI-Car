#include "stdafx.h"
#include "ScreenObj.h"

ScreenObj::ScreenObj(cv::Rect location, cv::Mat image)
{
    this->location = location;
    this->image = image;
}

long ScreenObj::getdistance()
{
    long balance = balance_pixel();

    return balance;
}

long ScreenObj::balance_pixel(int number_pixels)
{
    int x = int(location.x + location.width * 0.3);
    int y = int(location.y + location.height * 0.3);
    int width = int(location.width * 0.3);
    int height = int(location.height * 0.3);
    cv::Rect inside = cv::Rect(x, y, width, height);
    cv::Mat crop = image(inside);
    int sumvalue = 0;
    if (crop.size[0] == 0 || crop.size[1] == 0)
	   return 0;
    for (int i = 0; i < number_pixels; ++i)
    {
	   int rand_x = std::rand() % (crop.size[0]);
	   int rand_y = std::rand() % (crop.size[1]);
	   sumvalue += crop.at<uchar>(rand_x, rand_y);
    }
    return sumvalue / number_pixels;
}

