#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <mutex>
#include "EditorGUI.h"

glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f); //like is this allowed in C++?

Camera::Camera()
{
	FOV = 45.f;
	nearClipLane = 0.1f;
	farClipLane = 100.f;
	isOrthographic = false;
	projection = glm::perspective(glm::radians(FOV), 1280.f / 720.f, nearClipLane, farClipLane); //should make variables out of 1280 / 720.

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

void Camera::UpdateCameraProjection()
{
	if (isOrthographic)
	{
		projection = glm::ortho(0.f, (EditorGUI::Get().sceneWindowWidth / 10.f), 0.f, (EditorGUI::Get().sceneWindowHeight / 10.f), nearClipLane, farClipLane);
	}
	else
	{
		projection = glm::perspective(glm::radians(FOV), EditorGUI::Get().sceneWindowWidth / EditorGUI::Get().sceneWindowHeight, nearClipLane, farClipLane);
	}
	
}

//pivot point move?

Camera& Camera::Get()
{
	static Camera instance;
	return instance;
}

//projection matrix is something our objects will be transformed with. The projection matrix calculations happen when we multiply the rest of our matrices (in the vertex shader)