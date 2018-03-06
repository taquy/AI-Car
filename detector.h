#ifndef DETECTOR_H
#define DETECTOR_H


class Detector;

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <vector>

class Detector
{
    public:
      static void detect(cv::Mat frame);
      static void draw(cv::Mat frame, std::vector<cv::Rect> boxes);
};

#endif // DETECTOR_H


