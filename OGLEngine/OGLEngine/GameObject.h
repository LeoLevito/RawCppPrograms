#pragma once
#include "Component.h"
#include <vector>
#include <string>

class GameObject //equivalent of entity class
{
public:
	GameObject();
	virtual ~GameObject(); //need to have this virtual since we have classes that derive from this one so that their destructors will be called instead of this one.
	//virtual void BeginPlay();
	//virtual void Tick(float deltaTime);
	void AddComponent(Component* component); //should look into having a choice between components in the ImGui window. And a button to add components like in Unity.

	

//make a RemoveComponent function here.



	std::vector<Component*> components;
	std::string name;
	virtual void Update(Shader* shader);
	virtual void DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec);
private:

};

