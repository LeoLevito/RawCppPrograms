#pragma once
#include "Camera.h"
#include "Input.h"
#include "EngineTime.h"

class FlyingCamera
{
public:
	FlyingCamera(Camera* camera, Input* input, EngineTime* engineTime, GLFWwindow* window);
	void Update();
	void SetNewCursorPosition();
	void MoveCamera();
	void RotateCamera(bool firstTimeRun);
	void MyScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
private:
	Camera* myCamera;
	Input* myInput;
	EngineTime* myEngineTime;

	float yaw, pitch;

	double lastX, lastY;

	GLFWwindow* myWindow;
};

