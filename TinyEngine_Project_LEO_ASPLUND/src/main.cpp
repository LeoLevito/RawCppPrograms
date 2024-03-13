#include "Engine/TinyEngine.h"
#include "Game/Game.h"
#include "Config.h"

int main()
{
	engInit("Hey", Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);

	game = new Game();

	while(engBeginFrame())
	{
		engSetDrawColor(COLOR_DARK_GRAY);
		engClearScreen();

		if (engKeyPressed(Key::Escape))
		{
			return 0;
		}

		game->update();
		game->render();
	}

	delete game;
}