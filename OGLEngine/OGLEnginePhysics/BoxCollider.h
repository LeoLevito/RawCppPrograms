#pragma once
#include "Collider.h"
#include <glm.hpp>
#include <vector>


class BoxCollider : public Collider
{
public:
	BoxCollider(ColliderComponent& componentRef);
	virtual void SetPosition(glm::vec3 pos) override;

	virtual void DrawImgui() override;

	glm::vec3 position; //center of box
	std::vector<glm::vec3> corners; //corners of box in relation to position
	std::vector<glm::vec3> normalVector; //normals of each corner.
private:
};

