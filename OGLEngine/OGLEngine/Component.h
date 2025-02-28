#pragma once
#include <string>
#include "Camera.h"
#include "glm.hpp"
#include "Shader.h"
#include <fstream>
#include <sstream>
//#include "GameObject.h"


class GameObject;

enum class ComponentType {
	Transform,
	Mesh,
	Light,
	Collider,
	Rigidbody
};

class Component
{
public:
	Component();
	virtual ~Component();
	std::string name;
	virtual void DrawComponentSpecificImGuiHierarchyAdjustables();
	virtual void Update();
	GameObject* owner;
	bool isMarkedForDeletion;

	ComponentType type;


	virtual void Serialization(std::fstream& file);
	virtual void Deserialization(std::fstream& file);


private:
};

