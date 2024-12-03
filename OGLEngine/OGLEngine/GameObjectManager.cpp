#include "GameObjectManager.h"

GameObject* GameObjectManager::CreateGameObject()
{
	GameObject* gameObject = new GameObject();
	gameObjects.push_back(gameObject);
	return gameObject;
}

void GameObjectManager::DeleteGameObject(GameObject* gameObject)
{
	//remove gameObject from vector.
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject));

	//release any resources/memory gameObject was using here:
	delete gameObject;
}
