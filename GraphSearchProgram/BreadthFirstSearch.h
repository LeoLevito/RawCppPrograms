#pragma once
#include <list>
#include "Vec2.h"

class BreadthFirstSearch{
public:
	void breadthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc, std::list<Vec2> nodePositions);
};