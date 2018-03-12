#pragma once
class ScreenObj
{
public:

    ScreenObj(cv::Rect location, cv::Mat image)//mat image la con tro
	   ;

    /**
    * \tim khoang cach tu xe den object,
    * 1. cat object in side box ( 40% ben trong)
    * 2. get distance via depth image
    *
    */
    long getdistance();


private:
    cv::Rect location;

    cv::Mat image;
    long balance_pixel(int number_pixels = 10);
};

