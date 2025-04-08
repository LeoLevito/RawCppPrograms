#pragma once
#include "Engine.h"
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
	Input* myInput;

	double lastX, lastY;
private:
	Camera* myCamera;

	EngineTime* myEngineTime;

	float yaw, pitch;



	GLFWwindow* myWindow;
};

