#include "detector.h"
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

using namespace std;
using namespace cv;


String modeldir = "/home/ubuntu/Desktop/LaneDetection/models/";

String stop_model = modeldir + "stop.xml";
String left_model = modeldir + "left.xml";
String face_model = modeldir + "cat.xml";

CascadeClassifier stop_classifier;
CascadeClassifier left_classifier;
CascadeClassifier face_classifier;

RNG rng(12345);

vector<String> Detector::signName = {"STOP", "LEFT", "RIGHT"};
int Detector::signId = -1;

int stop_difficult = 13;
int left_difficult = 17;

// Distance calculate
double w1 = (double) 170; // px
double d1 = (double) 20; // cm

double calcDistance (double w2) {
    return (w1 * d1) / w2;
}

int Detector::getID(Mat gpuFrame, double &distance, cv::Ptr<cv::cuda::CascadeClassifier> gpuCascade) {
//    std::vector<cv::Rect> found;

//    Mat gpuFrame = frame.clone();


     cv::cuda::GpuMat gpuMat, grayMat, gpuFound;

     gpuMat.upload(gpuFrame);
     cv::cuda::cvtColor(gpuMat, grayMat, cv::COLOR_BGR2GRAY);

     gpuCascade->detectMultiScale(grayMat, gpuFound);
//     gpuCascade->convert(gpuFound, found);


//     for (cv::Rect r : found) {
//         r.x += cvRound(r.width*0.1);
//         r.width = cvRound(r.width*0.8);
//         r.y += cvRound(r.height*0.06);
//         r.height = cvRound(r.height*0.9);
//         cv::rectangle(gpuFrame, r.tl(), r.br(), cv::Scalar(0, 0, 255), 2);
//     }
//     imshow("GPU", gpuFrame);


}


vector<Rect> Detector::detect(Mat frame, CascadeClassifier classifier, int difficulty) {

    vector<Rect> boxes;
    classifier.detectMultiScale(frame, boxes, 1.1, difficulty, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

    return boxes;
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
