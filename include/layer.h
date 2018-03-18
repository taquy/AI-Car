#ifndef LAYER_H
#define LAYER_H

#include "opencv2/highgui/highgui.hpp"

class Layer{
  public:
    cv::Mat img;
    cv::Mat mask;
    std::vector<cv::Point> points;
    int HEIGHT, WIDTH, X, Y;

    Layer();

    Layer(cv::Mat &, int, int, bool);

    ~Layer();

    void toOrigin();
};

#endif // LAYER_H
