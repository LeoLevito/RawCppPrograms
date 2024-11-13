#include "Engine.h"

void Engine::Initialize(GLFWwindow* window, Camera* camera)
{
	myInput = new Input(window);
	myEngineTime = new EngineTime();
	myFlyingCamera = new FlyingCamera(camera, myInput, myEngineTime);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Lock the cursor to the center of the window.
}

void Engine::Update(float deltaTime)
{
	myFlyingCamera->Update();
	myEngineTime->UpdateDeltaTime(deltaTime);
}
