#include "DepthFirstSearch.h"
#include <vector>
#include <stack>
#include <iostream>

#include <list>
//void DepthFirstSearch::depthFirstSearch(int nodeID, int size, int endNodeID, std::list<int>* adjc) {
//	std::vector<bool> visited(size, false);
//	std::stack<int> stack;
//	stack.push(nodeID);
//	std::cout << "\n \n entered DFS \n";
//	while (!stack.empty()) {
//		nodeID = stack.top(); //top of stack
//		stack.pop();
//		if (!visited[nodeID] && !visited[endNodeID]) { //stops the while loop when endNodeID is visited, since the stack becomes empty (I believe).
//			std::cout << "visited: " << nodeID << "\n";
//			visited[nodeID] = true;
//			for (auto i = adjc[nodeID].begin(); i != adjc[nodeID].end(); ++i) {
//				stack.push(*i);
//			}
//		}
//	}
//}
