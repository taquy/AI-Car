#include <svm.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/ml/ml.hpp>

void findReadColor(cv::Mat &hsv, cv::Mat &mask){
    cv::inRange(hsv, cv::Scalar(80, 70, 50), cv::Scalar(100, 255, 255), mask);
    cv::medianBlur(mask, mask, 3);
}

void findTraffic(cv::Mat &mask, cv::Mat &out, cv::Mat &img2){


    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Rect> boundRect( contours.size() );
      std::vector<cv::Vec4i> hierarchy;

      cv::findContours(mask, contours, hierarchy, cv::RETR_TREE , cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

      if (contours.size() > 0){

          std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
          std::vector<cv::Point2f>center(contours.size());
          std::vector<float> radius( contours.size() );


             int largest_area=0;
             int largest_contour=0;
             cv::Rect rect;
             for(unsigned int i = 0; i< contours.size(); i++ ){


               double a= cv::contourArea( contours[i],false);
               if( a > largest_area){
                    largest_area=a;
                    largest_contour=i;
                    rect = cv::boundingRect(contours[i]);
                }
              }


               if(rect.area() < mask.size().height * mask.size().width){
                   img2(rect).copyTo(out);
               }


         }
}


std::string predict(cv::Mat &test, cv::Ptr<cv::ml::SVM> &svm){

      cv::resize(test,test,cv::Size(64,48));
      cv::HOGDescriptor hog( cv::Size(64,48),cv::Size(8,8), cv::Size(4,4), cv::Size(4,4), 9);
      std::vector<float> size_;
      hog.compute(test,size_);
      int col= size_.size();
      cv::Mat testMat(1, col, CV_32FC1);
      std::vector<float> des;
      hog.compute(test,des);
      for (int i = 0; i < col; i++)
      testMat.at<float>(0,i)= des[i];

      return output(svm->predict(testMat));
}
std::string output(int testid){

  if(testid==0)      return "duong cam.";
  else if(testid==1) return "cam di nguoc chieu.";
  else if(testid==2) return "cam oto.";
  else if(testid==3) return "cam moto.";
  else if(testid==4) return "cam oto va moto.";
  else if(testid==5) return "cam xe tai.";
  else if(testid==6) return "cam xe tai va xe khach.";
  else if(testid==7) return "cam oto keo mooc.";
  else if(testid==8) return "cam may keo.";
  else if(testid==9) return "cam xe dap.";
  else if(testid==10) return "cam nguoi di bo.";
  else if(testid==11) return "stop.";
  else if(testid==12) return "cam re trai.";
  else if(testid==13) return "cam re phai.";
  else if(testid==14) return "cam quay dau xe.";
  else if(testid==15) return "cam dung.";
  else if(testid==16) return "cam dung va do.";
  else if(testid==17) return "toc do cho phep toi da.";

  return "";
}
