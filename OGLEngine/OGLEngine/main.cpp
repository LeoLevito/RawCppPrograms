#include <Graphics.h>
#include <Shader.h>
#include <Engine.h>

int main()
{
	Graphics graphics;
	graphics.Initialize(640, 480);

	Engine* engine = new Engine;
	engine->Initialize(graphics.window, graphics.myCamera);

	float lastTime = 0;
	float currentTime = 0;
	float deltaTime = 0;

	while (!graphics.ShouldClose())
	{
		currentTime = glfwGetTime(); //May or may not be super accurate at the moment.
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		graphics.Render();
		engine->Update(deltaTime);
	}
	return 0;
}