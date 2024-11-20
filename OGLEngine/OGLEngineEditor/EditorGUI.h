#pragma once
#include <GLFW/glfw3.h>

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui();
	void CloseImGui();

	void FrameRateWindow(float deltaTime);
private:
};

