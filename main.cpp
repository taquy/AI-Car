#include "detector.h"
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

#include "opencv2/core/core.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaarithm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <X11/Xlib.h>

#include <opencv2/gpu/gpu.hpp>

Mat imgsplitter(Mat img, double percentage) {
    double height = img.rows * (percentage / (double) 100) - 1;
    return img(cv::Range(0, height), cv::Range(0, img.cols - 1));
}


double st = 0, et = 0, fps = 0;
double freq = getTickFrequency();

int main (int argc, char* argv[])
{
    VideoCapture cap(0);
    Detector *detector = new Detector();

    while (1) {
        st = getTickCount();


        Mat frame;
        cap >> frame;

        if (detector->isDetect) {
            detector->getProxID(frame);
        } else {
            cv::imshow("done", frame);
        }


        et = getTickCount();
        fps = 1.0 / ((et-st) / freq);
//        cerr << "FPS: "<< fps<< '\n';
        if (waitKey(1) >= 0) break;

    }
    return 0;
}
