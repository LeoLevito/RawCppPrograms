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
	//glm::vec3 scale;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutoff;
	float outerCutoff;

	Shader* shaderRef;
	bool isShaderRefSet;

private:
};

