#pragma once
#include "Component.h"
#include "Collider.h"
#include "RigidbodyComponent.h"

//class Collider;

class ColliderComponent : public Component
{
public:
	ColliderComponent();
	~ColliderComponent();
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	void Update() override;

	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	int selectedType = 0;

	Collider* myCollider;
	RigidbodyComponent* myRigidbody;
private:

};
