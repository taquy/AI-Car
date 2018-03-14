#include <frameprocess.h>
#include <line.h>
#include <layer.h>
#include <imageprocess.h>
#include <pointsprocess.h>
#include <laneprocess.h>
#include <debug.h>
#include <detector.h>

using namespace std;
using namespace cv;

// reference angle
void alluse(double &angle) {
    double OA = refer::A - refer::O;
    double OB = refer::O - refer::B;
    if(angle < 0){
       angle = OB/cv::abs(refer::b) * angle + refer::O;
    } else {
       angle = OA/refer::a * angle + refer::O;
    }

}

// get angle from two points
double getTheta(cv::Point &car, cv::Point &dst) {
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
    std::cout << "angle : " << angle << " -- ";
    alluse(angle);
    std::cout << angle << std::endl;

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

    cv::Mat roi = src(imp::roi);
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
    conf::NGABA = ngaba(lanes, road, src);

    if(!conf::NGABA){
        // process lanes
        separateLeftRightByDistance(lanes, road);
    }


    if(road.hasLeft){
        std::vector<cv::Point> new_cnt;
        for(cv::Point &p : road.left.cnt){
            if(p.y > conf::DOWN_Y){
                new_cnt.push_back(p);
            } else {
                break;
            }
        }
        if(new_cnt.size() > 2){
            road.setLeft(Lane(new_cnt));
        } else {
            road.setLeft(Lane());
            road.hasLeft = false;
        }

    }

    if(road.hasRight){
        std::vector<cv::Point> new_cnt;
        for(cv::Point &p : road.right.cnt){
            if(p.y > conf::DOWN_Y){
                new_cnt.push_back(p);
            } else {
                break;
            }
        }

        if(new_cnt.size() > 2){
            road.setRight(Lane(new_cnt));
        } else {
            road.setRight(Lane());
            road.hasRight = false;
        }
    }

    genLine(road, road_q);
    Road new_r = road_q.front();


    // show images process to debug, don't care about alrogithm
    if(conf::DEBUG){
        draw(src, roi, b, road, copy_lanes);
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

    Detector *d = new Detector();
    d->isDebug = true;

    while(true){
        st = getTickCount();

        Mat frame;
        video >> frame;

        if(frame.empty()){
            break;
        }
        num_frames ++;


            d->getProxID(frame);


            if (d->signId != -1) {

                if (d->signId == Detector::LEFT) {
                   cout << "This is Left" << endl;
                    conf::LEFT = 1;
                } else if (d->signId == Detector::RIGHT)  {
                    cout << "This is Right" << endl;
                    conf::LEFT = 0;
                }
                cout << conf::LEFT << endl;
                waitKey();
            }




        double angle = processImg(frame, road_q);

        int k = waitKey(conf::WAIT_KEY) & 0xff;

        if(k == 27){
            break;
        }
        if(k == 32){
            waitKey();
        }

        et = getTickCount();
        sum_fps += 1.0 / ((et-st)/freq);
        cerr << "FPS: "<< sum_fps/num_frames << '\n';
        frame.release();
    }
    video.release();
    destroyAllWindows();
    return 1;
}
