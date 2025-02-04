#pragma once
#include "Collider.h"
#include <glm.hpp>

class BoxCollider : public Collider
{
public:

	virtual void SetPosition(glm::vec3 pos) override;

	virtual void DrawImgui() override;

	glm::vec3 position;
	glm::vec3 extents;
private:
};

