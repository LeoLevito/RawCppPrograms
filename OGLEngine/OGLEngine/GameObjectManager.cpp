#include "GameObjectManager.h"

GameObjectManager::GameObjectManager()
{
}

GameObjectManager::~GameObjectManager()
{
}

GameObjectManager& GameObjectManager::Get()
{
	static GameObjectManager instance;
	return instance;
}

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

void GameObjectManager::ProcessMessage(Message* message)
{
	std::string& msg = message->msg;
	switch (message->type) 
	{
	case MessageType::String:
		if (msg == "StartProcess1") 
		{
			//StartProcess1();
		}
		else if (msg == "StopProcess1")
		{
			//StopProcess1();
		}
		break;

	case MessageType::Raycast:
		//do raycast immediately and reply result,
		//.. or queue it up for processing.
		//it is up to us to reply asynchronously with a messaging system like this.

		break;
	}

}
