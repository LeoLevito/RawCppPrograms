#pragma once
#include "glm.hpp"
#include "Component.h"
class TransformComponent : public Component
{
public:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

