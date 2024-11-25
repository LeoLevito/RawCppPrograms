#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
class MeshComponent : public Component
{
public:
	MeshComponent();
	Mesh* mesh;
	Texture* myTexture;
	Shader* myShader;
	char* textureFileName;

	void DrawComponentSpecificImGuiHierarchyAdjustables(Camera& camera, glm::mat4& projection, Shader& shader) override;

	void DrawMesh(Camera& camera, glm::mat4& projection, Shader& shader);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
private:
};

