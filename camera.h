#ifndef CAMERA_H
#define CAMERA_H

#include "api_kinect_cv.h"


class Camera{
  public:


    Camera();
    void init_cam();
    char analyzeFrame(const VideoFrameRef& ,const VideoFrameRef& ,Mat& , Mat& );

};
#endif // CAMERA_H
