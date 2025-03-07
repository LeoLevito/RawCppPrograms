#pragma once
#include "Collider.h"

class RaycastCollider : public Collider
{
public:

	RaycastCollider();
	~RaycastCollider();
	//default position and rotation variables, may be used as startPoint and direction if desired.
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	//start & endpoints
	glm::vec3 startPoint;
	glm::vec3 endPoint;

	//or: direction and distance, using startPoint.
	glm::vec3 direction;
	float distance;

	bool useDirectionAndDistanceAsEndPoint = false;
	bool usePositionAsStartPoint = false;
	bool useRotationAsDirection = false;
	bool drawDebugLine = false;


	virtual void SetPosition(glm::vec3 pos) override;
	virtual void SetRotation(glm::vec3 rot) override;
	virtual void SetScale(glm::vec3 scaleIn) override;


	virtual void Update() override;
	virtual void DrawImgui() override;

	void CalculateStartAndEndPoints();

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

 
	void Serialization(std::fstream& file) override;
	void Deserialization(std::fstream& file) override;
private:
};

