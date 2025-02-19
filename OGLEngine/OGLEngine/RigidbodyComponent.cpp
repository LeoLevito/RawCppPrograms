#include "RigidbodyComponent.h"
#include <GameObject.h>
#include <TransformComponent.h>
#include <Engine.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <ColliderComponent.h>

RigidbodyComponent::RigidbodyComponent()
{
	name = "Rigidbody component";
	
}

RigidbodyComponent::~RigidbodyComponent()
{
}

void RigidbodyComponent::Update()
{
	if (useGravity)
	{
		for (size_t i = 0; i < owner->components.size(); i++)
		{
			if (dynamic_cast<TransformComponent*>(owner->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
			{
				//multiple casts, in UPDATE()? not sure how efficient this is. EDIT 5 dec 2024, apparently this dynamic_cast is better than other types of casts, even Emil has used them multiple times.
				velocity += gravity * gravityMultiplier * Engine::Get().deltaTime;
				dynamic_cast<TransformComponent*>(owner->components[i])->position += velocity * Engine::Get().deltaTime; //multiplying again with delta time results in the proper acceleration of gravity... in theory.
			}
		}
	}
	else 
	{
		velocity = { 0,0,0 };
	}

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
	if (ImGui::Checkbox("Use Gravity", &useGravity))
	{
	}

	if (ImGui::Button("reverse velocity"))
	{
		velocity *= -1;
	}

	if (ImGui::DragFloat("Gravity multiplier", &gravityMultiplier, .01f))
	{
	}
}
