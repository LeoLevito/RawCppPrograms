#include "DepthFirstSearch.h"
#include <vector>
#include <stack>
#include <iostream>
#include "Vec2.h"

void printVisitedNodes(std::list<Vec2> nodePositions, int nodeID) {
	auto iterator = nodePositions.begin();
	std::advance(iterator, nodeID);						//advance in the nodes list each loop.
	Vec2 currPos = *iterator;
	std::cout << "visited: " << nodeID << " (" << currPos.x << ", " << currPos.y << ")\n";
}

void DepthFirstSearch::depthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int> *adjc, std::list<Vec2> nodePositions) {	//non-recursive DFS implementation
	std::vector<bool> visited(nodesListSize, false);
	std::stack<int> stack;
	stack.push(nodeID);
																													//std::cout << "\n \n entered DFS \n";
	while (!stack.empty()) {
		nodeID = stack.top();																						//top of stack
		stack.pop();
		if (!visited[nodeID] && !visited[endNodeID]) {																//stops the while loop when endNodeID is visited, since the stack becomes empty.
			
			//printVisitedNodes(nodePositions, nodeID);
			visited[nodeID] = true;
			for (auto i = adjc[nodeID].begin(); i != adjc[nodeID].end(); ++i) {
				stack.push(*i);
			}
		}
	}
}

