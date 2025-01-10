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

	//mesh = MeshManager::Get().LoadMesh("../Models/TreeTrunk");
	//mesh->bufferMesh();
	//mesh->ApplyTexture(myTexture);

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);


	//Get all texture file names.
	std::string path = "../Textures/";
	for (const std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(path))
	{
		textureVector.push_back(entry);
	}

	//Get all mesh file names.
	std::string path2 = "../Meshes/";
	for (const std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(path2))
	{
		if (!entry.path().has_extension())
		{
			meshVector.push_back(entry);
		}
		else
		{
			if (entry.path().extension() == ".obj")
			{
				meshVector.push_back(entry);
			}
		}
	}
}

MeshComponent::~MeshComponent()
{
	std::cout << "-->Deleting Mesh component." << std::endl;
	delete myTexture;
}

void MeshComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();

	//<-----------------MESH SELECTION UI--------------------->

	if (ImGui::Button("Select Mesh"))
	{
		ImGui::OpenPopup("Mesh Popup");
	}

	if (ImGui::BeginPopup("Mesh Popup"))
	{
		ImGui::SeparatorText("MESHES:");

		std::string currentDirectoryName;

		for (int i = 0; i < meshVector.size(); i++)
		{
			ImGui::PushID(i);

			if (!meshVector[i].path().has_extension())
			{
				currentDirectoryName = meshVector[i].path().string().c_str();
				ImGui::SeparatorText(currentDirectoryName.c_str()); //Display new SeparatorText for directories.
			}
			else
			{
				std::string loadableMeshName = meshVector[i].path().string().c_str();
				loadableMeshName.erase(loadableMeshName.length() - 4);

				std::string selectableMeshName = loadableMeshName;
				selectableMeshName.erase(selectableMeshName.begin(), selectableMeshName.begin() + currentDirectoryName.length() + 1);

				if (ImGui::Selectable(selectableMeshName.c_str())) //Display new Selectable for texture files.
				{
					lastSelectedMeshName = selectableMeshName;
					MeshMessage* newMessage = new MeshMessage();
					newMessage->meshToLoad = loadableMeshName.c_str();
					MeshManager::Get().QueueMessage(newMessage);
				}
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}

	if (MeshManager::Get().currentlyLoadingMesh != false)
	{
		ImGui::SameLine();
		ImGui::Text("Loading mesh into memory...");
	}

	if (ImGui::Button("Apply mesh")) //Not ideal right now, but it was the fastest way to get threaded mesh loading working. Maybe wanna send back a message to this component to change the mesh once the MeshManager is done loading the mesh.
	{
		mesh = MeshManager::Get().lastAccessedMesh;
		if (mesh->meshLoadedCorrectly == false)
		{
			meshInvalid = true;
			ImGui::OpenPopup("MeshInvalidPopupModal");
		}
		else
		{
			meshInvalid = false;
			mesh->bufferMesh();
			mesh->ApplyTexture(myTexture);
		}
	}

	if (ImGui::BeginPopupModal("MeshInvalidPopupModal"))
	{
		ImGui::Text("Error message: Failed to read mesh data. ");
		ImGui::Text("Name of mesh: %s", lastSelectedMeshName.c_str());
		ImGui::Text("The specified mesh may not be compatible with our .obj parser, \ntry re-exporting it from your 3D-program.");
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}


	//<-----------------TEXTURE SELECTION UI--------------------->


	if (ImGui::Button("Texture Popup test"))
	{
		ImGui::OpenPopup("Texture Popup");
	}

	if (ImGui::BeginPopup("Texture Popup"))
	{
		//ImTextureID texid = myTexture->TextureObject;
		//ImVec2 texsize = ImVec2(32, 32);

		ImGui::SeparatorText("TEXTURES:");

		std::string currentDirectoryName;

		for (int i = 0; i < textureVector.size(); i++)
		{
			ImGui::PushID(i);

			if (!textureVector[i].path().has_extension())
			{
				currentDirectoryName = textureVector[i].path().string().c_str();
				ImGui::SeparatorText(currentDirectoryName.c_str()); //Display new SeparatorText for directories.

			}
			else
			{
				//ImGui::Image(texid, texsize); //Display image. I need a texture manager. 
				//ImGui::SameLine(); 
				std::string textureName = textureVector[i].path().string().c_str();
				textureName.erase(textureName.length() - 4);
				textureName.erase(textureName.begin(), textureName.begin() + currentDirectoryName.length() + 1);
				if (ImGui::Selectable(textureName.c_str())) //Display new Selectable for texture files.
				{
					delete myTexture;
					myTexture = new Texture(textureVector[i].path().string().c_str()); //full path with file extension needed for stbi_load to work.

					if (mesh != nullptr)
					{
						mesh->ApplyTexture(myTexture);
					}
				}
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
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
	shader.SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
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

	if (mesh != nullptr)
	{
		DrawMesh(*shader);
	}
}
