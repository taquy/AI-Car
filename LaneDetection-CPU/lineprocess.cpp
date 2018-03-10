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
    long long D = l1.A * l2.B - l1.B * l2.A;
    long long Dx = l1.C * l2.B - l1.B * l2.C;
    long long Dy = l1.A * l2.C - l1.C * l2.A;

    if (cv::abs(D) < ESP){
        return false;
    } else {
        R.x = (int)(Dx/D);
        R.y = (int)(Dy/D);
        return true;
    }

}

bool checkInRegion(cv::Point &p){
    if(p.x >= 0 && p.x <= conf::W_ROI && p.y >= 0 && p.y <= conf::H_ROI){
        return true;
    }
    return false;
}

//! Some lines is defined by two points but they are may be not in a rectangle which are defined HEIGHT and WIDTH
//! Some line is infinite
//! What we want is a line whose points are on edge of the rectangle
void regionLine(Line &l){
    cv::Point R(0,0);
    int num_R = 0;

    // intersection of the line and bottom line
    bool with_bottom = intersection(R, l, imp::bottom);

    if(with_bottom && checkInRegion(R)){
        num_R++;
        l.start.x = R.x;
        l.start.y = R.y;
    }

    // intersection of the line and left line
    bool with_left = intersection(R, l, imp::left);

    if( with_left && checkInRegion(R)){
        if(num_R == 1){
           l.end.x = R.x;
           l.end.y = R.y;
        } else {
           l.start.x = R.x;
           l.start.y = R.y;
        }

        num_R++;
    }

    // intersection of the line and right line
    if(num_R < 2){
        bool with_right = intersection(R, l, imp::right);
        if( with_right && checkInRegion(R)){
            if(num_R == 1){
               l.end.x = R.x;
               l.end.y = R.y;
            } else {
               l.start.x = R.x;
               l.start.y = R.y;
            }

            num_R++;
        }
    }

    // intersection of the line and top line
    if(num_R < 2){
        bool with_top = intersection(R, l, imp::top);
        if( with_top && checkInRegion(R)){
            l.end.x = R.x;
            l.end.y = R.y;

        }
    }

}
