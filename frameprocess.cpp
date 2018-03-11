#include <frameprocess.h>
#include <line.h>
#include <layer.h>
#include <imageprocess.h>
#include <pointsprocess.h>
#include <laneprocess.h>

using namespace std;
using namespace cv;

// reference angle of API
double map2 (double x, int in_min, int in_max, int out_min, int out_max){
    double toReturn =  (double)(1.0 * (x - in_min) * (out_max - out_min) /
            (in_max - in_min) + out_min );
    return toReturn;
}

// reference angle
double alluse(double angle) {
    double OA = refer::A - refer::O;
    double OB = refer::O - refer::B;
    if(angle < 0) return OB/cv::abs(refer::b) * angle + refer::O;
    return OA/refer::a * angle + refer::O;
}

// get angle from two points
double getTheta(cv::Point car, cv::Point dst) {
    if (dst.x == car.x) return 0;
    if (dst.y == car.y) return (dst.x < car.x ? -90 : 90);
    double pi = acos(-1.0);
    double dx = dst.x - car.x;
    double dy = car.y - dst.y; // image coordinates system: car.y > dst.y

    double angle = 0;

    if (dx < 0){
        angle = std::atan(-dx / dy) * 180 / pi;
    } else{
        angle = -std::atan(dx / dy) * 180 / pi;
    }

    angle = alluse(angle);

    return angle;
}

// Main image processing
/*!
 * \param src       frame readed from camera
 * \param road_q    a queue which saves data (angle, left line, right line,...) of previous frame
 * \return          angle of current frame;
 */
double processImg(cv::Mat &src, std::queue<Road> &road_q){
     // prex process image

    cv::Mat roi;
    roiImg(src, roi, imp::roi);



    cv::Mat gray, thres;
    cvtColor(roi, gray, CV_BGR2GRAY);
    threshold(gray, thres, conf::THRES, 255, cv::THRESH_BINARY);

    cv::Mat b;
    birdView(thres, b);



    // process layer
    std::vector<Layer> layers;
    separateLayers(b, layers);

    for(unsigned int i = 0; i < layers.size(); i++){
        findCenterPoint(layers[i]);
    }


    // process point
    std::vector<Lane> lanes;
    pointsToLane(layers, lanes);

    std::vector<Lane> copy_lanes = lanes;

    // ngaba
    Road road;
    int hasRoadInjection = ngaba(lanes, road);

    if(!hasRoadInjection){
        // process lanes
        separateLeftRight(lanes, road);
    }

    genLine(road, road_q);
    Road new_r = road_q.front();


    // show images process to debug, don't care about alrogithm
    if(conf::DEBUG){
        Mat mask = Mat::zeros(conf::H_ROI, 2*conf::W_ROI, CV_8UC3);
        for(Lane &lane : copy_lanes){
            for(Point &p : lane.cnt){
                cv::circle(mask, p, 5,cv::Scalar(0,255,255),-1);
            }
        }

        Line *line_left = &new_r.left.line;
        if(line_left->slope != 0){
            line(mask, line_left->start, line_left->end, cv::Scalar(0,0,255),2);
        }

        Line *line_right = &new_r.right.line;
        if(line_right->slope != 0){
            line(mask, line_right->start, line_right->end, cv::Scalar(0,255,0),2);
        }


        line(mask, new_r.pointBot,new_r.pointTop,cv::Scalar(0,255,255),6);
        line(mask, Point(conf::W_ROI, 0),Point(conf::W_ROI, conf::H_ROI),cv::Scalar(255,255,255),6);
        Rect r(conf::W_ROI, 0, conf::W_ROI, conf::H_ROI);
        cv::Mat R;
        R = Mat(mask, r);

        cvtColor(b, b, cv::COLOR_GRAY2BGR);
        b.copyTo(R);

        Mat mask2 = Mat::zeros(conf::H_ROI + conf::H_CUT, conf::W_CUT, CV_8UC3);
        r = Rect(0,0, mask.size().width, mask.size().height);

        R = Mat(mask2, r);
        mask.copyTo(R);

        r = Rect(0,conf::H_ROI, mask2.size().width, conf::H_CUT);
        R = Mat(mask2, r);

        roi.copyTo(R);

        int H_MASK_NEW = 320;
        int W_MASK_NEW = (int)(H_MASK_NEW*mask2.size().width/mask2.size().height);

        int H_SRC_NEW = H_MASK_NEW;
        int W_SRC_NEW = (int)(H_SRC_NEW*conf::WIDTH/conf::HEIGHT);

        resize(mask2, mask2, Size(W_MASK_NEW, H_MASK_NEW));
        resize(src, src, Size(W_SRC_NEW, H_SRC_NEW));

        Mat mask3;
        hconcat(mask2, src, mask3);
        imshow("debug", mask3);

    }

    src = b;
    layers.clear();
    lanes.clear();

    return getTheta(new_r.pointBot, new_r.pointTop);
}

// test image processing from video
int testVideo(string filename){

    cv::VideoCapture video(filename);

    if(!video.isOpened()){
        cout << "OPEN VIDEO FAILED!" << endl;
        return -1;
    }

    namedWindow("frame");
    double total_time = 0;
    int num_frames = 0;
    queue<Road> road_q;

    road_q.push(Road());

    double freq = getTickFrequency();
    double st = 0, et = 0, fps = 0;
    double sum_fps = 0;

    while(true){
        st = getTickCount();

        Mat frame;
        video >> frame;

        if(frame.empty()){
            break;
        }
        num_frames ++;

        double angle = processImg(frame, road_q);
        std::cout << "angle: " << angle << std::endl;
        imshow("frame", frame);

        int k = waitKey(conf::WAIT_KEY) & 0xff;

        if(k == 27){
            break;
        }
        if(k == 32){
            waitKey();
        }

        et = getTickCount();
        sum_fps += 1.0 / ((et-st)/freq);
        cerr << "FPS: "<< sum_fps/num_frames << ' ';
        frame.release();
    }
    video.release();
    destroyAllWindows();
    return 1;
}
