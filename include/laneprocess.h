#ifndef LANEPROCESS_H
#define LANEPROCESS_H

#include <iostream>
#include "lane.h"
#include "line.h"
#include "road.h"
#include <queue>

void drawLanes(std::vector<Lane> &lanes);

void combineLanes(std::vector<Lane> &lanes);

int ngaba(std::vector<Lane> &lanes, Road &road);

void separateLeftRightByDistance(std::vector<Lane> &lanes, Road &road);

void genLine(Road &road, std::queue<Road> &road_q);

#endif // LANEPROCESS_H
