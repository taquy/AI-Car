#include <iostream>
#include <string.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "api_kinect_cv.h"
#include "api_i2c_pwm.h"
#include "Hal.h"
#include "LCDI2C.h"

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

bool analyzeFrame(const VideoFrameRef& frame_depth,const VideoFrameRef& frame_color,Mat& depth_img, Mat& color_img) {
        DepthPixel* depth_img_data;
        RGB888Pixel* color_img_data;

        int w = frame_color.getWidth();
        int h = frame_color.getHeight();

        depth_img = Mat(h, w, CV_16U);
        color_img = Mat(h, w, CV_8UC3);
        Mat depth_img_8u;
        depth_img_data = ( DepthPixel*)frame_depth.getData();
          for( int y = 0; y < frame_depth.getHeight() - 1; ++ y )
          {
            for( int x = 0; x < frame_depth.getWidth() - 1; ++ x )
            {
              int idx = x + y * frame_depth.getWidth();
              const DepthPixel&  rDepth = depth_img_data[idx];
              float fX, fY, fZ;
              openni::CoordinateConverter::convertDepthToWorld( streamDepth,x, y, rDepth, &fX, &fY, &fZ );
              cout<< fZ << endl;
            }
          }
       // memcpy(depth_img.data, depth_img_data, h*w*sizeof(DepthPixel));
       // normalize(depth_img, depth_img_8u, 255, 0, NORM_MINMAX);
        depth_img_8u.convertTo(depth_img, CV_8U);
        color_img_data = (RGB888Pixel*)frame_color.getData();
        memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
        cvtColor(color_img, color_img, COLOR_RGB2BGR);
        return true;
}

char key = 0;
bool running = false;
double st = 0, et = 0, fps = 0;
double freq = getTickFrequency();
int frame_id = 0;

void run(){

    OpenNI::initialize();
    Device devAnyDevice;
    devAnyDevice.open( openni::ANY_DEVICE );
    streamDepth.create( devAnyDevice, openni::SENSOR_DEPTH );
    streamDepth.start();
    streamColor.create( devAnyDevice, openni::SENSOR_COLOR);
    streamColor.start();

    while (true)
    {
        st = getTickCount();
        key = getkey();



        frame_id ++;
        streamDepth.readFrame( &frameDepth );
        streamColor.readFrame( &frameColor);
        bool recordStatus = analyzeFrame(frameDepth,frameColor, depthImg, colorImg);

        if (recordStatus) {
           // cv::imshow("depth", depthImg);
        } else {
            cout << "asdfasdfasdfa" << endl;
        }
        et = getTickCount();
        fps = 1.0 / ((et-st)/freq);
        cerr << "FPS: "<< fps<< '\n';
        waitKey(10);
        if( key == 27 ) break;

    }

}

void runCar(){
    run();

}
int main()
{
    runCar();
    return 0;
}
