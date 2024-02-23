#include "Player.h"
#include "PlayerInput.h"

Player::Player(Vector position) 
	: Actor(position, Vector(32.f), COLOR_WHITE) //call parent constructor and apply own values. I dont know bro.
{
}

void Player::Update(){
	UpdateInput(position, speed);
}