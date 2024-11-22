#include "EditorGUI.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <GameObjectTest.h>

void EditorGUI::Initialize(GLFWwindow* window, Graphics* graphics, Game* game)
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
	myGame = game;
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
	HierarchyWindow();
}

void EditorGUI::RenderImGui()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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

void EditorGUI::HierarchyWindow()
{
	ImGui::Begin("Hierarchy"); //start rendering new ImGui window


	if (ImGui::Button("Add Game Object"))
	{
		GameObjectTest* test = new GameObjectTest;
		myGame->gameObjectVector.push_back(test);
	}

	int objectIndex = 0;
	for (auto var : myGame->gameObjectVector) //for every game object
	{
		ImGui::PushID(objectIndex); //ID system is required for items in an ImGui windows that will be named the same.

		if (ImGui::CollapsingHeader(myGame->gameObjectVector[objectIndex]->name.c_str()))
		{
			int componentIndex = 0;
			for (auto var : myGame->gameObjectVector[objectIndex]->componentVector)//for every component on the current index game object
			{
				if (ImGui::TreeNode(myGame->gameObjectVector[objectIndex]->componentVector[componentIndex]->name.c_str())) //treenode shall be named after the components attached to the specified game object!
				{
					//ImGui::InputFloat3("transform", &myGraphics->myCubePositions[objectIndex].x); //wait, why does this just work? How does Martin do it?
					//ImGui::DragFloat3("Position", &myGraphics->myCubePositions[objectIndex].x, .01f); //wait, why does this just work? EDIT: looks like pos.x is the way to do it properly: https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#qa-usage
					//std::string str5 = myGame->gameObjectVector[objectIndex]->componentVector[0]->name;

					ImGui::Text("You can see the name of this component above this text!");
					myGame->gameObjectVector[objectIndex]->componentVector[componentIndex]->DrawComponentSpecificImGuiHierarchyAdjustables();
					//maybe the component itself should authorize what to draw in this tree node and we just call its DrawImGui() function here?
					ImGui::TreePop();
				}
				componentIndex++;
			}
		}

		objectIndex++;
		ImGui::PopID();
	}

	//for (auto var : myGraphics->myCubePositions) //for each loop.
	//{
	//	ImGui::PushID(objectIndex); //ID system is required for items in an ImGui windows that will be named the same.
	//	std::string str1 = "myCubePositions"; //should be able to rename objects in editor in the future. Maybe have a name variable in the GameObject class.
	//	std::string str2 = std::to_string(objectIndex);
	//	str1.append(str2);

	//	if (ImGui::CollapsingHeader(str1.c_str()))
	//	{
	//		if (ImGui::TreeNode("Psuedo Transform Component")) //treenode shall be named after the components attached to the specified game object!
	//		{
	//			//ImGui::InputFloat3("transform", &myGraphics->myCubePositions[objectIndex].x); //wait, why does this just work? How does Martin do it?
	//			ImGui::DragFloat3("Position", &myGraphics->myCubePositions[objectIndex].x, .01f); //wait, why does this just work? EDIT: looks like pos.x is the way to do it properly: https://github.com/ocornut/imgui/blob/master/docs/FAQ.md#qa-usage
	//			ImGui::TreePop();
	//		}
	//	}

	//	objectIndex++;
	//	ImGui::PopID();
	//}
	ImGui::End(); //stop rendering new ImGui window
}
