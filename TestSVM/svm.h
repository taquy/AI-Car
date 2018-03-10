#ifndef SVM_H
#define SVM_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <iostream>
#include <string>

void findReadColor(cv::Mat &hsv,cv::Mat &mask);

void findTraffic(cv::Mat &mask, cv::Mat &out, cv::Mat &img2);

std::string predict(cv::Mat &test, cv::Ptr<cv::ml::SVM> &svm);

std::string output(int testid);

#endif // SVM_H
