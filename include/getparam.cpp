#include "getparam.h"
#include "config.h"

#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unistd.h>

class Config{
private:
    std::string path;
    std::map<std::string, int> items;
    void readlines()
    {
        std::ifstream infile(path);

        if(infile.is_open()){
            std::cout << "Reading config ... " << std::endl;
            std::string line;
            while (std::getline(infile, line))
            {
                int specialChar = 0;
                for(char c : line){
                    if(c == '/'){
                        specialChar++;
                        break;
                    }
                }

                if(specialChar == 0){

                    split(line);
                }

            }
        } else {
            std::cout << "Failed open file config..." << std::endl;
        }

    }

    void split(std::string t)
    {
        std::stringstream test(t);
        std::string segment;
        std::vector<std::string> object;

        while (std::getline(test, segment, '='))
        {
            segment.erase(std::remove(segment.begin(), segment.end(), ' '), segment.end());
            object.push_back(segment);
        }

        for (unsigned int i = 0; i < object.size(); i += 2)
        {
            int value = std::stoi(object[i + 1]);
            items.insert(std::pair<std::string, int>(object[i], value));
            std::cout << object[i] <<  " = " << value << std::endl;

        }

    }
public:

    void setpath(std::string t)
    {
        path = t;
        readlines();
    }

    int find(std::string t)
    {

        auto it = items.find(t);
        if(it != items.end()){
            return it->second;
        } else {
            return -1;
        }
    }

};

std::string GetCurrentWorkingDir( void ) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        return std::string(cwd);
    else
        perror("getcwd() error");
    return "";
}

void init(){
    std::string dir_config = GetCurrentWorkingDir()  + "/config.txt";
    Config getConfigs;
    getConfigs.setpath(dir_config);


    conf::WIDTH = getConfigs.find("WIDTH");
    conf::HEIGHT = getConfigs.find("HEIGHT");

    // Region of lane
    conf::X_CUT = getConfigs.find("X_CUT");
    conf::Y_CUT = getConfigs.find("Y_CUT");
    conf::Y_BOT_OFFSET = getConfigs.find("Y_BOT_OFFSET");
    conf::W_CUT = conf::WIDTH;
    conf::H_CUT = conf::HEIGHT - conf::Y_CUT - conf::Y_BOT_OFFSET;

    // Region of birdview which will be taked from region of lane
    conf::W_ROI = getConfigs.find("W_ROI");
    conf::X_ROI = (int)(conf::W_CUT - conf::W_ROI)/2;
    conf::Y_ROI = getConfigs.find("Y_ROI");
    conf::H_ROI = conf::H_CUT - conf::Y_ROI;

    // config for image processing
    conf::THRES = getConfigs.find("THRES");
    conf::NUMLAYERS = getConfigs.find("NUMLAYERS");
    conf::MIN_AREA_CNT = getConfigs.find("MIN_AREA_CNT");

    conf::DOWN_Y = getConfigs.find("DOWN_Y");

    // Region of layer
    conf::H_LAYER = (int)(conf::H_ROI / conf::NUMLAYERS);
    conf::W_LAYER = conf::W_ROI;
    conf::MIN_DIS_TWO_POINTS = conf::H_LAYER*3;

    // Point center of region's bird
    conf::X_CENTER = (int) (conf::W_ROI/2);
    conf::Y_CENTER = conf::H_ROI;

    conf::MIN_POINTS = getConfigs.find("MIN_POINTS");
    conf::DIS_GEN_LINE_TOP = getConfigs.find("DIS_GEN_LINE_TOP");
    conf::DIS_GEN_LINE_BOT = getConfigs.find("DIS_GEN_LINE_BOT");
    conf::DIS_ROAD_INJUCTION = getConfigs.find("DIS_ROAD_INJUCTION");
    conf::SPEED = getConfigs.find("SPEED");
    conf::LEFT = getConfigs.find("LEFT");
    conf::DEBUG = getConfigs.find("DEBUG");

    conf::WAIT_KEY = getConfigs.find("WAIT_KEY");
    conf::NGABA = getConfigs.find("NGABA");
    conf::WRITE_VIDEO = getConfigs.find("WRITE_VIDEO");

    // set referecences
    refer::A = getConfigs.find("A");
    refer::O = getConfigs.find("O");
    refer::B = getConfigs.find("B");
    refer::a = getConfigs.find("a");
    refer::b = getConfigs.find("b");


    // set imp
    imp::ALPHA = getConfigs.find("ALPHA");
    imp::F = getConfigs.find("F");
    imp::DIST = getConfigs.find("DIST");
    imp::Y = getConfigs.find("Y");

    imp::roi.x = conf::X_CUT;
    imp::roi.y = conf::Y_CUT;
    imp::roi.width = conf::W_CUT;
    imp::roi.height = conf::H_CUT;

    imp::roiCut.x = conf::X_ROI;
    imp::roiCut.y = conf::Y_ROI;
    imp::roiCut.width = conf::W_ROI;
    imp::roiCut.height = conf::H_ROI;

    getMatrixWrap(imp::matrixWrap, conf::W_CUT, conf::H_CUT, imp::ALPHA, imp::F, imp::DIST, imp::Y);

}

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
