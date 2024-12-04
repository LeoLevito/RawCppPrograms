#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>

glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); //like is this allowed in C++?

Camera* Camera::instance = nullptr; //it's supposedly possible to avoid this, see https://youtu.be/PPup1yeU45I?t=888

Camera::Camera()
{
	projection = glm::perspective(glm::radians(45.0f), 1280.f / 720.f, 0.1f, 100.0f); //should make variables out of 1280 / 720.

	myDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	myPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	myUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::CameraUpdate()
{
	myRight = glm::normalize(glm::cross(myDirection, WorldUp));
	myUp = glm::cross(myRight, myDirection);
	myView = glm::lookAt(myPosition, myPosition + myDirection, myUp); //we get a matrix looking at a specified point. We also specify the up direction so our camera isn't upside down. 
	//we will later write myView to the vertex shader.
}

void Camera::CameraMove(const glm::vec3& move)
{
	myPosition += myDirection * move.z;
	myPosition += myRight * move.x;
	myPosition += WorldUp * move.y;
	//std::cout << "CAMERA LOCATION: " << myPosition.x << " " << myPosition.y << " " << myPosition.z << "\n";
}

void Camera::SetDirection(const glm::vec3& direction)
{
	myDirection = glm::normalize(direction);
	//std::cout << "CAMERA ROTATION(?) NORMALIZED: " << myDirection.x << " " << myDirection.y << " " << myDirection.z << "\n";
}



Camera& Camera::Get()
{
	return *instance;
}

void Camera::Allocate()
{
	//we only want one instance so we do an assert.
	assert(instance == nullptr); //assert will terminate the program if its argument is false. Common to use when debugging to make the program fail more obviously if an unexpected condition occurs. So basically this is a nullptr check that will terminate the program if false, if I understand correctly.
	instance = new Camera();
}

//projection matrix is something our objects will be transformed with. The projection matrix calculations happen when we multiply the rest of our matrices (in the vertex shader)