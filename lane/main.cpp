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

#include <frameprocess.h>

using namespace std;
using namespace cv;
using namespace openni;
using namespace EmbeddedFramework;

PCA9685 *pca9685 = new PCA9685() ;
GPIO *gpio = new GPIO();
#define HEIGHT 640
#define WIDTH 480

#define SAMPLE_READ_WAIT_TIMEOUT 1000 //2000ms
#define SW1_PIN	160
#define SW2_PIN	161
#define SW3_PIN	160
#define SW4_PIN	164
#define SENSOR	166
#define STOP_SPEED 20;
#define START_ANGLE 0;

int sw3_stat = 1;
int sw4_stat = 1;

int sensor = 0;

Status rc;
Device device;

VideoStream depth, color;
VideoWriter video("/home/ubuntu/Desktop/LaneDetection/color.avi",CV_FOURCC('M','J','P','G'),25, Size(640,480));

char analyzeFrame(const VideoFrameRef& frame_depth,const VideoFrameRef& frame_color,Mat& depth_img, Mat& color_img) {
    DepthPixel* depth_img_data;
    RGB888Pixel* color_img_data;

    int w = frame_color.getWidth();
    int h = frame_color.getHeight();

    color_img = Mat(h, w, CV_8UC3);
    color_img_data = (RGB888Pixel*)frame_color.getData();
    memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
    cvtColor(color_img, color_img, COLOR_RGB2BGR);
    flip(color_img, color_img, 1);
    return 'c';
}

void setup_button(){

        // Setup input

        gpio->gpioExport(SW3_PIN);
        gpio->gpioExport(SW4_PIN);

        gpio->gpioSetDirection(SW3_PIN, INPUT);
        gpio->gpioSetDirection(SW4_PIN, INPUT);
    }

int stop(){
        int throttle_stop = STOP_SPEED;
        api_set_FORWARD_control( pca9685,throttle_stop);

    }

void setup(PCA9685 *pca9685){
    int inti_throttle= STOP_SPEED;
    double inti_angle = START_ANGLE;
    setup_button();
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
bool running = false;
double st = 0, et = 0, fps = 0;
double freq = getTickFrequency();
int frame_id = 0;
int throttle = STOP_SPEED;
double angle = START_ANGLE;

void run(){
    VideoFrameRef frame_depth, frame_color;
    VideoStream* streams[] = {&depth, &color};
    Mat depthImg, colorImg, grayImage;

    queue<Road> road_q;
    road_q.push(Road());

    while (true)
    {
        st = getTickCount();
        key = getkey();

        unsigned int bt4_status = 0;
        unsigned int bt3_status = 1;

         gpio->gpioGetValue(SW4_PIN, &bt4_status);
         gpio->gpioGetValue(SW3_PIN, &bt3_status);

         if (!bt4_status) {
             if (bt4_status != sw4_stat) {
                 running = !running;
                 sw4_stat = bt4_status;
                 throttle = 50;
                 api_set_FORWARD_control( pca9685,throttle);
             }
         } else{
             sw4_stat = bt4_status;
             if (bt3_status == 0) {
                 stop();
                 break;
             }
         }
        if( key == 's'){
            running = !running;
            throttle = 40;
            api_set_FORWARD_control( pca9685,throttle);
        }
        if( key == 'f') {
            stop();
            break;
        }
        if( key == 'p') {

            stop();
            running = !running;

        }
        if( running )
        {
            int readyStream = -1;
            rc = OpenNI::waitForAnyStream(streams, 2, &readyStream, SAMPLE_READ_WAIT_TIMEOUT);
            depth.readFrame(&frame_depth);
            color.readFrame(&frame_color);
            frame_id ++;
            char recordStatus = analyzeFrame(frame_depth,frame_color, depthImg, colorImg);
            //flip(colorImg, colorImg, 1);
            /////////////////////////////////////////////////////////////////////////////////////////
            ///////////////////////////// color image ///////////////////////////////////////////
            if (recordStatus == 'c') {

                 if(conf::WRITE_VIDEO){

                     video.write(colorImg);
                 }

                 angle = processImg(colorImg, road_q);
                 throttle = conf::SPEED;

                 // control speed car
                 api_set_FORWARD_control( pca9685,throttle);
                 // control steering car
                 api_set_STEERING_control(pca9685 , angle);

            }
            ///////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////
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
    init_cam();
    run();
    api_pwm_pca9685_release(pca9685 );
}

void runVideo(){
    init();
    string fileName = "/home/ubuntu/Desktop/color.avi";

    testVideo(fileName);
}

int main()
{
    runCar();;
//    runVideo();

    return 0;
}
