#pragma once
#include <string>
#include <vector>
#include "GameObject.h"
#include "GameObjectManager.h"

class Level
{
public: 
	Level();
	bool LoadLevel(const std::string& filename); //Maybe these are better suited for a level manager?
	bool SaveLevel(const std::string& filename); //Maybe these are better suited for a level manager?
private:
	std::string name;

	std::vector<GameObject*> gameObjects; //should update GameObjectManager, or maybe read from it?
};

