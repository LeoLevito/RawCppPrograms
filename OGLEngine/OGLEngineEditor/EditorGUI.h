#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "Game.h"
#include "Camera.h"
#include "glm.hpp"

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window, Graphics* graphics, Game* game, Camera& camera, Shader& shader);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui(glm::mat4& projection);
	void CloseImGui();

	void FrameRateWindow(float deltaTime);
	void HierarchyWindow(Camera& camera, glm::mat4& projection, Shader& shader);
private:
	Graphics* myGraphics;
	Game* myGame;
	Camera myCamera;
	glm::mat4 myProjection;
	Shader myShader;
};

