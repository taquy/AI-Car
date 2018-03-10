
#include "detector.h"

cv::String modeldir = "/home/taquy/Projects/cpp/GPUTrafficSignDetector/models/";

cv::String stop_model = modeldir + "stop.xml";
cv::String left_model = modeldir + "left.xml";
cv::String right_model = modeldir + "right.xml";

//cv::RNG rng(12345);

Detector::Detector() {
    this->signName = {"STOP", "LEFT", "RIGHT"};
    this->classifiers = {
        cv::cuda::CascadeClassifier::create(stop_model),
        cv::cuda::CascadeClassifier::create(left_model),
        cv::cuda::CascadeClassifier::create(right_model)
    };
    this->signId = -1;
    // Distance calculate
    this->w1 = (double) 170; // px
    this->d1 = (double) 20; // cm
}

int Detector::difficulty[3] = {15, 18, 18};

//                              T    R    B  L
double Detector::boundbox[4] = {0.2, 0.2, 0.2, 0.2};

cv::Mat Detector::splitter(cv::Mat img) {
    cv::Rect box = this->cropbox(img);
    return img(box);
}

cv::Rect Detector::cropbox(cv::Mat frame) {
    double w = frame.cols;
    double h = frame.rows;
    double x = w * this->boundbox[3];
    double y = h * this->boundbox[0];
    double w2 = w - w * (this->boundbox[1] + this->boundbox[3]);
    double h2 = h - h * (this->boundbox[0] + this->boundbox[2]);
    return cv::Rect(x, y, w2, h2);
}

double Detector::calcDistance (double w2) {
    return (this->w1 * this->d1) / w2;
}

std::vector<cv::Rect> Detector::detectGpu(cv::cuda::GpuMat grayMat, int signId) {
    grayMat = grayMat.clone();

    cv::Ptr<cv::cuda::CascadeClassifier> gpuCascade = this->classifiers[signId];

    std::vector<cv::Rect> boxes;

    cv::cuda::GpuMat gpuFound;

    gpuCascade->setMinNeighbors(this->difficulty[signId]);
    gpuCascade->detectMultiScale(grayMat, gpuFound);
    gpuCascade->convert(gpuFound, boxes);

    return boxes;
}

int Detector::getID(cv::Mat frame) {
    cv::Mat area = frame.clone();
    area = this->splitter(area);

    cv::cuda::GpuMat gpuMat, grayMat;

    gpuMat.upload(area);

    cv::cuda::cvtColor(gpuMat, grayMat, cv::COLOR_BGR2GRAY);

    int n = this->signName.size();

//    std::vector<cv::Rect> boxes;

    std::vector<cv::Rect> boxes1 = this->detectGpu(grayMat, Detector::STOP);
//    if (boxes.size() > 0) return Detector::STOP;

    std::vector<cv::Rect> boxes2 = this->detectGpu(grayMat, Detector::LEFT);
//    if (boxes.size() > 0) return Detector::LEFT;

    std::vector<cv::Rect> boxes3 = this->detectGpu(grayMat, Detector::RIGHT);
//    if (boxes.size() > 0) return Detector::RIGHT;

    frame = this->draw(frame, boxes1, this->signName[Detector::STOP]);
    frame = this->draw(frame, boxes2, this->signName[Detector::LEFT]);
    frame = this->draw(frame, boxes3, this->signName[Detector::RIGHT]);
    imshow("test", frame);

    return -1;
}

cv::Mat Detector::draw(cv::Mat frame, vector<cv::Rect> boxes, cv::String label) {
    // draw frontier line
    cv::Rect box = this->cropbox(frame);
    rectangle(frame, cv::Point(box.x, box.y), cv::Point(box.x + box.width, box.y + box.height), cv::Scalar(255,255,255), 2);

    // draw rects
    for( size_t i = 0; i < boxes.size(); i++ )
    {
        int x = boxes[i].x + box.x;
        int y = boxes[i].y + box.y;
        cv::Point a(x, y);
        cv::Point b(x + boxes[i].width, y + boxes[i].height);

        cv::rectangle(frame, a, b, cv::Scalar(0, 255, 0), 3);

        cv::putText(frame, label, a, cv::FONT_HERSHEY_SIMPLEX, 1, (0, 125, 255), 3, 0, false);
    }
    return frame;
}
