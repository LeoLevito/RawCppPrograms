#include "TransformComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

TransformComponent::TransformComponent()
{
	name = "Transform component";

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
	ImGui::DragFloat3("Position", &position.x, .01f);
	ImGui::DragFloat3("Rotation", &rotation.x, .01f);
	ImGui::DragFloat3("Scale", &scale.x, .01f);
}
