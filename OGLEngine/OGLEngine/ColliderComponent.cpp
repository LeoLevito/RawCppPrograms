#include "ColliderComponent.h"
#include <iostream>
#include <GameObject.h>
#include "TransformComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CollisionManager.h"


ColliderComponent::ColliderComponent()
{
	name = "Collider component";
	myCollider = CollisionManager::Get().AddNewCollider(ColliderType::SphereType, *owner);

}

ColliderComponent::~ColliderComponent()
{
	std::cout << "-->Deleting Collider component." << std::endl;
	CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);

}

void ColliderComponent::Update()
{
	for (size_t i = 0; i < owner->components.size(); i++)
	{
		if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		{
			//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
			position = dynamic_cast<TransformComponent*>(owner->components[i])->position; //need to get a reference to the transform component if there is one.
			rotation = dynamic_cast<TransformComponent*>(owner->components[i])->rotation;
			scale = dynamic_cast<TransformComponent*>(owner->components[i])->scale;

			myCollider->SetPosition(position);
			myCollider->SetRotation(rotation);
			myCollider->SetScale(scale);
			myCollider->Update();
		}
	}


}

void ColliderComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
	if (isMarkedForDeletion)
	{
		return;
	}

	const char* typeNames[] = { "Sphere collider", "Box collider", "Mesh collider SAT", "Raycast collider"};

	ImGui::Text("Current type:");
	ImGui::SameLine();
	ImGui::Text(typeNames[selectedType]); 


	if (ImGui::Button("Select type"))
	{
		ImGui::OpenPopup("Collider Type Popup");
	}

	if (ImGui::BeginPopup("Collider Type Popup"))
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
					CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					myCollider = CollisionManager::Get().AddNewCollider(ColliderType::SphereType, *owner);
					break;
				case 1:
					CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					myCollider = CollisionManager::Get().AddNewCollider(ColliderType::BoxType, *owner);
					break;
				case 2:
					CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					myCollider = CollisionManager::Get().AddNewCollider(ColliderType::MeshType, *owner);
					break;
				case 3:
					CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					myCollider = CollisionManager::Get().AddNewCollider(ColliderType::RaycastType, *owner);
					break;
				default:
					break;
				}
			}
		}
		ImGui::EndPopup();
	}
	myCollider->DrawImgui();
}
