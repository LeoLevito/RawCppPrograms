#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(Vector position)
	: Actor(position, Vector(4.f), COLOR_YELLOW)
{
	spawn_time = engCurrentTime();
}

void Bullet::update()
{
	position += direction * SPEED * engDeltaTime();

	// Check for collisions !!!
	Actor* hit_actor = game->get_colliding_actor(this, Collision_Channel::Enemy);
	if (hit_actor != nullptr)
	{
		hit_actor->hit(DAMAGE);
		destroy();
	}

	if (engTimePassedSince(spawn_time) > LIFETIME)
		destroy();
}
