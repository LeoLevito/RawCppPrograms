#include "BreadthFirstSearch.h"
#include <vector>
#include <queue>
#include <iostream>
#include "Vec2.h"

void printVisitedNode(std::list<Vec2> nodePositions, int frontOfQueue) {
	auto iterator = nodePositions.begin();
	std::advance(iterator, frontOfQueue);	//advance in the nodes list each loop.
	Vec2 currPos = *iterator;
	std::cout << "visited: " << frontOfQueue << " (" << currPos.x << ", " << currPos.y << ")\n";
}

void printQueuedNode(std::list<Vec2> nodePositions, int backOfQueue) {
	auto iterator = nodePositions.begin();
	std::advance(iterator, backOfQueue);	//advance in the nodes list each loop.
	Vec2 queuedPos = *iterator;
	std::cout << " queued: " << backOfQueue << " (" << queuedPos.x << ", " << queuedPos.y << ")\n";
}

void BreadthFirstSearch::breadthFirstSearch(int nodeID, int endNodeID, size_t nodesListSize, std::list<int>* adjc, std::list<Vec2> nodePositions){ //non-recursive BFS implementation
	//std::cout << "\n \nentered BFS \n";
	std::vector<bool> visited(nodesListSize, false);
	std::queue<int> q{};
	visited[nodeID] = true;
	q.push(nodeID);											
	while (!q.empty()) {
		int front = q.front();
		//printVisitedNode(nodePositions, q.front());

		q.pop();

		if (front == endNodeID) {
			//std::cout << "FOUND THE BFS END NODE " << front << "\n";
			return;
		}
		for (auto i = adjc[front].begin(); i != adjc[front].end(); ++i) {
			if (!visited[*i] && !visited[endNodeID]) {		//stops the while-loop when endNodeID is visited, since the queue becomes empty.
				visited[*i] = true;
				q.push(*i);

				//printQueuedNode(nodePositions, q.back());
			}
		}
	}
}
