#include "lane.h"

Lane::Lane(){
    this->line = Line();
}
Lane::Lane(std::vector<cv::Point> contour){
    this->cnt = contour;

    cv::Vec4f line;
    cv::fitLine(this->cnt, line, CV_DIST_L2, 0, 0.01, 0.01);

    if(line[0] < ESP_VX){
        line[0] = (float)ESP_VX;
    }

    this->line = Line( line[1]/line[0], cv::Point((int)line[2], (int)line[3]));

}
