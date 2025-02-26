#include "GameObjectManager.h"
#include "ObjectMessage.h"
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>

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


void GameObjectManager::Serialization(const std::string& filename)
{
	std::fstream file;

	file.open(filename.c_str(), std::ios_base::out | std::ios_base::binary); //binary files require the use of 
	////write meta-data first, if you want
	if (file.is_open())
	{
		//get size of vectors to integers.
		int gameObjectsSize = gameObjects.size();

		//write sizes of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&gameObjectsSize), sizeof(gameObjectsSize));

		//write contents of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&gameObjects[0]), sizeof(GameObject*) * gameObjects.size());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			//get size of vectors to integers.
			int nameSize = gameObjects[i]->name.size();
			int IDSize = gameObjects[i]->ID;
			//int componentsSize = go->components.size();

			//write sizes of vectors, https://stackoverflow.com/a/31213593
			file.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
			file.write(reinterpret_cast<char*>(&IDSize), sizeof(IDSize));
			//file.write(reinterpret_cast<char*>(&componentsSize), sizeof(componentsSize));


			//write contents of vectors, https://stackoverflow.com/a/31213593
			file.write(reinterpret_cast<char*>(&gameObjects[i]->name[0]), sizeof(std::string) * gameObjects[i]->name.size());
			file.write(reinterpret_cast<char*>(&gameObjects[i]->ID), sizeof(int) * gameObjects[i]->ID);
			//file.write(reinterpret_cast<char*>(&go->components[0]), sizeof(Component*) * go->components.size());
		}
	}
	file.close();
}

void GameObjectManager::Deserialization(const std::string& filename)
{
	std::cout << "Starting deserialization of gameobjects" << std::endl;
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	////read meta-data if you wrote that.
	if (file.is_open())
	{
		int gameObjectsSize;

		//read sizes of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&gameObjectsSize), sizeof(gameObjectsSize)); //I see the issue now, we don't know the size of the vector when we want to read it, and since we input the size of an empty vector right now, it's gonna return nothing as well. so maybe we need to write the size to the file for us to retrieve when reading it.


		//resize vectors to the size read from binary file.
		gameObjects.resize(gameObjectsSize);


		//read contents of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&gameObjects[0]), sizeof(GameObject*) * gameObjects.size());

		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i] = new GameObject(); //multithreading error? Clicking continue here sometimes makes the program work again, if no other errors occur.

			int nameSize;
			int IDSize;
			//int componentsSize;

			//read sizes of vectors, https://stackoverflow.com/a/31213593
			file.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize)); //I see the issue now, we don't know the size of the vector when we want to read it, and since we input the size of an empty vector right now, it's gonna return nothing as well. so maybe we need to write the size to the file for us to retrieve when reading it.
			file.read(reinterpret_cast<char*>(&IDSize), sizeof(IDSize));
			//file.read(reinterpret_cast<char*>(&componentsSize), sizeof(componentsSize));

			//resize vectors to the size read from binary file.
			gameObjects[i]->name.resize(nameSize);
			gameObjects[i]->ID = IDSize;
			//gameObject.components.resize(componentsSize);

			//read contents of vectors, https://stackoverflow.com/a/31213593
			file.read(reinterpret_cast<char*>(&gameObjects[i]->name[0]), sizeof(std::string) * gameObjects[i]->name.size());
			file.read(reinterpret_cast<char*>(&gameObjects[i]->ID), sizeof(int) * gameObjects[i]->ID);
			//file.read(reinterpret_cast<char*>(&gameObject.components[0]), sizeof(Component*) * gameObject.components.size());
		}
	}
	file.close();
}
