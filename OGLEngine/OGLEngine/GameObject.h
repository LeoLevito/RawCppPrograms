#pragma once
#include "Component.h"
#include <vector>
#include <string>
class GameObject
{
public:
	GameObject();
	virtual ~GameObject(); //need to have this virtual since we have classes that derive from this one so that their destructors will be called instead of this one.
	//virtual void BeginPlay();
	//virtual void Tick(float deltaTime);
	void AddComponent(Component* component);
	std::vector<Component*> componentVector;
	std::string name;
	virtual void Update();
private:

};

