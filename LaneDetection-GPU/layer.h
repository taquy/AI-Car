#ifndef LAYER_H
#define LAYER_H

#include "opencv2/highgui/highgui.hpp"
#include <opencv2/cudaarithm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
class Layer{
  public:
    cv::Mat img;
    std::vector<cv::Point> points;
    int HEIGHT, WIDTH, X, Y;

    Layer();

    Layer(cv::Mat &src, int, int);

    ~Layer();

    void toOrigin();
};

#endif // LAYER_H
