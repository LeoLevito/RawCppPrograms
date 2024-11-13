#include "FlyingCamera.h"
#include <glm.hpp>

const float Sensitivity = 0.5f;
const float MoveSpeed = 10.0f;

FlyingCamera::FlyingCamera(Camera* camera, Input* input, EngineTime* engineTime)
{
	myCamera = camera;
	myInput = input;
	myEngineTime = engineTime;
}

void FlyingCamera::Update()
{
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);

	if (myInput->IsKeyDown(GLFW_KEY_W)) velocity.z = 1;
	if (myInput->IsKeyDown(GLFW_KEY_A)) velocity.x = -1; //super low numbers overriding up down forward backward
	if (myInput->IsKeyDown(GLFW_KEY_S)) velocity.z = -1;
	if (myInput->IsKeyDown(GLFW_KEY_D)) velocity.x = 1; //super low numbers overriding up down forward backward

	if (myInput->IsKeyDown(GLFW_KEY_E)) velocity.y = 1;
	if (myInput->IsKeyDown(GLFW_KEY_Q)) velocity.y = -1;

	float xpos, ypos;
	xpos = myInput->GetCursorX();
	ypos = myInput->GetCursorY();

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f; //I guess these may be some kind of safeguard, will try putting it to 90 later.
	if (pitch < -89.0f) pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	myCamera->CameraMove(velocity * MoveSpeed * myEngineTime->DeltaTime());
	myCamera->SetDirection(direction);
	myCamera->CameraUpdate();
}
