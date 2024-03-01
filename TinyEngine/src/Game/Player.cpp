#include "Player.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"

Player::Player(Vector position)
	: Actor(position, Vector(32.f), COLOR_WHITE) //call parent constructor and apply own values. I dont know bro.
{
	collisionChannel = CollisionChannel::Player;
}

void Player::Update() {
	Vector input;
	if (engKeyDown(Key::W)) {
		input.y -= 1.f;
	}
	if (engKeyDown(Key::S)) {
		input.y += 1.f;
	}
	if (engKeyDown(Key::D)) {
		input.x += 1.f;
	}
	if (engKeyDown(Key::A)) {
		input.x -= 1.f;
	}

	input.Normalize();
	position += input * speed * engDeltaTime(); //this was made possible using the operator overload defined in the Vector.h struct

	if (engKeyPressed(Key::E)) {
		speed += 50.f;
	}
	if (engKeyPressed(Key::Q)) {
		speed -= 50.f;
	}

	if (engMouseButtonPressed(Mouse_Button::Left)) {
		// Spawn a bullet
		Vector mousePosition = Vector(engMouseX(), engMouseY());
		mousePosition = game->GetCamera().screenToWorld(mousePosition);
		Vector bulletDirection = mousePosition - position;
		bulletDirection.Normalize();
		game->SpawnActor(new Bullet(position, bulletDirection));
	}

	if (health < MAXHEALTH) {
		Actor* pickupActor = game->GetCollidingActor(this, CollisionChannel::PickUp);
		if (pickupActor) {
			pickupActor->Destroy();
			health++;
		}
	}
}

void Player::Draw() {
	Vector mousePosition = Vector(engMouseX(), engMouseY());
	mousePosition = game->GetCamera().screenToWorld(mousePosition);
	Vector aimDirection = mousePosition - position;
	aimDirection.Normalize();

	Vector crossHairStart = game->GetCamera().worldToScreen(position + aimDirection * 45.f);
	Vector crossHairEnd = game->GetCamera().worldToScreen(position + aimDirection * 5000.f);

	engSetDrawColor(COLOR_RED);
	engDrawLine(crossHairStart.x, crossHairStart.y, crossHairEnd.x, crossHairEnd.y);

	//Draw health ui;
	for (int i = 0; i < MAXHEALTH; i++) {
		if (i < health) {
			engSetDrawColor(COLOR_WHITE);
		}
		else {
			engSetDrawColor(0xFFFFFF55); //white with alpha value at 55.
		}
		engFillRect(10 + 40 * i, 10, 32, 32);
	}

	//Flash player when invincible
	if (isInvincible()) {
		if (int(engCurrentTime() * 15) % 2) { // % is called modulo, don't know what it means.
			color = 0xFFFFFF77;
		}
		else {
			color = COLOR_WHITE;
		}
	}
	else {
		color = COLOR_WHITE;
	}

	Actor::Draw(); //call original function after performing override stuff above.
}
