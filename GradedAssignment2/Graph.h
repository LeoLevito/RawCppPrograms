#pragma once
#include "Node.h"
#include <list>
class Graph{
public:
	int startNodeID{};
	int endNodeID{};
	std::list<Node> nodes{};
	void InitializeGraphFromFile(const char* path);
};