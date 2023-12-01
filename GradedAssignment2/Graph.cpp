#include "Graph.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>

void Graph::InitializeGraphFromFile(const char* path/*, int startNodeID, int endNodeID*/)
{
	// This will not really be a node graph. More like a grid
	// Parse the text file and create nodes. On 'o' create a node. On 'X' don't do anything
	int graphNodeIds = 0;
	float xPos = 0;
	float yPos = 0;
	std::string line{};
	std::ifstream myFile(path);
	if (myFile.is_open()){
		while (std::getline(myFile, line)){
			//std::cout << line << "\n";
			if (yPos < 20) { //stop from going to text below the grid.
				// Parse and create nodes
				for (char c : line){
					if (c != 'X'){
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
	else{
		std::cout << "Error: Unable to open file! '" << path << "'\n";
		return;
	}
}

