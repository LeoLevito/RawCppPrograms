#include "EditorGUI.h"
#include <string>
#include <GameObjectTest.h>
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtx/matrix_decompose.hpp>
#include <gtc/type_ptr.hpp>
#include "ObjectMessage.h"
#include "LevelMessage.h"
#include "Engine.h"
#include "MeshManager.h"
#include "CollisionManager.h"
#include "LightManager.h"
#include "TextureManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"
#include <imgui_internal.h>

EditorGUI::EditorGUI()
{
	currentImGizmoOperation = ImGuizmo::TRANSLATE;
	currentImGizmoMode = ImGuizmo::WORLD;
	currentPolygonMode = GL_FILL;
}

EditorGUI::~EditorGUI()
{
}

EditorGUI& EditorGUI::Get()
{
	// TODO: insert return statement here
	static EditorGUI instance;
	return instance;
}

void EditorGUI::Initialize(GLFWwindow* window, Graphics* graphics, Camera& camera)
{
	//Initialize ImGui, check https://github.com/ocornut/imgui/wiki/Getting-Started#example-if-you-are-using-glfw--openglwebgl, and cross-reference with Martin's project as well.
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		  // Enable ImGui windows outside of the main window
	io.ConfigWindowsMoveFromTitleBarOnly = true;
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
	ImGui::DockSpaceOverViewport(); //https://github.com/ocornut/imgui/issues/5086

	ImGuizmo::SetOrthographic(Camera::Get().isOrthographic);
	ImGuizmo::BeginFrame();

	SceneWindow();
	ImGui::ShowDemoWindow(); // Show demo window! :)
	FrameRateWindow(deltaTime);
	HierarchyWindow();
	InspectorWindow();
	CameraWindow();
	MainMenuBar();
	QuickGUITesting();
}

void EditorGUI::RenderImGui()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls glfwSwapBuffers() etc.)

	ImGuiIO& io = ImGui::GetIO();
	//https://github.com/ocornut/imgui/wiki/Multi-Viewports
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) //one issue with floating imgui windows is that they don't map to the GLFW input context or whatever, so if I want to rotate the camera when the SceneWindow is floating, it won't work. In the future I may swap that part of the Input class with ImGui's input implementation. Or maybe I want to create a new GLFW window for every floating window, like how Unity does it. That would allow for docking the windows to MS Windows's area.
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context); //https://github.com/ocornut/imgui/blob/0c079e453bf74acebc6c8b4f64c4fa40e76af760/examples/example_glfw_opengl3/main.cpp#L213
	}

	if (DoOnce == false)
	{
		DoOnce = true;
		Deserialization();
	}
}

void EditorGUI::CloseImGui() //When shutting down program, make sure ImGui is shut down properly as well. Could probably make this a destructor instead of a void function for it to automatically be called once the program closes!
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

//https://gamedev.stackexchange.com/questions/140693/how-can-i-render-an-opengl-scene-into-an-imgui-window
void EditorGUI::SceneWindow()
{
	//Camera view can become stretched for a fraction of a second when resizing this window (not resizing main GLFW window), presumably because there's a delay from when sceneWindowWidth and sceneWindowHeight are set below and when they're used to render the framebuffer image.

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0)); //https://github.com/ocornut/imgui/issues/1490 gets rid of window padding so we can display the framebuffer image on the ENTIRE window.
	ImGui::Begin("Scene");

	//if (!(ImGui::IsWindowDocked()))
	//{
	//	std::cout << "floating" << std::endl;
	//	GLFWwindow* myWindow;
	//	myWindow = glfwCreateWindow(500, 500, "SceneWindow", NULL, Graphics::Get().window); //create window.
	//	//glfwMakeContextCurrent(myWindow); //make window current context of OpenGL calling thread.
	//}

	{
		if (ImGui::RadioButton("Translate", currentImGizmoOperation == ImGuizmo::TRANSLATE))
		{
			currentImGizmoOperation = ImGuizmo::TRANSLATE;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", currentImGizmoOperation == ImGuizmo::ROTATE))
		{
			currentImGizmoOperation = ImGuizmo::ROTATE;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", currentImGizmoOperation == ImGuizmo::SCALE))
		{
			currentImGizmoOperation = ImGuizmo::SCALE;
		}

		ImGui::SameLine();
		ImGui::Text(" | ");
		ImGui::SameLine();

		if (ImGui::RadioButton("World", currentImGizmoMode == ImGuizmo::WORLD))
		{
			currentImGizmoMode = ImGuizmo::WORLD;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Local", currentImGizmoMode == ImGuizmo::LOCAL))
		{
			currentImGizmoMode = ImGuizmo::LOCAL;
		}

		ImGui::SameLine();
		ImGui::Text(" | ");
		ImGui::SameLine();

		if (ImGui::RadioButton("Regular", currentPolygonMode == GL_FILL))
		{
			currentPolygonMode = GL_FILL;
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Wireframe", currentPolygonMode == GL_LINE))
		{
			currentPolygonMode = GL_LINE;
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Points", currentPolygonMode == GL_POINT))
		{
			currentPolygonMode = GL_POINT;
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
	}

	ImGui::BeginChild("Scene2"); //child allows for larger area of window to be used.

	ImGuizmo::SetDrawlist();


	if (ImGui::GetWindowSize().x != sceneWindowWidth || ImGui::GetWindowSize().y != sceneWindowHeight)
	{
		sceneWindowWidth = ImGui::GetWindowSize().x;
		sceneWindowHeight = ImGui::GetWindowSize().y;

		Camera::Get().UpdateCameraProjection();
	}

	if (ImGui::GetWindowPos().x != sceneWindowPosX || ImGui::GetWindowPos().y != sceneWindowPosY)
	{
		sceneWindowPosX = ImGui::GetWindowPos().x;
		sceneWindowPosY = ImGui::GetWindowPos().y;
	}


	ImTextureID texid = Graphics::Get().sceneTexture;
	ImVec2 texsize = { sceneWindowWidth, sceneWindowHeight };
	ImGui::Image(texid, texsize, ImVec2(0, 1), ImVec2(1, 0)); //Display framebuffer image.

	if (Graphics::Get().allowOutline) //Display outline framebuffer image on top of sceneTexture.
	{
		ImTextureID texid2 = Graphics::Get().sceneTextureOutlineActual;
		ImVec2 texsize2 = { sceneWindowWidth, sceneWindowHeight };
		ImGui::GetWindowDrawList()->AddImage(texid2, ImVec2(sceneWindowPosX, sceneWindowPosY), ImVec2(sceneWindowPosX + sceneWindowWidth, sceneWindowPosY + sceneWindowHeight), ImVec2(0, 1), ImVec2(1, 0));
	}


	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, sceneWindowWidth, sceneWindowHeight);

	//this here is so stupid, Maybe I SHOULD make the TransformComponent an inherent trait to the GameObject class.	
	if (currentlySelectedGameObject >= 0)
	{
		int gameObjectsSize = GameObjectManager::Get().gameObjects.size() - 1; //instance where setting an int before the if statement works better than having the full .size getter inside the if statement.
		if (!(currentlySelectedGameObject > gameObjectsSize))
		{
			for (size_t i = 0; i < GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components.size(); i++)
			{
				if (dynamic_cast<TransformComponent*>(GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components[i])) //checking if owner has a component of type TransformComponent. Is of-type correct word-use in this case?
				{
					TransformComponent* tComp = dynamic_cast<TransformComponent*>(GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components[i]);
					EditTransform(&Camera::Get().myView, &Camera::Get().projection, *tComp);
				}
			}
		}
	}
	ImGui::EndChild();
	ImGui::End();
	ImGui::PopStyleVar();
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
		ObjectMessage* newMessage = new ObjectMessage(ObjectMessageType::Create);
		GameObjectManager::Get().QueueMessage(newMessage); //send a message to create an empty game object.
	}

	if (ImGui::Button("Save Game Objects"))
	{
		//LevelMessage* newMessage = new LevelMessage(LevelMessageType::Save);
		//GameObjectManager::Get().QueueMessage(newMessage); //send a message to create an empty game object.
		GameObjectManager::Get().Serialization("../Levels/LevelSaveTest2.scene");
	}

	if (ImGui::Button("Load Game Objects"))
	{
		//LevelMessage* newMessage = new LevelMessage(LevelMessageType::Load);
		//GameObjectManager::Get().QueueMessage(newMessage); //send a message to create an empty game object.
		GameObjectManager::Get().Deserialization("../Levels/LevelSaveTest2.scene");
	}

	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	static int selection_mask = (1 << 2);
	int node_clicked = -1;
	for (int i = 0; i < GameObjectManager::Get().gameObjects.size(); i++)
	{
		// Disable the default "open on single-click behavior" + set Selected flag according to our selection.
		// To alter selection we use IsItemClicked() && !IsItemToggledOpen(), so clicking on an arrow doesn't alter selection.
		ImGuiTreeNodeFlags node_flags = base_flags;
		const bool is_selected = (selection_mask & (1 << i)) != 0;
		if (is_selected)
		{
			node_flags |= ImGuiTreeNodeFlags_Selected;
		}
		bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, GameObjectManager::Get().gameObjects.at(i)->name.c_str());
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			node_clicked = i;
		}
		if (node_open)
		{
			//if i decide on having child objects, display them here.
			ImGui::TreePop();
		}
	}
	if (node_clicked != -1)
	{
		// Update selection state
		// (process outside of tree loop to avoid visual inconsistencies during the clicking frame)
		if (ImGui::GetIO().KeyCtrl)
		{
			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
		}
		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, may want to preserve selection when clicking on item that is part of the selection
		{
			selection_mask = (1 << node_clicked);           // Click to single-select
			currentlySelectedGameObject = node_clicked;
			lastSelectedGameObject = currentlySelectedGameObject;
		}
	}
	if (lastSelectedGameObject != currentlySelectedGameObject)
	{
		lastSelectedGameObject = currentlySelectedGameObject;
		selection_mask = (1 << currentlySelectedGameObject);
	}

	ImGui::End(); //stop rendering new ImGui window
}

void EditorGUI::InspectorWindow()
{
	ImGui::Begin("Inspector");
	if (!GameObjectManager::Get().gameObjects.size() > 0 || !(currentlySelectedGameObject >= 0))
	{
		ImGui::End();
		return;
	}
	if (currentlySelectedGameObject > GameObjectManager::Get().gameObjects.size() - 1)
	{
		ImGui::End();
		return;
	}

	int componentIndex = 0;
	for (Component* var : GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components) //for every component on the current index game object
	{
		int componentsSize = GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components.size();
		if (ImGui::TreeNode(GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components.at(componentIndex)->name.c_str())) //treenode shall be named after the components attached to the specified game object!
		{
			GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components.at(componentIndex)->DrawComponentSpecificImGuiHierarchyAdjustables();
			ImGui::TreePop();
		}

		if (GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->components.size() == componentsSize)//check if vector has changed size, e.g. if we've deleted a component. If the size hasn't changed we increase the iterator as per usual.
		{
			componentIndex++;
		}
	}
	GameObjectManager::Get().gameObjects.at(currentlySelectedGameObject)->DrawObjectSpecificImGuiHierarchyAdjustables(GameObjectManager::Get().gameObjects); //allows deletion of game object without code above complaining about component being missing. I should probably stream line this a little bit.

	ImGui::End();
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
	bool openScene = false;
	bool saveNewScene = false;
	bool startupScene = false;
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New"))
			{
				GameObjectManager::Get().DeleteAllGameObjects();
				GameObjectManager::Get().currentSceneName = "empty";

			}
			if (ImGui::MenuItem("Open"))
			{
				openScene = true;
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) //Ctrl+S doesn't do anything.
			{
				if (GameObjectManager::Get().currentSceneName != "empty") //if current scene has been loaded from file.
				{
					GameObjectManager::Get().Serialization(GameObjectManager::Get().currentSceneName.c_str());
				}
				else //if it hasn't been loaded from file, it's a new scene.
				{
					saveNewScene = true;
				}
			}
			if (ImGui::MenuItem("Save as...")) //manual 
			{
				saveNewScene = true;
			}
			if (ImGui::MenuItem("Specify startup scene")) //manual 
			{
				startupScene = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit")) //this menu doesn't do anything at the moment. Though in the future i would like to get Undo and Redo functionality.
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z"))
			{

			}
			if (ImGui::MenuItem("Redo", "Ctrl+Shift+Z", false, false)) //disabled item, need to update false to true whenever I get this functionality in and can redo stuff.
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

	if (openScene == true)
	{
		ImGui::OpenPopup("Scene selection popup");
	}

	if (saveNewScene == true)
	{
		ImGui::OpenPopup("New scene save popup");
	}

	if (startupScene == true)
	{
		ImGui::OpenPopup("Startup scene popup");
	}

	if (ImGui::BeginPopup("Scene selection popup"))
	{
		//Get all scene file names.
		std::vector<std::filesystem::directory_entry> sceneVector;

		std::string path = "../Levels/";
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
		{
			sceneVector.push_back(entry);
		}

		ImGui::SeparatorText("SCENES:");

		for (int i = 0; i < sceneVector.size(); i++)
		{
			std::string selectableSceneName = sceneVector[i].path().string().c_str();
			if (!(selectableSceneName == "../Levels/_startupScene.startup"))
			{
				if (ImGui::Selectable(selectableSceneName.c_str())) //Display new Selectable for scene files.
				{
					GameObjectManager::Get().Deserialization(selectableSceneName);
				}
			}
		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("New scene save popup"))
	{
		ImGui::SeparatorText("SAVE THIS SCENE AS:");

		static char str0[128]; //how it's done in the ImGui demo, tho it is replicated across all objects now...
		ImGui::InputText("Input scene name", str0, IM_ARRAYSIZE(str0));

		std::string inputSceneName = str0;
		if (ImGui::Button("Save"))
		{
			std::string newSceneName = "../Levels/";
			newSceneName.append(inputSceneName);
			newSceneName.append(".scene");

			identicalSceneName = false;
			invalidSceneName = false;

			std::vector<std::filesystem::directory_entry> sceneVector;
			std::string path = "../Levels/";
			for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
			{
				sceneVector.push_back(entry);
			}
			for (int i = 0; i < sceneVector.size(); i++)
			{
				if (newSceneName == sceneVector[i].path())
				{
					identicalSceneName = true;
				}
			}

			if (inputSceneName == "empty" || inputSceneName.size() == 0)
			{
				invalidSceneName = true;
			}
			else
			{
				invalidSceneName = false;
			}

			if (identicalSceneName == false && invalidSceneName == false)
			{
				GameObjectManager::Get().Serialization(newSceneName);
				ImGui::CloseCurrentPopup();
			}
		}
		if (invalidSceneName == true)
		{
			ImGui::SameLine();
			ImGui::Text("please write anything other than 'empty'.");
		}
		if (identicalSceneName == true)
		{
			ImGui::SameLine();
			ImGui::Text("That name already exists, overwrite?");
			if (ImGui::Button("Overwrite"))
			{
				std::string newSceneName = "../Levels/"; //copied from above.
				newSceneName.append(inputSceneName);
				newSceneName.append(".scene");

				GameObjectManager::Get().Serialization(newSceneName);
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Startup scene popup"))
	{
		//Get all scene file names.
		std::vector<std::filesystem::directory_entry> sceneVector;

		std::string path = "../Levels/";
		for (const std::filesystem::directory_entry& entry : std::filesystem::recursive_directory_iterator(path))
		{
			sceneVector.push_back(entry);
		}

		ImGui::SeparatorText("SELECT STARTUP SCENE:");

		for (int i = 0; i < sceneVector.size(); i++)
		{

			std::string selectableSceneName = sceneVector[i].path().string().c_str();
			if (!(selectableSceneName == "../Levels/_startupScene.startup"))
			{
				if (ImGui::Selectable(selectableSceneName.c_str())) //Display new Selectable for scene files.
				{
					Serialization(selectableSceneName);
				}
			}
		}
		ImGui::EndPopup();
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
	LightManager::Get().DrawImgui();
	Graphics::Get().DrawImgui();
	TextureManager::Get().DrawImgui();









	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("X: %f Y: %f", io.MousePos.x, io.MousePos.y);
	double* glfwPosX = new double;
	double* glfwPosY = new double;
	//glfwGetWindowPos(Graphics::Get().window, windowPosX, windowPosY);
	glfwGetCursorPos(Graphics::Get().window, glfwPosX, glfwPosY);
	ImGui::Text("X: %f Y: %f", (float)*glfwPosX, (float)*glfwPosY);
	delete glfwPosX;
	delete glfwPosY;

	if (ImGuizmo::IsUsing())
	{
		ImGui::Text("Using gizmo");
	}
	else
	{
		ImGui::Text(ImGuizmo::IsOver() ? "Over gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::ROTATE) ? "Over rotate gizmo" : "");
		ImGui::SameLine();
		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::SCALE) ? "Over scale gizmo" : "");
	}

	ImGui::End();
}

void EditorGUI::TransformStart(glm::mat4* cameraView, glm::mat4* cameraProjection, glm::mat4* objectMatrix)
{

}

void EditorGUI::EditTransform(glm::mat4* cameraView, glm::mat4* cameraProjection, TransformComponent& tComp)
{

	float tmpMatrix[16]; //https://github.com/CedricGuillemet/ImGuizmo/issues/125 came in clutch, always try solutions that my brain feels I understand first.
	ImGuizmo::RecomposeMatrixFromComponents(&tComp.position.x, &tComp.rotation.x, &tComp.scale.x, tmpMatrix);
	ImGuizmo::Manipulate(glm::value_ptr(*cameraView), glm::value_ptr(*cameraProjection), ImGuizmo::OPERATION(currentImGizmoOperation), ImGuizmo::MODE(currentImGizmoMode), tmpMatrix);

	if (ImGuizmo::IsUsing())
	{
		manipulatingImGuizmo = true;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(tmpMatrix, matrixTranslation, matrixRotation, matrixScale);

		switch (currentImGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			tComp.position = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
			break;
		case ImGuizmo::ROTATE:
			tComp.rotation = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]); //Note about rotation: gizmo behavior is as expected both for WORLD and LOCAL space compared with Unity, where two axes will affect every other axis when dragging the gizmo, while one axis will only affect itself.
			//and setting rotation manually on the TransformComponent in the inspector (in world and local space) results in same behavior as Unity does it, where one axis is world aligned (when in world space), 
			//while the other two are either locally aligned or something because of the underlying code. And two axes will affect every other axis when dragging the gizmo, while one axis will only affect itself.
			break;
		case ImGuizmo::SCALE:
			tComp.scale = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);
			break;
		default:
			break;
		}
	}
	else
	{
		manipulatingImGuizmo = false;
	}
}

void EditorGUI::TransformEnd()
{
}

void EditorGUI::Serialization(const std::string& sceneName) 
{
	std::fstream file;
	file.open(_startupSceneFilePath.c_str(), std::ios_base::out | std::ios_base::binary);

	if (file.is_open())
	{
		std::string write = sceneName;
		int nameSize = sceneName.size();

		file.write(reinterpret_cast<char*>(&nameSize), sizeof(int));

		file.write(reinterpret_cast<char*>(&write[0]), nameSize); //https://stackoverflow.com/a/37035925
	}
	file.close();
}

void EditorGUI::Deserialization() 
{
	std::fstream file;
	file.open(_startupSceneFilePath.c_str(), std::ios_base::in | std::ios_base::binary);

	if (file.is_open())
	{
		int nameSize;

		file.read(reinterpret_cast<char*>(&nameSize), sizeof(int));
		GameObjectManager::Get().currentSceneName.resize(nameSize);

		file.read(reinterpret_cast<char*>(&GameObjectManager::Get().currentSceneName[0]), nameSize); //https://stackoverflow.com/a/37035925

		GameObjectManager::Get().Deserialization(GameObjectManager::Get().currentSceneName);
	}
	file.close();
}