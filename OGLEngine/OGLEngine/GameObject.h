#pragma once
#include "Component.h"
#include <vector>
#include <string>

class GameObject //equivalent of entity class
{
public:
	GameObject();
	virtual ~GameObject(); //need to have this virtual since we have classes that derive from this one so that their destructors will be called instead of this one.

	void AddComponent(Component* component); //should look into having a choice between components in the ImGui window. And a button to add components like in Unity.

	std::vector<Component*> components; //I need to make it so you can only have one of each component on a game object. Maybe grey out the options in the dropdown menu.
	std::string name;
	int ID{};
	virtual void Update();
	virtual void DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec);

	virtual void Serialization(std::fstream& file);
	virtual void Deserialization(std::fstream& file);

private:

};

