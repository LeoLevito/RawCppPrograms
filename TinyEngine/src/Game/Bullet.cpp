#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(Vector position, Vector direction)
	: Actor(position, Vector(4.f), COLOR_YELLOW), direction(direction)
{
}

void Bullet::Update()
{
	position += direction * 1000.f * engDeltaTime();

	//check for collisions
	Actor* hitActor = game.GetCollidingActor(this, CollisionChannel::Enemy);
	if (hitActor != nullptr) { //flag actors for destruction.
		hitActor->Destroy();
		Destroy();
	}
}
