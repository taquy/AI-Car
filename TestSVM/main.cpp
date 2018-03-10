#include <svm.h>
#include <iostream>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dirent.h>
#include <getfiles.h>
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

        memcpy(depth_img.data, depth_img_data, h*w*sizeof(DepthPixel));
        normalize(depth_img, depth_img_8u, 255, 0, NORM_MINMAX);
        depth_img_8u.convertTo(depth_img, CV_8U);
        color_img_data = (RGB888Pixel*)frame_color.getData();
        memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
        cvtColor(color_img, color_img, COLOR_RGB2BGR);
        flip(colorImg, colorImg, 1);
        return true;
}



int train(){
    int m=0;
    int numfiles;
    cv::Mat img, img0;
    vector<string> files;
    vector<string> folders;
    string f = "/home/nc/TestSVM/data";

    img0 = cv::imread(f+"/101/1.bmp", 1);

    cv::Mat re;

    cv::resize(img0, re, cv::Size(64,48));

    vector<float> size_;

    cv::HOGDescriptor hog( cv::Size(64,48), cv::Size(8,8), cv::Size(4,4), cv::Size(4,4), 9);


    hog.compute(img0, size_);



    list_folders(f, folders);

    for(string s: folders){
        cout << s << endl;
    }
    numfiles= 20 * folders.size();
    cv::Mat labels(numfiles, 1, CV_32S);
    cv::Mat trainMat(numfiles, size_.size(), CV_32FC1);
    for(unsigned int x= 0; x < folders.size(); x++ ){
        list_file(folders[x], files);
        for(unsigned int y =0; y< files.size(); y++){
            img = cv::imread(files[y], 0);

            cv::resize(img, img, cv::Size(64,48));
            vector<float> des;
            hog.compute(img,des);

            for (unsigned int i = 0; i < size_.size(); i++)
                trainMat.at<float>(m,i)= des[i];
                labels.at<int>(m, 0) = x;
                m++;
            }
      }

    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 10000, 1e-6));
    svm->train(trainMat, cv::ml::ROW_SAMPLE, labels);
    svm->save(f + "/train.txt");

}



void testFileImage(cv::Mat &img, cv::Ptr<cv::ml::SVM> &svm){
    cv::Mat img2 ,mask , hsv_image, out;

    cv::cvtColor(img , img2, cv::COLOR_BGR2GRAY);

    img = ~img;

    cv::cvtColor(img, hsv_image, cv::COLOR_BGR2HSV);

    findReadColor(hsv_image, mask);

    findTraffic(mask, out, img2);

    if(!out.empty()){
        cv::imshow("out", out);

        cout << predict(out, svm) << endl;
    }


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


    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm = cv::ml::SVM::load("/home/ubuntu/Desktop/CDS/TestSVM/data/train.txt");

    while (true)
    {
        st = getTickCount();
        key = getkey();



        frame_id ++;
        streamDepth.readFrame( &frameDepth );
        streamColor.readFrame( &frameColor);
        bool recordStatus = analyzeFrame(frameDepth,frameColor, depthImg, colorImg);

        if (recordStatus) {
           testFileImage(colorImg, svm);
           imshow("origin", colorImg);
        }

        et = getTickCount();
        fps = 1.0 / ((et-st)/freq);
        cerr << "FPS: "<< fps<< '\n';
        int k = cv::waitKey(1) & 0xff;

        if(k == 27){
            break;
        }
        if(k == 32){
            cv::waitKey();
        }

    }

}

int main(){
    run();
}
