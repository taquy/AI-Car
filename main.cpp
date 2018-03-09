#include "detector.h"
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;

#include "opencv2/core/core.hpp"

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaarithm.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <X11/Xlib.h>

#include <opencv2/gpu/gpu.hpp>


int main (int argc, char* argv[])
{
    VideoCapture cap(0);
    Detector *detector = new Detector();
    while (1) {
        Mat frame;
        cap >> frame;

        detector->getID(frame);


        if (waitKey(1) >= 0) break;

    }
    return 0;
}
