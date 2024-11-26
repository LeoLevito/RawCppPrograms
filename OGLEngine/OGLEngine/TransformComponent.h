#pragma once
#include "glm.hpp"
#include "Component.h"
#include "Camera.h"
class TransformComponent : public Component
{
public:
	TransformComponent();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void DrawComponentSpecificImGuiHierarchyAdjustables(Camera& camera, glm::mat4& projection) override;
};

