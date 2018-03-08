
#include <camera.h>



char Camera::analyzeFrame(const VideoFrameRef& frame_depth,const VideoFrameRef& frame_color,Mat& depth_img, Mat& color_img) {
        DepthPixel* depth_img_data;
        RGB888Pixel* color_img_data;

        int w = frame_color.getWidth();
        int h = frame_color.getHeight();
        cout<<"asdf"<<endl;
        depth_img = Mat(h, w, CV_16U);
        color_img = Mat(h, w, CV_8UC3);
        Mat depth_img_8u;
                depth_img_data = (DepthPixel*)frame_depth.getData();

                memcpy(depth_img.data, depth_img_data, h*w*sizeof(DepthPixel));
                normalize(depth_img, depth_img_8u, 255, 0, NORM_MINMAX);
                depth_img_8u.convertTo(depth_img_8u, CV_8U);
                color_img_data = (RGB888Pixel*)frame_color.getData();
                memcpy(color_img.data, color_img_data, h*w*sizeof(RGB888Pixel));
                cvtColor(color_img, color_img, COLOR_RGB2BGR);
//                cv::imshow("d",depth_img_8u);
//                cv::imshow("depth", color_img);

                return 'c';
}

void Camera::init_cam(){
        OpenNI::initialize();
        Device devAnyDevice;
        devAnyDevice.open( openni::ANY_DEVICE );
        streamDepth.create( devAnyDevice, openni::SENSOR_DEPTH );
        streamDepth.start();
        streamColor.create( devAnyDevice, openni::SENSOR_COLOR);
        streamColor.start();

}
