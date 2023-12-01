#include "Graph.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>

bool foundEndNode{ false };
std::size_t size{};
std::list<int>* adjc;
Graph graph{};

auto currentPointInTime() {
	return  std::chrono::steady_clock::now();
}

void addEdge(int p, int q) {
	adjc[p].push_back(q);
}

void DepthFirstTraversal(int nodeID) {
	std::vector<bool> visited(size, false);
	std::stack<int> stack;
	stack.push(nodeID);
	std::cout << "\n \n entered DFS \n";
	while (!stack.empty()) {
		nodeID = stack.top(); //top of stack
		stack.pop();
		if (!visited[nodeID] && !visited[graph.endNodeID]) { //stops the while loop when endNodeID is visited, since the stack becomes empty (I believe).
			std::cout << "visited: " << nodeID << "\n";
			visited[nodeID] = true;
			for (auto i = adjc[nodeID].begin(); i != adjc[nodeID].end(); ++i) {
				stack.push(*i);
			}
		}
	}
}

void BreadthFirstTraversal(int r) {
	std::vector<bool> visited(size, false);
	std::queue<int> q{};
	visited[r] = true;
	q.push(r);
	std::cout << "\n \n entered BFS \n";
	while (!q.empty()) {
		int front = q.front();
		std::cout << "visited: " << q.front() << "\n";
		q.pop();
		if (front == graph.endNodeID) {
			std::cout << "FOUND THE BFS END NODE " << front;
			foundEndNode = true; //this has no effect right now.
		}
		for (auto i = adjc[front].begin(); i != adjc[front].end(); ++i) {
			if (!visited[*i] && !visited[graph.endNodeID]) { //stops the while loop when endNodeID is visited, since the queue becomes empty (I believe).
				visited[*i] = true;
				q.push(*i);
				std::cout << " queued: " << q.back() << "\n";
			}
		}
	}
}
void foo() {
	//after nodes list has been filled, resize the adjc list according to nodes list size.
	size = graph.nodes.size();
	adjc = new std::list<int>[size];
	adjc->resize(size);


	for (std::size_t i = 0; i < graph.nodes.size(); ++i)
	{
		auto iterator = graph.nodes.begin();
		std::advance(iterator, i); //advance in the nodes list
		Node node = *iterator;
		//std::cout << "\nid: " << node.id << " " << "| pos: (" << node.position.x << ", " << node.position.y << ")\n";

		//neighbor positions, to be checked for later.
		const Vec2 nodePos = node.position;
		const Vec2 upNeighborPos = { nodePos.x, nodePos.y - 1 };
		const Vec2 rightNeighborPos = { nodePos.x + 1, nodePos.y };
		const Vec2 downNeighborPos = { nodePos.x, nodePos.y + 1 };
		const Vec2 leftNeighborPos = { nodePos.x - 1, nodePos.y };

		// Nested for-loop. Do not bother with map now. Optimization wont be necessary. This runs once
		for (std::size_t n = 0; n < graph.nodes.size(); n++)
		{
			auto itn = graph.nodes.begin();
			std::advance(itn, n);
			Node node2 = *itn;
			Vec2 pos = node2.position;
			//std::cout << "(" << pos.x << ", " << pos.y <<")\n";
			if (pos.x == upNeighborPos.x && pos.y == upNeighborPos.y) {
				addEdge(node.id, node2.id);
				//std::cout << "UP add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == rightNeighborPos.x && pos.y == rightNeighborPos.y) {
				addEdge(node.id, node2.id);
				//std::cout << "RIGHT add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == downNeighborPos.x && pos.y == downNeighborPos.y) {
				addEdge(node.id, node2.id);
				//std::cout << "DOWN add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == leftNeighborPos.x && pos.y == leftNeighborPos.y) {
				addEdge(node.id, node2.id);
				//std::cout << "LEFT add edge: " << node.id << "-" << node2.id << "\n";
			}
		}
	}
}

int main() {


	graph.InitializeGraphFromFile("AssignmentNodes.txt");
	foo();

	const auto start1{ currentPointInTime() };
	DepthFirstTraversal(graph.startNodeID);
	const auto end1{ currentPointInTime() };
	const std::chrono::duration<double, std::milli> elapsedSeconds1{ end1 - start1 };
	std::cout << "\nTime for Depth-first search: " << elapsedSeconds1 << " \n";

	const auto start2{ currentPointInTime() };
	BreadthFirstTraversal(graph.startNodeID);
	const auto end2{ currentPointInTime() };
	const std::chrono::duration<double, std::milli> elapsedSeconds2{ end2 - start2 };
	std::cout << "\nTime for Breadth-first search: " << elapsedSeconds2 << " \n";

	return 0;
}






