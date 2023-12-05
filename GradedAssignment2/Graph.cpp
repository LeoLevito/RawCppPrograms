#include "Graph.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>

void Graph::initializeGraphFromFile(const char* path)
{
	int graphNodeID = 0;
	float xPos = 0;
	float yPos = 0;
	std::string line{};
	std::ifstream myFile(path);

	if (myFile.is_open()){
		while (std::getline(myFile, line)){
																				//std::cout << line << "\n";
			if (yPos < 20) {													//stop it from reading the text "Load 'o's as nodes, 'X's are walls...", since that is not part of our graph.
				for (char c : line){											//create nodes for each char c except for the 'X' walls.
					if (c != 'X'){
						if (c == 'S') {
							startNodeID = graphNodeID;							//save startNodeID
						}
						if (c == 'G') {
							endNodeID = graphNodeID;							//save endNodeID
						}
						Node* gNode = new Node({ xPos, yPos }, graphNodeID);	//create new node with position of (xPos, yPos) and apply the current ID value. 														
						nodes.push_back(*gNode);
						++graphNodeID;											//NOTE how IDs are assigned: from left to right on each line in the text file, ID increases with each node that's added.
					}
					++xPos;
				}
				xPos = 0;														//New line
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