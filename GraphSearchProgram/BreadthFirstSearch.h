#pragma once
#include <list>

class BreadthFirstSearch{
public:
	void breadthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc);
};