#include "GameObjectManager.h"
#include "ObjectMessage.h"
#include <mutex>

std::mutex myMutex2; //oh my god I can't be doing this, I need to have a new mutex name for every new mutex I make. Not convenient.

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
	gameObject->ID = gameObjects.size() - 1;
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
	case MessageType::ObjectMessage:
		ObjectMessage& objectmessage = dynamic_cast<ObjectMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.
		
		if (objectmessage.myMessageType == ObjectMessageType::Create)
		{
			CreateGameObject();
		}
		if (objectmessage.myMessageType == ObjectMessageType::Delete)
		{
			DeleteGameObject(objectmessage.gameObjectToDelete);
		}
		break;
	}
}

void GameObjectManager::Process()
{
	while (shouldRun)
	{
		ProcessMessages();
	}
}

void GameObjectManager::QueueMessage(Message* message)
{
	//mutex, since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex2); //wrap into Mutex class.
	queuedMessages.push_back(message);
	//mutex is released when out of scope, lock_guard does this automatically for us.
}

void GameObjectManager::ProcessMessages() //I should probably wrap this into a function inside a processmessages class (OR A MANAGER PARENT CLASS!) so I don't need to copy this same code all the time. Because this code will be the same regardless of manager.
{
	//mutex since we modify the queuedmessages here
	std::lock_guard<std::mutex> guard(myMutex2); //wrap into Mutex class.
	while (queuedMessages.size() >= 1)
	{
		Message* message = queuedMessages.front();
		ProcessMessage(message);
		queuedMessages.erase(std::remove(queuedMessages.begin(), queuedMessages.end(), message));
		delete message;
	}
	//mutex is released when out of scope, lock_guard does this automatically for us.
}
