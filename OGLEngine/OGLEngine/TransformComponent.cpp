#include "TransformComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

TransformComponent::TransformComponent()
{
	name = "Transform component";
	type = ComponentType::Transform;

	position = glm::vec3(0, 0, 0);
	rotation = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
}

TransformComponent::~TransformComponent()
{
	std::cout << "-->Deleting Transform component." << std::endl;
}

void TransformComponent::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	Component::DrawComponentSpecificImGuiHierarchyAdjustables();
	if (isMarkedForDeletion)
	{
		return;
	}
	ImGui::DragFloat3("Position", &position.x, .01f);
	ImGui::DragFloat3("Rotation", &rotation.x, .01f);
	ImGui::DragFloat3("Scale", &scale.x, .01f);
}

void TransformComponent::Serialization(std::fstream& file)
{
	file.write(reinterpret_cast<char*>(&position[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&rotation[0]), sizeof(glm::vec3));
	file.write(reinterpret_cast<char*>(&scale[0]), sizeof(glm::vec3));
}

void TransformComponent::Deserialization(std::fstream& file)
{
	file.read(reinterpret_cast<char*>(&position[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&rotation[0]), sizeof(glm::vec3));
	file.read(reinterpret_cast<char*>(&scale[0]), sizeof(glm::vec3));
}
