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

	void QueueMessage(Message* message);
	void ProcessMessages();
	void ProcessMessage(Message* message); //you have to use pointer here since a subclass of Message can be used.
	std::vector <Message*> queuedMessages;
	void Process();
	bool shouldRun = true;





	//I should have a copy/paste function for game objects.




private:
};

