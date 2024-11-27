#include <Graphics.h>
#include <Engine.h>
#include <EditorGUI.h>
#include <Game.h>

int main()
{
	Game* game = new Game;
	Graphics* graphics = new Graphics;
	graphics->Initialize(1280, 720, game);

	Engine* engine = new Engine;
	engine->Initialize(graphics->window, graphics->myCamera);

	EditorGUI* editorGUI = new EditorGUI;
	editorGUI->Initialize(graphics->window, graphics, game, *graphics->myCamera, *graphics->myShader);

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	glfwSwapInterval(1); //VSYNC disable / set max framerate, think (1/50) * 50 for 1 fps at 50hz

	while (!graphics->ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;



		glfwPollEvents(); //moved from Graphics::Render().


		editorGUI->StartImGuiFrame(deltaTime);

		engine->Update(graphics->window,deltaTime);
		graphics->Render();
		//for (int i = 0; i < game->gameObjectVector.size(); i++)
		//{
		//	game->gameObjectVector[i]->Update();
		//}

		//could do so Game.gameObjectVector calls Update() on every game object implementing Update() and that Update() can call Update() in every component implementing Update().
		editorGUI->HierarchyWindow(*graphics->myCamera, graphics->projection, *graphics->myShader);

		editorGUI->RenderImGui(graphics->projection);

		glfwSwapBuffers(graphics->window); //moved from Graphics::Render().
	}

	editorGUI->CloseImGui();

	return 0;
}