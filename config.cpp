#include <config.h>
#include <iostream>
namespace conf {
    int WIDTH = 640;
    int HEIGHT = 480;

    // Region of lane
    int X_CUT = 0;
    int Y_CUT = 180;
    int Y_BOT_OFFSET = 0;
    int W_CUT = WIDTH;
    int H_CUT = HEIGHT - Y_CUT - Y_BOT_OFFSET;

    // Region of birdview which will be taked from region of lane
    int W_ROI = 140;
    int X_ROI = (int)(W_CUT - W_ROI)/2;
    int Y_ROI = 0;
    int H_ROI = H_CUT - Y_ROI;


    // config for image processing
    int THRES = 200;
    int NUMLAYERS = 25;
    int MIN_AREA_CNT = 20;

    int DOWN_Y = 60;

    // Region of layer
    int H_LAYER = (int)(H_ROI / NUMLAYERS);
    int W_LAYER = W_ROI;


    int MIN_DIS_TWO_POINTS = H_LAYER*3;

    // Point center of region's bird
    int X_CENTER = (int) (W_ROI/2);
    int Y_CENTER = H_ROI;


    int MIN_POINTS = 5;

    // Distace between two line, use for processing one lane;
    int DIS_GEN_LINE_TOP = 30;
    int DIS_GEN_LINE_BOT = 35;
    // distance between Intersection and lowest line, use for prosseing road injection
    int DIS_ROAD_INJUCTION = 150;


    // speed of car
    int SPEED = 40;
    int LEFT = 1;
    int DEBUG = 0;
    int WAIT_KEY = 1;
    int MIN_Y_LANE = 0;
    int NGABA = 0;
    int WRITE_VIDEO = 0;
        // when run detect
    int isDetect = 1;
}

namespace imp {
    cv::Rect roi(conf::X_CUT, conf::Y_CUT, conf::W_CUT, conf::H_CUT);
    cv::Rect roiCut(conf::X_ROI, conf::Y_ROI, conf::W_ROI, conf::H_ROI);
    cv::Mat matrixWrap;
}


namespace refer{
    double A = 200;
    double O = 0;
    double B = -200;
    double a = 16;
    double b = -16;
}

void init(){

    double alpha = 12, f = 1500, dist = 205, y = 70;
    getMatrixWrap(imp::matrixWrap, conf::W_CUT, conf::H_CUT, alpha, f, dist, y);
}
