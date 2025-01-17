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
	myLight = LightManager::Get().AddNewLight(LightType::DirectionalLightType);
	myLight->SetToDefault();
}

LightComponent::~LightComponent()
{
	std::cout << "-->Deleting Light component." << std::endl;
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
}

void LightComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();

	const char* typeNames[] = { "Directional light", "Point light", "Spot light" };

	ImGui::Text("Current type:");
	ImGui::SameLine();
	ImGui::Text(typeNames[selectedType]);
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
					LightManager::Get().DeleteLight(myLight->type, myLight);
					myLight = LightManager::Get().AddNewLight(LightType::DirectionalLightType);
					break;
				case 1:
					LightManager::Get().DeleteLight(myLight->type, myLight);
					myLight = LightManager::Get().AddNewLight(LightType::PointLightType);
					break;
				case 2:
					LightManager::Get().DeleteLight(myLight->type, myLight);
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