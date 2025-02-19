#include "Engine.h"

Engine& Engine::Get()
{
	static Engine instance;
	return instance;
}

void Engine::Initialize(GLFWwindow* window, Camera* camera)
{
	myInput = new Input(window);
	myEngineTime = new EngineTime();
	myFlyingCamera = new FlyingCamera(camera, myInput, myEngineTime, window);
	myFlyingCamera->RotateCamera(true);
}

void Engine::Update(GLFWwindow* window, float inDeltaTime)
{
	if (myInput->IsMouseButtonDown(GLFW_MOUSE_BUTTON_2))
	{
		if (DoOnce == false)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			DoOnce = true;
			myFlyingCamera->SetNewCursorPosition();
		}	
	}
	else 
	{
		if (DoOnce == true)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			DoOnce = false;
		}
	}
	myFlyingCamera->Update();
	myEngineTime->UpdateDeltaTime(deltaTime);
	deltaTime = inDeltaTime;
}
