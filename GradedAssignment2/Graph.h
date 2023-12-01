#pragma once
#include <list>
#include "Node.h"
#include <vector>
class Graph{
public:
	int startNodeID{};
	int endNodeID{};
	std::list<Node> nodes{};
	void InitializeGraphFromFile(const char* path);
};





