#pragma once
#include "Graphics.h" 
#include "Input.h" 
#include "FlyingCamera.h" 
#include "EngineTime.h" 

class FlyingCamera;


class Engine
{

public:

	static Engine& Get();

	void Initialize(GLFWwindow* window, Camera* camera);
	void Update(GLFWwindow* window, float inDeltaTime);
	float lastX;
	float lastY;
	FlyingCamera* myFlyingCamera;
	float deltaTime;

	bool mouseWasWithinSceneWindow;
	bool mouseWasOutsideSceneWindow;
private:
	Graphics* graphics;
	Engine* engine;
	Input* myInput;

	EngineTime* myEngineTime;

	bool DoOnce = false;

};

