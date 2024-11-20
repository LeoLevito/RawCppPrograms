#include <Graphics.h>
#include <Engine.h>
#include <EditorGUI.h>

int main()
{
	Graphics* graphics = new Graphics;
	graphics->Initialize(1280, 720);

	Engine* engine = new Engine;
	engine->Initialize(graphics->window, graphics->myCamera);

	EditorGUI* editorGUI = new EditorGUI;
	editorGUI->Initialize(graphics->window);

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	//glfwSwapInterval(0); VSYNC disable

	while (!graphics->ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;



		glfwPollEvents(); //moved from Graphics::Render().

		editorGUI->StartImGuiFrame(deltaTime);

		engine->Update(graphics->window,deltaTime);
		graphics->Render();

		editorGUI->RenderImGui();

		glfwSwapBuffers(graphics->window); //moved from Graphics::Render().
	}

	editorGUI->CloseImGui();

	return 0;
}