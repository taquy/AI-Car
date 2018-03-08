#include <getparam.h>

// get matrix to bird view
void getMatrixWrap(cv::Mat &dst, double w, double h, int bird_alpha){

    double PI = 3.1415926;
    int alpha_ = bird_alpha, beta_ = 90, gamma_ = 90;
    int f_ = 1616, dist_ = 565;

    double focalLength, dist, alpha, beta, gamma;

    alpha =((double)alpha_ -90) * PI/180;
    beta =((double)beta_ -90) * PI/180;
    gamma =((double)gamma_ -90) * PI/180;
    focalLength = (double)f_;
    dist = (double)dist_;

    // Projecion matrix 2D -> 3D
    cv::Mat A1 = (cv::Mat_<float>(4, 3)<<
        1, 0, -w/2,
        0, 1, -h/2,
        0, 0, 0,
        0, 0, 1 );


    // Rotation matrices Rx, Ry, Rz

    cv::Mat RX = (cv::Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, cos(alpha), -sin(alpha), 0,
        0, sin(alpha), cos(alpha), 0,
        0, 0, 0, 1 );

    cv::Mat RY = (cv::Mat_<float>(4, 4) <<
        cos(beta), 0, -sin(beta), 0,
        0, 1, 0, 0,
        sin(beta), 0, cos(beta), 0,
        0, 0, 0, 1	);

    cv::Mat RZ = (cv::Mat_<float>(4, 4) <<
        cos(gamma), -sin(gamma), 0, 0,
        sin(gamma), cos(gamma), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1	);


    // R - rotation matrix
    cv::Mat R = RX * RY * RZ;

    // T - translation matrix
    cv::Mat T = (cv::Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, dist,
        0, 0, 0, 1);

    // K - intrinsic matrix
    cv::Mat K = (cv::Mat_<float>(3, 4) <<
        focalLength, 0, w/2, 0,
        0, focalLength, h/2, 0,
        0, 0, 1, 0
        );
    dst = K * (T * (R * A1));

}
