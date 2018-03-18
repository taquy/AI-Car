#include "laneprocess.h"
#include "lineprocess.h"
#include "pointsprocess.h"
#include "config.h"

void combineLanes(std::vector<Lane> &lanes){

}

// process road injection
int ngaba(std::vector<Lane> &lanes, Road &road){
    unsigned int num_lanes = lanes.size();


    if(num_lanes > 1){
        cv::Point R;
        for(unsigned int i = 0; i < num_lanes-1; i++){
            Lane *l1 = &lanes[i];

            for(unsigned int k = i + 1; k < num_lanes; k++){
                if(i != k){
                    Lane *l2 = &lanes[k];

                    // if two line cut together, they may be two line of road injection
                    bool isCut = intersection(R, l1->line, l2->line);

                    if(isCut && checkInRegion(R)){
                        cv::Point p_low = l1->cnt[0].y > l2->cnt[0].y ? l1->cnt[0] : l2->cnt[0];
                        // check if intersection is belong to road injection or not
                        int dis = distanceTwoPoints(l1->cnt[0], R) + distanceTwoPoints(l2->cnt[0], R);

                        if(dis < conf::DIS_ROAD_INJUCTION){
                            int current_x_bot, current_x_top;
                            unsigned int index;

                            if(conf::LEFT){
                                index = l1->line.slope > l2->line.slope ? i : k ;
                                lanes[index].cnt.insert(lanes[index].cnt.begin(), p_low);
                                // the taked line is always right side if we want to turn left
                                road.setRight(lanes[index]);

                                // if right side is exists, we find left side
                                // x bottom point and x top point of left side are always less than
                                // x bottom ponnt and x top point of right side
                                current_x_bot = lanes[index].line.start.x;
                                current_x_top = lanes[index].line.end.x;

                                for(unsigned int j = 0; j < lanes.size(); j++){
                                    if(j != index && lanes[j].line.start.x < current_x_bot
                                            && lanes[j].line.end.x < current_x_top){
                                        road.setLeft(lanes[j]);

                                        break;
                                    }
                                }

                            } else {
                                index = l1->line.slope < l2->line.slope ? i : k;
                                lanes[index].cnt.insert(lanes[index].cnt.begin(), p_low);
                                road.setLeft(lanes[index]);

                                //if left line is exists, we find right line
                                current_x_bot = lanes[index].line.start.x;
                                current_x_top = lanes[index].line.end.x;

                                for(unsigned int j = 0; j < lanes.size(); j++){
                                    if(j != index && lanes[j].line.start.x > current_x_bot
                                            && lanes[j].line.end.x > current_x_top){
                                        road.setRight(lanes[j]);

                                        break;
                                    }
                                }

                            }

                            return 1;
                        }
                    }
                }
            }
        }
    }

    return 0;

}


//! separte left side and right side by distance from center two to the lines
/*!
 * \param lanes     each lane consists cluster of points and a line fit the points
 * \param road
 */

void separateLeftRightByDistance(std::vector<Lane> &lanes, Road &road){
    unsigned int num_lanes = lanes.size();
    int min_gap_left = conf::WIDTH;
    int min_gap_right = conf::HEIGHT;

    int index_left = -1;
    int index_right = -1;

    for(unsigned int i = 0; i < num_lanes; i++){

        int gap = conf::X_CENTER - lanes[i].line.start.x;

        if(gap > 0 && gap < min_gap_left){
            min_gap_left = gap;
            index_left = i;
        }

        if(gap < 0 && cv::abs(gap) < min_gap_right){
            min_gap_right = cv::abs(gap);
            index_right = i;
        }
    }

    if(index_left != -1){
        road.setLeft(lanes[index_left]);
    }

    if(index_right != -1){
        road.setRight(lanes[index_right]);
    }
}

//! generate the losting line
//! calculate the top point of the car
void genLine(Road &road, std::queue<Road> &road_q){

    if(road.hasLeft && road.hasRight){

        Line *line_left = &road.left.line;
        Line *line_right = &road.right.line;

        road.pointTop.x= (line_left->end.x + line_right->end.x)/2;
        road.pointTop.y = (line_left->end.y + line_right->end.y)/2;

    } else {

        if(road.hasLeft){
            Line *line_left = &road.left.line;
            int DIS = conf::X_CENTER - line_left->start.x;
            road.pointTop.x = line_left->end.x + DIS;
        }

        if(road.hasRight){
            Line *line_right = &road.right.line;
            int DIS = line_right->start.x - conf::X_CENTER;
            road.pointTop.x = line_right->end.x - DIS;
        }
    }

    road_q.pop();
    road_q.push(road);
}

