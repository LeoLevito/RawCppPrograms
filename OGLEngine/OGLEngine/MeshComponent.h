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
#include <filesystem>

class MeshMessage;
class ObjReader;

class MeshComponent : public Component
{
public:
	MeshComponent();
	~MeshComponent();
	Mesh* mesh;

	Texture* diffuseMap;
	Texture* specularMap;
	std::string diffuseMapPath;
	std::string specularMapPath;

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void DrawMesh();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Update() override;
	bool meshInvalid = false;
	std::string lastSelectedMeshName;
	std::vector<std::filesystem::directory_entry> textureVector;
	std::vector<std::filesystem::directory_entry> meshVector;

	//material properties.
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	enum TextureChoice
	{
		choice_diffuse,
		choice_specular
	};

	TextureChoice textureChoice;

	int selectedMinType;
	int selectedMagType;

	void ReloadTextures();



private:
};

