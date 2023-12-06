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

#define NAMEOF(variable) ((decltype(&variable))nullptr, #variable)

/*---------------------------------------------------------------------------------------------------------------------
	This program reads a graph from a text file and assigns nodes to the elements
	in that graph and then performs a Depth-first search and a Breadth-first 
	search multiple times and measures the average time taken to traverse 
	the graph from a pre-specified start node to a pre-specified end node.
---------------------------------------------------------------------------------------------------------------------*/

Graph graph{};
DepthFirstSearch DFS{};
BreadthFirstSearch BFS{};

std::size_t nodesListSize{};
std::list<int>* adjc;

int repeatMeasurements{ 1000 };
std::chrono::duration<double, std::milli> totalSearchTime{};
std::chrono::duration<double, std::milli> zero{ std::chrono::duration<double>::zero() };

bool onlyShowAverages{ false };

auto currentPointInTime() {
	return  std::chrono::steady_clock::now();
}

void addNeighbor(int p, int q) {
	adjc[p].push_back(q);
}

void performSpecifiedSearchMethod(int searchMethod) {
	switch (searchMethod) {
	case 1:
		DFS.depthFirstSearch(graph.startNodeID, graph.endNodeID, nodesListSize, adjc, graph.nodePositions);
		break;
	case 2:
		BFS.breadthFirstSearch(graph.startNodeID, graph.endNodeID, nodesListSize, adjc, graph.nodePositions);
		break;
	}
}

void runSearchAndTakeTime(int searchMethod, std::string searchMethodName) {
	for (int i = 0; i < repeatMeasurements; i++) {
		const auto start{ currentPointInTime() };
		performSpecifiedSearchMethod(searchMethod);			// perform specified search method
		const auto end{ currentPointInTime() };
		const std::chrono::duration<double, std::milli> elapsedSeconds{ end - start };
		totalSearchTime = totalSearchTime + elapsedSeconds;
		if (!onlyShowAverages) {
			std::cout << "\nTime for " << searchMethodName << " (cycle #" << i + 1 << "): " << elapsedSeconds;
		}
	}
	std::cout << "\nAverage time for " << searchMethodName << " (" << repeatMeasurements << " repeat measurements)" << ": " << totalSearchTime / repeatMeasurements << "\n";
	totalSearchTime = zero;
}

void specifyNeighbors() {
	//after nodes list has been filled, resize the adjc list according to nodes list size.
	nodesListSize = graph.nodes.size();
	adjc = new std::list<int>[nodesListSize];
	adjc->resize(nodesListSize);

	for (std::size_t i = 0; i < graph.nodes.size(); i++){
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

		for (std::size_t n = 0; n < graph.nodes.size(); n++){
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
	std::cout << "/*-------------------------------------------------------------------------------------------------\n"
		"This program reads a graph from a text file and assigns nodes to the elements\n"
		"in that graph. It then performs a Depth-first search and a Breadth-first search\n"
		"from a pre-specified start node to a pre-specified end node. This is repeated multiple\n"
		"times to calculate the average time taken to traverse the graph for each search algorithm.\n"
		"-------------------------------------------------------------------------------------------------*/ \n\n";

	std::cout << "How many times do you want to repeat the time measurements?" <<
		"\n--Insert amount here: ";
	std::cin >> repeatMeasurements;

	std::cout << "\nDo you want to only show the average times, or show times for each repeat measurement as well?" <<
		"\nInserting [1] will only show average times, else Inserting [0] will show all time measurements." <<
		"\n--Insert [1] or [0] here: ";
	std::cin >> onlyShowAverages;

	graph.initializeGraphFromFile("AssignmentNodes.txt");
	specifyNeighbors();

	int depthFirstSearch{ 1 };
	int breadthFirstSearch{ 2 };

	runSearchAndTakeTime(depthFirstSearch, NAMEOF(depthFirstSearch));
	runSearchAndTakeTime(breadthFirstSearch, NAMEOF(breadthFirstSearch));

	return 0;
}