#include "Input.h"
#include <glfw3.h>
#include <unordered_map>
std::unordered_map<int, bool> myKeyStates; //this will get bigger and bigger as we press more (different) keys, as I understood it.
float lastX;
float lastY; //like, why does putting these here fix the issue "a nonstatic member reference must be relative to a specific object"?

Input::Input(GLFWwindow* window)
{
	myWindow = window;

	GLFWvidmode return_struct;

	int width, height = 0;

	glfwGetWindowSize(window, &width, &height);

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetCursorPosCallback(window, MouseCallBack);
}

void Input::KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		myKeyStates[key] = true;
	}
	else if (action == GLFW_RELEASE)
	{
		myKeyStates[key] = false;
	}
}

void Input::MouseCallBack(GLFWwindow* window, double xPos, double yPos)
{
	lastX = xPos;
	lastY = yPos;
}

float Input::GetCursorX()
{
	return lastX;
}

float Input::GetCursorY()
{
	return lastY;
}

bool Input::IsKeyPressed(const int& key)
{
	if (myKeyStates[key])
	{
		myKeyStates[key] = false;
		return true;
	}
	return false;
}

bool Input::IsKeyDown(const int& key)
{
	return glfwGetKey(myWindow, key) == GLFW_PRESS;
}
