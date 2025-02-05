#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(ColliderComponent& componentRef)
{
	compRef = &componentRef;
}

void Collider::SetPosition(glm::vec3 pos)
{
}

void Collider::DrawImgui()
{
}
