#ifndef DETECTOR_H
#define DETECTOR_H

class TrafficSignDetector;

#include <iostream>

using namespace std;

#include <opencv2/highgui.hpp>

#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>

class Detector
{

    private:
        std::vector<cv::String> signName;
        std::vector<cv::Ptr<cv::cuda::CascadeClassifier>> classifiers;
        int signId;

        // Distance calculate
        double w1;
        double d1;

        // Image process

        cv::Mat splitter(cv::Mat img);
        cv::Rect cropbox(cv::Mat img);

    public:
      Detector();
      double calcDistance (double w2);
      int getID(cv::Mat frame);
      std::vector<cv::Rect> detectGpu(cv::cuda::GpuMat grayMat, int signId);
      cv::Mat draw(cv::Mat frame, std::vector<cv::Rect> boxes, cv::String label);

      static const int STOP = 0;
      static const int LEFT = 1;
      static const int RIGHT = 2;

      static double boundbox[4];
      static int difficulty[3];

};

#endif // DETECTOR_H
