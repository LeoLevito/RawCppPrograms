#include "BreadthFirstSearch.h"
#include <vector>
#include <queue>
#include <iostream>

void BreadthFirstSearch::breadthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc) {
	std::vector<bool> visited(nodesListSize, false);
	std::queue<int> q{};
	visited[nodeID] = true;
	q.push(nodeID);
																		//std::cout << "\n \nentered BFS \n";
	while (!q.empty()) {
		int front = q.front();
		std::cout << "visited: " << q.front() << "\n";
		q.pop();
		if (front == endNodeID) {
																		//std::cout << "FOUND THE BFS END NODE " << front << "\n";
			return;
		}
		for (auto i = adjc[front].begin(); i != adjc[front].end(); ++i) {
			if (!visited[*i] && !visited[endNodeID]) {					//stops the while-loop when endNodeID is visited, since the queue becomes empty.
				visited[*i] = true;
				q.push(*i);
				std::cout << " queued: " << q.back() << "\n";
			}
		}
	}
}