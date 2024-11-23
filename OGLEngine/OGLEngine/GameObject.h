#pragma once
#include "Component.h"
#include <vector>
#include <string>
class GameObject
{
public:
	GameObject();
	//virtual void BeginPlay();
	//virtual void Tick(float deltaTime);
	void AddComponent(Component* component);
	std::vector<Component*> componentVector;
	std::string name;
	virtual void Update();
private:

};

