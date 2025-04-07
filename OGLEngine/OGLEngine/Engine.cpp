#include "Engine.h"
#include "EditorGUI.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
	if (myInput->IsMouseButtonDown(GLFW_MOUSE_BUTTON_2)) //right click
	{
		if (DoOnce == false)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.MousePos.x >= EditorGUI::Get().sceneWindowPosX
				&& io.MousePos.x <= EditorGUI::Get().sceneWindowPosX + EditorGUI::Get().sceneWindowWidth
				&& io.MousePos.y >= EditorGUI::Get().sceneWindowPosY
				&& io.MousePos.y <= EditorGUI::Get().sceneWindowPosY + EditorGUI::Get().sceneWindowHeight)
			{
				std::cout << "Inside of window" << std::endl;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				DoOnce = true;
				myFlyingCamera->SetNewCursorPosition();
				mouseWasWithinSceneWindow = true;
			}
			else
			{
				std::cout << "Outside of window" << std::endl;
				DoOnce = true;
				mouseWasOutsideSceneWindow = true;
			}
		}
	}
	else if (myInput->IsMouseButtonDown(GLFW_MOUSE_BUTTON_1)) //left click
	{
		if (DoOnce == false)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.MousePos.x >= EditorGUI::Get().sceneWindowPosX
				&& io.MousePos.x <= EditorGUI::Get().sceneWindowPosX + EditorGUI::Get().sceneWindowWidth
				&& io.MousePos.y >= EditorGUI::Get().sceneWindowPosY
				&& io.MousePos.y <= EditorGUI::Get().sceneWindowPosY + EditorGUI::Get().sceneWindowHeight)
			{
				std::cout << "clicked mouse one Inside of window" << std::endl;
				DoOnce = true;
				mouseWasWithinSceneWindow = true;
			}
			else
			{
				std::cout << "clicked mouse one Outside of window" << std::endl;
				DoOnce = true;
				mouseWasOutsideSceneWindow = true;
			}
		}
	}
	else //no click
	{
		if (DoOnce == true)
		{
			std::cout << "Not clicking mouse one or two" << std::endl;

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			DoOnce = false;
			mouseWasWithinSceneWindow = false;
			mouseWasOutsideSceneWindow = false;
		}
	}

	myFlyingCamera->Update();
	myEngineTime->UpdateDeltaTime(deltaTime);
	deltaTime = inDeltaTime;
}
