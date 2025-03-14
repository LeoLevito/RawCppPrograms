#include "RigidbodyComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <Engine.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <ColliderComponent.h>
#include <CollisionManager.h>

RigidbodyComponent::RigidbodyComponent()
{
	name = "Rigidbody component";
	type = ComponentType::Rigidbody;

}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Update()
{
	if (owner != nullptr && setupMyRigidbody == false)
	{
		for (size_t i = 0; i < owner->components.size(); i++) //too early.
		{
			if (dynamic_cast<ColliderComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
			{
				//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
				dynamic_cast<ColliderComponent*>(owner->components[i])->myRigidbody = this;
				setupMyRigidbody = true;
			}
		}
	}
}

void RigidbodyComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();

	if (ImGui::Checkbox("Is kinematic", &isKinematic))
	{
	}

	if (ImGui::Checkbox("Use gravity", &useGravity))
	{
	}

	if (ImGui::Button("reverse velocity"))
	{
		velocity *= -1;
	}

	if (ImGui::DragFloat("Gravity multiplier", &gravityMultiplier, .01f))
	{
	}

	if (ImGui::DragFloat("Mass", &mass, .01f))
	{
	}

	if (ImGui::DragFloat("Restitution", &restitution, .01f))
	{
	}
}

void RigidbodyComponent::ApplyVelocity(float deltaTime, glm::vec3 velocityToAdd)
{
	if (isKinematic) //not needed since we already do the check in CollisionManager, but in the future doing this here would clean up the CollisionManager.
	{
		velocity = { 0,0,0 };
	}
	else
	{
		velocity += velocityToAdd;
		//for (size_t i = 0; i < owner->components.size(); i++)
		//{
		//	if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
		//	{
		//		//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.

		//		dynamic_cast<TransformComponent*>(owner->components[i])->position += velocity * deltaTime; //multiplying again with delta time results in the proper acceleration of gravity... in theory.
		//	}
		//}
	}
}

void RigidbodyComponent::ApplyGravity(float deltaTime)
{
	if (CollisionManager::Get().simulate)
	{
		if (useGravity) //move this check somewhere else, we should be checking isKinematic first, apply any velocities to the position and then do the gravity check and if it's true then we also apply gravity to the velocities.
		{
			velocity += gravity * gravityMultiplier * deltaTime;
			for (size_t i = 0; i < owner->components.size(); i++)
			{
				if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
				{
					//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.

					dynamic_cast<TransformComponent*>(owner->components[i])->position += velocity * deltaTime; //multiplying again with delta time results in the proper acceleration of gravity... in theory.
				}
			}
		}
	}
	else 
	{
		velocity = { 0,0,0 }; //not having this here would make it so we can stop simulation and then start it again with the previous velocities applied, which could be useful in the future.
	}
}

void RigidbodyComponent::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&isKinematic), sizeof(int));
	file.write(reinterpret_cast<char*>(&useGravity), sizeof(int));
	file.write(reinterpret_cast<char*>(&gravityMultiplier), sizeof(float));
	file.write(reinterpret_cast<char*>(&mass), sizeof(float));
	file.write(reinterpret_cast<char*>(&restitution), sizeof(float));
}

void RigidbodyComponent::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&isKinematic), sizeof(int));
	file.read(reinterpret_cast<char*>(&useGravity), sizeof(int));
	file.read(reinterpret_cast<char*>(&gravityMultiplier), sizeof(float));
	file.read(reinterpret_cast<char*>(&mass), sizeof(float));
	file.read(reinterpret_cast<char*>(&restitution), sizeof(float));
}
