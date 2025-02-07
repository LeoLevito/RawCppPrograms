#pragma once
#include "Collider.h"
#include <glm.hpp>

class SphereCollider : public Collider
{
public:
	SphereCollider();
	virtual void SetPosition(glm::vec3 pos) override;

	virtual void DrawImgui() override;

	glm::vec3 position{ glm::vec3(0.0f,0.0f,0.0f) };
	float radius{ 1.0f };
private:
};

