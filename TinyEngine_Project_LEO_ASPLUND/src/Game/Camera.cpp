#include "Camera.h"
#include "Game.h"
#include "Actor.h"
#include "Config.h"

void Camera::update()
{
	if (game->get_player() != nullptr)
	{
		Vector player_position = game->get_player()->position;
		Vector mouse_position = screen_to_world(Vector(engMouseX(), engMouseY()));

		Vector target_position = Vector::lerp(player_position, mouse_position, 0.3f);

		position = Vector::lerp(position, target_position, CAMERA_SPEED * engDeltaTime());
	}
}

Vector Camera::world_to_screen(Vector vec)
{
	using namespace Config;
	return vec - position + Vector(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

Vector Camera::screen_to_world(Vector vec)
{
	using namespace Config;
	return vec + position - Vector(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}
