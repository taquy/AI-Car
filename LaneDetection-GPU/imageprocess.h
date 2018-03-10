#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>


#include <layer.h>

void separateLayers(cv::Mat &src, std::vector<Layer> &layers);

void concatenateLayers(std::vector<Layer> &layers, cv::Mat &dst);

void birdView(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst);

void roiImg(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst , cv::Rect &roi);

void findCnts(cv::Mat &src, std::vector<std::vector<cv::Point>> &contours);

void findCenterPoint(Layer &layer);

void findPoints(Layer &layer);

#endif // IMAGEPROCESS_H
