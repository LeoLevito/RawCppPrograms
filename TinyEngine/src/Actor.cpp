#include "Actor.h"
#include "Game/Game.h"

void Actor::Draw() { //defining a function that's inside this class
	
	Vector drawPosition = position - size * 0.5f;
	drawPosition = game->GetCamera().worldToScreen(drawPosition);

	engSetDrawColor(color);
	engFillRect(
		(int)(drawPosition.x), 
		(int)(drawPosition.y),
		(int)size.x, 
		(int)size.y
	); // - size.x / 2 means center of actor or player.
}