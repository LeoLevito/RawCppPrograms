#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "Game.h"

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window, Graphics* graphics, Game* game);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui();
	void CloseImGui();

	void FrameRateWindow(float deltaTime);
	void HierarchyWindow();
private:
	Graphics* myGraphics;
	Game* myGame;
};

