#pragma once
#include <GLFW/glfw3.h>

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window);
	void StartImGuiFrame();
	void RenderImGui();
	void CloseImGui();
private:
};

