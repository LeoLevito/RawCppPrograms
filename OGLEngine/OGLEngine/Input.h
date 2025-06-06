#pragma once
#include <GLFW/glfw3.h>
class Input
{
public:
	Input(GLFWwindow* window);
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MouseCallBack(GLFWwindow* window, double xPos, double yPos);
	static void ScrollCallBack(GLFWwindow* window, double xOffset, double yOffset);
	bool IsKeyPressed(const int& key);
	bool IsKeyDown(const int& key);
	bool IsMouseButtonDown(const int& button);
	int GetScrollValue();

private:
	GLFWwindow* myWindow;
};

