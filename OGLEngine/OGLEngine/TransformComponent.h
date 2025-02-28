#pragma once
#include "glm.hpp"
#include "Component.h"
#include "Camera.h"
class TransformComponent : public Component
{
public:
	TransformComponent();
	~TransformComponent();
	glm::vec3 position;
	glm::vec3 rotation; //currently used as radians in the mesh and light components. (pi * degrees) / 180 = radian. 45 degrees is roughly 0.785 radians, 90 degrees is roughly 1.508 radians.
	glm::vec3 scale;

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;

};

