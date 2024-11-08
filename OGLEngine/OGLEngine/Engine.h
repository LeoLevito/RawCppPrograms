#pragma once
#include "Graphics.h" 
#include "Input.h" 
#include "FlyingCamera.h" 
#include "EngineTime.h" 
class Engine
{

public:
	void Initialize(GLFWwindow* window, Camera* camera);
	void Update(float deltaTime);
private:
	Graphics* graphics;
	Engine* engine;
	Input* myInput;
	FlyingCamera* myFlyingCamera;
	EngineTime* myEngineTime;
};

