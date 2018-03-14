#ifndef LINE_H
#define LINE_H

#include "opencv2/highgui/highgui.hpp"

#define ESP 1e-8

// Line class declaration
class Line{
public:
    double slope;

    // y = slope*x + c
    double c;

    cv::Point start;
    cv::Point end;
    cv::Point anchor;

    Line();

    Line(double, cv::Point);
    // toString
    friend std::ostream& operator << (std::ostream& out, const Line line){
        out << "(" << line.start << " , " << line.end << ")" << std::endl;
        return out;
    }
};

#endif // LINE_H
