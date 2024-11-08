#include "Engine.h"
//int main()
//{
//	//Engine::Initialize(); //wait what the hell is Martin doing here, did he stitch together two screenshots seamlessly?
//	//Engine::EngineTick();
//}

void Engine::Initialize(GLFWwindow* window, Camera* camera)
{
	//graphics->Initialize(640, 480); //martin must've made this a static library or struct or whatever like he said at somepoint in the 2nd lecture.
	myInput = new Input(window);
	myEngineTime = new EngineTime();
	myFlyingCamera = new FlyingCamera(camera, myInput, myEngineTime);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Lock the cursor to the center of the window.
}

void Engine::Update(float deltaTime)
{
	//while (!graphics->ShouldClose())
	//{
	//	graphics->Render();
	//}

	myFlyingCamera->Update();
	myEngineTime->UpdateDeltaTime(deltaTime);
}
