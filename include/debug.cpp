#include "debug.h"
#include "lane.h"
#include "config.h"
#include "road.h"

void draw(cv::Mat &src, cv::Mat &roi, cv::Mat &bird, Road &road, std::vector<Lane> lanes){

    cv::Mat mask = cv::Mat::zeros(conf::H_ROI, 2*conf::W_ROI, CV_8UC3);
    for(Lane &lane : lanes){
        for(cv::Point &p : lane.cnt){
            cv::circle(mask, p, 5,cv::Scalar(0,255,255),-1);
        }
        cv::circle(mask, lane.line.start, 5,cv::Scalar(255,255,0),3);
        cv::circle(mask, lane.line.end, 5,cv::Scalar(255,255,255),3);
        cv::circle(mask, lane.line.anchor, 5,cv::Scalar(255,255,255),3);
    }

    if(road.hasRight){
        for(cv::Point &p : road.right.cnt){
            cv::circle(mask, p, 5,cv::Scalar(0,0,255),-1);
        }
        line(mask, road.right.line.start, road.right.line.end, cv::Scalar(0,255,0),2);
    }

    if(road.hasLeft){
        for(cv::Point &p : road.left.cnt){
            cv::circle(mask, p, 5,cv::Scalar(0,0,255),-1);
        }
        line(mask, road.left.line.start, road.left.line.end, cv::Scalar(0,0,255),2);
    }

    cv::line(mask, road.pointBot, road.pointTop, cv::Scalar(0,255,255), 6);

    cv::Rect r(conf::W_ROI, 0, conf::W_ROI, conf::H_ROI);
    cv::Mat R;
    R = cv::Mat(mask, r);

    cv::cvtColor(bird, bird, cv::COLOR_GRAY2BGR);
    bird.copyTo(R);

    cv::Mat mask2 = cv::Mat::zeros(conf::H_ROI + conf::H_CUT, conf::W_CUT, CV_8UC3);
    r = cv::Rect(0,0, mask.size().width, mask.size().height);

    R = cv::Mat(mask2, r);
    mask.copyTo(R);

    r = cv::Rect(0,conf::H_ROI, mask2.size().width, conf::H_CUT);
    R = cv::Mat(mask2, r);
    roi.copyTo(R);

    int H_MASK_NEW = 320;
    int W_MASK_NEW = (int)(H_MASK_NEW*mask2.size().width/mask2.size().height);

    int H_SRC_NEW = H_MASK_NEW;
    int W_SRC_NEW = (int)(H_SRC_NEW*conf::WIDTH/conf::HEIGHT);

    resize(mask2, mask2, cv::Size(W_MASK_NEW, H_MASK_NEW));
    resize(src, src, cv::Size(W_SRC_NEW, H_SRC_NEW));

    cv::Mat mask3;
    cv::hconcat(mask2, src, mask3);
    cv::imshow("debug", mask3);
}
