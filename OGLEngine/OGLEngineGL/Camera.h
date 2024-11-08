#pragma once
#include <glm.hpp>
class Camera
{
public:
	Camera();
	void CameraUpdate();
	void CameraMove(const glm::vec3& move);
	void SetDirection(const glm::vec3& direction);
	glm::mat4 myView;
private:
	glm::vec3 myDirection;
	glm::vec3 myPosition;
	glm::vec3 myUp;

	glm::vec3 myRight;
};

