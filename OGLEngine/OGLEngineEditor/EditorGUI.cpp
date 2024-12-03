#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <GameObjectTest.h>
#include <iostream>

void EditorGUI::Initialize(GLFWwindow* window, Graphics* graphics, GameObjectManager* gameObjectManager, Camera& camera, Shader& shader)
{
	//Initialize ImGui, check https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-glfw--openglwebgl, and cross-reference with Martin's project as well.
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();

	myGraphics = graphics;
	myGameObjectManager = gameObjectManager;
	myCamera = camera;


	currentTime = 0; //May or may not be super accurate at the moment.
	deltaTime = 0;
	lastTime = 0;
}

void EditorGUI::StartImGuiFrame(float deltaTime)
{
	// (Your code calls glfwPollEvents())
	// ...
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();




	//CHANGE THIS TO WHATEVER I WANT / NEED!
	//ImGui::ShowDemoWindow(); // Show demo window! :)
	FrameRateWindow(deltaTime);
	//HierarchyWindow(myCamera, myProjection, myShader);
}

void EditorGUI::RenderImGui(glm::mat4& projection)
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	myProjection = projection;
	// (Your code calls glfwSwapBuffers() etc.)
}

void EditorGUI::CloseImGui() //When shutting down program, make sure ImGui is shut down properly as well. Could probably make this a destructor instead of a void function for it to automatically be called once the program closes!
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void EditorGUI::FrameRateWindow(float deltaTime)
{
	float fps = 1.f / deltaTime;

	ImGui::Begin("Current frame rate:"); //start rendering new ImGui window

	ImGui::Text("FPS = %f", fps);
	ImGui::Text("frame time = %f", deltaTime);

	ImGui::End(); //stop rendering new ImGui window
}

void EditorGUI::HierarchyWindow(Camera& camera, glm::mat4& projection, Shader& shader)
{
	ImGui::Begin("Hierarchy"); //start rendering new ImGui window


	if (ImGui::Button("Add Game Object")) //replace code here with GameObjectManager AddGameObject() or similar.
	{
		GameObjectTest* test = new GameObjectTest;
		myGameObjectManager->gameObjects.push_back(test);
		test->index = myGameObjectManager->gameObjects.size() - 1;
		test->myCamera = &camera;
		test->myProjection = &projection;
		test->LateSetComponentVariables();
	}

	int objectIndex = 0;
	for (GameObject* var : myGameObjectManager->gameObjects) //for every game object
	{
		int vectorSize = myGameObjectManager->gameObjects.size();

		ImGui::PushID(objectIndex); //ID system is required for items in an ImGui windows that will be named the same.

		if (ImGui::CollapsingHeader(myGameObjectManager->gameObjects.at(objectIndex)->name.c_str()))
		{
			int componentIndex = 0;
			for (Component* var : myGameObjectManager->gameObjects.at(objectIndex)->componentVector) //for every component on the current index game object
			{
				if (ImGui::TreeNode(myGameObjectManager->gameObjects.at(objectIndex)->componentVector.at(componentIndex)->name.c_str())) //treenode shall be named after the components attached to the specified game object!
				{
					myGameObjectManager->gameObjects.at(objectIndex)->componentVector.at(componentIndex)->DrawComponentSpecificImGuiHierarchyAdjustables(camera, projection);
					ImGui::TreePop();
				}
				componentIndex++;
			}
			myGameObjectManager->gameObjects.at(objectIndex)->DrawObjectSpecificImGuiHierarchyAdjustables(myGameObjectManager->gameObjects); //allows deletion of game object without code above complaining about component being missing. I should probably stream line this a little bit.
		}

		if (myGameObjectManager->gameObjects.size() == vectorSize)//check if vector has changed size, e.g. if we've deleted a game object. If the size hasn't changed we increase the iterator as per usual.
		{
			objectIndex++;
		}
		ImGui::PopID();
	}
	ImGui::End(); //stop rendering new ImGui window
}
