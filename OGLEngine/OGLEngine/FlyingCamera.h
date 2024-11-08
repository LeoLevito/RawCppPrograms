#pragma once
#include "Camera.h"
#include "Input.h"
#include "EngineTime.h"
class FlyingCamera
{
public:
	FlyingCamera(Camera* camera, Input* input, EngineTime* engineTime);
	void Update();
private:
	Camera* myCamera;
	Input* myInput;
	EngineTime* myEngineTime;

	float yaw, pitch;

	double lastX, lastY;
};

