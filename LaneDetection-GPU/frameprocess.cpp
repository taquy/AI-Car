#include <frameprocess.h>
#include <line.h>
#include <layer.h>
#include <imageprocess.h>
#include <pointsprocess.h>
#include <laneprocess.h>
#include <opencv2/core.hpp>

#include <opencv2/cudaarithm.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <debug.h>

using namespace std;
using namespace cv;
 double start , end;
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
    cout << "new a: " << angle << endl;

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
    cv::cuda::GpuMat src_cuda,
            roi, gray, thres,
            b;

    src_cuda.upload(src);

    roiImg(src_cuda, roi, imp::roi);

    cv::cuda::cvtColor(roi, gray, CV_BGR2GRAY);
    cv::cuda::threshold(gray, thres, conf::THRES, 255, cv::THRESH_BINARY);


    birdView(thres, b);
    cv::Mat down_bird;
    b.download(down_bird);


    // process layer
    std::vector<Layer> layers;
    separateLayers(down_bird, layers);

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
        cout <<"-------------------" <<endl;
        separateLeftRight(lanes, road);

    }

    genLine(road, road_q);
    Road new_r = road_q.front();


    // show images process to debug, don't care about alrogithm
    if(conf::DEBUG){
        Mat down_roi;
        roi.download(down_roi);
        draw(src, down_roi, down_bird, new_r, copy_lanes);
    }


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
