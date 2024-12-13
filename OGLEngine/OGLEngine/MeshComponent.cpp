#include "MeshComponent.h"
#include "Cube.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <TransformComponent.h>
#include <GameObject.h>
#include <windows.h>
#include <thread>
#include <MeshManager.h>

MeshComponent::MeshComponent()
{
	name = "Mesh component";

	myTexture = new Texture("../Textures/Bliss.jpg");
	myShader = new Shader;
	myShader->Initialize("../Shaders/VertexShader.vertexs", "../Shaders/FragmentShader.fragments");

	mesh = MeshManager::Get().LoadMesh("../Models/TreeTrunk");
	
	mesh->ApplyTexture(myTexture);

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

MeshComponent::~MeshComponent()
{
	std::cout << "-->Deleting Mesh component." << std::endl;
	delete myShader;
	delete myTexture;
	//delete mesh; //now we cache meshes in the MeshManager, so deleting here screws things up in the MeshManager.
}

void MeshComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
	//Change texture of mesh using ImGui. Would ideally improve this by being able to choose available textures from a drop down menu for example.
	static char str0[128] = "Bliss2.jpg"; //how it's done in the ImGui demo, tho it is replicated across all objects now...
	ImGui::InputText("Texture name", str0, IM_ARRAYSIZE(str0)); //Yeah, I gotta change this to a dropdown or something.
	if (ImGui::Button("Change Texture"))
	{
		std::string path = "../Textures/";
		path.append(str0);

		std::cout << path << std::endl;

		delete myTexture;
		myTexture = new Texture(path.c_str());

		mesh->ApplyTexture(myTexture);
	}

	//Change mesh of mesh using ImGui. Would ideally improve this by being able to choose available meshes from a drop down menu for example.
	//EDIT: WOAH, just saw that it's possible to open file explorer with ImGui to choose .obj files from a specified directory, that's a possibility!.
	static char str1[128] = "TreeTrunk"; //how it's done in the ImGui demo, tho it is replicated across all objects now...
	ImGui::InputText("Mesh name", str1, IM_ARRAYSIZE(str1)); //Yeah, I gotta change this to a dropdown or something.
	if (ImGui::Button("Change Mesh"))
	{
		std::string path = "../Models/";
		path.append(str1);

		std::cout << path << std::endl;

		//should probably delete mesh before assigning a new one so as to clear memory.
		//delete mesh; //woah is it because I delete the mesh?
		mesh = MeshManager::Get().LoadMesh(path);
		
		//should do like an ImGui warning popup saying that the mesh couldn't load in case the parse fails. 
		//maybe ImGui::IsPopupOpen()?)
		mesh->ApplyTexture(myTexture);
	}
}

void MeshComponent::DrawMesh(Shader& shader)
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, scale);

	//write to Vertex Shader
	shader.SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
	shader.SetMatrix4(Camera::Get().myView, "view");
	shader.SetMatrix4(Camera::Get().projection, "projection");
	mesh->Draw(&shader);
}

void MeshComponent::Update(Shader* shader)
{
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;
		}
	}
	
	DrawMesh(*shader); //Shader is scuffed right now.
}