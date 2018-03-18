#ifndef DETECTORSVM_H
#define DETECTORSVM_H

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect.hpp>
#include <dirent.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;
using namespace ml;
using namespace std;


class DetectorSVM
{
private:
    Ptr<SVM> svm;
    string model;  // model file
    string data;  // data train folder
    string sample; // seeding file

    vector<string> labels; // labels

    void load();

public:
    int isDebug = 1;
    int isTrain = 0;

    DetectorSVM();

    void filter(Mat &hsv,Mat &mask);

    Rect pooling(Mat &mask, Mat &out, Mat &gray);

    int predict(Mat &test);

    string label(int &id);

    int train();

    int detect(Mat &img);

    Mat draw(Mat frame, vector<Rect> boxes, String label);

    void lsdirs(string path, vector<string> &folders);

    void lsfiles(string path, vector<string> &files);

    void setLabels(string labels[]);
};


#endif // DETECTORSVM_H
