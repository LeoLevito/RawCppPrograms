#pragma once
#include "Component.h"
#include <glm.hpp>
class RigidbodyComponent : public Component
{
public:
	RigidbodyComponent();
	~RigidbodyComponent();

	void Update() override;
	void DrawComponentSpecificImGuiHierarchyAdjustables() override;

	void ApplyVelocity(float deltaTime, glm::vec3 velocityToAdd);
	void ApplyGravity(float deltaTime);


	glm::vec3 position;
	glm::vec3 rotation;
	//glm::vec3 scale;

	bool isKinematic;
	bool useGravity;

	float gravityMultiplier = 1.0f;
	float mass = 1.0f;
	float restitution = 0.1f; //this is just supposed to be between 0 and one.

	glm::vec3 velocity = { 0,0,0 };
	glm::vec3 lastVelocity = { 0,0,0 };
	glm::vec3 gravity = { 0,-9.81f,0 };
	glm::vec3 acceleration = { 0,0,0 };


	


	bool setupMyRigidbody = false;

	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;
private:
};

