#include "Enemy.h"
#include "Game.h"

int Enemy::NUMENEMIES{ 0 };

Enemy::Enemy(Vector position)
	:Actor(position, Vector(20.f, 20.f), COLOR_RED)
{
	collisionChannel = CollisionChannel::Enemy;
	NUMENEMIES++;
}

Enemy::~Enemy() //destructor, antithesis of constructor. This is called when the 'delete' expression is called for heap allocated stuff. Shift-FN-Escape to write tilde on my tiny keyboard.
{
	NUMENEMIES--;
}

void Enemy::Update() {
	if (game->GetPlayer() != nullptr) {
		Vector direction = game->GetPlayer()->position - position; //position - position is possible because I defined an inline operator overload in the Vector.h file.
		Vector direction2 = direction;
		direction.Normalize();

		position += direction * speed * engDeltaTime();

		Actor* player = game->GetCollidingActor(this, CollisionChannel::Player);
		if (player) {
			player->Hit(1);
			//engClose();
		}
	}
}


void Enemy::Draw(){
	if (health < MAXHEALTH) {
		float healthPercentage = (float(health) / MAXHEALTH); //this used to be two integers divided, which returned an integer, which truncates the resulting number and thus didn't show up in the UI. Now we convert one of the integers to float, which fixes the problem. If we were to divide the entire division expression, the conversion to float would happen after the division and thus wouldn't work.

		Vector screenPosition = game->GetCamera().worldToScreen(position);
		engSetDrawColor(0x000000AA);
		engFillRect(screenPosition.x - 26.f, screenPosition.y - 32.f, 26.f * 2.f, 8.f);

		engSetDrawColor(COLOR_RED);
		engFillRect(screenPosition.x - 26.f, screenPosition.y - 30.f, 26.f * 2.f * healthPercentage, 8.f);
	}
	Actor::Draw();
}
