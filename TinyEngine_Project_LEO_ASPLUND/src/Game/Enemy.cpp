#include "Enemy.h"
#include "Game.h"
#include "Engine/TinyEngine.h"

int Enemy::NUM_ENEMIES = 0;

Enemy::Enemy(Vector position)
	: Actor(position, Vector(20.f, 20.f), COLOR_RED)
{
	collision_channel = Collision_Channel::Enemy;
	NUM_ENEMIES++;
}
Enemy::~Enemy()
{
	NUM_ENEMIES--;
}

void Enemy::update()
{
	if (game->get_player() != nullptr)
	{
		Vector direction = game->get_player()->position - position;
		direction.normalize();

		position += direction * speed * engDeltaTime();

		Actor* player = game->get_colliding_actor(this, Collision_Channel::Player);
		if (player)
		{
			player->hit(1);
		}
	}
}

void Enemy::draw()
{
	if (health < MAX_HEALTH)
	{
		float health_percentage = float(health) / MAX_HEALTH;
		Vector screen_position = game->get_camera().world_to_screen(position);

		engSetDrawColor(0x000000AA);
		engFillRect(screen_position.x - 26.f, screen_position.y - 30.f, 26.f * 2.f, 8.f);

		engSetDrawColor(COLOR_RED);
		engFillRect(
			screen_position.x - 26.f,
			screen_position.y - 30.f,
			26.f * 2.f * health_percentage,
			8.f
		);
	}

	Actor::draw();
}
