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
	type = ComponentType::Collider;

	myCollider = CollisionManager::Get().AddNewCollider(ColliderType::SphereType, *owner, *this);
	myCollider->parent = this; //why do i do this again after AddNewCollider()?
	myCollider->topParent = owner;
	CollisionManager::Get().sphereColliderVector.push_back(dynamic_cast<SphereCollider*>(myCollider));  //this might happen too early, this happens before the parent game object gets to assign itself as the owner of this component, which could lead to the read access violation I get in the CollisionManager.
																										//though it shouldn't matter, since we did the check for the rigidbody before and got the same error.
																										//Something weird happens here, when push_back is done, the CollisionManager's Process() function tries to access it immediately,
																										//EVEN BEFORE THE CONSTRUCTOR IS DONE WITH EVERYTHING.
	myRigidbody = nullptr;
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

			if (myCollider != nullptr)
			{
				myCollider->SetPosition(position);
				myCollider->SetRotation(rotation);
				myCollider->SetScale(scale);
				myCollider->Update();
			}
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

	const char* typeNames[] = { "Sphere collider", "Box collider", "Mesh collider SAT", "Raycast collider" };

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
				ColliderType type = static_cast<ColliderType>(selectedType);
				switch (type)
				{
				case ColliderType::SphereType:
					if (myCollider != nullptr)
					{
						CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					}
					myCollider = CollisionManager::Get().AddNewCollider(type, *owner, *this);
					myCollider->parent = this;
					CollisionManager::Get().sphereColliderVector.push_back(static_cast<SphereCollider*>(myCollider));
					break;
				case  ColliderType::BoxType:
					if (myCollider != nullptr)
					{
						CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					}
					myCollider = CollisionManager::Get().AddNewCollider(type, *owner, *this);
					myCollider->parent = this;
					CollisionManager::Get().boxColliderVector.push_back(static_cast<BoxCollider*>(myCollider));
					break;
				case  ColliderType::MeshType:
					CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					myCollider = CollisionManager::Get().AddNewCollider(type, *owner, *this);
					break;
				case  ColliderType::RaycastType:
					if (myCollider != nullptr)
					{
						CollisionManager::Get().DeleteCollider(myCollider->type, myCollider);
					}
					myCollider = CollisionManager::Get().AddNewCollider(type, *owner, *this);
					myCollider->parent = this; //do this here again after AddNewCollider to make it so parent has the correct myCollider value instead of a nullptr after the DeleteCollider.
					CollisionManager::Get().raycastColliderVector.push_back(static_cast<RaycastCollider*>(myCollider)); //fixes issue where doing push_back in the AddNewCollider would have an incomplete parent reference, later causing an error below on mycollider->DrawImGui().
					break;
				default:
					break;
				}
			}
		}
		ImGui::EndPopup();
	}
	if (myCollider != nullptr)
	{
		myCollider->DrawImgui();
	}
}

void ColliderComponent::Serialization(std::fstream& file)
{
	int ColliderTypeSize = static_cast<int>(myCollider->type); //type is also a pointer so gotta do [0] to get memory address of it.
	file.write(reinterpret_cast<char*>(&ColliderTypeSize), sizeof(int));

	file.write(reinterpret_cast<char*>(&selectedType), sizeof(int));

	myCollider->Serialization(file);
}

void ColliderComponent::Deserialization(std::fstream& file)
{
	int ColliderTypeSize;
	file.read(reinterpret_cast<char*>(&ColliderTypeSize), sizeof(int));

	file.read(reinterpret_cast<char*>(&selectedType), sizeof(int));

	ColliderType currentType = static_cast<ColliderType>(ColliderTypeSize);

	if (myCollider != nullptr)
	{
		CollisionManager::Get().DeleteCollider(myCollider->type, myCollider); //this can become an issue in the future since it only marks it for deletion, actual deletion may happen later than executions below this scope, causing errors.
	}
	myCollider = CollisionManager::Get().AddNewCollider(currentType, *owner, *this);
	myCollider->parent = this; //why do I do this again after AddNewCollider()?

	switch (currentType)
	{
	case ColliderType::SphereType:
		CollisionManager::Get().sphereColliderVector.push_back(dynamic_cast<SphereCollider*>(myCollider));
		break;
	case ColliderType::BoxType:
		CollisionManager::Get().boxColliderVector.push_back(dynamic_cast<BoxCollider*>(myCollider));
		break;
	case ColliderType::MeshType:

		break;
	case ColliderType::RaycastType:
		CollisionManager::Get().raycastColliderVector.push_back(dynamic_cast<RaycastCollider*>(myCollider)); //fixes issue where doing push_back in the AddNewCollider would have an incomplete parent reference, later causing an error below on mycollider->DrawImGui().
		break;
	default:
		break;
	}

	myCollider->Deserialization(file);
}
