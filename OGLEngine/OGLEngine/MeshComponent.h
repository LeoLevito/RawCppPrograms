#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <string>
#include "ObjReader.h"
#include "vboindexer.h"

class ObjReader;

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();
	Mesh* mesh;
	Texture* myTexture;
	Shader* myShader;
	ObjReader* myObjReader;
	vboindexer* myVBOindexer;

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void DrawMesh(Shader& shader);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Update(Shader* shader) override;

	void printMemoryStatus();
private:
};

