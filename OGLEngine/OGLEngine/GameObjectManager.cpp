#include "GameObjectManager.h"
#include "ObjectMessage.h"
#include "LevelMessage.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "LightComponent.h"
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>

std::mutex myMutex2; //oh my god I can't be doing this, I need to have a new mutex name for every new mutex I make. Not convenient.

GameObjectManager::GameObjectManager()
{
	gameObjects.reserve(3 * sizeof(GameObject*));
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

	std::cout << gameObjects.back() << std::endl;
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
	{
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
	case MessageType::LevelMessage:
	{
		LevelMessage& levelmessage = dynamic_cast<LevelMessage&>(*message); //is this bad practice for messaging? RE:(MeshComponent), apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times. Plus this cast only happens when processing messages, so not too frequently.

		if (levelmessage.myMessageType == LevelMessageType::Save)
		{
			Serialization("../Levels/LevelSaveTest2.scene");
		}
		if (levelmessage.myMessageType == LevelMessageType::Load)
		{
			Deserialization("../Levels/LevelSaveTest2.scene");
		}
		break;
	}
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
	file.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);

	if (file.is_open())
	{
		int gameObjectsSize = gameObjects.size();
		file.write(reinterpret_cast<char*>(&gameObjectsSize), sizeof(int));

		for (int i = 0; i < gameObjectsSize; i++)
		{
			gameObjects[i]->Serialization(file);

			for (int j = 0; j < gameObjects[i]->components.size(); j++)
			{
				int componentType = static_cast<int>(gameObjects[i]->components[j]->type);
				file.write(reinterpret_cast<char*>(&componentType), sizeof(int));

				switch (gameObjects[i]->components[j]->type)
				{
				case ComponentType::Transform:
				{
					dynamic_cast<TransformComponent*>(gameObjects[i]->components[j])->Serialization(file);
					break;
				}
				case ComponentType::Mesh:
				{
					dynamic_cast<MeshComponent*>(gameObjects[i]->components[j])->Serialization(file);
					break;
				}
				case ComponentType::Light:
				{
					dynamic_cast<LightComponent*>(gameObjects[i]->components[j])->Serialization(file);
					break;
				}
				case ComponentType::Collider:
				{
					dynamic_cast<ColliderComponent*>(gameObjects[i]->components[j])->Serialization(file);
					break;
				}
				case ComponentType::Rigidbody:
				{
					dynamic_cast<RigidbodyComponent*>(gameObjects[i]->components[j])->Serialization(file);
					break;
				}
				default:
					break;
				}
			}
		}
	}
	file.close();
}

void GameObjectManager::Deserialization(const std::string& filename)
{
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		int gameObjectsSize;
		file.read(reinterpret_cast<char*>(&gameObjectsSize), sizeof(int));

		gameObjects.resize(gameObjectsSize);

		for (int i = 0; i < gameObjectsSize; i++)
		{
			gameObjects[i] = new GameObject();
			gameObjects[i]->Deserialization(file);

			for (int j = 0; j < gameObjects[i]->components.size(); j++) //components.size() is read in gameObjects[i]->Deserialization().
			{
				int componentType;
				file.read(reinterpret_cast<char*>(&componentType), sizeof(int));
				ComponentType currentType = static_cast<ComponentType>(componentType);

				switch (currentType)
				{
				case ComponentType::Transform:
				{
					TransformComponent* newTransform = new TransformComponent();
					newTransform->owner = gameObjects[i];
					newTransform->Deserialization(file);
					gameObjects[i]->components[j] = newTransform;
					break;
				}
				case ComponentType::Mesh:
				{
					MeshComponent* newMesh = new MeshComponent();
					newMesh->owner = gameObjects[i];
					newMesh->Deserialization(file);
					gameObjects[i]->components[j] = newMesh;
					break;
				}
				case ComponentType::Light:
				{
					LightComponent* newLight = new LightComponent();
					newLight->owner = gameObjects[i];
					newLight->Deserialization(file);
					gameObjects[i]->components[j] = newLight;
					break;
				}
				case ComponentType::Collider:
				{
					ColliderComponent* newCollider = new ColliderComponent();
					newCollider->owner = gameObjects[i];
					newCollider->Deserialization(file);
					gameObjects[i]->components[j] = newCollider;
					break;
				}
				case ComponentType::Rigidbody:
				{
					RigidbodyComponent* newRigidbody = new RigidbodyComponent();
					newRigidbody->owner = gameObjects[i];
					newRigidbody->Deserialization(file);
					gameObjects[i]->components[j] = newRigidbody;
					break;
				}
				default:
					break;
				}
			}
		}
	}
	file.close();
}
