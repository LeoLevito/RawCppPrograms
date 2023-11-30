#include "Graph.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>
int startNodeID{};
int endNodeID{};
bool foundEndNode{ false };

auto currentPointInTime() {
	return  std::chrono::steady_clock::now();
}

Graph::Graph(int V1) {
	this->V1 = V1;
	adjc = new std::list<int>[V1];
}

void Graph::addEdge(int p, int q) {
	adjc[p].push_back(q);
}

void Graph::DepthFirstTraversal(int r, std::vector<bool>& visited) {
	std::stack<int> stack;
	stack.push(r);
	//std::cout << "\n \n entered DFS ";
	while (!stack.empty()) {
		r = stack.top();
		stack.pop();
		if (!visited[r] && !visited[endNodeID]) { //stops the while loop when endNodeID is visited, since the stack becomes empty (I believe).
			//std::cout << "visited: " << r << "\n";
			visited[r] = true;
			for (auto i = adjc[r].begin(); i != adjc[r].end(); ++i) {
				stack.push(*i);
			}
		}
	}
}

void Graph::DFS2() {
	//std::cout << "\nDepth first traversal\n";
	std::vector<bool> visited(V1, false);
	//for (int i = startNodeID; i < V1; i++) {
		if (!visited[startNodeID] && !visited[endNodeID]) {
			//std::cout << "\n \n Calling DFS function?";
			DepthFirstTraversal(startNodeID, visited);	
		}
	//}
}

void Graph::BreadthFirstTraversal(int r, std::vector<bool>& visited) {
	std::queue<int> q{};
	visited[r] = true;
	q.push(r);
	//std::cout << "\n \n entered BFS ";
	while (!q.empty()) {
		int front = q.front();
		//std::cout << "visited: " << q.front() << "\n ";
		q.pop();
		if (front == endNodeID) {
			//std::cout << "FOUND THE BFS END NODE " << front;
			foundEndNode = true;
		}
		for (auto i = adjc[front].begin(); i != adjc[front].end(); ++i) {
			if (!visited[*i] && !visited[endNodeID]) { //stops the while loop when endNodeID is visited, since the queue becomes empty (I believe).
				visited[*i] = true;
				q.push(*i);
				//std::cout << "queued: " << q.back() << "\n ";
			}
		}
	}
}

void Graph::BFS2() {
	//std::cout << "\nBreadth first traversal\n";
	std::vector<bool> visited(V1, false);
	//for (int i = startNodeID; i < V1; i++) {
		if (!visited[startNodeID]/* && !foundEndNode*/) {
			//std::cout << "\n \n Calling BFS function?";
			BreadthFirstTraversal(startNodeID, visited);
		}
	//}
}

void Graph::InitializeGraphFromFile(const char* path)
{
	// This will not really be a node graph. More like a grid
	// Parse the text file and create nodes. On 'o' create a node. On 'X' don't do anything
	int graphNodeIds = 0;
	float xPos = 0;
	float yPos = 0;
	std::string line{};
	std::ifstream myFile(path);
	if (myFile.is_open())
	{
		while (std::getline(myFile, line))
		{
			std::cout << line << "\n";
			if (yPos < 20) { //stop from going to text below the grid.

				// Parse and create nodes
				for (char c : line)
				{
					if (c != 'X')
					{
						if (c == 'S') {
							startNodeID = graphNodeIds;
							Node* gNode = new Node({ xPos, yPos }, graphNodeIds);
							nodes.push_back(*gNode);

							++graphNodeIds;
						}
						else if (c == 'G') {
							endNodeID = graphNodeIds;
							Node* gNode = new Node({ xPos, yPos }, graphNodeIds);
							nodes.push_back(*gNode);
							++graphNodeIds;
						}
						else {
							Node* gNode = new Node({ xPos, yPos }, graphNodeIds);
							nodes.push_back(*gNode);
							++graphNodeIds;
						}
					}
					++xPos;
				}
				// New line
				xPos = 0;
				++yPos;
			}
		}
		myFile.close();
	}
	else
	{
		std::cout << "Error: Unable to open file! '" << path << "'\n";
		return;
	}

	for (std::size_t i = 0; i < nodes.size(); ++i)
	{
		auto iterator = nodes.begin();
		std::advance(iterator, i); //advance in the nodes list
		Node node = *iterator;
		std::cout << "\nid: " << node.id << " " << "| pos: (" << node.position.x << ", " << node.position.y << ")\n";

		//neighbor positions, to be checked for later.
		const Vec2 nodePos = node.position;
		const Vec2 upNeighborPos = { nodePos.x, nodePos.y - 1 };
		const Vec2 rightNeighborPos = { nodePos.x + 1, nodePos.y };
		const Vec2 downNeighborPos = { nodePos.x, nodePos.y + 1 };
		const Vec2 leftNeighborPos = { nodePos.x - 1, nodePos.y };

		// Nested for-loop. Do not bother with map now. Optimization wont be necessary. This runs once
		for (std::size_t n = 0; n < nodes.size(); n++)
		{
			auto itn = nodes.begin();
			std::advance(itn, n);
			Node node2 = *itn;
			Vec2 pos = node2.position;
			//std::cout << "(" << pos.x << ", " << pos.y <<")\n";
			if (pos.x == upNeighborPos.x && pos.y == upNeighborPos.y) {
				addEdge(node.id, node2.id);
				std::cout << "UP add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == rightNeighborPos.x && pos.y == rightNeighborPos.y) {
				addEdge(node.id, node2.id);
				std::cout << "RIGHT add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == downNeighborPos.x && pos.y == downNeighborPos.y) {
				addEdge(node.id, node2.id);
				std::cout << "DOWN add edge: " << node.id << "-" << node2.id << "\n";
			}
			else if (pos.x == leftNeighborPos.x && pos.y == leftNeighborPos.y) {
				addEdge(node.id, node2.id);
				std::cout << "LEFT add edge: " << node.id << "-" << node2.id << "\n";
			}
		}
	}
}

int main() {
	Graph graph(328); //woah this isn't good, this needs to be the exact value of how many nodes that will be created,
	//any lower and an error occurs, any higher and we get extra independent nodes that the breadth 
	//first and depth first searches just runs through but this can be confusing. 
	//(EDIT, now searches actually stop when hitting the endNodeID, so that's remedied.)
	//This needs to be remedied.

	graph.InitializeGraphFromFile("AssignmentNodes.txt");

	const auto start1{ currentPointInTime() };
	graph.DFS2();
	const auto end1{ currentPointInTime() };
	const std::chrono::duration<double, std::milli> elapsedSeconds1{ end1 - start1 };
	std::cout << "\nTime for depth first search: " << elapsedSeconds1 << " \n";

	const auto start2{ currentPointInTime() };
	graph.BFS2();
	const auto end2{ currentPointInTime() };
	const std::chrono::duration<double, std::milli> elapsedSeconds2{ end2 - start2 };
	std::cout << "\nTime for breadth first search: " << elapsedSeconds2 << " \n";

	return 0;
}

