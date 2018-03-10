
#include <iostream>
#include <stdio.h>


using namespace std;


#include "detector.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

String modeldir = "/home/ubuntu/Desktop/CDS/LaneDetection-GPU/models/";

String stop_model = modeldir + "stop.xml";
String left_model = modeldir + "left.xml";
String right_model = modeldir + "left.xml";

RNG rng(12345);

Detector::Detector() {
    this->signName = {"STOP", "LEFT", "RIGHT"};
    this->classifiers = {
        cv::cuda::CascadeClassifier::create(stop_model),
        cv::cuda::CascadeClassifier::create(left_model),
        cv::cuda::CascadeClassifier::create(right_model)
    };
    this->signId = -1;

    this->stop_difficult = 13;
    this->left_difficult = 17;

    // Distance calculate
    this->w1 = (double) 170; // px
    this->d1 = (double) 20; // cm

}


double Detector::calcDistance (double w2) {
    return (this->w1 * this->d1) / w2;
}



std::vector<cv::Rect> Detector::detectGpu(cv::cuda::GpuMat grayMat, int signId) {
    grayMat = grayMat.clone();
    cv::Ptr<cv::cuda::CascadeClassifier> gpuCascade = this->classifiers[signId];

    std::vector<cv::Rect> boxes;

    cv::cuda::GpuMat gpuFound;


    gpuCascade->setMinNeighbors(15);
    gpuCascade->detectMultiScale(grayMat, gpuFound);
    gpuCascade->convert(gpuFound, boxes);


    return boxes;
}


int Detector::getID(Mat frame) {

    cv::cuda::GpuMat gpuMat, grayMat;
    gpuMat.upload(frame.clone());
    cv::cuda::cvtColor(gpuMat, grayMat, cv::COLOR_BGR2GRAY);
//        cv::cuda::histEven(gpuFrame, gpuFrame);

//    int n = this->signName.size();
//    std::vector<cv::Rect> boxes = this->detectGpu(grayMat, 0);
//    frame = this->draw(frame, boxes, this->signName[0]);


    std::vector<cv::Rect> boxes = this->detectGpu(grayMat, 0);
//    frame = this->draw(frame, boxes, this->signName[0]);
//    cv::imshow("result", frame);

    return -1;
}

Mat Detector::draw(Mat frame, vector<Rect> boxes, String label) {
    for( size_t i = 0; i < boxes.size(); i++ )
    {
        Point a(boxes[i].x, boxes[i].y);
        Point b(boxes[i].x + boxes[i].width, boxes[i].y + boxes[i].height);

        rectangle(frame, a, b, Scalar(0, 255, 0), 3);

        putText(frame, label, a, FONT_HERSHEY_SIMPLEX, 1, (0, 125, 255), 3, 0, false);
    }
    return frame;
}
