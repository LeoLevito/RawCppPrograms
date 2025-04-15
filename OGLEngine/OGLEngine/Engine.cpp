#include "Engine.h"
#include "EditorGUI.h"
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		if (!EditorGUI::Get().manipulatingImGuizmo)
		{
			ImGuiIO& io = ImGui::GetIO();
			if (io.MousePos.x >= EditorGUI::Get().sceneWindowPosX
				&& io.MousePos.x <= EditorGUI::Get().sceneWindowPosX + EditorGUI::Get().sceneWindowWidth
				&& io.MousePos.y >= EditorGUI::Get().sceneWindowPosY
				&& io.MousePos.y <= EditorGUI::Get().sceneWindowPosY + EditorGUI::Get().sceneWindowHeight)
			{
				Graphics::Get().RenderPickingPass(); //Render scene once when clicking inside the SceneWindow, determine what object was clicked.
			}
		}
	}

	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	//{
	//	ImGuiIO& io = ImGui::GetIO();
	//	if (io.MousePos.x >= EditorGUI::Get().sceneWindowPosX
	//		&& io.MousePos.x <= EditorGUI::Get().sceneWindowPosX + EditorGUI::Get().sceneWindowWidth
	//		&& io.MousePos.y >= EditorGUI::Get().sceneWindowPosY
	//		&& io.MousePos.y <= EditorGUI::Get().sceneWindowPosY + EditorGUI::Get().sceneWindowHeight)
	//	{
	//		std::cout << "Inside of window" << std::endl;
	//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//		io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
	//		Engine::Get().myFlyingCamera->SetNewCursorPosition();
	//		Engine::Get().mouseWasWithinSceneWindow = true;
	//	}
	//}

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		std::cout << "Not clicking mouse one or two" << std::endl;
		ImGuiIO& io = ImGui::GetIO();
		Engine::Get().DoOnce = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		Engine::Get().mouseWasWithinSceneWindow = false;
	}
}

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
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Engine::Update(GLFWwindow* window, float inDeltaTime)
{
	if (myInput->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) //right click
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
		}
	}
	myFlyingCamera->Update();
	myEngineTime->UpdateDeltaTime(deltaTime);
	deltaTime = inDeltaTime;
}
