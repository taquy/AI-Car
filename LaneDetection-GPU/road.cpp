#include <road.h>
#include <config.h>

Road::Road(){
    this->pointBot.x =  (int)(conf::W_ROI/2);
    this->pointBot.y = conf::H_ROI;

    this->pointTop.x = this->pointBot.x;
    this->pointTop.y = 0;

    widthBot = conf::DIS_GEN_LINE_BOT;
    widthTop = conf::DIS_GEN_LINE_TOP;
}

void Road::setRight(Lane lane){
    this->right = lane;
    num_lanes ++;
}

void Road::setLeft(Lane lane){
    this->left = lane;
    num_lanes ++;
}
