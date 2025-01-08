#pragma once
#include "Component.h"

class LightComponent : public Component
{

public:
	LightComponent();
	~LightComponent();

	void Update(Shader* shader) override;
	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

private:
};

