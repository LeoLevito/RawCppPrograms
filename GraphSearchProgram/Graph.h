#pragma once
#include "Node.h"
#include "Vec2.h"
#include <list>
class Graph{
public:
	int startNodeID{};
	int endNodeID{};
	std::list<Node> nodes{};
	std::list<Vec2> nodePositions{};
	void initializeGraphFromFile(const char* path);
};