#pragma once
#include <string>
#include "Camera.h"
#include "glm.hpp"
#include "Shader.h"

class Component
{
public:
	std::string name;
	virtual void DrawComponentSpecificImGuiHierarchyAdjustables(Camera& camera, glm::mat4& projection, Shader& shader);
private:
};

