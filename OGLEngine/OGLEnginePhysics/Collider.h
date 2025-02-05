#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm.hpp>

class ColliderComponent;

enum class ColliderType
{
	SphereType,
	BoxType,
	RaycastType
};

class Collider
{
public:
	Collider();
	Collider(ColliderComponent& componentRef);
	ColliderType type;

	ColliderComponent* compRef = nullptr; //this shit is bumfuck stupid.

	virtual void SetPosition(glm::vec3 pos);

	virtual void DrawImgui();
private:
};

