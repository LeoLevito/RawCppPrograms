#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window, Graphics* graphics);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui();
	void CloseImGui();

	void FrameRateWindow(float deltaTime);
	void HierarchyWindow();
private:
	Graphics* myGraphics;
};

