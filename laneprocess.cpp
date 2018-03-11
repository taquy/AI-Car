#include <laneprocess.h>
#include <lineprocess.h>
#include <pointsprocess.h>
#include <config.h>


// process road injection
int ngaba(std::vector<Lane> &lanes, Road &road){
    unsigned int num_lanes = lanes.size();
    cv::Point R;

    if(num_lanes > 1){
        for(unsigned int i = 0; i < num_lanes-1; i++){
            Lane l1 = lanes[i];

            for(unsigned int k = i + 1; k < num_lanes; k++){
                if(i != k){
                    Lane l2 = lanes[k];

                    // if two line cut together, they may be two line of road injection
                    bool isCut = intersection(R, l1.line, l2.line);

                    if(isCut && checkInRegion(R)){

                        cv::Point p_low;
                        cv::Point p_bot_1 = l1.cnt[0];
                        cv::Point p_bot_2 = l2.cnt[0];

                        if(p_bot_1.y < p_bot_2.y){
                            p_low = p_bot_1;
                        } else {
                            p_low = p_bot_2;
                        }

                        // check if intersection is belong to road injection or not

                        if(distanceTwoPoints(p_low, R) < conf::DIS_ROAD_INJUCTION){

                            int current_x_bot, current_x_top;
                            unsigned int index;

                            if(conf::LEFT){
                                index = l1.line.slope > l2.line.slope ? i : k ;

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
                                index = l1.line.slope < l2.line.slope ? i : k;
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

void separteByDistance(std::vector<Lane> &lanes, Road &road){
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


//! Main separate left and right side
//! You can negative other ways to separate
void separateLeftRight(std::vector<Lane> &lanes, Road &road){

    unsigned int num_lanes = lanes.size();
    // convert line to region line
    for(unsigned int i = 0; i < num_lanes; i++){
        regionLine(lanes[i].line);
    }

    separteByDistance(lanes, road);
}

//! generate the losting line
//! calculate the top point of the car
void genLine(Road &road, std::queue<Road> &road_q){
    if(road.num_lanes != 0){

        Line *line_left = &road.left.line;
        Line *line_right = &road.right.line;

        if(road.num_lanes == 2){

            int X_TOP = (int)(line_left->end.x + line_right->end.x)/2;
            int Y_Top = (int)(line_left->end.y + line_right->end.y)/2;

            road.pointTop.x = X_TOP;
            road.pointTop.y = Y_Top;


        } else {

            if(line_left->start.x != line_left->end.x){

                int DIS = conf::X_CENTER - line_left->start.x;
                road.pointTop.x = line_left->end.x + DIS;

            } else {

                int DIS = line_right->start.x - conf::X_CENTER;
                road.pointTop.x = line_right->end.x - DIS;
            }
        }

         std::cout << road.pointTop << std::endl;
    }

    road_q.pop();
    road_q.push(road);
}

