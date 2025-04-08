#include "Input.h"
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>
std::unordered_map<int, bool> myKeyStates; //this will get bigger and bigger as we press more (different) keys, as I understood it.

float lastX;
float lastY;
int scrollIterator = 1;

Input::Input(GLFWwindow* window)
{
	myWindow = window;

	GLFWvidmode return_struct;

	int width, height = 0;

	glfwGetWindowSize(window, &width, &height);

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	glfwSetKeyCallback(window, KeyCallBack);
	glfwSetScrollCallback(window, ScrollCallBack);

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

void Input::ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset)
{
	scrollIterator += yOffset; 
	if (scrollIterator < 1)
	{
		scrollIterator = 1; //I should make this psuedo exponential when at camera MoveSpeeds lower than 1.
	}
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

int Input::GetScrollValue()
{
	return scrollIterator;
}
