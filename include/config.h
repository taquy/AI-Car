#ifndef CONFIG_H
#define CONFIG_H

#include <opencv2/highgui/highgui.hpp>
namespace conf {
    extern int WIDTH;
    extern int HEIGHT;

    // Region of lane
    extern int X_CUT;
    extern int Y_CUT;
    extern int Y_BOT_OFFSET;
    extern int W_CUT;
    extern int H_CUT;

    // Region of birdview which will be taked from region of lane
    extern int W_ROI;
    extern int X_ROI;
    extern int Y_ROI;
    extern int H_ROI;


    // config for image processing
    extern int THRES;
    extern int NUMLAYERS;
    extern int MIN_AREA_CNT;

    extern int DOWN_Y;

    // Region of layer
    extern int H_LAYER;
    extern int W_LAYER;


    extern int MIN_DIS_TWO_POINTS;

    // Point center of region's bird
    extern int X_CENTER;
    extern int Y_CENTER;


    extern int MIN_POINTS;

    // Distace between two line, use for processing one lane;
    extern int DIS_GEN_LINE_TOP;
    extern int DIS_GEN_LINE_BOT;

    // distance between Intersection and lowest line, use for prosseing road injection
    extern int DIS_ROAD_INJUCTION;


    // speed of car
    extern int SPEED;
    extern int LEFT;
    extern int DEBUG;
    extern int WAIT_KEY;

    extern int NGABA;
    extern int WRITE_VIDEO;
}

namespace imp {
    extern int ALPHA;
    extern int F;
    extern int DIST;
    extern int Y;

    extern cv::Rect roi;
    extern cv::Rect roiCut;
    extern cv::Mat matrixWrap;
}

namespace refer{
    extern double A;
    extern double O;
    extern double B;
    extern double a;
    extern double b;
}

#endif // CONFIG_H

