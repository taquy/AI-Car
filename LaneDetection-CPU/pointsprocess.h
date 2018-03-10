#ifndef POINTSPROCESS_H
#define POINTSPROCESS_H

#include <layer.h>
#include <lane.h>
int distanceTwoPoints(cv::Point &p1, cv::Point &p2);

float sloptTwoPoints(cv::Point &p1, cv::Point &p2);

void findRelatedPoint(std::vector<Layer> &layers, std::vector<cv::Point> &points, unsigned int current_layer);

void pointsToLane(std::vector<Layer> &layers, std::vector<Lane> &dst_lanes);

#endif // POINTSPROCESS_H
