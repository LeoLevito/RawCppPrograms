#include "Component.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "GameObject.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::DrawComponentSpecificImGuiHierarchyAdjustables()
{
	if (ImGui::Button("Remove component"))
	{
		owner->components.erase(std::remove(owner->components.begin(), owner->components.end(), this));
		delete this;
		std::cout << "Deletion of component completed." << std::endl;
	}
}

void Component::Update()
{
}
