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
		Vector bulletDirection = Vector(engMouseX(), engMouseY()) - position;
		bulletDirection.Normalize();
		game.SpawnActor(new Bullet(position, bulletDirection));
	}
}

void Player::Draw() {

	Vector aimDirection = Vector(engMouseX(), engMouseY()) - position;
	aimDirection.Normalize();

	Vector crossHairStart = position + aimDirection * 45.f;
	Vector crossHairEnd = position + aimDirection * 5000.f;

	engSetDrawColor(COLOR_RED);
	engDrawLine(crossHairStart.x, crossHairStart.y, crossHairEnd.x, crossHairEnd.y);

	Actor::Draw(); //call original function after performing override stuff above.
}
