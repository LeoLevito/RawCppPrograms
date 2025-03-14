#include "LightComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "LightManager.h"
#include "ShaderManager.h"

LightComponent::LightComponent()
{
	name = "Light component";
	type = ComponentType::Light;

	myLight = LightManager::Get().AddNewLight(LightType::DirectionalLightType);
	myLight->SetToDefault();
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
}

void LightComponent::Serialization(std::fstream& file)
{
	int lightTypeSize = static_cast<int>(myLight->type);
	file.write(reinterpret_cast<char*>(&lightTypeSize), sizeof(int));

	file.write(reinterpret_cast<char*>(&myLight->ID), sizeof(int));
	file.write(reinterpret_cast<char*>(&selectedType), sizeof(int));

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

	myLight->UpdateIDBasedStrings();

	myLight->Deserialization(file);
}
