#pragma once
#include "Graphics.h"
#include <GLFW/glfw3.h>
#include "GameObjectManager.h"
#include "Camera.h"
#include "glm.hpp"
#include "TransformComponent.h"

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
	void TransformStart(glm::mat4* cameraView, glm::mat4* cameraProjection, glm::mat4* objectMatrix);
	void EditTransform(glm::mat4* cameraView, glm::mat4* cameraProjection, TransformComponent& tComp);
	void TransformEnd();
	bool gammaCorrection;

	//testing if writing whole game objects will work.
	void Serialization(const std::string& filename);
	void Deserialization();

	float sceneWindowWidth = 0.f;
	float sceneWindowHeight = 0.f;
	float sceneWindowPosX = 0.f;
	float sceneWindowPosY = 0.f;


	int currentlySelectedGameObject = -1;
	int lastSelectedGameObject = -1;

	glm::mat4 trans;

	int currentImGizmoOperation;
	int currentImGizmoMode;

	int currentPolygonMode;

	bool manipulatingImGuizmo;
	bool invalidSceneName = false;
	bool identicalSceneName = false;

	bool DoOnce = false;

	std::string _startupSceneFilePath = "../Levels/_startupScene.startup";
private:
	Graphics* myGraphics;
	//Camera myCamera;
	float currentTime; //May or may not be super accurate at the moment.
	float deltaTime;
	float lastTime;


};

