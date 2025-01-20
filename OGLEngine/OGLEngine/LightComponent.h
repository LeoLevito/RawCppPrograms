#pragma once
#include "Component.h"
#include "Light.h"

class LightComponent : public Component
{

public:
	LightComponent();
	~LightComponent();

	void Update() override;
	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 scale;

	int selectedType = 0;

	Light* myLight;

private:
};

