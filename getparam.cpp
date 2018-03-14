#include <getparam.h>

void getMatrixWrap(cv::Mat &dst, double w, double h, double alpha,
                   double focalLength, double dist, double y){

    double PI = 3.1415926;

    alpha =(alpha - 90) * PI/180;
    double beta = 0;
    double gamma = 0;


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

    cv::Mat RO = (cv::Mat_<float>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, -y,
        0, 0, 1, 0,
        0, 0, 0, 1	);


    // R - rotation matrix
    cv::Mat R = RX * RY * RZ * RO;

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
