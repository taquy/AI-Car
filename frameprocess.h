#ifndef FRAMEPROCESS_H
#define FRAMEPROCESS_H

#include <queue>
#include <iostream>
#include <road.h>
#include <string>

void alluse(double &angle);
double processImg(cv::Mat &src, std::queue<Road> &road_q);
int testVideo(std::string filename);

#endif // FRAMEPROCESS_H
