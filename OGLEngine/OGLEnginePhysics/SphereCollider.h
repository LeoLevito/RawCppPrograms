#pragma once
#include "Collider.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	virtual void SetPosition(glm::vec3 pos) override;

	virtual void DrawImgui() override;

	glm::vec3 position{ 0.0f,0.0f,0.0f };
	float radius{ 1.0f };

	bool hasGotFirstPosition = false;

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;
private:
};

