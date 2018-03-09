#ifndef DETECTOR_H
#define DETECTOR_H

class TrafficSignDetector;

#include <iostream>

using namespace std;

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
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
#include <thread>

#include <opencv2/gpu/gpu.hpp>


using namespace cv;

class Detector
{
    private:
        vector<String> signName;
        vector<cv::Ptr<cv::cuda::CascadeClassifier>> classifiers;
        int signId;

        int stop_difficult;
        int left_difficult;

        // Distance calculate
        double w1;
        double d1;

    public:
      Detector();
      double calcDistance (double w2);
      int getID(Mat frame);
      std::vector<cv::Rect> detectGpu(cv::cuda::GpuMat grayMat, int signId);
      Mat draw(Mat frame, std::vector<Rect> boxes, String label);
};

#endif // DETECTOR_H
