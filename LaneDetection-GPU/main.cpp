#include <iostream>
#include <string.h>
#include <time.h>
#include <queue>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "api_kinect_cv.h"
#include "api_i2c_pwm.h"
#include "Hal.h"
#include "LCDI2C.h"
#include <config.h>
#include <detector.h>

#include <frameprocess.h>

using namespace std;
using namespace cv;
using namespace openni;
using namespace EmbeddedFramework;

PCA9685 *pca9685 = new PCA9685() ;
#define HEIGHT 640
#define WIDTH 480

#define SAMPLE_READ_WAIT_TIMEOUT 1000 //2000ms

#define SENSOR	166
#define STOP_SPEED 20;
#define START_ANGLE 0;

Mat depthImg, colorImg;
VideoFrameRef frameDepth, frameColor;
VideoStream depth, color;
VideoStream streamDepth, streamColor;

VideoWriter video("/home/ubuntu/Desktop/LaneDetection/color1.avi",CV_FOURCC('M','J','P','G'),25, Size(640,480));
char analyzeFrame(const VideoFrameRef& frame_depth,const VideoFrameRef& frame_color,Mat& depth_img, Mat& color_img) {
        DepthPixel* depth_img_data;
        RGB888Pixel* color_img_data;

        int w = frame_color.getWidth();
        int h = frame_color.getHeight();

        depth_img = Mat(h, w, CV_16U);
        color_img = Mat(h, w, CV_8UC3);
        Mat depth_img_8u;
        depth_img_data = (DepthPixel*)frame_depth.getData();
        memcpy(depth_img.data, depth_img_data, h*w*sizeof(DepthPixel));
        normalize(depth_img, depth_img_8u, 255, 0, NORM_MINMAX);
        depth_img_8u.convertTo(depth_img, CV_8U);
        color_img_data = (RGB888Pixel*)frame_color.getData();
        memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
        cvtColor(color_img, color_img, COLOR_RGB2BGR);

        return 'c';
}
int stop(){
        int throttle_stop = STOP_SPEED;
        api_set_FORWARD_control( pca9685,throttle_stop);
    }

void setup(PCA9685 *pca9685){
    int inti_throttle= STOP_SPEED;
    double inti_angle = START_ANGLE;
    api_pwm_pca9685_init( pca9685);
    if (pca9685->error >= 0){
        api_set_FORWARD_control( pca9685,inti_throttle);
        api_set_STEERING_control(pca9685, inti_angle);
    }
}

char key = 0;
bool running = false;
double st = 0, et = 0, fps = 0;
double freq = getTickFrequency();
int frame_id = 0;
int throttle = STOP_SPEED;
double angle = START_ANGLE;

void run(){

    OpenNI::initialize();
    Device devAnyDevice;
    devAnyDevice.open( openni::ANY_DEVICE );
    streamDepth.create( devAnyDevice, openni::SENSOR_DEPTH );
    streamDepth.start();
    streamColor.create( devAnyDevice, openni::SENSOR_COLOR);
    streamColor.start();

    queue<Road> road_q;
    road_q.push(Road());

    int frameCount = 0;

    while (true)
    {
        st = getTickCount();
        key = getkey();


        if( key == '2'){
            running = !running;
            throttle = 30;
            api_set_FORWARD_control( pca9685,throttle);
        }
        if( key == '8') {
            stop();
            break;
        }
        if( key == '4') {
            stop();
            running = !running;

        }
        if( running )
        {
            frame_id ++;
            streamDepth.readFrame( &frameDepth );
            streamColor.readFrame( &frameColor);
            char recordStatus = analyzeFrame(frameDepth,frameColor, depthImg, colorImg);
            //flip(colorImg, colorImg, 1);
            ///////////////////////////// color image ///////////////////////////////////////////
            if (recordStatus == 'c') {

                 if(conf::WRITE_VIDEO){
                     video.write(colorImg);
                 }

                 imshow("asf", colorImg);
//                 angle = processImg(colorImg, road_q);
                 cout << angle << endl;
                 throttle = conf::SPEED;
                 api_set_FORWARD_control( pca9685,throttle);
                 api_set_STEERING_control(pca9685 , angle);

            }
            et = getTickCount();
            fps = 1.0 / ((et-st)/freq);
            cerr << "FPS: "<< fps<< '\n';
            waitKey(10);
            if( key == 27 ) break;
        }
    }
    if(conf::WRITE_VIDEO){
        video.release();
    }
}

void runCar(){
    init();
    setup(pca9685);
    run();
    api_pwm_pca9685_release(pca9685 );
}

void runVideo(){
    init();
    string fileName = "/home/ubuntu/Desktop/LaneDetection/2.avi";

    testVideo(fileName);
}

int main()
{
//    runCar();
        runVideo();

//    init_cam();
//    run();

    return 0;
}
