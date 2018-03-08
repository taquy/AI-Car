#ifndef DETECTOR_H
#define DETECTOR_H

class Detector;

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <vector>
#include <iostream>

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;

class Detector
{

    public:
      static std::vector<String> signName;
      static int signId;

      static int getID(Mat frame, double &distance, cv::Ptr<cv::cuda::CascadeClassifier> gpuCascade);
      static std::vector<Rect> detect(Mat frame, CascadeClassifier classifier, int difficulty);
      static Mat draw(Mat frame, std::vector<Rect> boxes, String label);
};

#endif // DETECTOR_H
