#pragma once
#include "GameObject.h"
#include <vector>
#include "Message.h"
#include <string>
class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
public:
	static GameObjectManager& Get();
	GameObject* CreateGameObject();
	void DeleteGameObject(GameObject* gameObject);
	void DeleteAllGameObjects();

	std::vector<GameObject*> gameObjects;

	void QueueMessage(Message* message);
	void ProcessMessages();
	void ProcessMessage(Message* message); //you have to use pointer here since a subclass of Message can be used.
	std::vector <Message*> queuedMessages;
	void Process();
	bool shouldRun = true;


	void Serialization(const std::string& filename);
	void Deserialization(const std::string& filename);



	//I should have a copy/paste function for game objects.




private:
};

