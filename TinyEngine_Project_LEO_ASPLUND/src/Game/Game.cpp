#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "Tank.h" //Add: Include Tank.h
#include "PickUp.h"
#include "Math/AABB.h"
#include "Math/Math.h"
#include <cmath>

Game* game = nullptr;

Game::Game()
{
	actors[0] = new Player(Vector(100.f, 100.f));
	player = actors[0];

	timers.add_timer(2.f, [this]() {
		if (get_player() == nullptr)
			return;

		if (Enemy::NUM_ENEMIES < 20)
		{
			spawn_actor<Enemy>(
				get_player()->position + Vector::random_point_on_circle(500.f)
			);
		}
	}, true);

	timers.add_timer(15.f, [this]() { //Copy-Add: Timer for spawning tank enemies, every 15 seconds.
		if (get_player() == nullptr)
			return;

		if (Enemy::NUM_ENEMIES < 20)
		{
			spawn_actor<Tank>( //change: Tank instead of Enemy
				get_player()->position + Vector::random_point_on_circle(500.f)
			);
		}
	}, true);

	timers.add_timer(10.f, [this]() {
		if (get_player() == nullptr)
			return;

		spawn_actor<PickUp>(
			get_player()->position + Vector::random_point_on_circle(200.f)
		);
	}, true);
}

void Game::update()
{
	timers.update();

	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] != nullptr)
		{
			actors[i]->update();
		}
	}

	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] == nullptr)
			continue;

		if (actors[i]->get_is_destroyed())
		{
			if (actors[i] == player)
			{
				player = nullptr;
			}

			delete actors[i];
			actors[i] = nullptr;
		}
	}

	camera.update();
}

void Game::render()
{
	// Draw background
	for (int x = -50; x <= 50; ++x)
	{
		for (int y = -50; y <= 50; ++y)
		{
			if ((x + y) % 2 == 0)
				engSetDrawColor(0x120D0FFF);
			else
				engSetDrawColor(0x21181BFF);

			Vector position = Vector(x * GRID_SIZE, y * GRID_SIZE);
			position = camera.world_to_screen(position);

			engFillRect(position.x, position.y, GRID_SIZE, GRID_SIZE);
		}
	}

	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] != nullptr)
		{
			actors[i]->draw();
		}
	}
}

Actor* Game::get_colliding_actor(Actor* other, Collision_Channel channel)
{
	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] == other)
			continue;

		if (actors[i] == nullptr)
			continue;

		if (actors[i]->collision_channel != channel)
			continue;

		AABB a = AABB::from_position_size(other->position, other->size);
		AABB b = AABB::from_position_size(actors[i]->position, actors[i]->size);

		if (aabb_overlap(a, b))
		{
			return actors[i];
		}
	}

	return nullptr;
}
