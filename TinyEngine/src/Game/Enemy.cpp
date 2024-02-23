#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(Vector position)
	:Actor(position, Vector(20.f, 20.f), COLOR_RED) {

}

void Enemy::Update() {
	Vector direction = game.GetPlayer()->position - position; //position - position is possible because I defined an inline operator overload in the Vector.h file.
	Vector direction2 = direction;
	direction.Normalize();

	position += direction * speed * engDeltaTime();
}