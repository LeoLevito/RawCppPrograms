#pragma once
#include <glm.hpp>
class Camera
{
private:
	Camera(); //is this standard practice? or do you only do this if you want the singleton getter to be able to be defined in the cpp file?
public:
	static Camera& Get(); //get dingleton

	void CameraUpdate();
	void CameraMove(const glm::vec3& move);
	void SetDirection(const glm::vec3& direction);
	void UpdateCameraProjection();
	glm::mat4 myView;
	glm::mat4 projection; //time to make this a singleton/static globally accessible. Will put this in camera since it dictates both FOV, near and far clip lanes.
	float FOV;
	float nearClipLane;
	float farClipLane;

	bool isOrthographic;

	glm::vec3 myDirection;
	glm::vec3 myPosition;
	glm::vec3 myUp;
	glm::vec3 myRight;

	glm::vec3 startPosition{ 0,0,3 };
private:

};

