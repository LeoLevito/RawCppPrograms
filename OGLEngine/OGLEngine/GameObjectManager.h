#pragma once
#include "GameObject.h"
#include <vector>
#include "Message.h"
class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
public:
	static GameObjectManager& Get();
	GameObject* CreateGameObject();
	void DeleteGameObject(GameObject* gameObject);

	std::vector<GameObject*> gameObjects;

	void ProcessMessage(Message* message);
private:
};

