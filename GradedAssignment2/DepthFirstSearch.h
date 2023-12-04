#pragma once
#include <list>

class DepthFirstSearch{
public:
	void depthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc);
};