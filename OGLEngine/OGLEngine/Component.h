#pragma once
#include <string>
#include "Camera.h"
#include "glm.hpp"
#include "Shader.h"

class Component
{
public:
	std::string name;
	virtual void DrawComponentSpecificImGuiHierarchyAdjustables(glm::mat4& projection);
	virtual void Update(Shader* shader);

	Camera* myCamera;
	glm::mat4* myProjection;
private:
};

