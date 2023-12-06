#pragma once
#include <list>
#include "Vec2.h"

class DepthFirstSearch{
public:
	void depthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc, std::list<Vec2> nodePositions);
};