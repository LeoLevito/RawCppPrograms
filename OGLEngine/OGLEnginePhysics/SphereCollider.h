#pragma once
#include "Collider.h"
#include "Mesh.h"

class SphereCollider : public Collider
{
public:
	SphereCollider();
	~SphereCollider();
	virtual void SetPosition(glm::vec3 pos) override;
	virtual void SetRotation(glm::vec3 rot) override;

	virtual void Update() override;
	virtual void DrawImgui() override;
	virtual void DrawDebugLines() override;
	void ActuallyDrawDebugLines(glm::mat4 trans, int axis, float radius);

	glm::vec3 position{ 0.0f,0.0f,0.0f };
	glm::vec3 rotation{ 0.0f,0.0f,0.0f };
	float radius{ 1.0f };

	bool hasGotFirstPosition = false;

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;

	bool drawDebugLines;

	Mesh* sphereColliderVisualizer;

	int roundingAmount = 32;
	float divByRoundingAmount = (360.f / roundingAmount);
private:
};

