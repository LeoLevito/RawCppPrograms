#include "LightComponent.h"
#include <GLAD/glad.h>
#include <GameObject.h>
#include <TransformComponent.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "LightManager.h"
#include "ShaderManager.h"
#include "glm.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include "TextureManager.h"

LightComponent::LightComponent()
{
	name = "Light component";
	type = ComponentType::Light;

	myLight = LightManager::Get().AddNewLight(LightType::DirectionalLightType);
	myLight->SetToDefault();


	SetIconTexture();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);	
	glGenBuffers(1, &UVBO);
}

LightComponent::~LightComponent()
{
	std::cout << "-->Deleting Light component." << std::endl;
	LightManager::Get().DeleteLight(myLight->type, *myLight);
}

void LightComponent::Update()
{
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			//scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;

			myLight->SetPosition(position);
			myLight->SetDirection(rotation);
		}
	}
	myLight->SetLightSpaceMatrix(); //fix issue where shadowmap light view position wouldn't be set to 0,0,0 upon light deletion (well, it still doesn't do that), and when re-adding a light without a transform component the shadows would have a different angle compared to where the light is if you changed the angles/position before deleting the first time. You can still, for a split second, see the previous shadowmap when adding a new light after deleting a light that had a position other than 0,0,0.
	
	if (allowDrawIcon)
	{
		DrawIcon();
	}
}

void LightComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
	if (isMarkedForDeletion)
	{
		return;
	}

	const char* typeNames[] = { "Directional light", "Point light", "Spot light" };

	ImGui::Text("Current type:");
	ImGui::SameLine();
	ImGui::Text(typeNames[selectedType]); //getting error here after deletion because selectedType has been cleared and is therefore garbled. Same thing now happens with the MeshComponent as well.
	ImGui::Text("ID:");
	std::string currentID = std::to_string(myLight->ID);
	ImGui::SameLine();
	ImGui::Text(currentID.c_str());


	if (ImGui::Button("Select type"))
	{
		ImGui::OpenPopup("Light Type Popup");
	}

	if (ImGui::BeginPopup("Light Type Popup"))
	{
		ImGui::SeparatorText("TYPES:");
		for (int i = 0; i < IM_ARRAYSIZE(typeNames); i++)
		{
			if (ImGui::Selectable(typeNames[i]))
			{
				selectedType = i;
				SetIconTexture();
				switch (selectedType)
				{
				case 0:
					LightManager::Get().DeleteLight(myLight->type, *myLight);
					myLight = LightManager::Get().AddNewLight(LightType::DirectionalLightType);
					break;
				case 1:
					LightManager::Get().DeleteLight(myLight->type, *myLight);
					myLight = LightManager::Get().AddNewLight(LightType::PointLightType);
					break;
				case 2:
					LightManager::Get().DeleteLight(myLight->type, *myLight);
					myLight = LightManager::Get().AddNewLight(LightType::SpotLightType);
					break;
				default:
					break;
				}
			}
		}
		ImGui::EndPopup();
	}
	myLight->DrawImgui();


	ImGui::Checkbox("Draw Icon", &allowDrawIcon);
}

void LightComponent::DrawIcon()
{
	//setup iconfacing camera transform
	glm::vec3 camToIconDirection = position - Camera::Get().myPosition;

	glm::quat myRotationQuaternion = glm::quatLookAt(Camera::Get().myDirection, Camera::Get().myUp);
	glm::mat4 rotationMatrix = glm::toMat4(myRotationQuaternion);

	glm::mat4 FacingCameraTrans = glm::mat4(1.0f);
	FacingCameraTrans = glm::translate(FacingCameraTrans, position); //translate first so that each object rotates independently.
	FacingCameraTrans = FacingCameraTrans * rotationMatrix;



	if (ShaderManager::Get().pickingPass == true)
	{
		ShaderManager::Get().pickingShader->Use();
		ShaderManager::Get().pickingShader->SetMatrix4(FacingCameraTrans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
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
		ShaderManager::Get().iconShader->Use();
		ShaderManager::Get().iconShader->SetMatrix4(FacingCameraTrans, "transform"); //apperently there's a better way to do this compared to using a Uniform type variable inside the vertex shader, Shader Buffer Storage Object, something like that, where we can have even more variables inside the shader and update them.
		ShaderManager::Get().iconShader->SetMatrix4(Camera::Get().myView, "view");
		ShaderManager::Get().iconShader->SetMatrix4(Camera::Get().projection, "projection");
	}

	


	//copied from RenderWorldGrid(), we need to have a two triangle plane so the OutlineActualShader can do its magic using the Normalized Device Coordinates positions of these vertices:
	//doing draw elements to a shader that ONLY HAS VERTEX POSITION (aPos) and not multiplying with a transform, projection or view, means that the vertices will be layed out in Normalized Device Coordinates: https://learnopengl.com/Getting-started/Hello-Triangle
	glm::vec3 vert1 = { 1.0f,  1.0f,  0.0f };
	glm::vec3 vert2 = { 1.0f, -1.0f,  0.0f };
	glm::vec3 vert3 = { -1.0f,-1.0f,  0.0f };
	glm::vec3 vert4 = { -1.0f, 1.0f,  0.0f };

	std::vector<glm::vec3> verts = { vert1 / 2.0f, vert2 / 2.0f, vert3 / 2.0f, vert4 / 2.0f }; // div by 2.0f for smaller icon.
	std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };

	glm::vec2 texCoord1 = { 1.0f, 1.0f };
	glm::vec2 texCoord2 = { 1.0f, 0.0f };
	glm::vec2 texCoord3 = { 0.0f, 0.0f };
	glm::vec2 texCoord4 = { 0.0f, 1.0f };

	std::vector<glm::vec2> indexed_uvs = { texCoord1, texCoord2, texCoord3, texCoord4 };

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW); //the indices might be wrong

	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //these are probably not right, need to modify to read correctly.

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, iconTexture->TextureObject); //so we have a new texture binding in Draw() because otherwise fragment shader would take the last binded texture, this allows us to use different textures for different objects (in the future).
	ShaderManager::Get().iconShader->SetInt(5, "icon");

	glBindVertexArray(VAO); //only bind VAO when drawing the mesh since it already has a VBO reference already.
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); //doing draw elements to a shader that only has vertex position and not a transform, projection or view, means that the vertices will be layed out in Normalized Device Coordinates: https://learnopengl.com/Getting-started/Hello-Triangle
	glBindVertexArray(0);
}

void LightComponent::SetIconTexture()
{
	delete iconTexture;
	iconTexture = new Texture(TextureManager::Get().lightIconPaths[selectedType].path().string().c_str(), 5, 1, false, true, true); //assumes order of entries in lightIconPaths is the same as the other uses of selectedType in this script.
}

void LightComponent::Serialization(std::fstream& file)
{
	int lightTypeSize = static_cast<int>(myLight->type);
	file.write(reinterpret_cast<char*>(&lightTypeSize), sizeof(int));

	file.write(reinterpret_cast<char*>(&myLight->ID), sizeof(int));
	file.write(reinterpret_cast<char*>(&selectedType), sizeof(int));

	file.write(reinterpret_cast<char*>(&allowDrawIcon), sizeof(int));

	myLight->Serialization(file);
}

void LightComponent::Deserialization(std::fstream& file)
{
	int lightTypeSize;
	file.read(reinterpret_cast<char*>(&lightTypeSize), sizeof(int));

	LightType currentType = static_cast<LightType>(lightTypeSize);
	LightManager::Get().DeleteLight(myLight->type, *myLight);
	myLight = LightManager::Get().AddNewLight(currentType);

	file.read(reinterpret_cast<char*>(&myLight->ID), sizeof(int));
	file.read(reinterpret_cast<char*>(&selectedType), sizeof(int));

	file.read(reinterpret_cast<char*>(&allowDrawIcon), sizeof(int));

	SetIconTexture();

	myLight->UpdateIDBasedStrings();

	myLight->Deserialization(file);
}
