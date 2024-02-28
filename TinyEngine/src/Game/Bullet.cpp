#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(Vector position, Vector direction)
	: Actor(position, Vector(4.f), COLOR_YELLOW), direction(direction)
{
	spawnTime = engCurrentTime();
}

void Bullet::Update()
{
	position += direction * SPEED * engDeltaTime();

	//check for collisions
	Actor* hitActor = game->GetCollidingActor(this, CollisionChannel::Enemy);
	if (hitActor != nullptr) { //flag actors for destruction.
		hitActor->Destroy();
		Destroy();
	}

	if (engTimePassedSince(spawnTime) > BULLET_LIFETIME) {
		Destroy();
	}
}
