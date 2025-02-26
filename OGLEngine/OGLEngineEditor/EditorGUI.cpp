#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <GameObjectTest.h>
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include "ObjectMessage.h"
#include "Engine.h"
#include "MeshManager.h"
#include "CollisionManager.h"

void EditorGUI::Initialize(GLFWwindow* window, Graphics* graphics, Camera& camera)
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


	ImGui::ShowDemoWindow(); // Show demo window! :)
	FrameRateWindow(deltaTime);
	HierarchyWindow(Camera::Get(), Camera::Get().projection);
	CameraWindow();
	MainMenuBar();
	QuickGUITesting();
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

void EditorGUI::HierarchyWindow(Camera& camera, glm::mat4& projection)
{
	ImGui::Begin("Hierarchy"); //start rendering new ImGui window

	if (ImGui::Button("Add Game Object")) 
	{
		ObjectMessage* newMessage = new ObjectMessage(ObjectMessageType::Create);
		GameObjectManager::Get().QueueMessage(newMessage); //send a message to create an empty game object.
	}

	if (ImGui::Button("Save Game Objects"))
	{
		GameObjectManager::Get().Serialization("../Levels/LevelSaveTest2.scene");
	}

	if (ImGui::Button("Load Game Objects"))
	{
		GameObjectManager::Get().Deserialization("../Levels/LevelSaveTest2.scene");
	}

	int objectIndex = 0;
	for (GameObject* var : GameObjectManager::Get().gameObjects) //for every game object
	{
		int vectorSize = GameObjectManager::Get().gameObjects.size();

		ImGui::PushID(objectIndex); //ID system is required for items in an ImGui windows that will be named the same.

		if (ImGui::CollapsingHeader(GameObjectManager::Get().gameObjects.at(objectIndex)->name.c_str()))
		{
			int componentIndex = 0;
			for (Component* var : GameObjectManager::Get().gameObjects.at(objectIndex)->components) //for every component on the current index game object
			{
				int componentsSize = GameObjectManager::Get().gameObjects.at(objectIndex)->components.size();
				if (ImGui::TreeNode(GameObjectManager::Get().gameObjects.at(objectIndex)->components.at(componentIndex)->name.c_str())) //treenode shall be named after the components attached to the specified game object!
				{
					GameObjectManager::Get().gameObjects.at(objectIndex)->components.at(componentIndex)->DrawComponentSpecificImGuiHierarchyAdjustables();
					ImGui::TreePop();
				}

				if (GameObjectManager::Get().gameObjects.at(objectIndex)->components.size() == componentsSize)//check if vector has changed size, e.g. if we've deleted a component. If the size hasn't changed we increase the iterator as per usual.
				{
					componentIndex++;
				}
			}
			GameObjectManager::Get().gameObjects.at(objectIndex)->DrawObjectSpecificImGuiHierarchyAdjustables(GameObjectManager::Get().gameObjects); //allows deletion of game object without code above complaining about component being missing. I should probably stream line this a little bit.
		}

		if (GameObjectManager::Get().gameObjects.size() == vectorSize)//check if vector has changed size, e.g. if we've deleted a game object. If the size hasn't changed we increase the iterator as per usual.
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
	ImGui::Text("Camera speed: %i", Engine::Get().myFlyingCamera->myInput->GetScrollValue());

	// //add new camera (not implemented as of yet, need to make a camera manager).
	//if (ImGui::Button("Add new Camera", &CameraManager::Get().AddNewCamera()))
	//{
	//	Camera::Get().UpdateCameraProjection();
	//}

	ImGui::End();
}

void EditorGUI::MainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}
			if (ImGui::MenuItem("Open"))
			{

			}
			if (ImGui::MenuItem("Save", "Ctrl+S"))
			{
				Serialization("../Levels/LevelSaveTest.test");
			}
			if (ImGui::MenuItem("Save As..."))
			{

			}
			if (ImGui::MenuItem("Load"))
			{
				Deserialization("../Levels/LevelSaveTest.test");
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			{

			}
			if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z", false,false)) //disabled item, need to update false to true whenever I get this functionality in and can redo stuff.
			{

			}
			if (ImGui::MenuItem("Cut", "Ctrl+X"))
			{

			}
			if (ImGui::MenuItem("Copy", "Ctrl+C"))
			{

			}
			if (ImGui::MenuItem("Paste", "Ctrl+V"))
			{

			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

}

void EditorGUI::QuickGUITesting()
{
	ImGui::Begin("Quick GUI testing");

	if (ImGui::Checkbox("Gamma Correction?", &gammaCorrection))
	{
		if (gammaCorrection)
		{
			glEnable(GL_FRAMEBUFFER_SRGB); //enable gamma correction.
		}
		else 
		{
			glDisable(GL_FRAMEBUFFER_SRGB); //disable gamma correction.
		}
	}

	MeshManager::Get().ErrorGUI();
	CollisionManager::Get().DebugGUI();

	ImGui::End();
}

void EditorGUI::Serialization(const std::string& filename) //not functional at the moment.
{
	//I could try to write the entire GameObjectManager, if it's possible to even do something like that. Like, getting everything in it and writing/reading all of the data to a binary file.
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	static const size_t DATA_CAPACITY = 0x1000;

	if (file.is_open())
	{
		//get size of vectors to integers.
		int a = GameObjectManager::Get().gameObjects.size();

		//write sizes of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&a), sizeof(a));
		//write contents of vectors, https://stackoverflow.com/a/31213593
		file.write(reinterpret_cast<char*>(&GameObjectManager::Get().gameObjects[0]), sizeof(GameObject) * GameObjectManager::Get().gameObjects.size());

		for (size_t i = 0; i < a; i++)
		{
			int b = GameObjectManager::Get().gameObjects[i]->components.size();
			if (b > 0)
			{
				file.write(reinterpret_cast<char*>(&b), sizeof(b));
				file.write(reinterpret_cast<char*>(&GameObjectManager::Get().gameObjects[i]->components[0]), sizeof(Component) * GameObjectManager::Get().gameObjects[i]->components.size());
			}
		}
	}
	file.close();
}

void EditorGUI::Deserialization(const std::string& filename) //not functional at the moment.
{
	std::fstream file;
	file.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	if (file.is_open())
	{
		int a;

		//read sizes of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&a), sizeof(a)); //I see the issue now, we don't know the size of the vector when we want to read it, and since we input the size of an empty vector right now, it's gonna return nothing as well. so maybe we need to write the size to the file for us to retrieve when reading it.
		GameObjectManager::Get().gameObjects.clear();
		GameObjectManager::Get().gameObjects.resize(a);

		//read contents of vectors, https://stackoverflow.com/a/31213593
		file.read(reinterpret_cast<char*>(&GameObjectManager::Get().gameObjects[0]), sizeof(GameObject) * GameObjectManager::Get().gameObjects.size());
		
		for (size_t i = 0; i < a; i++)
		{
			int b;
			file.read(reinterpret_cast<char*>(&b), sizeof(b));
			if (b > 0)
			{
				GameObjectManager::Get().gameObjects[i]->components.clear();
				GameObjectManager::Get().gameObjects[i]->components.resize(b);
				file.write(reinterpret_cast<char*>(&GameObjectManager::Get().gameObjects[i]->components[0]), sizeof(Component) * GameObjectManager::Get().gameObjects[i]->components.size());
			}
		}
	}
	file.close();
}
