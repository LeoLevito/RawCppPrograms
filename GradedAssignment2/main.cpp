#include "Graph.h"
#include "DepthFirstSearch.h"
#include "BreadthFirstSearch.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>

Graph graph{};
DepthFirstSearch DFS{};
BreadthFirstSearch BFS{};

std::size_t nodesListSize{};
std::list<int>* adjc;

auto currentPointInTime() {
	return  std::chrono::steady_clock::now();
}

void addNeighbor(int p, int q) {
	adjc[p].push_back(q);
}

void performSpecifiedSearchMethod(int searchMethod) {
	switch (searchMethod) {
	case 1:
		DFS.depthFirstSearch(graph.startNodeID, graph.endNodeID, nodesListSize, adjc);
		break;
	case 2:
		BFS.breadthFirstSearch(graph.startNodeID, graph.endNodeID, nodesListSize, adjc);
		break;
	}
}

std::chrono::duration<double, std::milli> searchReturnTime(int searchMethod) {
	const auto start{ currentPointInTime() };
	performSpecifiedSearchMethod(searchMethod);			// perform specified search method
	const auto end{ currentPointInTime() };
	const std::chrono::duration<double, std::milli> elapsedSeconds{ end - start };
	return elapsedSeconds;
}

void specifyNeighbors() {
														//after nodes list has been filled, resize the adjc list according to nodes list size.
	nodesListSize = graph.nodes.size();
	adjc = new std::list<int>[nodesListSize];
	adjc->resize(nodesListSize);

	for (std::size_t i = 0; i < graph.nodes.size(); i++)
	{
		auto iterator = graph.nodes.begin();
		std::advance(iterator, i);						//advance in the nodes list each loop.
		Node node = *iterator;
		Vec2 nodePos = node.position;
														//std::cout << "\nid: " << node.id << " " << "| pos: (" << node.position.x << ", " << node.position.y << ")\n";

														//neighbor positions, used in the coming for-loop.	
		const Vec2 upNeighborPos = { nodePos.x, nodePos.y - 1 };
		const Vec2 rightNeighborPos = { nodePos.x + 1, nodePos.y };
		const Vec2 downNeighborPos = { nodePos.x, nodePos.y + 1 };
		const Vec2 leftNeighborPos = { nodePos.x - 1, nodePos.y };

		for (std::size_t n = 0; n < graph.nodes.size(); n++)
		{
			auto iterator2 = graph.nodes.begin();
			std::advance(iterator2, n);					//advance in the nodes list each loop, again.
			Node node2 = *iterator2;
			Vec2 node2Pos = node2.position;

			if (node2Pos.isEqualTo(upNeighborPos)) {
				addNeighbor(node.id, node2.id);
														//std::cout << "UP add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (node2Pos.isEqualTo(rightNeighborPos)) {
				addNeighbor(node.id, node2.id);
														//std::cout << "RIGHT add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (node2Pos.isEqualTo(downNeighborPos)) {
				addNeighbor(node.id, node2.id);
														//std::cout << "DOWN add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (node2Pos.isEqualTo(leftNeighborPos)) {
				addNeighbor(node.id, node2.id);
														//std::cout << "LEFT add edge: " << node.id << "-" << node2.id << "\n";
			}
		}
	}
}

int main() {
	graph.InitializeGraphFromFile("AssignmentNodes.txt");
	specifyNeighbors();

	int depthFirstSearch{ 1 };
	int breadthFirstSearch{ 2 };

	std::cout << "\nTime for Depth-first search: " << searchReturnTime(depthFirstSearch) << " \n";
	std::cout << "\nTime for Breadth-first search: " << searchReturnTime(breadthFirstSearch) << " \n";

	return 0;
}