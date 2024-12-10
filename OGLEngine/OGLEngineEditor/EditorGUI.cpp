#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <GameObjectTest.h>
#include <iostream>
#include <gtc/matrix_transform.hpp>

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
	//myCamera = camera;


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
	ImGui::ShowDemoWindow(); // Show demo window! :)
	FrameRateWindow(deltaTime);
	HierarchyWindow(Camera::Get(), Camera::Get().projection, *myGraphics->myShader);
	CameraWindow();

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
		myGameObjectManager->CreateGameObject(); //create an empty game object.
	}

	int objectIndex = 0;
	for (GameObject* var : myGameObjectManager->gameObjects) //for every game object
	{
		int vectorSize = myGameObjectManager->gameObjects.size();

		ImGui::PushID(objectIndex); //ID system is required for items in an ImGui windows that will be named the same.

		if (ImGui::CollapsingHeader(myGameObjectManager->gameObjects.at(objectIndex)->name.c_str()))
		{
			int componentIndex = 0;
			for (Component* var : myGameObjectManager->gameObjects.at(objectIndex)->components) //for every component on the current index game object
			{
				int componentsSize = myGameObjectManager->gameObjects.at(objectIndex)->components.size();
				if (ImGui::TreeNode(myGameObjectManager->gameObjects.at(objectIndex)->components.at(componentIndex)->name.c_str())) //treenode shall be named after the components attached to the specified game object!
				{
					myGameObjectManager->gameObjects.at(objectIndex)->components.at(componentIndex)->DrawComponentSpecificImGuiHierarchyAdjustables();
					ImGui::TreePop();
				}

				if (myGameObjectManager->gameObjects.at(objectIndex)->components.size() == componentsSize)//check if vector has changed size, e.g. if we've deleted a game object. If the size hasn't changed we increase the iterator as per usual.
				{
					componentIndex++;
				}
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

void EditorGUI::CameraWindow()
{
	ImGui::Begin("Camera settings");

	if (ImGui::SliderFloat("FOV slider", &Camera::Get().FOV, 5, 120))
	{
		Camera::Get().UpdateCameraProjection();
	}

	if (ImGui::SliderFloat("Near clip", &Camera::Get().nearClipLane, 0.001f, 10.f))
	{
		Camera::Get().UpdateCameraProjection();
	}

	if (ImGui::SliderFloat("Far clip", &Camera::Get().farClipLane, 11.f, 1000.f))
	{
		Camera::Get().UpdateCameraProjection();
	}

	if (ImGui::Checkbox("orthographic", &Camera::Get().isOrthographic)) 
	{
		Camera::Get().UpdateCameraProjection();
	}
	//std::cout << Camera::Get().myPosition << std::endl;

	ImGui::Text("Camera position: %f %f %f", Camera::Get().myPosition.x, Camera::Get().myPosition.y, Camera::Get().myPosition.z);
	ImGui::Text("Camera direction: %f %f %f", Camera::Get().myDirection.x, Camera::Get().myDirection.y, Camera::Get().myDirection.z);
	ImGui::Text("Camera up: %f %f %f", Camera::Get().myUp.x, Camera::Get().myUp.y, Camera::Get().myUp.z);
	ImGui::Text("Camera right: %f %f %f", Camera::Get().myRight.x, Camera::Get().myRight.y, Camera::Get().myRight.z);
	//ImGui::Text("Camera speed: %f", &Camera::Get().MoveSpeed); //need to call flying camera for this.
	
	// //add new camera (not implemented as of yet, need to make a camera manager).
	//if (ImGui::Button("Add new Camera", &CameraManager::Get().AddNewCamera()))
	//{
	//	Camera::Get().UpdateCameraProjection();
	//}

	ImGui::End();
}
