#pragma once
#include "Collider.h"
#include <glm.hpp>

class SphereCollider : public Collider
{
public:

	virtual void SetPosition(glm::vec3 pos) override;

	virtual void DrawImgui() override;

	glm::vec3 position;
	float radius;
private:
};

