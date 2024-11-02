#pragma once
#include <glfw3.h>

class Graphics
{
private:
	void Initialize(int width, int height);
	void Render();
	bool ShouldClose();
	GLFWwindow* window;
};

