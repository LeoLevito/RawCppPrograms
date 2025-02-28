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
	int positionSize = position.length();
	int rotationSize = rotation.length();
	int scaleSize = scale.length();

	file.write(reinterpret_cast<char*>(&positionSize), sizeof(positionSize));
	file.write(reinterpret_cast<char*>(&rotationSize), sizeof(rotationSize));
	file.write(reinterpret_cast<char*>(&scaleSize), sizeof(scaleSize));

	file.write(reinterpret_cast<char*>(&position[0]), sizeof(glm::vec3) * positionSize);
	file.write(reinterpret_cast<char*>(&rotation[0]), sizeof(glm::vec3) * rotationSize);
	file.write(reinterpret_cast<char*>(&scale[0]), sizeof(glm::vec3) * scaleSize);
}

void TransformComponent::Deserialization(std::fstream& file)
{
	int positionSize;
	int rotationSize;
	int scaleSize;

	file.read(reinterpret_cast<char*>(&positionSize), sizeof(positionSize)); 
	file.read(reinterpret_cast<char*>(&rotationSize), sizeof(rotationSize));
	file.read(reinterpret_cast<char*>(&scaleSize), sizeof(scaleSize));

	file.read(reinterpret_cast<char*>(&position[0]), sizeof(glm::vec3) * positionSize);
	file.read(reinterpret_cast<char*>(&rotation[0]), sizeof(glm::vec3) * rotationSize);
	file.read(reinterpret_cast<char*>(&scale[0]), sizeof(glm::vec3) * scaleSize);
}
