#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "GameObjectManager.h"
#include "Camera.h"
#include "glm.hpp"

class EditorGUI
{
private:
	EditorGUI();
	~EditorGUI();
public:
	static EditorGUI& Get();

	void Initialize(GLFWwindow* window, Graphics* graphics, Camera& camera);
	void StartImGuiFrame(float deltaTime);
	void RenderImGui(glm::mat4& projection);
	void CloseImGui();

	void SceneWindow();
	void FrameRateWindow(float deltaTime);
	void HierarchyWindow(Camera& camera, glm::mat4& projection);
	void CameraWindow();
	void MainMenuBar();
	void QuickGUITesting();
	bool gammaCorrection;

	//testing if writing whole game objects will work.
	void Serialization(const std::string& filename);
	void Deserialization(const std::string& filename);

	float sceneWindowWidth;
	float sceneWindowHeight;
private:
	Graphics* myGraphics;
	//Camera myCamera;
	glm::mat4 myProjection;
	float currentTime; //May or may not be super accurate at the moment.
	float deltaTime;
	float lastTime;


};

