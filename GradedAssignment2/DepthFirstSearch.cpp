#include "DepthFirstSearch.h"
#include <vector>
#include <stack>
#include <iostream>

void DepthFirstSearch::depthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int> *adjc) {	//non-recursive DFS implementation
	std::vector<bool> visited(nodesListSize, false);
	std::stack<int> stack;
	stack.push(nodeID);
																													//std::cout << "\n \n entered DFS \n";
	while (!stack.empty()) {
		nodeID = stack.top();																						//top of stack
		stack.pop();
		if (!visited[nodeID] && !visited[endNodeID]) {																//stops the while loop when endNodeID is visited, since the stack becomes empty.
																													//std::cout << "visited: " << nodeID << "\n";
			visited[nodeID] = true;
			for (auto i = adjc[nodeID].begin(); i != adjc[nodeID].end(); ++i) {
				stack.push(*i);
			}
		}
	}
}