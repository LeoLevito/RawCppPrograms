#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(Vector position)
	:Actor(position, Vector(20.f, 20.f), COLOR_RED)
{
	collisionChannel = CollisionChannel::Enemy;
}

void Enemy::Update() {
	if (game->GetPlayer() != nullptr) {
		Vector direction = game->GetPlayer()->position - position; //position - position is possible because I defined an inline operator overload in the Vector.h file.
		Vector direction2 = direction;
		direction.Normalize();

		position += direction * speed * engDeltaTime();

		Actor* player = game->GetCollidingActor(this, CollisionChannel::Player);
		if (player) {
			player->Destroy();
			//engClose();
		}
	}
}