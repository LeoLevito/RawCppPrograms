#include "MeshComponent.h"
#include "Cube.h"
#include "EditorGUI.h"
#include "Texture.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
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
#include "TextureManager.h"


MeshComponent::MeshComponent()
{
	name = "Mesh component";
	type = ComponentType::Mesh;

	selectedMinType = 0;
	selectedMagType = 0;

	diffuseMapPath = "../Textures/Bliss\\Bliss.jpg";
	specularMapPath = "../Textures/Bliss\\Bliss2.jpg";
	diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType, false);
	specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType, false);

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
	//ambient = glm::vec3(0, 0, 0);
	//diffuse = glm::vec3(1, 1, 1);
	//specular = glm::vec3(1, 1, 1);
	shininess = 128.0f;
	//ShaderManager::Get().shader->SetVector3(ambient, "material.ambient"); //does this even do anything?
	//ShaderManager::Get().shader->SetVector3(diffuse, "material.diffuse");
	//ShaderManager::Get().shader->SetVector3(specular, "material.specular");
	ShaderManager::Get().shader->Use();
	ShaderManager::Get().shader->SetFloat(shininess, "material.shininess"); //Emil said I should make the shininess property dependent on the specular map, for example it's RED values, like other people have done.
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

	//if (ImGui::Button("Choose shadow map")) //do I even need this anymore?
	//{
	//	if (ShaderManager::Get().depthPass == false)
	//	{
	//		ShaderManager::Get().shader->SetInt(2, "material.shadowMap"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
	//		mesh->ApplyShadowMap(*(ShaderManager::Get().shadowMap));
	//	}
	//}

	if (ImGui::Button("Choose texture Min filter"))
	{
		ImGui::OpenPopup("Min filter Popup");
	}

	if (ImGui::Button("Choose texture Mag filter"))
	{
		ImGui::OpenPopup("Mag filter Popup");
	}

	//if (ImGui::Button("Reload textures"))
	//{
	//	ReloadTextures();
	//}

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

				ReloadTextures();
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

				ReloadTextures();
			}
			//ImGui::EndDisabled();
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Texture Popup"))
	{
		/*ImTextureID texid = diffuseMap->TextureObject;
		ImVec2 texsize = ImVec2(32, 32);*/

		ImGui::SeparatorText("TEXTURES:");

		std::string currentDirectoryName;
		int previewIterator = 0;
		for (int i = 0; i < TextureManager::Get().texturePaths.size(); i++)
		{
			ImGui::PushID(i);

			if (!TextureManager::Get().texturePaths[i].path().has_extension())
			{
				currentDirectoryName = TextureManager::Get().texturePaths[i].path().string().c_str();
				if (TextureManager::Get().texturePaths[i].path().string().find("\\_previews") == std::string::npos) //if path name doesn't have "/_previews" in it.
				{
					//currentDirectoryName = TextureManager::Get().texturePaths[i].path().string().c_str();
					//lastDirectoryName = currentDirectoryName;
					ImGui::SeparatorText(currentDirectoryName.c_str()); //Display new SeparatorText for directories.
				}

			}
			else
			{
				if (TextureManager::Get().texturePaths[i].path().string().find("_preview.jpg") != std::string::npos)
				{


					ImTextureID texid = TextureManager::Get().texturePreviews[previewIterator]->TextureObject;
					ImVec2 texsize = ImVec2(32, 32);
					previewIterator++;

					ImGui::Image(texid, texsize, ImVec2(0, 1), ImVec2(1, 0)); //Display image. I need a texture manager. ImVec2 is for manipulating uv, we invert vertical so opengl displays the image correctly.
					
					ImGui::SameLine();
					
					std::string textureName = TextureManager::Get().texturePaths[i].path().string().c_str();
					textureName.erase(textureName.length() - 12);
					std::string textureNameNoPreview = textureName + ".jpg";
					int pos = textureNameNoPreview.find("_previews");
					textureNameNoPreview.erase(pos, 10);
					textureName.erase(textureName.begin(), textureName.begin() + currentDirectoryName.length() + 1);

					if (ImGui::Selectable(textureName.c_str())) //Display new Selectable for texture files.
					{
						if (mesh != nullptr)
						{
							switch (textureChoice)
							{

							case MeshComponent::choice_diffuse:
								diffuseMapPath = textureNameNoPreview;
								break;
							case MeshComponent::choice_specular:
								specularMapPath = textureNameNoPreview;
								break;
							default:
								break;
							}
							ReloadTextures();
						}
					}
				}
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}

	if (ImGui::DragFloat("shininess", &shininess, .01f))
	{
		if (ShaderManager::Get().depthPass == false)
		{
			ShaderManager::Get().shader->Use();
			ShaderManager::Get().shader->SetFloat(shininess, "material.shininess");
		}
	}
}

void MeshComponent::DrawMesh()
{

	glm::mat4 trans = glm::mat4(1.0f);
	glm::quat myRotationQuaternion = glm::quat(glm::radians(rotation)); //https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	glm::mat4 rotationMatrix = glm::toMat4(myRotationQuaternion);
	trans = glm::translate(trans, position); //translate first so that each object rotates independently.
	trans = trans * rotationMatrix;
	trans = glm::scale(trans, scale);


	//write to Vertex Shader
	if (EditorGUI::Get().currentPolygonMode == GL_LINE || EditorGUI::Get().currentPolygonMode == GL_POINT)
	{
		//use shader for line rendering
		ShaderManager::Get().lineShader->Use();
		ShaderManager::Get().lineShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().lineShader->SetMatrix4(Camera::Get().projection, "projection");
		glm::vec3 color = { 1,1,1 };
		ShaderManager::Get().lineShader->SetVector3(color, "vertexColor");
	}
	else if (ShaderManager::Get().pickingPass == true)
	{
		ShaderManager::Get().pickingShader->Use();
		ShaderManager::Get().pickingShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().pickingShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().pickingShader->SetMatrix4(Camera::Get().projection, "projection");

		//here I want to write to the fragment shader with my unique color.
		int r = (owner->ID & 0x000000FF) >> 0;
		int g = (owner->ID & 0x0000FF00) >> 8;
		int b = (owner->ID & 0x00FF0000) >> 16;

		glm::vec4 color = { r / 255.0f, g / 255.0f, b / 255.0f, 1.0f }; //change to r/5.0f for a more noticeable color with a small amount of meshes in the scene.

		ShaderManager::Get().pickingShader->SetVector4(color, "pickingColor");

	}
	else if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().shader->SetMatrix4(Camera::Get().projection, "projection");
		ShaderManager::Get().shader->SetVector3(Camera::Get().myPosition, "viewPos"); //Doesn't really make sense to update this here but whatever.
	}
	else
	{
		ShaderManager::Get().depthCubeMapShader->Use(); //this might be the issue with my lighting, I now need to use this shader here and for my lights for it to work correctly, otherwise they will be overridden by other shaders in use.
		ShaderManager::Get().depthCubeMapShader->SetMatrix4(trans, "transform"); //this is required for meshes to be rendered to depthMap.
	}

	if (ShaderManager::Get().outlinePass == true)
	{
		ShaderManager::Get().outlineShader->Use();
		ShaderManager::Get().outlineShader->SetMatrix4(trans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().outlineShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().outlineShader->SetMatrix4(Camera::Get().projection, "projection");
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		ShaderManager::Get().outlineShader->SetVector4(color, "outlineColor");
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
	diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType, false); //full path with file extension needed for stbi_load to work.
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();
		ShaderManager::Get().shader->SetInt(0, "material.diffuse"); //why do I need to do this again? time to re-read https://learnopengl.com/Lighting/Lighting-maps
		mesh->ApplyDiffuseMap(diffuseMap);
	}
	delete specularMap;
	specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType, false); //full path with file extension needed for stbi_load to work.
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();
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

		diffuseMap = new Texture(diffuseMapPath.c_str(), selectedMinType, selectedMagType, false);
		specularMap = new Texture(specularMapPath.c_str(), selectedMinType, selectedMagType, false);
	}

	//do (MATERIAL):
	if (ShaderManager::Get().depthPass == false)
	{
		ShaderManager::Get().shader->Use();

		ShaderManager::Get().shader->SetInt(0, "material.diffuse");
		mesh->ApplyDiffuseMap(diffuseMap); //BUG!!! (nothing's wrong with the deserialization, but actually the texture application in general, the latest texture is applied to all game objects which shouldn't be the case). EDIT: actually, the texture is applied to all objects with the same mesh, meaning that mesh* from MeshManager is not doing me a favour in this case.

		ShaderManager::Get().shader->SetInt(1, "material.specular");
		mesh->ApplySpecularMap(specularMap);

		ShaderManager::Get().shader->SetFloat(shininess, "material.shininess");
	}
}
