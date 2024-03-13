#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include <stdio.h> //include stdio for debugging

Player::Player(Vector position)
	: Actor(position, Vector(32), COLOR_WHITE)
{
	collision_channel = Collision_Channel::Player;
}

void Player::update()
{

	Vector input;

	if (engKeyDown(Key::W))
		input.y -= 1.f;
	if (engKeyDown(Key::S))
		input.y += 1.f;
	if (engKeyDown(Key::D))
		input.x += 1.f;
	if (engKeyDown(Key::A))
		input.x -= 1.f;

	input.normalize();
	position += input * speed * engDeltaTime();

	if (engKeyPressed(Key::E))
		speed += 50.f;
	if (engKeyPressed(Key::Q))
		speed -= 50.f;

	if (engMouseButtonPressed(Mouse_Button::Left)) //change: MouseButtonDown instead of Pressed.
	{
		Vector mouse_position = Vector(engMouseX(), engMouseY());
		mouse_position = game->get_camera().screen_to_world(mouse_position);
		Vector bullet_direction = mouse_position - position;
		bullet_direction.normalize();
		auto* bullet = game->spawn_actor<Bullet>(position);
		bullet->set_direction(bullet_direction);
	}

	if (engMouseButtonDown(Mouse_Button::Left)) {
		bulletTimer.add_timer(0.3f, [this]() {
			printf("start timer\n");
			}, true);
		bulletTimer.update();
	}


	if (health < MAX_HEALTH)
	{
		Actor* pickupActor = game->get_colliding_actor(this, Collision_Channel::PickUp);
		if (pickupActor)
		{
			pickupActor->destroy();
			health++;
		}
	}
}

void Player::draw()
{
	Vector mouse_position = Vector(engMouseX(), engMouseY());
	mouse_position = game->get_camera().screen_to_world(mouse_position);

	Vector aim_direction = mouse_position - position;
	aim_direction.normalize();

	Vector crosshair_start = game->get_camera().world_to_screen(position + aim_direction * 45.f);
	Vector crosshair_end = game->get_camera().world_to_screen(position + aim_direction * 5000.f);

	engSetDrawColor(COLOR_RED);
	engDrawLine(
		crosshair_start.x, crosshair_start.y,
		crosshair_end.x, crosshair_end.y
	);

	// Draw health UI 
	for (int i = 0; i < MAX_HEALTH; ++i)
	{
		if (i < health)
			engSetDrawColor(COLOR_WHITE);
		else
			engSetDrawColor(0xFFFFFF55);

		engFillRect(10 + 40 * i, 10, 32, 32);
	}

	// Flash player when invincible
	if (is_invincible())
	{
		if (int(engCurrentTime() * 15) % 2 == 0)
			color = 0xFFFFFF77;
		else
			color = COLOR_WHITE;
	}
	else
	{
		color = COLOR_WHITE;
	}

	Actor::draw();
}
