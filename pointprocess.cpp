#include <iostream>
#include <pointsprocess.h>
#include <line.h>
#include <config.h>

int distanceTwoPoints(cv::Point &p1, cv::Point &p2){
    return cv::abs(p1.x - p2.x) + cv::abs(p1.y - p2.y);
}

//! This is a recursive function. It finds a point of current layer which is nearest with
//! current point of previous layer
void findRelatedPoint(std::vector<Layer> &layers, std::vector<cv::Point> &points, unsigned int current_layer){

    unsigned int size_points = layers[current_layer].points.size();

    if(size_points > 0){
        int minDistance = conf::MIN_DIS_TWO_POINTS;
        int index = -1;
        cv::Point p_end = points[points.size() - 1];


        for(unsigned int i = 0; i < size_points; i++){
            int distance = distanceTwoPoints(layers[current_layer].points[i], p_end);

            if(distance < minDistance){
                minDistance = distance;
                index = i;
            }
        }

        if(index != -1){
            points.push_back(layers[current_layer].points[index]);
            layers[current_layer].points.erase(layers[current_layer].points.begin() + index);
        }
    }

    if(current_layer < layers.size() - 1){
        findRelatedPoint(layers, points, ++current_layer);
    }

}

//! find cluster of points (Lane object)
/*!
 * \param layers
 * \param dst_lanes
 * \param isDebug
 */
void pointsToLane(std::vector<Layer> &layers, std::vector<Lane> &dst_lanes){

    // convert all points of layers to original coornidations;
    for(Layer &l : layers){
       l.toOrigin();
    }


    // find clusters of points
    for(unsigned int i = 0; i < layers.size() - 1; i++){
        unsigned int size_points = layers[i].points.size();

        if(size_points > 0){
            for(unsigned int j = 0; j < size_points; j++){

                std::vector<cv::Point> x;
                x.push_back(layers[i].points[j]);

                findRelatedPoint(layers, x, i+1);
                if(x.size() > conf::MIN_POINTS && x[0].y > conf::MIN_Y_LANE){
                    dst_lanes.push_back(Lane(x));
                }
            }
        }
    }
}
