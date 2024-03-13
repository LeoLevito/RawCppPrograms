#include "Actor.h"
#include "Game/Game.h"

void Actor::draw()
{
	Vector draw_position = position - size * 0.5f;
	draw_position = game->get_camera().world_to_screen(draw_position);

	engSetDrawColor(color);
	engFillRect(
		(int)draw_position.x,
		(int)draw_position.y,
		(int)size.x,
		(int)size.y
	);
}
