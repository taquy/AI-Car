#include <road.h>
#include <config.h>

Road::Road(){
    this->pointBot.x =  (int)(conf::W_ROI/2);
    this->pointBot.y = conf::H_ROI;

    this->pointTop.x = this->pointBot.x;
    this->pointTop.y = conf::DOWN_Y;

    this->hasLeft = false;
    this->hasRight = false;
}

void Road::setRight(Lane lane){
    this->right = lane;
    this->hasRight = true;
}

void Road::setLeft(Lane lane){
    this->left = lane;
    this->hasLeft = true;
}
