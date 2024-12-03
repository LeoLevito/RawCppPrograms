#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "GameObjectManager.h"
#include "Camera.h"
#include "glm.hpp"

class EditorGUI
{
public:
	void Initialize(GLFWwindow* window, Graphics* graphics, GameObjectManager* gameObjectManager, Camera& camera, Shader& shader);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui(glm::mat4& projection);
	void CloseImGui();

	void FrameRateWindow(float deltaTime);
	void HierarchyWindow(Camera& camera, glm::mat4& projection, Shader& shader);
private:
	Graphics* myGraphics;
	GameObjectManager* myGameObjectManager;
	//Camera myCamera;
	glm::mat4 myProjection;
	float currentTime; //May or may not be super accurate at the moment.
	float deltaTime;
	float lastTime;
};

