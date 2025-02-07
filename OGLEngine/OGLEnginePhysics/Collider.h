#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm.hpp>
#include "GameObject.h"

class ColliderComponent;

enum class ColliderType
{
	SphereType,
	BoxType,
	MeshType,
	RaycastType
};

class Collider
{
public:
	Collider();
	ColliderType type;

	virtual void SetPosition(glm::vec3 pos);
	virtual void SetRotation(glm::vec3 rot);
	virtual void SetScale(glm::vec3 scaleIn);

	GameObject* topParent = nullptr;

	virtual void DrawImgui();
private:
};

