#include <lineprocess.h>
#include <iostream>
#include <config.h>

//! Calculate intersection of two line
/*!
 * \param R     output inttersection point
 * \param l1
 * \param l2
 * \return      true if intersection point exist, false if two line are parallel
 */
bool intersection(cv::Point &R, Line &l1, Line &l2){
    double D = l1.slope - l2.slope;

    if(cv::abs(D) > 0.005){
        R.x = (l2.c - l1.c)/D;
        R.y = l1.slope*R.x + l1.c;
        return true;
    } else {
        return false;
    }

}

bool checkInRegion(cv::Point &p){
    if(p.x >= 0 && p.x <= conf::W_ROI && p.y >= 0 && p.y <= conf::H_ROI){
        return true;
    }
    return false;
}
