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
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"


GameObject::GameObject()
{
	name = "Game object";
	components.reserve(3 * sizeof(Component*));
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

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
}

void GameObject::DrawObjectSpecificImGuiHierarchyAdjustables(std::vector<GameObject*>& vec) //maybe this should be called in update instead.
{
	ImGui::Text("ID:");
	ImGui::SameLine();
	ImGui::Text(std::to_string(ID).c_str());

	static char str0[128] = "name"; //how it's done in the ImGui demo, tho it is replicated across all objects now...
	ImGui::InputText("Object name", str0, IM_ARRAYSIZE(str0)); //Yeah, I gotta change this to a dropdown or something.
	if (ImGui::Button("Change object name"))
	{
		name = str0;
	}

	static int selectedComponent = -1;
	const char* componentNames[] = { "Transform component", "Mesh component", "Light component", "Collider component", "Rigidbody component" };
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
				else if (i == 3)
				{
					AddComponent(new ColliderComponent);
				}
				else if (i == 4)
				{
					AddComponent(new RigidbodyComponent);
				}
			}
			//ImGui::EndDisabled();
		}
		ImGui::EndPopup();
	}

	if (ImGui::Button("Add EVERY component"))
	{
		AddComponent(new TransformComponent);
		AddComponent(new MeshComponent);
		AddComponent(new LightComponent);
		AddComponent(new ColliderComponent);
		AddComponent(new RigidbodyComponent);
	}


	if (ImGui::Button("Remove Game Object"))
	{
		//maybe I should make this a function, in case I want to call delete from another class. This would require vec be more accessible, like a singleton maybe.
		GameObjectManager::Get().DeleteGameObject(this);
		std::cout << "Deletion of Game Object completed." << std::endl;
	}
}

void GameObject::Serialization(std::fstream& file)
{
	int nameSize = name.size();
	int componentsSize = components.size();
	file.write(reinterpret_cast<char*>(&nameSize), sizeof(int));
	file.write(reinterpret_cast<char*>(&componentsSize), sizeof(int));

	file.write(reinterpret_cast<char*>(&name[0]), nameSize); //https://stackoverflow.com/a/37035925

	file.write(reinterpret_cast<char*>(&ID), sizeof(int));
}

void GameObject::Deserialization(std::fstream& file)
{
	int nameSize;
	int componentsSize;
	file.read(reinterpret_cast<char*>(&nameSize), sizeof(int)); 
	file.read(reinterpret_cast<char*>(&componentsSize), sizeof(int));
	name.resize(nameSize);
	components.resize(componentsSize);

	file.read(reinterpret_cast<char*>(&name[0]), nameSize); //https://stackoverflow.com/a/37035925

	file.read(reinterpret_cast<char*>(&ID), sizeof(int));
}
