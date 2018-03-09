#ifndef READCAM_H
#define READCAM_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>

void getMatrixWrap(cv::Mat &dst, double w, double h, int bird_alpha);



#endif // READCAM_H
