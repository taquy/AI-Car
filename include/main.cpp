#include "frameprocess.h"
#include "getparam.h"

int main(){
    init();
    testVideo("/home/nc/Desktop/video/7.avi");
    std::cout << conf::HEIGHT << std::endl;
}
