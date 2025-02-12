#pragma once
#include "Collider.h"
#include <glm.hpp>
#include <vector>


class BoxCollider : public Collider
{
public:
	BoxCollider();
	virtual void SetPosition(glm::vec3 pos) override;
	virtual void SetRotation(glm::vec3 rot) override;
	virtual void SetScale(glm::vec3 scaleIn) override;

	void UpdateBounds();

	virtual void DrawImgui() override;

	glm::vec3 position; //center of box
	glm::vec3 rotation;
	glm::vec3 scale;
	std::vector<glm::vec3> corners; //corners of box in relation to position
	std::vector<glm::vec3> normalVector; //normals of each corner.
	std::vector<glm::vec3> averageVector; //vertex averages of each face.

	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 forward;
private:
};

