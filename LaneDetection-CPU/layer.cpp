#include <layer.h>

Layer::Layer(){
    this->HEIGHT = 0;
    this->WIDTH = 0;
    this->X = 0;
    this->Y = 0;
}

Layer::Layer(cv::Mat &src, int X_COR = 0, int Y_COR = 0, bool isDebug = false){
    this->img = src.clone();
    this->HEIGHT = img.rows;
    this->WIDTH = img.cols;

    this->X = X_COR;
    this->Y = Y_COR;

    if(isDebug){
        this->mask = cv::Mat::zeros(src.size(), CV_8UC3);
    }
}

Layer::~Layer(){

    this->img.release();
    this->mask.release();

}

void Layer::toOrigin(){
    for(cv::Point &p : this->points){
        p.x += this->X;
        p.y += this->Y;
    }
}

