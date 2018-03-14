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
GPIO *gpio = new GPIO();

#define HEIGHT 640
#define WIDTH 480
#define SAMPLE_READ_WAIT_TIMEOUT 1000

#define ARC 10.6 * 3.14159 / 40
bool is_log_map = true;
bool running = false;
#define SENSOR	165
#define LED		166
#define STOP_SPEED 20;
#define START_ANGLE 0;
Status rc;
Device device;
FILE *logMap;
VideoWriter depth_videoWriter;
VideoWriter color_videoWriter;

VideoStream depth, color;

char analyzeFrame(const VideoFrameRef& frame_depth,const VideoFrameRef& frame_color,Mat& depth_img, Mat& color_img) {
    DepthPixel* depth_img_data;
       RGB888Pixel* color_img_data;

       int w = frame_color.getWidth();
       int h = frame_color.getHeight();

       depth_img = Mat(h, w, CV_16U);
       color_img = Mat(h, w, CV_8UC3);
//       Mat depth_img_8u;
//       depth_img_data = (DepthPixel*)frame_depth.getData();
//       memcpy(depth_img.data, depth_img_data, h*w*sizeof(DepthPixel));
//       normalize(depth_img, depth_img_8u, 255, 0, NORM_MINMAX);
//       depth_img_8u.convertTo(depth_img, CV_8U);
       color_img_data = (RGB888Pixel*)frame_color.getData();
       memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
       cvtColor(color_img, color_img, COLOR_RGB2BGR);
       flip(color_img, color_img, 1);
       flip(depth_img, depth_img, 1);
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

int init_cam()
{
    rc = OpenNI::initialize();
    if (rc != STATUS_OK) {
        printf("Initialize failed\n%s\n", OpenNI::getExtendedError());
        return 0;
    }
    rc = device.open(ANY_DEVICE);
    if (rc != STATUS_OK) {
        printf("Couldn't open device\n%s\n", OpenNI::getExtendedError());
        return 0;
    }
    if (device.getSensorInfo(SENSOR_DEPTH) != NULL) {
        rc = depth.create(device, SENSOR_DEPTH);
        if (rc == STATUS_OK) {
            VideoMode depth_mode = depth.getVideoMode();
            depth_mode.setFps(30);
            depth_mode.setResolution(WIDTH, HEIGHT);
            depth_mode.setPixelFormat(PIXEL_FORMAT_DEPTH_100_UM);
            depth.setVideoMode(depth_mode);
            rc = depth.start();
            if (rc != STATUS_OK) {
                printf("Couldn't start the color stream\n%s\n", OpenNI::getExtendedError());
            }
        }
        else {
            printf("Couldn't create depth stream\n%s\n", OpenNI::getExtendedError());
        }
    }

    if (device.getSensorInfo(SENSOR_COLOR) != NULL) {
        rc = color.create(device, SENSOR_COLOR);
        if (rc == STATUS_OK) {
            VideoMode color_mode = color.getVideoMode();
            color_mode.setFps(30);
            color_mode.setResolution(WIDTH, HEIGHT);
            color_mode.setPixelFormat(PIXEL_FORMAT_RGB888);
            color.setVideoMode(color_mode);

            rc = color.start();
            if (rc != STATUS_OK)
            {
                printf("Couldn't start the color stream\n%s\n", OpenNI::getExtendedError());
            }
        }
        else {
            printf("Couldn't create color stream\n%s\n", OpenNI::getExtendedError());
        }
    }
}
char key = 0;
double st = 0, et = 0, fps = 0;
double freq = getTickFrequency();
int frame_id = 0;
int throttle = STOP_SPEED;
double angle = START_ANGLE;

void run(){
    VideoFrameRef frame_depth, frame_color;
    VideoStream* streams[] = {&depth, &color};
    Mat depthImg, colorImg;
    gpio->gpioExport(SENSOR);
    gpio->gpioExport(LED);
    gpio->gpioSetDirection(SENSOR, INPUT);
    gpio->gpioSetDirection(LED, OUTPUT);

    if(is_log_map) {
        logMap = fopen("/home/ubuntu/Desktop/LaneDetection/map.txt", "w");
    }
    if(conf::WRITE_VIDEO) {
        color_videoWriter.open("/home/ubuntu/Desktop/LaneDetection/color.avi", CV_FOURCC('D','I','V', 'X'), 8, Size(640,480), true);
        depth_videoWriter.open("/home/ubuntu/Desktop/LaneDetection/depth.avi", CV_FOURCC('D','I','V', 'X'), 8, Size(640,480), false);
    }

    queue<Road> road_q;
    road_q.push(Road());

    Detector *d = new Detector();
    d->isDebug = true;

    unsigned int sensor_status = 1, count = 0;
    int s = 1, l = 1, x = 0, sensor = 1;
    while (true)
    {
        st = getTickCount();
        key = getkey();
        if( key == '2'){
            running = !running;
//            throttle = 30;
            api_set_FORWARD_control( pca9685,throttle);
        }
        if( key == '8') {
            fclose(logMap);
            stop();
            break;
        }
        if( key == '4') { // 4
            stop();
            running = !running;
        }
//        gpio->gpioGetValue(SENSOR, &sensor_status);
        if( running )
        {
            int readyStream = -1;
            rc = OpenNI::waitForAnyStream(streams, 2, &readyStream, SAMPLE_READ_WAIT_TIMEOUT);
            depth.readFrame(&frame_depth);
            color.readFrame(&frame_color);
            frame_id ++;
            char recordStatus = analyzeFrame(frame_depth,frame_color, depthImg, colorImg);
//            flip(colorImg, colorImg, 1);

            if (recordStatus == 'c'){
                 if(conf::WRITE_VIDEO){
                     if (!colorImg.empty())
                          color_videoWriter.write(colorImg);
                     if (!depthImg.empty())
                          depth_videoWriter.write(depthImg);
                 }
                 if(conf::isDetect ==1){
                     d->getProxID(colorImg);

                     if (d->signId != -1) {

                         if (d->signId == Detector::LEFT) {
                            cout << "This is Left" << endl;
                             conf::LEFT = 1;
                         } else if (d->signId == Detector::RIGHT)  {
                             cout << "This is Right" << endl;
                             conf::LEFT = 0;
                         } else {
                             stop();
                             break;
                         }
                     }
                 }
                 angle = processImg(colorImg, road_q);

//                 imshow("asdf", colorImg);
                 throttle = conf::SPEED;
                 // control speed car
                 api_set_FORWARD_control( pca9685,throttle);
                 // control steering car
                 api_set_STEERING_control(pca9685 , angle);
            }
//            cout<< sensor_status<< endl;

            ///////////////////////////////////488////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////
            et = getTickCount();
            fps = 1.0 / ((et-st)/freq);
            cout << "FPS: "<< fps<< '\n';
//            fprintf(logMap, "%f %f \n", angle, fps);
            waitKey(1);
            if( key == 27 ) break;
        }
    }
    if(conf::WRITE_VIDEO){
        color_videoWriter.release();
        depth_videoWriter.release();
    }
}
void runCar(){
    init();
    setup(pca9685);
    init_cam();
    run();
    api_pwm_pca9685_release(pca9685 );
}
void runVideo(){
    init();
    string fileName = "/home/ubuntu/Desktop/1212/color.avi";

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
