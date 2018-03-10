#include <imageprocess.h>
#include <config.h>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudaobjdetect.hpp>
#include <opencv2/cudaarithm.hpp>

#include <opencv2/cudawarping.hpp>

//! Separate a image to small layers
void separateLayers(cv::Mat &src, std::vector<Layer> &layers){

    cv::Rect roi(0, 0, conf::W_LAYER, conf::H_LAYER);
    for(int i = 0; i < conf::NUMLAYERS - 1; i++){
        roi.y = conf::H_ROI - (i+1)*conf::H_LAYER;
        cv::Mat sub = src(roi);
        layers.push_back(Layer(sub, roi.x, roi.y));
    }

    roi.y = 0;
    roi.height = conf::H_ROI - conf::H_LAYER*(conf::NUMLAYERS - 1);
    cv::Mat sub = src(roi);
    layers.push_back(Layer(sub, roi.x, roi.y));
}


//! Concatenate small layers to a imgs
void concatenateLayers(std::vector<Layer> &layers, cv::Mat &dst){
    dst = cv::Mat(conf::H_ROI, conf::W_ROI, CV_8UC3);
    cv::Mat R;

    cv::Rect roi(0, 0, conf::W_LAYER, conf::H_LAYER);

    for(int i = 0; i < conf::NUMLAYERS - 1; i++){
        roi.y = conf::H_ROI - (i+1)*conf::H_LAYER;
        R = cv::Mat(dst, roi);
//        layers[i].mask.copyTo(R);
    }
    roi.y = 0;
    roi.height = conf::H_ROI - conf::H_LAYER*(conf::NUMLAYERS - 1);
    R = cv::Mat(dst, roi);
//    layers[conf::NUMLAYERS - 1].mask.copyTo(R);
}


//! Bird View a image
void birdView(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst){

    cv::cuda::warpPerspective(src, dst, imp::matrixWrap, src.size(), cv::INTER_CUBIC | cv::WARP_INVERSE_MAP);
    dst = dst(imp::roiCut);

}


//! Take a region of image
/*!
 * \brief roiImg
 * \param src   input image
 * \param roi   a rectangle which define the roi on the image
 * \return      roi
 */
void roiImg(cv::cuda::GpuMat &src, cv::cuda::GpuMat &dst , cv::Rect &roi){
    dst = src(roi);
}

//! Find contours of a image and filter contours
/*!
 * \brief findCnts
 * \param src       input image
 * \param contours  output contours
 */
void findCnts(cv::Mat &src, std::vector<std::vector<cv::Point>> &contours){

    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(src, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    // filter contours
    for(unsigned int i = 0; i < contours.size(); i++){
        if(cv::contourArea(contours[i]) < conf::MIN_AREA_CNT){
            contours.erase(contours.begin() + i);
            i--;
        }
    }
}

void findCenterPoint(Layer &layer){

    // find contours
    std::vector<std::vector<cv::Point>> contours;
    //findCnts(layer.img, contours);

    int cX, cY;
    cv::Moments M;

    for(std::vector<cv::Point> &cnt : contours){
        M = cv::moments(cnt);
        cX = (int)(M.m10 / M.m00);
        cY = (int)(M.m01 / M.m00);

        layer.points.push_back(cv::Point(cX, cY));
    }
}


//! This function will find all contours and center points of these contours on a layer
//! Contours after finding is going to filtered
/*!
 * \brief findPoints
 * \param layer     class Layer
 * \param isDebug   if true, contours and centers poitns will be drawn on mask of the layer
 */
void findPoints(Layer &layer){
    // find contours
    std::vector<std::vector<cv::Point>> contours;
    //findCnts(layer.img, contours);
}
