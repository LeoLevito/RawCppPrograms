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
	void RenderImGui();
	void CloseImGui();

	void SceneWindow();
	void FrameRateWindow(float deltaTime);
	void HierarchyWindow();
	void InspectorWindow();
	void CameraWindow();
	void MainMenuBar();
	void QuickGUITesting();
	bool gammaCorrection;

	//testing if writing whole game objects will work.
	void Serialization(const std::string& filename);
	void Deserialization(const std::string& filename);

	float sceneWindowWidth;
	float sceneWindowHeight;

	int currentlySelectedGameObject;
private:
	Graphics* myGraphics;
	//Camera myCamera;
	float currentTime; //May or may not be super accurate at the moment.
	float deltaTime;
	float lastTime;


};

