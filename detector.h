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

        std::vector<cv::Ptr<cv::cuda::CascadeClassifier>> classifiers;


        // Distance calculate
        double w1;
        double d1;

        // Image process
        cv::Mat splitter(cv::Mat img);
        cv::Rect cropbox(cv::Mat img);

        // Scoring approximate

        int maxFrame;
        int crrFrame;
        int *score;

        // Choose detect region
        void selectRegion();

        // Limit frame detection
        int fpslimit;

    public:
      Detector();
      std::vector<cv::String> signName;
      double calcDistance (double w2);
      int getID(cv::Mat frame);
      void getProxID(cv::Mat frame);
      void reset();
      std::vector<cv::Rect> detectGpu(cv::cuda::GpuMat grayMat, int signId);
      cv::Mat draw(cv::Mat frame, std::vector<cv::Rect> boxes, cv::String label);
      void detectObject(cv::Mat frame);

      static const int STOP = 0;
      static const int LEFT = 1;
      static const int RIGHT = 2;

      static double boundbox[4];
      static int difficulty[3];

      bool isDetect;
      bool isDebug;

      int signId;
};

#endif // DETECTOR_H
