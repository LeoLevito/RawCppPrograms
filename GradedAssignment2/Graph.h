#pragma once
#include <list>
#include "Node.h"
#include <vector>
class Graph
{
	int V1;
	std::list<int>* adjc;
public:
	Graph(int V1);
	std::list<Node> nodes{};
	void DepthFirstTraversal(int r, std::vector<bool> &visited);
	void addEdge(int p, int q);
	void DFS2();
	void BreadthFirstTraversal(int r, std::vector<bool> &visited);
	void BFS2();
	void InitializeGraphFromFile(const char* path);
};





