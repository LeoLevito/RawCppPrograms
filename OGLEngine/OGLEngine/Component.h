#pragma once
#include <string>
#include "Camera.h"
#include "glm.hpp"
#include "Shader.h"
//#include "GameObject.h"

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();
	std::string name;
	virtual void DrawComponentSpecificImGuiHierarchyAdjustables();
	virtual void Update();
	GameObject* owner;
private:
};

