#include "Input.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>
std::unordered_map<int, bool> myKeyStates; //this will get bigger and bigger as we press more (different) keys, as I understood it.

float lastX;
float lastY;

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
	std::cout << " MOVED MOUSE !!! " << std::endl;
	std::cout << lastX << " - XPOS || " << lastY << " - YPOS" << std::endl;
}

float Input::GetCursorX()
{
	return lastX;
}

float Input::GetCursorY()
{
	return lastY;
}

void Input::SetCursorX(float x)
{
	lastX = x;
}

void Input::SetCursorY(float y)
{
	lastY = y;
}

void Input::SetCursorXY(float x, float y)
{
	glfwSetCursorPos(myWindow,x, y);
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

bool Input::IsMouseButtonDown(const int& button)
{
	return glfwGetMouseButton(myWindow, button) == GLFW_PRESS;
}
