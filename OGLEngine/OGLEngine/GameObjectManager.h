#pragma once
#include "GameObject.h"
#include <vector>
class GameObjectManager
{
public:
	GameObject* CreateGameObject();
	void DeleteGameObject(GameObject* gameObject);

	std::vector<GameObject*> gameObjects;
private:
};

