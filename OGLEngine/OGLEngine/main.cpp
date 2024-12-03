#include <Graphics.h>
#include <Engine.h>
#include <EditorGUI.h>
#include <GameObjectManager.h>

int main()
{
	GameObjectManager* gameObjectManager = new GameObjectManager;
	Camera::Allocate();
	Graphics* graphics = new Graphics;
	graphics->Initialize(1280, 720, gameObjectManager);

	Engine* engine = new Engine;
	engine->Initialize(graphics->window, &Camera::Get());

	EditorGUI* editorGUI = new EditorGUI;
	editorGUI->Initialize(graphics->window, graphics, gameObjectManager, Camera::Get(), *graphics->myShader);

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	glfwSwapInterval(1); //set max framerate, think (1/50) * 50 for 1 fps at 50hz, this doesn't function like a VSYNC toggle it seems to my eyes.

	while (!graphics->ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents(); //moved from Graphics::Render().
		editorGUI->StartImGuiFrame(deltaTime);

		engine->Update(graphics->window,deltaTime);
		graphics->Render();
		editorGUI->HierarchyWindow(Camera::Get(), graphics->projection, *graphics->myShader);

		editorGUI->RenderImGui(graphics->projection);
		glfwSwapBuffers(graphics->window); //moved from Graphics::Render().
	}

	editorGUI->CloseImGui();
	return 0;
}