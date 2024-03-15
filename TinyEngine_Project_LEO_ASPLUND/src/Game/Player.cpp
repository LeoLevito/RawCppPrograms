#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Player::Player(Vector position)
	: Actor(position, Vector(32), COLOR_WHITE)
{
	collision_channel = Collision_Channel::Player;
}

void Player::shoot() //Add: Shoot function. Moved shooting code here.
{
	if (ammo > 0) //Add: ammo check, can't shoot when you have no ammo.
	{ 
		Vector mouse_position = Vector(engMouseX(), engMouseY());
		mouse_position = game->get_camera().screen_to_world(mouse_position);

		Vector bullet_direction = mouse_position - position;
		bullet_direction.normalize();

		auto* bullet = game->spawn_actor<Bullet>(position);
		bullet->set_direction(bullet_direction);

		ammo--; //decrease ammo when shooting.
	}
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

	if (engMouseButtonPressed(Mouse_Button::Left)) //kept this to allow tap-shooting.
	{
		shoot(); //change: moved code to new function shoot(). 
	}

	if (engMouseButtonDown(Mouse_Button::Left)) //Add: engMouseButtonDown. Hold for automatic fire.
	{
		timeSinceLastBullet += engDeltaTime(); //update timeSinceLastBullet

		if (timeSinceLastBullet > shootDelay) //shoot when this condition is met.
		{ 
			shoot();
			timeSinceLastBullet = 0; //reset timeSinceLastBullet after shooting, allows for automatic fire since this if-statement will be checked again after updating timeSinceLastBullet.
		}
	}

	if (engMouseButtonReleased(Mouse_Button::Left)) //Add: engMouseButtonReleased. 
	{
		timeSinceLastBullet = 0; //reset timeSinceLastBullet, so delay between shots when starting automatic fire again is consistent.
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

	if (ammo < MAX_AMMO) { //Copy-Add: logic for replenishing ammo. Similar to health pickup logic above.
		Actor* ammoPickUpActor = game->get_colliding_actor(this, Collision_Channel::PickUpAmmo); //change from health pick up logic: PickUpAmmo collision channel, ammoPickUpActor actor pointer.
		if (ammoPickUpActor)
		{
			ammoPickUpActor->destroy();
			ammo = MAX_AMMO; //when touching ammo pick up, replenish ammo to max amount.
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

	for (int i = 0; i < MAX_AMMO; ++i) //Copy-Add: draw ammo UI, just like health UI.
	{
		if (i < ammo)
			engSetDrawColor(COLOR_YELLOW); //change from health UI: Yellow instead of white.
		else
			engSetDrawColor(0xFFFFFF55);

		engFillRect(130 + 20 * i, 10, 15, 15); //change from health UI: ammo ui is offset by 130 instead of 10 on x-axis, distance between bullets is 20 * i, and size of bullets is 15x15.
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
