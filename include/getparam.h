#ifndef READCAM_H
#define READCAM_H

#include <opencv2/highgui/highgui.hpp>

void init();
void getMatrixWrap(cv::Mat &dst, double w, double h, double alpha,
                   double focalLength, double dist, double y);



#endif // READCAM_H
