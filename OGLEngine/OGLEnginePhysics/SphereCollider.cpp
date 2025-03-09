#include "SphereCollider.h"
#include "CollisionManager.h"

SphereCollider::SphereCollider()
{
	position = { 0.0f,0.0f,0.0f };
	radius = { 1.0f };

	hasGotFirstPosition = false;
}

SphereCollider::~SphereCollider()
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

void SphereCollider::Serialization(std::fstream& file)
{
	int radiusSize = sizeof(float);

	file.write(reinterpret_cast<char*>(&radiusSize), sizeof(radiusSize));

	file.write(reinterpret_cast<char*>(&radius), radiusSize);

}

void SphereCollider::Deserialization(std::fstream& file)
{
	int radiusSize;

	file.read(reinterpret_cast<char*>(&radiusSize), sizeof(radiusSize));

	file.read(reinterpret_cast<char*>(&radius), radiusSize);

}
