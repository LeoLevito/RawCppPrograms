#pragma once
#include <glm.hpp>
class Camera
{
	
public:

	void CameraUpdate();
	void CameraMove(const glm::vec3& move);
	void SetDirection(const glm::vec3& direction);
	glm::mat4 myView;

	static Camera& Get(); //get dingleton
	static void Allocate();

	glm::mat4 projection; //time to make this a singleton/static globally accessible. Will put this in camera since it dictates both FOV, near and far clip lanes.
private:
	glm::vec3 myDirection;
	glm::vec3 myPosition;
	glm::vec3 myUp;
	glm::vec3 myRight;

	static Camera* instance;
	Camera(); //is this standard practice? or do you only do this if you want the singleton getter to be able to be defined in the cpp file?
};

