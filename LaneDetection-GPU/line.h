#ifndef LINE_H
#define LINE_H

#include "opencv2/highgui/highgui.hpp"

#define ESP 1e-8

// Line class declaration
class Line{
public:
    float slope;
    cv::Point start;
    cv::Point end;
    cv::Point anchor;

    long long A, B, C;
    Line();

    Line(int, int, int, int);

    Line(cv::Point, cv::Point);

    Line(float, cv::Point);

    void lineTransfrom();

    // toString
    friend std::ostream& operator << (std::ostream& out, const Line line){
        out << "(" << line.start << " , " << line.end << ")" << std::endl;
        return out;
    }

private:
};

#endif // LINE_H
