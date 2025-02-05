#include "SphereCollider.h"

SphereCollider::SphereCollider(ColliderComponent& componentRef)
{
}

void SphereCollider::SetPosition(glm::vec3 pos)
{
	position = pos;
}

void SphereCollider::DrawImgui()
{
	//Collider::DrawImgui();

	if (ImGui::DragFloat("Radius", &radius, .01f))
	{
	}
}
