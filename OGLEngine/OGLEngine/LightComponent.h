#pragma once
#include "Component.h"

class LightComponent : public Component
{

public:
	LightComponent();
	~LightComponent();

	void Update(Shader* shader) override;
	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void SetDirectionalLightToDefault();
	void SetDirectionalLightToCurrent();
	void SetDirectionalLightToZero();

	void SetPointLightToDefault();
	void SetPointLightToCurrent();
	void SetPointLightToZero();

	void SetSpotLightToDefault();
	void SetSpotLightToCurrent();
	void SetSpotLightToZero();

	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 scale;


		//directional light properties
	glm::vec3 directionalAmbient;
	glm::vec3 directionalDiffuse;
	glm::vec3 directionalSpecular;

		//point light properties
	glm::vec3 pointAmbient;
	glm::vec3 pointDiffuse;
	glm::vec3 pointSpecular;

	float pointLinear;
	float pointQuadratic;

		//spot light properties
	glm::vec3 spotAmbient;
	glm::vec3 spotDiffuse;
	glm::vec3 spotSpecular;

	float spotLinear;
	float spotQuadratic;

	float spotCutoff;
	float spotOuterCutoff;



	Shader* shaderRef;
	bool isShaderRefSet;

	int selectedType = 0;
private:
};

