#pragma once
class Detector
{
public:
    Detector();
    ~Detector();
	void debug(cv::Mat image);
	std::vector<cv::Rect> detect(cv::Mat image);
private:
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<std::vector<cv::Point>> findcontour(cv::Mat mask);
    cv::Mat findmask(cv::Mat image, cv::Scalar min, cv::Scalar max);
};

