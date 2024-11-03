#pragma once
#include <glfw3.h>
#include "Shader.h"

class Graphics
{
private:
	void Initialize(int width, int height);
	void Render();
	bool ShouldClose();
	GLFWwindow* window;
	Shader* myShader;
};

