#ifndef FRAMEPROCESS_H
#define FRAMEPROCESS_H

#include <queue>
#include <iostream>
#include <road.h>
#include <string>

double alluse(double angle);
double processImg(cv::cuda::GpuMat &src, std::queue<Road> &road_q);
int testVideo(std::string filename);

#endif // FRAMEPROCESS_H
