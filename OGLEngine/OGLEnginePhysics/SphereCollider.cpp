#include "SphereCollider.h"

SphereCollider::SphereCollider()
{
}

void SphereCollider::SetPosition(glm::vec3 pos)
{
	if (hasGotFirstPosition == false) 
	{
		hasGotFirstPosition = true;
	}
	position = pos;
}

void SphereCollider::DrawImgui()
{
	//Collider::DrawImgui();

	if (ImGui::DragFloat("Radius", &radius, .01f))
	{
	}
}
