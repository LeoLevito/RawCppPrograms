#include "GameObject.h"
#include "GameObjectManager.h"
#include "GLFW/glfw3.h"
#include "iostream"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "LightComponent.h"


GameObject::GameObject()
{
	name = "Game object";
}

GameObject::~GameObject()
{
	std::cout << "Initiating deletion of Game Object." << std::endl;
	std::cout << "Deleting components of Game Object." << std::endl;

	for (int i = 0; i < components.size(); i++)
	{
		delete components[i];
	}
	std::cout << "Deletion of Game Object's components completed." << std::endl;
	std::cout << "-->Deleting Game Object." << std::endl;
}

void GameObject::AddComponent(Component* component)
{
	component->owner = this;
	components.push_back(component);
}

void GameObject::Update(Shader* shader) 
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

void GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec) //maybe this should be called in update instead.
{
	static char str0[128] = "name"; //how it's done in the ImGui demo, tho it is replicated across all objects now...
	ImGui::InputText("Object name", str0, IM_ARRAYSIZE(str0)); //Yeah, I gotta change this to a dropdown or something.
	if (ImGui::Button("Change object name"))
	{
		name = str0;
	}

	static int selectedComponent = -1;
	const char* componentNames[] = { "Transform component", "Mesh component", "Light component" };
	if (ImGui::Button("Add component"))
	{
		ImGui::OpenPopup("Component popup");
	}
	//ImGui::SameLine();
	if (ImGui::BeginPopup("Component popup"))
	{
		ImGui::SeparatorText("Components:");

		for (int i = 0; i < IM_ARRAYSIZE(componentNames); i++)
		{
			//ImGui::BeginDisabled();
			if (ImGui::Selectable(componentNames[i])) 
			{
				selectedComponent = i;
				if (i == 0) // need to streamline this.
				{
					AddComponent(new TransformComponent);
				}
				else if (i == 1) 
				{
					AddComponent(new MeshComponent);
				}
				else if (i == 2)
				{
					AddComponent(new LightComponent);
				}
			}
			//ImGui::EndDisabled();
		}
		ImGui::EndPopup();
	}


	if (ImGui::Button("Remove Game Object"))
	{
		//maybe I should make this a function, in case I want to call delete from another class. This would require vec be more accessible, like a singleton maybe.
		GameObjectManager::Get().DeleteGameObject(this);
		std::cout << "Deletion of Game Object completed." << std::endl;
	}
}
