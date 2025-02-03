#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm.hpp>

enum class ColliderType
{
	SphereType,
	BoxType,
	RaycastType
};

class Collider
{
public:
	ColliderType type;

	virtual void SetPosition(glm::vec3 pos);

	virtual void DrawImgui();
private:
};

