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
#include "ShaderManager.h"

MeshComponent::MeshComponent()
{
	name = "Mesh component";
	type = ComponentType::Mesh;

	diffuseMapPath = "../Textures/Bliss/Bliss.jpg";
	specularMapPath = "../Textures/Bliss/Bliss2.jpg";
	diffuseMap = new Texture(diffuseMapPath.c_str(), 5, 1);
	specularMap = new Texture(specularMapPath.c_str(), 5, 1);
	//mesh = MeshManager::Get().LoadMesh("../Models/TreeTrunk");
	//mesh->bufferMesh();
	//mesh->ApplyTexture(myTexture);

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);


	//Get all texture file names.
	std::string path = "../Textures/";
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
	{
		textureVector.push_back(entry);
	}

	//Get all mesh file names.
	std::string path2 = "../Meshes/";
	for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path2))
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

	//setting default values for material light behavior. Also, why does the Light Component only work properly when I add a new Mesh Component to the same GameObject? Need to investigate futher.
	ambient = glm::vec3(0, 0, 0);
	diffuse = glm::vec3(1, 1, 1);
	specular = glm::vec3(1, 1, 1);
	shininess = 128.0f;
	ShaderManager::Get().shader->SetVector3(ambient, "material.ambient");
	ShaderManager::Get().shader->SetVector3(diffuse, "material.diffuse");
	ShaderManager::Get().shader->SetVector3(specular, "material.specular");
	ShaderManager::Get().shader->SetFloat(shininess, "material.shininess"); //Emil said I should make the shininess property dependent on the specular map, for example it's RED values, like other people have done.

	selectedMinType = 0;
	selectedMagType = 0;
}

MeshComponent::~MeshComponent()
{
	std::cout << "-->Deleting Mesh component." << std::endl;
	delete diffuseMap;
	delete specularMap;
}

void MeshComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
	if (isMarkedForDeletion)
	{
		return;
	}

	//<-----------------MESH SELECTION UI--------------------->

	ImGui::BeginDisabled(MeshManager::Get().currentlyLoadingMesh != false);
	if (ImGui::Button("Select Mesh"))
	{
		ImGui::OpenPopup("Mesh Popup");
	}
	ImGui::EndDisabled();

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

				if (ImGui::Selectable(selectableMeshName.c_str())) //Display new Selectable for mesh files.
				{
					lastSelectedMeshName = selectableMeshName;
					lastLoadableMeshName = loadableMeshName;
					MeshMessage* newMessage = new MeshMessage();
					newMessage->meshToLoad = lastLoadableMeshName.c_str();
					newMessage->directoryEntry = meshVector[i];
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

	ImGui::BeginDisabled(MeshManager::Get().currentlyLoadingMesh != false);
	if (ImGui::Button("Apply mesh")) //Not ideal right now, but it was the fastest way to get threaded mesh loading working. Maybe wanna send back a message to this component to change the mesh once the MeshManager is done loading the mesh.
	{
		mesh = MeshManager::Get().lastAccessedMesh;
		if (mesh != nullptr)
		{
			if (mesh->meshLoadedCorrectly == false)
			{
				meshInvalid = true;
				ImGui::OpenPopup("MeshInvalidPopupModal");
			}
			else
			{
				meshInvalid = false;
				mesh->bufferMesh();
				mesh->ApplyDiffuseMap(diffuseMap);
				mesh->ApplySpecularMap(specularMap);
			}
		}
		else
		{
			std::cout << "MESH IS NULLPTR" << std::endl;
		}
	}
	ImGui::EndDisabled();

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


	if (ImGui::Button("Choose diffuse map"))
	{
		textureChoice = TextureChoice::choice_diffuse;
		ImGui::OpenPopup("Texture Popup");
	}

	if (ImGui::Button("Choose specular map"))
	{
		textureChoice = TextureChoice::choice_specular;
		ImGui::OpenPopup("Texture Popup");
	}

	if (ImGui::Button("Choose shadow map")) //do I even need this anymore?
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetInt(2, "material.shadowMap"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
			mesh->ApplyShadowMap(*(ShaderManager::Get().shadowMap));
		}
	}

	if (ImGui::Button("Choose texture Min filter"))
	{
		ImGui::OpenPopup("Min filter Popup");
	}

	if (ImGui::Button("Choose texture Mag filter"))
	{
		ImGui::OpenPopup("Mag filter Popup");
	}

	if (ImGui::Button("Reload textures"))
	{
		ReloadTextures();
	}

	const char* minNames[] = { "Nearest", "Linear", "Nearest Mipmap Nearest", "Linear Mipmap Nearest", "Nearest Mipmap Linear", "Linear Mipmap Linear" };

	ImGui::Text("Selected Min Filter:");
	ImGui::SameLine();
	ImGui::Text(minNames[selectedMinType]);

	if (ImGui::BeginPopup("Min filter Popup"))
	{
		ImGui::SeparatorText("MIN FILTERS:");

		for (int i = 0; i < IM_ARRAYSIZE(minNames); i++)
		{
			//ImGui::BeginDisabled();
			if (ImGui::Selectable(minNames[i]))
			{
				selectedMinType = i;
			}
			//ImGui::EndDisabled();
		}
		ImGui::EndPopup();
	}

	const char* magNames[] = { "Nearest", "Linear" };

	ImGui::Text("Selected Mag Filter:");
	ImGui::SameLine();
	ImGui::Text(magNames[selectedMagType]);

	if (ImGui::BeginPopup("Mag filter Popup"))
	{
		ImGui::SeparatorText("MAG FILTERS:");

		for (int i = 0; i < IM_ARRAYSIZE(magNames); i++)
		{
			//ImGui::BeginDisabled();
			if (ImGui::Selectable(magNames[i]))
			{
				selectedMagType = i;
			}
			//ImGui::EndDisabled();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Texture Popup"))
	{
		ImTextureID texid = diffuseMap->TextureObject;
		ImVec2 texsize = ImVec2(32, 32);

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
				ImGui::Image(texid, texsize); //Display image. I need a texture manager. 
				ImGui::SameLine();
				std::string textureName = textureVector[i].path().string().c_str();
				textureName.erase(textureName.length() - 4);
				textureName.erase(textureName.begin(), textureName.begin() + currentDirectoryName.length() + 1);
				if (ImGui::Selectable(textureName.c_str())) //Display new Selectable for texture files.
				{
					if (mesh != nullptr)
					{
						switch (textureChoice)
						{
						case MeshComponent::choice_diffuse:
							delete diffuseMap;
							diffuseMapPath = textureVector[i].path().string().c_str();
							diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType); //full path with file extension needed for stbi_load to work.
							if (ShaderManager::Get().depthPass == false)
							{
								ShaderManager::Get().shader->SetInt(0, "material.diffuse"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
								mesh->ApplyDiffuseMap(diffuseMap);
							}

							break;
						case MeshComponent::choice_specular:
							delete specularMap;
							specularMapPath = textureVector[i].path().string().c_str();
							specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType); //full path with file extension needed for stbi_load to work.
							if (ShaderManager::Get().depthPass == false)
							{
								ShaderManager::Get().shader->SetInt(1, "material.specular"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
								mesh->ApplySpecularMap(specularMap);
							}
							break;
						default:
							break;
						}

					}
				}
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}


	//if (ImGui::DragFloat3("ambient", &ambient.x, .01f)) //these don't do anything, except for the shininess float, because in the fragment shader the material struct only has diffuse and specular SAMPLER2Ds and not vec3s. And I removed ambient, maybe if I reintroduce it I can get emissive textures?
	//{
	//	ShaderManager::Get().shader->SetVector3(ambient, "material.ambient");
	//}
	//if (ImGui::DragFloat3("diffuse", &diffuse.x, .01f))
	//{
	//	ShaderManager::Get().shader->SetVector3(diffuse, "material.diffuse");
	//}
	//if (ImGui::DragFloat3("specular", &specular.x, .01f))
	//{
	//	ShaderManager::Get().shader->SetVector3(specular, "material.specular");
	//}
	if (ImGui::DragFloat("shininess", &shininess, .01f))
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->SetFloat(shininess, "material.shininess");
		}
	}
}

void MeshComponent::DrawMesh()
{
	glm::mat4 trans = glm::mat4(1.0f);

	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = glm::rotate(trans, rotation.x, glm::vec3(1, 0, 0));
	trans = glm::rotate(trans, rotation.y, glm::vec3(0, 1, 0));
	trans = glm::rotate(trans, rotation.z, glm::vec3(0, 0, 1));
	trans = glm::scale(trans, scale);

	//write to Vertex Shader
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().projection, "projection");
		ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
	}
	else
	{
		//ShaderManager::Get().depthShader->SetMatrix4(trans, "transform"); //this is required for meshes to be rendered to depthMap.
		ShaderManager::Get().depthCubeMapShader->SetMatrix4(trans, "transform"); //this is required for meshes to be rendered to depthMap.
	}

	mesh->Draw();
}

void MeshComponent::Update()
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
		DrawMesh();
	}
}

void MeshComponent::ReloadTextures()
{
	delete diffuseMap;
	diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType); //full path with file extension needed for stbi_load to work.
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetInt(0, "material.diffuse"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
		mesh->ApplyDiffuseMap(diffuseMap);
	}
	delete specularMap;
	specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType); //full path with file extension needed for stbi_load to work.
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetInt(1, "material.specular"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
		mesh->ApplySpecularMap(specularMap);
	}
}

void MeshComponent::Serialization(std::fstream& file)
{
	int lastLoadableMeshNameSize = lastLoadableMeshName.size();
	int diffuseMapPathSize = diffuseMapPath.size();
	int specularMapPathSize = specularMapPath.size();

	file.write(reinterpret_cast<char*>(&lastLoadableMeshNameSize), sizeof(int));
	file.write(reinterpret_cast<char*>(&diffuseMapPathSize), sizeof(int));
	file.write(reinterpret_cast<char*>(&specularMapPathSize), sizeof(int));

	file.write(reinterpret_cast<char*>(&lastLoadableMeshName[0]), lastLoadableMeshNameSize);
	file.write(reinterpret_cast<char*>(&diffuseMapPath[0]), diffuseMapPathSize);
	file.write(reinterpret_cast<char*>(&specularMapPath[0]), specularMapPathSize);

	file.write(reinterpret_cast<char*>(&selectedMinType), sizeof(int));
	file.write(reinterpret_cast<char*>(&selectedMagType), sizeof(int));
	file.write(reinterpret_cast<char*>(&shininess), sizeof(float));
}

void MeshComponent::Deserialization(std::fstream& file)
{
	int lastLoadableMeshNameSize;
	int diffuseMapPathSize;
	int specularMapPathSize;

	file.read(reinterpret_cast<char*>(&lastLoadableMeshNameSize), sizeof(int));
	file.read(reinterpret_cast<char*>(&diffuseMapPathSize), sizeof(int));
	file.read(reinterpret_cast<char*>(&specularMapPathSize), sizeof(int));

	lastLoadableMeshName.resize(lastLoadableMeshNameSize);
	diffuseMapPath.resize(diffuseMapPathSize);
	specularMapPath.resize(specularMapPathSize);
	file.read(reinterpret_cast<char*>(&lastLoadableMeshName[0]), lastLoadableMeshNameSize);
	file.read(reinterpret_cast<char*>(&diffuseMapPath[0]), diffuseMapPathSize);
	file.read(reinterpret_cast<char*>(&specularMapPath[0]), specularMapPathSize);

	file.read(reinterpret_cast<char*>(&selectedMinType), sizeof(int));
	file.read(reinterpret_cast<char*>(&selectedMagType), sizeof(int));
	file.read(reinterpret_cast<char*>(&shininess), sizeof(float));


	if (lastLoadableMeshName.size() > 0)
	{
		//do (MESH):
		MeshManager::Get().currentlyLoadingMesh = true;
		MeshManager::Get().IsAvailableMemoryOK = true; //okay, this may become an edge case problem if the computer's RAM is already filled up when loading the scene.
		MeshManager::Get().IsObjSizeOK = true; //we know this to be true because we've loaded it before.
		
		mesh = MeshManager::Get().LoadMesh(lastLoadableMeshName);
		MeshManager::Get().currentlyLoadingMesh = false;
		mesh->bufferMesh();

		//do (TEXTURE):
		diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType);
		specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType);
		mesh->ApplyDiffuseMap(diffuseMap);
		mesh->ApplySpecularMap(specularMap);
	}


	//do (MATERIAL):
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->SetFloat(shininess, "material.shininess");
	}
}
