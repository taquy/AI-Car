#ifndef LINEPROCESS_H
#define LINEPROCESS_H
#include <line.h>

bool intersection(cv::Point &R, Line &l1, Line &l2);
bool checkInRegion(cv::Point &p);

#endif // LINEPROCESS_H
