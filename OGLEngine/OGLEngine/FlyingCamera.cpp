#include "FlyingCamera.h"
#include <glm.hpp>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


const float Sensitivity = 0.5f;
const float MoveSpeed = 1.0f;

FlyingCamera::FlyingCamera(Camera* camera, Input* input, EngineTime* engineTime, GLFWwindow* window)
{
	myCamera = camera;
	myInput = input;
	myEngineTime = engineTime;
	myWindow = window;


}

void FlyingCamera::Update() //maybe this entire update should be run only if the right mouse button is down. 
{
	//if (myInput->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
	//{
		if (Engine::Get().mouseWasWithinSceneWindow == true)
		{
			MoveCamera();
			RotateCamera(false);
		}
	/*}*/
	myCamera->CameraUpdate();
}

void FlyingCamera::SetNewCursorPosition()
{
	ImGuiIO& io = ImGui::GetIO();
	//lastX = io.MousePos.x;
	//lastY = io.MousePos.y;



	//double* glfwPosX = new double;
	//double* glfwPosY = new double;
	////glfwGetWindowPos(Graphics::Get().window, windowPosX, windowPosY);
	//glfwGetCursorPos(Graphics::Get().window, glfwPosX, glfwPosY);

	////imgui: (0,0)
	////glfw:(-1, -31);
	//lastX = *glfwPosX + io.MousePos.x;
	//lastY = *glfwPosY + io.MousePos.y;
	glfwSetCursorPos(Graphics::Get().window, lastX, lastY);
}

void FlyingCamera::MoveCamera()
{
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//move camera
	if (myInput->IsKeyDown(GLFW_KEY_W)) velocity.z = 1;
	if (myInput->IsKeyDown(GLFW_KEY_A)) velocity.x = -1; //super low numbers overriding up down forward backward
	if (myInput->IsKeyDown(GLFW_KEY_S)) velocity.z = -1;
	if (myInput->IsKeyDown(GLFW_KEY_D)) velocity.x = 1; //super low numbers overriding up down forward backward

	if (myInput->IsKeyDown(GLFW_KEY_E)) velocity.y = 1;
	if (myInput->IsKeyDown(GLFW_KEY_Q)) velocity.y = -1;

	myCamera->CameraMove(velocity * MoveSpeed * (float)myInput->GetScrollValue() * myEngineTime->DeltaTime());
}

void FlyingCamera::RotateCamera(bool firstTimeRun)
{
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	//if holding right click
	//youre able to rotate the camera, mouse cursor disappears. CHECK
	//of you release right click, mouse cursor will reappear where you started holding down right click. CHECK
	//rotation of camera should be set relative to where you start holding right click, not be immediately set to where the mouse cursor is. CHECK


	//rotate camera
	double* myX = new double; // what the hell am I doing here? Why am I doing [new] double?
	double* myY = new double;

	float xpos = 0;
	float ypos = 0; //new variables

	if (!firstTimeRun) //special condition for straightening the camera at start.
	{
		/*ImGuiIO& io = ImGui::GetIO();*/
		//xpos = io.MousePos.x;
		//ypos = io.MousePos.y;
		//glfwGetCursorPos(myWindow, myX, myY);
		glfwGetCursorPos(Graphics::Get().window, myX, myY);
		xpos = *myX /*+ io.MousePos.x*/;
		ypos = *myY /*+ io.MousePos.y*/;
	}

	delete myX;
	delete myY;

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f) pitch = 89.0f; //I guess these may be some kind of safeguard, will try putting it to 90 later.
	if (pitch < -89.0f) pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	myCamera->SetDirection(direction);
}

void FlyingCamera::MyScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	std::cout << "test" << std::endl;
}
