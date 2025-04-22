#pragma once
#include "Component.h"
#include "Light.h"
#include "Texture.h"


class Light;

class LightComponent : public Component
{

public:
	LightComponent();
	~LightComponent();

	void Update() override;
	void DrawComponentSpecificImGuiHierarchyAdjustables() override;
	void DrawIcon();
	void SetIconTexture();
	void SetupIconMesh();

	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 scale;

	int selectedType = 0;

	Light* myLight = nullptr;

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;

	Texture* iconTexture;

	unsigned int VBO, VAO, EBO, UVBO;

	bool allowDrawIcon = false;
private:
};

