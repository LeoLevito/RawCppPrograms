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
	Mesh* mesh = nullptr;

	Texture* diffuseMap;
	Texture* specularMap;
	std::string diffuseMapPath; //important for serialization
	std::string specularMapPath; //important for serialization

	void ReceiveRequestedMesh();

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void DrawMesh();

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Update() override;
	bool meshInvalid = false;
	std::string lastSelectedMeshName;
	std::string lastLoadableMeshName; //important for serialization
	std::vector<std::filesystem::directory_entry> textureVector;
	std::vector<std::filesystem::directory_entry> meshVector;

	std::string lastDirectoryName;

	//material properties.
	glm::vec3 ambient{ 0, 0, 0 };
	glm::vec3 diffuse{ 1, 1, 1 };
	glm::vec3 specular{ 1, 1, 1 };

	float shininess;//important for serialization

	enum TextureChoice
	{
		choice_diffuse,
		choice_specular, 
		none
	};

	TextureChoice textureChoice = TextureChoice::none;

	int selectedMinType; //important for serialization
	int selectedMagType; //important for serialization

	void ReloadTextures();

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;

	bool DoOnce = true;

private:
};

