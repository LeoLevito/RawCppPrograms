#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include <string>
#include "ObjReader.h"
#include "vboindexer.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Message.h"
#include "MeshMessage.h"
class MeshMessage;
class ObjReader;

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();
	Mesh* mesh;
	Texture* myTexture;
	Shader* myShader;

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void DrawMesh(Shader& shader);

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Update(Shader* shader) override;
	bool meshInvalid = false;
private:
};

